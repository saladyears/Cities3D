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
#include "PlayerGame.h"
#include "CornerObject.h"
#include "SideObject.h"
#include "IGameObjectEngine.h"
#include "define/defineHelper.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
PlayerGame::PlayerGame(const Player &player, const ColorType color)
: mPlayer(player)
, mColor(color)
{
	mOriginalPlayers[mPlayer] = 0;

	//initialize player game data
	data<wxInt32>(shPoints) = 0;
	data<wxInt32>(shVictoryPoints) = 0;
}

void PlayerGame::originalPlayers(TurnPlayerMap &originals) const
{
	originals.clear();

	PlayerTurnMap::const_iterator it, itEnd = mOriginalPlayers.end();
	for(it = mOriginalPlayers.begin(); it != itEnd; ++it)
	{
		originals[it->second] = it->first;
	}
}

void PlayerGame::newPlayer(const Player &player)
{
	mPlayer = player;

	if( (NullPlayer != player) &&
		(mOriginalPlayers.end() == mOriginalPlayers.find(player))
		)
	{
		mOriginalPlayers[player] = 0;
	}
}

void PlayerGame::incrementTurn()
{
	if(NullPlayer != mPlayer)
	{
		wxASSERT(mOriginalPlayers.end() != mOriginalPlayers.find(mPlayer));

		mOriginalPlayers[mPlayer]++;
	}
}

void PlayerGame::getGameObjects(std::list<GameObjectPtr> &objects) const
{
	//corner objects
	CornerObjectHash::const_iterator it, itEnd = mCornerObjects.end();
	for(it = mCornerObjects.begin(); it != itEnd; ++it)
	{
		CornerObjectArray::const_iterator it2, it2End = it->second.end();
		for(it2 = it->second.begin(); it2 != it2End; ++it2)
		{
			objects.push_back((*it2));
		}
	}

	//side objects
	SideObjectHash::const_iterator it3, it3End = mSideObjects.end();
	for(it3 = mSideObjects.begin(); it3 != it3End; ++it3)
	{
		SideObjectArray::const_iterator it4, it4End = it3->second.end();
		for(it4 = it3->second.begin(); it4 != it4End; ++it4)
		{
			objects.push_back((*it4));
		}
	}
}

void PlayerGame::getAllCornerObjects(CornerObjectArray &objects) const
{
	CornerObjectHash::const_iterator it, itEnd = mCornerObjects.end();

	for(it = mCornerObjects.begin(); it != itEnd; ++it)
	{
		std::copy(it->second.begin(), it->second.end(), 
			std::back_inserter(objects));
	}
}

void PlayerGame::addCornerObject(const HashString &key, 
								 const CornerObjectPtr &object)
{
	CornerObjectArray& objects = mCornerObjects[key];
	objects.push_back(object);
}

void PlayerGame::removeCornerObject(const HashString &key, 
									const CornerObjectPtr &object)
{
	CornerObjectArray &array = mCornerObjects[key];

	CornerObjectArray::iterator it, itEnd = array.end();
	for(it = array.begin(); it != itEnd; ++it)
	{
		//this confusing line is the result of having to compare 
		//deferenced boost::smart_ptrs where one of them is inside
		//a std::list iterator
		if(*((*it).get()) == *(object.get()))
		{
			array.erase(it);
			return;
		}
	}
}

size_t PlayerGame::countCornerObjects() const
{
	size_t count = 0;
	CornerObjectHash::const_iterator it, itEnd = mCornerObjects.end();
	for(it = mCornerObjects.begin(); it != itEnd; ++it)
	{
		count += it->second.size();
	}

	return count;
}

void PlayerGame::addSideObject(const HashString &key, 
							   const SideObjectPtr &object)
{
	mSideObjects[key].push_back(object);
}

void PlayerGame::removeSideObject(const HashString &key, 
								  const SideObjectPtr &object)
{
	SideObjectArray &array = mSideObjects[key];

	SideObjectArray::iterator it, itEnd = array.end();
	for(it = array.begin(); it != itEnd; ++it)
	{
		//this confusing line is the result of having to compare 
		//deferenced boost::smart_ptrs where one of them is inside
		//a std::list iterator
		if(*((*it).get()) == *(object.get()))
		{
			array.erase(it);
			return;
		}
	}
}

const PlayerGame::CornerObjectArray &PlayerGame::getCornerObjects(
	const HashString &key)
{
	return mCornerObjects[key];
}

const PlayerGame::SideObjectArray &PlayerGame::getSideObjects(
	const HashString &key)
{
	return mSideObjects[key];
}

void PlayerGame::getAllSideObjects(SideObjectArray &objects) const
{
	SideObjectHash::const_iterator it, itEnd = mSideObjects.end();

	for(it = mSideObjects.begin(); it != itEnd; ++it)
	{
		std::copy(it->second.begin(), it->second.end(), 
			std::back_inserter(objects));
	}
}

