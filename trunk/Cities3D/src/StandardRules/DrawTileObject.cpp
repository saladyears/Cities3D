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
#include "DrawTileObject.h"
#include "TileObject.h"
#include "GLTextures.h"
#include "GLMaterial.h"
#include "RuleSetDatabase.h"
#include "ConfigDatabase.h"
#include "ImageEngine.h"
#include "Controller.h"
#include "UtilityGL.h"
#include "define/defineGL.h"
#include "ImageEngine.h"
#include "DataObject.h"
#include "ChipObject.h"
#include "IRuleEngine.h"

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
	const wxInt32 NUM_LISTS_TILE				= 3;

	enum
	{
		Tile,
		TileEdge,
		TileOutline
	};

	//tile edge depth
	const float TILE_DEPTH			= 0.23f;

	GLuintArray sLists_Tile;

	//standard vectors used for determining selection
	const float COS_120				= cosf(120.0f * D2R);

	Vector V1(X / 2, 0.0, -Z);
	Vector V2(-X / 2, 0.0, -Z);
	Vector V3(-X, 0.0, 0.0);
	Vector V4(-X / 2, 0.0, Z);
	Vector V5(X / 2, 0.0, Z);
	Vector V6(X, 0.0, 0.0);

	Vector C, H;
	Vector sOrigin, sRay, sPlane;
	float dot, sRayLength;

	//hexagon shaped tile
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

	//the edge of a hexagon along the board
	void buildTileEdge(GLuint id)
	{
		glNewList(id, GL_COMPILE);
			glBegin(GL_QUADS);
				glNormal3d(0.0f, 0.0f, 1.0f);
				glVertex3d(-(X / 2), 0.0, 0.0);
				glVertex3d((X / 2), 0.0, 0.0);
				glVertex3d((X / 2), -TILE_DEPTH, 0.0);
				glVertex3d(-(X / 2), -TILE_DEPTH, 0.0);
			glEnd();
		glEndList();
	}

	//an outline around a hexagon tile
	void buildTileOutline(GLuint id)
	{
		glNewList(id, GL_COMPILE);
			glBegin(GL_LINES);
				glNormal3d(0.0f, 1.0f, 0.0f);
				glVertex3d(X, 0.1, 0.0);
				glVertex3d((X / 2), 0.1, -Z);
				glVertex3d((X / 2), 0.1, -Z);
				glVertex3d(-(X / 2), 0.1, -Z);
				glVertex3d(-(X / 2), 0.1, -Z);
				glVertex3d(-X, 0.1, 0.0);
				glVertex3d(-X, 0.1, 0.0);
				glVertex3d(-(X / 2), 0.1, Z);
				glVertex3d(-(X / 2), 0.1, Z);
				glVertex3d((X / 2), 0.1, Z);
				glVertex3d((X / 2), 0.1, Z);
				glVertex3d(X, 0.1, 0.0);
			glEnd();
		glEndList();
	}

	void obtainPrivate_TileObject()
	{
		//grab all tile images from the image database
		ImageEngine::ImagePairArray images;
		IMAGE.GetAllImages(shTileType, images);

		//now convert them all into GL textures
		wxBOOL mipmap = CONFIG.read<wxInt32>(swConfigSectionOpenGL, swMipmap, TRUE);
		ImageEngine::ImagePairArray::const_iterator it, itEnd(images.end());
		for(it = images.begin(); it != itEnd; ++it)
		{
			sTexHash[it->first] = 
				GLTextures::BuildTexture(it->second, mipmap || false);
		}
		
		//display lists
		sLists_Tile.reset(new GLuint[NUM_LISTS_TILE]);

		//generate
		for(wxInt32 i = 0; i < NUM_LISTS_TILE; i++)
		{
			sLists_Tile[i] = glGenLists(1);
		}

		//create the lists
		buildTile(sLists_Tile[Tile]);
		buildTileEdge(sLists_Tile[TileEdge]);
		buildTileOutline(sLists_Tile[TileOutline]);

		//normalize vectors
		V1.Normalize();
		V2.Normalize();
		V3.Normalize();
		V4.Normalize();
		V5.Normalize();
		V6.Normalize();
	}

	void releasePrivate_TileObject()
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
		glDeleteLists(sLists_Tile[0], NUM_LISTS_TILE);
	}

	void drawTileOutline(const GLMaterial &mat)
	{
		//set polygon mode to outline
		glDisable(GL_TEXTURE_2D);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(3.0f);

		//use the sent in color
		mat.UseMaterial();
		
		//draw the outline
		glCallList(sLists_Tile[TileOutline]);

		//reset GL properties
		glLineWidth(1.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_TEXTURE_2D);
	}

	void drawTile(const DrawTileObject * const pObject)
	{
		TileObject *pTile = static_cast<TileObject *>(pObject->GetObject());

		HashString type = pTile->read<HashString>(shTileType);

		//if this tile has an existing port object on it, then don't draw it as
		//a port, draw it as an ocean so that there aren't two representations 
		//of a port, one on top of the other
		if(pTile->port())
		{
			type = shOcean;
		}

		//sanity
		GLuintHash::iterator it(sTexHash.find(type));
		wxASSERT(sTexHash.end() != it);

		glBindTexture(GL_TEXTURE_2D, sTexHash[type]);

		glCallList(sLists_Tile[Tile]);

		//special flags
		if(true == pTile->isset(TileObject::OutlineRed))
		{
			drawTileOutline(DrawObject::material(DrawObject::Red));
		}
	}
}

