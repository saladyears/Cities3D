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
#pragma once

#include "style.h"	//READ THIS BEFORE MAKING ANY CHANGES TO THIS FILE!!!

//---------------------------- SYSTEM INCLUDES  -----------------------------//

//---------------------------- USER INCLUDES    -----------------------------//
#include "DrawShipObject.h"
#include "RuleSetDatabase.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class PirateObject;

//---------------------------- CLASSES          -----------------------------//
class DrawPirateObject : public DrawShipObject
{
	DECLARE_DRAW_OBJECT(DrawPirateObject, PirateObject)

public:
	DrawPirateObject(const GameObjectPtr &object);

	virtual void PopupFunction(wxString& name, wxString& owner, 
		wxBitmap& bitmap, bool &createBitmap, ColorType& color);

	virtual void obtain() {}
	virtual void release() {}
};

//---------------------------- PROTOTYPES       -----------------------------//
