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
#include "DrawWallsObject.h"
#include "WallsObject.h"
#include "GLMaterial.h"
#include "GLTextures.h"
#include "RuleSetDatabase.h"
#include "Box.h"
#include "DataObject.h"
#include "define/defineGL.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	//textures
	const wxInt32 NUM_TEXTURES_WALLS			= 3;
	GLuintArray sTexPieces_Walls;

	//display lists
	const wxInt32 NUM_LISTS_WALLS				= 3;
	GLuintArray sLists_Walls;

	enum
	{
		Walls			= 0,
		WallsSideA,
		WallsSideB
	};

	//walls dimensions
	const float CWSIDE				= (2.1 * SCALE);
	const float CWHIGH				= (0.5 * SCALE);

	//city walls
	void buildWalls(GLuint id)
	{
		glNewList(id, GL_COMPILE);
			//top face
			glBegin(GL_QUADS);
				glNormal3f(0.0, 1.0, 0.0);
				glTexCoord2d(1.00, 0.00); glVertex3d(-CWSIDE / 2, CWHIGH, 
					-CWSIDE / 2);
				glTexCoord2d(0.00, 0.00); glVertex3d(CWSIDE / 2, CWHIGH, 
					-CWSIDE / 2);
				glTexCoord2d(0.00, 1.00); glVertex3d(CWSIDE / 2, CWHIGH, 
					CWSIDE / 2);
				glTexCoord2d(1.00, 1.00); glVertex3d(-CWSIDE / 2, CWHIGH, 
					CWSIDE / 2);
			glEnd();
		//finish up
		glEndList();

		//side faces
		glNewList(id + 1, GL_COMPILE);
			//front face
			glBegin(GL_QUADS);
				glNormal3f(0.0, 0.0, 1.0);
				glTexCoord2d(0.00, 0.00); glVertex3d(CWSIDE / 2, 0.0, 
					CWSIDE / 2);
				glTexCoord2d(1.00, 0.00); glVertex3d(-CWSIDE / 2, 0.0, 
					CWSIDE / 2);
				glTexCoord2d(1.00, 1.00); glVertex3d(-CWSIDE / 2, CWHIGH, 
					CWSIDE / 2);
				glTexCoord2d(0.00, 1.00); glVertex3d(CWSIDE / 2, CWHIGH, 
					CWSIDE / 2);
			glEnd();
			//back face
			glBegin(GL_QUADS);
				glNormal3f(0.0, 0.0, -1.0);
				glTexCoord2d(0.00, 0.00); glVertex3d(CWSIDE / 2, 0.0, 
					-CWSIDE / 2);
				glTexCoord2d(1.00, 0.00); glVertex3d(-CWSIDE / 2, 0.0, 
					-CWSIDE / 2);
				glTexCoord2d(1.00, 1.00); glVertex3d(-CWSIDE / 2, CWHIGH, 
					-CWSIDE / 2);
				glTexCoord2d(0.00, 1.00); glVertex3d(CWSIDE / 2, CWHIGH, 
					-CWSIDE / 2);
			glEnd();
		glEndList();

		//side faces
		glNewList(id + 2, GL_COMPILE);
			//right side face
			glBegin(GL_QUADS);
				glNormal3f(1.0, 0.0, 0.0);
				glTexCoord2d(0.00, 0.00); glVertex3d(CWSIDE / 2, 0.0, 
					CWSIDE / 2);
				glTexCoord2d(1.00, 0.00); glVertex3d(CWSIDE / 2, 0.0, 
					-CWSIDE / 2);
				glTexCoord2d(1.00, 1.00); glVertex3d(CWSIDE / 2, CWHIGH, 
					-CWSIDE / 2);
				glTexCoord2d(0.00, 1.00); glVertex3d(CWSIDE / 2, CWHIGH, 
					CWSIDE / 2);
			glEnd();
			//left side face
			glBegin(GL_QUADS);
				glNormal3f(-1.0, 0.0, 0.0);
				glTexCoord2d(0.00, 0.00); glVertex3d(-CWSIDE / 2, 0.0, 
					CWSIDE / 2);
				glTexCoord2d(1.00, 0.00); glVertex3d(-CWSIDE / 2, 0.0, 
					-CWSIDE / 2);
				glTexCoord2d(1.00, 1.00); glVertex3d(-CWSIDE / 2, CWHIGH, 
					-CWSIDE / 2);
				glTexCoord2d(0.00, 1.00); glVertex3d(-CWSIDE / 2, CWHIGH, 
					CWSIDE / 2);
			glEnd();
		//finish up
		glEndList();
	}

	void obtainPrivate_Walls()
	{
		sTexPieces_Walls.reset(new GLuint[NUM_TEXTURES_WALLS]);

		//read in piece textures
		GLTextures::ReadGLTextures(NUM_TEXTURES_WALLS, &sTexPieces_Walls[0], 
			swGlWallsTex);
		
		//display lists
		sLists_Walls.reset(new GLuint[NUM_LISTS_WALLS]);

		//generate
		for(wxInt32 i = 0; i < NUM_LISTS_WALLS; i++)
		{
			sLists_Walls[i] = glGenLists(1);
		}

		//create the lists
		buildWalls(sLists_Walls[Walls]);
	}

	void releasePrivate_Walls()
	{
		//set no texture
		glBindTexture(GL_TEXTURE_2D, 0);

		//clear piece textures
		glDeleteTextures(NUM_TEXTURES_WALLS, &sTexPieces_Walls[0]);

		//clear out display lists
		glDeleteLists(sLists_Walls[0], NUM_LISTS_WALLS);
	}

	void drawWalls()
	{
		glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, sTexPieces_Walls[Walls]);
			glCallList(sLists_Walls[Walls]);

			glBindTexture(GL_TEXTURE_2D, sTexPieces_Walls[WallsSideA]);
			glCallList(sLists_Walls[WallsSideA]);

			glBindTexture(GL_TEXTURE_2D, sTexPieces_Walls[WallsSideB]);
			glCallList(sLists_Walls[WallsSideB]);
		glPopMatrix();
	}
}

