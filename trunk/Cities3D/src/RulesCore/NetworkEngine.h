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
#pragma once

#include "style.h"	//READ THIS BEFORE MAKING ANY CHANGES TO THIS FILE!!!

//---------------------------- SYSTEM INCLUDES  -----------------------------//

//---------------------------- USER INCLUDES    -----------------------------//
#include "INetworkEngine.h"
#include "NetworkPlayers.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
enum NetworkMessageType
{
	NetworkMessageRule		= 0,
	
	NetworkMessageDword		= 0x7fffffff
};

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: NetworkEngine
// 
// Needs documentation.
//
// Derived From:
//     <INetworkEngine>
//
// Project:
//     <NetworkRules>
//
// Include:
//     NetworkEngine.h
//
class NetworkEngine : public INetworkEngine
{
public:
	virtual void StartServer(const wxIPV4address &addr, const Player &player);
	virtual void StartClient(const wxIPV4address &addr, const Player &player);

	virtual void Stop();

	virtual bool IsConnected() const;
	virtual bool DoesServerHaveClients() const;

	virtual NetworkPlayers &Players() {return mNetworkPlayers;}
	virtual const NetworkPlayers &Players() const {return mNetworkPlayers;}

	virtual void SendRule(const HashString &rule, 
		const DataObject &object) const;

private:
	//network objects
	NetworkServerPtr mNetworkServer;
	NetworkClientPtr mNetworkClient;

	NetworkPlayers mNetworkPlayers;
};

//---------------------------- PROTOTYPES       -----------------------------//
