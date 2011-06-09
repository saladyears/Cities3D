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
#include "BaseModalDialog.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class wxTreeEvent;
class DataObject;

class IOptionUI;
typedef boost::shared_ptr<IOptionUI> OptionUIPtr;

class Game;
typedef boost::shared_ptr<Game> GamePtr;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxOptionsDialog
// 
// Dialog that allows users to choose game options.
//
// Derived From:
//     <wxBaseModalDialog>
//
// Project:
//     <Cities3D>
//
// Include:
//     OptionsDialog.h
//
class UI_EXPORT_SPEC wxOptionsDialog : public wxBaseModalDialog
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxOptionsDialog
	//
	// The wxOptionsDialog constructor.
	//
	// Parameters:
	//     parent - The parent window.
	//
	wxOptionsDialog(wxWindow *parent);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxOptionsDialog
	//
	// The wxOptionsDialog destructor.
	//
	~wxOptionsDialog();

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Macro: DECLARE_EVENT_TABLE
	//
	// The wxWidgets event table declaration.
	//
	DECLARE_EVENT_TABLE()

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:

	virtual void Ok();

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: UpdateTreeColors
	//
	// Updates the colors of items in the tree.
	//
	void UpdateTreeColors();

	void UpdateSettings(const wxString& module, const wxString& option, bool selected);

	void OnPreGame(const GamePtr& game);

	//-----------------------------------------------------------------------//
	// Group: Game Event Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnUpdateOption
	//
	// Updates the current option.
	//
	// Parameters:
	//     object - The information to update.
	//
	void OnUpdateOption(const DataObject& object);

	//-----------------------------------------------------------------------//
	// Group: Event Message Handling
	//

	//-----------------------------------------------------------------------//
	// Function: OnTreeSelection
	//
	// Updates the currently displayed option based on the tree node selected 
	// by the user.
	//
	// Parameters:
	//     event - The tree selection event.
	//
	void OnTreeSelection(wxTreeEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnTreeActivate
	//
	// Handles a double-click in the tree, which selects or unselects the 
	// current item, if possible.
	//
	// Parameters:
	//     event - The tree selection event.
	//
	void OnTreeActivate(wxTreeEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnSelectButton
	//
	// Handles the user selecting an option.
	//
	void OnSelectButton(wxCommandEvent& );

	//-----------------------------------------------------------------------//
	// Function: OnUnselectButton
	//
	// Handles the user unselecting an option.
	//
	void OnUnselectButton(wxCommandEvent& );

	void OnClearButton(wxCommandEvent&);

	void OnSaveButton(wxCommandEvent&);

	void OnLoadButton(wxCommandEvent&);

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Pointer: mOptionUI
	//
	// The currently loaded option.
	//
	OptionUIPtr mOptionUI;
};

//---------------------------- PROTOTYPES       -----------------------------//

