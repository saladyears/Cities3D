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
#include "ChipObject.h"
#include "PortObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleResetTileFlags
// 
// Resets the <IGameObject> flags for all <TileObjects>,
// and any <PortObjects> or <ChipObjects> that lay on
// them in the <Game>.
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
class RuleResetTileFlags : public Rule
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

		wxInt32 tiles = numTiles();
		for(wxInt32 i = 0; i < tiles; ++i)
		{
			tile(i)->reset();

			//set desired flags
			FlagArray::const_iterator it, itEnd = flags.end();
			for(it = flags.begin(); it != itEnd; ++it)
			{
				tile(i)->set((*it));
			}
		}
	}
};

IMPLEMENT_RULE(RuleResetTileFlags, CORE)
