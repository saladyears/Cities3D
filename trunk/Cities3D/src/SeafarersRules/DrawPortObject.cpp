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
#include "DrawPortObject.h"
#include "PortObject.h"
#include "GLTextures.h"
#include "GLMaterial.h"
#include "ImageEngine.h"
#include "RuleSetDatabase.h"
#include "ConfigDatabase.h"
#include "define/defineGL.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	WX_DECLARE_HASH_STRING_HASH_MAP(GLuint, GLuintHash);
	GLuintHash sTexHash;

	//display lists
	const wxInt32 NUM_LISTS_PORT				= 2;

	enum
	{
		Seaport			= 0,
		SeaportEdge,
	};

	//tile edge depth
	const float TILE_DEPTH			= 0.23f;

	GLuintArray sLists;

	//crazy shaped seaports
	void buildSeaport(GLuint id)
	{
		wxInt32 i;
		wxInt32 iSlices = 20;
		double dRadius = 1.01;
		double fWidth = X * .91;
		double fLength = X * 1.14;
		double dTheta;
		double dX, dZ;
		double dXConvert;

		//calculate conversion
		dXConvert = dRadius / fWidth;

		//calculate sea port
		glNewList(id, GL_COMPILE);
			
			glBegin(GL_POLYGON);

				//set the normal
				glNormal3f(0.0, 1.0, 0.0);
			
				//create the circle
				for(i = 0; i < iSlices; i++)
				{
					//calculate theta
					dTheta = ((360. / (iSlices + 3)) * i) + 122;

					//convert to radians
					dTheta *= D2R;

					//calc coordinates
					dX = dRadius * cos(dTheta);
					dZ = dRadius * sin(dTheta);

					//calculate texture coordinates
					glTexCoord2d(0.5 - (dX * dXConvert), 0.2 + 
						(dZ * dXConvert));
					
					//set the point
					glVertex3d(dX, TILE_DEPTH, dZ - ((fLength / 2) - dRadius));
				}

				//create the angled part
				glTexCoord2d(0.00, 1.00); glVertex3d(fWidth / 2, TILE_DEPTH, 
					fLength / 2);
				glTexCoord2d(1.00, 1.00); glVertex3d(-fWidth / 2, TILE_DEPTH, 
					fLength / 2);

			glEnd();

		//finish up
		glEndList();

		//build the seaport edge list
		glNewList(id + 1, GL_COMPILE);

			glBegin(GL_QUAD_STRIP);

				//create the circle
				for(i = 0; i < iSlices; i++)
				{
					//calculate theta
					dTheta = ((360. / (iSlices + 3)) * i) + 122;

					//convert to radians
					dTheta *= D2R;

					//calc coordinates
					dX = dRadius * cos(dTheta);
					dZ = dRadius * sin(dTheta);

					//no texture coords
					
					//set the point
					glVertex3d(dX, 0.0, dZ - ((fLength / 2) - dRadius));
					glVertex3d(dX, TILE_DEPTH, dZ - ((fLength / 2) - dRadius));
				}

				//create the angled part
				glVertex3d(fWidth / 2, 0.00, fLength / 2);
				glVertex3d(fWidth / 2, TILE_DEPTH, fLength / 2);

				glVertex3d(-fWidth / 2, 0.00, fLength / 2);
				glVertex3d(-fWidth / 2, TILE_DEPTH, fLength / 2);

				//end at the first
				dTheta = 122 * D2R;

				//calc coordinates
				dX = dRadius * cos(dTheta);
				dZ = dRadius * sin(dTheta);

				//set the point
				glVertex3d(dX, 0.0, dZ - ((fLength / 2) - dRadius));
				glVertex3d(dX, TILE_DEPTH, dZ - ((fLength / 2) - dRadius));

			glEnd();

		//finish up
		glEndList();
	}

	void obtainPrivate_Port()
	{
		//grab all tile images from the image database
		ImageEngine::ImagePairArray images;
		IMAGE.GetAllImages(shPortType, images);

		//now convert them all into GL textures
		wxBOOL mipmap = CONFIG.read<wxInt32>(swConfigSectionOpenGL, swMipmap, TRUE);
		ImageEngine::ImagePairArray::const_iterator it, itEnd(images.end());
		for(it = images.begin(); it != itEnd; ++it)
		{
			sTexHash[it->first] = GLTextures::BuildTexture(
				it->second, mipmap || false);
		}
		
		//display lists
		sLists.reset(new GLuint[NUM_LISTS_PORT]);

		//generate
		for(wxInt32 i = 0; i < NUM_LISTS_PORT; i++)
		{
			sLists[i] = glGenLists(1);
		}

		//create the lists
		buildSeaport(sLists[Seaport]);
	}

	void releasePrivate_Port()
	{
		//set no texture
		glBindTexture(GL_TEXTURE_2D, 0);

		//clear out the old textures
		GLuintHash::iterator it, itEnd(sTexHash.end());
		for(it = sTexHash.begin(); it != itEnd; ++it)
		{
			glDeleteTextures(1, &(it->second));
		}
		
		//clear out display lists
		glDeleteLists(sLists[0], NUM_LISTS_PORT);
	}

	void drawSeaport(const DrawPortObject * const pObject)
	{
		PortObject *pTile = static_cast<PortObject *>(pObject->GetObject());

		HashString type = pTile->type();

		//sanity
		GLuintHash::iterator it(sTexHash.find(type));
		wxASSERT(sTexHash.end() != it);

		glBindTexture(GL_TEXTURE_2D, sTexHash[type]);

		glPushMatrix();
			glTranslated(0.0, 0.0, 1.0);
			glCallList(sLists[Seaport]);
			glCallList(sLists[SeaportEdge]);
		glPopMatrix();
	}
}

//---------------------------- PUBLIC           -----------------------------//
IMPLEMENT_DRAW_OBJECT(DrawPortObject, PortObject, SEAFARERS);

DrawPortObject::DrawPortObject(const GameObjectPtr &object)
: DrawObject(object, 10)
{
}

DrawPortObject::~DrawPortObject()
{
}

void DrawPortObject::Obtain()
{
	obtainPrivate_Port();
}

void DrawPortObject::Release()
{
	releasePrivate_Port();
}

void DrawPortObject::RenderOutline() const
{
}

void DrawPortObject::RenderSelection() const
{
}

void DrawPortObject::RenderModel(const wxInt32 selection) const
{
	(void) selection;

	drawSeaport(this);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//


