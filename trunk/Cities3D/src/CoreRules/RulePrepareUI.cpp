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
#include "RuleExtensionMixin.h"
#include "RuleSetDatabase.h"
#include "GameEngine.h"
#include "Controller.h"
#include "AggregatorObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RulePrepareUI
// 
// Gets all <IGameObjects> from the loaded <Game> and <PlayerGame> data
// and sends it up to the View so that the board will be properly
// displayed.  Transmits the eventInitializeUI <Event> so the
// View may be properly initialized.  
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
//     eventGameObjects,
//     eventInitializeUI
//
class RulePrepareUI 
	: public Rule
	, public RuleExtensionMixin
{
public:
	virtual void Execute(const DataObject &)
	{
		AggregatorObjectPtr aggregate(new AggregatorObject);

		std::vector<GameObjectPtr> objects;

		AggregatorObject::GameObjectList &list = aggregate->list();

		//game data
		GetGame()->getGameObjects(list);

		//player data
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			playerGame(i).getGameObjects(list);
		}

		//notify the display engine
		Controller::get().Transmit(shEventGameObjects, aggregate);

		//the board is generated, now get the UI ready
		Controller::get().Transmit(shEventInitializeUI, GetGame());

		ExecuteExtensions(DataObject());
	}
};

IMPLEMENT_RULE(RulePrepareUI, CORE)
