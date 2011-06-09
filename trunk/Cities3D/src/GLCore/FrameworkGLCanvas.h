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
#include "BaseGLCanvas.h"
#include "GLExport.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
struct Vector;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxFrameworkGLCanvas
// 
// A class that provides mouse handling, camera, and selection
// for an OpenGL window.  
//
// Derived From:
//     <wxBaseGLCanvas>
//
// Project:
//     <GLCore>
//
// Include:
//     FrameworkGLCanvas.h
//
class GL_EXPORT_SPEC wxFrameworkGLCanvas : public wxBaseGLCanvas
{
	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxFrameworkGLCanvas
	// 
	// The wxFrameworkGLCanvas constructor.  Calls <ResetCamera>
	// to set the camera to its starting position.
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
	wxFrameworkGLCanvas(wxWindow *parent, const wxWindowID id = -1, 
		const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, 
		long style = 0, const wxString& name = swFrameworkGLCanvas, 
		wxInt32 *attrib = NULL);

	//-----------------------------------------------------------------------//
	// Group: Destructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: ~wxFrameworkGLCanvas
	// 
	// The wxFrameworkGLCanvas destructor.
	//	
	~wxFrameworkGLCanvas();

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: SetHitTest
	//
	// Turns hit tests on or off based on the hit parameter.  Always
	// resets the selection to -1.
	// 
	// Parameters:
	//     hit - Turn hit tests on or off.
	//
	void SetHitTest(const bool hit);

	//-----------------------------------------------------------------------//
	// Function: GotFinalHit
	//
	// Returns whether an object was actually selected or simply
	// moused over.
	//
	// Returns:
	//     A bool set to true only when an object was selected 
	//     with a left mouse down as opposed to a mouseover selection that 
	//     just highlights the object.
	//
	bool GotFinalHit() const {return mFinalHit;}

	//-----------------------------------------------------------------------//
	// Function: GetSelection
	//
	// Returns the id of the currently selected object.
	//
	// Returns:
	//     The id of the currently selected object.
	//
	wxInt32 GetSelection() const {return mSelection;}

	//-----------------------------------------------------------------------//
	// Function: GetCameraPos
	//
	// Returns the position of the current view camera.
	//
	// Returns:
	//     A <Vector> containing the view camera's position.
	//
	const Vector &GetCameraPos() const;

	//-----------------------------------------------------------------------//
	// Function: GetCameraLookAt
	//
	// Returns the lookat position of the current view camera.
	//
	// Returns:
	//     A <Vector> containing the view camera's lookat position.
	//
	const Vector &GetCameraLookAt() const;

	//-----------------------------------------------------------------------//
	// Function: SaveCamera
	//
	// Stores the current camera settings in the <ConfigDatabase>.
	//
	void SaveCamera();

	//-----------------------------------------------------------------------//
	// Function: ResetCamera
	//
	// Resets the camera position and lookat back 
	// to original <Map> values.
	// 
	void ResetCamera();

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
	// Function: rotate
	//
	// Rotates the view by adjusting the <mXRot> and <mYRot> variables.
	// 
	// Parameters:
	//     x - The amount of mouse movement in the x axis.
	//     y - The amount of mouse movement in the y axis.
	//
	void rotate(const float x, const float y);

	//-----------------------------------------------------------------------//
	// Function: pan
	//
	// Pans the view by adjusting the camera lookat position
	// 
	// Parameters:
	//     x - The amount of mouse movement in the x axis.
	//     y - The amount of mouse movement in the y axis.
	//
	void pan(const float x, const float y);

	//-----------------------------------------------------------------------//
	// Function: zoom
	//
	// Zooms the view by adjusting the camera position.
	// 
	// Parameters:
	//     x - The amount of mouse movement in the x axis.
	//     y - The amount of mouse movement in the y axis.
	//
	void zoom(const float x, const float y);

	//-----------------------------------------------------------------------//
	// Group: Pure Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Select
	//
	// Does object selection checking based on the given
	// window coordinates.
	//
	// Parameters:
	//     x - The x coordinate of the current mouse position.
	//     y - The y coordinate of the current mouse position.
	//
	// Returns:
	//     The OpenGL id of nearest object, or -1 if none.
	//
	virtual wxInt32 Select(const wxInt32 x, const wxInt32 y) const=0;

