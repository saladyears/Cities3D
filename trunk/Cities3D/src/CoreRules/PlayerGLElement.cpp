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
#include "PlayerGLElement.h"
#include "SettlersGLCanvas.h"
#include "ConfigDatabase.h"
#include "IGameObject.h"
#include "ImageEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
PlayerGLElement::PlayerGLElement(wxWindow *parent, 
	const PlayerUpdateFunc &update, const PlayerColorFunc &color, 
	const ColorType playerColor, const wxInt32 threshold, 
	const GameObjectPtr &gameObject, const DrawObjectPtr &drawObject, 
	const HashString &image, const HashString &mask, const wxString& text,
	wxSettlersGLCanvas* canvas)
: PlayerElement(parent, update, color, true, threshold, text)
, mPlayerColor(playerColor)
, mGameObject(gameObject)
, mDrawObject(drawObject)
, mImage(image)
, mMask(mask)
, mCanvas(canvas)
{
	wxASSERT(NULL != parent);
	wxASSERT(NULL != canvas);
}

void PlayerGLElement::SetPlayerColor(const ColorType color)
{
	mPlayerColor = color;
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
wxBitmap PlayerGLElement::GetBitmap(const wxSize &size)
{
	wxBitmap bitmap;

	if(TRUE == 
		CONFIG.read<wxInt32>(swConfigSectionOpenGL, swRenderBitmaps, TRUE))
	{
		//set the game object's color
		mGameObject->color(mPlayerColor);

		//use the conversion function
		Vector vector(10.0f, 10.0f, 10.0f);
		bitmap = mCanvas->ConvertGLtoWX(mDrawObject, size.GetWidth(), 
			size.GetHeight(), vector);
	}
	else
	{
		bitmap = wxBitmap(
			IMAGE.Colorize(mImage, mMask, mPlayerColor, size));
	}

	return bitmap;
}
