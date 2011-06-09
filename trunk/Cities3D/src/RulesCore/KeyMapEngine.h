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
#include "RuleSetDatabase.h"
#include "RulesExport.h"
#include "IKeyMapEngine.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
bool operator<(const wxKeyEvent& lhs, const wxKeyEvent& rhs);

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: KeyMapEngine
// 
// TODO: Fill this in.
//
// Derived From:
//     <IKeyMapEngine>
//
// Project:
//     <RulesCore>
//
// Include:
//     KeyMapEngine.h
//
class RULES_EXPORT_SPEC KeyMapEngine : public IKeyMapEngine
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//
    void Start();

	virtual void ProcessKeyEvent(wxKeyEvent& event) const;

	virtual void GetEventMap(EventMap& eventMap) const;

    virtual void OverrideKeyEvent(const wxKeyEvent& event, const wxString& name,
        wxString& collision);

    virtual void IgnoreNextEvent();

	virtual void GetKeyText(const wxKeyEvent& event, wxString& text) const;

	virtual void GetKeyText(const HashString& keyEvent, wxString& text) const;

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//	
	// Group: Functions
	//
	void ProcessRuleSet(const RuleSetPtr& ruleset);

    void SaveOverrides();
    void LoadOverrides();

	//-----------------------------------------------------------------------//
	// Section: Variables
	//
	typedef std::map<wxKeyEvent, KeyMap> KeyMapMap;
	KeyMapMap mKeyMap;

	EventMap mEventMap;

    typedef std::map<wxString, wxKeyEvent> OverrideMap;
    OverrideMap mOverrideMap;

	friend class PluginEngine;
};

//---------------------------- PROTOTYPES       -----------------------------//

