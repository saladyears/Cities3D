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
#include "OwnerDrawFrame.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class ILayoutCreator;
typedef boost::shared_ptr<ILayoutCreator> ILayoutCreatorPtr;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxBaseContextMenu
// 
// A custom-drawn context menu window.  Handles the drawing, fade, and unfade.
//
// Derived From:
//     <wxFrame>
//
// Project:
//     <UICore>
//
// Include:
//     BaseContextMenu.h
//
class UI_EXPORT_SPEC wxBaseContextMenu : public wxFrame
{
	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxBaseContextMenu
	//
	// The wxBaseContextMenu constructor.
	//
	// Parameters:
	//     parent - The parent window.  Must not be NULL.
	//     layout - The <ILayoutCreator> that creates the <wxBaseListCtrl> for
	//              the context menu.
	//     pos - The starting position for the upper-left corner of
	//           the context menu.  May be adjusted depending on availble
	//           screen real estate.
	//
	wxBaseContextMenu(wxWindow *parent, ILayoutCreatorPtr layout, 
		const wxPoint &pos);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxBaseContextMenu
	//
	// The wxBaseContextMenu destructor.
	//
	~wxBaseContextMenu();

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//
	
	//-----------------------------------------------------------------------//
	// Function: SampleMouse
	//
	// Samples the mouse to see where it is in relation to the menu.
	//
	void SampleMouse();

	//-----------------------------------------------------------------------//
	// Group: Game Event Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnContextMenuClick
	//
	// Shuts down the context menu if one of its options is clicked.
	//
	// Parameters:
	//     id - The id of the selection clicked.
	//
	virtual void OnContextMenuClick(wxInt32 id);

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Object: mWatch
	//
	// The stopwatch that keeps track of how long the mouse has been outside
	// the frame.
	//
	wxStopWatch mWatch;

	//-----------------------------------------------------------------------//
	// Bool: mIsOutside
	//
	// A flag that is true if the mouse is outside the frame, false if not.
	//
	bool mIsOutside;

	//-----------------------------------------------------------------------//
	// Object: mOutside
	//
	// The last time the mouse went outside the frame.
	//
	long mOutside;
};

//---------------------------- PROTOTYPES       -----------------------------//

