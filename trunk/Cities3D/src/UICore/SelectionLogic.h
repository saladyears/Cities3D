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
#include "UIExport.h"
#include "IResourceLogic.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class Game;
class wxInteractiveImageStrip;
class wxBaseButton;
class wxBaseStaticText;

typedef boost::shared_ptr<Game> GamePtr;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: SelectionLogic
// 
// Glues two <wxInteractiveImageStrip> together by providing the logic that 
// controls how images get moved among them.  There is one selection 
// <wxInteractiveImageStrip> and one resource pool <wxInteractiveImageStrip> 
// that make up a SelectionLogic.
// 
// Project:
//     <Cities3D>
//
// Include:
//     SelectionLogic.h
//
class UI_EXPORT_SPEC SelectionLogic : public IResourceLogic
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: SetParameters
	//
	// Sets the image strips and button controlled by the trade logic.
	//
	// Parameters:
	//     selection - The selection.
	//     resourcePool - The pool resources are drawn from.
	//     button - The button that executes the selection.
	//     text - The informational text about the selection.
	//
	void SetParameters(wxInteractiveImageStrip* selection, 
		wxInteractiveImageStrip* resourcePool,	wxBaseButton* button, 
		wxBaseStaticText* text);

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//
	
	//-----------------------------------------------------------------------//
	// Constructor: SelectionLogic
	//
	// The SelectionLogic constructor.
	//
	// Parameters:
	//     game - The current <Game>.
	//
	explicit SelectionLogic(boost::weak_ptr<Game> game);

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: GetGame
	//
	// Returns the current <Game>.
	//
	// Returns:
	//     The current <Game>.
	//
	const GamePtr GetGame() const
	{
		GamePtr game = mGame.lock();
		wxASSERT(game);
		return game;
	}

	//-----------------------------------------------------------------------//
	// Function: GetSelection
	//
	// Returns the selection.
	//
	// Returns:
	//    The <wxSelectionImageStrip> that is the player's selection.
	//
	wxInteractiveImageStrip* GetSelection() const
	{
		wxASSERT(mSelection);
		return mSelection;
	}

	//-----------------------------------------------------------------------//
	// Function: GetResourcePool
	//
	// Returns the resource pool that the selection draws from.
	//
	// Returns:
	//    The <wxResourcePoolImageStrip> that is the resource pool.
	//
	wxInteractiveImageStrip* GetResourcePool() const
	{
		wxASSERT(mResourcePool);
		return mResourcePool;
	}

	//-----------------------------------------------------------------------//
	// Function: GetButton
	//
	// Returns the button that executes the selection.
	//
	// Returns:
	//    The <wxBaseButton> that is the selection button.
	//
	wxBaseButton* GetButton() const
	{
		wxASSERT(mButton);
		return mButton;
	}

	//-----------------------------------------------------------------------//
	// Function: GetText
	//
	// Returns the information text for the selection.
	//
	// Returns:
	//    The <wxBaseStaticText> that is the information text.
	//
	wxBaseStaticText* GetText() const
	{
		wxASSERT(mText);
		return mText;
	}
	
	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Pointer: mGame
	//
	// The current <Game>.
	//
	boost::weak_ptr<Game> mGame;

	//-----------------------------------------------------------------------//
	// Pointer: mSelection
	//
	// The player's selection.
	//
	wxInteractiveImageStrip* mSelection;

	//-----------------------------------------------------------------------//
	// Pointer: mResourcePool
	//
	// The resource pool.
	//
	wxInteractiveImageStrip* mResourcePool;

	//-----------------------------------------------------------------------//
	// Pointer: mButton
	//
	// The trade button.
	//
	wxBaseButton *mButton;

	//-----------------------------------------------------------------------//
	// Pointer: mText
	//
	// The informational text.
	//
	wxBaseStaticText *mText;
};

//---------------------------- PROTOTYPES       -----------------------------//
typedef boost::shared_ptr<SelectionLogic> SelectionLogicPtr;
