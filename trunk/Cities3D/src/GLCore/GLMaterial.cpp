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
#include "GLMaterial.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	template <typename T> void clamp(T &val, const T low, const T high)
	{
		if(val < low)
		{
			val = low;
		}
		else if(val > high)
		{
			val = high;
		}
	}
}

//---------------------------- PUBLIC           -----------------------------//
GLMaterial::GLMaterial()
: mShininess(0.0)
{
}

GLMaterial::GLMaterial(wxColour color)
: mShininess(0.0)
{
	SetMaterial(color);
}

void GLMaterial::SetProperty(Property property, wxColour color)
{
	GLColor *glcolor = NULL;

	switch(property)
	{
	case Ambient:
		glcolor = &mAmbient;
		break;
	case Diffuse:
		glcolor = &mDiffuse;
		break;
	case Emissive:
		glcolor = &mEmissive;
		break;
	case Specular:
		glcolor = &mSpecular;
		break;
	default:
		break;
	}

	wxASSERT(NULL != glcolor);

	glcolor->r = (GLfloat) (color.Red() / 255.0);
	glcolor->g = (GLfloat) (color.Green() / 255.0);
	glcolor->b = (GLfloat) (color.Blue() / 255.0);
}

void GLMaterial::SetMaterial(wxColour color)
{
	SetProperty(Diffuse, color);
	SetProperty(Ambient, color);
}

void GLMaterial::MultProperty(Property property, GLfloat val)
{
	GLColor *glcolor = NULL;

	switch(property)
	{
	case Ambient:
		glcolor = &mAmbient;
		break;
	case Diffuse:
		glcolor = &mDiffuse;
		break;
	case Emissive:
		glcolor = &mEmissive;
		break;
	case Specular:
		glcolor = &mSpecular;
		break;
	default:
		break;
	}

	wxASSERT(NULL != glcolor);
	
	glcolor->r *= val;
	glcolor->g *= val;
	glcolor->b *= val;

	clamp(glcolor->r, 0.0f, 1.0f);
	clamp(glcolor->g, 0.0f, 1.0f);
	clamp(glcolor->b, 0.0f, 1.0f);
}

void GLMaterial::UseMaterial() const
{
	GLfloat amb[] = {mAmbient.r, mAmbient.g, mAmbient.b, mAmbient.a};
	GLfloat dif[] = {mDiffuse.r, mDiffuse.g, mDiffuse.b, mDiffuse.a};
	GLfloat emi[] = {mEmissive.r, mEmissive.g, mEmissive.b, mEmissive.a};
	GLfloat spec[] = {mSpecular.r, mSpecular.g, mSpecular.b, mSpecular.a};

	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	glMaterialfv(GL_FRONT, GL_EMISSION, emi);
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
	glMaterialfv(GL_FRONT, GL_SHININESS, &mShininess);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//


