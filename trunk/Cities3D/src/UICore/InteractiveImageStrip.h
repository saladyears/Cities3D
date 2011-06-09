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
#include "ImageStrip.h"
#include "DropTarget.h"
#include "DropSource.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//
class IResourceLogic;

//---------------------------------------------------------------------------//
// Class: wxInteractiveImageStrip
// 
// A <wxImageStrip> that responds to mouse clicks and button presses, tied
// to some kind of logic that handles those clicks or presses.
//
// Derived From:
//     <wxImageStrip>
//     <wxDropTarget>
//     <wxDropSource>
//
// Project:
//     <Cities3D>
//
// Include:
//     InteractiveImageStrip.h
//
class UI_EXPORT_SPEC wxInteractiveImageStrip 
	: public wxImageStrip
	, public DropTarget<HashString>
	, public DropSource<HashString>
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Virtual functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnDragOver
	//
	// Handles a drag event over the drop target.
	//
	// Parameters:
	//     x - The x coordinate of the drag position.
	//     y - The y coordinate of the drag position.
	//     data - The data that may be dropped.
	//
	// Returns:
	//     A bool that is true if the drag operation is supported by the 
	//     target and false if not.
	//
	virtual bool OnDragOver(wxCoord x, wxCoord y, const HashString& data);

	//-----------------------------------------------------------------------//
	// Function: OnData
	//
	// Handles a drop event.
	//
	// Parameters:
	//     x - The x coordinate of the drop position.
	//     y - The y coordinate of the drop position.
	//     data - The data being dropped.
	//
	virtual void OnData(wxCoord x, wxCoord y, const HashString& data);

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxInteractiveImageStrip
	//
	// The wxInteractiveImageStrip constructor.
	//
	// Parameters:
	//     parent - The parent window (must not be NULL).
	//     id - The id for message handling.
	//     maxImages - The maximum number of images in the image strip.
	//     minTextHeight - The minimum text height.
	//     logic - The <IResourceLogic> handling this strip.
	//     key - The image key to use in the <ImageEngine>.
	//     sizeImage - An image that gives the default size for images in the
	//                 strip.
	//
	wxInteractiveImageStrip(wxWindow* parent, wxWindowID id, 
		const size_t maxImages,	const size_t minTextHeight, 
		IResourceLogic *logic, const HashString& key = shResources,
		const HashString& sizeImage = shTimber);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxInteractiveImageStrip
	//
	// The wxInteractiveImageStrip destructor.
	//
	virtual ~wxInteractiveImageStrip() {}

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Event Message Handling
	//

	//-----------------------------------------------------------------------//
	// Function: OnMouseMove
	//
	// Forwards mouse move events to the parent.
	//
	// Parameters:
	//     event - The mouse event.
	//
	virtual void OnMouseMove(wxMouseEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnLeftDown
	//
	// Handles the left mouse down event.
	// 
	// Parameters:
	//     event - The mouse event.
	//
	virtual void OnLeftDown(wxMouseEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnLeftUp
	//
	// Handles the left mouse up event.
	// 
	// Parameters:
	//     event - The mouse event.
	//
	virtual void OnLeftUp(wxMouseEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnRightUp
	//
	// Handles the right mouse up event.
	// 
	// Parameters:
	//     event - The mouse event.
	//
	virtual void OnRightUp(wxMouseEvent &event);

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Pointer: mpIResourceLogic
	//
	// A pointer to the <IResourceLogic> that handles this strip.
	//
	IResourceLogic *mpResourceLogic;

	wxPoint mLeftPosition;
};

//---------------------------- PROTOTYPES       -----------------------------//

