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
#include "DrawKnightObject.h"
#include "KnightObject.h"
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
	const wxInt32 NUM_TEXTURES_KNIGHT = 7;
	GLuintArray sTexPieces_Knight;

	enum
	{
		Knight1Off		= 0,
		Knight1On		,
		Knight2Off		,
		Knight2On		,
		Knight3Off		,
		Knight3On		,
		KnightSide		,
	};

	//quad object for cylinders
	GLUquadricObj *sQuad_Knight;

	//knight dimensions
	const float KNRAD				= (1.0 * SCALE);
	const float KNTEXRAD			= (0.85 * SCALE);
	const float KNHIGH				= (0.6 * SCALE);

	void obtainPrivate_Knight()
	{
		//read in piece textures
		sTexPieces_Knight.reset(new GLuint[NUM_TEXTURES_KNIGHT]);
		GLTextures::ReadGLTextures(NUM_TEXTURES_KNIGHT, &sTexPieces_Knight[0], 
			swGlKnightsTex);

		//create the object
		sQuad_Knight = gluNewQuadric();

		//turn on textures
		gluQuadricTexture(sQuad_Knight, GL_TRUE);
		gluQuadricOrientation(sQuad_Knight, GLU_OUTSIDE);
	}

	void releasePrivate_Knight()
	{
		//set no texture
		glBindTexture(GL_TEXTURE_2D, 0);

		//clear piece textures
		glDeleteTextures(NUM_TEXTURES_KNIGHT, &sTexPieces_Knight[0]);

		//clean up the quadric
		gluDeleteQuadric(sQuad_Knight);
	}

	void drawKnight(const DrawKnightObject * const pObject)
	{
		KnightObject *knight = static_cast<KnightObject *>(
			pObject->GetObject());

		//base cylinder
		glPushMatrix();
			glRotated(90.0, -1.0, 0.0, 0.0);

			glBindTexture(GL_TEXTURE_2D, sTexPieces_Knight[KnightSide]);

			//sides of the cylinder
			gluCylinder(sQuad_Knight, KNRAD, KNRAD, KNHIGH, 20, 5);
		glPopMatrix();
		
		//top disk
		glPushMatrix();
			glTranslated(0.0, KNHIGH, 0.0);
			glRotated(90.0, -1.0, 0.0, 0.0);
			
			glBindTexture(GL_TEXTURE_2D, sTexPieces_Knight[KnightSide]);

			gluDisk(sQuad_Knight, 0.0, KNRAD, 20, 5);
		glPopMatrix();

		GLMaterial mat(DrawObject::material(DrawObject::White));

		//calculate which texture to use
		wxInt32 tex = ((knight->strength() - 1) * 2);

		if(true == knight->active())
		{
			//active knights get the brighter picture
			++tex;
		}
		else
		{
			//inactive knights are darker
			mat.MultProperty(GLMaterial::Ambient, 0.75);
		}
		mat.UseMaterial();

		//top texture
		glPushMatrix();
			glTranslated(0.0, KNHIGH + 0.01, 0.0);
			glRotated(90.0, -1.0, 0.0, 0.0);

			//knights texture
			glBindTexture(GL_TEXTURE_2D, sTexPieces_Knight[tex]);

			gluDisk(sQuad_Knight, 0.0, KNTEXRAD, 20, 5);

		glPopMatrix();
	}

	void drawKnightOutline()
	{
		//top ring
		glPushMatrix();
			glTranslated(0.0, KNHIGH + .01, 0.0);
			glRotated(90.0, 1.0, 0.0, 0.0);
			gluDisk(sQuad_Knight, KNRAD, KNRAD + 0.1, 20, 5);
		glPopMatrix();
	}
}

//---------------------------- PUBLIC           -----------------------------//
IMPLEMENT_DRAW_OBJECT(DrawKnightObject, KnightObject, CITIES);

DrawKnightObject::DrawKnightObject(const GameObjectPtr &object)
: DrawObject(object, 10)
{
}

DrawKnightObject::~DrawKnightObject()
{
}

bool DrawKnightObject::CanSelect(const Vector &origin, const Vector &ray, 
	float &distance, wxInt32 &id, bool popup)
{
	bool hit = false;
	IGameObject *object = GetObject();

	// Use a simple bounding box for knight hit detection.

	if( (NULL != object) &&
		((object->isset(IGameObject::Select)) ||
		(true == popup)))
	{
		// Test the intersection of the ray against a bounding box around the
		// knight.
		const Vector &o = object->coords();

		// Point 1 is max height, one edge of x/z away.
		Vector p1 = o + Vector(-KNRAD, 0.0f, -KNRAD);
		Vector p2 = o + Vector(KNRAD, KNHIGH, KNRAD);

		Box box(p1, p2);

		if(true == box.TestIntersection(origin, ray))
		{
			Vector line = o + Vector(0.0f, KNHIGH, 0.0f) - origin;
			distance = line.Length();

			id = object->id();
			hit = true;
		}
	}

	return hit;
}

void DrawKnightObject::Obtain()
{
	obtainPrivate_Knight();
}

void DrawKnightObject::Release()
{
	releasePrivate_Knight();
}

void DrawKnightObject::PopupFunction(wxString& name, wxString& owner, 
	wxBitmap&, bool &createBitmap, ColorType& color)
{
	KnightObject *object = static_cast<KnightObject*>(GetObject());
	wxASSERT(NULL != object);

	switch(object->strength())
	{
	case 1:
		name = stBasicKnight;
		break;
	case 2:
		name = stStrongKnight;
		break;
	case 3:
		name = stMightyKnight;
		break;
	default:
		wxASSERT(false);
		break;
	}

	color = object->color();

	createBitmap = true;

	// Get the owner's name.
	wxInt32 ownerIndex = object->owner();
	DataObject input(ownerIndex), output;
	RULE.Decide(shLogicPlayerName, input, output);
	owner = output.read<wxString>();
}

void DrawKnightObject::RenderOutline() const
{
	drawKnightOutline();
}

void DrawKnightObject::RenderSelection() const
{
	DrawObject::material(Selection).UseMaterial();
	
	// Draw a single disc around the top of the knight.
	drawKnightOutline();
}

void DrawKnightObject::RenderModel(const wxInt32) const
{
	DrawObject::material(GetObject()).UseMaterial();
	drawKnight(this);
	DrawObject::material(White).UseMaterial();
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//


