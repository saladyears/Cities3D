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
#include "Logic.h"
#include "RuleSetDatabase.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: NetworkLogicUpdateMessageUI
// 
// Provides <RuleUpdateMessageUI> with the wrapper text for network games.  
// Wraps the text with "Waiting for <name> to" and a period.
//
// Derived From:
//     <Logic>
//
// Project:
//     <NetworkRules>
//
// RuleSet:
//     <Network>
//
// Mixin To:
//     <RuleUpdateMessageUI>
//
class NetworkLogicUpdateMessageUI : public Logic
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		DataObject thisInput(-1), thisOutput;
		RULE.Decide(shLogicIsThisPlayer, thisInput, thisOutput);

		// If it's not this player's turn, use the "Waiting for.." text.
		if(false == thisOutput.read<bool>())
		{
			//get the name of the current player
			wxString str = wxString::Format(input.read<wxString>(1).c_str(),
				playerGame().player().Name().c_str());

			output = DataObject(str);
		}
		// Otherwise, run the normal kind.
		else
		{
			// Use the given string as the format string and add the player's name.
			wxString str = wxString::Format(input.read<wxString>().c_str(),
				playerGame().player().Name().c_str());

			output = DataObject(str);
		}
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(NetworkLogicUpdateMessageUI, Network, 
							RuleUpdateMessageUI, NETWORK)
