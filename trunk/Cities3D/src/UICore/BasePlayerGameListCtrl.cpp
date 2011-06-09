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
#include "BasePlayerGameListCtrl.h"
#include "Game.h"
#include "Controller.h"
#include "GameEngine.h"
#include "DataObject.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	wxString sFields_BasePlayerGameListCtrl[] = {swPlayer};

	// The map of players to colors in the control.
	typedef std::map<ColorType, Player> ColorMap;
	ColorMap sPlayers;
}

//---------------------------- PUBLIC           -----------------------------//
#ifdef __WIN32__
	//disables the 'this' : used in base member initializer list warning
	#pragma warning(disable:4355)
#endif
wxBasePlayerGameListCtrl::wxBasePlayerGameListCtrl(wxWindow* parent, 
	wxWindowID id, const wxPoint &pos, const wxSize &size)
: wxBaseListCtrl(parent, id, sFields_BasePlayerGameListCtrl, 
	_countof(sFields_BasePlayerGameListCtrl), shPlayerListMainFont,
	shPlayerListSubFont, pos, size)
, DropSource<Player>(this)
, DropTarget<Player>(this)
, mDragIndex(-1)
{
	// Add each of the colors as a selection.
	for(size_t i = 0; i < CR_SIZE; ++i)
	{
		wxImage image = ColorInfoData::UIImage(static_cast<ColorType>(i),
			wxSize(16, 16));
		AppendItem(swEmpty, image);
	}

	Controller::get().AddReceiver(shEventPreGame, 
		&wxBasePlayerGameListCtrl::OnUpdatePlayers, this);
}

wxBasePlayerGameListCtrl::~wxBasePlayerGameListCtrl()
{
	// Cleanup.
	sPlayers.clear();

	Controller::get().RemoveReceiver(shEventPreGame, 
		&wxBasePlayerGameListCtrl::OnUpdatePlayers, this);
}

bool wxBasePlayerGameListCtrl::OnDragOver(wxCoord x, wxCoord y, const Player&)
{
	// If they are within a few pixels from the top or bottom, scroll the
	// control.
	wxSize size = GetSize();

	wxInt32 range = GetScrollRange(wxVERTICAL);
	wxInt32 pos = GetScrollPos(wxVERTICAL);

	wxInt32 vX, vY;
	GetViewStart(&vX, &vY);

	if( (4 > y) && 
		(0 < pos))
	{
		Scroll(vX, --vY);
	}
	else if( (size.y - 8 <= y) &&
			 (range - 1 > pos))
	{
		Scroll(vX, ++vY);
	}
	else
	{
		wxInt32 index = HitTest(wxPoint(x, y));
		
		// Also clears the selection if they are not over anything.
		SetSelectedItem(index);
	}

	return true;
}

void wxBasePlayerGameListCtrl::OnData(wxCoord x, wxCoord y, 
									  const Player &player)
{
	wxInt32 index = HitTest(wxPoint(x, y));
	
	if(-1 != index)
	{
		// If this is a drag happening from within the list, swap colors
		// instead of adding or removing.
		if(-1 != mDragIndex)
		{
			// Don't drop on the same person.
			if(index != mDragIndex)
			{
				// Get originator player.
				ColorType color = static_cast<ColorType>(mDragIndex);
				ColorMap::const_iterator it = sPlayers.find(color);
				if(sPlayers.end() != it)
				{
					ChangeColor(it->second, static_cast<ColorType>(index));
				}
			}
		}
		else
		{
			ColorType color = static_cast<ColorType>(index);

			// See if we have to remove an existing player from the game
			ColorMap::const_iterator it = sPlayers.find(color);
			if(sPlayers.end() != it)
			{
				RemovePlayer(it->second);
			}

			// Add the new player.
			AddPlayer(player, color);
		}
	}
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void wxBasePlayerGameListCtrl::MouseMove(wxMouseEvent &event)
{
	wxPoint pos = event.GetPosition();

	DoDrag(pos.x, pos.y);
}

void wxBasePlayerGameListCtrl::LeftDown(wxMouseEvent &event)
{
	wxInt32 index = HitTest(event.GetPosition());

	if(-1 != index)
	{
		// See if there's a player there.
		ColorType color = static_cast<ColorType>(index);
		ColorMap::const_iterator it = sPlayers.find(color);

		if(sPlayers.end() != it)
		{
			const Player &player = it->second;

			// Ensure that this player is allowed to be moved.
			DataObject input(player), output;
			RULE.Decide(shLogicUIAllowColorChange, input, output);

			if(true == output.read<bool>())
			{
				// Set the index being dragged.
				mDragIndex = index;

				// Drag and drop.
				BeginDrag(player);
			}
		}
	}
}

void wxBasePlayerGameListCtrl::LeftUp(wxMouseEvent &event)
{
	wxPoint pos = event.GetPosition();

	DoDrop(pos.x, pos.y);

	// Reset the index being dragged.
	mDragIndex = -1;
}

void wxBasePlayerGameListCtrl::LeftDoubleClick(wxMouseEvent &)
{
	wxInt32 index = GetSelectedItem();

	if(-1 != index)
	{
		// There has to be a player there to do anything.
		ColorType color = static_cast<ColorType>(index);
		ColorMap::const_iterator it = sPlayers.find(color);

		if(sPlayers.end() != it)
		{
			const Player &player = it->second;

			// Ensure that this player is allowed to be moved.
			DataObject input(player), output;
			RULE.Decide(shLogicUIAllowColorChange, input, output);

			if(true == output.read<bool>())
			{
				RemovePlayer(player);
			}

			SetSelectedItem(-1);
		}
	}
}

void wxBasePlayerGameListCtrl::OnUpdatePlayers(const GamePtr &game)
{
	sPlayers.clear();

	Lock();

	// First, clear all of the current names.
	for(size_t i = 0; i < GetItemCount(); ++i)
	{
		SetItemText(i, swPlayer, swEmpty);
	}

	// Then, set all of the players in the game.
	for(wxInt32 i = 0; i < game->numPlayers(); ++i)
	{
		const PlayerGame &playerGame = game->playerGame(i);

		// Read the player's name and color.
		ColorType color = playerGame.color();
		const Player &player = playerGame.player();

		// Set it in the map and the control.
		sPlayers[color] = player;
		SetItemText(color, swPlayer, player.Name());

		// Allow derived classes to set text colors.
		SetItemColor(playerGame, color);
	}

	// Clear any current selection.
	SetSelectedItem(-1);

	Unlock();
}
