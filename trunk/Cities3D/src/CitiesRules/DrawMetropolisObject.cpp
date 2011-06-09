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
#include "DrawMetropolisObject.h"
#include "MetropolisObject.h"
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
	// Textures and Display Lists.
	GLuintArray sLists_Metropolis;

	enum
	{
		MetropolisSide		= 0,
		MetropolisTop		,

		MetropolisCount		,
	};

	// Dimensions.
	const float CLEN				= (2.0 * SCALE);
	const float CWID				= (1.0 * SCALE);
	const float CLOW				= (0.8 * SCALE);
	const float CHIGH				= (1.5 * SCALE);
	const float CPEAK				= (2.0 * SCALE);

	const float MMWID				= (1.0 * SCALE);	// front middle width
	const float MSWID				= (0.8 * SCALE);	// front side width
	const float MDEPTH				= (0.8 * SCALE);	// depth
	const float MSHIGH				= (2.0 * SCALE);	// front side height
	const float MSPEAK				= (2.4 * SCALE);	// front side peak
	const float MMLOW				= (1.0 * SCALE);	// front middle low
	const float MMHIGH				= (1.6 * SCALE);	// front middle high

	const float MTWID				= MMWID + (2 * MSWID);	// total width

	GLMaterial sYellow;
	GLMaterial sBlue;
	GLMaterial sGreen;

	// Geometry.
	void buildMetropolis(GLuint id)
	{
		double dTW = MTWID / 2;
		double dTD = MDEPTH / 2;

		glNewList(id, GL_COMPILE);
			// Front face.  Two 'pillars' on each side with a central bar.
			glBegin(GL_POLYGON);
				glNormal3f(0.0, 0.0, 1.0);
				glVertex3d(dTW, 0.0, dTD);
				glVertex3d(dTW - MSWID, 0.0, dTD);
				glVertex3d(dTW - MSWID, MSHIGH, dTD);
				glVertex3d(dTW - (MSWID / 2), MSPEAK, dTD);
				glVertex3d(dTW, MSHIGH, dTD);
			glEnd();
			glBegin(GL_POLYGON);
				glNormal3f(0.0, 0.0, 1.0);
				glVertex3d(-dTW, 0.0, dTD);
				glVertex3d(-dTW + MSWID, 0.0, dTD);
				glVertex3d(-dTW + MSWID, MSHIGH, dTD);
				glVertex3d(-dTW + (MSWID / 2), MSPEAK, dTD);
				glVertex3d(-dTW, MSHIGH, dTD);
			glEnd();
			glBegin(GL_QUADS);
				glNormal3f(0.0, 0.0, 1.0);
				glVertex3d(dTW - MSWID, MMLOW, dTD);
				glVertex3d(-dTW + MSWID, MMLOW, dTD);
				glVertex3d(-dTW + MSWID, MMHIGH, dTD);
				glVertex3d(dTW - MSWID, MMHIGH, dTD);
			glEnd();
		
			// Back face.
			glBegin(GL_POLYGON);
				glNormal3f(0.0, 0.0, 1.0);
				glVertex3d(dTW, 0.0, -dTD);
				glVertex3d(dTW - MSWID, 0.0, -dTD);
				glVertex3d(dTW - MSWID, MSHIGH, -dTD);
				glVertex3d(dTW - (MSWID / 2), MSPEAK, -dTD);
				glVertex3d(dTW, MSHIGH, -dTD);
			glEnd();
			glBegin(GL_POLYGON);
				glNormal3f(0.0, 0.0, 1.0);
				glVertex3d(-dTW, 0.0, -dTD);
				glVertex3d(-dTW + MSWID, 0.0, -dTD);
				glVertex3d(-dTW + MSWID, MSHIGH, -dTD);
				glVertex3d(-dTW + (MSWID / 2), MSPEAK, -dTD);
				glVertex3d(-dTW, MSHIGH, -dTD);
			glEnd();
			glBegin(GL_QUADS);
				glNormal3f(0.0, 0.0, 1.0);
				glVertex3d(dTW - MSWID, MMLOW, -dTD);
				glVertex3d(-dTW + MSWID, MMLOW, -dTD);
				glVertex3d(-dTW + MSWID, MMHIGH, -dTD);
				glVertex3d(dTW - MSWID, MMHIGH, -dTD);
			glEnd();
		glEndList();

		glNewList(id + 1, GL_COMPILE);
			// Right side face.
			glBegin(GL_QUADS);
				glNormal3f(1.0, 0.0, 0.0);
				glVertex3d(dTW, 0.0, -dTD);
				glVertex3d(dTW, 0.0, dTD);
				glVertex3d(dTW, MSHIGH, dTD);
				glVertex3d(dTW, MSHIGH, -dTD);
			glEnd();
			// Left side face.
			glBegin(GL_QUADS);
				glNormal3f(-1.0, 0.0, 0.0);
				glVertex3d(-dTW, 0.0, -dTD);
				glVertex3d(-dTW, 0.0, dTD);
				glVertex3d(-dTW, MSHIGH, dTD);
				glVertex3d(-dTW, MSHIGH, -dTD);
			glEnd();
			// Top face.
			glBegin(GL_QUADS);
				glNormal3f(0.0, 1.0, 0.0);
				glVertex3d(dTW - MSWID, MMHIGH, -dTD);
				glVertex3d(dTW - MSWID, MMHIGH, dTD);
				glVertex3d(-dTW + MSWID, MMHIGH, dTD);
				glVertex3d(-dTW + MSWID, MMHIGH, -dTD);
			glEnd();
			// Right side top face 1.
			glBegin(GL_QUADS);
				glNormal3f(1.0, 1.0, 0.0);
				glVertex3d(dTW, MSHIGH, -dTD);
				glVertex3d(dTW, MSHIGH, dTD);
				glVertex3d(dTW - (MSWID / 2), MSPEAK, dTD);
				glVertex3d(dTW - (MSWID / 2), MSPEAK, -dTD);		
			glEnd();
			// Right side top face 2.
			glBegin(GL_QUADS);
				glNormal3f(-1.0, 1.0, 0.0);
				glVertex3d(dTW - (MSWID / 2), MSPEAK, -dTD);
				glVertex3d(dTW - (MSWID / 2), MSPEAK, dTD);
				glVertex3d(dTW - MSWID, MSHIGH, dTD);
				glVertex3d(dTW - MSWID, MSHIGH, -dTD);
			glEnd();
			// Left side top face 1.
			glBegin(GL_QUADS);
				glNormal3f(-1.0, 1.0, 0.0);
				glVertex3d(-dTW, MSHIGH, -dTD);
				glVertex3d(-dTW, MSHIGH, dTD);
				glVertex3d(-dTW + (MSWID / 2), MSPEAK, dTD);
				glVertex3d(-dTW + (MSWID / 2), MSPEAK, -dTD);		
			glEnd();
			// Left side top face 2.
			glBegin(GL_QUADS);
				glNormal3f(1.0, 1.0, 0.0);
				glVertex3d(-dTW + (MSWID / 2), MSPEAK, -dTD);
				glVertex3d(-dTW + (MSWID / 2), MSPEAK, dTD);
				glVertex3d(-dTW + MSWID, MSHIGH, dTD);
				glVertex3d(-dTW + MSWID, MSHIGH, -dTD);
			glEnd();
			// Inner right-side face.
			glBegin(GL_QUADS);
				glNormal3f(-1.0, 0.0, 0.0);
				glVertex3d(dTW - MSWID, 0.0, -dTD);
				glVertex3d(dTW - MSWID, 0.0, dTD);
				glVertex3d(dTW - MSWID, MMLOW, dTD);
				glVertex3d(dTW - MSWID, MMLOW, -dTD);
			glEnd();
			// Inner left-side face.
			glBegin(GL_QUADS);
				glNormal3f(1.0, 0.0, 0.0);
				glVertex3d(-dTW + MSWID, 0.0, -dTD);
				glVertex3d(-dTW + MSWID, 0.0, dTD);
				glVertex3d(-dTW + MSWID, MMLOW, dTD);
				glVertex3d(-dTW + MSWID, MMLOW, -dTD);
			glEnd();
		glEndList();
	}

	void obtainPrivate_Metropolis()
	{
		//display lists
		sLists_Metropolis.reset(new GLuint[MetropolisCount]);

		//generate
		for(wxInt32 i = 0; i < MetropolisCount; i++)
		{
			sLists_Metropolis[i] = glGenLists(1);
		}

		//create the lists
		buildMetropolis(sLists_Metropolis[MetropolisSide]);

		// Create the GL materials for the three types.
		sYellow.SetMaterial(wxColour(0, 0, 0));
		sYellow.SetProperty(GLMaterial::Emissive, wxColour(255, 255, 0));
		sBlue.SetMaterial(wxColour(0, 0, 0));
		sBlue.SetProperty(GLMaterial::Emissive, wxColour(0, 0, 170));
		sGreen.SetMaterial(wxColour(0, 0, 0));
		sGreen.SetProperty(GLMaterial::Emissive, wxColour(0, 75, 0));
	}

	void releasePrivate_Metropolis()
	{
		//clear out display lists
		glDeleteLists(sLists_Metropolis[0], MetropolisCount);

	}

	void drawMetropolis()
	{
		glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, 0);
			glCallList(sLists_Metropolis[MetropolisSide]);

			glCallList(sLists_Metropolis[MetropolisTop]);
		glPopMatrix();
	}
}

