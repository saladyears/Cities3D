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
#include "Image.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	struct pixel
	{
		pixel(wxUint8 r_, wxUint8 g_, wxUint8 b_) : r(r_), b(b_), g(g_) {}

		wxUint8 r;
		wxUint8 g;
		wxUint8 b;

		inline pixel &operator+=(const pixel &p)
		{
			this->r += p.r;
			this->g += p.g;
			this->b += p.b;

			return *this;
		}
	};

	inline pixel operator*(float mult, const pixel &p)
	{
		return pixel(
			static_cast<wxUint8>(p.r * mult),
			static_cast<wxUint8>(p.g * mult),
			static_cast<wxUint8>(p.b * mult));
	}

	inline pixel *GetPixel(pixel *start, size_t x, size_t y, size_t width, 
		size_t height)
	{
		pixel *ret = NULL;

		// The pixel has to be within the given dimensions.
		if( (width > x) && (height > y) )
		{
			ret = start + (y * width) + x;
		}

		return ret;
	}

	wxImage sImage;
}

//---------------------------- PUBLIC           -----------------------------//
wxImage &Image::Colorize(wxImage &image, wxColour overlay)
{
	wxInt32 r, g, b;
	r = overlay.Red();
	g = overlay.Green();
	b = overlay.Blue();

	wxInt32 size;
	size = image.GetHeight() * image.GetWidth() * 3;

	wxUint8 *p, *data;
	data = image.GetData();

	for(p = data; p < data + size; p += 3)
	{
		//set new values
		p[0] = int(((float) p[0] / 255.0) * (float) r);
		p[1] = int(((float) p[1] / 255.0) * (float) g);
		p[2] = int(((float) p[2] / 255.0) * (float) b);
	}

	sImage = image.Copy();
	return sImage;
}

wxImage &Image::Borderize(wxImage &image)
{
	wxInt32 w, h;
	w = image.GetWidth() * 3;
	h = image.GetHeight();

	wxInt32 size;
	size = w * h;

	wxUint8 *p, *data;
	data = image.GetData();

	bool nextRow = false;
	for(p = data; p < data + size; p += 3)
	{
		bool lastCol;
		lastCol = (((p - data) % w) == (w - 3));

		//border must be in the first row, last row, or one of the edges
		if( (p < data + w) ||
			(p > (data + size - w)) ||
			(true == lastCol) ||
			(true == nextRow))
		{
			//set new values
			p[0] = 255;
			p[1] = 255;
			p[2] = 255;

			nextRow = lastCol;
		}
	}

	sImage = image.Copy();
	return sImage;
}

wxImage &Image::Taberize(const wxImage &image, const wxInt32 width)
{
	wxInt32 w, h;
	w = image.GetWidth();
	h = image.GetHeight();

	//get original image data
	wxUint8 *p, *data;
	data = image.GetData();

	//create a new image with the target dimensions
	wxImage destImage(width, h);

	//get target image data
	wxUint8 *t, *target;
	target = destImage.GetData();

	wxInt32 size;
	size = width * h * 3;

	//set the pivot point
	wxInt32 pivot;
	pivot = w >> 1;

	wxInt32 count = 0;
	wxInt32 extra = (width - w) + pivot;

	//traverse the new data, setting bits
	for(t = target, p = data; t < target + size; t += 3)
	{
		//if we're inside the pivot area, fill with the pivot pixel
		if( (count >= pivot) &&
			(count < extra))
		{
			t[0] = t[-3];
			t[1] = t[-2];
			t[2] = t[-1];
		}
		//otherwise, fill with the original source
		else
		{
			t[0] = p[0];
			t[1] = p[1];
			t[2] = p[2];
			p += 3;
		}
		
		//increment row count
		++count;

		//see if we are back to a new row
		if(count == width)
		{
			count = 0;
		}
	}

	sImage = destImage.Copy();
	return sImage;
}

