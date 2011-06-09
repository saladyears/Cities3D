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
#include "GLTextures.h"
#include "ImageEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	bool isCompliant(const wxImage &image)
	{
		wxInt32 w = image.GetWidth();
		wxInt32 h = image.GetHeight();

		//check to see if the width and height are even multiples of two -- 
		//this works by bit shifting the number to the right one place at a 
		//time until a 1 appears in the 1's digit.  If it is a multiple of two,
		//the number will have been bit shifted all the way to its only 1 digit
		//and should now be 1. Otherwise, it is not a multiple of two.
		for(; !(w & 1); w >>= 1);
		for(; !(h & 1); h >>= 1);
		return ((1 == w) && (1 == h));
	}

	wxImage makeGLCompliant(const wxImage &image)
	{
		wxUint32 w = image.GetWidth();
		wxUint32 h = image.GetHeight();

		//find the next largest multiple of two to scale to
		wxInt32 count;
		for(count = 0; !(w & (1 << 31)); w <<= 1, ++count);
		w = (1 << (31 - count));
		for(count = 0; !(h & (1 << 31)); h <<= 1, ++count);
		h = (1 << (31 - count));

		return image.Scale(w, h);
	}
}

//---------------------------- PUBLIC           -----------------------------//
GLuint GLTextures::BuildTexture(const wxImage &image, bool mipmap)
{
	GLuint texID = 0;

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmap ? 
		GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);

	//because GL reads textures with 0,0 being the lower-left, we have to
	//flip each image vertically
	wxImage glImage = image.Mirror(false);

	//make the image GL compliant (on 2^ size boundaries)
	if(false == isCompliant(glImage))
	{
		glImage = makeGLCompliant(glImage);
	}

	if(true == mipmap)
	{
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, glImage.GetWidth(), 
			glImage.GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, glImage.GetData());
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, glImage.GetWidth(),
					glImage.GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, 
					glImage.GetData());
	}

	return texID;
}

void GLTextures::ReadGLTextures(const wxInt32 size, GLuint *pList, 
								const wxString &filename, const bool mipmap)
{
	wxASSERT(0 <= size);

	boost::scoped_array<wxImage> images(new wxImage[size]);

	//first thing to do is read all of the images off the disk
	IMAGE.ReadImages(size, images.get(), filename);

	//sanity check
	if(false == images[0].Ok())
	{
		return;
	}

	//now convert each of these images into a GL texture
	for(wxInt32 i = 0; i < size; ++i)
	{
		//create the actual texture
		pList[i] = BuildTexture(images[i], mipmap);
	}
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//


