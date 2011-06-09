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


#include "NetworkClient.h"
#include "NetworkEngine.h"
#include "Game.h"
#include "GameEngine.h"
#include "DataObject.h"
#include "Controller.h"
#include "RNG.h"
#include "NetworkPlayers.h"
#include "NetworkUtility.h"
#include "RuleEvent.h"
#include "IGameObjectEngine.h"
#include "INetworkEngine.h"
#include "IStatEngine.h"
#include "IOptionEngine.h"
#include "IPluginEngine.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


typedef std::vector<Player> PlayerArray;

//---------------------------- STATICS          -----------------------------//
namespace
{
	enum
	{
		ID_SOCKET		= 0,
	};

	struct no_delete_network
	{
		void operator()(void *){}
	};
}

//---------------------------- PUBLIC           -----------------------------//
NetworkClient::NetworkClient(const wxIPV4address &addr, const Player &player)
: mpSocketClient(NULL)
{
	// Store off this player so we know who is at this machine.
	NET.Players().SetThisPlayer(player);

	mpSocketClient = new wxSocketClient(wxSOCKET_REUSEADDR_FLAG);

	// I don't like this very much, but I can't think of any other way to block
	// the UI in a modal dialog during connection.  The dialog that responds to
	// this event must call the Connect function with the given address.
	Controller::get().Transmit(shEventNetworkConnectionAttempt, 
		DataObject(addr, static_cast<INetworkClient*>(this)));
}

NetworkClient::~NetworkClient()
{
	//close it down if necessary
	if(mpSocketClient)
	{
		mpSocketClient->Destroy();
		mpSocketClient = NULL;
	}
}

