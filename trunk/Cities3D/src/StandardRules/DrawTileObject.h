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
class TileObject;
class GLMaterial;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: DrawTileObject
// 
// Needs documentation.
//
// Derived From:
//     <DrawObject>
//
// Project:
//     <StandardRules>
//
// Include:
//     DrawTileObject.h
//
class DrawTileObject : public DrawObject
{
	DECLARE_DRAW_OBJECT(DrawTileObject, TileObject)

public:
	DrawTileObject(const GameObjectPtr &object);
	virtual ~DrawTileObject();

	//-----------------------------------------------------------------------//
	// Function: CanSelect
	//
	// Returns whether the TileObject for this DrawTileObject
	// can be selected.
	//
	// Parameters:
	//     origin - The origin point of the selection ray.
	//     ray - The selection ray direction.
	//     distance - Filled with the outgoing distance the object is
	//                from the ray origin, if the object is selectable.
	//     id - Filled with the ougoing id of the selectable <IGameObject>,
	//          if the object is selectable.
	//
	// Returns:
	//     A bool that is true if the object can be selected and false
	//     if not.
	//
	virtual bool CanSelect(const Vector &origin, const Vector &ray, 
		float &distance, wxInt32 &id, bool popup = false);

	virtual void Obtain();
	virtual void Release();

	virtual void PopupFunction(wxString& name, wxString& owner, 
		wxBitmap& bitmap, bool &createBitmap, ColorType& color);

	virtual void RenderOutline() const;
	virtual void RenderSelection() const;
	virtual void RenderModel(const wxInt32 selection) const;
};

//---------------------------- PROTOTYPES       -----------------------------//
