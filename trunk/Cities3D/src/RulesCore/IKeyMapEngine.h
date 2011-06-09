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
#include "RulesExport.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class wxKeyEvent;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: IKeyMapEngine
//
// TODO: Fill this in.
//
// Project:
//     <RulesCore>
//
// Include:
//     IKeyMapEngine.h
//
class RULES_EXPORT_SPEC IKeyMapEngine
{
public:
	struct KeyMap
	{
		KeyMap(const wxString& d, const wxString& n, const HashString& e,
			const wxKeyEvent& def)
			: description(d)
			, name(n)
			, event(e)
			, defaultEvent(def)
		{}
		
		KeyMap() {}

		wxString description;
		wxString name;
		HashString event;
		wxKeyEvent defaultEvent;
	};

	typedef std::vector<KeyMap> KeyMapArray;
	typedef std::map<wxString, KeyMap> EventMap;

	virtual ~IKeyMapEngine()=0;

	virtual void ProcessKeyEvent(wxKeyEvent& event) const=0;

	virtual void GetEventMap(EventMap& eventMap) const=0;

    virtual void OverrideKeyEvent(const wxKeyEvent& event, const wxString& name,
        wxString& collision)=0;

    virtual void IgnoreNextEvent()=0;

	virtual void GetKeyText(const wxKeyEvent& event, wxString& text) const=0;

	virtual void GetKeyText(const HashString& keyEvent, wxString& text) const=0;
};

//---------------------------- PROTOTYPES       -----------------------------//
inline IKeyMapEngine::~IKeyMapEngine() {}
