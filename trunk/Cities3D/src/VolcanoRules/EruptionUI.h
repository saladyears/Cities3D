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
#include "BaseUI.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class DataObject;

class Game;
typedef boost::shared_ptr<Game> GamePtr;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxEruptionUI
//
// Houses the dice Volcano window.
// 
// Derived From:
//     wxBaseUI
//
// Project:
//     <VolcanoRules>
//
// Include:
//     EruptionUI.h
//
class wxEruptionUI : public wxBaseUI<wxPanel>
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxEruptionUI
	//
	// The wxEruptionUI constructor.
	//
	// Parameters:
	//     parent - The parent window.  Must not be NULL.
	//     game - The current <Game>.
	//
	wxEruptionUI(wxWindow* parent, const GamePtr &game);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxEruptionUI
	//
	// The wxEruptionUI destructor.
	//
	virtual ~wxEruptionUI();

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
	// Function: OnVolcanoRoll
	//
	// Handles the volcano die being rolled.
	//
	// Parameters:
	//     roll - The value of the dice roll.
	//
	void OnVolcanoRoll(wxInt32 roll);

	//-----------------------------------------------------------------------//
	// Function: OnVolcanoResult
	//
	// Handles the results of the volcano being displayed.
	//
	// Parameters:
	//     results - The array containing information about what happened.
	//
	void OnVolcanoResult(const StringArray& results);

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
	// Function: OnTabVolcano
	//
	// Handles a raise tab event.
	//
	void OnTabVolcano(wxInt32);
};

//---------------------------- PROTOTYPES       -----------------------------//

