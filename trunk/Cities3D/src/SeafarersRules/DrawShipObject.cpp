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
#include "DrawShipObject.h"
#include "ShipObject.h"
#include "GLTextures.h"
#include "GLMaterial.h"
#include "Box.h"
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
	const wxInt32 NUM_TEXTURES_SHIP			= 2;
	GLuintArray sTexPieces_Ship;

	//display lists
	const wxInt32 NUM_LISTS_SHIP				= 3;
	GLuintArray sLists_Ship;

	enum
	{
		ShipSide		= 0,
		ShipTop			,
		ShipOutline		,
	};

	//ships dimensions
	const float SHWID			= (0.8 * SCALE);
	const float SHTOP			= (1.5 * SCALE);
	const float SHMID			= (0.5 * SCALE);
	const float SHLEN			= (1.8 * SCALE);
	const float SHMAST			= (0.5 * SCALE);
	const float SHHULLR			= (0.3 * SCALE);
	const float SHHULLRB		= (0.4 * SCALE);
	const float SHMASTR			= (0.5 * SCALE);
	const wxInt32 SHSLICES		= 6;

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

	//the outline of a ship
	void buildShipOutline(GLuint id)
	{
		float dY, dZ;

		//build the ship outline
		glNewList(id, GL_COMPILE);
			//start with the sides and then draw the cross lines
			glBegin(GL_LINE_LOOP);
				glNormal3f(1.0, 0.0, 0.0);
				glVertex3d(SHWID / 2, SHTOP, -SHMAST / 2);

				//mast curve
				for(wxInt32 i = SHSLICES; i >= 1; i--)
				{
					dZ = ((SHMAST / 2) - SHMASTR) - (SHMASTR * 
						cos(D2R * (180.0 - ((90.0 / (double) SHSLICES) * i))));
					dY = (SHTOP - SHMASTR) + (SHMASTR * 
						sin(D2R * (180.0 - ((90.0 / (double) SHSLICES) * i))));

					//draw it
					glVertex3d(SHWID / 2, dY, dZ);
				}
				
				//mast base
				glVertex3d(SHWID / 2, SHMID, SHMAST / 2);

				//front top
				glVertex3d(SHWID / 2, SHMID, SHLEN / 2);

				//front curve
				for(wxInt32 i = 1; i <= SHSLICES; i++)
				{
					dZ = ((SHLEN / 2) - SHHULLR) - (SHHULLR * 
						cos(D2R * (180.0 + ((90.0 / (double) SHSLICES) * i))));
					dY = SHHULLR + (SHHULLR * 
						sin(D2R * (180.0 + ((90.0 / (double) SHSLICES) * i))));

					//draw it
					glVertex3d(SHWID / 2, dY, dZ);
				}
			
				//base
				glVertex3d(SHWID / 2, 0.0, -(SHLEN / 2) + SHHULLRB);

				//back curve
				for(wxInt32 i = SHSLICES; i >= 1; i--)
				{
					dZ = (-(SHLEN / 2) + SHHULLRB) - (SHHULLRB * 
						cos(D2R * (0.0 - ((90.0 / (double) SHSLICES) * i))));
					dY = SHHULLRB + (SHHULLRB * 
						sin(D2R * (0.0 - ((90.0 / (double) SHSLICES) * i))));

					//draw it
					glVertex3d(SHWID / 2, dY, dZ);
				}

				//back corner
				glVertex3d(SHWID / 2, SHMID, -SHLEN / 2);

				//back top
				glVertex3d(SHWID / 2, SHMID, -SHMAST / 2);
			glEnd();

			//now repeat for the other side
			glBegin(GL_LINE_LOOP);
				glNormal3f(-1.0, 0.0, 0.0);
				glVertex3d(-SHWID / 2, SHTOP, -SHMAST / 2);

				//mast curve
				for(wxInt32 i = SHSLICES; i >= 1; i--)
				{
					dZ = ((SHMAST / 2) - SHMASTR) - (SHMASTR * 
						cos(D2R * (180.0 - ((90.0 / (double) SHSLICES) * i))));
					dY = (SHTOP - SHMASTR) + (SHMASTR * 
						sin(D2R * (180.0 - ((90.0 / (double) SHSLICES) * i))));

					//draw it
					glVertex3d(-SHWID / 2, dY, dZ);
				}
				
				//mast base
				glVertex3d(-SHWID / 2, SHMID, SHMAST / 2);

				//front top
				glVertex3d(-SHWID / 2, SHMID, SHLEN / 2);

				//front curve
				for(wxInt32 i = 1; i <= SHSLICES; i++)
				{
					dZ = ((SHLEN / 2) - SHHULLR) - (SHHULLR * 
						cos(D2R * (180.0 + ((90.0 / (double) SHSLICES) * i))));
					dY = SHHULLR + (SHHULLR * 
						sin(D2R * (180.0 + ((90.0 / (double) SHSLICES) * i))));

					//draw it
					glVertex3d(-SHWID / 2, dY, dZ);
				}
			
				//base
				glVertex3d(-SHWID / 2, 0.0, -(SHLEN / 2) + SHHULLRB);

				//back curve
				for(wxInt32 i = SHSLICES; i >= 1; i--)
				{
					dZ = (-(SHLEN / 2) + SHHULLRB) - (SHHULLRB * 
						cos(D2R * (0.0 - ((90.0 / (double) SHSLICES) * i))));
					dY = SHHULLRB + (SHHULLRB * 
						sin(D2R * (0.0 - ((90.0 / (double) SHSLICES) * i))));

					//draw it
					glVertex3d(-SHWID / 2, dY, dZ);
				}

				//back corner
				glVertex3d(-SHWID / 2, SHMID, -SHLEN / 2);

				//back top
				glVertex3d(-SHWID / 2, SHMID, -SHMAST / 2);
			glEnd();

			//now the cross lines
			glBegin(GL_LINES);
				//mast top
				glNormal3f(0.0, 0.0, -1.0);
				glVertex3d(SHWID / 2, SHTOP, -SHMAST / 2);
				glVertex3d(-SHWID / 2, SHTOP, -SHMAST / 2);

				//mast base front
				glNormal3f(0.0, 0.0, 1.0);
				glVertex3d(SHWID / 2, SHMID, SHMAST / 2);
				glVertex3d(-SHWID / 2, SHMID, SHMAST / 2);

				//mast base back
				glNormal3f(0.0, 0.0, -1.0);
				glVertex3d(SHWID / 2, SHMID, -SHMAST / 2);
				glVertex3d(-SHWID / 2, SHMID, -SHMAST / 2);

				//hull front
				glNormal3f(0.0, 0.0, 1.0);
				glVertex3d(SHWID / 2, SHMID, SHLEN / 2);
				glVertex3d(-SHWID / 2, SHMID, SHLEN / 2);

				//hull back
				glNormal3f(0.0, 0.0, -1.0);
				glVertex3d(SHWID / 2, SHMID, -SHLEN / 2);
				glVertex3d(-SHWID / 2, SHMID, -SHLEN / 2);
			glEnd();
		glEndList();
	}

	void obtainPrivate_Ship()
	{
		sTexPieces_Ship.reset(new GLuint[NUM_TEXTURES_SHIP]);

		//read in piece textures
		GLTextures::ReadGLTextures(NUM_TEXTURES_SHIP, &sTexPieces_Ship[ShipSide], 
			swGlShipTex);

		//display lists
		sLists_Ship.reset(new GLuint[NUM_LISTS_SHIP]);

		//generate
		for(wxInt32 i = 0; i < NUM_LISTS_SHIP; i++)
		{
			sLists_Ship[i] = glGenLists(1);
		}

		//create the lists
		buildShip(sLists_Ship[ShipSide]);
		buildShipOutline(sLists_Ship[ShipOutline]);
	}

	void releasePrivate_Ship()
	{
		//set no texture
		glBindTexture(GL_TEXTURE_2D, 0);

		//clear piece textures
		glDeleteTextures(NUM_TEXTURES_SHIP, &sTexPieces_Ship[0]);

		//clear out display lists
		glDeleteLists(sLists_Ship[0], NUM_LISTS_SHIP);
	}

	void drawShip()
	{
		glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, sTexPieces_Ship[ShipSide]);
			glCallList(sLists_Ship[ShipSide]);

			glBindTexture(GL_TEXTURE_2D, sTexPieces_Ship[ShipTop]);
			glCallList(sLists_Ship[ShipTop]);
		glPopMatrix();
	}

	void drawShipOutline()
	{
		glPushMatrix();
			glCallList(sLists_Ship[ShipOutline]);
		glPopMatrix();
	}
}

