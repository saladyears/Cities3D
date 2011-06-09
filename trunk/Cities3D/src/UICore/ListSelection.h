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
#include "UIExport.h"
#include "OwnerDrawWindow.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class wxBaseListCtrl;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxListSelection
//
// A class that encapsulates one selection in a <wxBaseListCtrl>.
//
// Derived From:
//     <wxOwnerDrawWindow>
//
// Project:
//     <UICore>
//
// Include:
//     ListSelection.h
//
class UI_EXPORT_SPEC wxListSelection : public wxOwnerDrawWindow
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxListSelection
	//
	// The icon wxListSelection constructor.  Takes a string parameter that the
	// <SkinManager> uses to look up an icon.
	//
	// Parameters:
	//     parent - The parent window.  Must not be NULL.
	//     index - The selection index.
	//     array - The display order of the "columns" of information.
	//     data - The data that goes in the main column for this selection.
	//     subItemsPerRow - The number of sub items of text to display per row.
	//     mainFont - The font to use for the main item in the control.
	//     subFont - The font to use for all sub items in the control.
	//     icon - The icon, if any to display with this selection.
	//
	wxListSelection(wxBaseListCtrl *parent, const size_t index, 
		const StringArray &array, const wxString &data, 
		const size_t subItemsPerRow, const HashString &mainFont, 
		const HashString &subFont, const HashString &icon);

	//-----------------------------------------------------------------------//
	// Constructor: wxListSelection
	//
	// The image wxListSelection constructor.  Takes an image directly.
	//
	// Parameters:
	//     parent - The parent window.  Must not be NULL.
	//     index - The selection index.
	//     array - The display order of the "columns" of information.
	//     data - The data that goes in the main column for this selection.
	//     subItemsPerRow - The number of sub items of text to display per row.
	//     mainFont - The font to use for the main item in the control.
	//     subFont - The font to use for all sub items in the control.
	//     image - The image, if any to display with this selection.
	//
	wxListSelection(wxBaseListCtrl *parent, const size_t index, 
		const StringArray &array, const wxString &data, 
		const size_t subItemsPerRow, const HashString &mainFont, 
		const HashString &subFont, const wxImage &image);

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: SetIndex
	//
	// Sets the index of the item to the given index.
	//
	// Parameters:
	//     index - The new index of the item.
	//
	void SetIndex(const size_t index);

	//-----------------------------------------------------------------------//
	// Function: SetSelected
	//
	// Sets the item as selected or not.
	//
	// Parameters:
	//     selected - A bool that is true if the item is selected and false if
	//                not.
	//
	void SetSelected(const bool selected);

	//-----------------------------------------------------------------------//
	// Function: SetItemText
	//
	// Sets the item text for the given field.
	//
	// Parameters:
	//     field - The field.
	//     text - The item text.
	//
	void SetItemText(const wxString &field, const wxString &text);

	//-----------------------------------------------------------------------//
	// Function: GetItemText
	//
	// Gets the item text for the given field.
	//
	// Parameters:
	//     field - The field.
	//
	// Returns:
	//     A const reference to the text.
	//
	const wxString &GetItemText(const wxString &field) const;

	//-----------------------------------------------------------------------//
	// Function: SetItemColor
	//
	// Sets the color for the main item text to the given color.
	//
	// Parameters:
	//     color - The color to set the main item text to.
	//
	void SetItemColor(const wxColour color);

	//-----------------------------------------------------------------------//
	// Function: SetItemColor
	//
	// Sets the color for the main item text to the given color.
	//
	// Parameters:
	//     color - The color to set the main item text to.
	//
	wxColour GetItemColor() const;

	//-----------------------------------------------------------------------//
	// Function: OnSkin
	//
	// Handles a new skin
	//
	void OnSkin();

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
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Init
	//
	// Private initialization function both constructors use.
	//
	// Parameters:
	//     array - The display order of the "columns" of information.
	//     data - The data for the main item.
	//
	void Init(const StringArray &array, const wxString &data);

	//-----------------------------------------------------------------------//
	// Function: SetMinimumSize
	//
	// Calculates and sets the minimum size required to draw the selection.
	//
	void SetMinimumSize();

	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Paint
	//
	// Paints the window.
	//
	// Parameters:
	//     dc - The memory dc to draw into.
	//
	virtual void Paint(wxMemoryDC &dc);

	//-----------------------------------------------------------------------//
	// Group: Event Message Handling
	//

	//-----------------------------------------------------------------------//
	// Function: OnEnterWindow
	//
	// Handles the mouse entering the window's rectangle.  Sets the background
	// color to the selection color.
	//
	// Parameters:
	//     event - The mouse event.
	//
	void OnEnterWindow(wxMouseEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnLeaveWindow
	//
	// Handles the mouse leaving the window's rectangle.  Sets the background
	// color to the default color.
	//
	// Parameters:
	//     event - The mouse event.
	//
	void OnLeaveWindow(wxMouseEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnMouseMove
	//
	// Forwards mouse move events to the parent.
	//
	// Parameters:
	//     event - The mouse event.
	//
	void OnMouseMove(wxMouseEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnLeftDown
	//
	// Handles the left mouse down event.
	// 
	// Parameters:
	//     event - The mouse event.
	//
	void OnLeftDown(wxMouseEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnLeftUp
	//
	// Handles the left mouse up event.
	// 
	// Parameters:
	//     event - The mouse event.
	//
	void OnLeftUp(wxMouseEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnLeftDoubleClick
	//
	// Handles the left mouse double-click event.
	// 
	// Parameters:
	//     event - The mouse event.
	//
	void OnLeftDoubleClick(wxMouseEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnMouseWheel
	//
	// Handles the mouse wheel event.
	// 
	// Parameters:
	//     event - The mouse event.
	//
	void OnMouseWheel(wxMouseEvent &event);

    //-----------------------------------------------------------------------//
	// Function: OnKeyDown
	//
	// Handles a key event
	// 
	// Parameters:
	//     event - The mouse event.
	//
	void OnKeyDown(wxKeyEvent &event);

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Pointer: mParent
	//
	// The owner <wxBaseListCtrl>.
	//
	wxBaseListCtrl *mParent;

	struct Impl;

	//-----------------------------------------------------------------------//
	// Pointer: mpImpl
	//
	// The private implementation of the list selection.
	//
	boost::shared_ptr<Impl> mpImpl;	
};

//---------------------------- PROTOTYPES       -----------------------------//