wxImage &Image::Resize(const wxImage &image, const wxSize &size)
{
	// If the source image has a mask, just use the default implementation.
	if( (true == image.HasMask()) ||
		(true == image.HasAlpha()))
	{
		sImage = image.Scale(size.x, size.y);
		return sImage;
	}

	wxImage destImage(size.x, size.y);

	wxInt32 srcX = image.GetWidth();
	wxInt32 srcY = image.GetHeight();
	wxInt32 destX = size.x;
	wxInt32 destY = size.y;

	float stepX = static_cast<float>(srcX) / static_cast<float>(destX);
	float stepY = static_cast<float>(srcY) / static_cast<float>(destY);
	float startX = stepX / 2.0f;
	float startY = stepY / 2.0f;

	pixel *src = reinterpret_cast<pixel *>(image.GetData());
	pixel *dest = reinterpret_cast<pixel *>(destImage.GetData());
	pixel *end = dest + (destX * destY);

	float TL, TR;
	float BL, BR;
	float yFactor;
	float xPos, yPos;

	pixel *pTL, *pTR;
	pixel *pBL, *pBR;

	wxInt32 x = 0, y = 0;
	wxInt32 pixelX, pixelY;
	float roundX, roundY;

	while(dest < end)
	{
		// Determine where in the source texels we are.
		xPos = startX + (stepX * x);
		yPos = startY + (stepY * y);

		pixelX = static_cast<wxInt32>(xPos);
		pixelY = static_cast<wxInt32>(yPos);

		roundX = xPos - pixelX;
		roundY = yPos - pixelY;

		TR = TL = BR = BL = 0.0f;

		// Set pixels.
		if(0.5f < roundX)
		{
			if(0.5f < roundY)
			{
				// Source pixel is upper left quadrant.
				pTL = GetPixel(src, pixelX, pixelY, srcX, srcY);
				pTR = GetPixel(src, pixelX + 1, pixelY, srcX, srcY);
				pBL = GetPixel(src, pixelX, pixelY + 1, srcX, srcY);
				pBR = GetPixel(src, pixelX + 1, pixelY + 1, srcX, srcY);

				// Weighting factors.
				TR = BR = (roundX - 0.5f);
				TL = BL = 1.0f - TR;

				yFactor = roundY - 0.5f;
				TL *= (1.0f - yFactor);
				TR *= (1.0f - yFactor);
				BL *= yFactor;
				BR *= yFactor;
			}
			else
			{
				// Source pixel is lower left quadrant.
				pTL = GetPixel(src, pixelX, pixelY - 1, srcX, srcY);
				pTR = GetPixel(src, pixelX + 1, pixelY - 1, srcX, srcY);
				pBL = GetPixel(src, pixelX, pixelY, srcX, srcY);
				pBR = GetPixel(src, pixelX + 1, pixelY, srcX, srcY);

				// Weighting factors.
				TR = BR = (roundX - 0.5f);
				TL = BL = 1.0f - TR;
				
				yFactor = 0.5f - roundY;
				TL *= yFactor;
				TR *= yFactor;
				BL *= (1.0f - yFactor);
				BR *= (1.0f - yFactor);
			}
		}
		else
		{
			if(0.5f < roundY)
			{
				// Source pixel is upper right quadrant.
				pTL = GetPixel(src, pixelX - 1, pixelY, srcX, srcY);
				pTR = GetPixel(src, pixelX, pixelY, srcX, srcY);
				pBL = GetPixel(src, pixelX - 1, pixelY + 1, srcX, srcY);
				pBR = GetPixel(src, pixelX, pixelY + 1, srcX, srcY);

				// Weighting factors.
				TL = BL = 0.5f - roundX;
				TR = BR = (1.0f - TL);
				
				yFactor = roundY - 0.5f;
				TL *= (1.0f - yFactor);
				TR *= (1.0f - yFactor);
				BL *= yFactor;
				BR *= yFactor;
			}
			else
			{
				// Source pixel is bottom right quadrant.
				pTL = GetPixel(src, pixelX - 1, pixelY - 1, srcX, srcY);
				pTR = GetPixel(src, pixelX, pixelY - 1, srcX, srcY);
				pBL = GetPixel(src, pixelX - 1, pixelY, srcX, srcY);
				pBR = GetPixel(src, pixelX, pixelY, srcX, srcY);

				// Weighting factors.
				TL = BL = 0.5f - roundX;
				TR = BR = (1.0f - TL);

				yFactor = 0.5f - roundY;
				TL *= yFactor;
				TR *= yFactor;
				BL *= (1.0f - yFactor);
				BR *= (1.0f - yFactor);
			}
		}

		// Perform the weighted calculation.
		*dest = pixel(0, 0, 0);
		if(NULL != pTL)
		{
			*dest += (TL * *pTL);
		}
		if(NULL != pTR)
		{
			*dest += (TR * *pTR);
		}
		if(NULL != pBL)
		{
			*dest += (BL * *pBL);
		}
		if(NULL != pBR)
		{
			*dest += (BR * *pBR);
		}

		// Increment pixel locations.
		++x;

		if(x >= destX)
		{
			x = 0;
			++y;
		}

		++dest;
	}

	sImage = destImage.Copy();
	return sImage;
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//



