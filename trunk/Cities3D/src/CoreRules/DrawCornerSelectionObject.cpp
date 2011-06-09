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
#include "DrawCornerSelectionObject.h"
#include "CornerSelectionObject.h"
#include "RuleSetDatabase.h"
#include "GLMaterial.h"
#include "UtilityGL.h"
#include "define/defineGL.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	const float DISKRAD					= (0.8 * SCALE);
	const float DISKHEIGHT				= 0.15f;

	GLUquadricObj *sQuad;

	void obtainPrivate_CornerSelection()
	{
		//create the object
		sQuad = gluNewQuadric();
	}

	void releasePrivate_CornerSelection()
	{
		//clean up the quadric
		gluDeleteQuadric(sQuad);
	}

	void drawObjects(const DrawCornerSelectionObject * pObject, 
		const wxInt32 selection)
	{
		CornerSelectionObject *pSelection = 
			static_cast<CornerSelectionObject *>(pObject->GetObject());
		
		CornerSelectionObject::CornerSelectionList::const_iterator it(
			pSelection->selection_begin()), itEnd(pSelection->selection_end());

		//set the draw color
		DrawObject::material(pSelection->color()).UseMaterial();

		//no textures here
		glDisable(GL_TEXTURE_2D);

		for(; it != itEnd; ++it)
		{
			glPushMatrix();
				glTranslated(it->second.x, DISKHEIGHT, it->second.z);
				glRotated(90.0, -1.0, 0.0, 0.0);
				gluDisk(sQuad, 0.0, DISKRAD, 25, 5);

				//draw the selection ring if needed
				if(selection == it->first)
				{
					//set the outline material
					DrawObject::material(DrawObject::Selection).UseMaterial();

					//draw the secondary selection disk
					gluDisk(sQuad, DISKRAD, DISKRAD + 0.5, 20, 5);			

					//reset the original material
					DrawObject::material(pSelection->color()).UseMaterial();
				}

			glPopMatrix();
		}

		//reset to white
		DrawObject::material(DrawObject::White).UseMaterial();
		glEnable(GL_TEXTURE_2D);
	}
}

//---------------------------- PUBLIC           -----------------------------//
IMPLEMENT_DRAW_OBJECT(DrawCornerSelectionObject, CornerSelectionObject, CORE);

DrawCornerSelectionObject::DrawCornerSelectionObject(
	const GameObjectPtr &object)
: DrawObject(object, 1)
{
}

DrawCornerSelectionObject::~DrawCornerSelectionObject()
{
}

bool DrawCornerSelectionObject::CanSelect(const Vector &origin, 
										  const Vector &ray, float &distance, 
										  wxInt32 &id, bool popup)
{
	CornerSelectionObject *pSelection = static_cast<CornerSelectionObject *>(
		GetObject());

	//sanity
	if( (NULL == pSelection) ||
		(true == popup))
	{
		return false;
	}

	float rayLength;
	Vector plane;

	//calculate where the ray, coming from the origin, intersects the x-z plane
	//(plus the DISKHEIGHT), which gives the coordinates where we need to look 
	//for an intersecting circle
	UtilityGL::planeCoords(DISKHEIGHT, origin, ray, plane, rayLength);

	//iterate through each of the draw corners, looking for one that contains
	//the x-z coordinates
	CornerSelectionObject::CornerSelectionList::const_iterator it(
		pSelection->selection_begin()), itEnd(pSelection->selection_end());

	//use an optimization to avoid having to calculate square roots
	float radSquared = DISKRAD * DISKRAD;
	float localDist, xDist, zDist;

	for(; it != itEnd; ++it)
	{
		xDist = it->second.x - plane.x;
		zDist = it->second.z - plane.z;

		//calculate the local distance between the circle point and the plane 
		//point
		localDist = (xDist * xDist) + (zDist * zDist);

		//if it's within the chip radius amount, we have one
		if(radSquared >= localDist)
		{
			id = it->first;
			distance = rayLength;
			return true;
		}
	}

	return false;
}

void DrawCornerSelectionObject::Obtain()
{
	obtainPrivate_CornerSelection();
}

void DrawCornerSelectionObject::Release()
{
	releasePrivate_CornerSelection();
}

void DrawCornerSelectionObject::RenderOutline() const
{
	//should never be called
	wxASSERT(false);
}

void DrawCornerSelectionObject::RenderSelection() const
{
	//should never be called
	wxASSERT(false);
}

void DrawCornerSelectionObject::RenderModel(const wxInt32 selection) const
{
	drawObjects(this, selection);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//



