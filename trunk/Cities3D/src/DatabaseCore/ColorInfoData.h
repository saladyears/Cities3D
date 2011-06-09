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
#include "DatabaseExport.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
enum ColorType
{
	CR_WHITE				= 0,
	CR_RED					,
	CR_ORANGE				,
	CR_BLUE					,
	CR_BROWN				,
	CR_GREEN				,
	CR_PURPLE				,
	CR_BLACK				,

	CR_SIZE					,
	CR_DWORD				= 0x7fffffff
};

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Namespace: ColorInfoData
// 
// A namespace that provides information about each of the colors
// available to players in the game.
//
// Project:
//     <DatabaseCore>
//
// Include:
//     ColorInfoData.h
//
namespace ColorInfoData
{
	//-----------------------------------------------------------------------//
	// Function: Name
	//
	// Provides a string representation of the name of a color.
	//
	// Parameters:
	//     type - The color in question.
	//
	// Returns:
	//     A wxString containing the name of the color.
	//
	const wxString DATABASE_EXPORT_SPEC &Name(const ColorType type);

	//-----------------------------------------------------------------------//
	// Function: InternalName
	//
	// Provides a string representation of the untranslated name of a color.
	//
	// Parameters:
	//     type - The color in question.
	//
	// Returns:
	//     A wxString containing the name of the color.
	//
	const wxString DATABASE_EXPORT_SPEC &InternalName(const ColorType type);

	//-----------------------------------------------------------------------//
	// Function: GLColor
	//
	// Provides a wxColour for displaying OpenGL objects with a color.  The GL 
	// color of a given ColorType is used in for appropriately painting all 
	// OpenGL objects of the player who is using that color.
	//
	// Parameters:
	//     type - The color in question.
	//
	// Returns:
	//     A wxColour containing the OpenGL color.
	//
	wxColour DATABASE_EXPORT_SPEC GLColor(const ColorType type);

	//-----------------------------------------------------------------------//
	// Function: UIColor
	//
	// Provides a wxColour for displaying text with a color.  The UI color
	// of a given ColorType is used in the UI for all text associated with the
	// player who is using that color.
	//
	// Parameters:
	//     type - The color in question.
	//
	// Returns:
	//     A wxColour containing the UI color.
	//
	wxColour DATABASE_EXPORT_SPEC UIColor(const ColorType type);

	//-----------------------------------------------------------------------//
	// Function: UIImage
	//
	// Creates a rectangular image of a color.  The image is the solid color 
	// with two black bars across the top and bottom.  The image color is the 
	// UIColor version of the given ColorType.
	//
	// Parameters:
	//     type - The color in question.
	//     size - The size of the wxImage desired.
	//
	// Returns:
	//     A wxImage containing a rectangluar image of the color.
	//
	wxImage DATABASE_EXPORT_SPEC UIImage(const ColorType type, 
		const wxSize &size = wxSize(16, 16));
};

//---------------------------- PROTOTYPES       -----------------------------//

