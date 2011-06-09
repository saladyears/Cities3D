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
#include "NetworkPlayerGameListCtrl.h"
#include "GameEngine.h"
#include "DataObject.h"
#include "PlayerGame.h"
#include "NetworkPlayers.h"
#include "SkinManager.h"
#include "INetworkEngine.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
wxNetworkPlayerGameListCtrl::wxNetworkPlayerGameListCtrl(wxWindow *parent,
	wxWindowID id, const wxPoint &pos, const wxSize &size)
: wxBasePlayerGameListCtrl(parent, id, pos, size)
, mIsHost(false)
{
	mIsHost = NET.Players().IsHost();
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void wxNetworkPlayerGameListCtrl::ChangeColor(const Player &player, 
										  const ColorType color)
{
	// A player may only change their color if the server allows it, so clients
	// must make a request instead of just changing colors.
	HashString rule = mIsHost ? shNetworkRuleChangeColor :
		shNetworkRuleRequestChangeColor;

	wxBOOL ready = FALSE;

	// The host is always ready, but they might change the color of someone
	// else, and they should not be ready when that happens, so only set the
	// ready flag to true if the host is moving themself.
	if( (true == mIsHost) &&
		(player == NET.Players().GetThisPlayer())
		)
	{
		ready = TRUE;
	}
	
	RULE.Execute(rule, DataObject(player, color, ready));
}

void wxNetworkPlayerGameListCtrl::RemovePlayer(const Player &player)
{
	// Players may only remove themself from the game, unless they are
	// the host.
	if( (true == mIsHost) ||
		(player == NET.Players().GetThisPlayer())
		)
	{
		RULE.Execute(shNetworkRuleRemovePlayer, DataObject(player, CR_SIZE));
	}
}

void wxNetworkPlayerGameListCtrl::AddPlayer(const Player &player, 
										const ColorType color)
{
	// A player may only add themself to the game if the server allows it, so
	// clients must make a request instead of just executing the add.
	HashString rule = mIsHost ? shNetworkRuleAddPlayer :
		shNetworkRuleRequestAddPlayer;

	// Only set their ready flag to true if the player is the host, and they
	// are adding themself.
	wxBOOL ready = FALSE;

	if( (true == mIsHost) &&
		(player == NET.Players().GetThisPlayer())
		)
	{
		ready = TRUE;
	}

	RULE.Execute(rule, DataObject(player, color, ready));
}

void wxNetworkPlayerGameListCtrl::SetItemColor(const PlayerGame &playerGame, 
											   const wxInt32 index)
{
	wxBOOL ready = playerGame.read<wxInt32>(shReady);

	wxBaseListCtrl::SetItemColor(index, ready ? 
		SKIN.Element(shNetworkPlayerReady) : 
		SKIN.Element(shNetworkPlayerNotReady));
}