	//-----------------------------------------------------------------------//
	// Function: SelectObject
	//
	// Handles object selection.
	// 
	// Parameters:
	//     id - The OpenGL ID of the selected object.
	//
	virtual void SelectObject(wxInt32 selection)=0;

	//-----------------------------------------------------------------------//
	// Group: Event Message Handling
	//

	//-----------------------------------------------------------------------//
	// Function: OnMouseMove
	//
	// Handles the mouse moving.  May pan, zoom, or rotate the OpenGL
	// view.
	// 
	// Parameters:
	//     event - The mouse event.
	//
	void OnMouseMove(wxMouseEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnMouseWheel
	//
	// Handles the mouse wheel moving.  Zooms the OpenGL view.
	// 
	// Parameters:
	//     event - The mouse event.
	//
	void OnMouseWheel(wxMouseEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnLButtonDown
	//
	// Handles the left mouse click.  Performs a hit test to see
	// if any objects is selected.
	// 
	// Parameters:
	//     event - The mouse event.
	//
	void OnLButtonDown(wxMouseEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnMButtonDown
	//
	// Handles the middle mouse down.  Captures the mouse for
	// panning.
	// 
	// Parameters:
	//     event - The mouse event.
	//
	void OnMButtonDown(wxMouseEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnMButtonUp
	//
	// Handles the middle mouse up.  Releases the mouse.
	// 
	// Parameters:
	//     event - The mouse event.
	//
	void OnMButtonUp(wxMouseEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnRButtonDown
	//
	// Handles the right mouse down.  Captures the mouse for
	// rotation or zooming.
	// 
	// Parameters:
	//     event - The mouse event.
	//
	void OnRButtonDown(wxMouseEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnRButtonUp
	//
	// Handles the right mouse up.  Releases the mouse.
	// 
	// Parameters:
	//     event - The mouse event.
	//
	void OnRButtonUp(wxMouseEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnRButtonDblClk
	//
	// Handles the right mouse double click.  Resets the camera to its
	// original position and look at.
	// 
	// Parameters:
	//     event - The mouse event.
	//
	void OnRButtonDblClk(wxMouseEvent &event);

	//-----------------------------------------------------------------------//
	// Group: Game Event Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnMapZoomIn
	//
	// Zooms the map in.
	//
	void OnMapZoomIn(wxInt32);

	//-----------------------------------------------------------------------//
	// Function: OnMapZoomOut
	//
	// Zooms the map out.
	//
	void OnMapZoomOut(wxInt32);

	//-----------------------------------------------------------------------//
	// Function: OnMapPanLeft
	//
	// Pans the map left.
	//
	void OnMapPanLeft(wxInt32);

	//-----------------------------------------------------------------------//
	// Function: OnMapPanRight
	//
	// Pans the map right.
	//
	void OnMapPanRight(wxInt32);

	//-----------------------------------------------------------------------//
	// Function: OnMapPanUp
	//
	// Pans the map up.
	//
	void OnMapPanUp(wxInt32);

	//-----------------------------------------------------------------------//
	// Function: OnMapPanDown
	//
	// Pans the map down.
	//
	void OnMapPanDown(wxInt32);

	//-----------------------------------------------------------------------//
	// Function: OnMapRotateLeft
	//
	// Rotates the map left.
	//
	void OnMapRotateLeft(wxInt32);

	//-----------------------------------------------------------------------//
	// Function: OnMapRotateRight
	//
	// Rotates the map right.
	//
	void OnMapRotateRight(wxInt32);

	//-----------------------------------------------------------------------//
	// Function: OnMapRotateUp
	//
	// Rotates the map up.
	//
	void OnMapRotateUp(wxInt32);

	//-----------------------------------------------------------------------//
	// Function: OnMapRotateDown
	//
	// Rotates the map down.
	//
	void OnMapRotateDown(wxInt32);

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Bool: mHitTest
	//
	// Whether hit tests are on or off.
	//
	bool mHitTest;

	//-----------------------------------------------------------------------//
	// Bool: mFinalHit
	//
	// Whether an actual selection has occurred or just a mouseover.
	//
	bool mFinalHit;

	//-----------------------------------------------------------------------//
	// Object: mPoint
	//
	// The last captured position of the mouse. We can use it to get how
	// much it has moved between then and now.
	//
	wxPoint mPoint;

	//-----------------------------------------------------------------------//
	// Int: mSelection
	//
	// The OpenGL id of the current selection.
	//
	wxInt32 mSelection;
};

//---------------------------- PROTOTYPES       -----------------------------//

