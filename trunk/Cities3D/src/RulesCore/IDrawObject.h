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
#include "RulesExport.h"
#include "ColorInfoData.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class IGameObject;
typedef boost::shared_ptr<IGameObject> GameObjectPtr;

struct Vector;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: IDrawObject
// 
// The interface for objects that are drawn on screen.  The IDrawObject
// class is an implementation of the Prototype design pattern.  When
// a <RuleSet> is loaded, all of its associated IDrawObjects are
// stored in the <IDrawObjectEngine>.  When a new <IGameObject> is
// added to the game, the <IDrawObjectEngine> is queried for any
// <IDrawObject> capable of drawing it.  If one is found, that <IDrawObject>
// is cloned and given the <IGameObject> to draw.
//
// Project:
//     <RulesCore>
//
// Include:
//     IDrawObject.h
//
class RULES_EXPORT_SPEC IDrawObject
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~IDrawObject
	// 
	// The DrawObject destructor.  It is pure virtual because
	// this class must be derived from in order to be used.
	// 
	virtual ~IDrawObject()=0;

	//-----------------------------------------------------------------------//
	// Group: Pure Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: GetObject
	//
	// Returns the <IGameObject> this IDrawObject references.
	//
	// Returns:
	//     A pointer to the <IiGameObject>.
	//
	virtual IGameObject *GetObject() const=0;

	//-----------------------------------------------------------------------//
	// Function: CanBuild
	//
	// Queries the IDrawObject to see if they can draw a certain
	// <IGameObject>.
	//
	// Parameters:
	//     object - The GameObject this IDrawObject is being asked if it can 
	//              build.
	//
	// Returns:
	//     A bool indicating whether the IDrawObject can build the given
	//     <IGameObject>.  The value is true if it can, false if not.
	//
	virtual bool CanBuild(const GameObjectPtr &object) const=0;
	
	//-----------------------------------------------------------------------//
	// Function: CanDelete
	//
	// Returns whether this IDrawObject can be deleted.
	//
	// Returns:
	//     A bool that is true if the object can be deleted and false
	//     if not.
	//
	virtual bool CanDelete() const=0;

	//-----------------------------------------------------------------------//
	// Function: CanSelect
	//
	// Returns whether this IDrawObject can be selected.
	//
	// Parameters:
	//     origin - The origin point of the selection ray.
	//     ray - The selection ray direction.
	//     distance - Filled with the outgoing distance the object is
	//                from the ray origin, if the object is selectable.
	//     id - Filled with the ougoing id of the selectable <IGameObject>,
	//          if the object is selectable.
	//     popup - A bool that is true if the object is being tested for
	//             a popup.
	//
	// Returns:
	//     A bool that is true if the object can be selected and false
	//     if not.
	//
	virtual bool CanSelect(const Vector &origin, const Vector &ray, 
		float &distance, wxInt32 &id, bool popup = false)=0;

	//-----------------------------------------------------------------------//
	// Function: Clone
	//
	// Creates a clone of the IDrawObject.  IDrawObjects are an implementation
	// of the Prototype pattern and must provide a way to clone themselves from
	// a stored prototype.
	//
	// Parameters:
	//     pObject - The GameObject the cloned DrawObject will reference.
	//
	virtual IDrawObject *Clone(const GameObjectPtr &pObject) const=0;

	//-----------------------------------------------------------------------//
	// Function: GetPriority
	//
	// Returns the draw priority of this object.
	//
	// Returns:
	//     The draw priority.  The lower the number, the
	//     earlier in the list this object will be drawn.
	//
	virtual wxInt32 GetPriority() const=0;

	//-----------------------------------------------------------------------//
	// Function: Render
	//
	// Draws the object on the screen.
	//
	// Parameters:
	//     selection - The id of the currently selected object.
	//                 If it matches the id of this object, it should be 
	//                 rendered with a selection outline.
	//
	virtual void Render(const wxInt32 selection) const=0;

	//-----------------------------------------------------------------------//
	// Function: Obtain
	//
	// Allows derived classes to do their own resource management. 
	// They must override this function to do any special OpenGL texture 
	// or display list initialization.  It is called once per Cities3D game.
	//
	virtual void Obtain()=0;

	//-----------------------------------------------------------------------//
	// Function: Release
	//
	// Allows derived classes to do clean up after themselves.  It is 
	// called once per Cities3D game.
	//
	virtual void Release()=0;

	//-----------------------------------------------------------------------//
	// Function: PopupFunction
	//
	// Provides information about how the popup (if any) should be created
	// for this IDrawObject.
	//
	// Parameters:
	//     name - [out] The name of the object.
	//     owner - [out] The name of the owner of the object.
	//     bitmap - [out] The bitmap associated with the object (if any).
	//     createBitmap - [out] A bool that is true if a bitmap should be
	//                    created for the object and false if not.
	//     color - [out] - Any color associated with the popup.
	//
	virtual void PopupFunction(wxString& name, wxString& owner, 
		wxBitmap& bitmap, bool &createBitmap, ColorType& color)=0;
};

typedef boost::shared_ptr<IDrawObject> DrawObjectPtr;

//---------------------------- PROTOTYPES       -----------------------------//
inline IDrawObject::~IDrawObject() {}
