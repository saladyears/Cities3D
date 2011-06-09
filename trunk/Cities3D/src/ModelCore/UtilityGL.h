/*
 *  Cities3D - Copyright (C) 2001-2009 Jason Fugate (saladyears@gmail.com)
 * 
 *  This program is free software; you can redistribute it and/or modify it 
 *  under the terms of the GNU General Public License as published by the Free 
 *  Software Foundation; either version 2 of the License, or (at your option) 
 *  any later version.
 * 
 *  This program is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; withcoords even the implied warranty of MERCHANTABILITY 
 *  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License 
 *  for more details.
 */
#pragma once

#include "style.h"	//READ THIS BEFORE MAKING ANY CHANGES TO THIS FILE!!!

//---------------------------- SYSTEM INCLUDES  -----------------------------//

//---------------------------- USER INCLUDES    -----------------------------//
#include "ModelExport.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
struct Vector;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Namespace: UtilityGL
// 
// A namespace that provides miscellaneous utility functions
// for OpenGL.
//
// Project:
//     <DatabaseCore>
//
// Include:
//     UtilityGL.h
//
namespace UtilityGL
{
	//-----------------------------------------------------------------------//
	// Function: cornerCoords
	//
	// Adjusts given coordinates from the center of a tile to the
	// specified corner.
	//
	// Parameters:
	//     coords - The coordinates to adjust.  They should already be set to
	//              the center position of the tile.
	//     corner - The corner to adjust the coordinates to.  Must be between 
	//              0 and 5, inclusive.
	//     offset - The scale factor for the adjustment.
	//
	void MODEL_EXPORT_SPEC cornerCoords(Vector &coords, wxInt32 corner, 
		float offset = 1.0f);

	//-----------------------------------------------------------------------//
	// Function: sideCoords
	//
	// Adjusts given coordinates from the center of a tile to the
	// midpoint of the specified side.
	//
	// Parameters:
	//     coords - The coordinates to adjust.  They should already be set to
	//              the center position of the tile.
	//     side - The side to adjust the coordinates to.  Must be between 0 
	//            and 5, inclusive.
	//
	void MODEL_EXPORT_SPEC sideCoords(Vector &coords, float &rotation, 
		wxInt32 side);

	void MODEL_EXPORT_SPEC planeCoords(const float height, 
		const Vector &origin, const Vector &ray, Vector &coords, 
		float &length);
}

//---------------------------- PROTOTYPES       -----------------------------//
