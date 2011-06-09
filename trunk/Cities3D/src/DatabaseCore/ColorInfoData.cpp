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
#include "ColorInfoData.h"
#include "SkinManager.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	struct ColorData
	{
		ColorData() : ui(0ul), gl(0ul) {}
		ColorData(const wxString& n, const wxString& i, wxColour u, wxColour g)
			: name(n), internalName(i), ui(u), gl(g) {}

		wxColour ui;
		wxColour gl;
		wxString name;
		wxString internalName;
	};

	typedef std::pair<ColorType, ColorData> ColorPair;
	typedef std::map<ColorType, ColorData> ColorMap;
	ColorMap sColorMap;

	void checkColors()
	{
		if(0 < sColorMap.size())
		{
			return;
		}

        static const wxString stBlack = _("Black");
        static const wxString stBlue = _("Blue");
        static const wxString stBrown = _("Brown");
        static const wxString stGreen = _("Green");
        static const wxString stOrange = _("Orange");
        static const wxString stPurple = _("Purple");
        static const wxString stRed = _("Red");
        static const wxString stWhite = _("White");

		for(wxInt32 i = 0; i < CR_SIZE; i++)
		{
			ColorType type = static_cast<ColorType>(i);

			switch(type)
			{
			case CR_WHITE:
				sColorMap[type] = ColorData(stWhite, swWhite,
					wxColour(245, 245, 245), wxColour(185, 185, 185));
				break;
			case CR_RED:
				sColorMap[type] = ColorData(stRed, swRed, 
					wxColour(255, 170, 170), wxColour(223, 32, 32));
				break;
			case CR_ORANGE:
				sColorMap[type] = ColorData(stOrange, swOrange,
					wxColour(255, 210, 151), wxColour(255, 131, 6));
				break;
			case CR_BLUE:
				sColorMap[type] = ColorData(stBlue, swBlue,
					wxColour(179, 213, 255), wxColour(32, 32, 223));
				break;
			case CR_BROWN:
				sColorMap[type] = ColorData(stBrown, swBrown,
					wxColour(209, 182, 156), wxColour(100, 50, 0));
				break;
			case CR_GREEN:
				sColorMap[type] = ColorData(stGreen, swGreen,
					wxColour(170, 255, 170), wxColour(0, 128, 0));
				break;
			case CR_PURPLE:
				sColorMap[type] = ColorData(stPurple, swPurple,
					wxColour(255, 175, 255), wxColour(140, 0, 130));
				break;
			case CR_BLACK:
				sColorMap[type] = ColorData(stBlack, swBlack,
					wxColour(165, 165, 165), wxColour(40, 40, 40));
				break;
			default:
				wxASSERT(0);
				break;
			}
		}
	}
}

//---------------------------- PUBLIC           -----------------------------//
const wxString &ColorInfoData::Name(const ColorType type)
{
	wxASSERT((0 <= type) && (CR_SIZE > type));

	checkColors();

	ColorMap::const_iterator it;
	it = sColorMap.find(type);
	wxASSERT(sColorMap.end() != it);

	return it->second.name;
}

const wxString &ColorInfoData::InternalName(const ColorType type)
{
	wxASSERT((0 <= type) && (CR_SIZE > type));

	checkColors();

	ColorMap::const_iterator it;
	it = sColorMap.find(type);
	wxASSERT(sColorMap.end() != it);

	return it->second.internalName;
}

wxColour ColorInfoData::GLColor(const ColorType type)
{
	wxASSERT((0 <= type) && (CR_SIZE > type));

	checkColors();

	ColorMap::const_iterator it;
	it = sColorMap.find(type);
	wxASSERT(sColorMap.end() != it);

	return it->second.gl;
}

wxColour ColorInfoData::UIColor(const ColorType type)
{
	wxASSERT((0 <= type) && (CR_SIZE > type));

	checkColors();

	ColorMap::const_iterator it;
	it = sColorMap.find(type);
	wxASSERT(sColorMap.end() != it);

	return it->second.ui;
}

wxImage ColorInfoData::UIImage(const ColorType type, const wxSize &size)
{
	wxASSERT((0 <= type) && (CR_SIZE > type));

	checkColors();

	//now fill it with the color requested
	wxColour color = UIColor(type);
	wxColour border = GLColor(type);

	// Create a transparent border so it's not just a blob of color.
	wxColour transparent = SKIN.Element(shIconTransparent);

	// The easiest way to do this is to just create a memory bitmap and draw
	// into it.
	wxBitmap bitmap(size.x, size.y);
	wxMemoryDC dc;
	dc.SelectObject(bitmap);

	// Transparent outline.
	dc.SetBrush(wxBrush(transparent));
	dc.SetPen(wxPen(transparent));
	dc.DrawRectangle(0, 0, size.x, size.y);

	// Border.
	dc.SetBrush(wxBrush(border));
	dc.SetPen(wxPen(border));
	dc.DrawRectangle(1, 1, size.x - 2, size.y - 2);

	// Color.
	dc.SetBrush(wxBrush(color));
	dc.SetPen(wxPen(color));
	dc.DrawRectangle(2, 2, size.x - 4, size.y - 4);

	dc.SelectObject(wxNullBitmap);
	return bitmap.ConvertToImage();
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//


