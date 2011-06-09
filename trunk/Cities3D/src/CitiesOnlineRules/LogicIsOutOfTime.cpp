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
#include "DataObject.h"
#include "Logic.h"
#include "RuleSetDatabase.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicIsOutOfTime
// 
// Determines if the given player is completely out of time, with no refills
// available.
//
// Derived From:
//     <Logic>
//
// Project:
//     <CitiesOnlineRules>
//
// RuleSet:
//     <CitiesOnline>
//
class LogicIsOutOfTime : public Logic
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
        bool done = false;

        RULE.Decide(shLogicIsThisPlayer, input, output);
        wxInt32 actualIndex = output.read<wxInt32>(1);
        
        // If this person is in a passive turn timer, and they hit the time
        // limit, they are done.  Or, if they are out of extra turn timers.
        if( (-1 != actualIndex) &&
            ((TRUE == playerGameData<wxInt32>(shPassiveTurnTimer, actualIndex)) ||
            (0 == playerGameData<wxInt32>(shExtraTimers, actualIndex))))
        {
            done = true;
        }
        
        output = DataObject(done);
	}
};

IMPLEMENT_LOGIC(LogicIsOutOfTime, CITIESONLINE)
