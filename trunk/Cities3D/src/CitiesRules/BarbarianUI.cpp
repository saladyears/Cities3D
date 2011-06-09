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
#include "BarbarianUI.h"
#include "ImageEngine.h"
#include "ConfigDatabase.h"
#include "GLTextures.h"
#include "define/defineGL.h"
#include "GLMaterial.h"
#include "SkinManager.h"
#include "Vector.h"
#include "Controller.h"
#include "Game.h"
#include "TimerManager.h"
#include "KnightCount.h"
#include "DataObject.h"
#include "IRuleEngine.h"
#include "CommonUI.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	enum
	{
		Tile			= 0,
		Overlay			,
		ShipSide		,
		ShipTop			,

		NumLists		,
	};

	enum
	{
		TileTex			= 0,
		OverlayTex		,
		ShipSideTex		,
		ShipTopTex		,

		NumTextures		,
	};

	typedef boost::scoped_array<GLuint> GLuintArray;
	GLuintArray sLists;
	GLuintArray sTextures;

	const float sOverlayPercent = 0.90f;
	const float sShipPercent = 1.1f;

	// Ship dimensions.
	const float SHWID			= (0.8 * SCALE) * sShipPercent;
	const float SHTOP			= (1.5 * SCALE) * sShipPercent;
	const float SHMID			= (0.5 * SCALE) * sShipPercent;
	const float SHLEN			= (1.8 * SCALE) * sShipPercent;
	const float SHMAST			= (0.5 * SCALE) * sShipPercent;
	const float SHHULLR			= (0.3 * SCALE) * sShipPercent;
	const float SHHULLRB		= (0.4 * SCALE) * sShipPercent;
	const float SHMASTR			= (0.5 * SCALE) * sShipPercent;
	const wxInt32 SHSLICES		= 6;

	void buildTile(GLuint id)
	{
		glNewList(id, GL_COMPILE);
			glBegin(GL_POLYGON);
				glNormal3d(0.0f, 1.0f, 0.0f);
				glTexCoord2d(1.00, 0.50); glVertex3d(X, 0.0, 0.0);
				glTexCoord2d(0.75, 1.00); glVertex3d((X / 2), 0.0, -Z);
				glTexCoord2d(0.25, 1.00); glVertex3d(-(X / 2), 0.0, -Z);
				glTexCoord2d(0.00, 0.50); glVertex3d(-X, 0.0, 0.0);
				glTexCoord2d(0.25, 0.00); glVertex3d(-(X / 2), 0.0, Z);
				glTexCoord2d(0.75, 0.00); glVertex3d((X / 2), 0.0, Z);
				glTexCoord2d(1.00, 0.50); glVertex3d(X, 0.0, 0.0);
			glEnd();
		glEndList();
	}

	void buildOverlay(GLuint id)
	{
		double x = (X / 2) * sOverlayPercent;
		double z = (Z * 2) * sOverlayPercent;

		glNewList(id, GL_COMPILE);
			glBegin(GL_QUADS);
				glNormal3d(0.0f, 1.0f, 0.0f);
				glTexCoord2d(0.00, 0.00); glVertex3d(-x, 0.0, z);
				glTexCoord2d(1.00, 0.00); glVertex3d(x, 0.0, z);
				glTexCoord2d(1.00, 1.00); glVertex3d(x, 0.0, -z);
				glTexCoord2d(0.00, 1.00); glVertex3d(-x, 0.0, -z);
			glEnd();
		glEndList();
	}

	//a ship
	void buildShip(GLuint id)
	{
		wxInt32 i;
		double dY, dZ;
		double dTX, dTY;
		Vector v1Old, v2Old;
		Vector v1New, v2New;
		Vector x;

		//ships are really, really, annoying

		//SIDE FACES
		glNewList(id, GL_COMPILE);
			//side face
			glBegin(GL_QUADS);
				glNormal3f(1.0, 0.0, 0.0);
				glTexCoord2d(0.01, 0.198); glVertex3d(SHWID / 2, SHHULLR, 
					SHLEN / 2);
				glTexCoord2d(0.01, 0.33); glVertex3d(SHWID / 2, SHMID, 
					SHLEN / 2);
				glTexCoord2d(0.22, 0.33); glVertex3d(SHWID / 2, SHMID, 
					(SHLEN / 2) - SHHULLR);
				glTexCoord2d(0.22, 0.198); glVertex3d(SHWID / 2, SHHULLR, 
					(SHLEN / 2) - SHHULLR);
			glEnd();
			glBegin(GL_QUADS);
				glNormal3f(1.0, 0.0, 0.0);
				glTexCoord2d(0.22, 0.0); glVertex3d(SHWID / 2, 0.0, 
					(SHLEN / 2) - SHHULLR);
				glTexCoord2d(0.22, 0.33); glVertex3d(SHWID / 2, SHMID, 
					(SHLEN / 2) - SHHULLR);
				glTexCoord2d(0.72, 0.33); glVertex3d(SHWID / 2, SHMID, 
					-(SHLEN / 2) + SHHULLRB);
				glTexCoord2d(0.72, 0.0); glVertex3d(SHWID / 2, 0.0, 
					-(SHLEN / 2) + SHHULLRB);
			glEnd();
			glBegin(GL_QUADS);
				glNormal3f(1.0, 0.0, 0.0);
				glTexCoord2d(1.00, 0.264); glVertex3d(SHWID / 2, SHHULLRB, 
					-SHLEN / 2);
				glTexCoord2d(1.00, 0.33); glVertex3d(SHWID / 2, SHMID, 
					-SHLEN / 2);
				glTexCoord2d(0.72, 0.33); glVertex3d(SHWID / 2, SHMID, 
					-(SHLEN / 2) + SHHULLRB);
				glTexCoord2d(0.72, 0.264); glVertex3d(SHWID / 2, SHHULLRB, 
					-(SHLEN / 2) + SHHULLRB);
			glEnd();

			//create the polygon
			glBegin(GL_POLYGON);
				//set the first two coords
				glNormal3f(1.0, 0.0, 0.0);
				glTexCoord2d(0.22, 0.198); glVertex3d(SHWID / 2, SHHULLR, 
					(SHLEN / 2) - SHHULLR);
				glTexCoord2d(0.01, 0.198); glVertex3d(SHWID / 2, SHHULLR, 
					SHLEN / 2);
			
				for(i = 1; i <= SHSLICES; i++)
				{
					dZ = ((SHLEN / 2) - SHHULLR) - (SHHULLR * 
						cos(D2R * (180.0 + ((90.0 / (double) SHSLICES) * i))));
					dY = SHHULLR + (SHHULLR * 
						sin(D2R * (180.0 + ((90.0 / (double) SHSLICES) * i))));

					//calc tex coords
					dTX = 0.22 - (0.21 * cos(D2R * (180.0 + 
						((90.0 / (double) SHSLICES) * i))));
					dTY = 0.198 + (0.198 * sin(D2R * (180.0 + 
						((90.0 / (double) SHSLICES) * i))));

					//draw it
					glTexCoord2d(dTX, dTY); glVertex3d(SHWID / 2, dY, dZ);
				}
			glEnd();

			//create the polygon
			glBegin(GL_POLYGON);
				//set the first two coords
				glNormal3f(1.0, 0.0, 0.0);
				glTexCoord2d(0.72, 0.264); glVertex3d(SHWID / 2, SHHULLRB, 
					-(SHLEN / 2) + SHHULLRB);
				glTexCoord2d(1.00, 0.264); glVertex3d(SHWID / 2, SHHULLRB, 
					-SHLEN / 2);
			
				for(i = 1; i <= SHSLICES; i++)
				{
					dZ = (-(SHLEN / 2) + SHHULLRB) - (SHHULLRB * 
						cos(D2R * (0.0 - ((90.0 / (double) SHSLICES) * i))));
					dY = SHHULLRB + (SHHULLRB * 
						sin(D2R * (0.0 - ((90.0 / (double) SHSLICES) * i))));

					//calc tex coords
					dTX = 0.72 - (0.28 * cos(D2R * (0.0 - 
						((90.0 / (double) SHSLICES) * i))));
					dTY = 0.264 + (0.264 * sin(D2R * (0.0 - 
						((90.0 / (double) SHSLICES) * i))));

					//draw it
					glTexCoord2d(dTX, dTY); glVertex3d(SHWID / 2, dY, dZ);
				}
			glEnd();

			//side face
			glBegin(GL_QUADS);
				glNormal3f(-1.0, 0.0, 0.0);
				glTexCoord2d(0.01, 0.198); glVertex3d(-SHWID / 2, SHHULLR, 
					SHLEN / 2);
				glTexCoord2d(0.01, 0.33); glVertex3d(-SHWID / 2, SHMID,
					SHLEN / 2);
				glTexCoord2d(0.22, 0.33); glVertex3d(-SHWID / 2, SHMID, 
					(SHLEN / 2) - SHHULLR);
				glTexCoord2d(0.22, 0.198); glVertex3d(-SHWID / 2, SHHULLR, 
					(SHLEN / 2) - SHHULLR);
			glEnd();
			glBegin(GL_QUADS);
				glNormal3f(-1.0, 0.0, 0.0);
				glTexCoord2d(0.22, 0.0); glVertex3d(-SHWID / 2, 0.0, 
					(SHLEN / 2) - SHHULLR);
				glTexCoord2d(0.22, 0.33); glVertex3d(-SHWID / 2, SHMID, 
					(SHLEN / 2) - SHHULLR);
				glTexCoord2d(0.72, 0.33); glVertex3d(-SHWID / 2, SHMID, 
					-(SHLEN / 2) + SHHULLRB);
				glTexCoord2d(0.72, 0.0); glVertex3d(-SHWID / 2, 0.0, 
					-(SHLEN / 2) + SHHULLRB);
			glEnd();
			glBegin(GL_QUADS);
				glNormal3f(-1.0, 0.0, 0.0);
				glTexCoord2d(1.00, 0.264); glVertex3d(-SHWID / 2, SHHULLRB, 
					-SHLEN / 2);
				glTexCoord2d(1.00, 0.33); glVertex3d(-SHWID / 2, SHMID, 
					-SHLEN / 2);
				glTexCoord2d(0.72, 0.33); glVertex3d(-SHWID / 2, SHMID, 
					-(SHLEN / 2) + SHHULLRB);
				glTexCoord2d(0.72, 0.264); glVertex3d(-SHWID / 2, SHHULLRB, 
					-(SHLEN / 2) + SHHULLRB);
			glEnd();

			//create the polygon
			glBegin(GL_POLYGON);
				//set the first two coords
				glNormal3f(-1.0, 0.0, 0.0);
				glTexCoord2d(0.22, 0.198); glVertex3d(-SHWID / 2, SHHULLR, 
					(SHLEN / 2) - SHHULLR);
				glTexCoord2d(0.01, 0.198); glVertex3d(-SHWID / 2, SHHULLR, 
					SHLEN / 2);
			
				for(i = 1; i <= SHSLICES; i++)
				{
					dZ = ((SHLEN / 2) - SHHULLR) - (SHHULLR * 
						cos(D2R * (180.0 + ((90.0 / (double) SHSLICES) * i))));
					dY = SHHULLR + (SHHULLR * 
						sin(D2R * (180.0 + ((90.0 / (double) SHSLICES) * i))));

					//calc tex coords
					dTX = 0.22 - (0.21 * cos(D2R * (180.0 + 
						((90.0 / (double) SHSLICES) * i))));
					dTY = 0.198 + (0.198 * sin(D2R * (180.0 + 
						((90.0 / (double) SHSLICES) * i))));

					//draw it
					glTexCoord2d(dTX, dTY); glVertex3d(-SHWID / 2, dY, dZ);
				}
			glEnd();

			//create the polygon
			glBegin(GL_POLYGON);
				//set the first two coords
				glNormal3f(-1.0, 0.0, 0.0);
				glTexCoord2d(0.72, 0.264); glVertex3d(-SHWID / 2, SHHULLRB,
					-(SHLEN / 2) + SHHULLRB);
				glTexCoord2d(1.00, 0.264); glVertex3d(-SHWID / 2, SHHULLRB, 
					-SHLEN / 2);
			
				for(i = 1; i <= SHSLICES; i++)
				{
					dZ = (-(SHLEN / 2) + SHHULLRB) - (SHHULLRB * 
						cos(D2R * (0.0 - ((90.0 / (double) SHSLICES) * i))));
					dY = SHHULLRB + (SHHULLRB * 
						sin(D2R * (0.0 - ((90.0 / (double) SHSLICES) * i))));

					//calc tex coords
					dTX = 0.72 - (0.28 * 
						cos(D2R * (0.0 - ((90.0 / (double) SHSLICES) * i))));
					dTY = 0.264 + (0.264 * 
						sin(D2R * (0.0 - ((90.0 / (double) SHSLICES) * i))));

					//draw it
					glTexCoord2d(dTX, dTY); glVertex3d(-SHWID / 2, dY, dZ);
				}
			glEnd();

			//mast side
			glBegin(GL_QUADS);
				glNormal3f(1.0, 0.0, 0.0);
				glTexCoord2d(0.35, 0.33); glVertex3d(SHWID / 2, SHMID, 
					SHMAST / 2);
				glTexCoord2d(0.68, 0.33); glVertex3d(SHWID / 2, SHMID, 
					-SHMAST / 2);
				glTexCoord2d(0.68, 0.66); glVertex3d(SHWID / 2, SHTOP - SHMASTR, 
					-SHMAST / 2);
				glTexCoord2d(0.35, 0.66); glVertex3d(SHWID / 2, SHTOP - SHMASTR, 
					SHMAST / 2);
			glEnd();

			//create the polygon
			glBegin(GL_POLYGON);
				//set the first two coords
				glNormal3f(1.0, 0.0, 0.0);
				glTexCoord2d(0.68, 0.66); glVertex3d(SHWID / 2, SHTOP - SHMASTR, 
					(SHMAST / 2) - SHMASTR);
				glTexCoord2d(0.35, 0.66); glVertex3d(SHWID / 2, SHTOP - SHMASTR, 
					SHMAST / 2);
			
				for(i = 1; i <= SHSLICES; i++)
				{
					dZ = ((SHMAST / 2) - SHMASTR) - (SHMASTR * 
						cos(D2R * (180.0 - ((90.0 / (double) SHSLICES) * i))));
					dY = (SHTOP - SHMASTR) + (SHMASTR * 
						sin(D2R * (180.0 - ((90.0 / (double) SHSLICES) * i))));

					//calc tex coords
					dTX = 0.68 + (0.23 * cos(D2R * (180.0 - 
						((90.0 / (double) SHSLICES) * i))));
					dTY = 0.66 - (0.33 * sin(D2R * (180.0 - 
						((90.0 / (double) SHSLICES) * i))));

					//draw it
					glTexCoord2d(dTX, dTY); glVertex3d(SHWID / 2, dY, dZ);
				}
			glEnd();

			//mast side
			glBegin(GL_QUADS);
				glNormal3f(-1.0, 0.0, 0.0);
				glTexCoord2d(0.35, 0.33); glVertex3d(-SHWID / 2, SHMID, 
					SHMAST / 2);
				glTexCoord2d(0.68, 0.33); glVertex3d(-SHWID / 2, SHMID, 
					-SHMAST / 2);
				glTexCoord2d(0.68, 0.66); glVertex3d(-SHWID / 2, 
					SHTOP - SHMASTR, -SHMAST / 2);
				glTexCoord2d(0.35, 0.66); glVertex3d(-SHWID / 2, 
					SHTOP - SHMASTR, SHMAST / 2);
			glEnd();

			//create the polygon
			glBegin(GL_POLYGON);
				//set the first two coords
				glNormal3f(-1.0, 0.0, 0.0);
				glTexCoord2d(0.68, 0.66); glVertex3d(-SHWID / 2, 
					SHTOP - SHMASTR, (SHMAST / 2) - SHMASTR);
				glTexCoord2d(0.35, 0.66); glVertex3d(-SHWID / 2, 
					SHTOP - SHMASTR, SHMAST / 2);
			
				for(i = 1; i <= SHSLICES; i++)
				{
					dZ = ((SHMAST / 2) - SHMASTR) - (SHMASTR * 
						cos(D2R * (180.0 - ((90.0 / (double) SHSLICES) * i))));
					dY = (SHTOP - SHMASTR) + (SHMASTR * 
						sin(D2R * (180.0 - ((90.0 / (double) SHSLICES) * i))));

					//calc tex coords
					dTX = 0.68 + (0.23 * cos(D2R * (180.0 - 
						((90.0 / (double) SHSLICES) * i))));
					dTY = 0.66 - (0.33 * sin(D2R * (180.0 - 
						((90.0 / (double) SHSLICES) * i))));

					//draw it
					glTexCoord2d(dTX, dTY); glVertex3d(-SHWID / 2, dY, dZ);
				}
			glEnd();
		glEndList();

		//build the ship
		glNewList(id + 1, GL_COMPILE);
			//front curved face
			glBegin(GL_QUADS);
				glNormal3f(0.0, 0.0, 1.0);
				glTexCoord2d(0.8, 0.0); glVertex3d(SHWID / 2, SHHULLR, 
					SHLEN / 2);
				glTexCoord2d(0.8, 1.0); glVertex3d(-SHWID / 2, SHHULLR, 
					SHLEN / 2);
				glTexCoord2d(1.0, 1.0); glVertex3d(-SHWID / 2, SHMID, 
					SHLEN / 2);
				glTexCoord2d(1.0, 0.0); glVertex3d(SHWID / 2, SHMID, 
					SHLEN / 2);
			glEnd();

			//the curves
			glBegin(GL_QUAD_STRIP);
				//set the first two
				glNormal3f(0.0, 0.0, 1.0);
				//set the vectors
				v1Old = Vector(-SHWID / 2, SHHULLR, SHLEN / 2);
				v2Old = Vector(SHWID / 2, SHHULLR, SHLEN / 2);

				//draw the first two
				glTexCoord2d(0.8, 1.0); glVertex3d(v1Old.x, v1Old.y, v1Old.z);
				glTexCoord2d(0.8, 0.0); glVertex3d(v2Old.x, v2Old.y, v2Old.z);

				for(i = 1; i <= SHSLICES; i++)
				{
					dZ = ((SHLEN / 2) - SHHULLR) - (SHHULLR * 
						cos(D2R * (180.0 + ((90.0 / (double) SHSLICES) * i))));
					dY = SHHULLR + (SHHULLR * 
						sin(D2R * (180.0 + ((90.0 / (double) SHSLICES) * i))));

					//set the comparison vectors
					v1New = Vector(-SHWID / 2, dY, dZ);
					v2New = Vector(SHWID / 2, dY, dZ);

					//now, we cross product
					x = CrossProduct(v1Old - v2Old, v1New - v1Old);
					x.Normalize();

					//set the new normal
					glNormal3d(x.x, x.y, x.z);

					//draw it
					glTexCoord2d(0.8 - ((0.4 / SHSLICES) * (double) i), 1.0); 
					glVertex3d(v1New.x, v1New.y, v1New.z);
					glTexCoord2d(0.8 - ((0.4 / SHSLICES) * (double) i), 0.0); 
					glVertex3d(v2New.x, v2New.y, v2New.z);

					//reset the old vectors
					v1Old = v1New;
					v2Old = v2New;
				}
			glEnd();

			//back curved face
			glBegin(GL_QUADS);
				glNormal3f(0.0, 0.0, -1.0);
				glTexCoord2d(0.7, 0.0); glVertex3d(SHWID / 2, SHHULLRB, 
					-SHLEN / 2);
				glTexCoord2d(0.7, 1.0); glVertex3d(-SHWID / 2, SHHULLRB, 
					-SHLEN / 2);
				glTexCoord2d(0.8, 1.0); glVertex3d(-SHWID / 2, SHMID, 
					-SHLEN / 2);
				glTexCoord2d(0.8, 0.0); glVertex3d(SHWID / 2, SHMID, 
					-SHLEN / 2);
			glEnd();

			//the curves
			glBegin(GL_QUAD_STRIP);
				//set the first two
				glNormal3f(0.0, 0.0, -1.0);

				//set the vectors
				v1Old = Vector(SHWID / 2, SHHULLRB, -SHLEN / 2);
				v2Old = Vector(-SHWID / 2, SHHULLRB, -SHLEN / 2);

				//draw the first two
				glTexCoord2d(0.7, 0.0); glVertex3d(v1Old.x, v1Old.y, v1Old.z);
				glTexCoord2d(0.7, 1.0); glVertex3d(v2Old.x, v2Old.y, v2Old.z);

				for(i = 1; i <= SHSLICES; i++)
				{
					dZ = (-(SHLEN / 2) + SHHULLRB) - (SHHULLRB * 
						cos(D2R * (0.0 - ((90.0 / (double) SHSLICES) * i))));
					dY = SHHULLRB + (SHHULLRB * 
						sin(D2R * (180.0 + ((90.0 / (double) SHSLICES) * i))));

					//set the comparison vectors
					v1New = Vector(SHWID / 2, dY, dZ);
					v2New = Vector(-SHWID / 2, dY, dZ);

					//now, we cross product
					x = CrossProduct(v1Old - v2Old, v1New - v1Old);
					x.Normalize();

					//set the new normal
					glNormal3d(x.x, x.y, x.z);

					//draw it
					glTexCoord2d(0.7 - ((0.5 / SHSLICES) * (double) i), 0.0); 
					glVertex3d(v1New.x, v1New.y, v1New.z);
					glTexCoord2d(0.7 - ((0.5 / SHSLICES) * (double) i), 1.0); 
					glVertex3d(v2New.x, v2New.y, v2New.z);

					//reset the old vectors
					v1Old = v1New;
					v2Old = v2New;
				}
			glEnd();

			//front top half face
			glBegin(GL_QUADS);
				glNormal3f(0.0, 1.0, 0.0);
				glTexCoord2d(0.2, 0.0); glVertex3d(SHWID / 2, SHMID, 
					SHLEN / 2);
				glTexCoord2d(0.65, 0.0); glVertex3d(SHWID / 2, SHMID, 
					SHMAST / 2);
				glTexCoord2d(0.65, 1.0); glVertex3d(-SHWID / 2, SHMID, 
					SHMAST / 2);
				glTexCoord2d(0.2, 1.0); glVertex3d(-SHWID / 2, SHMID, 
					SHLEN / 2);
			glEnd();
			//back top half face
			glBegin(GL_QUADS);
				glNormal3f(0.0, 1.0, 0.0);
				glTexCoord2d(0.5, 0.0); glVertex3d(SHWID / 2, SHMID, 
					-SHLEN / 2);
				glTexCoord2d(0.95, 0.0); glVertex3d(SHWID / 2, SHMID, 
					-SHMAST / 2);
				glTexCoord2d(0.95, 1.0); glVertex3d(-SHWID / 2, SHMID, 
					-SHMAST / 2);
				glTexCoord2d(0.5, 1.0); glVertex3d(-SHWID / 2, SHMID, 
					-SHLEN / 2);
			glEnd();

			//mast front face
			glBegin(GL_QUADS);
				glNormal3f(0.0, 0.0, 1.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(SHWID / 2, SHMID, 
					SHMAST / 2);
				glTexCoord2d(0.0, 1.0); glVertex3d(-SHWID / 2, SHMID, 
					SHMAST / 2);
				glTexCoord2d(0.45, 1.0); glVertex3d(-SHWID / 2, 
					SHTOP - SHMASTR, SHMAST / 2);
				glTexCoord2d(0.45, 0.0); glVertex3d(SHWID / 2, 
					SHTOP - SHMASTR, SHMAST / 2);
			glEnd();
			//mast back face
			glBegin(GL_QUADS);
				glNormal3f(0.0, 0.0, -1.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(SHWID / 2, SHMID, 
					-SHMAST / 2);
				glTexCoord2d(0.0, 1.0); glVertex3d(-SHWID / 2, SHMID, 
					-SHMAST / 2);
				glTexCoord2d(0.8, 1.0); glVertex3d(-SHWID / 2, SHTOP, 
					-SHMAST / 2);
				glTexCoord2d(0.8, 0.0); glVertex3d(SHWID / 2, SHTOP, 
					-SHMAST / 2);
			glEnd();

			//the curved face
			glBegin(GL_QUAD_STRIP);
				//set the vectors
				v1Old = Vector(SHWID / 2, SHTOP - SHMASTR, SHMAST / 2);
				v2Old = Vector(-SHWID / 2, SHTOP - SHMASTR, SHMAST / 2);

				//draw the first two
				glNormal3f(0.0, 0.0, 1.0);
				glTexCoord2d(0.45, 0.0); glVertex3d(v1Old.x, v1Old.y, v1Old.z);
				glTexCoord2d(0.45, 1.0); glVertex3d(v2Old.x, v2Old.y, v2Old.z);

				for(i = 1; i <= SHSLICES; i++)
				{
					dZ = ((SHMAST / 2) - SHMASTR) - (SHMASTR * 
						cos(D2R * (180.0 - ((90.0 / (double) SHSLICES) * i))));
					dY = (SHTOP - SHMASTR) + (SHMASTR * 
						sin(D2R * (180.0 - ((90.0 / (double) SHSLICES) * i))));

					//set the comparison vectors
					v1New = Vector(SHWID / 2, dY, dZ);
					v2New = Vector(-SHWID / 2, dY, dZ);

					//now, we cross product
					x = CrossProduct(v1Old - v2Old, v1New - v1Old);
					x.Normalize();

					//set the new normal
					glNormal3d(x.x, x.y, x.z);

					//draw it
					glTexCoord2d(0.45 + ((0.55 / SHSLICES) * (double) i), 0.0); 
					glVertex3d(v1New.x, v1New.y, v1New.z);
					glTexCoord2d(0.45 + ((0.55 / SHSLICES) * (double) i), 1.0); 
					glVertex3d(v2New.x, v2New.y, v2New.z);

					//reset the old vectors
					v1Old = v1New;
					v2Old = v2New;
				}
			glEnd();
		glEndList();
	}

	bool isCompliant(const wxImage &image)
	{
		wxInt32 w = image.GetWidth();
		wxInt32 h = image.GetHeight();

		//check to see if the width and height are even multiples of two -- 
		//this works by bit shifting the number to the right one place at a 
		//time until a 1 appears in the 1's digit.  If it is a multiple of two,
		//the number will have been bit shifted all the way to its only 1 digit
		//and should now be 1. Otherwise, it is not a multiple of two.
		for(; !(w & 1); w >>= 1);
		for(; !(h & 1); h >>= 1);
		return ((1 == w) && (1 == h));
	}

	wxImage makeGLCompliant(const wxImage &image)
	{
		wxUint32 w = image.GetWidth();
		wxUint32 h = image.GetHeight();

		//find the next largest multiple of two to scale to
		wxInt32 count;
		for(count = 0; !(w & (1 << 31)); w <<= 1, ++count);
		w = (1 << (31 - count));
		for(count = 0; !(h & (1 << 31)); h <<= 1, ++count);
		h = (1 << (31 - count));

		return image.Scale(w, h);
	}

	GLuint buildOverlayTexture(const wxImage &image, bool mipmap)
	{
		GLuint texID = 0;

		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);

		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmap ? 
			GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);

		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
		glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
		glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);

		//because GL reads textures with 0,0 being the lower-left, we have to
		//flip each image vertically
		wxImage glImage = image.Mirror(false);

		//make the image GL compliant (on 2^ size boundaries)
		if(false == isCompliant(glImage))
		{
			glImage = makeGLCompliant(glImage);
		}

		// Since this image is special and has a transparency color, we need
		// to set the alpha channel properly per pixel.
		wxColour t = SKIN.Element(shTransparent);
		glImage.SetMaskColour(t.Red(), t.Green(), t.Blue());
		glImage.InitAlpha();

		size_t w = glImage.GetWidth();
		size_t h = glImage.GetHeight();

		size_t bytes = w * h * 4;

		// Now that we have alpha calculated, create memory to hold the color
		// information including alpha.
		boost::scoped_array<wxUint8> data(new wxUint8[bytes]);
		unsigned char* src = glImage.GetData();
		unsigned char* end = src + (w * h * 3);
		unsigned char* dest = data.get();
		unsigned char* alpha = glImage.GetAlpha();

		while(src < end)
		{
			dest[0] = src[0];
			dest[1] = src[1];
			dest[2] = src[2];
			dest[3] = *alpha;

			dest += 4;
			src += 3;
			++alpha;
		}

		if(true == mipmap)
		{
			gluBuild2DMipmaps(GL_TEXTURE_2D, 4, w, h, GL_RGBA, 
				GL_UNSIGNED_BYTE, data.get());
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, 
				GL_UNSIGNED_BYTE, data.get());
		}

		return texID;
	}

	void obtain()
	{
		// Textures.
		sTextures.reset(new GLuint[NumTextures]);

		// Ocean
		wxImage image = IMAGE.GetImage(shTileType, shOcean);
		bool mipmap = 
			CONFIG.read<wxInt32>(swConfigSectionOpenGL, swMipmap, TRUE) || false;
		sTextures[TileTex] = GLTextures::BuildTexture(image, mipmap);

		// Overlay
		image = IMAGE.GetImage(shBarbarians, shBarbarians);
		sTextures[OverlayTex] = buildOverlayTexture(image, mipmap);

		// Ship
		GLTextures::ReadGLTextures(2, &sTextures[ShipSideTex], 
			swGlShipTex);

		// Display lists.
		sLists.reset(new GLuint[NumLists]);

		for(size_t i = 0; i < NumLists; i++)
		{
			sLists[i] = glGenLists(1);
		}

		buildTile(sLists[Tile]);
		buildOverlay(sLists[Overlay]);
		buildShip(sLists[ShipSide]);
	}

	void release()
	{
		glBindTexture(GL_TEXTURE_2D, 0);

		// Clear out textures.
		glDeleteLists(sTextures[0], NumTextures);

		// Clear out lists.
		glDeleteLists(sLists[0], NumLists);
	}

	void drawTile()
	{
		glBindTexture(GL_TEXTURE_2D, sTextures[TileTex]);

		glCallList(sLists[Tile]);
	}

	void drawOverlay()
	{
		glBindTexture(GL_TEXTURE_2D, sTextures[OverlayTex]);

		glCallList(sLists[Overlay]);
	}

	void drawShip()
	{
		glBindTexture(GL_TEXTURE_2D, sTextures[ShipSideTex]);
		glCallList(sLists[ShipSide]);

		glBindTexture(GL_TEXTURE_2D, sTextures[ShipTopTex]);
		glCallList(sLists[ShipTop]);
	}

	GLMaterial sWhite;
	GLMaterial sBlack;

	Vector sPositions[] = 
	{
		Vector(0.0f, 0.0f, (-Z * 1.25)),
		Vector((-X * 0.175), 0.0f, (-Z * 0.60)),
		Vector((X * 0.0575), 0.0f, (-Z * 0.40)),
		Vector((X * 0.275), 0.0f, (-Z * 0.20)),
		Vector((X * 0.275), 0.0f, (Z * 0.15)),
		Vector((X * 0.0575), 0.0f, (Z * 0.40)),
		Vector((-X * 0.175), 0.0f, (Z * 0.60)),
		Vector(0.0f, 0.0f, (Z * 1.25)),
	};

	float sOrientations[] = 
	{
		-20.0f,
		57.5f,
		50.0f,
		0.0f,
		-45.0f,
		-55.0f,
		20.0f,
		0.0f,
	};

	Vector sCurrentPosition = sPositions[0];
	float sCurrentOrientation = sOrientations[0];

	// Use 30 fps for ship animation.
	const size_t ANIMATION_RESOLUTION = 33;
	const wxLongLong sAnimationLength = 2000;

	// KnightCount sizes.
	const wxInt32 sDefaultCountSize = 24;
	const wxInt32 sDefaultTextHeight = 11;
	const wxInt32 sDefaultSpace = 1;

	const wxSize sMinSizeBarbarian = wxSize(165, 150);
}

