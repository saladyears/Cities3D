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
#include "DrawMerchantObject.h"
#include "MerchantObject.h"
#include "GLMaterial.h"
#include "GLTextures.h"
#include "RuleSetDatabase.h"
#include "DataObject.h"
#include "Box.h"
#include "define/defineGL.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	//textures
	const wxInt32 NUM_TEXTURES_MERCHANT = 1;
	GLuintArray sTexPieces_Merchant;

	enum
	{
		MerchantDraw,
	};

	//quad object for cylinders
	GLUquadricObj *sQuad_Merchant;

	GLMaterial sPurple;

	// MerchantDraw dimensions.
	const float MRAD				= (0.75 * SCALE);
	const float MHIGH				= (2.2 * SCALE);

	void obtainPrivate_Merchant()
	{
		//read in piece textures
		/*
		sTexPieces_Merchant.reset(new GLuint[NUM_TEXTURES_MERCHANT]);
		GLTextures::ReadGLTextures(NUM_TEXTURES_MERCHANT, &sTexPieces_Merchant[0], 
			"glknights.tex");
		*/

		//create the object
		sQuad_Merchant = gluNewQuadric();

		//turn on textures
		gluQuadricTexture(sQuad_Merchant, GL_TRUE);
		gluQuadricOrientation(sQuad_Merchant, GLU_OUTSIDE);

		sPurple.SetMaterial(wxColour(0, 0, 0));
		sPurple.SetProperty(GLMaterial::Emissive, wxColour(75, 0, 75));
	}

	void releasePrivate_Merchant()
	{
		//set no texture
		glBindTexture(GL_TEXTURE_2D, 0);

		//clear piece textures
		//glDeleteTextures(NUM_TEXTURES_MERCHANT, &sTexPieces_Merchant[0]);

		//clean up the quadric
		gluDeleteQuadric(sQuad_Merchant);
	}

	void drawMerchant()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		
		// The merchant is just a cone, thankfully.
		glPushMatrix();
			glRotated(90.0, -1.0, 0.0, 0.0);

			//glBindTexture(GL_TEXTURE_2D, sTexPieces_Merchant[MerchantDraw]);

			gluCylinder(sQuad_Merchant, MRAD, 0.11, MHIGH + 0.1, 20, 5);
		glPopMatrix();
		glPushMatrix();
			glTranslated(0.0, MHIGH, 0.0);
			glRotated(90.0, -1.0, 0.0, 0.0);
			gluDisk(sQuad_Merchant, 0.0, 0.1, 10, 10);
		glPopMatrix();
	}
}

//---------------------------- PUBLIC           -----------------------------//
IMPLEMENT_DRAW_OBJECT(DrawMerchantObject, MerchantObject, CITIES);

DrawMerchantObject::DrawMerchantObject(const GameObjectPtr &object)
: DrawObject(object, 10)
{
}

DrawMerchantObject::~DrawMerchantObject()
{
}

bool DrawMerchantObject::CanSelect(const Vector &origin, const Vector &ray, 
	float &distance, wxInt32 &id, bool popup)
{
	bool hit = false;
	IGameObject *object = GetObject();

	// Use a simple bounding box for merchant hit detection, which is crap.
	if( (NULL != object) &&
		(false == object->isset(IGameObject::Hidden)) &&
		((object->isset(IGameObject::Select)) ||
		(true == popup)))
	{
		// Test the intersection of the ray against a bounding box around the
		// merchant, which is terrible.
		const Vector &o = object->coords();

		// Point 1 is max height, one edge of x/z away.
		Vector p1 = o + Vector(-MRAD, 0.0f, -MRAD);
		Vector p2 = o + Vector(MRAD, MHIGH, MRAD);

		Box box(p1, p2);

		if(true == box.TestIntersection(origin, ray))
		{
			Vector line = o + Vector(0.0f, MHIGH, 0.0f) - origin;
			distance = line.Length();

			id = object->id();
			hit = true;
		}
	}

	return hit;
}

void DrawMerchantObject::Obtain()
{
	obtainPrivate_Merchant();
}

void DrawMerchantObject::Release()
{
	releasePrivate_Merchant();
}

void DrawMerchantObject::PopupFunction(wxString& name, wxString& owner, 
	wxBitmap&, bool &createBitmap, ColorType&)
{
	name = stMerchant;

	createBitmap = true;

	DataObject input(0), output;
	RULE.Decide(shLogicMerchantOwner, input, output);
	wxInt32 ownerIndex = output.read<wxInt32>();
	wxASSERT(-1 != ownerIndex);

	// Get the owner's name.
	input = DataObject(ownerIndex);
	output.reset();
	RULE.Decide(shLogicPlayerName, input, output);
	owner = output.read<wxString>();
}

void DrawMerchantObject::RenderOutline() const
{
	wxASSERT(false);
}

void DrawMerchantObject::RenderSelection() const
{
	wxASSERT(false);
}

void DrawMerchantObject::RenderModel(const wxInt32) const
{
	sPurple.UseMaterial();
	drawMerchant();
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//


