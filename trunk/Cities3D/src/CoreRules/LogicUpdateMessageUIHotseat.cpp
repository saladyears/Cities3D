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

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicUpdateMessageUIHotseat
// 
// Provides <RuleUpdateMessageUI> with the wrapper text for
// hotseat games.  Puts the player name and a comma in front
// of the given text, and a period after the given text.
//
// Derived From:
//     <Logic>
//
// Project:
//     <CoreRules>
//
// RuleSet:
//     <Core>
//
// Mixin To:
//     <RuleUpdateMessageUI>
//
class LogicUpdateMessageUIHotseat : public Logic
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		// Use the given string as the format string and add the player's name.
		wxString str = wxString::Format(input.read<wxString>().c_str(),
			playerGame().player().Name().c_str());

		output = DataObject(str);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(LogicUpdateMessageUIHotseat, Hotseat, 
							RuleUpdateMessageUI, HOTSEAT)
