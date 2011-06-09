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
#pragma once

#include "style.h"	//READ THIS BEFORE MAKING ANY CHANGES TO THIS FILE!!!

//---------------------------- SYSTEM INCLUDES  -----------------------------//

//---------------------------- USER INCLUDES    -----------------------------//
#include "BaseUI.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class Game;
typedef boost::shared_ptr<Game> GamePtr;

class wxImageCenteredBitmap;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxUpgradeUI
//
// Houses the city upgrade window.
// 
// Derived From:
//     <wxBaseUI>
//
// Project:
//     <CitiesRules>
//
// Include:
//     UpgradeUI.h
//
class wxUpgradeUI : public wxBaseUI<wxPanel>
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxUpgradeUI
	//
	// The wxUpgradeUI constructor.
	//
	// Parameters:
	//     parent - The parent window.  Must not be NULL.
	//     game - The current <Game>.
	//
	wxUpgradeUI(wxWindow* parent, const GamePtr &game);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxUpgradeUI
	//
	// The wxUpgradeUI destructor.
	//
	virtual ~wxUpgradeUI();

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Game Event Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnCityUpgrade
	//
	// Handles cities being upgraded.
	//
	// Parameters:
	//     game - The <Game> data.
	//
	void OnCityUpgrade(const GamePtr& game);

	//-----------------------------------------------------------------------//
	// Function: OnPlayerReorder
	//
	// Handles the players being reordered.
	//
	// Parameters:
	//     game - The current <Game>.
	//
	void OnPlayerReorder(const GamePtr& game);

	//-----------------------------------------------------------------------//
	// Function: OnTabUpgrades
	//
	// Handles a raise tab event.
	//
	void OnTabUpgrades(wxInt32);

	//-----------------------------------------------------------------------//
	// Group: Variables
	//
	typedef std::vector<wxImageCenteredBitmap*> CenteredBitmapArray;

	CenteredBitmapArray mTrade;
	CenteredBitmapArray mPolitics;
	CenteredBitmapArray mScience;
};

//---------------------------- PROTOTYPES       -----------------------------//

