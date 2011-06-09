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
#include "BaseStaticText.h"
#include "SkinManager.h"
#include "Controller.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
wxBaseStaticText::wxBaseStaticText(wxWindow* parent, wxWindowID id, 
								   const wxString &label, 
								   const HashString &color, 
								   const HashString &font,
								   const wxSize& size, 
								   long style)
: wxStaticText(parent, id, swEmpty, wxDefaultPosition, size, style)
, mColor(color)
{
	SetFont(SKIN.Font(font));
	OnSkin();

	SetLabel(label);

	Controller::get().AddReceiver(shEventSkin, &wxBaseStaticText::OnSkin, 
		this);
}

wxBaseStaticText::~wxBaseStaticText()
{
	Controller::get().RemoveReceiver(shEventSkin, &wxBaseStaticText::OnSkin, 
		this);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void wxBaseStaticText::OnSkin(wxInt32)
{
	SetBackgroundColour(SKIN.Element(shBackground));
	SetForegroundColour(SKIN.Element(mColor));
    Refresh();
}