//---------------------------- PUBLIC           -----------------------------//
wxBarbarianUI::wxBarbarianUI(wxWindow *parent, const GamePtr& game,
	const wxSize& size)
: wxBaseUI<wxCoreGLCanvas>(parent, size)
, mAnimateDepth(0)
, mAnimatePos(0)
, mAnimationStart(0)
{
	SetMinSize(sMinSizeBarbarian);
	Show();

	SetBackgroundColour(SKIN.Element(shBackground));

	// Set up the knight counts.
	wxInt32 players = game->numPlayers();
	mKnightCounts.reserve(players);

	wxSize countSize = wxSize(sDefaultCountSize + 
		static_cast<wxInt32>(sDefaultTextHeight * 1.5),
		static_cast<wxInt32>(sDefaultCountSize + 2));
	wxSize bitmapSize = wxSize(sDefaultCountSize, sDefaultCountSize);

	for(wxInt32 i = 0; i < players; ++i)
	{
		KnightCount* count = new KnightCount(this, i, game, wxDefaultPosition, 
			countSize);
		count->Resize(bitmapSize, sDefaultTextHeight);
		mKnightCounts.push_back(count);
	}

	OnKnightCounts(game);

	// Set up the GL scene.
	if(GetContext())
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		SetCurrent();
		glEnable(GL_TEXTURE_2D);
		obtain();
	}

	sWhite.SetMaterial(wxColour(255, 255, 255));
	sBlack.SetMaterial(wxColour(0, 0, 0));

	wxInt32 val = game->read<wxInt32>(shBarbarianAdvance);
	wxASSERT((0  <= val) && (7 >= val));
	mAnimatePos = val + 1;

	sCurrentPosition = sPositions[val];
	sCurrentOrientation = sOrientations[val];

	Controller::get().AddReceiver(shEventKnightCounts, 
		&wxBarbarianUI::OnKnightCounts, this);
	Controller::get().AddReceiver(shEventPlayerReorder, 
		&wxBarbarianUI::OnPlayerReorder, this);
	Controller::get().AddReceiver(shEventBarbarianAdvance, 
		&wxBarbarianUI::OnBarbarianAdvance, this);
	Controller::get().AddReceiver(shEventImageUpdate,
		&wxBarbarianUI::OnImageUpdate, this);
	Controller::get().AddReceiver(shEventTabBarbarians, 
		&wxBarbarianUI::OnTabBarbarians, this);
}

