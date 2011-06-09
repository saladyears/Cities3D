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
#include "DrawRobberObject.h"
#include "RobberObject.h"
#include "GLTextures.h"
#include "GLMaterial.h"
#include "RuleSetDatabase.h"
#include "ConfigDatabase.h"
#include "define/defineGL.h"
#include "UtilityGL.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	//display lists
	const wxInt32 NUM_LISTS_ROBBER				= 2;
	GLuintArray sLists_Robber;

	enum
	{
		Robber			= 0,
		RobberGouraud	,
	};

	//robber dimensions
	const wxInt32 GL_ROBBER_SLICE		= 15;
	const wxInt32 GL_ROBBER_STACK		= 20;

	GLUquadricObj *sQuad_Robber;

	//gouraud normals
	Vector sNormals[GL_ROBBER_STACK][GL_ROBBER_SLICE];

	double sRadii[GL_ROBBER_STACK];
	double sHeight = 3.0;

	Vector calcGouraudVector(wxInt32 i, wxInt32 j)
	{
		wxInt32 k;
		wxInt32 iNum = 2;
		Vector normal;
		
		//set wrap
		k = (j == 0) ? GL_ROBBER_SLICE - 1 : j - 1;

		//get lower faces
		if(i > 0)
		{
			normal += sNormals[i - 1][j];
			normal += sNormals[i - 1][k];

			iNum = 4;
		}

		//get upper faces
		normal += sNormals[i][j];
		normal += sNormals[i][k];

		//average
		normal /= iNum;

		//normalize
		normal.Normalize();

		return normal;
	}

	//the robber
	void buildRobber(GLuint id)
	{
		wxInt32 i, j, k;
		double dTwoPi = 360 * D2R;
		double dH;
		double dT;
		Vector v1, v2;
		Vector points[GL_ROBBER_STACK][GL_ROBBER_SLICE];
		Vector gouraud;

		sRadii[19] = 0.0;
		sRadii[18] = 0.33;
		sRadii[17] = 0.42;
		sRadii[16] = 0.47;
		sRadii[15] = 0.5;
		sRadii[14] = 0.47;
		sRadii[13] = 0.40;
		//start increase for thick middle part
		sRadii[12] = 0.49;
		sRadii[11] = 0.57;
		sRadii[10] = 0.65;
		sRadii[9] = 0.67;
		sRadii[8] = 0.69;
		sRadii[7] = 0.70;
		sRadii[6] = 0.69;
		sRadii[5] = 0.67;
		sRadii[4] = 0.61;
		sRadii[3] = 0.52;
		sRadii[2] = 0.4;
		sRadii[1] = 0.55;
		sRadii[0] = 0.55;

		//create points
		for(i = 0; i < GL_ROBBER_STACK; i++)
		{
			//calculate height
			dH = (sHeight / GL_ROBBER_STACK) * i;

			for(j = 0; j < GL_ROBBER_SLICE; j++)
			{				//calculate theta
				dT = (dTwoPi / GL_ROBBER_SLICE) * j;

				//create point
				points[i][j] = Vector(sRadii[i] * cos(dT), dH, 
					sRadii[i] * sin(dT));
			}
		}

		//create face normals
		for(i = 0; i < GL_ROBBER_STACK - 1; i++)
		{
			for(j = 0; j < GL_ROBBER_SLICE; j++)
			{
				//set wrap around
				k = (j < GL_ROBBER_SLICE - 1) ? j : -1;

				//create face vectors
				v1 = points[i][j] - points[i][k + 1];
				v2 = points[i + 1][k + 1] - points[i][k + 1];

				//create face normal
				sNormals[i][j] = CrossProduct(v1, v2);
				sNormals[i][j].Normalize();
			}
		}

		//build the robber
		glNewList(id, GL_COMPILE);

		//draw
		for(i = 0; i < GL_ROBBER_STACK - 1; i++)
		{
			for(j = 0; j < GL_ROBBER_SLICE; j++)
			{
				//set wrap around
				k = (j < GL_ROBBER_SLICE - 1) ? j : -1;

				glBegin(GL_QUADS);
					//normal
					glNormal3d(sNormals[i][j].x, sNormals[i][j].y, 
						sNormals[i][j].z);

					//two bottom points
					glVertex3d(points[i][j].x, points[i][j].y, points[i][j].z);
					glVertex3d(points[i][k + 1].x, points[i][k + 1].y, 
						points[i][k + 1].z);

					//two top points
					glVertex3d(points[i + 1][k + 1].x, points[i + 1][k + 1].y, 
						points[i + 1][k + 1].z);
					glVertex3d(points[i + 1][j].x, points[i + 1][j].y, 
						points[i + 1][j].z);
				glEnd();
			}
		}

		//finish up
		glEndList();

		//build gouraud robber
		glNewList(id + 1, GL_COMPILE);

		//draw
		for(i = 0; i < GL_ROBBER_STACK - 1; i++)
		{
			for(j = 0; j < GL_ROBBER_SLICE; j++)
			{
				//set wrap around
				k = (j < GL_ROBBER_SLICE - 1) ? j : -1;

				glBegin(GL_QUADS);
					//two bottom points
					gouraud = calcGouraudVector(i, j);
					glNormal3d(gouraud.x, gouraud.y, gouraud.z);
					glVertex3d(points[i][j].x, points[i][j].y, 
						points[i][j].z);
					
					gouraud = calcGouraudVector(i, j + 1);
					glNormal3d(gouraud.x, gouraud.y, gouraud.z);
					glVertex3d(points[i][k + 1].x, points[i][k + 1].y, 
						points[i][k + 1].z);

					//two top points
					gouraud = calcGouraudVector(i + 1, j + 1);
					glNormal3d(gouraud.x, gouraud.y, gouraud.z);
					glVertex3d(points[i + 1][k + 1].x, points[i + 1][k + 1].y, 
						points[i + 1][k + 1].z);

					gouraud = calcGouraudVector(i + 1, j);
					glNormal3d(gouraud.x, gouraud.y, gouraud.z);
					glVertex3d(points[i + 1][j].x, points[i + 1][j].y, 
						points[i + 1][j].z);
				glEnd();
			}
		}

		//finish up
		glEndList();
	}

	void obtainPrivate_RobberObject()
	{
		//display lists
		sLists_Robber.reset(new GLuint[NUM_LISTS_ROBBER]);

		//generate
		for(wxInt32 i = 0; i < NUM_LISTS_ROBBER; i++)
		{
			sLists_Robber[i] = glGenLists(1);
		}

		//create the lists
		buildRobber(sLists_Robber[Robber]);

		sQuad_Robber = gluNewQuadric();

		//turn on textures
		gluQuadricTexture(sQuad_Robber, GL_TRUE);
		gluQuadricOrientation(sQuad_Robber, GLU_OUTSIDE);
	}

	void releasePrivate_RobberObject()
	{
		//clear out display lists
		glDeleteLists(sLists_Robber[0], NUM_LISTS_ROBBER);

		gluDeleteQuadric(sQuad_Robber);
	}

	void drawRobber()
	{
		glDisable(GL_TEXTURE_2D);

		glPushMatrix();
			glScaled(1.2, 1.2, 1.2);
			if(TRUE == CONFIG.read<wxInt32>(swConfigSectionOpenGL, 
				swRobberGouraud, TRUE))
			{
				glCallList(sLists_Robber[RobberGouraud]);
			}
			else
			{
				glCallList(sLists_Robber[Robber]);
			}
		glPopMatrix();

		glEnable(GL_TEXTURE_2D);
	}

	void drawRobberOutline()
	{
		float rad = sRadii[GL_ROBBER_STACK / 2] * 1.2;

		//top ring
		glPushMatrix();
			glTranslated(0.0, sHeight / 2.0f, 0.0);
			glRotated(90.0, 1.0, 0.0, 0.0);
			gluDisk(sQuad_Robber, rad, rad + 0.1, 20, 5);
		glPopMatrix();
	}
}

