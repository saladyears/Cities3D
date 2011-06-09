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
#include "PlayerDatabase.h"
#include "FilePath.h"
#include "define/defineFile.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	//current array of players
	typedef std::vector<Player> PlayerArray;
	PlayerArray sPlayers;

	void SavePlayers()
	{
		wxString file;
		file = FilePath::Data(DataFilePlayers);

		//clear any existing file
		::wxRemoveFile(file);

		wxFile playerdb;
		playerdb.Open(file, wxFile::write);
		wxFileOutputStream x(playerdb);
		wxDataOutputStream out(x);

		//write the size
		out << (wxUint32) sPlayers.size();

		//serialize the objects
		for(wxUint32 i = 0; i < sPlayers.size(); i++)
		{
			out << sPlayers[i];
		}

		playerdb.Close();
	}
}

//---------------------------- PUBLIC           -----------------------------//
size_t PlayerDatabase::NumPlayers() 
{
	return sPlayers.size();
}

//get a specific player
const Player &PlayerDatabase::GetPlayer(const size_t index) const
{
	wxASSERT(sPlayers.size() > index);

	return sPlayers[index];
}

//player management
wxUint32 PlayerDatabase::AddPlayer(const Player &player)
{
	//add this player to the array
	sPlayers.push_back(player);

	//save the database and reload
	SavePlayers();
	InitializeDatabase();

	//return added index
	return (sPlayers.size() - 1);
}

void PlayerDatabase::UpdatePlayer(const size_t index, const Player &player)
{
	wxASSERT(sPlayers.size() > index);

	sPlayers[index] = player;
}

void PlayerDatabase::DeletePlayer(const size_t index)
{
	wxASSERT(sPlayers.size() > index);

	sPlayers.erase(sPlayers.begin() + index);

	//save the file
	SavePlayers();
	InitializeDatabase();
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
PlayerDatabase::PlayerDatabase()
{
	InitializeDatabase();
}

void PlayerDatabase::InitializeDatabase(void)
{	
	//clear all
	sPlayers.clear();

	wxString file;
	file = FilePath::Data(DataFilePlayers);

	if(false == ::wxFileExists(file))
	{
		return;
	}

	wxFile playerdb;
	playerdb.Open(file);

	if(false == playerdb.IsOpened())
	{
		return;
	}

	wxFileInputStream x(playerdb);
	wxDataInputStream in(x);

	//read the size
	wxUint32 size;
	in >> size;

	//resize the array
	sPlayers.resize(size);

	//read the players
	for(wxUint32 i = 0; i < size; i++)
	{
		in >> sPlayers[i];
	}

	playerdb.Close();
}