//---------------------------- PUBLIC           -----------------------------//
IMPLEMENT_DRAW_OBJECT(DrawWallsObject, WallsObject, CITIES);

DrawWallsObject::DrawWallsObject(const GameObjectPtr &object)
: DrawObject(object, 5)
{
	WallsObject* thisObject = static_cast<WallsObject*>(GetObject());
	if(NULL != thisObject)
	{
		thisObject->offset(CWHIGH);
	}
}

DrawWallsObject::~DrawWallsObject()
{
}

void DrawWallsObject::Obtain()
{
	obtainPrivate_Walls();
}

void DrawWallsObject::Release()
{
	releasePrivate_Walls();
}

bool DrawWallsObject::CanSelect(const Vector &origin, const Vector &ray, 
	float &distance, wxInt32 &id, bool popup)
{
	bool hit = false;
	IGameObject *object = GetObject();

	// Use a simple bounding box for walls hit detection.

	if( (NULL != object) &&
		((object->isset(IGameObject::Select)) ||
		(true == popup)))
	{
		// Test the intersection of the ray against a bounding box around the
		// walls.
		const Vector &o = object->coords();

		// Point 1 is max height, one edge of x/z away.
		Vector p1 = o + Vector(-CWSIDE / 2.0f, 0.0f, -CWSIDE / 2.0f);
		Vector p2 = o + Vector(CWSIDE / 2.0f, CWHIGH, CWSIDE / 2.0f);

		Box box(p1, p2);

		if(true == box.TestIntersection(origin, ray))
		{
			Vector line = o + Vector(0.0f, CWHIGH, 0.0f) - origin;
			distance = line.Length();

			id = object->id();
			hit = true;
		}
	}

	return hit;
}

void DrawWallsObject::PopupFunction(wxString& name, wxString& owner, 
	wxBitmap&, bool &createBitmap, ColorType& color)
{
	name = stCityWalls;

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

void DrawWallsObject::RenderOutline() const
{
}

void DrawWallsObject::RenderSelection() const
{
}

void DrawWallsObject::RenderModel(const wxInt32) const
{
	DrawObject::material(GetObject()).UseMaterial();
	drawWalls();
	DrawObject::material(White).UseMaterial();
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//


