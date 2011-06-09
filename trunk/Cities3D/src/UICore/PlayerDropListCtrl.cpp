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
#include "PlayerDropListCtrl.h"
#include "Controller.h"
#include "DataObject.h"
#include "Game.h"
#include "GameEngine.h"
#include "PlayerDatabase.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
#ifdef __WIN32__
	//disables the 'this' : used in base member initializer list warning
	#pragma warning(disable:4355)
#endif
wxPlayerDropListCtrl::wxPlayerDropListCtrl(wxWindow* parent, wxWindowID id, 
										   const wxPoint& pos, 
										   const wxSize& size)
: wxPlayerListCtrl(parent, id, pos, size)
, DropSource<Player>(this)
, DropTarget<Player>(this)
, mDragIndex(-1)
{
	Controller::get().AddReceiver(shEventPreGame, 
		&wxPlayerDropListCtrl::OnUpdatePlayers, this);
}

wxPlayerDropListCtrl::~wxPlayerDropListCtrl()
{
	Controller::get().RemoveReceiver(shEventPreGame, 
		&wxPlayerDropListCtrl::OnUpdatePlayers, this);
}

void wxPlayerDropListCtrl::OnData(wxCoord, wxCoord, const Player &player)
{
	// Remove the player from the game.
	if(-1 == mDragIndex)
	{
		RULE.Execute(shRuleUIRemovePlayer, DataObject(player, CR_SIZE));
	}
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void wxPlayerDropListCtrl::MouseMove(wxMouseEvent &event)
{
	wxPoint pos = event.GetPosition();

	DoDrag(pos.x, pos.y);
}

void wxPlayerDropListCtrl::LeftDown(wxMouseEvent &)
{
	// Since the base instantiation already determins if something got 
	// selected, just use the current selected item, if it exists.
	wxInt32 index = GetSelectedItem();

	if(-1 != index)
	{
		//get the associated player data
		wxInt32 playerIndex = GetItemData(index);
		Player player(PlayerDatabase::get().GetPlayer(playerIndex));

		mDragIndex = index;

		// Do the drag and drop.
		BeginDrag(player);
	}
}

void wxPlayerDropListCtrl::LeftUp(wxMouseEvent &event)
{
	wxPoint pos = event.GetPosition();

	DoDrop(pos.x, pos.y);

	mDragIndex = -1;
}

void wxPlayerDropListCtrl::LeftDoubleClick(wxMouseEvent &)
{
	wxInt32 index = GetSelectedItem();
	
	if(-1 != index)
	{
		wxInt32 playerIndex;
		playerIndex = GetItemData(index);

		RULE.Execute(shRuleUIAddPlayer, 
			DataObject(PlayerDatabase::get().GetPlayer(playerIndex), CR_SIZE));
	}
}

void wxPlayerDropListCtrl::OnUpdatePlayers(const GamePtr &game)
{
	wxASSERT(game);

	//reget all players from the player database
	ImportPlayers();

	Lock();

	//now remove all that are currently in the game
	for(wxInt32 i = 0; i < game->numPlayers(); ++i)
	{
		for(size_t j = 0; j < GetItemCount(); ++j)
		{
			wxString name = GetItemText(j, swPlayer);

			if(name == game->player(i).Name())
			{
				DeleteItem(j);
				break;
			}
		}
	}

	Unlock();
}
