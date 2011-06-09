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
#include "DropRegistry.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: DropTarget
// 
// A drag and drop target for data objects.
//
// Project:
//     <Cities3D>
//
// Include:
//     DataTarget.h
//
template <typename T>
class DropTarget
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:	
	//-----------------------------------------------------------------------//
	// Group: Pure Virtual functions
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
	virtual bool OnDragOver(wxCoord x, wxCoord y, const T& data)=0;

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
	virtual void OnData(wxCoord x, wxCoord y, const T& data)=0;

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: ScreenToClient
	//
	// Converts the given point from screen coordinates to this client's
	// coordinates.
	//
	// Parameters:
	//     pos - The point to convert.
	//     client - The outgoing point.
	//
	// Returns:
	//     A bool that is true if the outgoing point is inside the client's
	//     rect, and false if not.
	//
	const bool ScreenToClient(const wxPoint &pos, wxPoint &client)
	{
		client = mThis->ScreenToClient(pos);
		wxSize size = mThis->GetSize();
		
		bool ret = true;

		if( (0 > client.x) || (0 > client.y) ||
			(size.x < client.x) || (size.y < client.y))
		{
			ret = false;
		}

		return ret;
	}

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: DropTarget
	//
	// The DropTarget constructor. Registers itself as a possible drop
	// target for any <DropSource>
	//
	DropTarget(wxWindow *thisWindow)
		: mThis(thisWindow)
	{
		wxASSERT(NULL != mThis);

		DropRegistry<T>::RegisterDropTarget(this);
	}

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~DropTarget
	//
	// The DropTarget destructor.
	//
	virtual ~DropTarget()
	{
		DropRegistry<T>::UnregisterDropTarget(this);
	}

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Pointer: mThis
	//
	// A pointer to the window this DropSource is handling.
	//
	wxWindow *mThis;
};

//---------------------------- PROTOTYPES       -----------------------------//
