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
#pragma once

#include "style.h"	//READ THIS BEFORE MAKING ANY CHANGES TO THIS FILE!!!

//---------------------------- SYSTEM INCLUDES  -----------------------------//

//---------------------------- USER INCLUDES    -----------------------------//
#include "ModelExport.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class Game;
typedef boost::shared_ptr<Game> GamePtr;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Namespace: Utility
// 
// A namespace that provides miscellaneous utility functions.
//
// Project:
//     <DatabaseCore>
//
// Include:
//     Utility.h
//
namespace Utility
{
	//-----------------------------------------------------------------------//
	// Function: encodeSel
	//
	// Encodes a tile/corner selection into one integer value.
	//
	// Parameters:
	//     tile - The tile index.
	//     side - The side/corner index.
	//
	// Returns:
	//     A single integer value representing the tile/corner combination.
	//
	wxInt32 MODEL_EXPORT_SPEC encodeSel(const wxInt32 tile, const wxInt32 side);

	//-----------------------------------------------------------------------//
	// Function: decodeSel
	//
	// Decodes a single tile/corner selection into separate tile and 
	// corner values.
	//
	// Parameters:
	//     sel - The combined tile/corner value.
	//
	// Returns:
	//     A std::pair of the separate tile and corner.  The first value in
	//     the pair is the tile, the second value is the corner.
	//
	std::pair<wxInt32, wxInt32> MODEL_EXPORT_SPEC decodeSel(const wxInt32 sel);

	//-----------------------------------------------------------------------//
	// Function: otherTiles
	//
	// Calculates neighboring tiles based on the given <Game>, tile and corner.
	//
	// Parameters:
	//     game - The current <Game>.
	//     nT1 - The given tile.
	//     nC1 - The given corner.
	//     nT2 - The first neighbor tile at the given tile and corner.
	//     nC2 - The first neighbor corner at the given tile and corner.
	//     nT3 - The second neighbor tile at the given tile and corner.
	//     nC3 - The second neighbor corner at the given tile and corner.
	//
	void MODEL_EXPORT_SPEC otherTiles(const GamePtr &game, const wxInt32 nT1, 
		const wxInt32 nC1, wxInt32 &nT2, wxInt32 &nC2, wxInt32 &nT3, 
		wxInt32 &nC3);

	//-----------------------------------------------------------------------//
	// Function: calculateFontSize
	//
	// Calculates the maximum font size for the given font in the given dc
	// that does not go over the given height.
	//
	// Parameters:
	//     dc - The draw context.
	//     font - The font.
	//     height - The maximum height.
	//     rows - The number of rows.
	//
	void MODEL_EXPORT_SPEC calculateFontSize(wxDC &dc, wxFont &font, 
		wxInt32 height, wxInt32 rows = 1);
}

//---------------------------- PROTOTYPES       -----------------------------//
