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
#include "PlayerListCtrl.h"
#include "PlayerDatabase.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{	
	wxInt32 FindPlayer(const wxString &name)
	{
		//find the player in the player database and return their index
		for(wxUint32 i = 0; i < PlayerDatabase::get().NumPlayers(); ++i)
		{
			if(PlayerDatabase::get().GetPlayer(i).Name() == name)
			{
				return i;
			}
		}

		return -1;
	}

	wxString sFields_PlayerListCtrl[] = {swPlayer};
}

//---------------------------- PUBLIC           -----------------------------//
wxPlayerListCtrl::wxPlayerListCtrl(wxWindow* parent, wxWindowID id, 
	const wxPoint& pos, const wxSize& size)
: wxBaseListCtrl(parent, id, sFields_PlayerListCtrl, 
	_countof(sFields_PlayerListCtrl), shPlayerListMainFont, shPlayerListSubFont, 
	pos, size)
{
	ImportPlayers();
}

void wxPlayerListCtrl::AddPlayer(const wxString &name)
{
	//get the player's index in the database
	wxInt32 player = FindPlayer(name);
	wxASSERT(-1 != player);

	//tack 'em on
	size_t index = AppendItem(name);
	
	//set the player data in this item
	SetItemData(index, player);

	// Resort.
	Sort(swPlayer);
}

void wxPlayerListCtrl::EditPlayer(const wxString &oldName, 
								  const wxString &newName)
{
	//find the index of the player in the list with the old name
	wxInt32 index = FindItem(swPlayer, oldName);
	wxASSERT(-1 != index);

	SetItemText(index, swPlayer, newName);
}

void wxPlayerListCtrl::DeletePlayer(const size_t)
{
	// Because deleting a player may have changed the indices of everyone
	// in the PlayerDatabase, we just have to do a complete reimport.
	ImportPlayers();
}

//---------------------------- PROTECTED        -----------------------------//
void wxPlayerListCtrl::ImportPlayers()
{
	wxUint32 i;

	Lock();

	//clear existing players
	DeleteAllItems();

	//read players from the database
	for(i = 0; i < PlayerDatabase::get().NumPlayers(); ++i)
	{
		//add the player
		const Player& player = PlayerDatabase::get().GetPlayer(i);

		AppendItem(player.Name());
	}

	Sort(swPlayer);

	Unlock();
}

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxPlayerListCtrl, wxBaseListCtrl)
	EVT_ENTER_WINDOW(wxPlayerListCtrl::OnEnterWindow)
END_EVENT_TABLE()

void wxPlayerListCtrl::OnEnterWindow(wxMouseEvent &)
{
	SetCursor(wxCursor(wxCURSOR_ARROW));
}
