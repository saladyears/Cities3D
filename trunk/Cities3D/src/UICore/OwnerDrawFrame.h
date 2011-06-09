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
// Class: wxOwnerDrawFrame
// 
// Needs documentation.
//
// Derived From:
//     wxFrame
//
// Project:
//     <UICore>
//
// Include:
//     OwnerDrawFrame.h
//
class UI_EXPORT_SPEC wxOwnerDrawFrame : public wxFrame
{
	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxOwnerDrawFrame
	//
	// The wxOwnerDrawFrame constructor.
	//
	// Parameters:
	//     parent - The parent window.
	//     title - The title caption.
	//     pos - The starting window position.
	//     size - The starting window size.
	//     style - The window style.
	//
	wxOwnerDrawFrame(wxWindow *parent, const wxString &title = swEmpty, 
		const wxPoint &pos = wxDefaultPosition, 
		const wxSize &size = wxDefaultSize, const wxInt32 style = 0);

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Group: Pure Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Paint
	//
	// Allows custom painting.  Derived classes can override
	// Paint to customize drawing.
	//
	// Parameters:
	//     dc - The memory device context to draw to.
	//
	virtual void Paint(wxMemoryDC &dc)=0;

	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Size
	//
	// Allows derived classes to handle the Size event.  Called when
	// the window is resized.
	//
	// Parameters:
	//     event - The size event.
	//
	virtual void Size(wxSizeEvent &) {}	

	//-----------------------------------------------------------------------//
	// Function: WindowCreate
	//
	// Allows derived classes to handle the WindowCreate event. It is
	// currently only available for wxGTK.
	//
	// Parameters:
	//     event - The WindowCreate event.
	//
	#ifdef __WXGTK__
	virtual void WindowCreate(wxWindowCreateEvent &) {}
	#endif

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
	// Group: Event Message Handling
	//

	//-----------------------------------------------------------------------//
	// Function: OnPaint
	//
	// Paints the window.  Selects the memory bitmap into
	// the device context and then calls the Paint function.
	// 
	// Parameters:
	//     event - The paint event.
	//
	void OnPaint(wxPaintEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnBackground
	//
	// Erases the window.  Necessary in order to prevent
	// flickering when redrawing the window.
	// 
	// Parameters:
	//     event - The erase event.
	//
	void OnBackground(wxEraseEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnSize
	//
	// Resizes the window.
	// 
	// Parameters:
	//     event - The size event.
	//
	void OnSize(wxSizeEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnWindowCreate
	//
	// Handles the WindowCreate event. It is currently only
	// available on wxGTK.
	//
	// Parameters:
	//     event - The WindowCreate event.
	//
	#ifdef __WXGTK__
	void OnWindowCreate(wxWindowCreateEvent &event);
	#endif
	
	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Object: mMemBitmap
	//
	// The memory bitmap for the window.  Prevents flickering
	// by having the entire scene drawn into the memory bitmap
	// via the Paint function and then blitting the bitmap in one
	// pass to the screen in OnPaint.
	//
	wxBitmap mMemBitmap;
};

//---------------------------- PROTOTYPES       -----------------------------//
