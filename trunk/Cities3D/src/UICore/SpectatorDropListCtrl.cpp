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
#include "SpectatorDropListCtrl.h"
#include "Controller.h"
#include "DataObject.h"
#include "Player.h"
#include "NetworkPlayers.h"
#include "ColorInfoData.h"
#include "INetworkEngine.h"
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
wxSpectatorDropListCtrl::wxSpectatorDropListCtrl(wxWindow* parent, wxWindowID id, 
										 const wxPoint& pos, 
										 const wxSize& size)
: wxSpectatorListCtrl(parent, id, pos, size)
, DropSource<Player>(this)
, DropTarget<Player>(this)
, mDragIndex(-1)
{
}


void wxSpectatorDropListCtrl::OnData(wxCoord, wxCoord, const Player &player)
{
	// Remove the player from the game.
	if(-1 == mDragIndex)
	{
		RULE.Execute(shNetworkRuleRemovePlayer, DataObject(player, CR_SIZE));
	}
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void wxSpectatorDropListCtrl::MouseMove(wxMouseEvent &event)
{
	wxPoint pos = event.GetPosition();

	DoDrag(pos.x, pos.y);
}

void wxSpectatorDropListCtrl::LeftDown(wxMouseEvent &)
{
	wxInt32 index = GetSelectedItem();

	if(-1 != index)
	{
		wxString str = GetItemText(index, swSpectator);

		// Retrieve the spectator information from the network.
		Player player = NET.Players().GetSpectators()[index].first;
		
		// Only actually drag/drop this player if the name matches the player 
		// data. If they don't match, the spectator list is out of synch and 
		// needs to be refreshed.
		if(player.Name() == str)
		{
			// They can only attempt to even move themselves (unless they are 
			// the host.
			DataObject input(player), output;
			RULE.Decide(shLogicUIAllowColorChange, input, output);

			if(true == output.read<bool>())
			{
				mDragIndex = index;

				// Do the drag and drop.
				BeginDrag(player);
			}
		}
		// Force an update if the data is out of synch.
		else
		{
			OnUpdateSpectators(0);
		}
	}
}

void wxSpectatorDropListCtrl::LeftUp(wxMouseEvent &event)
{
	wxPoint pos = event.GetPosition();

	DoDrop(pos.x, pos.y);

	mDragIndex = -1;
}

void wxSpectatorDropListCtrl::LeftDoubleClick(wxMouseEvent &)
{
	wxInt32 index = GetSelectedItem();

	if(-1 != index)
	{
		Player player = NET.Players().GetSpectators()[index].first;

		// Make sure they are allowed to join with this player.
		DataObject input(player), output;
		RULE.Decide(shLogicUIAllowColorChange, input, output);

		if(true == output.read<bool>())
		{
			bool isHost = NET.Players().IsHost();
			HashString rule = isHost ? shNetworkRuleAddPlayer : 
				shNetworkRuleRequestAddPlayer;

			// Only set their ready flag to true if the player is the host, and 
			// they are adding themself.
			wxBOOL ready = FALSE;

			if( (true == isHost) &&
				(player == NET.Players().GetThisPlayer())
				)
			{
				ready = TRUE;
			}

			RULE.Execute(rule, DataObject(player, CR_SIZE, ready));
		}
	}
}
