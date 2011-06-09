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
#include "NetworkEngine.h"
#include "DataObject.h"
#include "Controller.h"
#include "NetworkServer.h"
#include "NetworkClient.h"
#include "Player.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
void NetworkEngine::StartServer(const wxIPV4address &addr, 
								const Player &player)
{
	Stop();

	mNetworkServer = NetworkServerPtr(new NetworkServer(addr, player));
}

void NetworkEngine::StartClient(const wxIPV4address &addr, 
								const Player &player)
{
	Stop();

	mNetworkClient = NetworkClientPtr(new NetworkClient(addr, player));

	if(false == mNetworkClient->IsConnected())
	{
		mNetworkClient.reset();
	}
}

void NetworkEngine::Stop()
{
	mNetworkServer.reset();
	mNetworkClient.reset();

	mNetworkPlayers.Reset();
	
	Controller::get().Transmit(shEventGameConnect, DataObject(false));
}

bool NetworkEngine::IsConnected() const
{
	bool ret = false;

	if(mNetworkServer)
	{
		// A server is, by definition, always connected.
		ret = true;
	}
	else if(mNetworkClient)
	{
		ret = mNetworkClient->IsConnected();
	}

	return ret;
}

bool NetworkEngine::DoesServerHaveClients() const
{
	bool ret = false;

	if(mNetworkServer)
	{
		ret = mNetworkServer->IsConnected();
	}

	return ret;
}

void NetworkEngine::SendRule(const HashString &rule, 
							 const DataObject &object) const
{
	if(mNetworkServer)
	{
		mNetworkServer->SendRule(rule, object);
	}
	else if(mNetworkClient)
	{
		mNetworkClient->SendRule(rule, object);
	}
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
