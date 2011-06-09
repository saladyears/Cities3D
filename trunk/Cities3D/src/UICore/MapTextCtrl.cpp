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
#include "MapTextCtrl.h"
#include "Controller.h"
#include "Game.h"
#include "GameEngine.h"
#include "MapDatabase.h"
#include "SkinManager.h"
#include "HTML.h"
#include "HtmlWindowEx.h"
#include "IOptionEngine.h"
#include "IPluginEngine.h"
#include "Map.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	wxInt32 sFontSize = 0;
	wxColour sTextColor;
	
	wxString& Fontify(wxString& text, wxColour color, wxInt32 size, 
		bool b = false, bool u = false,	bool i = false)
	{
		if(true == b)
		{
			HTML::Bold(text);
		}

		if(true == u)
		{
			HTML::Underline(text);
		}

		if(true == i)
		{
			HTML::Italics(text);
		}

		const wxFont& font = SKIN.Font(shDefaultFont);
	
		wxString str = swHTMLFontFace;
		str += font.GetFaceName();
		str += swHTMLSize;
		str += wxString::Format(swIntFormat.c_str(), size);
		str += wxString::Format(swHTMLColor.c_str(), 
			HTML::ConvertColorToHex(color).c_str());
		str += swHTMLClose;
		str += text;
		str += swHTMLFontClose;

		text = str;

		return text;
	}
}

//---------------------------- PUBLIC           -----------------------------//
wxMapTextCtrl::wxMapTextCtrl(wxWindow* parent, wxWindowID id, 
	const wxPoint& pos, const wxSize& size)
: wxHtmlWindowEx(parent, id, pos, size, wxSUNKEN_BORDER)
{
	AddText(swHTMLBody);

	sFontSize = SKIN.Size(shMapText);
	sTextColor = SKIN.Element(shMapText);
	
    static const wxString stNoMapSelected = _("No map selected.");
	
	wxString str = stNoMapSelected;
	Fontify(str, sTextColor, sFontSize, true);
	
	AddText(str);
	
	Controller::get().AddReceiver(shEventPreGame, 
		&wxMapTextCtrl::OnUpdateText, this);
}

wxMapTextCtrl::~wxMapTextCtrl()
{
	Controller::get().RemoveReceiver(shEventPreGame, 
		&wxMapTextCtrl::OnUpdateText, this);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void wxMapTextCtrl::OnUpdateText(const GamePtr &)
{
	wxInt32 index = MAPINDEX;

	if(-1 != index)
	{
		//update the map text info
		StringArray pieces;
		MapDatabase::get().MapInfoString(index, pieces);

        static const wxString stMap = _("Map");
	
		wxString str = swHTMLBody;
		wxString map = stMap;
		str += Fontify(map, sTextColor, sFontSize + 1, true, true);
		HTML::Break(str);

		StringArray::const_iterator it, itEnd = pieces.end();
		for(it = pieces.begin(); it != itEnd; ++it)
		{
			wxString text = (*it);
			text = swDash + text;
			str += Fontify(text, sTextColor, sFontSize, true);
			HTML::Break(str);
		}

		// Add rulesets.
		HTML::Break(str);
		wxString rules = _("Rules");
		str += Fontify(rules, sTextColor, sFontSize + 1, true, true);
		HTML::Break(str);

		const Map::StringPairArray& rulesets = MAP.rulesets();
		Map::StringPairArray::const_iterator itMap, itMapEnd = rulesets.end();
		for(itMap = rulesets.begin(); itMap != itMapEnd; ++itMap)
		{
			const wxString& module = itMap->first;
			const wxString& ruleset = itMap->second;

			wxString name = PLUGIN.GetRulesetName(module, ruleset);

			if(false == name.IsEmpty())
			{
				name = swDash + name;
				str += Fontify(name, sTextColor, sFontSize, true);
				HTML::Break(str);
			}
		}

		// Add any options as well.
		pieces.clear();
		OPTION.GetOptionStrings(pieces);

		if(false == pieces.empty())
		{
			HTML::Break(str);
			wxString options = _("Options");
			str += Fontify(options, sTextColor, sFontSize + 1, true, true);

			itEnd = pieces.end();
			for(it = pieces.begin(); it != itEnd; ++it)
			{
				wxString text = (*it);

				if(false == text.IsEmpty())
				{
					str += swHTMLBreak;
					text = swDash + text;
					str += Fontify(text, sTextColor, sFontSize, true);
				}
			}
		}

		SetPage(str);
	}
}


