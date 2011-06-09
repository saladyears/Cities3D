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

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: GLMaterial
// 
// A class that wraps all of the parameters for specifying
// material properties in an OpenGL context.  
//
// Project:
//     <GLCore>
//
// Include:
//     GLMaterial.h
//
class GL_EXPORT_SPEC GLMaterial
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: GLMaterial
	// 
	// The default GLMaterial constructor.
	//
	GLMaterial();

	//-----------------------------------------------------------------------//
	// Constructor: GLMaterial
	// 
	// The wxColour GLMaterial constructor.
	// 
	// Parameters:
	//     color - The color the material should be initialized to.
	//
	GLMaterial(wxColour color);

	//-----------------------------------------------------------------------//
	// Group: Enumerations
	//

	//-----------------------------------------------------------------------//
	// Enum: Property
	//
	// The different kinds of properties that can be set.
	// A user can specify the following properties for
	// the material: *Ambient*, *Diffuse*, *Emissive*, *Specular*.
	//
	enum Property
	{
		Ambient		= 0,
		Diffuse		,
		Emissive	,
		Specular	,
	};

	//-----------------------------------------------------------------------//
	// Group: Functions
	//
	
	//-----------------------------------------------------------------------//
	// Function: SetProperty
	//
	// Sets the color values of a certain property of the material.
	// 
	// Parameters:
	//     property - The material property being set.
	//     color - The value the ambient color should be set to.
	//
	void SetProperty(Property property, wxColour color);

	//-----------------------------------------------------------------------//
	// Function: SetMaterial
	//
	// Sets both the ambient and diffuse properties of the material.
	// 
	// Parameters:
	//     color - The value the ambient and diffuse colors
	//             should be set to.
	//
	void SetMaterial(wxColour color);

	//-----------------------------------------------------------------------//
	// Function: MultProperty
	//
	// Multiplies the specified property by a certain value.  Useful for
	// dimming or brightening a material.
	// 
	// Parameters:
	//     property - The material property being multiplied.
	//     val - The value by which the property should be multipled.
	//
	void MultProperty(Property property, GLfloat val);

	//-----------------------------------------------------------------------//
	// Function: UseMaterial
	//
	// Calls OpenGL directly and sets the current material
	// properties to this material.
	// 
	void UseMaterial() const;
		
	//-----------------------------------------------------------------------//
	// Section: Private
	//
private: 
	struct GLColor
	{
		GLColor() : r(0.0), g(0.0), b(0.0), a(1.0) {}
		GLColor(GLfloat c1, GLfloat c2, GLfloat c3, GLfloat c4)
			: r(c1), g(c2), b(c3), a(c4) {}
		GLfloat r;
		GLfloat g;
		GLfloat b;
		GLfloat a;
	};

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Object: mAmbient
	//
	// The ambient color.
	//
	GLColor mAmbient;

	//-----------------------------------------------------------------------//
	// Object: mDiffuse
	//
	// The diffuse color.
	//
	GLColor mDiffuse;

	//-----------------------------------------------------------------------//
	// Object: mEmissive
	//
	// The emissive color.
	//
	GLColor mEmissive;

	//-----------------------------------------------------------------------//
	// Object: mSpecular
	//
	// The specular color.
	//
	GLColor mSpecular;

	//-----------------------------------------------------------------------//
	// Float: mShininess
	//
	// The shininess value.
	//
	GLfloat mShininess;
};

//---------------------------- PROTOTYPES       -----------------------------//
