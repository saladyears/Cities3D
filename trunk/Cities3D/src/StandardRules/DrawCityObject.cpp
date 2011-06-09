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
#include "DrawCityObject.h"
#include "CityObject.h"
#include "GLMaterial.h"
#include "GLTextures.h"
#include "RuleSetDatabase.h"
#include "Box.h"
#include "define/defineGL.h"
#include "DataObject.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	//textures
	const wxInt32 NUM_TEXTURES_CITY			= 2;
	GLuintArray sTexPieces_City;

	//display lists
	const wxInt32 NUM_LISTS_CITY				= 2;
	GLuintArray sLists_City;

	enum
	{
		City		= 0,
		CitySide
	};

	//city dimensions
	const float CLEN				= (2.0 * SCALE);
	const float CWID				= (1.0 * SCALE);
	const float CLOW				= (0.8 * SCALE);
	const float CHIGH				= (1.5 * SCALE);
	const float CPEAK				= (2.0 * SCALE);

	//a city piece
	void buildCity(GLuint id)
	{
		//build the city
		glNewList(id, GL_COMPILE);
			//front face
			glBegin(GL_POLYGON);
				glNormal3f(0.0, 0.0, 1.0);
				glTexCoord2d(0.00, 0.00); glVertex3d(CWID / 2, 0.0, CLEN / 2);
				glTexCoord2d(0.00, 1.00); glVertex3d(CWID / 2, CHIGH, 
					CLEN / 2);
				glTexCoord2d(1.00, 1.00); glVertex3d(-CWID / 2, CHIGH, 
					CLEN / 2);
				glTexCoord2d(1.00, 0.00); glVertex3d(-CWID / 2, 0.0, CLEN / 2);
				glTexCoord2d(0.00, 0.00); glVertex3d(CWID / 2, 0.0, CLEN / 2);
			glEnd();
			//back face
			glBegin(GL_POLYGON);
				glNormal3f(0.0, 0.0, -1.0);
				glTexCoord2d(0.00, 0.00); glVertex3d(CWID / 2, 0.0, -CLEN / 2);
				glTexCoord2d(0.00, 0.50); glVertex3d(CWID / 2, CLOW, 
					-CLEN / 2);
				glTexCoord2d(0.50, 0.50); glVertex3d(-CWID / 2, CLOW, 
					-CLEN / 2);
				glTexCoord2d(0.50, 0.00); glVertex3d(-CWID / 2, 0.0, 
					-CLEN / 2);
				glTexCoord2d(0.00, 0.00); glVertex3d(CWID / 2, 0.0, -CLEN / 2);
			glEnd();
			//second back face
			glBegin(GL_POLYGON);
				glNormal3f(0.0, 0.0, -1.0);
				glTexCoord2d(0.40, 0.40); glVertex3d(CWID / 2, CLOW, 0.0);
				glTexCoord2d(0.40, 0.90); glVertex3d(CWID / 2, CHIGH, 0.0);
				glTexCoord2d(0.90, 0.90); glVertex3d(-CWID / 2, CHIGH, 0.0);
				glTexCoord2d(0.90, 0.40); glVertex3d(-CWID / 2, CLOW, 0.0);
				glTexCoord2d(0.40, 0.40); glVertex3d(CWID / 2, CLOW, 0.0);
			glEnd();
			//top face
			glBegin(GL_POLYGON);
				glNormal3f(0.0, 1.0, 0.0);
				glTexCoord2d(0.00, 0.00); glVertex3d(CWID / 2, CLOW, 
					-CLEN / 2);
				glTexCoord2d(0.00, 0.67); glVertex3d(CWID / 2, CLOW, 0.0);
				glTexCoord2d(0.67, 0.67); glVertex3d(-CWID / 2, CLOW, 0.0);
				glTexCoord2d(0.67, 0.00); glVertex3d(-CWID / 2, CLOW,
					-CLEN / 2);
				glTexCoord2d(0.00, 0.00); glVertex3d(CWID / 2, CLOW, 
					-CLEN / 2);
			glEnd();
			//top back face
			glBegin(GL_POLYGON);
				glNormal3f(0.0, 1.0, -1.0);
				glTexCoord2d(0.00, 0.00); glVertex3d(CWID / 2, CHIGH, 0.0);
				glTexCoord2d(0.00, 0.33); glVertex3d(CWID / 2, CPEAK, 
					CLEN / 4);
				glTexCoord2d(0.33, 0.33); glVertex3d(-CWID / 2, CPEAK, 
					CLEN / 4);
				glTexCoord2d(0.33, 0.00); glVertex3d(-CWID / 2, CHIGH, 0.0);
				glTexCoord2d(0.00, 0.00); glVertex3d(CWID / 2, CHIGH, 0.0);
			glEnd();
			//top front face
			glBegin(GL_POLYGON);
				glNormal3f(0.0, 1.0, 1.0);
				glTexCoord2d(0.10, 0.10); glVertex3d(CWID / 2, CHIGH, 
					CLEN / 2);
				glTexCoord2d(0.10, 0.43); glVertex3d(CWID / 2, CPEAK, 
					CLEN / 4);
				glTexCoord2d(0.43, 0.43); glVertex3d(-CWID / 2, CPEAK, 
					CLEN / 4);
				glTexCoord2d(0.43, 0.10); glVertex3d(-CWID / 2, CHIGH, 
					CLEN / 2);
				glTexCoord2d(0.10, 0.10); glVertex3d(CWID / 2, CHIGH, 
					CLEN / 2);
			glEnd();
			// Bottom face.
			glBegin(GL_POLYGON);
				glNormal3f(0.0, -1.0, 0.0);
				glVertex3d(CWID / 2, 0.0, -CLEN / 2);
				glVertex3d(CWID / 2, 0.0, CLEN / 2);
				glVertex3d(-CWID / 2, 0.0, CLEN / 2);
				glVertex3d(-CWID / 2, 0.0, -CLEN / 2);
				glVertex3d(CWID / 2, 0.0, -CLEN / 2);
			glEnd();
		glEndList();

		glNewList(id + 1, GL_COMPILE);
			//right side face
			glBegin(GL_POLYGON);
				glNormal3f(1.0, 0.0, 0.0);
				glTexCoord2d(0.02, 0.00); glVertex3d(CWID / 2, 0.0, CLEN / 2);
				glTexCoord2d(0.98, 0.00); glVertex3d(CWID / 2, 0.0, -CLEN / 2);
				glTexCoord2d(0.98, 0.40); glVertex3d(CWID / 2, CLOW, 
					-CLEN / 2);
				glTexCoord2d(0.48, 0.40); glVertex3d(CWID / 2, CLOW, 0.0);
				glTexCoord2d(0.48, 0.71); glVertex3d(CWID / 2, CHIGH, 0.0);
				glTexCoord2d(0.25, 0.95); glVertex3d(CWID / 2, CPEAK, 
					CLEN / 4);
				glTexCoord2d(0.02, 0.71); glVertex3d(CWID / 2, CHIGH, 
					CLEN / 2);
				glTexCoord2d(0.02, 0.00); glVertex3d(CWID / 2, 0.0, CLEN / 2);
			glEnd();
			//left side face
			glBegin(GL_POLYGON);
				glNormal3f(-1.0, 0.0, 0.0);
				glTexCoord2d(0.02, 0.00); glVertex3d(-CWID / 2, 0.0, CLEN / 2);
				glTexCoord2d(0.98, 0.00); glVertex3d(-CWID / 2, 0.0, 
					-CLEN / 2);
				glTexCoord2d(0.98, 0.40); glVertex3d(-CWID / 2, CLOW, 
					-CLEN / 2);
				glTexCoord2d(0.48, 0.40); glVertex3d(-CWID / 2, CLOW, 0.0);
				glTexCoord2d(0.48, 0.71); glVertex3d(-CWID / 2, CHIGH, 0.0);
				glTexCoord2d(0.25, 0.95); glVertex3d(-CWID / 2, CPEAK, 
					CLEN / 4);
				glTexCoord2d(0.02, 0.71); glVertex3d(-CWID / 2, CHIGH, 
					CLEN / 2);
				glTexCoord2d(0.02, 0.00); glVertex3d(-CWID / 2, 0.0, CLEN / 2);
			glEnd();
		glEndList();
	}

	void obtainPrivate_CityObject()
	{
		sTexPieces_City.reset(new GLuint[NUM_TEXTURES_CITY]);

		//read in piece textures
		GLTextures::ReadGLTextures(NUM_TEXTURES_CITY, &sTexPieces_City[City], 
			swGlCityTex);

		//display lists
		sLists_City.reset(new GLuint[NUM_LISTS_CITY]);

		//generate
		for(wxInt32 i = 0; i < NUM_LISTS_CITY; i++)
		{
			sLists_City[i] = glGenLists(1);
		}

		//create the lists
		buildCity(sLists_City[City]);
	}

	void releasePrivate_CityObject()
	{
		//set no texture
		glBindTexture(GL_TEXTURE_2D, 0);

		//clear piece textures
		glDeleteTextures(NUM_TEXTURES_CITY, &sTexPieces_City[0]);

		//clear out display lists
		glDeleteLists(sLists_City[0], NUM_LISTS_CITY);
	}

	void drawCity()
	{
		glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, sTexPieces_City[City]);
			glCallList(sLists_City[City]);

			glBindTexture(GL_TEXTURE_2D, sTexPieces_City[CitySide]);
			glCallList(sLists_City[CitySide]);
		glPopMatrix();
	}
}

