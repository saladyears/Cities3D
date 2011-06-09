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
#ifndef __WIN32__
#include <time.h>
#endif

//---------------------------- USER INCLUDES    -----------------------------//
#include "NetworkServer.h"
#include "DataObject.h"
#include "GameEngine.h"
#include "NetworkUtility.h"
#include "ServerThread.h"
#include "ServerGameThread.h"
#include "RuleEvent.h"
#include "NetworkPlayers.h"
#include "Controller.h"
#include "INetworkEngine.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef std::vector<Player> PlayerArray;

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
NetworkServer::NetworkServer(const wxIPV4address &addr, const Player &player)
: mServerThread(NULL)
, mServerGameThread(NULL)
{
	// Store off this player so we always know who the host is.
	NET.Players().SetThisPlayer(player);
	NET.Players().SetHostPlayer(player);

	SpectatorArray spectators;
	spectators.push_back(std::make_pair(player, false));
	NET.Players().SetSpectators(spectators);

	// Also add a flag to the game for tracking if the host quit.
	gameData<wxInt32>(shHostQuit) = FALSE;

	// Start up the game thread first.
	mServerGameThread = new wxServerGameThread(this, player);
	if(wxTHREAD_NO_ERROR != mServerGameThread->Create())
	{
		wxLogError(wxT("Unable to create a game thread."));
	}
	else
	{
		// Start up the server thread.
		mServerThread = new wxServerThread(addr.Service(), this, 
			mServerGameThread);
		if(wxTHREAD_NO_ERROR != mServerThread->Create())
		{
			wxLogError(wxT("Unable to create a server thread."));
		}
		else
		{
			mServerThread->Run();
			mServerGameThread->Run();
		}
	}

	Controller::get().AddReceiver(shEventCloseGameWindow,
		&NetworkServer::OnCloseGameWindow, this);
	Controller::get().AddReceiver(shEventClose,
		&NetworkServer::OnCloseGameWindow, this);
}

NetworkServer::~NetworkServer()
{
	if(NULL != mServerGameThread)
	{
		mServerGameThread->Delete();
	}

	if(NULL != mServerThread)
	{
		mServerThread->Delete();
	}

	Controller::get().RemoveReceiver(shEventCloseGameWindow,
		&NetworkServer::OnCloseGameWindow, this);
	Controller::get().RemoveReceiver(shEventClose,
		&NetworkServer::OnCloseGameWindow, this);
}

bool NetworkServer::IsConnected() const
{
	bool ret = false;

	if(NULL != mServerGameThread)
	{
		ret = mServerGameThread->HasSockets();
	}

	return ret;
}

void NetworkServer::SendRule(const HashString &rule, 
							 const DataObject &object) const
{
	// In the special case of a system message, just add it to the normal
	// message handling without sending it to everyone.
	if(shNetworkRuleSystemMessage == rule)
	{
		wxRuleEvent event(rule, object, true);
		const_cast<NetworkServer *>(this)->AddPendingEvent(event);
	}
	else
	{
		// Write the rule out to memory first and pass it to the game thread.
		size_t size = 0;
		boost::shared_array<wxUint8> data;

		bool valid = true;

		try
		{
			NetworkUtility::WriteRuleToMemory(rule, object, data, size);
		}
		catch(const std::exception &e)
		{
			valid = false;

			wxLogDebug(HashString(e.what()).wx_str());
		}

		if( (true == valid) &&
			(NULL != mServerGameThread))
		{
			mServerGameThread->SendRule(data, size);
		}
	}
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(NetworkServer, wxEvtHandler)
	EVT_RULE(0, NetworkServer::OnRule)
END_EVENT_TABLE()

void NetworkServer::OnCloseGameWindow(wxInt32)
{
	// Shut down the server threads before we get a final game close.
	if(NULL != mServerThread)
	{
		mServerThread->Delete();
		mServerThread = NULL;
	}
	if(NULL != mServerGameThread)
	{
		mServerGameThread->Delete();
		mServerGameThread = NULL;
	}
}

void NetworkServer::OnRule(wxRuleEvent &event)
{
	const HashString &rule = event.Rule();
	const DataObject &data = event.Data();

	if(true == event.Asynch())
	{
		wxLogDebug(wxT("Server executing asynch rule %s."), rule.cwx_str());

		// In the special case of chat or system messages, timestamp things.
		if(shNetworkRuleSystemMessage == rule)
		{
			DataObject &thisObject = const_cast<DataObject &>(data);
			thisObject = DataObject(data.read<wxString>(), wxDateTime::Now());
		}
		else if(shNetworkRuleChatMessage == rule)
		{
			DataObject &thisObject = const_cast<DataObject &>(data);
			thisObject = DataObject(data.read<Player>(), 
				data.read<wxString>(1), wxDateTime::Now());
		}

		RULE.AsynchExecute(rule, data);
	}
	else
	{
		wxLogDebug(wxT("Server executing rule %s."), rule.cwx_str());
		RULE.Execute(rule, data);
	}
}
