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
#include "DrawObject.h"
#include "RuleSetDatabase.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class ShipObject;

//---------------------------- CLASSES          -----------------------------//
class DrawShipObject : public DrawObject
{
	DECLARE_DRAW_OBJECT(DrawShipObject, ShipObject)

public:
	DrawShipObject(const GameObjectPtr &object, 
		const Material material = MaterialSize);
	virtual ~DrawShipObject();

	virtual bool CanSelect(const Vector &origin, const Vector &ray, 
		float &distance, wxInt32 &id, bool popup = false);

	virtual void Obtain();
	virtual void Release();

	virtual void PopupFunction(wxString& name, wxString& owner, 
		wxBitmap& bitmap, bool &createBitmap, ColorType& color);

	virtual void RenderOutline() const;
	virtual void RenderSelection() const;
	virtual void RenderModel(const wxInt32 selection) const;

private:
	Material mMaterial;
};

//---------------------------- PROTOTYPES       -----------------------------//
