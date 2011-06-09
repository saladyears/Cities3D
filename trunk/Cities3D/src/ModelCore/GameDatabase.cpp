/*
 *  Cities3D - Copyright (C) 2001-2009 Jason Fugate (saladyears@gmail.com)
 * 
 *  This program is free software; you can redistribute it and/or modify it 
 *  under the terms of the GNU General Public License as published by the Free 
 *  Software Foundation; either version 2 of the License, or (at your option) 
 *  any later version.
 * 
 *  This program is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 *  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License 
 *  for more details.
 */
#include "stdwx.h"
#include "style.h"	//READ THIS BEFORE MAKING ANY CHANGES TO THIS FILE!!!

//---------------------------- SYSTEM INCLUDES  -----------------------------//

//---------------------------- USER INCLUDES    -----------------------------//
#include "GameDatabase.h"
#include "FilePath.h"
#include "Controller.h"
#include "define/defineFile.h"
#include "ISerialize.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
}

//---------------------------- PUBLIC           -----------------------------//
size_t GameDatabase::NumGames() const
{
	return mGames.size();
}

size_t GameDatabase::SaveGame(const Game &game, 
							  const StringPairArray &rulesets, 
							  const StringPairArray& options,
							  const ISerialize& stats, 
							  const bool reload)
{
	wxString file = FilePath::Data(DataFileGames);
	wxString keyfile = FilePath::Data(DataFileGameKeys);

	// Save the game.
	wxFile gamedb;
	gamedb.Open(file, wxFile::write_append);
	wxFileOutputStream x(gamedb);
	wxDataOutputStream games(x);

	// Move to the end.
	gamedb.SeekEnd();

	// Get the starting position
	wxUint32 pos = gamedb.Tell();

	// Write out the rulesets loaded.
	games << (wxUint32) rulesets.size();
	GameDatabase::StringPairArray::const_iterator it, itEnd = rulesets.end();
	for(it = rulesets.begin(); it != itEnd; ++it)
	{
		games << it->first;
		games << it->second;
	}

	// options data
	games << (wxUint32) options.size();

	itEnd = options.end();
	for(it = options.begin(); it != itEnd; ++it)
	{
		games << it->first;
		games << it->second;
	}

	// Write out the game.
	game.save(games);

	// Stats must always go at the end so we can ignore them while loading if
	// desired.
	stats.save(games);
		
	// Close the file.
	gamedb.Close();

	// Now update the key file.
	wxFile keydb;
	keydb.Open(keyfile, wxFile::write_append);
	wxFileOutputStream y(keydb);
	wxDataOutputStream keys(y);

	// Move to the end
	keydb.SeekEnd();

	// Write the offset
	keys << pos;

	wxUint32 keyLength;
	keyLength = keydb.Length();

	// Flush it.
	keydb.Close();

	if(true == reload)
	{
		InitializeDatabase();
	}

	// Calculate the number of games.
	size_t index = (keyLength / sizeof(size_t)) - 1;
	return index;
}

size_t GameDatabase::UpdateGame(const size_t index, 
								const StringPairArray &rulesets, 
								const StringPairArray& options, 
								const ISerialize& stats,
								const Game &game)
{
	// If this is the last game in the list, no need to reload the database.
	bool reload = (index != mGames.size() - 1);

	// Delete the game and then resave it.
	DeleteGame(index, false);

	return SaveGame(game, rulesets, options, stats, reload);
}

void GameDatabase::LoadGame(const size_t index, Game &game, 
							const GameObjectEnginePtr &engine, 
							ISerialize& stats)
{
	wxASSERT(mOffsets.size() > index);
	
	wxFileInputStream x(FilePath::Data(DataFileGames));
	wxDataInputStream games(x);

	// Seek to this position in the game file.
	x.SeekI(mOffsets[index]);

	// Read in the rulesets this game was using.
	wxUint32 size;
	games >> size;

	StringPairArray rulesets;
	rulesets.reserve(size);
	for(wxUint32 i = 0; i < size; ++i)
	{
		wxString module, ruleset;
		games >> module;
		games >> ruleset;

		rulesets.push_back(StringPair(module, ruleset));
	}

	games >> size;

	StringPairArray options;
	options.reserve(size);
	for(wxUint32 i = 0; i < size; ++i)
	{
		wxString module, ruleset;
		games >> module;
		games >> ruleset;

		options.push_back(StringPair(module, ruleset));
	}

	// Fire off the load event for any listeners to deal with it.
	if(engine)
	{
		Controller::get().Transmit(shEventLoadRulesets, rulesets);
		Controller::get().Transmit(shEventLoadOptions, options);

		// Immediately start up the game, too.
		Controller::get().Transmit(shEventStartGame, false);
	}

	game.load(games, engine);

	// Stats must always go at the end so we can ignore them while loading if
	// desired.
	stats.load(games);
}

