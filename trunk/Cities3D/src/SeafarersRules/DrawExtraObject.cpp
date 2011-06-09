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
#include "DrawExtraObject.h"
#include "ExtraObject.h"
#include "GLMaterial.h"
#include "GLTextures.h"
#include "RuleSetDatabase.h"
#include "define/defineGL.h"
#include "PlayerGame.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	//textures
	const wxInt32 NUM_TEXTURES_EXTRA			= 1;
	GLuintArray sTexPieces_Extra;

	//display lists
	const wxInt32 NUM_LISTS_EXTRA				= 1;
	GLuintArray sLists_Extra;

	enum
	{
		Extra			= 0,
	};

	//extra dimensions
	const float EXTRAWID			= (1.95 * SCALE);
	const float EXTRADEPTH			= 0.10f;

	//the small extra point under settlements/cities in Seafarers
	void buildExtra(GLuint id)
	{
		glNewList(id, GL_COMPILE);
			//top face
			glBegin(GL_QUADS);
				glTexCoord2d(1.00, 0.00); glVertex3d(-EXTRAWID / 2, EXTRADEPTH,
					-EXTRAWID / 2);
				glTexCoord2d(0.00, 0.00); glVertex3d(EXTRAWID / 2, EXTRADEPTH,
					-EXTRAWID / 2);
				glTexCoord2d(0.00, 1.00); glVertex3d(EXTRAWID / 2, EXTRADEPTH,
					EXTRAWID / 2);
				glTexCoord2d(1.00, 1.00); glVertex3d(-EXTRAWID / 2, EXTRADEPTH,
					EXTRAWID / 2);
			glEnd();

			//side faces (no tex coords)
			glBegin(GL_QUADS);
				glVertex3d(-EXTRAWID / 2, 0.0, -EXTRAWID / 2);
				glVertex3d(-EXTRAWID / 2, EXTRADEPTH, -EXTRAWID / 2);
				glVertex3d(-EXTRAWID / 2, EXTRADEPTH, EXTRAWID / 2);
				glVertex3d(-EXTRAWID / 2, 0.0, EXTRAWID / 2);

				glVertex3d(EXTRAWID / 2, 0.0, -EXTRAWID / 2);
				glVertex3d(EXTRAWID / 2, EXTRADEPTH, -EXTRAWID / 2);
				glVertex3d(EXTRAWID / 2, EXTRADEPTH, EXTRAWID / 2);
				glVertex3d(EXTRAWID / 2, 0.0, EXTRAWID / 2);

				glVertex3d(-EXTRAWID / 2, 0.0, -EXTRAWID / 2);
				glVertex3d(-EXTRAWID / 2, EXTRADEPTH, -EXTRAWID / 2);
				glVertex3d(EXTRAWID / 2, EXTRADEPTH, -EXTRAWID / 2);
				glVertex3d(EXTRAWID / 2, 0.0, -EXTRAWID / 2);

				glVertex3d(-EXTRAWID / 2, 0.0, EXTRAWID / 2);
				glVertex3d(-EXTRAWID / 2, EXTRADEPTH, EXTRAWID / 2);
				glVertex3d(EXTRAWID / 2, EXTRADEPTH, EXTRAWID / 2);
				glVertex3d(EXTRAWID / 2, 0.0, EXTRAWID / 2);
			glEnd();

		//finish up
		glEndList();
	}

	void obtainPrivate_Extra()
	{
		sTexPieces_Extra.reset(new GLuint[NUM_TEXTURES_EXTRA]);

		//read in piece textures
		GLTextures::ReadGLTextures(NUM_TEXTURES_EXTRA, &sTexPieces_Extra[0], 
			swGlExtraTex);
		
		//display lists
		sLists_Extra.reset(new GLuint[NUM_LISTS_EXTRA]);

		//generate
		for(wxInt32 i = 0; i < NUM_LISTS_EXTRA; i++)
		{
			sLists_Extra[i] = glGenLists(1);
		}

		//create the lists
		buildExtra(sLists_Extra[Extra]);
	}

	void releasePrivate_Extra()
	{
		//set no texture
		glBindTexture(GL_TEXTURE_2D, 0);

		//clear piece textures
		glDeleteTextures(NUM_TEXTURES_EXTRA, &sTexPieces_Extra[0]);

		//clear out display lists
		glDeleteLists(sLists_Extra[0], NUM_LISTS_EXTRA);
	}

	void drawExtra()
	{
		glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, sTexPieces_Extra[Extra]);
			glCallList(sLists_Extra[Extra]);
		glPopMatrix();
	}
}

//---------------------------- PUBLIC           -----------------------------//
IMPLEMENT_DRAW_OBJECT(DrawExtraObject, ExtraObject, SEAFARERS);

DrawExtraObject::DrawExtraObject(const GameObjectPtr &object)
: DrawObject(object, 3)
{
	ExtraObject* thisObject = static_cast<ExtraObject*>(GetObject());
	if(NULL != thisObject)
	{
		thisObject->offset(EXTRADEPTH);
	}
}

DrawExtraObject::~DrawExtraObject()
{
}

void DrawExtraObject::Obtain()
{
	obtainPrivate_Extra();
}

void DrawExtraObject::Release()
{
	releasePrivate_Extra();
}

void DrawExtraObject::PopupFunction(wxString& name, wxString&, 
	wxBitmap&, bool &, ColorType&)
{
	name = stExtraPoint;
}

void DrawExtraObject::RenderOutline() const
{
}

void DrawExtraObject::RenderSelection() const
{
}

void DrawExtraObject::RenderModel(const wxInt32) const
{
	drawExtra();
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//


