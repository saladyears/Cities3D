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
#include "Controller.h"
#include "SkinManager.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: PaneMixin
// 
// Mixin that provides common functionality for all panes.
//
// Project:
//     <UICore>
//
// Include:
//     PaneMixin.h
//
class PaneMixin
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: GetWindow
	//
	// Returns the wxWindow this interface is attached to.
	//
	// Returns:
	//     The wxWindow this interface is attached to.
	//
	wxWindow* GetWindow() const
	{
		return mpThis;
	}

	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: ClosePane
	//
	// Allows derived classes to perform any required cleanup prior to the
	// pane closing.
	//
	virtual void ClosePane()
	{
	}

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: PaneMixin
	//
	// The PaneMixin constructor.
	//
	// Parameters:
	//     thisWindow - The window this PaneMixin is mixed into.
	//     id - The id of the menu controlling this pane, if any.
	//
	PaneMixin(wxWindow* thisWindow, wxInt32 id = -1)
		: mpThis(thisWindow)
		, mMenuId(id)
	{
		thisWindow->SetBackgroundColour(SKIN.Element(shBackground));
	}

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~PaneMixin
	//
	// The PaneMixin destructor.
	//
	virtual ~PaneMixin()
	{
		// If a menu item is controlling this pane, make sure it is unchecked.
		if(-1 != mMenuId)
		{
			Controller::get().Transmit(shEventUncheckMenu, mMenuId);
		}
	}

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Pointer: mpThis
	//
	// A pointer to the window this PaneMixin is mixed into.
	//
	wxWindow* mpThis;

	//-----------------------------------------------------------------------//
	// Pointer: mMenuId
	//
	// The id of the menu controlling this pane, if any.
	//
	wxInt32 mMenuId;
};

//---------------------------- PROTOTYPES       -----------------------------//