//---------------------------- PUBLIC           -----------------------------//
IMPLEMENT_DRAW_OBJECT(DrawMetropolisObject, MetropolisObject, CITIES);

DrawMetropolisObject::DrawMetropolisObject(const GameObjectPtr &object)
: DrawObject(object, 20)
{
}

DrawMetropolisObject::~DrawMetropolisObject()
{
}

void DrawMetropolisObject::Obtain()
{
	obtainPrivate_Metropolis();
}

void DrawMetropolisObject::Release()
{
	releasePrivate_Metropolis();
}

bool DrawMetropolisObject::CanSelect(const Vector &origin, const Vector &ray, 
	float &distance, wxInt32 &id, bool popup)
{
	bool hit = false;

	return hit;
}

void DrawMetropolisObject::PopupFunction(wxString& name, wxString& owner, 
	wxBitmap&, bool &createBitmap, ColorType& color)
{
    /*
    // TODO: Get the type of metropolis.
	name = stMetropolis;

	IGameObject *object = GetObject();
	wxASSERT(NULL != object);

	color = object->color();

	createBitmap = true;

	// Get the owner's name.
	wxInt32 ownerIndex = object->owner();
	DataObject input(ownerIndex), output;
	RULE.Decide(shLogicPlayerName, input, output);
	owner = output.read<wxString>();
    */
}

void DrawMetropolisObject::RenderOutline() const
{
}

void DrawMetropolisObject::RenderSelection() const
{
}

void DrawMetropolisObject::RenderModel(const wxInt32) const
{
	MetropolisObject* object = static_cast<MetropolisObject*>(GetObject());
	MetropolisObject::Type type = object->type();

	switch(type)
	{
	case MetropolisObject::Trade:
		sYellow.UseMaterial();
		break;
	case MetropolisObject::Politics:
		sBlue.UseMaterial();
		break;
	case MetropolisObject::Science:
		sGreen.UseMaterial();
		break;
	default:
		wxASSERT(false);
		break;
	}

	drawMetropolis();
	DrawObject::material(White).UseMaterial();
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//


