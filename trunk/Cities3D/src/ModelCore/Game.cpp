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
#include "Game.h"
#include "HexObject.h"
#include "ChipObject.h"
#include "PortObject.h"
#include "IGameObjectEngine.h"
#include "define/defineHelper.h"
#include "define/defineStat.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
Game::Game()
{
	//settings that always exist
	data<wxInt32>(shID) = (wxInt32) time(NULL);
	data<wxInt32>(shMapID) = 0;
	data<wxInt32>(shWinner) = -1;

	data<IntArray>(shPorts).resize(0, 0);
	
	data<HashString>(shState) = shNew;
	data<HashString>(shMode) = shHotseat;
	
	data<wxDateTime>(shTimeStart) = wxDateTime::UNow();
}

void Game::buildBoard(std::vector<TileObject>::const_iterator itBegin, 
					  std::vector<TileObject>::const_iterator itEnd)
{
	std::vector<TileObject>::const_iterator it;

	mBoard.reserve(std::distance(itBegin, itEnd));
	for(it = itBegin; it != itEnd; ++it)
	{
		TileObjectPtr object(new TileObject(*it));
		object->randomizeRotation();

		//add it to the board, along with any chip it might have
		mBoard.push_back(object);
	}
}

void Game::getGameObjects(std::list<GameObjectPtr> &objects) const
{
	//add all game objects in this Game into the array
	for(wxInt32 i = 0; i < numTiles(); ++i)
	{
		//tiles
		objects.push_back(tile(i));

		//chips
		if(tile(i)->chip())
		{
			objects.push_back(tile(i)->chip());
		}

		//ports
		if(tile(i)->port())
		{
			objects.push_back(tile(i)->port());
		}
	}

	//hex objects
	HexObjectHash::const_iterator it, itEnd = mHexObjects.end();
	for(it = mHexObjects.begin(); it != itEnd; ++it)
	{
		objects.push_back(it->second);
	}
}

void Game::setHexObject(const HashString &key, const HexObjectPtr &object)
{
	mHexObjects[key] = object;
}

const HexObjectPtr &Game::getHexObject(const HashString &key)
{
	if(mHexObjects.end() == mHexObjects.find(key))
	{
		wxLogError(wxT("Programmer Error: No HexObject at key %s"), 
			key.cwx_str());
	}

	return mHexObjects[key];
}

void Game::getAllHexObjects(HexObjectArray &objects) const
{
	HexObjectHash::const_iterator it, itEnd = mHexObjects.end();

	for(it = mHexObjects.begin(); it != itEnd; ++it)
	{
		objects.push_back(it->second);
	}
}

void Game::load(wxDataInputStream &stream, const GameObjectEnginePtr &engine)
{
	//immediately load data fields
	Data::load(stream);

	wxUint32 i, temp;

	//player data
	stream >> temp;
	mPlayers.clear();
	mPlayers.resize(temp);
	for(i = 0; i < temp; i++)
	{
		mPlayers[i].load(stream, engine);
	}

	//only load up game object data if we have a game object engine
	//to create it with
	if(engine)
	{
		//board
		wxUint32 size;
		stream >> size;
		mBoard.clear();
		mBoard.resize(size);
		for(i = 0; i < size; i++)
		{
			mBoard[i] = TileObjectPtr(new TileObject);
			mBoard[i]->load(stream);
		}

		//hex objects
		stream >> size;
		for(wxUint32 i = 0; i < size; ++i)
		{
			//get the hash key
			HashString key;
			stream >> key;

			//get the class name for the hex object and then call
			//the game object engine to get an instance of that
			//object
			HashString name;
			stream >> name;

			HexObject *pObject = dynamic_cast<HexObject *>(
				engine->CreateGameObject(name));

			//it must be a HexObject type or it's no good and we're going
			//to totally screw over any further data read-in
			if(NULL == pObject)
			{
				wxLogError(
					wxT("Programmer Error: No game object with name %s ") 
					wxT("in the GameObjectEngine!"), name.cwx_str());
				return;
			}

			pObject->load(stream);

			//add it to the list
			mHexObjects[key] = HexObjectPtr(pObject);
		}
	}
}

void Game::save(wxDataOutputStream &stream) const
{
	//data fields
	Data::save(stream);

	//player data
	stream << (wxInt32) mPlayers.size();
	for(wxUint32 i = 0; i < mPlayers.size(); i++)
	{
		mPlayers[i].save(stream);
	}

	//board
	stream << (wxInt32) mBoard.size();
	for(wxUint32 i = 0; i < mBoard.size(); i++)
	{
		mBoard[i]->save(stream);
	}

	//hex objects
	stream << (wxUint32) mHexObjects.size();

	HexObjectHash::const_iterator it, itEnd = mHexObjects.end();
	for(it = mHexObjects.begin(); it != itEnd; ++it)
	{
		//write out the hash value first and the number of items in the list
		stream << it->first;
		
		//write out the class name of the object so we know how
		//to recreate it
		stream << it->second->name();

		//dump the object itself
		it->second->save(stream);
	}
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//


