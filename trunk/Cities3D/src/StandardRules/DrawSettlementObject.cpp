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
#include "DrawSettlementObject.h"
#include "SettlementObject.h"
#include "GLMaterial.h"
#include "GLTextures.h"
#include "Box.h"
#include "RuleSetDatabase.h"
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
	const wxInt32 NUM_TEXTURES_SETTLEMENT			= 2;
	GLuintArray sTexPieces_Settlement;

	//display lists
	const wxInt32 NUM_LISTS_SETTLEMENT				= 2;
	GLuintArray sLists_Settlement;

	enum
	{
		Settlement			= 0,
		SettlementSide,
	};

	//settlement dimensions
	const float SLEN				= (1.4 * SCALE);
	const float SWID				= (1.0 * SCALE);
	const float SHIGH				= (0.7 * SCALE);
	const float SPEAK				= (1.2 * SCALE);

	//a house-shaped settlement
	void buildSettlement(GLuint id)
	{
		//build the settlement
		glNewList(id, GL_COMPILE);
			//front face
			glBegin(GL_POLYGON);
				glNormal3f(0.0, 0.0, 1.0);
				glTexCoord2d(0.00, 0.00); glVertex3d(SWID / 2, 0.0, SLEN / 2);
				glTexCoord2d(1.00, 0.00); glVertex3d(-SWID / 2, 0.0, SLEN / 2);
				glTexCoord2d(1.00, 0.58); glVertex3d(-SWID / 2, SHIGH, 
					SLEN / 2);
				glTexCoord2d(0.50, 0.98); glVertex3d(0.0, SPEAK, SLEN / 2);
				glTexCoord2d(0.00, 0.58); glVertex3d(SWID / 2, SHIGH, 
					SLEN / 2);
				glTexCoord2d(0.00, 0.00); glVertex3d(SWID / 2, 0.0, SLEN / 2);
			glEnd();
			//back face
			glBegin(GL_POLYGON);
				glNormal3f(0.0, 0.0, -1.0);
				glTexCoord2d(0.00, 0.00); glVertex3d(SWID / 2, 0.0, -SLEN / 2);
				glTexCoord2d(1.00, 0.00); glVertex3d(-SWID / 2, 0.0, -
					SLEN / 2);
				glTexCoord2d(1.00, 0.58); glVertex3d(-SWID / 2, SHIGH, 
					-SLEN / 2);
				glTexCoord2d(0.50, 0.98); glVertex3d(0.0, SPEAK, -SLEN / 2);
				glTexCoord2d(0.00, 0.58); glVertex3d(SWID / 2, SHIGH, 
					-SLEN / 2);
				glTexCoord2d(0.00, 0.00); glVertex3d(SWID / 2, 0.0, -SLEN / 2);
			glEnd();
		glEndList();

		glNewList(id + 1, GL_COMPILE);
			//right side face
			glBegin(GL_POLYGON);
				glNormal3f(1.0, 0.0, 0.0);
				glTexCoord2d(0.00, 0.00); glVertex3d(SWID / 2, 0.0, SLEN / 2);
				glTexCoord2d(0.00, 1.00); glVertex3d(SWID / 2, 0.0, -SLEN / 2);
				glTexCoord2d(1.00, 1.00); glVertex3d(SWID / 2, SHIGH, 
					-SLEN / 2);
				glTexCoord2d(1.00, 0.00); glVertex3d(SWID / 2, SHIGH, 
					SLEN / 2);
				glTexCoord2d(0.00, 0.00); glVertex3d(SWID / 2, 0.0, SLEN / 2);
			glEnd();
			//left side face
			glBegin(GL_POLYGON);
				glNormal3f(-1.0, 0.0, 0.0);
				glTexCoord2d(0.00, 0.00); glVertex3d(-SWID / 2, 0.0, SLEN / 2);
				glTexCoord2d(0.00, 1.00); glVertex3d(-SWID / 2, 0.0, 
					-SLEN / 2);
				glTexCoord2d(1.00, 1.00); glVertex3d(-SWID / 2, SHIGH,
					-SLEN / 2);
				glTexCoord2d(1.00, 0.00); glVertex3d(-SWID / 2, SHIGH, 
					SLEN / 2);
				glTexCoord2d(0.00, 0.00); glVertex3d(-SWID / 2, 0.0, SLEN / 2);
			glEnd();
			//top right side
			glBegin(GL_POLYGON);
				glNormal3f(1.0, 1.0, 0.0);
				glTexCoord2d(1.00, 1.00); glVertex3d(SWID / 2, SHIGH, 
					SLEN / 2);
				glTexCoord2d(0.00, 1.00); glVertex3d(0.0, SPEAK, SLEN / 2);
				glTexCoord2d(0.00, 0.00); glVertex3d(0.0, SPEAK, -SLEN / 2);
				glTexCoord2d(1.00, 0.00); glVertex3d(SWID / 2, SHIGH, 
					-SLEN / 2);
				glTexCoord2d(1.00, 1.00); glVertex3d(SWID / 2, SHIGH, 
					SLEN / 2);
			glEnd();
			//top left side
			glBegin(GL_POLYGON);
				glNormal3f(-1.0, 1.0, 0.0);
				glTexCoord2d(0.00, 0.00); glVertex3d(-SWID / 2, SHIGH, 
					SLEN / 2);
				glTexCoord2d(1.00, 0.00); glVertex3d(0.0, SPEAK, SLEN / 2);
				glTexCoord2d(1.00, 1.00); glVertex3d(0.0, SPEAK, -SLEN / 2);
				glTexCoord2d(0.00, 1.00); glVertex3d(-SWID / 2, SHIGH, 
					-SLEN / 2);
				glTexCoord2d(0.00, .00); glVertex3d(-SWID / 2, SHIGH, 
					SLEN / 2);
			glEnd();
		glEndList();
	}

	void obtainPrivate_SettlementObject()
	{
		sTexPieces_Settlement.reset(new GLuint[NUM_TEXTURES_SETTLEMENT]);

		//read in piece textures
		GLTextures::ReadGLTextures(NUM_TEXTURES_SETTLEMENT, 
			&sTexPieces_Settlement[0], swGlSettlementTex);
		
		//display lists
		sLists_Settlement.reset(new GLuint[NUM_LISTS_SETTLEMENT]);

		//generate
		for(wxInt32 i = 0; i < NUM_LISTS_SETTLEMENT; i++)
		{
			sLists_Settlement[i] = glGenLists(1);
		}

		//create the lists
		buildSettlement(sLists_Settlement[Settlement]);
	}

	void releasePrivate_SettlementObject()
	{
		//set no texture
		glBindTexture(GL_TEXTURE_2D, 0);

		//clear piece textures
		glDeleteTextures(NUM_TEXTURES_SETTLEMENT, &sTexPieces_Settlement[0]);

		//clear out display lists
		glDeleteLists(sLists_Settlement[0], NUM_LISTS_SETTLEMENT);
	}

	void drawSettlement()
	{
		glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, sTexPieces_Settlement[Settlement]);
			glCallList(sLists_Settlement[Settlement]);

			glBindTexture(GL_TEXTURE_2D, sTexPieces_Settlement[SettlementSide]);
			glCallList(sLists_Settlement[SettlementSide]);
		glPopMatrix();
	}
}

