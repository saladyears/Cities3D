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
#include "stdwx.h"
#include "style.h"	//READ THIS BEFORE MAKING ANY CHANGES TO THIS FILE!!!

//---------------------------- SYSTEM INCLUDES  -----------------------------//

//---------------------------- USER INCLUDES    -----------------------------//
#include "InteractiveImageStrip.h"
#include "IResourceLogic.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
bool wxInteractiveImageStrip::OnDragOver(wxCoord x, wxCoord y,
	const HashString& data)
{
	bool valid = !InDrag();

	if(true == valid)
	{
		valid = mpResourceLogic->AllowDrop(this, x, y, data);
	}

	return valid;
}

void wxInteractiveImageStrip::OnData(wxCoord x, wxCoord y, 
	const HashString& data)
{
	// Let the logic deal with it.
	mpResourceLogic->ExecuteDrop(this, x, y, data);
}

//---------------------------- PROTECTED        -----------------------------//
#ifdef __WIN32__
	//disables the 'this' : used in base member initializer list warning
	#pragma warning(disable:4355)
#endif
wxInteractiveImageStrip::wxInteractiveImageStrip(wxWindow* parent, 
	wxWindowID id, const size_t maxImages, const size_t minTextHeight, 
	IResourceLogic *logic, const HashString& key, const HashString& sizeImage)
: wxImageStrip(parent, id, maxImages, minTextHeight, key, sizeImage)
, mpResourceLogic(logic)
, DropTarget<HashString>(this)
, DropSource<HashString>(this)
, mLeftPosition(0, 0)
{
	wxASSERT(mpResourceLogic);

	SetFlags(FlagAlignCenter);
}

//---------------------------- PRIVATE          -----------------------------//
void wxInteractiveImageStrip::OnMouseMove(wxMouseEvent &event)
{
	wxImageStrip::OnMouseMove(event);

	wxPoint pos = event.GetPosition();

	if(true == InDrag())
	{
		DoDrag(pos.x, pos.y);
	}
	else
	{
		wxPoint point = pos - mLeftPosition;

		HashString res = GetImage(mLeftPosition);

		// If the left mouse button is down, and we are more than five pixels 
		// away from where we started, we are in a drag mode.
		if( (true == event.LeftIsDown()) &&
			(2 < abs(point.x)) &&
			(2 < abs(point.y)) &&
			(0 == (GetFlags() & FlagDisabled)) &&
			(false == res.empty()) &&
			(EmptyResource != res))
		{
			BeginDrag(res);
		}
		else
		{
			res = GetImage(pos);

			if( (GetFlags() & FlagDisabled) ||
				(true == res.empty()) ||
				(EmptyResource == res))
			{
				SetCursor(wxCursor(wxCURSOR_ARROW));
			}
			else
			{
				SetCursor(wxCursor(wxCURSOR_HAND));
			}
		}
	}
}

void wxInteractiveImageStrip::OnLeftDown(wxMouseEvent &event)
{
	wxImageStrip::OnLeftDown(event);

	// See if we're on a resource.
	mLeftPosition = event.GetPosition();
}

void wxInteractiveImageStrip::OnLeftUp(wxMouseEvent &event)
{
	wxImageStrip::OnLeftUp(event);

	wxPoint pos = event.GetPosition();

	if(true == InDrag())
	{
		DoDrop(pos.x, pos.y);
	}
	else
	{
		HashString res = GetImage(pos);

		if( (0 == (GetFlags() & FlagDisabled)) &&
			(false == res.empty()) &&
			(EmptyResource != res))
		{
			// Shift-clicks are handled as right mouse clicks.
			if(event.ShiftDown())
			{
				mpResourceLogic->HandleRightClick(this, res);
			}
			else
			{
				mpResourceLogic->HandleLeftClick(this, res);
			}
		}
	}
}

void wxInteractiveImageStrip::OnRightUp(wxMouseEvent &event)
{
	wxImageStrip::OnRightUp(event);

	wxPoint pos = event.GetPosition();

	HashString res = GetImage(pos);

	if( (0 == (GetFlags() & FlagDisabled)) &&
		(false == res.empty()) &&
		(EmptyResource != res))
	{
		mpResourceLogic->HandleRightClick(this, res);
	}
}
