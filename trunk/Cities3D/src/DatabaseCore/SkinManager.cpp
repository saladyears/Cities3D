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
#include "SkinManager.h"
#include "FilePath.h"
#include "ImageSet.h"
#include "ImageEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	WX_DECLARE_STRING_HASH_MAP(wxColour, ColorHash);
	ColorHash sColorHash;
}

//---------------------------- PUBLIC           -----------------------------//
void SkinManager::LoadXML()
{
	// Clear existing hashes.
	mFontHash.clear();
	mElementHash.clear();

	// Get the path to the skin XML file.
	const wxString path = FilePath::Skin(swCities3DXML);

	// Load it.
	wxFileInputStream x(path);
	
	// Parse it.
	if(true == x.Ok())
	{
		wxXmlDocument xmlDoc(x);

		if(true == xmlDoc.IsOk())
		{
			wxXmlNode *root = xmlDoc.GetRoot();

			if(NULL != root)
			{
				for(wxXmlNode *node = root->GetChildren(); NULL != node; 
					node = node->GetNext())
				{
					wxString name = node->GetName();

					// Fonts
					if(swFonts == name)
					{
						ReadFonts(node);	
					}
					// Colors
					else if(swColors == name)
					{
						ReadColors(node);
					}
					// Elements
					else if(swElements == name)
					{
						ReadElements(node);
					}
					// Sizes
					else if(swSizes == name)
					{
						ReadSizes(node);
					}
				}
			}
		}
		else
		{
			wxLogError(wxString::Format(wxT("SkinManager could not parse %s.  ")
				wxT("Expect no UI."), path.c_str()));
		}
	}
	else
	{
		wxLogError(wxString::Format(wxT("SkinManager could not open %s.")
			wxT("Expect no UI."), path.c_str()));
	}
}

const wxFont &SkinManager::Font(const HashString &key) const
{
	FontHash::iterator it = mFontHash.find(key);

	if(mFontHash.end() != it)
	{
		return it->second;
	}

	wxLogDebug(wxString::Format(wxT("Unable to find font %s in SkinManager"), 
		key.cwx_str()));

	return mFontHash[shDefaultFont];
}

const wxColour SkinManager::Element(const HashString &key) const
{
	ColorHash::const_iterator it = mElementHash.find(key);

	if(mElementHash.end() != it)
	{
		return it->second;
	}

	wxLogDebug(wxString::Format(wxT("Unable to find color %s in SkinManager"), 
		key.cwx_str()));

	// If it's not there, just return white.
	return wxColour(255, 255, 255);
}

