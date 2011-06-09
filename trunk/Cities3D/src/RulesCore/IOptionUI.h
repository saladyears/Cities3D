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
class wxXmlNode;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: IOptionUI
// 
// The interface for option UI items.  Options may provide their own UI for
// the <wxOptionsDialog> window, so that users may configure any settings
// the option provides.
//
// Project:
//     <RulesCore>
//
// Include:
//     IOptionUI.h
//
class RULES_EXPORT_SPEC IOptionUI
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	typedef std::pair<wxString, wxString> StringPair;
	typedef std::vector<StringPair> StringPairArray;

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~IOptionUI
	// 
	// The IOptionUI destructor.  It is pure virtual because
	// this class must be derived from in order to be used.
	// 
	virtual ~IOptionUI()=0;

	//-----------------------------------------------------------------------//
	// Group: Pure Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: dependencies
	//
	// Fills the given array with all other <RuleSets> this option depends on.
	//
	// Parameters:
	//     array - [out] The array of dependencies to fill.
	//
	virtual void dependencies(StringPairArray& array) const=0;

	//-----------------------------------------------------------------------//
	// Function: silent_dependencies
	//
	// Fills the given array with all other <RuleSets> this option depends on,
	// and which must be silently loaded.  Note that this is the inverse of
	// the dependencies function.  In the dependencies function, the option
	// may not be selected if any dependencies are not also selected.  However,
	// in the silent_dependencies function, if the option is selected, all
	// silent dependencies will be loaded (silently, of course).  Therefore, it
	// is imperative that options are careful with this list.  You do not want
	// to start loading alternate plugins unless absolutely required.
	//
	// Parameters:
	//     array - [out] The array of dependencies to fill.
	//
	virtual void silent_dependencies(StringPairArray& array) const=0;

	//-----------------------------------------------------------------------//
	// Function: exclusions
	//
	// Fills the given array with all other <RuleSets> excluded by this option.
	//
	// Parameters:
	//     array - [out] The array of exclusions to fill.
	//
	virtual void exclusions(StringPairArray& array) const=0;

	//-----------------------------------------------------------------------//
	// Function: is_silent
	//
	// Returns whether the option is a silent option, i.e., it does not appear
	// in the option dialog, but gets loaded automatically if its dependencies
	// are met.
	//
	// Returns:
	//     A bool that is true if the option is silent, and false if not.
	//
	virtual bool is_silent() const
	{
		return false;
	}

	//-----------------------------------------------------------------------//
	// Function: description
	//
	// Returns the textual description of the option.
	//
	// Returns:
	//     The textual description of the option.
	//
	virtual const wxString& description() const=0;

	//-----------------------------------------------------------------------//
	// Function: priority
	//
	// Returns the order priority of this option within the module.
	//
	// Returns:
	//     The order priority of this option within the module.
	//
	virtual wxInt32 priority() const=0;

	//-----------------------------------------------------------------------//
	// Function: short_description
	//
	// Returns the short textual description of the option.
	//
	// Returns:
	//     The short textual description of the option.
	//
	virtual wxString short_description() const=0;

	//-----------------------------------------------------------------------//
	// Function: HasUI
	//
	// Returns whether the option has UI associated with it.
	//
	// Returns:
	//     A bool that is true if the option has UI, and false if not.
	//
	virtual bool HasUI() const=0;

	//-----------------------------------------------------------------------//
	// Function: CreateUI
	//
	// Creates the UI associated with the option.
	//
	// Parameters:
	//     parent - The parent window.
	//     sizer - The sizer to add the UI to.
	//
	virtual void CreateUI(wxWindow*, wxSizer*)
	{
		wxASSERT(false == HasUI());
	}

	//-----------------------------------------------------------------------//
	// Function: DestroyUI
	//
	// Cleans up the UI associated with the option.
	//
	// Parameters:
	//     parent - The parent window.
	//     sizer - The sizer to remove the UI from.
	//
	virtual void DestroyUI(wxWindow*, wxSizer*)
	{
		wxASSERT(false == HasUI());
	}

	//-----------------------------------------------------------------------//
	// Function: UpdateUI
	//
	// Updates the UI with the given value.
	//
	// Parameters:
	//     key - The value to be updated.
	//     value - The new value.
	//
	virtual void UpdateUI(const wxString&, const wxString&)
	{
		wxASSERT(false == HasUI());
	}

	//-----------------------------------------------------------------------//
	// Function: SaveToXML
	//
	// Saves any UI specific values to XML.
	//
	// Parameters:
	//     node - The parent wxXmlNode.
	//
	virtual void SaveToXML(wxXmlNode*)
	{
		wxASSERT(false == HasUI());
	}

	//-----------------------------------------------------------------------//
	// Function: LoadFromXML
	//
	// Loads any UI specific values from XML.
	//
	// Parameters:
	//     option - The option.
	//     node - The parent wxXmlNode.
	//
	virtual void LoadFromXML(const wxString&, wxXmlNode*)
	{
		wxASSERT(false == HasUI());
	}
};

//---------------------------- PROTOTYPES       -----------------------------//
inline IOptionUI::~IOptionUI() {}
