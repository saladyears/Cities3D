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
#include "define/defineFile.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Namespace: FilePath
// 
// A namespace that provides correct file paths for various types
// of game data stored on disk.
//
// Project:
//     <DatabaseCore>
//
// Include:
//     FilePath.h
//
namespace FilePath
{
	//-----------------------------------------------------------------------//
	// Function: EnumDataSubDirs
	//
	// Given a dirname, it will look in all standard data dirs (system-wide
	// and user-specific) for directories by that name, and return a list of
	// all their subdirectories
	//
	// Parameters:
	//     dirname - The name of the directory to look for
	//
	// Returns:
	//     A wxArrayString containing the full paths of all the subdirectories
	//     found
	//
	wxArrayString DATABASE_EXPORT_SPEC EnumDataSubDirs(const wxString &sub);

	//-----------------------------------------------------------------------//
	// Function: EnumPlugins
	//
	// It will look in all standard plugin dirs (system-wide
	// and user-specific) for plugin libraries, and return a list of them.
	//
	// Returns:
	//     A wxArrayString containing the full paths of all the plugins found.
	//
	wxArrayString DATABASE_EXPORT_SPEC EnumPlugins();

	//-----------------------------------------------------------------------//
	// Function: EnumLocaleDirs
	//
	// Given a locale name, it will return a list of all the possible paths
	// in which catalogs of that locale may exist.
	// 
	// Parameters:
	//     lang - The name of the locale
	//
	// Returns:
	//     A wxArrayString containing the full paths of all possible locations 
	//     of the catalog files.
	//
	wxArrayString DATABASE_EXPORT_SPEC EnumLocaleDirs(const wxString &lang);

	//-----------------------------------------------------------------------//
	// Function: Tex
	//
	// Provides a full path to a .tex file.  Tex files are by
	// default stored in the /tex directory.  However, the user may
	// have selected an alternate subdirectory under /tex for custom tiles.
	// This function determines if the given file is in the custom
	// directory (if any) and returns the path to it if it exists, 
	// or the path to the default version of the .tex file if not.
	//
	// Parameters:
	//     filename - The name of the .tex file desired.
	//
	// Returns:
	//     A wxString containing the full path to the .tex file.
	//
	wxString DATABASE_EXPORT_SPEC Tex(const wxString &filename);

	//-----------------------------------------------------------------------//
	// Function: Skin
	//
	// Provides a full path to a .tex file.  Skin tex files are by
	// default stored in the /skins directory.  However, the user may
	// have selected an alternate subdirectory under /skins for custom skins.
	// This function determines if the given file is in the custom
	// directory (if any) and returns the path to it if it exists, 
	// or the path to the default version of the .tex file if not.
	//
	// Parameters:
	//     filename - The name of the .tex file desired.
	//
	// Returns:
	//     A wxString containing the full path to the .tex file.
	//
	wxString DATABASE_EXPORT_SPEC Skin(const wxString &filename); 

	//-----------------------------------------------------------------------//
	// Function: Sound
	//
	// Provides a full path to a sound file.  Sound files are by
	// default stored in the /sounds directory.  However, the user may
	// have selected an alternate subdirectory under /sounds for custom sounds.
	// This function determines if the given file is in the custom
	// directory (if any) and returns the path to it if it exists, 
	// or the path to the default version of the sound if not.
	//
	// Parameters:
	//     filename - The name of the sound file desired.
	//
	// Returns:
	//     A wxString containing the full path to the sound file.
	//
	wxString DATABASE_EXPORT_SPEC Sound(const wxString &filename);

	//-----------------------------------------------------------------------//
	// Function: Layout
	//
	// Provides a full path to a layout file.  Layout files are by default
	// stored in the /layouts directory.
	//
	// Parameters:
	//     filename - The name of the layout file desired.
	//
	// Returns:
	//     The full path to the layout file, or an empty string if it does not
	//     exist.
	//
	wxString DATABASE_EXPORT_SPEC Layout(const wxString& filename);

	//-----------------------------------------------------------------------//
	// Function: UserDataDir
	//
	// Provides the full path to the user-writable data directory.
	//
	// Parameters:
	//     dirname - (optional) a directory name sought under the user data dir
	//
	// Returns:
	//     A wxString containing the full path to the data dir.
	//
	wxString DATABASE_EXPORT_SPEC UserDataDir(
			const wxString &dirname = wxEmptyString);

	//-----------------------------------------------------------------------//
	// Function: Data
	//
	// Provides the full path to a data file.  Data files include
	// the game and player databases, as well as the map database.
	//
	// Parameters:
	//     filename - The type of the data file desired.
	//
	// Returns:
	//     A wxString containing the full path to the data file.
	//
	wxString DATABASE_EXPORT_SPEC Data(const DataFileType type);
};
//---------------------------- PROTOTYPES       -----------------------------//

