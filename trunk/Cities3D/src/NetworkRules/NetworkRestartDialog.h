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
#include "BaseGameDialog.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxNetworkRestartDialog
// 
// Dialog that handles restarting a network game.
//
// Derived From:
//     <wxBaseGameDialog>
//
// Project:
//     <Cities3D>
//
// Include:
//     NetworkRestartDialog.h
//
class wxNetworkRestartDialog : public wxBaseGameDialog
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxNetworkRestartDialog
	//
	// The wxNetworkRestartDialog constructor.
	//
	// Parameters:
	//     parent - The parent window.
	//
	wxNetworkRestartDialog(wxWindow *parent, bool restart);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxNetworkRestartDialog
	//
	// The wxNetworkRestartDialog destructor.
	//
	~wxNetworkRestartDialog();

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Macro: DECLARE_EVENT_TABLE
	//
	// The wxWidgets event table declaration.
	//
	DECLARE_EVENT_TABLE()

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnTimer
	//
	// Handles countdowns.
	//
	void OnTimer();

	//-----------------------------------------------------------------------//
	// Group: Game Event Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnUpdatePlayers
	//
	// Determines the current state of the restart.
	//
	// Parameters:
	//     game - The current <Game>.
	//
	void OnUpdatePlayers(const GamePtr &game);

	//-----------------------------------------------------------------------//
	// Function: OnAllSeatsFull
	//
	// Shuts down the dialog when all seats have been filled.
	//
	void OnAllSeatsFull(wxInt32);

	//-----------------------------------------------------------------------//
	// Function: OnSpectator
	//
	// Enables or disables the dialog buttons.
	//
	void OnSpectator(wxInt32);

	//-----------------------------------------------------------------------//
	// Group: Event Message Handling
	//

	//-----------------------------------------------------------------------//
	// Function: OnPlay
	//
	// Handles an I'll Play button click.
	//
	void OnPlay(wxCommandEvent &);

	//-----------------------------------------------------------------------//
	// Function: OnNoThanks
	//
	// Handles a No Thanks button click.
	//
	void OnNoThanks(wxCommandEvent &);


	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	struct Impl;

	//-----------------------------------------------------------------------//
	// Pointer: mpImpl
	//
	// The private implementation of wxNetworkRestartDialog.
	//
	boost::shared_ptr<Impl> mpImpl;
};

//---------------------------- PROTOTYPES       -----------------------------//

