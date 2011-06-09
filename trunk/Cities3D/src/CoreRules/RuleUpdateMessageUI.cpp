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
#include "LogicHashMixin.h"
#include "Controller.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleUpdateMessageUI
// 
// Updates the <MessageUI> with the given text.  Looks in its <ILogic>
// hash for an <ILogic> that can handle the given <Game> Mode.  The hashed
// <ILogic> should provide the wrapper text appropriate for that game mode.
// For instance, in hotseat mode, the hashed <ILogic> should add the player
// name and a comma in front of the given text so the message is directed
// to the current player.
//
// Derived From:
//     <Rule>
//     <LogicHashMixin>
//
// Project:
//     <RuleUpdateMessageUI>
//
// RuleSet:
//     <CoreRules>
//
// Mixin To:
//     <Core>
//
// Transmits To:
//     eventMessageUI
//
class RuleUpdateMessageUI 
: public Rule
, public LogicHashMixin
{
public:
	virtual void Execute(const DataObject &object)
	{
		HashString mode = gameData<HashString>(shMode);;

		DataObject output;
		if(false == DecideHash(mode, object, output))
		{
			wxLogError(wxT("Programmer Error: No Mode %s in ")
				wxT("RuleUpdateMessageUI."), mode.c_str());
			return;
		}

		Controller::get().Transmit(shEventMessageUI, 
			DataObject(output.read<wxString>(), GetGame()));
	}
};

IMPLEMENT_RULE(RuleUpdateMessageUI, CORE)