wxBarbarianUI::~wxBarbarianUI()
{
	if(GetContext())
	{
		SetCurrent();
		release();
	}

	TIME.RemovePeriodicFunction(&wxBarbarianUI::Animate, this,
		ANIMATION_RESOLUTION);

	Controller::get().RemoveReceiver(shEventKnightCounts, 
		&wxBarbarianUI::OnKnightCounts, this);
	Controller::get().RemoveReceiver(shEventPlayerReorder, 
		&wxBarbarianUI::OnPlayerReorder, this);
	Controller::get().RemoveReceiver(shEventBarbarianAdvance, 
		&wxBarbarianUI::OnBarbarianAdvance, this);
	Controller::get().RemoveReceiver(shEventImageUpdate,
		&wxBarbarianUI::OnImageUpdate, this);
	Controller::get().RemoveReceiver(shEventTabBarbarians, 
		&wxBarbarianUI::OnTabBarbarians, this);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxBarbarianUI, wxBaseUI<wxCoreGLCanvas>)
	EVT_SIZE(wxBarbarianUI::OnSize)
END_EVENT_TABLE()

void wxBarbarianUI::Paint()
{
	if (GetContext()) 
	{
		SetCurrent();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		DrawScene();

		SwapBuffers();
		glFinish();
	}
}

