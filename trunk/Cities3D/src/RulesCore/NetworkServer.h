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
#include <map>

//---------------------------- USER INCLUDES    -----------------------------//
#include "INetworkServer.h"
#include "Access.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class wxIPV4address;
class wxServerThread;
class wxServerGameThread;
class wxRuleEvent;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: NetworkServer
// 
// Needs documentation.
//
// Derived From:
//     wxEvtHandler,
//     <INetworkServer>
//
// Project:
//     <NetworkRules>
//
// Include:
//     NetworkServer.h
//
class NetworkServer 
: public wxEvtHandler
, public INetworkServer
, public Access
{
public:
	NetworkServer(const wxIPV4address &addr, const Player &player);
	~NetworkServer();

	virtual bool IsConnected() const;

	virtual void SendRule(const HashString &rule, 
		const DataObject &object) const;

	DECLARE_EVENT_TABLE()

private:
	NetworkServer();

	void OnCloseGameWindow(wxInt32);

	void OnRule(wxRuleEvent &event);
	
	wxServerThread *mServerThread;
	wxServerGameThread *mServerGameThread;
};
//---------------------------- PROTOTYPES       -----------------------------//

