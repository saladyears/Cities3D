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
#include "CenteredBitmap.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	wxColour sBackground_CenteredBitmap;
}

//---------------------------- PUBLIC           -----------------------------//
wxCenteredBitmap::wxCenteredBitmap(wxWindow* parent, const wxSize& size, 
	const wxSize& bitmapSize, bool display)
: wxOwnerDrawWindow(parent, -1, wxDefaultPosition, size)
, mDisplay(display)
, mOriginalSize(size)
, mOriginalBitmapSize(bitmapSize)
{
	sBackground_CenteredBitmap = parent->GetBackgroundColour();
}

//---------------------------- PROTECTED        -----------------------------//
void wxCenteredBitmap::Update()
{
	mDisplay = true;

	wxSizeEvent event(GetSize());
	Size(event);
}

//---------------------------- PRIVATE          -----------------------------//
void wxCenteredBitmap::Size(wxSizeEvent &event)
{
	wxSize size = event.GetSize();

	// Determine the smallest delta of change on the x or y axis due to the
	// resize.
	float xRatio = static_cast<float>(size.x) /
		static_cast<float>(mOriginalSize.x);
	float yRatio = static_cast<float>(size.y) /
		static_cast<float>(mOriginalSize.y);

	float ratio = std::min(xRatio, yRatio);

	// Apply that ratio to the bitmap size.
	wxSize bitmapSize = wxSize(static_cast<wxInt32>(mOriginalBitmapSize.x * ratio),
		static_cast<wxInt32>(mOriginalBitmapSize.y * ratio));

	if(wxSize(0, 0) != bitmapSize)
	{
		mDrawBitmap = GetBitmap(bitmapSize);
		Refresh();
	}
}

void wxCenteredBitmap::Paint(wxMemoryDC &dc)
{
	wxSize size = GetSize();
	wxSize bitmapSize = wxSize(mDrawBitmap.GetWidth(), 
		mDrawBitmap.GetHeight());

	dc.SetBrush(wxBrush(sBackground_CenteredBitmap));
	dc.SetPen(wxPen(sBackground_CenteredBitmap));
	dc.DrawRectangle(0, 0, size.x, size.y);

	// Draw the bitmap centered.
	wxInt32 x = (size.x - bitmapSize.x) / 2;
	wxInt32 y = (size.y - bitmapSize.y) / 2;

	wxASSERT(0 <= x);
	wxASSERT(0 <= y);

	if(true == mDisplay)
	{
		dc.DrawBitmap(mDrawBitmap, x, y, true);
	}
}
