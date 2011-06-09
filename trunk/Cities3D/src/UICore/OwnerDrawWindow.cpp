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
#include "OwnerDrawWindow.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//
wxOwnerDrawWindow::wxOwnerDrawWindow(wxWindow* parent, wxWindowID id, 
	const wxPoint &pos, const wxSize& size, long style)
: wxWindow(parent, id, pos, size, style)
{
	//flicker-free drawing (has to change on size events for resizable UI 
	//elements)
	mMemBitmap.Create(GetRect().GetWidth(), GetRect().GetHeight());
}

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxOwnerDrawWindow, wxWindow)
	EVT_ERASE_BACKGROUND(wxOwnerDrawWindow::OnBackground)
	EVT_SIZE(wxOwnerDrawWindow::OnSize)
	EVT_PAINT(wxOwnerDrawWindow::OnPaint)
END_EVENT_TABLE()

void wxOwnerDrawWindow::OnBackground(wxEraseEvent&)
{
	//do nothing and prevent flicker
}

void wxOwnerDrawWindow::OnSize(wxSizeEvent &event)
{
	mMemBitmap.Create(event.GetSize().x, event.GetSize().y);

	Size(event);

	event.Skip();
}

void wxOwnerDrawWindow::OnPaint(wxPaintEvent &)
{
	wxPaintDC dc(this);
	wxASSERT(TRUE == dc.Ok());

	//avoid flicker
	wxMemoryDC memDC;
	memDC.SelectObject(mMemBitmap);

	Paint(memDC);

	dc.Blit(0, 0, mMemBitmap.GetWidth(), mMemBitmap.GetHeight(), &memDC, 0, 0);
}
