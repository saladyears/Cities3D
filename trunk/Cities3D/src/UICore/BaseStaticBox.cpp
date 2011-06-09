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
#include "BaseStaticBox.h"
#include "Controller.h"
#include "SkinManager.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	wxInt32 sCurve = 0;
	wxColour sOutline_BaseStaticBox;
}

//---------------------------- PUBLIC           -----------------------------//
wxBaseStaticBox::wxBaseStaticBox(wxWindow* parent, wxWindowID id, 
								 const wxString& label, const wxPoint& pos, 
								 const wxSize& size, const HashString &color,
								 const HashString &font)
: wxStaticBox(parent, id, label, pos, size)
, mColor(color)
, mFont(font)
{
	Controller::get().AddReceiver(shEventSkin, &wxBaseStaticBox::OnSkin, this);
	OnSkin();
}

wxBaseStaticBox::~wxBaseStaticBox()
{
	Controller::get().RemoveReceiver(shEventSkin, &wxBaseStaticBox::OnSkin, 
			this);
}

//---------------------------- PROTECTED        -----------------------------//
#ifdef __WIN32__
void wxBaseStaticBox::PaintBackground(wxDC& dc, const struct tagRECT& rc)
{
	wxColour background = GetBackgroundColour();

	dc.SetPen(wxPen(background));
	dc.SetBrush(wxBrush(background));
	dc.DrawRectangle(0, 0, rc.right, rc.bottom);
}

void wxBaseStaticBox::PaintForeground(wxDC& dc, const struct tagRECT& rc)
{
	wxColour background = GetBackgroundColour();

	dc.SetFont(GetFont());
	dc.SetTextForeground(GetForegroundColour());
	dc.SetTextBackground(GetBackgroundColour());

	wxInt32 x, y;
	const wxString& label = GetLabel();
	dc.GetTextExtent(label, &x, &y);

	// Draw border.
	dc.SetPen(wxPen(sOutline_BaseStaticBox));
	dc.SetBrush(wxBrush(background));
	dc.DrawRoundedRectangle(0, y / 2, rc.right, rc.bottom - (y / 2), sCurve);

	// Black out area for text.
	dc.SetPen(wxPen(background));
	dc.SetBrush(wxBrush(background));
	dc.DrawRectangle(7, 0, x + 4, y);

	// Draw text.	
	dc.DrawText(label, 9, 0);

	// Draw second label if it exists.
	if(false == mSecondLabel.IsEmpty())
	{
		dc.GetTextExtent(mSecondLabel, &x, &y);

		// Black out area for text.
		dc.SetPen(wxPen(background));
		dc.SetBrush(wxBrush(background));
		dc.DrawRectangle(rc.right - x - 11, 0, x + 4, y);

		dc.DrawText(mSecondLabel, rc.right - x - 8, 0);
	}
}
#endif

//---------------------------- PRIVATE          -----------------------------//
void wxBaseStaticBox::OnSkin(wxInt32)
{
	SetBackgroundColour(SKIN.Element(shBackground));
	SetForegroundColour(SKIN.Element(mColor));
	SetFont(SKIN.Font(mFont));

	sCurve = SKIN.Size(shFrameCurve);
	sOutline_BaseStaticBox = SKIN.Element(shGameUIOutline);

	Refresh();
}


