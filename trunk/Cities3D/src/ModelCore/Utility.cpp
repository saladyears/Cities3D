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
#include "Utility.h"
#include "UtilityGL.h"
#include "UtilityMap.h"
#include "Map.h"
#include "Game.h"
#include "Vector.h"
#include "RNG.h"
#include "define/defineGL.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
wxInt32 Utility::encodeSel(const wxInt32 tile, const wxInt32 side)
{
	return (tile << 16) | side;
}

std::pair<wxInt32, wxInt32> Utility::decodeSel(const wxInt32 sel)
{
	wxInt32 tile = sel >> 16;
	wxInt32 side = sel & 0xFFFF;

	return std::make_pair(tile, side);
}

//helper for the ubiquitous need for calculating neighboring tiles and corners
void Utility::otherTiles(const GamePtr &game, const wxInt32 nT1, 
						 const wxInt32 nC1, wxInt32 &nT2, wxInt32 &nC2, 
						 wxInt32 &nT3, wxInt32 &nC3)
{
	//get the other two tiles
	nT2 = game->tile(nT1)->read<IntArray>(shSides)[nC1];
	nT3 = game->tile(nT1)->read<IntArray>(shSides)[(nC1 + 5) % 6];

	//set opposite corners
	nC2 = (nC1 + 4) % 6;
	nC3 = (nC1 + 2) % 6;
}

void Utility::calculateFontSize(wxDC &dc, wxFont &font, wxInt32 height,
								wxInt32 rows)
{
	for(wxInt32 size = 1; size <= 100; size++)
	{
		font.SetPointSize(size);
		dc.SetFont(font);

		//retrieve the text extent
		wxInt32 w, h;
		dc.GetTextExtent(swExtent, &w, &h);

		//as soon as go over the allowed height, set it back to the
		//previous valid size
		if(height < (h * (rows)) + (rows - 1))
		{
			font.SetPointSize(size - 1);
			break;
		}
	}
}

//determine translation from the center of a tile to a given corner
void UtilityGL::cornerCoords(Vector &coords, wxInt32 corner, float offset)
{
	float xOff = (X * 0.5f) * offset;
	float xVal = (X * offset);
	float zVal = (Z * offset);

	switch(corner)
	{
	//TOP LEFT
	case 0: coords.x -= xOff; coords.z -= zVal; break;
	//LEFT
	case 1: coords.x -= xVal; break;
	//BOTTOM LEFT
	case 2: coords.x -= xOff; coords.z += zVal; break;
	//BOTTOM RIGHT
	case 3: coords.x += xOff; coords.z += zVal; break;
	//RIGHT
	case 4: coords.x += xVal; break;
	//TOP RIGHT
	case 5: coords.x += xOff; coords.z -= zVal; break;
	default:
		wxASSERT(false);
		break;
	}
}

void UtilityGL::sideCoords(Vector &coords, float &rotation, wxInt32 side)
{
	float xOff = (X * 0.75f);
	float zOff = (Z * 0.5f);

	switch(side)
	{
	//TOP LEFT
	case 0: coords.x -= xOff; coords.z -= zOff; rotation = 150.0f; break;
	//BOTTOM LEFT
	case 1: coords.x -= xOff; coords.z += zOff; rotation = 30.0f; break;
	//BOTTOM
	case 2: coords.z += Z; rotation = 270.0f; break;
	//BOTTOM RIGHT
	case 3: coords.x += xOff; coords.z += zOff; rotation = 150.0f; break;
	//TOP RIGHT
	case 4: coords.x += xOff; coords.z -= zOff; rotation = 30.0f; break;
	//TOP
	case 5: coords.z -= Z; rotation = 270.0f; break;

	default:
		wxASSERT(false);
		break;
	}
}

void UtilityGL::planeCoords(const float height, const Vector &origin, 
							const Vector &ray, Vector &coords, float &length)
{
	//we assume that the plane we are finding the intersection for is the x-z 
	//plane, plus the given height offset; this means that any point outside of
	//the plane will always be just its y value away in total distance (minus 
	//the given height)
	Vector perpendicular(0.0f, origin.y - height, 0.0f);
	
	Vector perpUnit;
	perpendicular.Normalize(perpUnit);
	
	//find the length along the ray unit vector it takes to get to the plane by
	//using the projection property of dot products
	length = perpendicular.Length() / DotProduct(ray, perpUnit);

	//now create the ray vector that intersects with the plane
	Vector intersect = ray * length;

	//now subtract the intersection vector from the origin and we have the 
	//location on the plane where the ray intersects
	coords = origin - intersect;
}

wxInt32 UtilityMap::prepareDataArray(const Data::IntHash &fillTypes, 
									 HashStringArray &values)
{
	//do not assume that values is empty
	values.clear();

	Data::IntHash::const_iterator it, itEnd(fillTypes.end());
	for(it = fillTypes.begin(); it != itEnd; ++it)
	{
		//fill up the string array with the hash key types, up to the
		//number of that hash key type
		for(wxInt32 i = 0; i < it->second; i++)
		{
			values.push_back(it->first);
		}
	}
	
	// Sort the array alphabetically.
	std::sort(values.begin(), values.end());
	
	//now randomize the currently sorted array of strings
	wxUint32 size = values.size();
	
	for(wxUint32 i = 0; i < size; ++i)
	{
		wxUint32 swapIndex = RAND.pooled(size);
		HashString str = values[i];

		wxLogDebug(wxT("prepareDataArray - Swapping index %u (of %u) with %u - %s with %s"),
			i, size, swapIndex, str.cwx_str(), values[swapIndex].wx_str().c_str());

		values[i] = values[swapIndex];
		values[swapIndex] = str;
	}

	return size;
}
//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//



