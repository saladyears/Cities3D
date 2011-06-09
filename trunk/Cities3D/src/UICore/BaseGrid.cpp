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
#include "BaseGrid.h"
#include "SkinManager.h"
#include "Controller.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
wxBaseGrid::wxBaseGrid(wxWindow* parent, wxWindowID id, 
	const wxPoint& pos, const wxSize& size, const HashString &color, 
	const HashString &font, const HashString &labelFont)
: wxGrid(parent, id, pos, size)
, mColor(color)
, mFont(font)
, mLabelFont(labelFont)
{
    Controller::get().AddReceiver(shEventSkin, &wxBaseGrid::OnSkin, this);

    OnSkin();
}

wxBaseGrid::~wxBaseGrid()
{
    Controller::get().RemoveReceiver(shEventSkin, &wxBaseGrid::OnSkin, this);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void wxBaseGrid::OnSkin(wxInt32)
{
    // Set the grid display up.
    SetDefaultCellBackgroundColour(SKIN.Element(shBackground));
    SetDefaultCellTextColour(SKIN.Element(mColor));
    SetDefaultCellFont(SKIN.Font(mFont));

    SetLabelBackgroundColour(SKIN.Element(shBackground));
    SetLabelTextColour(SKIN.Element(mColor));
    SetLabelFont(SKIN.Font(mLabelFont));

    SetGridLineColour(SKIN.Element(shBackground));
    SetSelectionBackground(SKIN.Element(shListHighlight));
    SetSelectionForeground(SKIN.Element(shListMainText));

    Refresh();
}

