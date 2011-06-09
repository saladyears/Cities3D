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
#include <wx/socket.h>

//---------------------------- USER INCLUDES    -----------------------------//
#include "RulesExport.h"

//---------------------------- DEFINES          -----------------------------//

#ifdef __WIN32__
#define wxSOCKET_REUSEADDR_FLAG 0
#else 
#define wxSOCKET_REUSEADDR_FLAG wxSOCKET_REUSEADDR
#endif

//---------------------------- TYPEDEFS         -----------------------------//
class DataObject;
class Player;
class NetworkPlayers;

class INetworkServer;
typedef boost::shared_ptr<INetworkServer> NetworkServerPtr;

class INetworkClient;
typedef boost::shared_ptr<INetworkClient> NetworkClientPtr;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: INetworkEngine
// 
// Needs documentation.
//
// Project:
//     <RulesCore>
//
// Include:
//     INetworkEngine.h
//
class RULES_EXPORT_SPEC INetworkEngine
{
public:
	virtual void StartServer(const wxIPV4address &addr, 
		const Player &player)=0;
	virtual void StartClient(const wxIPV4address &addr, 
		const Player &player)=0;

	virtual void Stop()=0;

	virtual bool IsConnected() const=0;
	virtual bool DoesServerHaveClients() const=0;

	virtual NetworkPlayers &Players()=0;
	virtual const NetworkPlayers &Players() const=0;

	virtual void SendRule(const HashString &rule, 
		const DataObject &object) const=0;

protected:
	virtual ~INetworkEngine()=0;
};

//---------------------------- PROTOTYPES       -----------------------------//
typedef boost::shared_ptr<INetworkEngine> NetworkEnginePtr;
