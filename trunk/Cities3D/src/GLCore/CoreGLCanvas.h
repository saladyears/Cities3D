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
#include <wx/glcanvas.h>

//---------------------------- USER INCLUDES    -----------------------------//
#include "GLExport.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxCoreGLCanvas
// 
// A class that provides the barest of necessities for a GL window.
//
// Project:
//     <GLCore>
//
// Include:
//     CoreGLCanvas.h
//
class GL_EXPORT_SPEC wxCoreGLCanvas : public wxGLCanvas
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: GetContext
	//
	// Returns a pointer to the wxGLWidget's OpenGL context or NULL.
	//
	wxGLContext* GetContext();

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxCoreGLCanvas
	// 
	// The wxCoreGLCanvas constructor.  Initializes the OpenGL context
	// with default settings, sets up lighting, and resizes the OpenGL
	// context to match the window's dimensions.
	// 
	// Parameters:
	//     parent - The parent window (must not be NULL).
	//     id - The id for message handling.
	//     pos - The starting position.
	//     size - The starting size.
	//     style - The window style, using wxWindow parameters.
	//     name - The name.
	//     attribute - Starting OpenGL implementation attributes.
	//
	wxCoreGLCanvas(wxWindow *parent, const wxWindowID id = -1, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, long style = 0, 
		const wxString& name = swCoreGLCanvas, wxInt32 *attrib = NULL);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxCoreGLCanvas
	// 
	// The wxCoreGLCanvas destructor.  It is pure virtual because
	// this class cannot be instantiated directly.  It must be
	// derived from.
	// 
	virtual ~wxCoreGLCanvas()=0;

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: ResizeGL
	//
	// Resizes the OpenGL viewport the current wxWindow size.
	// 
	void ResizeGL();

	//-----------------------------------------------------------------------//
	// Function: IsDirty
	//
	// Returns whether the scene is dirty or not.
	//
	// Returns:
	//     A bool that is true if the scene is dirty and false if not.
	//
	bool IsDirty() const
	{
		return mDirty;
	}

	//-----------------------------------------------------------------------//
	// Function: SetDirty
	//
	// Marks the scene as dirty so that it will be redrawn.
	//
	// Parameters:
	//     dirty - A flag that is true if the scene is dirty and false if not.
	//
	void SetDirty(bool dirty = true);

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
	// Group: Pure Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Paint
	//
	// Paints the scene.
	//
	virtual void Paint()=0;

	//-----------------------------------------------------------------------//
	// Group: Event Message Handling
	//

	//-----------------------------------------------------------------------//
	// Function: OnSize
	//
	// Resizes the window.
	// 
	// Parameters:
	//     event - The size event.
	//
    void OnSize(wxSizeEvent& event);

	//-----------------------------------------------------------------------//
	// Function: OnPaint
	//
	// Handles paint messages when the window is in the background.
	//
	void OnPaint(wxPaintEvent &);

	//-----------------------------------------------------------------------//
	// Function: OnEraseBackground
	//
	// Erases the window.  Necessary in order to prevent
	// flickering when redrawing the OpenGL scene.
	// 
	// Parameters:
	//     event - The erase event.
	//
    void OnEraseBackground(wxEraseEvent& event);

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Pointer: mpTopLevelWindow
	//
	// A pointer to the top level window parent of this canvas, if any.
	//
	wxTopLevelWindow *mpTopLevelWindow;

	//-----------------------------------------------------------------------//
	// Bool: mDirty
	//
	// A flag that is true if the scene should be redrawn, false if not.
	//
	bool mDirty;

#	ifdef __WXGTK__
	//-----------------------------------------------------------------------//
	// Bool: mPainted
	//
	// A flag that is true if OnPaint() was triggered before, false if not.
	// It is only used with wxGTK at the moment.
	//
	bool mPainted;
#	endif
};

//---------------------------- PROTOTYPES       -----------------------------//