//---------------------------- PUBLIC           -----------------------------//
IMPLEMENT_DRAW_OBJECT(DrawTileObject, TileObject, STANDARD);

DrawTileObject::DrawTileObject(const GameObjectPtr &object)
: DrawObject(object, 0)
{
}

DrawTileObject::~DrawTileObject()
{
}

bool DrawTileObject::CanSelect(const Vector &origin, const Vector &ray, 
							   float &distance, wxInt32 &id, bool popup)
{
	TileObject *pTile = static_cast<TileObject *>(GetObject());

	//sanity
	if( (NULL == pTile) ||
		((false == popup) &&
		(false == pTile->isset(IGameObject::Select))))
	{
		return false;
	}

	//calculate where the ray, coming from the origin, intersects the x-z 
	//plane, which gives the coordinates where we need to see if this hex is
	if( (sOrigin != origin) ||
		(sRay != ray))
	{
		UtilityGL::planeCoords(0.0f, origin, ray, sPlane, sRayLength);

		sOrigin = origin;
		sRay = ray;
	}

	//the origin is the center of the tile
	Vector center = pTile->coords();

	//now test the dot products of each of the hexagon vectors
	C = center - Vector(X, 0.0, 0.0);
	H = sPlane - C;
	H.Normalize();
	dot = DotProduct(H, V1);

	if(COS_120 > dot)
	{
		return false;
	}

	C = center - Vector(X / 2, 0.0, -Z);
	H = sPlane - C;
	H.Normalize();
	dot = DotProduct(H, V2);

	if(COS_120 > dot)
	{
		return false;
	}

	C = center - Vector(-X / 2, 0.0, -Z);
	H = sPlane - C;
	H.Normalize();
	dot = DotProduct(H, V3);
	
	if(COS_120 > dot)
	{
		return false;
	}

	C = center - Vector(-X, 0.0, 0.0);
	H = sPlane - C;
	H.Normalize();
	dot = DotProduct(H, V4);
	
	if(COS_120 > dot)
	{
		return false;
	}

	C = center - Vector(-X / 2, 0.0, Z);
	H = sPlane - C;
	H.Normalize();
	dot = DotProduct(H, V5);
	
	if(COS_120 > dot)
	{
		return false;
	}

	C = center - Vector(X / 2, 0.0, Z);
	H = sPlane - C;
	H.Normalize();
	dot = DotProduct(H, V6);
	
	if(COS_120 > dot)
	{
		return false;
	}

	id = pTile->id();
	distance = sRayLength;

	return true;
}

void DrawTileObject::Obtain()
{
	obtainPrivate_TileObject();
}

void DrawTileObject::Release()
{
	releasePrivate_TileObject();
}

void DrawTileObject::PopupFunction(wxString& name, wxString& owner, 
	wxBitmap& bitmap, bool &, ColorType&)
{
	// Get information out of the tile object.
	TileObject *pTile = static_cast<TileObject *>(GetObject());

	HashString str = pTile->read<HashString>(shTileType);

	// See if there is a bitmap for this item.
	if(true == IMAGE.FindImage(shResources, str))
	{
		wxImage image = IMAGE.GetImage(shResources, str);

		// Use a 85% sized image.

		bitmap = wxBitmap(IMAGE.GetImage(shResources, str, wxSize(
			static_cast<wxInt32>(image.GetWidth() * .85),
			static_cast<wxInt32>(image.GetHeight() * .85))));

		float percent = 0.0f;
		
		// Set the "owner" text to what percentage this tile gets rolled.
		const ChipObjectPtr& chip = pTile->chip();
		
		if(chip)
		{
			wxInt32 roll = chip->roll();

			if(-1 != roll)
			{
				if(6 >= roll)
				{
					percent = static_cast<float>(roll - 1) / 36.0f;
				}
				else
				{
					percent = static_cast<float>(13 - roll) / 36.0f;
				}

				percent *= 100.0f;

				wxString temp = wxString::Format(swFloatPercent.c_str(), percent);
                owner = stOdds + swColon + swSpace + temp;
			}
		}
	}

	// Set the final outgoing resource name.
	DataObject input(str), output;
	RULE.Decide(shLogicResourceName, input, output);
	name = output.read<wxString>();
}

void DrawTileObject::RenderOutline() const
{
	drawTileOutline(DrawObject::material(DrawObject::Gold));
}

void DrawTileObject::RenderSelection() const
{
	drawTileOutline(DrawObject::material(DrawObject::Selection));
}

void DrawTileObject::RenderModel(const wxInt32 selection) const
{
	(void) selection;

	drawTile(this);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//