//---------------------------- PUBLIC           -----------------------------//
IMPLEMENT_DRAW_OBJECT(DrawShipObject, ShipObject, SEAFARERS);

DrawShipObject::DrawShipObject(const GameObjectPtr &object, 
							   const Material material)
: DrawObject(object, 10)
, mMaterial(material)
{
}

DrawShipObject::~DrawShipObject()
{
}

bool DrawShipObject::CanSelect(const Vector &origin, const Vector &ray, 
	float &distance, wxInt32 &id, bool popup)
{
	bool hit = false;
	IGameObject *object = GetObject();

	if( (NULL != object) &&
		((object->isset(IGameObject::Select)) ||
		(true == popup)))
	{
		// Test the intersection of the ray against the bounding boxes around 
		// the ship.
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

		// The first bounding box is defined by the two opposite corners.  Our 
		// origin point is on the bottom of the ship, centered between the 
		// edges.

		// Point 1 is the base height height, one edge of x/z away.
		Vector p1 = o + Vector(-SHWID / 2.0f, 0.0f, -SHLEN / 2.0f);
		Vector p2 = o + Vector(SHWID / 2.0f, SHMID, SHLEN / 2.0f);

		Vector p3 = o + Vector(-SHWID / 2.0f, SHMID, -SHMAST / 2.0f);
		Vector p4 = o + Vector(SHWID / 2.0f, SHTOP, SHMAST / 2.0f);

		// NOTE: If ships were ever rotated, we'd have to rotate the corner
		// points here, but they aren't.

		Box box1(p1, p2);
		Box box2(p3, p4);

		if( (true == box1.TestIntersection(orig, r)) ||
			(true == box2.TestIntersection(orig, r)))
		{
			Vector line = o + Vector(0.0f, SHTOP, 0.0f) - orig;
			distance = line.Length();

			id = object->id();
			hit = true;
		}
	}

	return hit;
}

void DrawShipObject::Obtain()
{
	obtainPrivate_Ship();
}

void DrawShipObject::Release()
{
	releasePrivate_Ship();
}

void DrawShipObject::PopupFunction(wxString& name, wxString& owner, 
	wxBitmap&, bool &createBitmap, ColorType& color)
{
	name = stShip;

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

void DrawShipObject::RenderOutline() const
{
}

void DrawShipObject::RenderSelection() const
{
	DrawObject::material(Selection).UseMaterial();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	drawShipOutline();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void DrawShipObject::RenderModel(const wxInt32) const
{
	//allow for ships with set colors (i.e., the pirate)
	if(MaterialSize != mMaterial)
	{
		DrawObject::material(mMaterial).UseMaterial();
	}
	else
	{
		DrawObject::material(GetObject()).UseMaterial();
	}

	drawShip();
	DrawObject::material(White).UseMaterial();

	// If this ship is part of Longest Road, also draw that.
	ShipObject *ship = static_cast<ShipObject *>(GetObject());
	wxASSERT(NULL != ship);
	if(true == ship->longest())
	{
		DrawObject::material(LongestOutline).UseMaterial();
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		drawShipOutline();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//



