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
#include "FittedStaticText.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
wxFittedStaticText::wxFittedStaticText(wxWindow* parent, wxWindowID id, 
	const wxString &label, const HashString &color, const HashString &font, 
	const wxSize &size, long style)
: wxBaseStaticText(parent, id, label, color, font, size, style)
{
	// We should never have a default size for this control.
	wxASSERT(wxDefaultSize != size);

	wxClientDC dc(this);
	dc.SetFont(GetFont());

	wxInt32 w, h;
	dc.GetTextExtent(label, &w, &h);

	// Determine how many lines will be required (with a 3% buffer).
	wxInt32 totalWidth = static_cast<wxInt32>(w * 1.03);

	size_t lines = static_cast<size_t>(ceilf(static_cast<float>(totalWidth) / 
		static_cast<float>(size.x)));
	
	// Calculate necessary height.
	wxInt32 height = (lines * h) + (lines - 1);

	wxSize thisSize = wxSize(size.x, height);
	
	SetMinSize(thisSize);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
