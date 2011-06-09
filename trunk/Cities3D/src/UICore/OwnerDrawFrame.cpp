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
#include "OwnerDrawFrame.h"

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//
wxOwnerDrawFrame::wxOwnerDrawFrame(wxWindow *parent, const wxString &title, 
								   const wxPoint &pos, const wxSize &size, 
								   const wxInt32 style)
: wxFrame(parent, -1, title, pos, size, wxCLIP_CHILDREN | wxNO_BORDER | style)
{
}

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxOwnerDrawFrame, wxFrame)
	EVT_PAINT(wxOwnerDrawFrame::OnPaint)
	EVT_ERASE_BACKGROUND(wxOwnerDrawFrame::OnBackground)
	EVT_SIZE(wxOwnerDrawFrame::OnSize)
	#ifdef __WXGTK__
	EVT_WINDOW_CREATE(wxOwnerDrawFrame::OnWindowCreate)
	#endif
END_EVENT_TABLE()

void wxOwnerDrawFrame::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	wxASSERT(TRUE == dc.Ok());

	//avoid flicker
	wxMemoryDC memDC;
	memDC.SelectObject(mMemBitmap);

	memDC.SetBackground(*wxBLACK);
	memDC.Clear();

	Paint(memDC);

	dc.Blit(0, 0, mMemBitmap.GetWidth(), mMemBitmap.GetHeight(), &memDC, 0, 0);

	event.Skip();
}

void wxOwnerDrawFrame::OnBackground(wxEraseEvent&)
{
	//do nothing and prevent flicker
}

void wxOwnerDrawFrame::OnSize(wxSizeEvent &event)
{
	mMemBitmap.Create(event.GetSize().GetWidth(), event.GetSize().GetHeight());

	event.Skip();

	Size(event);
}

#ifdef __WXGTK__
void wxOwnerDrawFrame::OnWindowCreate(wxWindowCreateEvent& event)
{
	WindowCreate(event);
}
#endif

