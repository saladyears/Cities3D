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
#include <map>

//---------------------------- USER INCLUDES    -----------------------------//
#include "UIExport.h"
#include "BaseUI.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class DataObject;

class Game;
typedef boost::shared_ptr<Game> GamePtr;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxRollsUI
//
// Houses the dice rolls window in the <SideUI>.
// 
// Derived From:
//     <wxBaseUI>
//
// Project:
//     <UICore>
//
// Include:
//     wxRollsUI.h
//
class UI_EXPORT_SPEC wxRollsUI : public wxBaseUI<wxPanel>
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxRollsUI
	//
	// The wxRollsUI constructor.
	//
	// Parameters:
	//     parent - The parent window.  Must not be NULL.
	//     game - The current <Game>.
	//     threeDice - A bool that is true if the window needs to show three
	//                 dice and false if it needs to show two.
	//
	wxRollsUI(wxWindow* parent, const GamePtr &game, bool threeDice);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxRollsUI
	//
	// The wxRollsUI destructor.
	//
	virtual ~wxRollsUI();

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Group: Game Event Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnDiceRoll
	//
	// Handles dice being rolled.
	//
	// Parameters:
	//     data - The <DataObject> containing the dice information.
	//
	void OnDiceRoll(const DataObject &data);

	//-----------------------------------------------------------------------//
	// Function: OnThirdDie
	//
	// Handles the third die being rolled.
	//
	// Parameters:
	//     die - The die value.
	//
	void OnThirdDie(const size_t die);

	typedef std::map<HashString, wxInt32> ResourceMap;
	typedef std::vector<ResourceMap> ResourceMapArray;

	//-----------------------------------------------------------------------//
	// Function: OnPlayerResources
	//
	// Handles resources being doled out.
	//
	// Parameters:
	//     resources - array of resource maps.
	//
	void OnPlayerResources(const ResourceMapArray &resources);

	//-----------------------------------------------------------------------//
	// Function: OnTabRolls
	//
	// Handles a raise tab event.
	//
	void OnTabRolls(wxInt32);

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
	// Group: Variables
	//
	
	//-----------------------------------------------------------------------//
	// Bool: mThreeDice
	//
	// A flag that is true if there are three dice in the UI and false if not.
	//
	bool mThreeDice;
};

//---------------------------- PROTOTYPES       -----------------------------//

