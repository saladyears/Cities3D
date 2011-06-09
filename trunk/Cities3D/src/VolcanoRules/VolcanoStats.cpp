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
#include "RuleSetDatabase.h"
#include "Stat.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	void ProcessRuleVolcanoDie(const GamePtr& game, HashString& modifier)
	{
		wxInt32 roll = game->read<wxInt32>(shVolcanoDie);

		if(-1 != roll)
		{
			modifier = HashString::Format("V%d", roll);
		}
		else
		{
			modifier = shEmpty;
		}
	}

}

//---------------------------- PUBLIC           -----------------------------//
IMPLEMENT_STAT(VOLCANO, _("Volcano Die"), "Volcano Die", VolcanoDie, RuleBeginTurn, 
	StatDiceRollFunc(boost::bind(ProcessRuleVolcanoDie, _1, _2)), 3, false);

IMPLEMENT_DICE_ROLL_STAT_MODIFIER(VOLCANO, wxT("1"), "1", V1, RuleNothing, VolcanoDie, 0, 1.0f/6.0f);
IMPLEMENT_DICE_ROLL_STAT_MODIFIER(VOLCANO, wxT("2"), "2", V2, RuleNothing, VolcanoDie, 10, 1.0f/6.0f);
IMPLEMENT_DICE_ROLL_STAT_MODIFIER(VOLCANO, wxT("3"), "3", V3, RuleNothing, VolcanoDie, 20, 1.0f/6.0f);
IMPLEMENT_DICE_ROLL_STAT_MODIFIER(VOLCANO, wxT("4"), "4", V4, RuleNothing, VolcanoDie, 30, 1.0f/6.0f);
IMPLEMENT_DICE_ROLL_STAT_MODIFIER(VOLCANO, wxT("5"), "5", V5, RuleNothing, VolcanoDie, 40, 1.0f/6.0f);
IMPLEMENT_DICE_ROLL_STAT_MODIFIER(VOLCANO, wxT("6"), "6", V6, RuleNothing, VolcanoDie, 50, 1.0f/6.0f);

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
