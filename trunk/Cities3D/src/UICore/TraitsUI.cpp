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
#include "TraitsUI.h"
#include "SkinManager.h"
#include "Utility.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
void TraitsUI::Resize(const float percent)
{
	Reset();

	//multiply everything in the hash map by the percent
	IntHash::iterator it, itEnd(mSizes.end());
	for(it = mSizes.begin(); it != itEnd; ++it)
	{
		it->second = static_cast<wxInt32>(it->second * percent);
	}
}

wxInt32 TraitsUI::size(const HashString &key)
{
	//first, check to see if there's a constant value with this key
	if(mConstants.end() != mConstants.find(key))
	{
		return mConstants[key];
	}

	//then look in the resizable array
	if(mSizes.end() != mSizes.find(key))
	{
		return mSizes[key];
	}

	//uh-oh, looking up something that has never been set
	wxLogError(wxT("Programmer Error: Invalid TraitsUI size: %s"), 
		key.cwx_str());

	return 0;
}

const wxColour TraitsUI::color(const HashString &key)
{
	//look in the color hash
	if(mColours.end() == mColours.find(key))
	{
		wxLogError(wxT("Programmer Error: Invalid TraitsUI color: %s"), 
			key.cwx_str());
	}

	return SKIN.Element(mColours[key]);
}

const wxFont TraitsUI::calculateFontSize(wxDC &dc, const HashString &font, 
								 const HashString &height, const wxInt32 num)
{
	wxFont sizedFont = SKIN.Font(mFonts[font]);

	//assume 1 pixel of buffer each way
	wxInt32 expanse = mSizes[height] - 2;

	//sanity
	if(0 < expanse)
	{
		Utility::calculateFontSize(dc, sizedFont, expanse, num);
	}

	return sizedFont;
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
TraitsUI::TraitsUI()
{
	//set up some default universal sizes
	mSizes[shWidth] = 0;
	mSizes[shHeight] = 0;

	//set up some default universal colours
	mColours[shBackground] = shBackground;
	mColours[shOutline] = shGameUIOutline;
	mColours[shClickable] = shGameUIClickable;
	mColours[shFontColor] = shGameUIText;

	//there should always be a default font
	mFonts[shFont] = shGameUIFont;
}
