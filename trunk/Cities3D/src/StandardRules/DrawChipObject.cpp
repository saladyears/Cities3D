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
#include "DrawChipObject.h"
#include "ChipObject.h"
#include "GLTextures.h"
#include "RuleSetDatabase.h"
#include "define/defineGL.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	//textures
	const wxInt32 GL_TEX_CHIPS			= 11;

	//chips
	const float CHIPDIAM				= (2.6 * SCALE);

	GLuintArray sTexBase;
	GLUquadricObj *sQuad_Chip;

	void obtainPrivate_ChipObject()
	{
		sTexBase.reset(new GLuint[GL_TEX_CHIPS]);

		//chip textures
		GLTextures::ReadGLTextures(GL_TEX_CHIPS, &sTexBase[0], 
			swGlChipsTex);

		//create the object
		sQuad_Chip = gluNewQuadric();

		//turn on textures
		gluQuadricTexture(sQuad_Chip, GL_TRUE);
		gluQuadricOrientation(sQuad_Chip, GLU_OUTSIDE);
	}

	void releasePrivate_ChipObject()
	{
		//set no texture
		glBindTexture(GL_TEXTURE_2D, 0);

		//clear out the old textures
		glDeleteTextures(GL_TEX_CHIPS, &sTexBase[0]);

		//clean up the quadric
		gluDeleteQuadric(sQuad_Chip);
	}

	void drawChip(const DrawChipObject * const pObject)
	{
		ChipObject *pChip = static_cast<ChipObject *>(pObject->GetObject());

		wxInt32 roll(pChip->roll());

		//sanity
		if(-1 == roll)
		{
			return;
		}

		//set the proper texture
		glBindTexture(GL_TEXTURE_2D, 
			sTexBase[(roll < 7) ? roll - 2 : roll - 3]);

		//draw chip top
		glPushMatrix();
			//give it some height off the map
			glTranslated(0.0, 0.1, 0.0);
			glRotated(90.0, -1.0, 0.0, 0.0);
			gluDisk(sQuad_Chip, 0.0, CHIPDIAM / 2, 25, 5);
		glPopMatrix();

		//draw chip base
		glBindTexture(GL_TEXTURE_2D, sTexBase[GL_TEX_CHIPS - 1]);
		glPushMatrix();
			glRotated(-90.0, 1.0, 0.0, 0.0);
			gluCylinder(sQuad_Chip, CHIPDIAM / 2, CHIPDIAM / 2, 0.1, 25, 5);
		glPopMatrix();
	}
}

//---------------------------- PUBLIC           -----------------------------//
IMPLEMENT_DRAW_OBJECT(DrawChipObject, ChipObject, STANDARD);

DrawChipObject::DrawChipObject(const GameObjectPtr &object)
: DrawObject(object, 1)
{
}

DrawChipObject::~DrawChipObject()
{
}

void DrawChipObject::Obtain()
{
	obtainPrivate_ChipObject();
}

void DrawChipObject::Release()
{
	releasePrivate_ChipObject();
}

void DrawChipObject::RenderOutline() const
{
}

void DrawChipObject::RenderSelection() const
{
}

void DrawChipObject::RenderModel(const wxInt32 selection) const
{
	(void) selection;

	drawChip(this);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//



