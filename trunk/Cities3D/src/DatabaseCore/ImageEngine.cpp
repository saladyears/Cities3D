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
#include "ImageEngine.h"
#include "FilePath.h"
#include "ColorInfoData.h"
#include "ImageSet.h"
#include "SkinManager.h"
#include "Image.h"
#include "define/defineFile.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	typedef boost::scoped_array<wxImage> ImageArray;

	//the currently opened .tex file; this provides a
	//simple caching mechanism based on the idea that
	//file requests will come in clumps and that the same
	//file will be requested numerous times in a row;
	//therefore, only one open file is maintained
	wxString sOpenFile;
	ImageArray sOpenImageArray;

	wxImage sImageEngine;
	bool sIgnoreCache = false;
}

//---------------------------- PUBLIC           -----------------------------//
void ImageEngine::AddImageSet(const ImageSetPtr &imageset)
{
	wxASSERT(imageset);

	mImageSetHash[imageset->key()].push_back(imageset);
}


void ImageEngine::AddSkinSet(const ImageSetPtr &imageset)
{
	wxASSERT(imageset);

	mSkinSetHash[imageset->key()].push_back(imageset);
}

void ImageEngine::Clear()
{
	mImageSetHash.clear();
}

wxImage ImageEngine::Colorize(const HashString &image, const HashString &mask, 
	const ColorType color, const wxSize &size, bool useTransparent)
{
	// Get the original image and the mask image.
	wxImage original = GetImage(shBuild, image).Copy();

	wxInt32 w = original.GetWidth();
	wxInt32 h = original.GetHeight();

	bool is_masked = (shNoMask != mask);

	wxImage masked;
	if(true == is_masked)
	{
		masked = GetImage(shBuild, mask).Copy();
		wxASSERT(w == masked.GetWidth());
		wxASSERT(h == masked.GetHeight());
	}	
	
	// Store off the background color.
	wxColour background = SKIN.Element(shBackground);
	wxColour transparent = SKIN.Element(shIconTransparent);
	wxColour colorize = ColorInfoData::GLColor(color);
	wxColour white(255, 255, 255);
	wxColour black(0, 0, 0);

	if(true == useTransparent)
	{
		background = transparent;
	}

	// Create the outgoing image at the full size.
	wxImage final(w, h);

	// Get data pointers to all three.
	wxUint8 *o = original.GetData();
	wxUint8 *f = final.GetData();
	wxUint8 *m = NULL;
	
	if(true == is_masked)
	{
		m = masked.GetData();
	}	

	size_t pixels = w * h * 3;
	wxUint8 *end = o + pixels;

	// Iterate through the pixels, copying as needed into the final image.
	while(o < end)
	{
		wxColour t(o[0], o[1], o[2]);

		// If we have a transparent pixel in the original, set the final image
		// pixel as the background color.
		if(transparent == t)
		{
			f[0] = background.Red();
			f[1] = background.Green();
			f[2] = background.Blue();
		}
		// Otherwise, if we have a masked pixel, or aren't using masks, 
		// colorize the original pixel with the new color.
		else if( (false == is_masked) ||
				 (white == wxColour(m[0], m[1], m[2])))
		{
			f[0] = static_cast<wxUint8>(o[0] * (colorize.Red() / 255.0f));
			f[1] = static_cast<wxUint8>(o[1] * (colorize.Green() / 255.0f));
			f[2] = static_cast<wxUint8>(o[2] * (colorize.Blue() / 255.0f));
		}
		else if( (black == t) &&
				 (true == useTransparent))
		{
			f[0] = background.Red();
			f[1] = background.Green();
			f[2] = background.Blue();
		}

		// And finally, if it's not transparent, and not masked, use the
		// original pixel.
		else
		{
			f[0] = o[0];
			f[1] = o[1];
			f[2] = o[2];
		}

		o += 3;
		f += 3;

		if(true == is_masked)
		{
			m += 3;
		}
	}

	final = Image::Resize(final, size);
	
	return final;
}

bool ImageEngine::FindImage(const HashString &key, const HashString &name) const
{
	bool ret = true;

	//first off, look to see what image set can handle this key
	ImageSetHash::const_iterator it(mImageSetHash.find(key));
	if(mImageSetHash.end() == it)
	{
		// If it doesn't exist in the normal images, perhaps it is in the
		// skin images.
		it = mSkinSetHash.find(key);

		if(mSkinSetHash.end() == it)
		{
			ret = false;
		}
	}

	if(true == ret)
	{
		ret = false;

		//we've got an image set for the key, now find an image set that can
		//handle the name
		wxInt32 index = -1;
		ImageSetList::const_iterator it2, itEnd(it->second.end());
		for(it2 = it->second.begin(); it2 != itEnd; ++it2)
		{
			wxASSERT((*it2));

			index = (*it2)->index(name);
			
			if(-1 != index)
			{
				ret = true;
				break;
			}
		}
	}

	return ret;
}

