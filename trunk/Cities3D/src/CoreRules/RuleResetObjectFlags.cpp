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
#include "Rule.h"
#include "RuleSetDatabase.h"
#include "HexObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleResetObjectFlags
// 
// Resets the <IGameObject> flags for any <IGameObjects> in the game belonging
// to the players.
//
// Derived From:
//     <Rule>
//
// Project:
//     <CoreRules>
//
// RuleSet:
//     <Core>
//
class RuleResetObjectFlags : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		typedef std::vector<IGameObject::ObjectFlag> FlagArray;
		FlagArray flags;

		//read desired flags out of the DataObject
		for(size_t i = 0; i < object.numItems(); ++i)
		{
			flags.push_back(object.read<IGameObject::ObjectFlag>(i));
		}

		// Gather all of the game objects.
		typedef std::list<GameObjectPtr> GameObjectList;
		GameObjectList objects;
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			playerGame(i).getGameObjects(objects);
		}

		// Reset them all.
		GameObjectList::const_iterator it, itEnd = objects.end();
		for(it = objects.begin(); it != itEnd; ++it)
		{
			// Keep the hidden flag.
			bool hidden = (*it)->isset(IGameObject::Hidden);

			(*it)->reset();

			for(size_t i = 0; i < flags.size(); ++i)
			{
				(*it)->set(flags[i]);
			}

			if(true == hidden)
			{
				(*it)->set(IGameObject::Hidden);
			}
		}

		// Repeat for hex objects, but they are special in that they cannot
		// be dimmed.
		Game::HexObjectArray hexes;
		GetGame()->getAllHexObjects(hexes);

		{
			Game::HexObjectArray::const_iterator it, itEnd = hexes.end();
			for(it = hexes.begin(); it != itEnd; ++it)
			{
				// Keep the hidden flag.
				bool hidden = (*it)->isset(IGameObject::Hidden);

				(*it)->reset();

				for(size_t i = 0; i < flags.size(); ++i)
				{
					(*it)->set(flags[i] & ~IGameObject::Dim);
				}

				if(true == hidden)
				{
					(*it)->set(IGameObject::Hidden);
				}
			}
		}
	}
};

IMPLEMENT_RULE(RuleResetObjectFlags, CORE)
