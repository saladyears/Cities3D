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
#include "DrawSideSelectionObject.h"
#include "SideSelectionObject.h"
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
	const float LINE_LENGTH				= X;
	const float LINE_WIDTH				= (0.75 * SCALE);
	const float LINE_HEIGHT				= 0.15f;

	//display lists
	const wxInt32 NUM_LISTS				= 1;
	GLuintArray sLists;

	void obtainPrivate_SideSelection()
	{
		//display lists
		sLists.reset(new GLuint[NUM_LISTS]);

		//generate
		for(wxInt32 i = 0; i < NUM_LISTS; i++)
		{
			sLists[i] = glGenLists(1);
		}

		//create the display list
		glNewList(sLists[0], GL_COMPILE);
			glBegin(GL_QUADS);
				glNormal3f(0.0, 1.0, 0.0);
				glVertex3d(LINE_WIDTH / 2, 0.0, LINE_LENGTH / 2);
				glVertex3d(LINE_WIDTH / 2, 0.0, -LINE_LENGTH / 2);
				glVertex3d(-LINE_WIDTH / 2, 0.0, -LINE_LENGTH / 2);
				glVertex3d(-LINE_WIDTH / 2, 0.0, LINE_LENGTH / 2);
			glEnd();
		glEndList();
	}

	void releasePrivate_SideSelection()
	{
		//clear out display lists
		glDeleteLists(sLists[0], NUM_LISTS);
	}

	void drawObjects(const DrawSideSelectionObject *pObject, 
		const wxInt32 selection)
	{
		SideSelectionObject *pSelection = 
			static_cast<SideSelectionObject *>(pObject->GetObject());

		SideSelectionObject::SideSelectionList::const_iterator it, 
			itEnd(pSelection->selection_end());

		//set the draw color
		DrawObject::material(pSelection->color()).UseMaterial();

		//no textures here
		glDisable(GL_TEXTURE_2D);

		wxInt32 id;
		Vector coords;
		float rotation;

		for(it = pSelection->selection_begin(); it != itEnd; ++it)
		{
			boost::tie(id, coords, rotation) = (*it);

			glPushMatrix();
				glTranslated(coords.x, LINE_HEIGHT, coords.z);
				glRotated(rotation, 0.0, 1.0, 0.0);
				
				glCallList(sLists[0]);
				
				//draw the selection if needed
				if(selection == id)
				{
					//set the outline material
					DrawObject::material(DrawObject::Selection).UseMaterial();

					//set an outline
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					glCallList(sLists[0]);
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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
IMPLEMENT_DRAW_OBJECT(DrawSideSelectionObject, SideSelectionObject, CORE);

DrawSideSelectionObject::DrawSideSelectionObject(const GameObjectPtr &object)
: DrawObject(object, 1)
{
}

DrawSideSelectionObject::~DrawSideSelectionObject()
{
}

bool DrawSideSelectionObject::CanSelect(const Vector &origin, 
										const Vector &ray, float &distance, 
										wxInt32 &id, bool popup)
{
	SideSelectionObject *pSelection = 
		static_cast<SideSelectionObject *>(GetObject());

	//sanity
	if( (NULL == pSelection) ||
		(true == popup))
	{
		return false;
	}

	float rayLength;
	Vector plane;

	//calculate where the ray, coming from the origin, intersects the x-z 
	//plane (plus the DISKHEIGHT), which gives the coordinates where we need 
	//to look for an intersecting circle
	UtilityGL::planeCoords(LINE_HEIGHT, origin, ray, plane, rayLength);

	SideSelectionObject::SideSelectionList::const_iterator it = 
		pSelection->selection_begin(), itEnd = pSelection->selection_end();

	wxInt32 objectid;
	Vector coords, unrotated, corner, hit;
	float rotation, hitlength;
	float dot1, dot2, length1, length2;
	float cosT, sinT;

	for(; it != itEnd; ++it)
	{
		boost::tie(objectid, coords, rotation) = (*it);

		//determine the unrotated position of the hit around the center point 
		//of the road segment, which makes it a hella lot easier to figure out 
		//if the hit point is within the road segment
		unrotated = coords - plane;

		cosT = cosf(-rotation * D2R);
		sinT = sinf(-rotation * D2R);

		unrotated = coords + Vector((unrotated.x * cosT) + 
			(unrotated.z * sinT), 0.0f, (-unrotated.x * sinT) + 
			(unrotated.z * cosT));

		//now that we have the unrotated position, we can use vector math to
		//determine if that position lies within the road length

		//we set the origin to the the upper-left corner of the road segment, 
		//which means that the vectors (LINE_WIDTH, 0.0, 0.0) and (0.0, 0.0, 
		//LINE_LENGTH) now describe the edges of the road segment from the 
		//origin
		corner = coords - Vector(LINE_WIDTH / 2, LINE_HEIGHT, LINE_LENGTH / 2);

		//create a vector that is the origin (the upper-left corner) to the hit
		//point; we can use this vector in conjuction with the edge vectors to 
		//determine if the hit point lies within the edges or not
		hit = unrotated - corner;

		//save off the length of the hit vector for later use
		hitlength = hit.Length();

		//normalize the hit vector so that finding the dot product
		//can be accurately used
		hit.Normalize();

		//now find the dot products between the edge vectors (which are just
		//unit vectors along the X and Z axis when normalized) and the
		//hit vector
		dot1 = DotProduct(hit, Vector(1.0f, 0.0f, 0.0f));
		dot2 = DotProduct(hit, Vector(0.0f, 0.0f, 1.0f));
		
		//finally, find the length of the hit vector projected along the
		//edge vectors--if either of these lengths is longer than the edge
		//vectors, then we know that they are outside the edge of the vector
		length1 = hitlength * dot1;
		length2 = hitlength * dot2;

		//if both dot products are positive, and the projection lengths
		//are shorter than their bounds, we have a winner
		if( (0.0 <= dot1) &&
			(0.0 <= dot2) &&
			(length1 <= LINE_WIDTH) &&
			(length2 <= LINE_LENGTH))
		{
			id = objectid;
			distance = rayLength;
			return true;
		}
	}

	return false;
}

void DrawSideSelectionObject::Obtain()
{
	obtainPrivate_SideSelection();
}

void DrawSideSelectionObject::Release()
{
	releasePrivate_SideSelection();
}

void DrawSideSelectionObject::RenderOutline() const
{
	//should never be called
	wxASSERT(false);
}

void DrawSideSelectionObject::RenderSelection() const
{
	//should never be called
	wxASSERT(false);
}

void DrawSideSelectionObject::RenderModel(const wxInt32 selection) const
{
	drawObjects(this, selection);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//



