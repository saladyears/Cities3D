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
#include "PlayerUIElement.h"
#include "Controller.h"
#include "ImageEngine.h"
#include "Image.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
PlayerUIElement::PlayerUIElement(wxWindow *parent, 
	const PlayerUpdateFunc &update, const PlayerColorFunc &color, bool right, 
	wxInt32 threshold, const HashString &imageKey, const HashString &imageName,
	const wxString& text, bool card, wxInt32 index)
: PlayerElement(parent, update, color, right, threshold, text, card, index)
, mImageKey(imageKey)
, mImageName(imageName)
{
	Controller::get().AddReceiver(shEventImageUpdate, 
		&PlayerUIElement::OnImageUpdate, this);
}

PlayerUIElement::~PlayerUIElement()
{
	Controller::get().RemoveReceiver(shEventImageUpdate, 
		&PlayerUIElement::OnImageUpdate, this);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
wxBitmap PlayerUIElement::GetBitmap(const wxSize &size)
{
	wxImage image = IMAGE.GetImage(mImageKey, mImageName, size);

	return wxBitmap(image);
}

void PlayerUIElement::OnImageUpdate(bool)
{
	IMAGE.IgnoreCache();
	ResetBitmap();
}
