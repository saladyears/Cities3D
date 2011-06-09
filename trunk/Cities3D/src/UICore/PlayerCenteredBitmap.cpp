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
#include "PlayerCenteredBitmap.h"
#include "SkinManager.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
wxPlayerCenteredBitmap::wxPlayerCenteredBitmap(wxWindow* parent, 
	const wxSize& size, const wxSize& bitmapSize, ColorType color, 
	bool display)
: wxCenteredBitmap(parent, size, bitmapSize, display)
, mColor(color)
{
}

void wxPlayerCenteredBitmap::SetPlayerColor(ColorType color)
{
	mColor = color;
	Update();
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
wxBitmap wxPlayerCenteredBitmap::GetBitmap(const wxSize& size)
{
	wxBitmap bitmap = ColorInfoData::UIImage(mColor, size);
	bitmap.SetMask(new wxMask(bitmap, 
				SKIN.Element(shIconTransparent)));

	return bitmap;
}
