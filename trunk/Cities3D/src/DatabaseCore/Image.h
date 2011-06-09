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

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Namespace: Image
// 
// A namespace that provides functions for manipulating
// a wxImage.
//
// Project:
//     <DatabaseCore>
//
// Include:
//     Image.h
//
namespace Image
{
	//-----------------------------------------------------------------------//
	// Function: Colorize
	//
	// Applies a color overlay to an image.  Colorize takes the given overlay 
	// color, breaks it into its component red, green, and blue colors, and 
	// then multiplies every pixel in the source image by the color percentage
	// of the overlay color compared to white (255, 255, 255). This has the 
	// effect of making a color overlay appear over the original image.
	//
	// Parameters:
	//     image - The image to colorize.
	//     overlay - The color to colorize the image with.
	//
	// Returns:
	//     A reference to a copy of the changed image.  This
	//     allows Image actions to be chained together.
	//
	wxImage DATABASE_EXPORT_SPEC &Colorize(wxImage &image, wxColour overlay);

	//-----------------------------------------------------------------------//
	// Function: Borderize
	//
	// Outlines an image with one pixel of white.  Borderize takes the image 
	// and changes every pixel along the edges of the image to white.
	//
	// Parameters:
	//     image - The image to borderize.
	//
	// Returns:
	//     A reference to a copy of the changed image.  This
	//     allows Image actions to be chained together.
	//
	wxImage DATABASE_EXPORT_SPEC &Borderize(wxImage &image);

	//-----------------------------------------------------------------------//
	// Function: Taberize
	//
	// Stretches an image to the given width.  Taberize takes the given image, 
	// finds its center column of pixels, and then stretches the image to fill 
	// the given width, filling in all stretched pixels per row with the one 
	// pixel in the center column.  This has the effect of keeping the ends of 
	// the image the same and having a long center section of the same pixels.
	//
	// Parameters:
	//     image - The image to taberize.
	//     width - The destination width of the image.
	//
	// Returns:
	//     A reference to a copy of the changed image.  This
	//     allows Image actions to be chained together.
	//
	wxImage DATABASE_EXPORT_SPEC &Taberize(const wxImage &image, 
		const wxInt32 width);

	//-----------------------------------------------------------------------//
	// Function: Resize
	//
	// Rescales the image to the given size using bilinear filtering.
	//
	// Parameters:
	//     image - The image to resize.
	//     size - The new size of the image.
	//
	// Returns:
	//     A reference to a copy of the changed image.  This allows Image
	//     actions to be chained together.
	//
	wxImage DATABASE_EXPORT_SPEC &Resize(const wxImage &image, 
		const wxSize &size);
}

//---------------------------- PROTOTYPES       -----------------------------//

