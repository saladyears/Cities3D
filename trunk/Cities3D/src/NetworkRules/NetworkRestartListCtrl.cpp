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
#include "NetworkRestartListCtrl.h"
#include "Controller.h"
#include "ColorInfoData.h"
#include "Game.h"
#include "SkinManager.h"
#include "DataObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	wxString sFields_RestartList[] = {swPlayer, swOriginalPlayer};
}

//---------------------------- PUBLIC           -----------------------------//
wxNetworkRestartListCtrl::wxNetworkRestartListCtrl(wxWindow* parent, 
	wxWindowID id, const wxPoint& pos, const wxSize& size)
: wxBaseListCtrl(parent, id, sFields_RestartList, 
	_countof(sFields_RestartList), shNetworkRestartListMainFont, 
	shNetworkRestartListSubFont, pos, size)
{
	Controller::get().AddReceiver(shEventNetworkRestartPlayers, 
		&wxNetworkRestartListCtrl::OnUpdatePlayers, this);
	Controller::get().AddReceiver(shEventCountdownTime,
		&wxNetworkRestartListCtrl::OnCountdownTime, this);
}

wxNetworkRestartListCtrl::~wxNetworkRestartListCtrl()
{
	Controller::get().RemoveReceiver(shEventNetworkRestartPlayers, 
		&wxNetworkRestartListCtrl::OnUpdatePlayers, this);
	Controller::get().RemoveReceiver(shEventCountdownTime,
		&wxNetworkRestartListCtrl::OnCountdownTime, this);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void wxNetworkRestartListCtrl::OnUpdatePlayers(const GamePtr &game)
{
	Lock();

	DeleteAllItems();

	// Then, set all of the players in the game.
	for(wxInt32 i = 0; i < game->numPlayers(); ++i)
	{
		const PlayerGame &playerGame = game->playerGame(i);

		// Read the player's name and color.
		ColorType color = playerGame.color();
		const Player &player = playerGame.player();

		// Create the image used to represent them.
		wxImage image = ColorInfoData::UIImage(color, wxSize(32, 32));

		// Set it in the map and the control.
		wxString name;
		if(NullPlayer == player)
		{
			name = stWaiting;
		}
		else
		{
			name = player.Name();
		}
		size_t index = AppendItem(name, image);

		// Set their color based on their presence or absence.
		SetItemColor(index, SKIN.Element(NullPlayer == player ? 
			shNetworkRestartAbsent : shNetworkRestartPresent));

		// Get the sorted original list.
		PlayerGame::TurnPlayerMap originals;
		playerGame.originalPlayers(originals);
		
		// Now add them in string form.
		wxString str;
		PlayerGame::TurnPlayerMap::const_iterator itMap, 
			itMapEnd = originals.end();
		for(itMap = originals.begin(); itMap != itMapEnd; ++itMap)
		{
			if(originals.begin() != itMap)
			{
				str += swCommaSpace;
			}

			str += wxString::Format(swStringInt.c_str(), 
				itMap->second.Name().c_str(), 
				itMap->first);
		}

		SetItemText(index, swOriginalPlayer, str);
	}

	Unlock();
}

void wxNetworkRestartListCtrl::OnCountdownTime(const DataObject &object)
{
	wxASSERT(2 <= object.numItems());

	const wxInt32 index = object.read<wxInt32>();
	const wxTimeSpan &time = object.read<wxTimeSpan>(1);

	// Update the player.
	wxString str = stWaiting + time.Format(swMinutesSeconds.c_str());

	SetItemText(index, swPlayer, str);
}
