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
#include "NetworkPane.h"
#include "NetworkConnectPage.h"
#include "NetworkGamePage.h"
#include "Controller.h"
#include "DataObject.h"
#include "Game.h"
#include "GameEngine.h"
#include "INetworkEngine.h"
#include "IRuleEngine.h"
#include "NetworkPlayers.h"
#include "DataObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	const wxSize sMinPaneSize = wxSize(500, 500);
}

//---------------------------- PUBLIC           -----------------------------//
wxNetworkPane::wxNetworkPane(wxWindow *parent)
: wxAuiNotebook(parent, wxID_ANY, 
	wxPoint(parent->GetClientSize().x, parent->GetClientSize().y), 
	sMinPaneSize, wxAUI_NB_SCROLL_BUTTONS | wxAUI_NB_TOP)
, PaneMixin(this)
{
	// Add the initial start page to the notebook.
	wxNetworkConnectPage* page = new wxNetworkConnectPage(this);
	AddPage(page, stConnection);
	page->Layout();

	Controller::get().AddReceiver(shEventGameConnect, 
		&wxNetworkPane::OnGameConnect, this);
	Controller::get().AddReceiver(shEventPrepareNetworkGame,
		&wxNetworkPane::OnPrepareNetworkGame, this);
}

wxNetworkPane::~wxNetworkPane()
{
	Controller::get().RemoveReceiver(shEventGameConnect, 
		&wxNetworkPane::OnGameConnect, this);
	Controller::get().RemoveReceiver(shEventPrepareNetworkGame,
		&wxNetworkPane::OnPrepareNetworkGame, this);
}

void wxNetworkPane::ClosePane()
{
	// Make sure the network engine is stopped.
	NET.Stop();
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void wxNetworkPane::OnGameConnect(const DataObject& object)
{
	bool connect = object.read<bool>();

	if(true == connect)
	{
		bool add = true;

		// See if we need to add the new page.
		if(2 <= object.numItems())
		{
			const GamePtr& game = object.read<GamePtr>(1);
			add = (shNew == game->read<HashString>(shState));
		}

		if(true == add)
		{
			wxNetworkGamePage* page = new wxNetworkGamePage(this);
			InsertPage(1, page, stGameSetup, true);
			page->Layout();
		}
	}
	else
	{
		DeletePage(1);
	}
}

void wxNetworkPane::OnPrepareNetworkGame(bool own)
{
	if(true == NET.IsConnected())
	{
		// Joining a game in progress.
		if(false == own)
		{
			// Get things going.
			RULE.AsynchExecute(shNetworkRuleJoinGame, DataObject());

			// Immediately check to see if this player takes any of the seats
			// in the game.
			RULE.Execute(shNetworkRuleTakeSeat, 
				DataObject(NET.Players().GetThisPlayer()));
		}
		// Joining a game at the start of the game.
		else
		{			
			// Tell the RuleEngine this is from the network so we don't send it out
			// to everyone, since every player is executing this code at the 
			// same time.
			RULE.AsynchExecute(shNetworkRuleStartGame, DataObject());
		}
	}
}
