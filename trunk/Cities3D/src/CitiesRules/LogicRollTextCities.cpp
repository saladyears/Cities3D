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
#include "SideObject.h"
#include "CornerObject.h"
#include "Utility.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicRollTextCities
// 
// Sets the text on the dice roll button appropriately in the <ControlsUI>.
// Replaces the eponymously named rule in StandardRules.
//
// Derived From:
//     <Logic>
//
// Project:
//     <CitiesRules>
//
// RuleSet:
//     <Cities>
//
class LogicRollTextCities : public Logic
{
public:
	virtual void Decide(const DataObject &, DataObject &output)
	{
		wxString str;

		if(TRUE == playerGameData<wxInt32>(shAlchemist))
		{
			wxInt32 total = gameData<wxInt32>(shAlchemistDie1) + 
				gameData<wxInt32>(shAlchemistDie2);

			str = wxString::Format(stRollA.c_str(), total);
		}
		else
		{
			str = stRollDice;
		}

		output = DataObject(str);
	}
};

IMPLEMENT_LOGIC_REPLACE(LogicRollTextCities, CITIES, LogicRollText)