void wxBarbarianUI::DrawScene()
{
	gluLookAt(0.0, 17.15, 0.1, 
			  0.0, 0.0, 0.0, 
			  0.0, 1.0, 0.0);

	glEnable(GL_TEXTURE_2D);

	sWhite.UseMaterial();

	glRotated(-30.0, 0.0, 1.0, 0.0);

	glPushMatrix();
		glTranslated(0.0, 0.0, -Z);
		drawTile();
	glPopMatrix();
	glPushMatrix();
		glTranslated(0.0, 0.0, Z);
		drawTile();
	glPopMatrix();

	glEnable(GL_BLEND);
	glPushMatrix();
		glTranslated(0.0, 0.01, 0.0);
		drawOverlay();
	glPopMatrix();
	glDisable(GL_BLEND);

	sBlack.UseMaterial();

	glPushMatrix();
		glTranslated(sCurrentPosition.x, sCurrentPosition.y, 
			sCurrentPosition.z);
		glRotated(sCurrentOrientation, 0.0, 1.0, 0.0);
		drawShip();
	glPopMatrix();
	
	glDisable(GL_TEXTURE_2D);
}

void wxBarbarianUI::Animate()
{
	wxLongLong now = wxGetLocalTimeMillis();
	wxLongLong delta = now - mAnimationStart;

	// See if we're done with this segment of animation.
	if(sAnimationLength <= delta)
	{
		--mAnimateDepth;

		// Stop the timer if we're done.
		if(0 == mAnimateDepth)
		{
			TIME.RemovePeriodicFunction(&wxBarbarianUI::Animate, this,
				ANIMATION_RESOLUTION);
		}
		else
		{
			mAnimationStart = now;
		}

		++mAnimatePos;

		if(9 == mAnimatePos)
		{
			sCurrentPosition = sPositions[0];
			sCurrentOrientation = sOrientations[0];
			mAnimatePos = 1;
		}
		// Guarantee that the ship gets to the right spot.  It's possible that
		// we might not get a single frame of animation before we time out.
		else
		{
			sCurrentPosition = sPositions[mAnimatePos - 1];
			sCurrentOrientation = sOrientations[mAnimatePos - 1];
		}

		Refresh();
	}
	else if(7 >= mAnimatePos)
	{	
		// Do the interpolation.
		float percent = static_cast<float>(delta.ToDouble()) / 
			static_cast<float>(sAnimationLength.ToDouble());

		Vector v = sPositions[mAnimatePos] - sPositions[mAnimatePos - 1];
		float f = sOrientations[mAnimatePos] - sOrientations[mAnimatePos - 1];

		v *= percent;
		f *= percent;

		sCurrentPosition = sPositions[mAnimatePos - 1] + v;
		sCurrentOrientation = sOrientations[mAnimatePos - 1] + f;

		if(true == GetParent()->IsShown())
		{
			Paint();
		}
	}
}

