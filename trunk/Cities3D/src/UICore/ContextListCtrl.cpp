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
#include "ContextListCtrl.h"
#include "Controller.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	wxString sFields_ContextListCtrl[] = {swMenu};
}

//---------------------------- PUBLIC           -----------------------------//
wxContextListCtrl::wxContextListCtrl(wxWindow *parent)
: wxBaseListCtrl(parent, -1, sFields_ContextListCtrl, 
	_countof(sFields_ContextListCtrl), swContextMenuFont, swContextMenuFont, 
	wxDefaultPosition, wxDefaultSize, 1, 0)
{
}

void wxContextListCtrl::AddMenuItem(const wxString &text, const wxInt32 id, 
	const HashString &image)
{
	wxInt32 index = AppendItem(text, image);
	SetItemData(index, id);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void wxContextListCtrl::LeftDown(wxMouseEvent &)
{
	wxInt32 index = GetSelectedItem();

	if(-1 != index)
	{
		wxInt32 id = GetItemData(index);

		// Since we're a menu, we just want clicks, not selections.
		SetSelectedItem(-1);

		Controller::get().Transmit(shEventContextMenuClick, id);
		
		wxCommandEvent command(wxEVT_COMMAND_BUTTON_CLICKED, id);
		ProcessEvent(command);
	}
}
