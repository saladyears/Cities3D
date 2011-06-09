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
#include "BuildPopup.h"
#include "BaseStaticText.h"
#include "SkinManager.h"
#include "ImageStrip.h"
#include "GameEngine.h"
#include "IRuleEngine.h"
#include "IKeyMapEngine.h"
#include "DataObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
BuildPopup::BuildPopup(wxWindow* thisWindow)
	: BasePopup(thisWindow)
{
	// Get the order that resources appear in in the game.
	DataObject input(true), output;
	RULE.Decide(shLogicResourceOrder, input, output);

	mOrderMap = output.read<OrderMap>();
}

void BuildPopup::CreatePopup(wxWindow* window, wxSizer* sizer) const
{
	wxClientDC dc(window);
	wxFont font = SKIN.Font(shPopupFont);

	dc.SetFont(font);

	wxInt32 x, y;
	dc.GetTextExtent(mText, &x, &y);

	wxBaseStaticText* text = new wxBaseStaticText(window, -1, mText, 
		shDefaultText, shPopupFont, wxSize(x + 2, y + 2));
	sizer->Add(text, 1, wxEXPAND | wxALL, 4);

	wxString keyText;
	KEYMAP.GetKeyText(mKeyEvent, keyText);

	if(false == keyText.IsEmpty())
	{
		text = new wxBaseStaticText(window, -1, keyText, 
			shDefaultText, shPopupFont, wxSize(x + 2, y + 2), wxALIGN_CENTRE);
		sizer->Add(text, 1, wxEXPAND | wxALL, 4);
	}

	// See how much the item costs.
	DataObject input(0), output;
	RULE.Decide(mLogicCost, input, output);

	const ResourceMap& resources = output.read<ResourceMap>();
	if(false == resources.empty())
	{
		wxImageStrip *images = new wxImageStrip(window, -1, resources.size(), 10);
		images->SetFlags(wxImageStrip::FlagAlignCenter);
		images->SetPixelSpacing(3);
		
		// Go through the resources in order and add them.
		OrderMap::const_iterator it, itEnd = mOrderMap.end();
		for(it = mOrderMap.begin(); it != itEnd; ++it)
		{
			const HashString& res = it->second;

			ResourceMap::const_iterator itMap = resources.find(res);
			if(resources.end() != itMap)
			{
				wxInt32 count = itMap->second;

				if(0 < count)
				{
					images->Add(res, count);
				}
			}
		}

		wxSizeEvent event(wxSize(-1, -1));
		images->ProcessEvent(event);

		sizer->Add(images, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 4);
	}
}
//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
