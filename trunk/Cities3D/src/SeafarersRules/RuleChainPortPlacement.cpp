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
#include "PlayerData.h"
#include "RuleSetDatabase.h"
#include "Logic.h"
#include "LogicBranchDefault.h"
#include "GameEngine.h"
#include "Controller.h"
#include "Map.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------------------------------------------------------//
// Class: LogicBranchPortPlacement
// 
// If there are still more ports left to place, increments
// the current player and branches to <RuleRequestPortPlacement>.
// Otherwise, branches to <RuleSetPortPlacements>.
//
// Derived From:
//     <Logic>
//
// Project:
//     <SeafarersRules>
//
// RuleSet:
//     <PortPlacement>
//
// Rules Called:
//     <RuleIncrementPlayer>
//
// Branches To:
//     <RuleRequestPortPlacement>
//     <RuleSetPortPlacements>
//
// Transmits To:
//     <eventPlayerUI>
//
class LogicBranchPortPlacement : public Logic
{
	virtual void Decide(const DataObject &, DataObject &output)
	{
		HashString str;

		//sum up the number of random ports there are
		const Data::IntHash &portHash = MAP.read<Data::IntHash>(
			shRandomPorts);

		wxInt32 ports = 0;
		Data::IntHash::const_iterator it, itEnd = portHash.end();
		for(it = portHash.begin(); it != itEnd; ++it)
		{
			ports += it->second;
		}
		
		//see if we're done placing ports
		if(ports <= gameData<wxInt32>(shInitialPortCount))
		{
			str = shRuleSetPortPlacements;
		}
		else
		{
			str = shRuleRequestPortPlacement;

			//increment the current player now, too
			RULE.Execute(shRuleIncrementPlayer, DataObject());

			Controller::get().Transmit(shEventPlayerUI, 
				DataObject(GetGame(), -2));
		}

		output = DataObject(str);
	}
};

//---------------------------- BRANCH LOGIC     -----------------------------//
//the port placement rule chain
IMPLEMENT_PRIORITY_DEFAULT_BRANCH(PORT_PLACEMENT, RuleMarkerStartInitial, 
    RuleStartPortPlacement, 100);
IMPLEMENT_DEFAULT_BRANCH(PORT_PLACEMENT, RuleStartPortPlacement, 
	RuleRequestPortPlacement);
IMPLEMENT_BRANCH(PORT_PLACEMENT, RulePlacePortPlacement, 
	LogicBranchPortPlacement);
IMPLEMENT_DEFAULT_BRANCH(PORT_PLACEMENT, RuleSetPortPlacements, 
	RuleMarkerStartInitial);
