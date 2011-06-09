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
#include <wx/hashmap.h>

//---------------------------- USER INCLUDES    -----------------------------//
#include "DatabaseExport.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class ImageSet;
typedef boost::shared_ptr<ImageSet> ImageSetPtr;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: ImageSet
// 
// A repository of knowledge about a .tex file. An ImageSet is 
// a data structure that describes a .tex file and a way of
// knowing what is in that .tex file so that the images 
// contained in it can be used in the rest of the game.
// When a <RuleSet> is loaded, its ImageSets are given to the 
// <IImageEngine> which manages them and provides access for 
// the rest of the modules. 
//
// All ImageSets are accessed by 'keys', which are string values 
// that name groups of images.  It may be that multiple ImageSets 
// may have the same key if they  are all extensions for a certain 
// kind of image.  For instance, all tile images are referenced 
// by the key 'TileType' and there may be several ImageSets that 
// provide images for tiles, so they would all have the same key.
//
// ImageSets also contain a string hash that maps a string name
// to the index into the images in the .tex file.  When an ImageSet
// is created, its hash must be filled with data that corresponds
// to the layout of the .tex file it is supposed to represent.  This allows
// for objects that need to use the images in the .tex file to ask
// for a particular image by name, and the hash maps the name to the index
// into the .tex file of the image to return.  In this way, all image
// requests can be done without the use of difficult and error-prone
// number indices, but rather through simple strings.
//
// Project:
//     <RulesCore>
//
// Include:
//     ImageSet.h
//
class DATABASE_EXPORT_SPEC ImageSet
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: ImageSet
	//
	// The ImageSet constructor.  Stores the given key
	// and .tex file name.
	//
	// Parameters:
	//     key - The key for this ImageSet.  See <ImageSet> for more details.
	//     file - The name of the .tex file this ImageSet describes.
	//
	ImageSet(const HashString &key, const wxString &file) 
		: mKey(key)
		, mFile(file) 
	{}

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: numImages
	//
	// Returns the number of images in the .tex file the
	// ImageSet describes.
	//
	// Returns:
	//     The number of images in the .tex file.
	//
	wxInt32 numImages() const 
	{
		return mIndexHash.size();
	}

	//-----------------------------------------------------------------------//
	// Function: key
	//
	// Returns the key used to group the images in the ImageSet.
	//
	// Returns:
	//     A const reference to the key of the ImageSet.
	//
	const HashString &key() const 
	{
		return mKey;
	}
	
	//-----------------------------------------------------------------------//
	// Function: file
	//
	// Returns the file name of the .tex file the ImageSet
	// describes.
	//
	// Returns:
	//     A const reference to the name of the .tex file
	//
	const wxString &file() const 
	{
		return mFile;
	}
	
	//-----------------------------------------------------------------------//
	// Function: index
	//
	// Returns the index in the .tex file of the given
	// image name.
	//
	// Parameters:
	//     key - The name of the image to look up.
	//
	// Returns:
	//     The index in the .tex file of the given key.  May
	//     be -1 if the key does not exist in the ImageSet.
	//
	wxInt32 index(const HashString &key) const;

	//-----------------------------------------------------------------------//
	// Function: index
	//
	// Sets the index in the .tex file of the given image
	// name.
	//
	// Parameters:
	//     key - The name of the image in the .tex file.
	//     index - The index in the .tex file of the given key.
	//
	void index(const HashString &key, const wxInt32 index);

	WX_DECLARE_HASH_STRING_HASH_MAP(wxInt32, IntHash);

	//-----------------------------------------------------------------------//
	// Function: hash_begin
	//
	// Provides an iterator to the start of the .tex file index hash.
	//
	// Returns:
	//     A const_iterator to the start of the index hash.
	//
	IntHash::const_iterator hash_begin() const {return mIndexHash.begin();}

	//-----------------------------------------------------------------------//
	// Function: hash_end
	//
	// Provides an iterator to the end of the .tex file index hash.
	//
	// Returns:
	//     A const_iterator to the end of the index hash.
	//
	IntHash::const_iterator hash_end() const {return mIndexHash.end();}

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Object: mKey
	//
	// The key used to group the ImageSet.
	//
	HashString mKey;

	//-----------------------------------------------------------------------//
	// Object: mFile
	//
	// The .tex file the ImageSet describes.
	//
	wxString mFile;

	//-----------------------------------------------------------------------//
	// Object: mIndexHash
	//
	// The hash map of strings that name the images contained in the
	// .tex file to their index in that .tex file.
	//
	IntHash mIndexHash;
};

//---------------------------- PROTOTYPES       -----------------------------//