//---------------------------- PUBLIC           -----------------------------//
IMPLEMENT_DRAW_OBJECT(DrawSettlementObject, SettlementObject, STANDARD);

DrawSettlementObject::DrawSettlementObject(const GameObjectPtr &object)
: DrawObject(object, 10)
{
}

DrawSettlementObject::~DrawSettlementObject()
{
}

bool DrawSettlementObject::CanSelect(const Vector &origin, const Vector &ray, 
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

		// The bounding box is defined by the two opposite corners.  Our origin
		// point is on the bottom of the settlement, centered between the edges.

		// Point 1 is max height, one edge of x/z away.
		Vector p1 = o + Vector(-SWID / 2.0f, 0.0f, -SLEN / 2.0f);
		Vector p2 = o + Vector(SWID / 2.0f, SPEAK, SLEN / 2.0f);

		// NOTE: If settlements were ever rotated, we'd have to rotate the corner
		// points here, but they aren't.

		Box box(p1, p2);

		if(true == box.TestIntersection(origin, ray))
		{
			Vector line = o + Vector(0.0f, SHIGH, 0.0f) - origin;
			distance = line.Length();

			id = object->id();
			hit = true;
		}
	}

	return hit;
}

void DrawSettlementObject::Obtain()
{
	obtainPrivate_SettlementObject();
}

void DrawSettlementObject::Release()
{
	releasePrivate_SettlementObject();
}

void DrawSettlementObject::PopupFunction(wxString& name, wxString& owner, 
	wxBitmap&, bool &createBitmap, ColorType& color)
{
	name = stSettlement;

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

void DrawSettlementObject::RenderOutline() const
{
	DrawObject::material(Gold).UseMaterial();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	drawSettlement();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void DrawSettlementObject::RenderSelection() const
{
	DrawObject::material(Selection).UseMaterial();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	drawSettlement();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void DrawSettlementObject::RenderModel(const wxInt32) const
{
	DrawObject::material(GetObject()).UseMaterial();
	drawSettlement();
	DrawObject::material(White).UseMaterial();
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//


