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
#include "RuleSetDatabase.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

// Tabs.
IMPLEMENT_KEYMAP(CITIES, stShowBarbariansTab,	    TabBarbarians,	        'B',	wxMOD_SHIFT);
IMPLEMENT_KEYMAP(CITIES, stShowUpgradesTab,	        TabUpgrades,	        'U',	wxMOD_SHIFT);

// Build Options.
IMPLEMENT_KEYMAP(CITIES, stRepairACityUI,			RepairCity,				'P',	wxMOD_CONTROL);
IMPLEMENT_KEYMAP(CITIES, stUseCommercialHarbor,		UseHarbor,				'B',	wxMOD_CONTROL);
IMPLEMENT_KEYMAP(CITIES, stBuildABasicKnight,		BuildBasicKnight,		'1',	wxMOD_CONTROL);
IMPLEMENT_KEYMAP(CITIES, stBuildAStrongKnight,	    BuildStrongKnight,		'2',	wxMOD_CONTROL);
IMPLEMENT_KEYMAP(CITIES, stBuildAMightyKnight,	    BuildMightyKnight,		'3',	wxMOD_CONTROL);
IMPLEMENT_KEYMAP(CITIES, stBuildACityWalls,		    BuildCityWalls,			'W',	wxMOD_CONTROL);
IMPLEMENT_KEYMAP(CITIES, stActivateAKnightUI,		ActivateKnight,			'A',	wxMOD_CONTROL);
IMPLEMENT_KEYMAP(CITIES, stMoveAKnight,			    MoveKnight,				'V',	wxMOD_CONTROL);
IMPLEMENT_KEYMAP(CITIES, stAttackAnotherKnight,		AttackKnight,			'K',	wxMOD_CONTROL);
IMPLEMENT_KEYMAP(CITIES, stExpelCriminals,			ExpelCriminals,			'E',	wxMOD_CONTROL);
IMPLEMENT_KEYMAP(CITIES, stUpgradeScience,			UpgradeScience,			'I',	wxMOD_CONTROL);
IMPLEMENT_KEYMAP(CITIES, stUpgradePolitics,			UpgradePolitics,		'O',	wxMOD_CONTROL);
IMPLEMENT_KEYMAP(CITIES, stUpgradeTrade,			UpgradeTrade,			'T',	wxMOD_CONTROL);

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
