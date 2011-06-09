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
#include <list>
#include <wx/image.h>
#include <wx/hashmap.h>
#include <boost/scoped_array.hpp>

//---------------------------- USER INCLUDES    -----------------------------//
#include "DatabaseExport.h"
#include "Singleton.h"
#include "ColorInfoData.h"

//---------------------------- DEFINES          -----------------------------//
#define IMAGE ImageEngine::get()

//---------------------------- TYPEDEFS         -----------------------------//
class ImageSet;
typedef boost::shared_ptr<ImageSet> ImageSetPtr;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: ImageEngine
// 
// The standard implementation of the <IImageEngine> interface.
// ImageEngine is the respository for all of the <ImageSets> loaded from
// the current <RuleSets>.  Each <ImageSet> is stored in a string-based
// hash map where the <ImageSet> key is the string key in the hash.
// Since multiple <ImageSets> may have the same key, each string in
// the hash is mapped to a std::list of <ImageSets>, so that multiple
// <ImageSets> can share the same key in peace and harmony.  The functions
// required by the <IImageEngine> interface are implemented by taking
// the hash and performing the necessary operations.  See the function
// documentation below for details.
//
// Project:
//     <DatabaseCore>
//
// Derived From:
//     <Singleton>
//
// Include:
//     ImageEngine.h
//
class DATABASE_EXPORT_SPEC ImageEngine : public Singleton<ImageEngine>
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	typedef std::pair<HashString, wxImage> ImagePair;
	typedef std::vector<ImagePair> ImagePairArray;

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: AddImageSet
	//
	// Adds an <ImageSet> to the internal store.
	//
	// Parameters:
	//     imageSet - The <ImageSet> to add.
	//
	void AddImageSet(const ImageSetPtr &imageset);

	//-----------------------------------------------------------------------//
	// Function: Clear
	//
	// Clears the hash of <ImageSets>.
	//
	void Clear();

	//-----------------------------------------------------------------------//
	// Function: AddSkinSet
	//
	// Adds the given <ImageSet> for use as a skin.
	//
	// Parameters:
	//     imageset - The skin <ImageSet>.
	//
	void AddSkinSet(const ImageSetPtr &imageset);

	//-----------------------------------------------------------------------//
	// Function: Colorize
	//
	// Colors the given build image with the given color and resizes it to the
	// given size.  Uses the given mask as a template for filling in the color.
	// White parts of the mask are colorized, black parts are not.
	//
	// Parameters:
	//     image - The image in the Build category to color.
	//     mask - The mask image in the Build category to use as a mask.
	//     color - The color.
	//     size - The desired size.
	//
	// Returns:
	//     The colorized image.
	//
	wxImage Colorize(const HashString &image, const HashString &mask, 
		const ColorType color, const wxSize &size, bool useTransparent = false);

	//-----------------------------------------------------------------------//
	// Function: FindImage
	//
	// Tests to see if an image exists in the image database.
	//
	// Parameters:
	//     key - The image group key to search in.
	//     name - The image name to look up.
	//
	// Returns:
	//     A bool that is true if the image was found, and false if not.
	//
	bool FindImage(const HashString &key, const HashString &name) const;

	//-----------------------------------------------------------------------//
	// Function: GetImage
	//
	// Returns a single image based on the key/name combination.
	// The image is scaled to the given size, if any.  If the key
	// does not exist, or the name does not exist in the given key,
	// an error message is thrown to the screen.
	//
	// *NOTE*: Returning a wxImage by value is acceptable, because wxImage
	// is reference counted internally and no data is actually copied.
	//
	// Parameters:
	//     key - The image group key to search in.
	//     name - The image name to look up.
	//     size - The size of the outgoing image.
	//
	// Returns:
	//     The image.  May be an empty image if the image was not
	//     based on the given key and name.
	//
	wxImage &GetImage(const HashString &key, const HashString &name, 
		const wxSize &size = wxDefaultSize) const;
	
	//-----------------------------------------------------------------------//
	// Function: GetAllImages
	//
	// Fills an array with all images belonging to a given key.  The
	// array is filled with std::pair<wxString, wxImage> combinations
	// The string is the name of the image, and the wxImage is the image
	// itself.
	//
	// Parameters:
	//     key - The image group key of the images to retrieve.
	//     array - The outgoing array of string/image pairs.
	//
	void GetAllImages(const HashString &key, ImagePairArray &array);
	
	//-----------------------------------------------------------------------//
	// Function: ReadImages
	//
	// Reads images from a .tex file.
	//
	// Parameters:
	//     size - The number of images in the .tex file.
	//     pList - A pointer to an array of images.  Must be size in length.
	//     filename - The name of the .tex file to load.
	//     skin - Whether to read the images from the skins directory or not.
	//
	void ReadImages(const wxInt32 size, wxImage pList[], 
		const wxString &filename, bool skin = false) const;

	//-----------------------------------------------------------------------//
	// Function: IgnoreCache
	//
	// Tells the ImageEngine to read from the disk regardless
	// of the current cache on the next read.
	//
	void IgnoreCache() const;

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: LoadOnDemand
	//
	// Loads images from a .tex file on demand.  The image retrieval
	// strategy for ImageEngine assumes that any image loaded from a
	// certain .tex file probably means that another request for an
	// image from that same .tex file is going to come soon.  Therefore,
	// it keeps the currently loaded .tex file in a local cache.  The
	// LoadOnDemand function manages that local cache, only loading a 
	// new .tex file when the existing one doesn't provide the image
	// the caller is looking for.
	//
	// Parameters:
	//     filename - The name of the .tex file the image is in.
	//     size - The number of images in the .tex file.
	//     skin - Whether to read the images from the skins directory or not.
	//
	void LoadOnDemand(const wxString &filename, wxInt32 size, bool skin) const;

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	typedef std::list<ImageSetPtr> ImageSetList;
	WX_DECLARE_HASH_STRING_HASH_MAP(ImageSetList, ImageSetHash);

	//-----------------------------------------------------------------------//
	// Hash: mImageSetHash
	//
	// The string hash of <ImageSets>. All of the loaded <ImageSets> 
	// are stored grouped together by their key.  Any particular
	// key may have multiple <ImageSets> to look through in
	// order to find exactly which image to use, so each string key
	// in the hash actually maps to a std::list of <ImageSets> that might
	// all share the key.
	//
	ImageSetHash mImageSetHash;

	//-----------------------------------------------------------------------//
	// Hash: mImageSetHash
	//
	// The hash of <ImageSets> used in the skin.
	//
	ImageSetHash mSkinSetHash;

	friend class Singleton<ImageEngine>;
};

//---------------------------- PROTOTYPES       -----------------------------//
// This macro is basically a copy of the ImageEngine macro without ruleset
// information since skins don't use rulesets.
#define IMPLEMENT_SKIN_SET_START(Key) \
namespace \
{ \
	struct sAutoRegister##Key \
	{ \
		sAutoRegister##Key(ImageSet *imageset) \
		{ \
			wxInt32 count = 0;

#define IMPLEMENT_SKIN_SET_NAME(Name) \
			imageset->index(#Name, count++);

#define IMPLEMENT_SKIN_SET_END(Key, TexFile) \
			ImageEngine::get().AddSkinSet(ImageSetPtr(imageset)); \
		} \
	}; \
	ImageSet *imageset##Key = new ImageSet(#Key, wxT(#TexFile)); \
	sAutoRegister##Key object##Key(imageset##Key); \
}
