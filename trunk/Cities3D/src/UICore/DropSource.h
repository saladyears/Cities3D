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
#include "DropTarget.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: DropSource
//
// Mixin base class for controls that want to start drag and drop operations.
// 
// Project:
//     <Ciies3D>
//
// Include:
//    DropSource.h
//
template <typename T>
class DropSource
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: BeginDrag
	//
	// Begins the drag and drop with the given data.
	//
	// Parameters:
	//     data - The data to drag.
	//
	void BeginDrag(const T& data)
	{
		EndDrag();

		mData = data;
		mInDrag = true;
		mThis->CaptureMouse();
	}

	//-----------------------------------------------------------------------//
	// Function: DoDrag
	//
	// Searches all available <DropTargets> to see if the drag is over
	// one.
	//
	// Parameters:
	//     x - The x coordinate of the mouse in client coords.
	//     y - The y coordinate of the moues in client coords.
	//
	void DoDrag(const wxCoord x, const wxCoord y)
	{
		if(true == mInDrag)
		{
			// Convert the coords to screen coords.
			wxPoint pos = mThis->ClientToScreen(wxPoint(x, y));

			DropTarget<T>* const target = FindDropTarget(pos);
			if( (NULL != target) &&
				(true == target->OnDragOver(pos.x, pos.y, mData)))
			{
				wxSetCursor(wxCursor(wxCURSOR_RIGHT_ARROW));
			}
			else
			{
				wxSetCursor(wxCursor(wxCURSOR_NO_ENTRY));
			}
		}
	}

	//-----------------------------------------------------------------------//
	// Function: DoDrop
	//
	// Drops the stored <Player> on any <DropTarget> at the given coords.
	// Calls <EndDrag> to clean up.
	//
	// Parameters:
	//     x - The x coordinate of the mouse in client coords.
	//     y - The y coordinate of the moues in client coords.
	//
	void DoDrop(const wxCoord x, const wxCoord y)
	{
		if(true == mInDrag)
		{
			// Convert the coords to screen coords.
			wxPoint pos = mThis->ClientToScreen(wxPoint(x, y));

			DropTarget<T> * const target = FindDropTarget(pos);
			if( (NULL != target) &&
				(true == target->OnDragOver(pos.x, pos.y, mData)))
			{
				target->OnData(pos.x, pos.y, mData);
			}

			EndDrag();
		}
	}

	//-----------------------------------------------------------------------//
	// Function: EndDrag
	//
	// Ends the drag and drop operation.
	//
	void EndDrag()
	{
		if(true == mInDrag)
		{
			mInDrag = false;
			mThis->ReleaseMouse();
		}
	}

	//-----------------------------------------------------------------------//
	// Function: InDrag
	//
	// Returns whether there is a drag operation happening.
	//
	// Returns:
	//     A bool that is true if there is a drag operation happening and false
	//     if not.
	//
	bool InDrag() const
	{
		return mInDrag;
	}

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: DropSource
	//
	// The DropSource constructor.
	//
	DropSource(wxWindow *thisWindow)
	: mThis(thisWindow)
	, mInDrag(false)
	{
		wxASSERT(NULL != mThis);
	}

	//-----------------------------------------------------------------------//
	// Group: Destructors
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~DropSource
	//
	// The DropSource destructor.
	//
	virtual ~DropSource() {}

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: FindDropTarget
	//
	// Finds any registered <DropTarget> at the given screen point.
	//
	// Returns:
	//     A pointer to any <DropTarget> at the given point.  May be 
	//     NULL.
	//
	DropTarget<T>* const FindDropTarget(wxPoint &pos)
	{
		DropTarget<T>* target = NULL;

		// Go through each registered DropTarget and see if the point
		// is over it.
		typename DropRegistry<T>::DropTargetSet::const_iterator it, 
			itEnd = DropRegistry<T>::msDropTargets.end();
		for(it = DropRegistry<T>::msDropTargets.begin(); it != itEnd; ++it)
		{
			DropTarget<T>* thisTarget = (*it);

			// Convert the current position to each client position and 
			// compare.
			wxPoint clientPos;

			if(true == thisTarget->ScreenToClient(pos, clientPos))
			{
				target = thisTarget;
				pos = clientPos;
				break;
			}
		}

		return target;
	}

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Bool: mInDrag
	//
	// A flag that is true if the drop source is in a drag operation, and false
	// if not.
	//
	bool mInDrag;

	//-----------------------------------------------------------------------//
	// Object: mData
	//
	// The current data in a drag and drop operation.
	//
	T mData;

	//-----------------------------------------------------------------------//
	// Pointer: mThis
	//
	// A pointer to the window this DropSource is handling.
	//
	wxWindow *mThis;
};

//---------------------------- PROTOTYPES       -----------------------------//
