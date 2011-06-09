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
#include "GLExport.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Namespace: GLTextures
// 
// A namespace that provides functions for creating OpenGL textures.
//
// Project:
//     GLCore
//
// Include:
//     GLTextures.h
//
namespace GLTextures
{
	//-----------------------------------------------------------------------//
	// Function: BuildTexture
	//
	// Creates an OpenGL texture from a wxImage object.
	// 
	// Parameters:
	//     image - A wxImage to convert to an OpenGL texture.
	//     mipmap - Whether the OpenGL texture should be created as a set of 
	//              mipmaps.
	//
	// Returns:
	//     A GLuint that is the OpenGL texture ID of the created texture.
	//
	GLuint GL_EXPORT_SPEC BuildTexture(const wxImage &image, 
		bool mipmap = false);

	//-----------------------------------------------------------------------//
	// Function: ReadGLTextures
	//
	// Reads a set of images from a .tex file and uses them to create OpenGL 
	// textures.  Calls <BuildTexture> for each image read from the file.
	// 
	// Parameters:
	//     size - The number of images to be read from the file.
	//     pList - The array of GLuint texture values where the newly created 
	//             OpenGL texture ids will be stored.
	//     filename - The path to the file used to load images.
    //     mipmap - Whether the OpenGL textures should be created as sets of 
	//              mipmaps.
	//
	void GL_EXPORT_SPEC ReadGLTextures(const wxInt32 size, GLuint *pList, 
		const wxString &filename, const bool mipmap = false);
}

//---------------------------- PROTOTYPES       -----------------------------//
