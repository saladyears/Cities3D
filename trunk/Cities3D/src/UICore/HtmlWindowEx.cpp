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
#include "HtmlWindowEx.h"
#include "Controller.h"
#include "GameEngine.h"
#include "IKeyMapEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
void wxHtmlWindowEx::AddText(const wxString& text)
{
	Freeze();
	AppendToPage(text);

	ScrollNice();

	Thaw();
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxHtmlWindowEx, wxHtmlWindow)
	EVT_MOUSEWHEEL(wxHtmlWindowEx::OnMouseWheel)
	EVT_KEY_DOWN(wxHtmlWindowEx::OnKeyDown)
	EVT_SIZE(wxHtmlWindowEx::OnSize)
END_EVENT_TABLE()

void wxHtmlWindowEx::ScrollNice()
{
	// Scroll to the bottom.
	wxInt32 xunit, yunit;
	GetScrollPixelsPerUnit(&xunit, &yunit);

	wxInt32 x, y;
	GetVirtualSize(&x, &y);

	if(0 != yunit)
	{
		wxSize size = GetSize();
		wxInt32 w = size.x;
		wxInt32 h = size.y;

		// Calculate the number of total scroll units if everyhing was visible.
		int units = static_cast<int>(ceilf((static_cast<float>(y + (yunit / 2)) / 
			static_cast<float>(yunit))));

		// Subtract the height of any horizontal scroll window.
		if(x > w)
		{
			h -= yunit;
		}

		// Now calculate how much of the screen is actually visible (in units)
		int visible = h / yunit;

		Scroll(-1, units - visible);
	}
}

void wxHtmlWindowEx::OnMouseWheel(wxMouseEvent &event)
{
	wxHtmlWindow::HandleOnMouseWheel(event);
}

void wxHtmlWindowEx::OnKeyDown(wxKeyEvent &event)
{
	KEYMAP.ProcessKeyEvent(event);
}

void wxHtmlWindowEx::OnSize(wxSizeEvent &event)
{
	// Store off current scroll location because wxHtmlWindow is dumb.
	wxInt32 x, y;
	GetViewStart(&x, &y);

	wxHtmlWindow::OnSize(event);

	ScrollNice();

	event.Skip(false);
}
