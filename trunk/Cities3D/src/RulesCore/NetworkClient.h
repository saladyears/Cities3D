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
#include "INetworkClient.h"
#include "INetworkEngine.h"
#include "Access.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class wxIPV4address;
class wxDataInputStream;
class wxRuleEvent;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: NetworkClient
// 
// Needs documentation.
//
// Derived From:
//     wxEvtHandler,
//     <INetworkClient>
//
// Project:
//     <NetworkRules>
//
// Include:
//     NetworkClient.h
//
class NetworkClient
: public wxEvtHandler
, public INetworkClient
, public Access
{
public:
	NetworkClient(const wxIPV4address &addr, const Player &player);
	~NetworkClient();

	virtual bool Connect(const wxIPV4address &addr);
	virtual bool IsConnected() const;

	virtual void SendRule(const HashString &rule, 
		const DataObject &object) const;

	DECLARE_EVENT_TABLE()

private:
	NetworkClient();

	void OnSocketEvent(wxSocketEvent &event);
	void OnRule(wxRuleEvent &event);

	bool VersionCheck(wxSocketBase * const socket);

	void ReceiveSpectators(wxDataInputStream &input);
	void ReceiveGame(wxDataInputStream &input);
	void ReceiveRNG(wxDataInputStream &input);

	void ReceiveRule();
	
	void SendSpectator(const Player &player);

	void SocketInput();
	void SocketLost();

	wxSocketClient *mpSocketClient;
};
//---------------------------- PROTOTYPES       -----------------------------//
