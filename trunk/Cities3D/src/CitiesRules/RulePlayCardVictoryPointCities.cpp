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
#include "Controller.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RulePlayCardVictoryPointCities
// 
// Handles one player getting a victory point progress card.
//
// Derived From:
//     <Rule>
//
// Project:
//     <CitiesRules>
//
// RuleSet:
//     <Cities>
//
class RulePlayCardVictoryPointCities : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(3 <= object.numItems());

		wxInt32 thisPlayer = object.read<wxInt32>(1);
		const wxString& name = object.read<wxString>(2);
			
		// Play the sound.
		RULE.Execute(shRulePlaySound, DataObject(SOUND_PLAY_VICTORY_CARD));

		wxString str = wxString::Format(
			stReceivesOneVictoryPoint.c_str(), 
			swStringFormat.c_str(), name.c_str());
		RULE.Execute(shRuleUpdateNetworkUI, 
			DataObject(str, thisPlayer));

		// Record that they've played it.
		playerGameData<wxInt32>(shVictoryPoints, thisPlayer)++;

		RULE.Execute(shRuleAdjustPoints, DataObject(thisPlayer, 
			1));

		// Update UI.
		Controller::get().Transmit(shEventPlayerUI, 
			DataObject(GetGame(), thisPlayer));
	}
};

IMPLEMENT_RULE(RulePlayCardVictoryPointCities, CITIES);