const wxInt32 SkinManager::Size(const HashString &key) const
{
	SizeHash::const_iterator it = mSizeHash.find(key);

	if(mSizeHash.end() != it)
	{
		return it->second;
	}

	wxLogDebug(wxString::Format(wxT("Unable to find size %s in SkinManager"), 
		key.cwx_str()));

	// If it's not there, just return zero.
	return 0;
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
SkinManager::SkinManager()
{
	LoadXML();
}

void SkinManager::ReadFonts(wxXmlNode * const parent)
{
	long point;

	// Grab the default font information.
	wxString face = parent->GetPropVal(swFace, swTahoma);
	parent->GetPropVal(swPoint, sw8).ToLong(&point);
	bool bold = (swTrue == parent->GetPropVal(swBold, swFalse));
	bool italic = 
		(swTrue == parent->GetPropVal(swItalics, swFalse));
	bool underline =
		(swTrue == parent->GetPropVal(swUnderline, swFalse));
	
	// Set the default font.
	wxFont defaultFont(point, wxFONTFAMILY_DEFAULT, italic ?
		wxFONTSTYLE_ITALIC : wxFONTSTYLE_NORMAL, bold ?
		wxFONTWEIGHT_BOLD : wxFONTWEIGHT_NORMAL, underline,
		face);
	mFontHash[shDefaultFont] = defaultFont;

	wxFont thisFont;

	for(wxXmlNode *font = parent->GetChildren(); NULL != font;
		font = font->GetNext())
	{
		// Skip comments.
		if(wxXML_COMMENT_NODE == font->GetType())
		{
			continue;
		}

		wxString val;
		thisFont = defaultFont;

		wxString name;
		if(true == font->GetPropVal(swName, &name))
		{
			//Check each of the possible properties for a change.
			if(true == font->GetPropVal(swFace, &val))
			{
				thisFont.SetFaceName(val);
			}
			if(true == font->GetPropVal(swPoint, &val))
			{
				long point;
				val.ToLong(&point);
				thisFont.SetPointSize(point);
			}
			if(true == font->GetPropVal(swBold, &val))
			{
				thisFont.SetWeight((swTrue == val) ? wxFONTWEIGHT_BOLD : 
					wxFONTWEIGHT_NORMAL);
			}
			if(true == font->GetPropVal(swItalics, &val))
			{
				thisFont.SetStyle((swTrue == val) ? wxFONTSTYLE_ITALIC : 
					wxFONTSTYLE_NORMAL);
			}
			if(true == font->GetPropVal(swUnderline, &val))
			{
				thisFont.SetUnderlined(swTrue == val);
			}

			if(false == name.IsEmpty())
			{
				mFontHash[HashString(name)] = thisFont;
			}
		}
		else
		{
			wxLogError(wxT("Cities3D.xml has a font with a missing name."));
		}
	}
}

void SkinManager::ReadColors(wxXmlNode * const parent)
{
	// In this case, all we need to do is just read children in.
	for(wxXmlNode *color = parent->GetChildren(); NULL != color;
		color = color->GetNext())
	{
		// Skip comments.
		if(wxXML_COMMENT_NODE == color->GetType())
		{
			continue;
		}

		wxString name, red, green, blue;

		if( (true == color->GetPropVal(swName, &name)) &&
			(true == color->GetPropVal(swR, &red)) &&
			(true == color->GetPropVal(swG, &green)) &&
			(true == color->GetPropVal(swB, &blue)) &&
			(false == name.IsEmpty())
			)
		{
			long r, g, b;
			red.ToLong(&r);
			green.ToLong(&g);
			blue.ToLong(&b);

			sColorHash[name] = wxColour(r, g, b);
		}
	}
}

void SkinManager::ReadElements(wxXmlNode * const parent)
{
	for(wxXmlNode *element = parent->GetChildren(); NULL != element;
		element = element->GetNext())
	{
		// Skip comments.
		if(wxXML_COMMENT_NODE == element->GetType())
		{
			continue;
		}

		wxString name, red, green, blue, color;

		if( (true == element->GetPropVal(swName, &name)) &&
			(false == name.IsEmpty()))
		{
			if( (true == element->GetPropVal(swR, &red)) &&
				(true == element->GetPropVal(swG, &green)) &&
				(true == element->GetPropVal(swB, &blue))
				)
			{
				long r, g, b;
				red.ToLong(&r);
				green.ToLong(&g);
				blue.ToLong(&b);

				mElementHash[HashString(name)] = wxColour(r, g, b);
			}
			else if(true == element->GetPropVal(swColor, &color))
			{
				mElementHash[HashString(name)] = sColorHash[color];
			}
		}
	}

	// Clear out the color has as we don't need it anymore.
	sColorHash.clear();
}

void SkinManager::ReadSizes(wxXmlNode * const parent)
{
	// Just read in sizes
	for(wxXmlNode *size = parent->GetChildren(); NULL != size;
		size = size->GetNext())
	{
		// Skip comments.
		if(wxXML_COMMENT_NODE == size->GetType())
		{
			continue;
		}

		wxString name, sizeVal;

		if( (true == size->GetPropVal(swName, &name)) &&
			(true == size->GetPropVal(swSize, &sizeVal)) &&
			(false == name.IsEmpty())
			)
		{
			long val;
			sizeVal.ToLong(&val);

			// Clamp.
			val = std::max(val, 0l);
			
			mSizeHash[HashString(name)] = val;
		}
	}
}
