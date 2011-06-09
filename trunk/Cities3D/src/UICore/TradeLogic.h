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

typedef boost::shared_ptr<Game> GamePtr;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: TradeLogic
// 
// Glues three <wxInteractiveImageStrips> together by providing the logic that
// controls how images get moved among them.  There are two trade 
// <wxInteractiveImageStrips> and one resource pool <wxInteractiveImageStrip> 
// that make up a TradeLogic.
// 
// Project:
//     <Cities3D>
//
// Include:
//     TradeLogic.h
//
class UI_EXPORT_SPEC TradeLogic : public IResourceLogic
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
	//     left - The left side of the trade (the player).
	//     right - The right side of the trade (bank or other players).
	//     resourcePool - The pool resources are drawn from.
	//     button - The button that executes the trade.
	//
	void SetParameters(wxInteractiveImageStrip* left, 
		wxInteractiveImageStrip* right, wxInteractiveImageStrip* resourcePool, 
		wxBaseButton* button);

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//
	
	//-----------------------------------------------------------------------//
	// Constructor: TradeLogic
	//
	// The TradeLogic constructor.
	//
	// Parameters:
	//     game - The current <Game>.
	//
	explicit TradeLogic(boost::weak_ptr<Game> game);

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
	// Function: GetLeftSide
	//
	// Returns the left side of the trade (the player).
	//
	// Returns:
	//    The <wxSelectionImageStrip> that is the left side of the trade.
	//
	wxInteractiveImageStrip* GetLeftSide() const
	{
		wxASSERT(mLeftSide);
		return mLeftSide;
	}

	//-----------------------------------------------------------------------//
	// Function: GetRightSide
	//
	// Returns the right side of the trade (bank or other players).
	//
	// Returns:
	//    The <wxSelectionImageStrip> that is the right side of the trade.
	//
	wxInteractiveImageStrip* GetRightSide() const
	{
		wxASSERT(mRightSide);
		return mRightSide;
	}

	//-----------------------------------------------------------------------//
	// Function: GetResourcePool
	//
	// Returns the resource pool that the trade draws from.
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
	// Returns the button that executes the trade.
	//
	// Returns:
	//    The <wxBaseButton> that is the trade button.
	//
	wxBaseButton* GetButton() const
	{
		wxASSERT(mButton);
		return mButton;
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
	// Pointer: mLeftSide
	//
	// The left side of the trade.
	//
	wxInteractiveImageStrip* mLeftSide;

	//-----------------------------------------------------------------------//
	// Pointer: mRightSide
	//
	// The right side of the trade.
	//
	wxInteractiveImageStrip* mRightSide;

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
};

//---------------------------- PROTOTYPES       -----------------------------//

