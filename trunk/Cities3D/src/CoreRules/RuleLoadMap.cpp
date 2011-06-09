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
#include "GameEngine.h"
#include "Controller.h"
#include "Game.h"
#include "Map.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleLoadMap
// 
// Loads the given <Map> index from the <MapDatabase> into
// the <GameEngine>.
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
// Transmits To:
//     eventPreGame
//
class RuleLoadMap : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		//load the map into memory so that it is now the Map object in
		//the game engine
		GameEngine::get().LoadMap(object.read<wxInt32>());

		//the game must know what map it is using
		gameData<wxInt32>(shMapID) = MAP.read<wxInt32>(shID);

		//fire update event
		Controller::get().Transmit(shEventLoadMap, 0);
		Controller::get().Transmit(shEventPreGame, GetGame());
	}
};

IMPLEMENT_RULE(RuleLoadMap, CORE)
