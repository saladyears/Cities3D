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
#include "ServerConnectionThread.h"
#include "Cities3DSocket.h"
#include "RuleEvent.h"
#include "RNG.h"
#include "GameEngine.h"
#include "NetworkPlayers.h"
#include "IPluginEngine.h"
#include "INetworkEngine.h"
#include "IOptionEngine.h"
#include "IStatEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
wxServerConnectionThread::wxServerConnectionThread(
	wxEvtHandler * const handler, wxCities3DSocket * const socket, 
	const Game &game, const wxString &version, 
	const SpectatorArray &spectators, 
	const boost::scoped_array<wxUint8> &rng, const size_t size)
: mHandler(handler)
, mSocket(socket)
, mGame(game)
, mVersion(version)
, mSpectators(spectators)
, mSize(size)
{
	wxASSERT(NULL != mHandler);
	wxASSERT(NULL != mSocket);
	wxASSERT(0 < mSize);

	mRNG.reset(new wxUint8[mSize]);
	memcpy(mRNG.get(), rng.get(), mSize);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void *wxServerConnectionThread::Entry()
{
	bool valid = true;

	// It's ok to block this thread waiting for data.
	mSocket->SetFlags(wxSOCKET_WAITALL);

	try
	{
		// If we pass the check, but return false, it means we got hit by the
		// site requesting a host check.

		if(false == Cities3DCheck())
		{
            static const wxString stReceivedAHostCheck = _("Received a host check from the site.");
    
			wxRuleEvent event(shNetworkRuleSystemMessage, 
				DataObject(stReceivedAHostCheck, wxDateTime::Now()));
			mHandler->AddPendingEvent(event);

			valid = false;
			goto error;
		}
		VersionCheck();

		ReceiveSpectator();

		SendSpectators();
		SendGame();
		SendRNG();
	}
	catch(const std::exception &e)
	{
		valid = false;

		wxLogDebug(HashString(e.what()).wx_str());
	}

error:
	if(true == valid)
	{
		// This socket is complete and ready for data.
		mSocket->mIsComplete = true;
	}
	else
	{
		mSocket->Destroy();
		mSocket = NULL;
	}

	return NULL;
}

bool wxServerConnectionThread::Cities3DCheck() const
{
	wxLogDebug(wxT("Server starting Cities3D check."));

	// Handshake to determine that we're talking to a client or the site.
	wxUint32 number = 0;
	mSocket->Read(&number, sizeof(wxUint32));

	// Magic numbers.
	wxUint32 site = wxUINT32_SWAP_ON_LE('SITE');
	wxUint32 client = wxUINT32_SWAP_ON_LE('CC3D');

	// If it's not the site or the client, bail.
	if( (site != number) &&
		(client != number))
	{
		throw std::runtime_error("Server Error: Not talking to a Cities3D app.");
	}

	// We've got a client or the site, reassure them this is a server.
	wxUint32 magic = wxUINT32_SWAP_ON_LE('CS3D');
	mSocket->Write(&magic, sizeof(wxUint32));

	wxLogDebug(wxT("Server ending Cities3D check."));

	// Return true if we got a client, false for the site.
	return (client == number);
}

void wxServerConnectionThread::VersionCheck() const
{
	wxLogDebug(wxT("Server starting version check."));

	// First, receive the version information from the client.
	wxSocketInputStream sockIn(*mSocket);
	wxDataInputStream input(sockIn);

	wxString ver;
	input >> ver;

	// Send our version so they know what's happening.
	wxSocketOutputStream sockOut(*mSocket);
	wxDataOutputStream output(sockOut);

	output << mVersion;

	if(mVersion != ver)
	{
		wxRuleEvent event(shNetworkRuleVersionMismatch, 
			DataObject(mVersion, ver));
		mHandler->AddPendingEvent(event);

		throw std::runtime_error("Server Error: Incompatible versions.");
	}

	wxLogDebug(wxT("Server ending version check."));
}

void wxServerConnectionThread::ReceiveSpectator() const
{
	wxLogDebug(wxT("Server starting spectator receive."));

	// Get who the heck they are.
	wxSocketInputStream sock(*mSocket);
	wxDataInputStream input(sock);

	Player player;
	input >> player;

	mSocket->mPlayer = player;

	wxLogDebug(wxT("Server ending spectator receive."));
}

void wxServerConnectionThread::SendSpectators() const
{
	wxLogDebug(wxT("Server starting spectator send."));

	wxSocketOutputStream sock(*mSocket);
	wxDataOutputStream stream(sock);

	stream << (wxUint32) mSpectators.size();

	// Dump all of the spectators.
	SpectatorArray::const_iterator it, itEnd = mSpectators.end();
	for(it = mSpectators.begin(); it != itEnd; ++it)
	{
		stream << it->first;
		stream << it->second;
	}

	// Also send our own information so the client knows who the host player
	// is.
	stream << NET.Players().GetThisPlayer();
	
	wxLogDebug(wxT("Server ending spectator send."));
}

void wxServerConnectionThread::SendGame() const
{
	wxLogDebug(wxT("Server starting game send."));

	IPluginEngine::StringPairArray array;
	PLUGIN.GetLoadedRuleSets(array);

	//open a data socket stream to the client
	wxSocketOutputStream sock(*mSocket);
	wxDataOutputStream output(sock);

	//plugin data
	output << (wxUint32) array.size();

	IPluginEngine::StringPairArray::const_iterator it = array.begin(), 
		itEnd = array.end();
	for(; it != itEnd; ++it)
	{
		output << it->first;
		output << it->second;
	}

	// options data
	IOptionEngine::StringPairArray options; 
	OPTION.GetSelectedOptions(options);
	output << (wxUint32) options.size();

	itEnd = options.end();
	for(it = options.begin(); it != itEnd; ++it)
	{
		output << it->first;
		output << it->second;
	}

	//now send the game
	mGame.save(output);

	// Send the stats.
	STAT.save(output);

	// Also send the current map index so the client can load it.  NOTE:
	// this may have to change to the actual map data in the future.
	output << MAPINDEX;

	wxLogDebug(wxT("Server ending game send."));
}

void wxServerConnectionThread::SendRNG() const
{
	wxLogDebug(wxT("Server starting RNG send."));

	mSocket->Write(mRNG.get(), mSize);

	wxLogDebug(wxT("Server ending RNG send."));
}
