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
#include "DrawRoadObject.h"
#include "RoadObject.h"
#include "GLTextures.h"
#include "GLMaterial.h"
#include "RuleSetDatabase.h"
#include "define/defineGL.h"
#include "Box.h"
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
	const wxInt32 NUM_TEXTURES_ROAD			= 2;
	GLuintArray sTexPieces_Road;

	//display lists
	const wxInt32 NUM_LISTS_ROAD			= 2;
	GLuintArray sLists_Road;

	enum
	{
		Road		= 0,
		RoadSide
	};

	//road dimensions
	const float RLEN				= (2.2 * SCALE);
	const float RWID				= (0.5 * SCALE);
	const float RHIGH				= (0.5 * SCALE);

	//a road piece
	void buildRoad(GLuint id)
	{
		//build the road
		glNewList(id, GL_COMPILE);
			//front face
			glBegin(GL_QUADS);
				glNormal3f(0.0, 0.0, 1.0);
				glTexCoord2d(0.00, 0.00); glVertex3d(RWID / 2, 0.0, RLEN / 2);
				glTexCoord2d(1.00, 0.00); glVertex3d(-RWID / 2, 0.0, RLEN / 2);
				glTexCoord2d(1.00, 1.00); glVertex3d(-RWID / 2, RHIGH, 
					RLEN / 2);
				glTexCoord2d(0.00, 1.00); glVertex3d(RWID / 2, RHIGH, 
					RLEN / 2);
			glEnd();
			//back face
			glBegin(GL_QUADS);
				glNormal3f(0.0, 0.0, -1.0);
				glTexCoord2d(0.00, 0.00); glVertex3d(RWID / 2, 0.0, -RLEN / 2);
				glTexCoord2d(1.00, 0.00); glVertex3d(-RWID / 2, 0.0, 
					-RLEN / 2);
				glTexCoord2d(1.00, 1.00); glVertex3d(-RWID / 2, RHIGH, 
					-RLEN / 2);
				glTexCoord2d(0.00, 1.00); glVertex3d(RWID / 2, RHIGH, 
					-RLEN / 2);
			glEnd();
		glEndList();

		glNewList(id + 1, GL_COMPILE);
			//right side face
			glBegin(GL_QUADS);
				glNormal3f(1.0, 0.0, 0.0);
				glTexCoord2d(0.00, 0.00); glVertex3d(RWID / 2, 0.0, RLEN / 2);
				glTexCoord2d(1.00, 0.00); glVertex3d(RWID / 2, 0.0, -RLEN / 2);
				glTexCoord2d(1.00, 1.00); glVertex3d(RWID / 2, RHIGH, 
					-RLEN / 2);
				glTexCoord2d(0.00, 1.00); glVertex3d(RWID / 2, RHIGH, 
					RLEN / 2);
			glEnd();
			//left side face
			glBegin(GL_QUADS);
				glNormal3f(-1.0, 0.0, 0.0);
				glTexCoord2d(0.00, 0.00); glVertex3d(-RWID / 2, 0.0, RLEN / 2);
				glTexCoord2d(1.00, 0.00); glVertex3d(-RWID / 2, 0.0, -
					RLEN / 2);
				glTexCoord2d(1.00, 1.00); glVertex3d(-RWID / 2, RHIGH, 
					-RLEN / 2);
				glTexCoord2d(0.00, 1.00); glVertex3d(-RWID / 2, RHIGH, 
					RLEN / 2);
			glEnd();
			//top side
			glBegin(GL_QUADS);
				glNormal3f(0.0, 1.0, 0.0);
				glTexCoord2d(0.00, 0.00); glVertex3d(RWID / 2, RHIGH, 
					RLEN / 2);
				glTexCoord2d(1.00, 0.00); glVertex3d(RWID / 2, RHIGH, 
					-RLEN / 2);
				glTexCoord2d(1.00, 1.00); glVertex3d(-RWID / 2, RHIGH,
					-RLEN / 2);
				glTexCoord2d(0.00, 1.00); glVertex3d(-RWID / 2, RHIGH, 
					RLEN / 2);
			glEnd();
		glEndList();
	}

	void obtainPrivate_RoadObject()
	{
		sTexPieces_Road.reset(new GLuint[NUM_TEXTURES_ROAD]);

		//read in piece textures
		GLTextures::ReadGLTextures(NUM_TEXTURES_ROAD, &sTexPieces_Road[Road], 
			swGlRoadTex);

		//display lists
		sLists_Road.reset(new GLuint[NUM_LISTS_ROAD]);

		//generate
		for(wxInt32 i = 0; i < NUM_LISTS_ROAD; i++)
		{
			sLists_Road[i] = glGenLists(1);
		}

		//create the lists
		buildRoad(sLists_Road[Road]);
	}

	void releasePrivate_RoadObject()
	{
		//set no texture
		glBindTexture(GL_TEXTURE_2D, 0);

		//clear piece textures
		glDeleteTextures(NUM_TEXTURES_ROAD, &sTexPieces_Road[0]);

		//clear out display lists
		glDeleteLists(sLists_Road[0], NUM_LISTS_ROAD);
	}

	void drawRoad()
	{
		glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, sTexPieces_Road[Road]);
			glCallList(sLists_Road[Road]);

			glBindTexture(GL_TEXTURE_2D, sTexPieces_Road[RoadSide]);
			glCallList(sLists_Road[RoadSide]);
		glPopMatrix();
	}
}