void GameDatabase::DeleteGame(const size_t index, const bool reload)
{
	wxString file = FilePath::Data(DataFileGames);
	wxString keyfile = FilePath::Data(DataFileGameKeys);

	size_t size = NumGames();

	// If there's only one game in the file, just delete the files.
	if(1 == size)
	{
		wxRemoveFile(file);
		wxRemoveFile(keyfile);
	}
	else
	{
		// If this is not the last game in the file, we'll have to extricate 
		// it.
		if(index != (size - 1))
		{
			// Get two temp file names to use for the atomic deletion.
			wxString tempfile, tempkeyfile;
			tempfile = wxFileName::CreateTempFileName(swGame);
			tempkeyfile = wxFileName::CreateTempFileName(swKeys);

			wxFileInputStream db(file);
			wxFileOutputStream tempdb(tempfile);
			wxFileOutputStream tempkey(tempkeyfile);

			size_t totalLength = db.GetSize();
			size_t offset = 0;

			// Run through all the games in the db and save them to a temp file, 
			// except for the one being deleted, then copy that temp file to the 
			// desired  file.
			for(size_t i = 0; i < size; i++)
			{
				//calculate the size of this data chunk
				size_t chunk;

				if(i < (size - 1))
				{
					chunk = mOffsets[i + 1] - mOffsets[i];
				}
				else
				{
					chunk = totalLength - mOffsets[i];
				}

				// Just copy raw data instead of loading and adding.
				boost::scoped_array<wxUint8> bytes(new wxUint8[chunk]);
				db.Read(bytes.get(), chunk);

				// Skip the one we're deleting.
				if(i != index) 
				{
					tempdb.Write(bytes.get(), chunk);
					tempkey.Write(&offset, 4);

					offset += chunk;
				}
			}
			// Copy the temp files onto the real files.
			wxCopyFile(tempfile, file);
			wxCopyFile(tempkeyfile, keyfile);
		}
		// Otherwise, we can just truncate the file.
		else
		{
			wxFile in(file, wxFile::read);
			size_t chunk = mOffsets[index];

			boost::scoped_array<wxUint8> bytes(new wxUint8[chunk]);
			in.Read(bytes.get(), chunk);
			in.Close();

			// Now just write them back out.
			wxFile out(file, wxFile::write);
			out.Write(bytes.get(), chunk);
			out.Close();

			// Repeat with the key file.
			in.Open(keyfile, wxFile::read);
			chunk = in.Length() - sizeof(size_t);
			bytes.reset(new wxUint8[chunk]);
			in.Read(bytes.get(), chunk);
			in.Close();

			out.Open(keyfile, wxFile::write);
			out.Write(bytes.get(), chunk);
			out.Close();
		}
	}

	if(true == reload)
	{
		InitializeDatabase();
	}
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
GameDatabase::GameDatabase()
{
	InitializeDatabase();
}

void GameDatabase::InitializeDatabase()
{
	// Reset the array of game IDs
	mGames.clear();
	mOffsets.clear();
	
	// Make sure the database exists.
	wxString path = FilePath::Data(DataFileGameKeys);
	if(FALSE == wxFileExists(path))
	{
		return;
	}

	// Read in keys.
	wxFile keydb;
	keydb.Open(path);
	wxFileInputStream y(keydb);
	wxDataInputStream keys(y);

	//sanity
	if(false == keydb.IsOpened())
	{
		return;
	}

	// Calculate how many games there are.
	size_t size = keydb.Length() / sizeof(size_t);

	mOffsets.resize(size);

	// Read them in.
	for(size_t i = 0; i < size; i++)
	{
		keys >> mOffsets[i];
	}

	struct FakeSerialize : public ISerialize
	{
		void load(wxDataInputStream &stream) {}
		void save(wxDataOutputStream &stream) const {}
	};

	FakeSerialize serialize;

	// Now actually read all the game IDs.
	for(size_t i = 0; i < size; i++)
	{
		Game game;

		LoadGame(i, game, GameObjectEnginePtr(), serialize);
		
		mGames.push_back(game.read<wxInt32>(shID));
	}
}