void wxBarbarianUI::OnKnightCounts(const GamePtr& game)
{
	DataObject input(0), output;
	RULE.Decide(shLogicKnightCounts, input, output);

	typedef std::set<wxInt32> IntSet;
	bool winners = output.read<bool>();
	const IntSet& results = output.read<IntSet>(1);

	KnightCount::State state = winners ? KnightCount::StateWinning : 
		KnightCount::StateLosing;

	bool change = false;

	// Set the states for each player correctly.
	wxInt32 players = mKnightCounts.size();
	for(wxInt32 i = 0; i < players; ++i)
	{
		KnightCount* count = mKnightCounts[i];
		KnightCount::State curState = count->GetState();
		KnightCount::State newState = KnightCount::StateNeutral;

		if(game)
		{
			count->Update(game);
		}

		if(results.end() != results.find(i))
		{
			newState = state;
		}

		if(newState != curState)
		{
			count->SetState(newState);
			change = true;
		}
	}
}

void wxBarbarianUI::OnPlayerReorder(const GamePtr& game)
{
	// Go through and change the colors of each of the bitmaps.
	for(size_t i = 0; i < mKnightCounts.size(); ++i)
	{
		ColorType color = game->playerGame(i).color();
		mKnightCounts[i]->SetPlayerColor(color);
	}
}

