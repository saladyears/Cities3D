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
#include "GLExport.h"
#include "IDrawObject.h"
#include "ColorInfoData.h"

//---------------------------- TYPEDEFS         -----------------------------//
class IGameObject;
typedef boost::shared_ptr<IGameObject> GameObjectPtr;

class GLMaterial;

typedef boost::scoped_array<GLuint> GLuintArray;

//---------------------------- DEFINES          -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: DrawObject
// 
// A class that provides a default implementation of IDrawObject.
// It prepares the way for an object to be rendered using OpenGL, 
// providing pure virtual private functions that must be overridden by 
// derived classes when the actual model data needs to be rendered.
//
// Derived From:
//     <IDrawObject>
//
// Project:
//     <GLCore>
//
// Include:
//     DrawObject.h
//
class GL_EXPORT_SPEC DrawObject : public IDrawObject
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: DrawObject
	// 
	// The DrawObject constructor.
	// 
	// Parameters:
	//     pObject - The GameObject that this DrawObject is rendering.
	//     priority - The render priority for this DrawObject.  Lower means
	//                it draws earlier.
	DrawObject(const GameObjectPtr &pObject, const wxInt32 priority = 99);

	//-----------------------------------------------------------------------//
	// Group: Enumerations
	//
	
	//-----------------------------------------------------------------------//
	// Enum: Material
	// 
	// Provides a standard list of colors that can be used
	// by derived objects.  The standard colors are *Gray*,
	// *White*, *Gold*, *Red*, *Edge* (the tile edge color), *Black*,  
	// *Lava*.
	// 
	// There are also three colors that can be set by the user,
	// enumerated as: *Selection*, *WhiteOutline*, and *LongestOutline*.
	//
	enum Material
	{
		Gray					= 0,
		White					,
		Gold					,
		Red						,
		Black					,
		
		//colors that can be set the user
		UserBegin				,
		Selection				,
		WhiteOutline			,
		LongestOutline			,
		UserEnd					,

		MaterialSize			,
		MaterialDword			= 0x7fffffff
	};

	//-----------------------------------------------------------------------//
	// Group: Static Functions
	//

	//-----------------------------------------------------------------------//
	// Function: material
	//
	// Returns a material that matches the passed in 
	// Material enumeration type.
	//
	// Parameters:
	//     type - The material type.
	// 
	// Returns:
	//     A const GLMaterial that can be used to
	//     set the material properties in OpenGL.
	//
	static const GLMaterial &material(Material type);

	//-----------------------------------------------------------------------//
	// Function: material
	//
	// Returns a material that matches the passed in <IGameObject> color.
	// 
	//
	// Parameters:
	//     object - The <IGameObject> to create the material with.
	// 
	// Returns:
	//     A const GLMaterial that can be used to set the material properties 
	//     in OpenGL.
	//
	static const GLMaterial &material(IGameObject *object);

	//-----------------------------------------------------------------------//
	// Function: material
	//
	// Returns a material that matches the passed in ColorType definition.
	//
	// Parameters:
	//     color - The color to create the material with.
	// 
	// Returns:
	//     A const GLMaterial that can be used to set the material properties 
	//     in OpenGL.
	//
	static const GLMaterial &material(ColorType color);

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: GetObject
	//
	// Returns the GameObject this DrawObject references.
	//
	// Returns:
	//     A pointer to the GameObject.  The pointer may be NULL.
	//
	IGameObject *GetObject() const;

	//-----------------------------------------------------------------------//
	// Function: CanDelete
	//
	// Returns whether this DrawObject can be deleted.
	//
	// Returns:
	//     A bool that is true if the object can be deleted and false
	//     if not.  An object can be deleted when the GameObject it references
	//     has been destroyed.
	//
	virtual bool CanDelete() const {return mDelete;}

	//-----------------------------------------------------------------------//
	// Function: GetPriority
	//
	// Returns the draw priority of this object.
	//
	// Returns:
	//     The draw priority.  The lower the number, the
	//     earlier in the list this object will be drawn.
	//
	virtual wxInt32 GetPriority() const {return mPriority;}

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
		float &distance, wxInt32 &id, bool popup = false);

	//-----------------------------------------------------------------------//
	// Function: Render
	//
	// Draws the object on the screen using OpenGL.
	//
	// Parameters:
	//     selection - The OpenGL id of the currently selected object.
	//                 If it matches the id of this object, it should be 
	//                 rendered with a selection outline.
	//
	virtual void Render(const wxInt32 selection) const;

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
	virtual void PopupFunction(wxString&, wxString&, wxBitmap&, bool &, 
		ColorType&) {}

	//-----------------------------------------------------------------------//
	// Group: Pure Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: RenderModel
	//
	// Allows derived classes to render their model in the OpenGL canvas.
	//
	// Parameters:
	//     selection - The OpenGL id of the currently selected object.
	//
	virtual void RenderModel(const wxInt32 selection) const=0;

	//-----------------------------------------------------------------------//
	// Function: RenderOutline
	//
	// Allows derived classes to draw their model with an outline around it.
	//
	virtual void RenderOutline() const=0;

	//-----------------------------------------------------------------------//
	// Function: RenderSelection
	//
	// Allows derived classes to draw their model with a selection outline 
	// around it.
	//
	virtual void RenderSelection() const=0;

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Bool: mDelete
	//
	// Determines if this draw object be deleted.  It
	// is set to true when the GameObject this DrawObject
	// is drawing is destroyed.
	//
	mutable bool mDelete;

	//-----------------------------------------------------------------------//
	// Object: mpObject
	//
	// A weak pointer to the GameObject this DrawObject
	// draws.  It is a weak pointer because the DrawObject
	// should have no ownership of the GameObject, but simply
	// draw it on the screen, and disappear if the GameObject
	// is destroyed.
	//
	const boost::weak_ptr<IGameObject> mpObject;

	//-----------------------------------------------------------------------//
	// Int: mPriority
	//
	// The draw priority for this object.
	//
	wxInt32 mPriority;
};

//---------------------------- PROTOTYPES       -----------------------------//