//---------------------------- PUBLIC           -----------------------------//
IMPLEMENT_DRAW_OBJECT(DrawCityObject, CityObject, STANDARD);

DrawCityObject::DrawCityObject(const GameObjectPtr &object)
: DrawObject(object, 10)
{
}

DrawCityObject::~DrawCityObject()
{
}

bool DrawCityObject::CanSelect(const Vector &origin, const Vector &ray, 
	float &distance, wxInt32 &id, bool popup)
{
	bool hit = false;
	IGameObject *object = GetObject();

	if( (NULL != object) &&
		((object->isset(IGameObject::Select)) ||
		(true == popup)))
	{
		// Test the intersection of the ray against the bounding boxes around 
		// the city.
		const Vector &o = object->coords();

		// We need to rotate the origin and ray as if our rotation was 
		// axis-aligned.
		Vector rotation = object->rotation();

		Vector orig;
		Vector temp = origin - o;

		float cosT = cosf(rotation.z * D2R);
		float sinT = sinf(rotation.z * D2R);

		temp = Vector((temp.x * cosT) + (temp.y * sinT), 
			(-temp.x * sinT) + (temp.y * cosT), temp.z);
		orig = o + temp;

		Vector r = Vector((ray.x * cosT) + (ray.y * sinT), 
			(-ray.x * sinT) + (ray.y * cosT), ray.z);

		// The first bounding box is defined by the two opposite corners.  Our 
		// origin point is on the bottom of the city, centered between the 
		// edges.

		// Point 1 is the base height height, one edge of x/z away.
		Vector p1 = o + Vector(-CWID / 2.0f, 0.0f, -CLEN / 2.0f);
		Vector p2 = o + Vector(CWID / 2.0f, CLOW, CLEN / 2.0f);

		Vector p3 = o + Vector(-CWID / 2.0f, CLOW, 0.0f);
		Vector p4 = o + Vector(CWID / 2.0f, CPEAK, CLEN / 2.0f);

		// NOTE: If cities were ever rotated, we'd have to rotate the corner
		// points here, but they aren't.

		Box box1(p1, p2);
		Box box2(p3, p4);

		if( (true == box1.TestIntersection(orig, r)) ||
			(true == box2.TestIntersection(orig, r)))
		{
			Vector line = o + Vector(0.0f, CPEAK, 0.0f) - origin;
			distance = line.Length();

			id = object->id();
			hit = true;
		}
	}

	return hit;
}

void DrawCityObject::Obtain()
{
	obtainPrivate_CityObject();
}

void DrawCityObject::Release()
{
	releasePrivate_CityObject();
}

void DrawCityObject::PopupFunction(wxString& name, wxString& owner, 
	wxBitmap&, bool &createBitmap, ColorType& color)
{
	name = stCity;

	IGameObject *object = GetObject();
	wxASSERT(NULL != object);

	color = object->color();

	createBitmap = true;

	// Get the owner's name.
	wxInt32 ownerIndex = object->owner();
	DataObject input(ownerIndex), output;
	RULE.Decide(shLogicPlayerName, input, output);
	owner = output.read<wxString>();
}

void DrawCityObject::RenderOutline() const
{
	DrawObject::material(Gold).UseMaterial();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	drawCity();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void DrawCityObject::RenderSelection() const
{
	DrawObject::material(Selection).UseMaterial();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	drawCity();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void DrawCityObject::RenderModel(const wxInt32) const
{
	DrawObject::material(GetObject()).UseMaterial();
	drawCity();
	DrawObject::material(White).UseMaterial();
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//



