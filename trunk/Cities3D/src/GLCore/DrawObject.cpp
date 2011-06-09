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
#include "DrawObject.h"
#include "ConfigDatabase.h"
#include "ColorInfoData.h"
#include "GLMaterial.h"
#include "IGameObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	//static renderer so this code is only used once
	void RenderPrivate(const DrawObject * const pModel, IGameObject *pObject, 
		const wxInt32 selection)
	{
		//see if the object even needs to be drawn
		if(true == pObject->isset(IGameObject::Hidden))
		{
			return;
		}

		//set dimness
		DrawObject::Material type = pObject->isset(IGameObject::Dim) ? 
			DrawObject::Gray : DrawObject::White;
		DrawObject::material(type).UseMaterial();

		//set selection name if needed
		bool select = pObject->isset(IGameObject::Select);
		wxInt32 id = select ? (pObject->id() + 1) : 0;

		Vector coords = pObject->coords();
		Vector rotation = pObject->rotation();

		glPushMatrix();
			//object translation and rotation
			glTranslated(coords.x, coords.y, coords.z);
			glRotated(rotation.x, 1.0, 0.0, 0.0);
			glRotated(rotation.y, 0.0, 1.0, 0.0);
			glRotated(rotation.z, 0.0, 0.0, 1.0);

			//draw the model
			pModel->RenderModel(selection);

			//draw the selection outline or other outline if needed
			if( (true == select) &&
				((id - 1) == selection))
			{
				pModel->RenderSelection();
			}
			else if(true == pObject->isset(IGameObject::Outline))
			{
				pModel->RenderOutline();
			}

		glPopMatrix();
	}

	//static variables we need
	std::vector<GLMaterial> sMaterials;
	GLMaterial sColorMaterial;
}

const GLMaterial &DrawObject::material(Material type)
{
	static bool once = true;

	wxColour black(0, 0, 0);

	//prepare materials
	if(true == once)
	{
		sMaterials.resize(MaterialSize);

		//set up material colors
		sMaterials[Gray].SetMaterial(black);
		sMaterials[Gray].SetProperty(GLMaterial::Emissive, 
			wxColour(128, 128, 128));
		sMaterials[White].SetMaterial(wxColour(255, 255, 255));
		sMaterials[Gold].SetMaterial(black);
		sMaterials[Gold].SetProperty(GLMaterial::Emissive, 
			wxColour(255, 227, 100));
		sMaterials[Red].SetMaterial(black);
		sMaterials[Red].SetProperty(GLMaterial::Emissive, wxColour(255, 0, 0));
		sMaterials[Black].SetMaterial(black);
		
		once = false;
	}

	//check for special colors
	if( (UserBegin < type) && 
		(UserEnd > type))
	{
		wxString option;
		wxColour color = black;

		switch(type)
		{
		case Selection:
			option = swConfigGLSectionColor;
			color = wxColour(0, 0, 128);
			break;
		case WhiteOutline:
			option = swConfigGLWhiteColor;
			color = wxColour(128, 0, 128);
			break;
		case LongestOutline:
			option = swConfigGLLongestColor;
			color = wxColour(255, 227, 100);
			break;
		default:
			wxASSERT(false);
			break;
		}

		sMaterials[type].SetMaterial(black);
		wxInt32 pixel((wxInt32) color.GetPixel());
		sMaterials[type].SetProperty(GLMaterial::Emissive, 
			CONFIG.read(swConfigSectionOpenGL, option, pixel));
	}

	return sMaterials[type];
}

const GLMaterial &DrawObject::material(IGameObject *object)
{
	wxASSERT(NULL != object);
	
	// Set the color properties.
	material(object->color());

	//Handle dim
	if(true == object->isset(IGameObject::Dim))
	{
		sColorMaterial.MultProperty(GLMaterial::Ambient, 0.5f);
		sColorMaterial.MultProperty(GLMaterial::Diffuse, 0.5f);
	}

	return sColorMaterial;
}

const GLMaterial &DrawObject::material(ColorType color)
{
	sColorMaterial.SetMaterial(wxColour(0, 0, 0));
	sColorMaterial.SetProperty(GLMaterial::Ambient, 
		ColorInfoData::GLColor(color));
	sColorMaterial.MultProperty(GLMaterial::Ambient, .65f);

	sColorMaterial.SetProperty(GLMaterial::Diffuse, 
		ColorInfoData::GLColor(color));
	sColorMaterial.MultProperty(GLMaterial::Diffuse, .50f);

	return sColorMaterial;
}

//---------------------------- PUBLIC           -----------------------------//
DrawObject::DrawObject(const GameObjectPtr &pObject, const wxInt32 priority)
: mpObject(pObject)
, mDelete(false)
, mPriority(priority)
{
}

IGameObject *DrawObject::GetObject() const
{
	return mpObject.lock().get();
}

bool DrawObject::CanSelect(const Vector &, const Vector &, float &, wxInt32 &,
	bool)
{
	//let derived objects override to provide their own selection testing
	return false;
}

void DrawObject::Render(const wxInt32 selection) const
{
	//get the object
	IGameObject *pObject = GetObject();

	//see if the referenced object has expired and been removed, in which case,
	//we can delete this draw object at the next opportunity
	if(NULL == pObject)
	{
		mDelete = true;
		return;
	}

	RenderPrivate(this, pObject, selection);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//


