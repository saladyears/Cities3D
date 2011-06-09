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
#include <wx/listctrl.h>

//---------------------------- USER INCLUDES    -----------------------------//
#include "PaneMixin.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxTexEditorPane
// 
// Pane that allows users to edit .tex files.
//
// Derived From:
//     wxPanel
//     <PaneMixin>
//
// Project:
//     <TexEditor>
//
// Include:
//     TexEditorPane.h
//
class wxTexEditorPane 
	: public wxPanel
	, public PaneMixin
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxTexEditorPane
	//
	// The wxTexEditorPane constructor.
	//
	// Parameters:
	//     parent - The parent window.
	//
	wxTexEditorPane(wxWindow *parent);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxTexEditorPane
	//
	// The wxTexEditorPane destructor.
	//
	~wxTexEditorPane();

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: ResetSelection
	//
	// Resets the current selection in the image list control.
	//
	void ResetSelection() 
	{
		mLastSelection = -1;
	}

	//-----------------------------------------------------------------------//
	// Function: SetPath
	//
	// Sets the path of the given image in the image list control.
	// 
	// Parameters:
	//     index - The index of the path to set.
	//     path - The path.
	//     newTex - Whether the image is new or not.
	//
	void SetPath(wxInt32 index, const wxString &path, bool newTex = true);

	//-----------------------------------------------------------------------//
	// Function: DeleteItem
	//
	// Deletes the given item from the image list control.
	//
	// Parameters:
	//     index - The index of the image to delete.
	//
	void DeleteItem(wxInt32 index);

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
	void OnImportButton(wxCommandEvent &event);
	void OnExportButton(wxCommandEvent &event);
	void OnExtractButton(wxCommandEvent &event);
	void OnItemSelected(wxListEvent &event);

	void importFile(const wxString &path);
	void exportFile(const wxString &path);
	void extractFile(const wxString &path);
	void readFromDisk(const wxString &path);
	void populateTree();

	void previewItem(wxInt32 index);

	wxInt32 mLastSelection;

	typedef std::pair<bool, wxString> PathPair;

	std::vector<PathPair> mPaths;
};
//---------------------------- PROTOTYPES       -----------------------------//

