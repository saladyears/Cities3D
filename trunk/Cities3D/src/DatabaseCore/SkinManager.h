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
#include <wx/image.h>

//---------------------------- USER INCLUDES    -----------------------------//
#include "DatabaseExport.h"
#include "Singleton.h"

//---------------------------- DEFINES          -----------------------------//
#define SKIN	SkinManager::get()

//---------------------------- TYPEDEFS         -----------------------------//
class wxXmlNode;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: SkinManager
// 
// A Singleton class that reads in the XML document that generates the user's
// desired UI skin.
//
// Derived From:
//     <Singleton>
//
// Project:
//     <DatabaseCore>
//
// Include:
//     SkinManager.h
//
class DATABASE_EXPORT_SPEC SkinManager : public Singleton<SkinManager>
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: LoadXML
	//
	// Loads the skin XML file off disk, from the location specified by the
	// <ConfigDatabase>.
	//
	void LoadXML();

	//-----------------------------------------------------------------------//
	// Function: Font
	//
	// Returns the font specified by the given key, or the default font if the
	// key does not exist.
	//
	// Parameters:
	//     key - The key that specifies the font.
	//
	// Returns:
	//     A const reference to the font that matches the key.
	//
	const wxFont &Font(const HashString &key) const;

	//-----------------------------------------------------------------------//
	// Function: Element
	//
	// Returns the color specified by the given key, or white if the key does
	// not exist.
	//
	// Parameters:
	//     key - The key that specifies the color.
	//
	// Returns:
	//     The color.
	//
	const wxColour Element(const HashString &key) const;

	//-----------------------------------------------------------------------//
	// Function: Size
	//
	// Returns the size specified by the given key, or zero if the key does
	// not exist.
	//
	// Parameters:
	//     key - The key that specifies the size.
	//
	// Returns:
	//     The size.
	//
	const wxInt32 Size(const HashString &key) const;

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: SkinManager
	// 
	// The SkinManager constructor. 
	//
	SkinManager();

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: ReadFonts
	//
	// Reads the fonts section from the XML file.
	//
	// Parameters:
	//     parent - The parent XML node in the fonts section.
	//
	void ReadFonts(wxXmlNode * const parent);

	//-----------------------------------------------------------------------//
	// Function: ReadColors
	//
	// Reads the colors section from the XML file.
	//
	// Parameters:
	//     parent - The parent XML node in the colors section.
	//
	void ReadColors(wxXmlNode * const parent);

	//-----------------------------------------------------------------------//
	// Function: ReadElements
	//
	// Reads the elements section from the XML file.
	//
	// Parameters:
	//     parent - The parent XML node in the elements section.
	//
	void ReadElements(wxXmlNode * const parent);

	//-----------------------------------------------------------------------//
	// Function: ReadSizes
	//
	// Reads the sizes section from the XML file.
	//
	// Parameters:
	//     parent - The parent XML node in the sizes section.
	//
	void ReadSizes(wxXmlNode * const parent);

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Object: mFontHash
	//
	// The hash of UI fonts.
	//
	WX_DECLARE_HASH_STRING_HASH_MAP(wxFont, FontHash);
	mutable FontHash mFontHash;

	//-----------------------------------------------------------------------//
	// Object: mElementHash
	//
	// The hash of UI colors.
	//
	WX_DECLARE_HASH_STRING_HASH_MAP(wxColour, ColorHash);
	ColorHash mElementHash;

	//-----------------------------------------------------------------------//
	// Object: mSizeHash
	//
	// The hash of UI sizes.
	//
	WX_DECLARE_HASH_STRING_HASH_MAP(wxInt32, SizeHash);
	SizeHash mSizeHash;

	friend class Singleton<SkinManager>;
};

//---------------------------- PROTOTYPES       -----------------------------//
