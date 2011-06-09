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

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxOwnerDrawWindow
// 
// An abstract base class that allows derived classes to paint themselves with
// flicker-free bitmap blitting.
//
// Derived From:
//     wxWindow (see <http://www.wxwindows.org/manuals/2.4.2/wx411.htm>)
//
// Project:
//     <UICore>
//
// Include:
//     OwnerDrawWindow.h
//
class UI_EXPORT_SPEC wxOwnerDrawWindow : public wxWindow
{
	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxOwnerDrawWindow
	//
	// The wxOwnerDrawWindow constructor.  Passes through to the base
	// wxWindow constructor.
	//
	// Parameters:
	//     parent - The parent window (must not be NULL).
	//     id - The id for message handling.
	//     pos - The starting position.
	//     size - The starting size.
	//     style - The window style.
	//
	wxOwnerDrawWindow(wxWindow* parent, wxWindowID id, 
		const wxPoint &pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, long style = 0);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: wxOwnerDrawWindow
	//
	// The wxOwnerDrawWindow destructor.
	//
	virtual ~wxOwnerDrawWindow()=0;

	//-----------------------------------------------------------------------//
	// Group: Pure Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Paint
	//
	// Paints the window.  Uses a memory dc allows for flicker free 
	// drawing.  The derived class paints itself into the memory dc, 
	// which is then blitted to the screen all at once.
	//
	// Parameters:
	//     dc - The memory dc to draw into.
	//
	virtual void Paint(wxMemoryDC &dc)=0;

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
	// Variable: mMemBitmap
	//
	// The memory bitmap for flicker free drawing.
	//
	wxBitmap mMemBitmap;

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Size
	//
	// Resizes the window.  The derived class has the option
	// to do specific resize handling.
	//
	// Parameters:
	//     event - The resize event.
	//
	virtual void Size(wxSizeEvent &) {};

	//-----------------------------------------------------------------------//
	// Group: Event Message Handling
	//

	//-----------------------------------------------------------------------//
	// Function: OnBackground
	//
	// Erases the button.  Necessary in order to prevent
	// flickering when redrawing the window.
	// 
	// Parameters:
	//     event - The erase event.
	//
	void OnBackground(wxEraseEvent& event);

	//-----------------------------------------------------------------------//
	// Function: OnSize
	//
	// Resizes the window.  Resets the memory bitmap
	// to the proper dimensions.
	// 
	// Parameters:
	//     event - The size event.
	//
	void OnSize(wxSizeEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnPaint
	//
	// Paints the window.  Sets up the memory dc and then calls the Paint 
	// function for derived classes to do their custom drawing.
	// 
	// Parameters:
	//     event - The paint event.
	//
	void OnPaint(wxPaintEvent& event);
};

//---------------------------- PROTOTYPES       -----------------------------//
inline wxOwnerDrawWindow::~wxOwnerDrawWindow() {}