void PlayerGame::load(wxDataInputStream &stream, 
					  const GameObjectEnginePtr &engine)
{
	Data::load(stream);

	//player
	stream >> mPlayer;
	
	// Original players.
	mOriginalPlayers.clear();
	wxUint32 size;
	stream >> size;
	for(wxUint32 i = 0; i < size; ++i)
	{
		Player player;
		stream >> player;
		wxUint32 turns;
		stream >> turns;

		mOriginalPlayers[player] = turns;
	}

	//color
	wxInt32 temp;
	stream >> temp;
	mColor = static_cast<ColorType>(temp);

	//read the size of the game objects
	wxUint32 objectsSize;
	stream >> objectsSize;

	mCornerObjects.clear();
	mSideObjects.clear();

	if(engine)
	{
		//corner objects
		wxUint32 size;
		stream >> size;
		for(wxUint32 i = 0; i < size; ++i)
		{
			//get the hash key
			HashString key;
			stream >> key;

			//create the location in the hash
			mCornerObjects[key] = CornerObjectArray();

			wxUint32 listSize;
			stream >> listSize;

			//read in the hex objects for this list
			for(wxUint32 j = 0; j < listSize; ++j)
			{
				//get the class name for the hex object and then call
				//the game object engine to get an instance of that
				//object
				HashString name;
				stream >> name;

				CornerObject *pObject = dynamic_cast<CornerObject *>(
					engine->CreateGameObject(name));

				//it must be a CornerObject type or it's no good and we're 
				//going to totally screw over any further data read-in
				if(NULL == pObject)
				{
					wxLogError(
						wxT("Programmer Error: No game object with name %s ")
						wxT("in the GameObjectEngine!"), name.cwx_str());
					return;
				}

				pObject->load(stream);

				//add it to the list
				mCornerObjects[key].push_back(CornerObjectPtr(pObject));
			}
		}

		//side objects
		stream >> size;
		for(size_t i = 0; i < size; ++i)
		{
			//get the hash key
			HashString key;
			stream >> key;

			//create the location in the hash
			mSideObjects[key] = SideObjectArray();

			wxUint32 listSize;
			stream >> listSize;

			//read in the hex objects for this list
			for(wxUint32 j = 0; j < listSize; ++j)
			{
				//get the class name for the hex object and then call
				//the game object engine to get an instance of that
				//object
				HashString name;
				stream >> name;

				SideObject *pObject = dynamic_cast<SideObject *>(
					engine->CreateGameObject(name));

				//it must be a SideObject type or it's no good and we're going
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
				mSideObjects[key].push_back(SideObjectPtr(pObject));
			}
		}
	}
	else
	{
		size_t octs = objectsSize / 8;
		size_t bytes = objectsSize % 8;

		//speed this up by reading in 64-bit chunks as long as we can
		for(size_t i = 0; i < octs; ++i)
		{
			stream.Read64();
		}

		for(size_t i = 0; i < bytes; ++i)
		{
			stream.Read8();
		}
	}
}

void PlayerGame::save(wxDataOutputStream &stream) const
{
	Data::save(stream);

	//player
	stream << mPlayer;
	
	// Original players.
	stream << (wxUint32) mOriginalPlayers.size();
	PlayerTurnMap::const_iterator it, itEnd = mOriginalPlayers.end();
	for(it = mOriginalPlayers.begin(); it != itEnd; ++it)
	{
		stream << it->first;
		stream << (wxUint32) it->second;
	}

	//color
	stream << (wxInt32) mColor;

	//because wxWidgets, for some idiotic reason, doesn't allow access
	//to the underlying wxOutputStream in the wxDataOutputStream, we have
	//to do some crazy stuff to figure out exactly how large the game objects
	//section of code is
	wxCountingOutputStream counter;
	wxDataOutputStream fake(counter);

	//write the data to the fake counter stream
	WriteGameObjects(fake);

	//now write out the size of the objects
	stream << (wxUint32) counter.GetSize();

	//now actually write out the objects
	WriteGameObjects(stream);	
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void PlayerGame::WriteGameObjects(wxDataOutputStream &stream) const
{
	//corner objects
	stream << (wxUint32) mCornerObjects.size();

	CornerObjectHash::const_iterator it, itEnd = mCornerObjects.end();
	for(it = mCornerObjects.begin(); it != itEnd; ++it)
	{
		//write out the hash value first and the number of items in the list
		stream << it->first;
		stream << (wxUint32) it->second.size();

		//now dump all the objects in the hash list
		CornerObjectArray::const_iterator it2, it2End = it->second.end();
		for(it2 = it->second.begin(); it2 != it2End; ++it2)
		{
			//write out the class name of the object so we know how
			//to recreate it
			stream << (*it2)->name();

			//dump the object itself
			(*it2)->save(stream);
		}
	}

	//side objects
	stream << (wxUint32) mSideObjects.size();

	SideObjectHash::const_iterator it3, it3End = mSideObjects.end();
	for(it3 = mSideObjects.begin(); it3 != it3End; ++it3)
	{
		//write out the hash value first and the number of items in the list
		stream << it3->first;
		stream << (wxUint32) it3->second.size();

		//now dump all the objects in the hash list
		SideObjectArray::const_iterator it4, it4End = it3->second.end();
		for(it4 = it3->second.begin(); it4 != it4End; ++it4)
		{
			//write out the class name of the object so we know how
			//to recreate it
			stream << (*it4)->name();

			//dump the object itself
			(*it4)->save(stream);
		}
	}
}