wxImage &ImageEngine::GetImage(const HashString &key, const HashString &name, 
							   const wxSize &size) const
{
	bool skin = false;

	//first off, look to see what image set can handle this key
	ImageSetHash::const_iterator it(mImageSetHash.find(key));
	if(mImageSetHash.end() == it)
	{
		// If it doesn't exist in the normal images, perhaps it is in the
		// skin images.
		it = mSkinSetHash.find(key);

		if(mSkinSetHash.end() == it)
		{
			wxLogError(wxString::Format(
				wxT("Could not find any image set for %s."), 
				key.cwx_str()));
			return sImageEngine;
		}

		skin = true;
	}

	//we've got an image set for the key, now find an image set that can
	//handle the name
	wxInt32 index = -1;
	ImageSetList::const_iterator it2, itEnd(it->second.end());
	for(it2 = it->second.begin(); it2 != itEnd; ++it2)
	{
		wxASSERT((*it2));

		index = (*it2)->index(name);
		
		if(-1 != index)
		{
			break;
		}
	}

	//if we've got no index, this name isn't accounted for in the current
	//image sets
	if(-1 == index)
	{
		wxLogError(wxString::Format(
			wxT("Could not find %s in image sets for %s."), 
			name.cwx_str(), key.cwx_str()));
		return sImageEngine;
	}

	//we've found an image set that can handle it, so load its image file
	//into memory
	LoadOnDemand((*it2)->file(), (*it2)->numImages(), skin);

	wxImage image = sOpenImageArray[index].Copy();

	//rescale if needed
	if(wxDefaultSize != size)
	{
		image = Image::Resize(image, size);
	}

	sImageEngine = image;
	return sImageEngine;
}

void ImageEngine::GetAllImages(const HashString &key, ImagePairArray &array)
{
	//design by contract
	wxASSERT(true == array.empty());

	//first off, look to see what image set can handle this key
	ImageSetHash::iterator it(mImageSetHash.find(key));
	if(mImageSetHash.end() == it)
	{
		wxLogError(wxString::Format(wxT("Could not find any image set for %s."), 
			key.cwx_str()));
		return;
	}

	ImageSetList::const_iterator it2, itEnd(it->second.end());
	for(it2 = it->second.begin(); it2 != itEnd; ++it2)
	{
		//load this image sets images
		LoadOnDemand((*it2)->file(), (*it2)->numImages(), false);

		//fill the array with all image keys and the images themselves
		ImageSet::IntHash::const_iterator it3, itEnd3((*it2)->hash_end());
		for(it3 = (*it2)->hash_begin(); it3 != itEnd3; ++it3)
		{
			array.push_back(std::make_pair(it3->first, 
				sOpenImageArray[it3->second]));
		}
	}
}

void ImageEngine::ReadImages(const wxInt32 size, wxImage pList[], 
							 const wxString &filename, bool skin) const
{
	//figure out which file it is
	wxString file;
	if(true == skin)
	{
		file = FilePath::Skin(filename);
	}
	else
	{
		file = FilePath::Tex(filename);
	}
	
	wxFile in;
	in.Open(file);
	wxFileInputStream x(in);
	wxDataInputStream fileIn(x);

	//open the right texture set
	if(false == in.IsOpened())
	{
		wxLogError(wxString::Format(wxT("Could not open %s."), file.c_str()));
		
		return;
	}

	//make sure it's the write file type
	boost::scoped_array<char> flag(new char[TEX_FLAG.size()]);
	in.Read(flag.get(), TEX_FLAG.size());

	//sanity
	if(TEX_FLAG != HashString(flag.get(), TEX_FLAG.size()))
	{
		wxLogError(wxString::Format(wxT("The file %s is not a valid Cities3D ")
			wxT(".tex file."), file.c_str()));
		return;
	}

	//read through the base textures
	for(wxInt32 i = 0; i < size; i++)
	{
		wxString strIn;
		wxUint32 dwLength;

		//read the file name and length
		fileIn >> strIn;
		fileIn >> dwLength;

		//read the file into memory
		boost::scoped_array<wxUint8> bytes(new wxUint8[dwLength]);
		in.Read(bytes.get(), dwLength);

		wxMemoryInputStream mem(bytes.get(), dwLength);

		//read in the data
		wxImage image(mem);
		
		//see if we were successful
		if(FALSE == image.Ok())
		{
			wxLogError(wxString::Format(wxT("Unable to open file %s from %s."), 
				strIn.c_str(), file.c_str()));
		}
		else
		{
			pList[i] = image;
		}
	}
}

void ImageEngine::IgnoreCache() const
{
	sIgnoreCache = true;
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void ImageEngine::LoadOnDemand(const wxString &filename, wxInt32 size, 
							   bool skin) const
{
	//only load the images if we need to
	if( (filename != sOpenFile) ||
		(true == sIgnoreCache))
	{
		sOpenImageArray.reset(new wxImage[size]);
		ReadImages(size, sOpenImageArray.get(), filename, skin);
		sOpenFile = filename;

		sIgnoreCache = false;
	}
}
