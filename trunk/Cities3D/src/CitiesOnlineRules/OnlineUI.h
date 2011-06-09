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
class DataObject;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxOnlineUI
// 
// Displays Cities Online related information to the user.
//
// Derived From:
//     wxPanel
//
// Project:
//     <Cities3D>
//
// Include:
//     wxOnlineUI.h
//
class wxOnlineUI : public wxBaseUI<wxPanel>
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxOnlineUI
	//
	// The wxOnlineUI constructor.  
    //
	// Parameters:
	//     parent - The parent window.  Must not be NULL.
	//
	wxOnlineUI(wxWindow *parent);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxOnlineUI
	//
	// The wxOnlineUI destructor.
	//
	~wxOnlineUI();

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
    //-----------------------------------------------------------------------//
	// Group: Functions
	//
    
    //-----------------------------------------------------------------------//
	// Function: ResetCountdown
	//
    // Resets the countdown timer to the given number of seconds.
    //
    // Parameters:
    //     object - The data paremeter.
    //
    // Returns:
    //     A bool that is true if the timer is ticking, and false if not.
    //
    bool ResetCountdown(const DataObject& object);

	//-----------------------------------------------------------------------//
	// Group: Game Event Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnResetTurnTime
	//
	// Resets the turn timer.
	//
	// Parameters:
	//     object - The data paremeter.
	//
	void OnResetTurnTime(const DataObject& object);

    //-----------------------------------------------------------------------//
	// Function: OnResetPassiveTime
	//
	// Resets the passive quit timer.
	//
	// Parameters:
	//     seconds - The seconds to reset to.
	//
	void OnResetPassiveTime(wxInt32 seconds);

    //-----------------------------------------------------------------------//
	// Function: OnShutdownUI
	//
	// Turns off the timer.
	//
	void OnShutdownUI(wxInt32 = 0);

	//-----------------------------------------------------------------------//
	// Function: OnTimer
	//
	// Handles time countdown.
	//
	void OnTimer();
};

//---------------------------- PROTOTYPES       -----------------------------//