//---------------------------- PUBLIC           -----------------------------//
IMPLEMENT_DRAW_OBJECT(DrawRobberObject, RobberObject, STANDARD);

DrawRobberObject::DrawRobberObject(const GameObjectPtr &object)
: DrawObject(object, 10)
{
}

DrawRobberObject::~DrawRobberObject()
{
}

bool DrawRobberObject::CanSelect(const Vector &origin, const Vector &ray, 
	float &distance, wxInt32 &id, bool popup)
{
	bool hit = false;
	IGameObject *object = GetObject();

	if( (NULL != object) &&
		((object->isset(IGameObject::Select)) ||
		(true == popup)))
	{
		float rayLength;
		Vector plane;

		const Vector &o = object->coords();
		const float slice = sHeight / GL_ROBBER_STACK;

		// Go through each of the robber slices, seeing if we intersect.
		for(wxInt32 i = 0; i < GL_ROBBER_STACK; ++i)
		{
			// Determine height at this point.
			float height = slice * i;
			float radSquared = sRadii[i] * sRadii[i];
			float localDist, xDist, zDist;

			UtilityGL::planeCoords(height, origin, ray, plane, rayLength);

			xDist = o.x - plane.x;
			zDist = o.z - plane.z;

			localDist = (xDist * xDist) + (zDist * zDist);

			// If it's within the radius amount, it's a hit.
			if(radSquared >= localDist)
			{
				id = object->id();
				distance = rayLength;
				hit = true;
				break;
			}
		}
	}

	return hit;
}

void DrawRobberObject::Obtain()
{
	obtainPrivate_RobberObject();
}

void DrawRobberObject::Release()
{
	releasePrivate_RobberObject();
}

void DrawRobberObject::PopupFunction(wxString& name, wxString&, 
	wxBitmap&, bool &, ColorType&)
{
	name = stRobber;
}

void DrawRobberObject::RenderOutline() const
{
}

void DrawRobberObject::RenderSelection() const
{
	DrawObject::material(Selection).UseMaterial();
	
	drawRobberOutline();
}

void DrawRobberObject::RenderModel(const wxInt32) const
{
	GLfloat amb[] = {0.0f, 0.0f, 0.0f, 0.0f};
	GLfloat dif[] = {0.25f, 0.25f, 0.25f, 0.0f};
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);

	drawRobber();
	DrawObject::material(White).UseMaterial();
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//



