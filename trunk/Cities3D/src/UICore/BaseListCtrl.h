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

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class wxListSelection;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxBaseListCtrl
// 
// A basic list control.  A list control displays items in a list
// with selection and sorting.  
//
// Derived From:
//     wxScrolledWindow
//
// Project:
//     <UICore>
//
// Include:
//     BaseListCtrl.h
//
class UI_EXPORT_SPEC wxBaseListCtrl : public wxScrolledWindow
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxBaseListCtrl
	//
	// The wxBaseListCtrl constructor.  Creates the list control with the
	// given position, size, etc.
	//
	// Parameters:
	//     parent - The parent window.  Must not be NULL.
	//     id - The message handling ID.  Should be a unique (to the parent 
	//          window) ID, if the parent window wants to receive messages from
	//          the control.
	//     array - The list of "columns" in the control and the order they 
	//             should be displayed in.  The first column is the most
	//             prominent one.
	//     size - The number of columns in the array.
	//     mainFont - The font to use for the main item in each control.
	//     subFont - The font to use for any sub items in each control.
	//     pos - The list control position in window coordinates.
	//     size - The list control size.
	//     subItemsPerRow - The number of subitems per row to put in each item.
	//     style - The window style.
	//
	wxBaseListCtrl(wxWindow *parent, wxWindowID id,
		const wxString array[],	const size_t num_items,
		const HashString &mainFont,
		const HashString &subFont,
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize,
		const wxInt32 subItemsPerRow = 1,
		const wxInt32 style = wxSTATIC_BORDER | wxHSCROLL | wxVSCROLL);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//
	
	//-----------------------------------------------------------------------//
	// Destructor: ~wxBaseListCtrl
	//
	// The wxBaseListCtrl destructor.
	//
	~wxBaseListCtrl();

	//-----------------------------------------------------------------------//
	// Group: Functions
	//
	
	//-----------------------------------------------------------------------//
	// Function: Lock
	//
	// Locks the control so no size updates will occur when text or new items
	// are appended.
	//
	void Lock();

	//-----------------------------------------------------------------------//
	// Function: Unlock
	//
	// Unlocks the control so size updates will occur.
	//
	void Unlock();

	//-----------------------------------------------------------------------//
	// Function: AppendItem
	//
	// Appends a new <wxListSelection> item to the list, with the given string
	// as its main column of information, and an option icon image string.
	//
	// Parameters:
	//     str - The main text for the new item.
	//     image - The icon to use with the item.
	//
	// Returns:
	//     An unsigned integer that is the index in the list of the new item.
	//
	const size_t AppendItem(const wxString &str, const HashString &image = shEmpty);

	//-----------------------------------------------------------------------//
	// Function: AppendItem
	//
	// Appends a new <wxListSelection> item to the list, with the given string
	// as its main column of information, and an image for the selection to
	// use directly.
	//
	// Parameters:
	//     str - The main text for the new item.
	//     image - The image to use with the item.
	//
	// Returns:
	//     An unsigned integer that is the index in the list of the new item.
	//
	const size_t AppendItem(const wxString &str, const wxImage &image);

	//-----------------------------------------------------------------------//
	// Function: DeleteItem
	//
	// Deletes the item at the given index from the list.
	//
	// Parameters:
	//     index - The index of the item to delete.
	//
	void DeleteItem(const size_t index);

	//-----------------------------------------------------------------------//
	// Function: DeleteAllItems
	//
	// Removes all items from the control.
	//
	void DeleteAllItems();

	//-----------------------------------------------------------------------//
	// Function: GetItemCount
	//
	// Returns the number of items in the control.
	//
	// Returns:
	//     The number of items in the control.
	//
	const size_t GetItemCount() const {return mSelections.size();}

	//-----------------------------------------------------------------------//
	// Function: SetItemText
	//
	// Sets the item text for the given field for the item at the given index.
	//
	// Parameters:
	//     index - The index of the item to have the text set.
	//     field - The field.
	//     text - The item text.
	//
	void SetItemText(const size_t index, const wxString &field, 
		const wxString &text);

	//-----------------------------------------------------------------------//
	// Function: GetItemText
	//
	// Returns the item text for the given field for the item at the given 
	// index.
	//
	// Parameters:
	//     index - The index of the item to have the text get.
	//     field - The field.
	//
	// Returns:
	//     The text string.
	//
	const wxString GetItemText(const size_t index, 
		const wxString &field) const;

	//-----------------------------------------------------------------------//
	// Function: FindItem
	//
	// Returns the index of the first item in the control that has the given
	// text at the given field.
	//
	// Parameters:
	//     field - The field to search.
	//     text - The item text to search for.
	//
	// Returns:
	//     The index of the item in the control, or -1 if it could not be 
	//     found.
	//
	const wxInt32 FindItem(const wxString &field, const wxString &text) const;

	//-----------------------------------------------------------------------//
	// Function: HitTest
	//
	// Determines the index of the item at the given point, if any.
	//
	// Parameters:
	//     point - The point to test.
	//
	// Returns:
	//     The index of the item at the point, or -1 if none.
	//
	const wxInt32 HitTest(const wxPoint &point) const;

	//-----------------------------------------------------------------------//
	// Function: Sort
	//
	// Sorts the items in the control by the given field.
	//
	// Parameters:
	//     field - The field to sort the items by.
	//
	void Sort(const wxString &field);

	//-----------------------------------------------------------------------//
	// Function: SetSelectedItem
	//
	// Sets the item at the given index to selected.  If the index is -1, the
	// current selection, if any, is cleared.
	//
	// Parameters:
	//     index - The index of the item to be selected.
	//
	void SetSelectedItem(const wxInt32 index);

	//-----------------------------------------------------------------------//
	// Function: GetSelectedItem
	//
	// Returns the selected item index, if any.
	//
	// Returns:
	//     An integer that is the index of the selected item, or -1 if no item
	//     is selected.
	//
	const wxInt32 GetSelectedItem() const {return mSelectedItem;}

	//-----------------------------------------------------------------------//
	// Function: SetItemData
	//
	// Sets a user-defined piece of data for the item at the given index.
	//
	// Parameters:
	//     index - The index of the item.
	//     data - The data to set.
	//
	void SetItemData(const size_t index, const wxInt32 data);

	//-----------------------------------------------------------------------//
	// Function: GetItemData
	//
	// Returns the user-defined piece of data of the item at the given index.
	//
	// Parameters:
	//     index - The index of the item.
	//
	// Returns:
	//     The data at the given index.
	//
	const wxInt32 GetItemData(const size_t index) const;

	//-----------------------------------------------------------------------//
	// Function: SetItemColor
	//
	// Sets the color of the main text at the given index.
	//
	// Parameters:
	//     index - The index of the item.
	//     color - The new main text color.
	//
	void SetItemColor(const size_t index, const wxColour color);

	//-----------------------------------------------------------------------//
	// Function: GetItemColor
	//
	// Gets the color of the main text at the given index.
	//
	// Parameters:
	//     index - The index of the item.
	//
	// Returns:
	//     The color of the item at the given index.
	//
	wxColour GetItemColor(const size_t index) const;

	//-----------------------------------------------------------------------//
	// Function: GetDisplayOrder
	//
	// Returns the display order of the "columns" in the control.
	//
	// Returns:
	//     A const reference to the array of columns in the control.
	//
	const StringArray &GetDisplayOrder() const {return mDisplayOrder;}

	//-----------------------------------------------------------------------//
	// Function: GetListBestSize
	//
	// Returns the size for the list that will display all children.
	//
	// Returns:
	//     The size that will display all children.
	//
	const wxSize &GetListBestSize() const {return mBestSize;}

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
	// Group: Virtual Functions
	//
	
	//-----------------------------------------------------------------------//
	// Function: MouseMove
	//
	// Virtual function called when an OnMouseMove event is handled.  The 
	// default implementation does nothing.
	//
	virtual void MouseMove(wxMouseEvent &) {}

	//-----------------------------------------------------------------------//
	// Function: LeftDown
	//
	// Virtual function called when an OnLeftDown event is handled.  The 
	// default implementation does nothing.
	//
	virtual void LeftDown(wxMouseEvent &) {}

	//-----------------------------------------------------------------------//
	// Function: LeftUp
	//
	// Virtual function called when an OnLeftUp event is handled.  The 
	// default implementation does nothing.
	//
	virtual void LeftUp(wxMouseEvent &) {}

	//-----------------------------------------------------------------------//
	// Function: LeftDoubleClick
	//
	// Virtual function called when an OnLeftDoubleClick event is handled.  The 
	// default implementation does nothing.
	//
	virtual void LeftDoubleClick(wxMouseEvent &) {}

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: AppendItem
	//
	// Appends the item to the list.
	//
	// Parameters:
	//     index - The index of the item.
	//     selection - The <wxListSelection> to append.
	//
	void AppendItem(const size_t index, wxListSelection * const selection);

	//-----------------------------------------------------------------------//
	// Function: CalculateVirtualSize
	//
	// Determines the required virtual size based on all of the child windows.
	//
	void CalculateVirtualSize();

	//-----------------------------------------------------------------------//
	// Function: ReorderSelections
	//
	// Reassigns the correct indices to the children windows when the order
	// change (from a sort or delete, for example).
	//
	void ReorderSelections();

	//-----------------------------------------------------------------------//
	// Group: Game Event Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnSkin
	//
	// Handles a new skin.
	//
	void OnSkin(const wxInt32 = 0);

	//-----------------------------------------------------------------------//
	// Group: Event Message Handling
	//

	//-----------------------------------------------------------------------//
	// Function: OnSize
	//
	// Handles a window resize.
	//
	void OnSize(wxSizeEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnMouseMove
	//
	// Handles the mouse move event.
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
	// Handles the mouse wheel moving.  Scrolls the list if possible.
	// 
	// Parameters:
	//     event - The mouse event.
	//
	void OnMouseWheel(wxMouseEvent &event);

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Bool: mLocked
	//
	// A flag that is true if the control should not calculate sizes and true
	// if it should.
	//
	bool mLocked;

	//-----------------------------------------------------------------------//
	// Int: mSelectedItem
	//
	// The index of the currently selected item.
	//
	wxInt32 mSelectedItem;

	//-----------------------------------------------------------------------//
	// Int: mSubItemsPerRow
	//
	// The number of sub items per row to display in the items.
	//
	wxInt32 mSubItemsPerRow;

	//-----------------------------------------------------------------------//
	// String: mMainFont
	//
	// The font to use for the main item in each control.
	//
	HashString mMainFont;

	//-----------------------------------------------------------------------//
	// String: mSubFont
	//
	// The font to use for all sub items in each control.
	//
	HashString mSubFont;

	//-----------------------------------------------------------------------//
	// Object: mBestSize
	//
	// The best size calculated for this control that will show all selections.
	//
	wxSize mBestSize;

	//-----------------------------------------------------------------------//
	// Array: mDisplayOrder
	//
	// The "columns" in the control and the order in which they should be
	// displayed.  The first column is the main one.
	//
	StringArray mDisplayOrder;

	//-----------------------------------------------------------------------//
	// Array: mSelectionArray
	//
	// The array of selections in the control.
	//
	typedef std::pair<wxInt32, wxListSelection *> SelectionPair;
	typedef std::vector<SelectionPair> SelectionArray;
	SelectionArray mSelections;
};

//---------------------------- PROTOTYPES       -----------------------------//