void wxBarbarianUI::OnBarbarianAdvance(const GamePtr& game)
{
	wxInt32 pos = game->read<wxInt32>(shBarbarianAdvance);
	wxASSERT((0 <= pos) && (7 >= pos));

	wxBOOL animate = CONFIG.read<wxInt32>(swConfigSectionUI, swAnimationBoard, 
		TRUE);

	if(TRUE == animate)
	{
		++mAnimateDepth;

		// Start up the animation timer if we need to.
		if(1 == mAnimateDepth)
		{
			TIME.AddPeriodicFunction(&wxBarbarianUI::Animate, this,
				ANIMATION_RESOLUTION);

			mAnimationStart = wxGetLocalTimeMillis();
		}
	}
	else
	{
		sCurrentPosition = sPositions[pos];
		sCurrentOrientation = sOrientations[pos];
		mAnimatePos = pos + 1;
	}

	Refresh();
}

void wxBarbarianUI::OnImageUpdate(bool)
{
	bool shown = GetParent()->IsShown();
	GetParent()->Show();

	if (GetContext()) 
	{
		SetCurrent();

		release();
		obtain();
		Paint();
	}

	GetParent()->Show(shown);
}

void wxBarbarianUI::OnTabBarbarians(wxInt32)
{
	RaiseTab(this);
}

void wxBarbarianUI::OnSize(wxSizeEvent &event)
{
	wxSize eventSize = event.GetSize();

	// Determine how much we're being stretched.
	float x = static_cast<float>(eventSize.x) / static_cast<float>(sMinSizeBarbarian.x);
	float y = static_cast<float>(eventSize.y) / static_cast<float>(sMinSizeBarbarian.y);
	float percent = std::min(x, y);

	wxInt32 bitmapWidth = static_cast<wxInt32>(sDefaultCountSize * percent);
	wxInt32 textHeight = static_cast<wxInt32>(sDefaultTextHeight * percent);

	wxPoint pos = wxPoint(1, 0);
	wxSize size = wxSize(bitmapWidth + static_cast<wxInt32>(textHeight * 1.5),
		bitmapWidth + 2);
	wxSize bitmapSize = wxSize(bitmapWidth, bitmapWidth);

	for(size_t i = 0; i < mKnightCounts.size(); ++i)
	{
		KnightCount* count = mKnightCounts[i];

		// Adjust the size of the object.
		count->SetSize(pos.x, pos.y, size.x, size.y);
		count->Resize(bitmapSize, textHeight);

		pos.y += (size.y + sDefaultSpace);
	}

	event.Skip();
}