bool NetworkClient::Connect(const wxIPV4address &addr)
{
	bool success = true;
	wxUint32 magic, number, compare;

    static const wxString stConnected = _("Connected!");

	//for some insane reason, wx has no const methods on wxIPV4address, so it
	//is necessary to cast away the constness of the passed-in address; it is 
	//more important to maintain a const-correct interface than to make one 
	//const_cast cast
	wxIPV4address real_addr = const_cast<wxIPV4address &>(addr);

	if(NULL == mpSocketClient)
	{
		mpSocketClient = new wxSocketClient;
	}

	//now, try and connect
	mpSocketClient->Connect(real_addr, false);
	mpSocketClient->WaitOnConnect(10);

	// Prepare an input stream for the future.
	wxSocketInputStream sock(*mpSocketClient);
	wxDataInputStream input(sock);

	if(false == mpSocketClient->IsConnected())
	{
		wxString name = real_addr.Hostname();
		if(true == name.IsEmpty())
		{
			name = real_addr.IPAddress();
		}

        static const wxString stCouldNotEstablishAConnection = _("Could not establish a connection to %s:%d.");
		wxString str = wxString::Format(stCouldNotEstablishAConnection.c_str(), 
			name.c_str(), real_addr.Service());

		Controller::get().Transmit(shEventNetworkConnectionEvent, str);

		success = false;
		goto error;
	}

	wxLogDebug(wxString::Format(
		wxT("Connection established to %s:%d"), real_addr.Hostname().c_str(), 
		real_addr.Service()));

	// Handshake required by the Cities3D server.
	magic = wxUINT32_SWAP_ON_LE('CC3D');
	mpSocketClient->Write(&magic, sizeof(wxUint32));

	number = 0;
	mpSocketClient->Read(&number, sizeof(wxUint32));

	// Make sure we are talking to a Cities3D server.
	compare = wxUINT32_SWAP_ON_LE('CS3D');
	if(number != compare)
	{
        static const wxString stNotACities3DServer = _("It was not a Cities3D server on the other end!");
		Controller::get().Transmit(shEventNetworkConnectionEvent, 
			stNotACities3DServer);
		success = false;
		goto error;
	}
	
	// Run a version check.
	if(false == VersionCheck(mpSocketClient))
	{
		success = false;
		goto error;
	}

	// Let the server know who we are.
	SendSpectator(NET.Players().GetThisPlayer());

	// Get all the other information.
	ReceiveSpectators(input);
	ReceiveGame(input);
	ReceiveRNG(input);

	// See if we made it.
	wxUint8 check;
	mpSocketClient->ReadMsg(&check, 1);

	// If we failed the player check, stop.
	if(0 == check)
	{
        static const wxString stYouMayNotJoinTwice = _("You may not join the same game twice.");
    
		// Notify the player.
		Controller::get().Transmit(shEventNetworkConnectionEvent, 
			stYouMayNotJoinTwice);

		success = false;
		goto error;
	}

	mpSocketClient->SetEventHandler(*this, ID_SOCKET);
	mpSocketClient->SetNotify(wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
	mpSocketClient->SetFlags(wxSOCKET_WAITALL | wxSOCKET_REUSEADDR_FLAG);
	mpSocketClient->Notify(true);

    Controller::get().Transmit(shEventNetworkConnectionEvent, 
		stConnected);

	//now that we're successfully connected, tell the UI we can get the 
	//game going
	Controller::get().Transmit(shEventGameConnect, DataObject(true, GetGame()));

	// Fire off the notification event.
	Controller::get().Transmit(shEventPreGame, GetGame());

	// Update the UI.  This has to be here because the UI for spectators isn't
	// in place until after the game connect event.
	Controller::get().Transmit(shEventSpectator, 0);

	// Possibly the only acceptable use of a goto is the occasional catch-all
	// error handling.
error:
	if( (false == success) &&
		(NULL != mpSocketClient))
	{
		mpSocketClient->Destroy();
		mpSocketClient = NULL;
	}

	return success;
}

bool NetworkClient::IsConnected() const
{
	bool connected = false;

	if(NULL != mpSocketClient)
	{
		connected = mpSocketClient->IsConnected();
	}

	return connected;
}

void NetworkClient::SendRule(const HashString &rule, 
							 const DataObject &object) const
{
	// System messages are a special case because they need to be
	// timestamped.
	if(shNetworkRuleSystemMessage == rule)
	{
		wxRuleEvent event(rule, object, true);
		const_cast<NetworkClient *>(this)->AddPendingEvent(event);
	}
	else
	{
		wxASSERT(NULL != mpSocketClient);

		try
		{
			size_t size = 0;
			boost::shared_array<wxUint8> data;

			// Write the rule to memory.
			NetworkUtility::WriteRuleToMemory(rule, object, data, size);

			mpSocketClient->Write(&size, 4);

			if( (true == mpSocketClient->Error()) ||
				(4 != mpSocketClient->LastCount()))
			{
				throw std::runtime_error("Client failed to send 4 bytes.");
			}

			// Send it over the wire.
			mpSocketClient->Write(data.get(), size);

			if( (true == mpSocketClient->Error()) ||
				(size != mpSocketClient->LastCount()))
			{
				HashString str = HashString::Format("Client failed to send"
					" %d bytes.", size);
				throw std::runtime_error(str.c_str());
			}
		}
		catch(const std::exception &e)
		{
			wxLogDebug(HashString(e.what()).wx_str());
		}
	}
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(NetworkClient, wxEvtHandler)
	EVT_SOCKET(ID_SOCKET, NetworkClient::OnSocketEvent)
	EVT_RULE(0, NetworkClient::OnRule)
END_EVENT_TABLE()

void NetworkClient::OnSocketEvent(wxSocketEvent &event)
{
	try
	{
		switch(event.GetSocketEvent())
		{
		case wxSOCKET_INPUT:
			{
			wxLogDebug(wxT("Client started wxSOCKET_INPUT event."));

			//disable the input event again, so that we don't get interrupted
			//while reading
			mpSocketClient->SetNotify(wxSOCKET_LOST_FLAG);

			SocketInput();

			//reenable input events
			mpSocketClient->SetNotify(wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
			wxLogDebug(wxT("Client ended wxSOCKET_INPUT event."));

			break;
			}
		case wxSOCKET_LOST:
			{
			wxLogDebug(wxT("Client received wxSOCKET_LOST event."));

			SocketLost();
			}

			break;
		default:
			break;
		}
	}
	catch(const std::exception &e)
	{
		wxLogDebug(HashString(e.what()).wx_str());
	}
}

void NetworkClient::OnRule(wxRuleEvent &event)
{
	const HashString &rule = event.Rule();
	const DataObject &data = event.Data();

	if(true == event.Asynch())
	{
		// In the special case of a system message, timestamp things.
		if(shNetworkRuleSystemMessage == rule)
		{
			DataObject &thisObject = const_cast<DataObject &>(data);
			thisObject = DataObject(data.read<wxString>(), wxDateTime::Now());
		}

		wxLogDebug(wxT("Client executing asynch rule %s."), 
			rule.cwx_str());
		RULE.AsynchExecute(rule, data);
	}
	else
	{
		wxLogDebug(wxT("Client executing rule %s."), rule.cwx_str());

		RULE.Execute(rule, data);
	}
}

bool NetworkClient::VersionCheck(wxSocketBase * const socket)
{
	bool ret = true;

	wxString thisVer = GAMEVERSION;

	// First, send the information for this client.
	wxSocketOutputStream sockOut(*socket);
	wxDataOutputStream output(sockOut);

	output << thisVer;
	
	// Then receive their version.
	wxSocketInputStream sockIn(*socket);
	wxDataInputStream input(sockIn);

	wxString ver;
	input >> ver;

	// Compare it against our version.
	if(thisVer != ver)
	{
        static const wxString stVersionMismatchWithTheServer = _("Version mismatch with the server.  You have version %s.  The server has version %s.");

		wxString str = wxString::Format(stVersionMismatchWithTheServer.c_str(), 
            thisVer.c_str(), ver.c_str());
		
		Controller::get().Transmit(shEventNetworkConnectionEvent, str);

		ret = false;
	}

	return ret;
}

void NetworkClient::ReceiveSpectators(wxDataInputStream &stream)
{
	wxUint32 size;
	stream >> size;

	SpectatorArray spectators;

	for(wxUint32 i = 0; i < size; ++i)
	{
		Player player;
		stream >> player;
		
		wxInt32 rejoin;
		stream >> rejoin;

		spectators.push_back(std::make_pair(player, rejoin));
	}

	// Also receive the host player.
	Player player;
	stream >> player;
	
	// Set the values.
	NET.Players().SetSpectators(spectators);
	NET.Players().SetHostPlayer(player);
}

void NetworkClient::ReceiveGame(wxDataInputStream &input)
{
	wxUint32 size;

	//plugin size
	input >> size;

	//retrieve plugins
	IPluginEngine::StringPairArray array;
	array.reserve(size);

	wxString module, ruleset;
	for(wxUint32 i = 0; i < size; ++i)
	{
		input >> module;
		input >> ruleset;

		array.push_back(std::make_pair(module, ruleset));
	}

	//run the callback to load the rulesets
	Controller::get().Transmit(shEventLoadRulesets, array);

	// Options.
	input >> size;

	IOptionEngine::StringPairArray options;
	options.reserve(size);

	for(wxUint32 i = 0; i < size; ++i)
	{
		input >> module;
		input >> ruleset;

		options.push_back(std::make_pair(module, ruleset));
	}

	OPTION.SetSelectedOptions(options);

	//load the game
	GamePtr &game = GetGame();
	game->load(input, GameObjectEnginePtr(&GAMEOBJECT, no_delete_network()));

	// Stats.
	STAT.load(input);

	// Get the current map.
	wxInt32 mapIndex;
	input >> mapIndex;

	RULE.Execute(shRuleLoadMap, DataObject(mapIndex));
}

void NetworkClient::ReceiveRNG(wxDataInputStream &input)
{
	RAND.load(input);
}

void NetworkClient::ReceiveRule()
{
	// The exceptions generated from this are handled outside this function.
	size_t size = 0;
	mpSocketClient->Read(&size, 4);

	bool error = mpSocketClient->Error();
	size_t count = mpSocketClient->LastCount();

	if( (true == error) ||
		(4 != count))
	{
		throw std::runtime_error("Network failed to read 4 bytes.");
	}

	// Now that we've got the size, read the rest into memory so that
	// any errors in processing that memory will not result in a total 
	// desynchronization of the data coming over the network.
	boost::shared_array<wxUint8> data(new wxUint8[size]);
	mpSocketClient->Read(data.get(), size);

	error = mpSocketClient->Error();
	count = mpSocketClient->LastCount();
	
	if( (true == error) ||
		(size != count))
	{
		HashString str = 
			HashString::Format("Network failed to read %d bytes.", size);
		throw std::runtime_error(str.c_str());
	}

	// Get the rule out of the data.
	HashString rule;
	DataObject object;

	try
	{
		NetworkUtility::ReadRuleFromMemory(rule, object, data, size);
	}
	catch(const std::exception &e)
	{
		wxLogDebug(HashString(e.what()).wx_str());
	}

	wxLogDebug(wxT("Client received rule: %s"), rule.cwx_str());

	// In the special case of a chat message, timestamp things.
	if(shNetworkRuleChatMessage == rule)
	{
		object = DataObject(object.read<Player>(), object.read<wxString>(1), 
			wxDateTime::Now());
	}

	//execute it
	RULE.AsynchExecute(rule, object);
}

void NetworkClient::SendSpectator(const Player &player)
{
	wxSocketOutputStream sock(*mpSocketClient);
	wxDataOutputStream output(sock);

	output << player;
}

void NetworkClient::SocketInput()
{
	// Read the message type.
	wxInt32 type;
	mpSocketClient->Read(&type, 4);

	switch(static_cast<NetworkMessageType>(type))
	{
	case NetworkMessageRule:
		ReceiveRule();
		break;
	default:
		wxASSERT(false);
		break;
	}
}

void NetworkClient::SocketLost()
{
	mpSocketClient->Destroy();
	mpSocketClient = NULL;

	// Notify the UI.
	RULE.Execute(shNetworkRuleClientConnectionLost, DataObject());
}