//---------------------------- PUBLIC           -----------------------------//
IMPLEMENT_DRAW_OBJECT(DrawRoadObject, RoadObject, STANDARD);

DrawRoadObject::DrawRoadObject(const GameObjectPtr &object)
: DrawObject(object, 10)
{
}

DrawRoadObject::~DrawRoadObject()
{
}

bool DrawRoadObject::CanSelect(const Vector &origin, const Vector &ray, 
	float &distance, wxInt32 &id, bool popup)
{
	bool hit = false;
	IGameObject *object = GetObject();

	if( (NULL != object) &&
		((object->isset(IGameObject::Select)) ||
		(true == popup)))
	{
		// Test the intersection of the ray against a bounding box around the
		// settlement.
		const Vector &o = object->coords();

		// We need to rotate the origin and ray as if our rotation was 
		// axis-aligned.
		Vector rotation = object->rotation();

		Vector orig;
		Vector temp = origin - o;

		float cosT = cosf(-rotation.y * D2R);
		float sinT = sinf(-rotation.y * D2R);

		temp = Vector((temp.x * cosT) + (temp.z * sinT), temp.y, 
			(-temp.x * sinT) + (temp.z * cosT));
		orig = o + temp;

		Vector r = Vector((ray.x * cosT) + (ray.z * sinT), ray.y,
			(-ray.x * sinT) + (ray.z * cosT));

		// The bounding box is defined by the two opposite corners.  Our origin
		// point is on the bottom of the settlement, centered between the edges.

		// Point 1 is max height, one edge of x/z away.
		Vector p1 = o + Vector(-RWID / 2.0f, 0.0f, -RLEN / 2.0f);
		Vector p2 = o + Vector(RWID / 2.0f, RHIGH, RLEN / 2.0f);

		// NOTE: If settlements were ever rotated, we'd have to rotate the corner
		// points here, but they aren't.

		Box box(p1, p2);

		if(true == box.TestIntersection(orig, r))
		{
			Vector line = o + Vector(0.0f, RHIGH, 0.0f) - orig;
			distance = line.Length();

			id = object->id();
			hit = true;
		}
	}

	return hit;
}

void DrawRoadObject::Obtain()
{
	obtainPrivate_RoadObject();
}

void DrawRoadObject::Release()
{
	releasePrivate_RoadObject();
}

void DrawRoadObject::PopupFunction(wxString& name, wxString& owner, 
	wxBitmap&, bool &createBitmap, ColorType& color)
{
	name = stRoad;

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

void DrawRoadObject::RenderOutline() const
{
}

void DrawRoadObject::RenderSelection() const
{
}

void DrawRoadObject::RenderModel(const wxInt32) const
{
	DrawObject::material(GetObject()).UseMaterial();
	drawRoad();
	DrawObject::material(White).UseMaterial();

	// If this road is part of Longest Road, also draw that.
	RoadObject *road = static_cast<RoadObject *>(GetObject());
	wxASSERT(NULL != road);
	if(true == road->longest())
	{
		DrawObject::material(LongestOutline).UseMaterial();
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		drawRoad();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//



