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
#include "ServerThread.h"
#include "Cities3DSocket.h"
#include "ServerGameThread.h"
#include "RuleEvent.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
wxServerThread::wxServerThread(const size_t port, wxEvtHandler * const handler,
	wxServerGameThread *thread)
: mPort(port)
, mHandler(handler)
, mGameThread(thread)
{
	wxASSERT(NULL != handler);
	wxASSERT(NULL != thread);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void *wxServerThread::Entry()
{
	// Create the server on the given port.
	wxIPV4address addr;
	addr.Service(mPort);

	mServer = new wxSocketServer(addr, wxSOCKET_REUSEADDR_FLAG);

	// Ensure we got started up.
	if(false == mServer->Ok())
	{
		wxLogError(wxT("Unable to start a server at %s:%d."), 
			addr.Hostname().c_str(), addr.Service());

		return NULL;
	}

	wxLogDebug(wxT("Server listening at %s:%d"), 
		addr.Hostname().c_str(), addr.Service());

	// We're listening, so pass along a rule getting things going to the 
	// event handler.
	wxRuleEvent event(shNetworkRuleServerStart, DataObject(), true);
	mHandler->AddPendingEvent(event);

	// Now, loop, waiting for connections to happen.
	while(false == TestDestroy())
	{
		wxCities3DSocket *socket = new wxCities3DSocket;

		// See if there's any connections to accept.
		if(false == mServer->AcceptWith(*socket, false))
		{
			// If not, clean up, sleep one second and continue.
			delete socket;
			socket = NULL;

			Sleep(1000);
			continue;
		}

		// Since we can be killed at any conceivable time, test before every
		// new action.
		if(false == TestDestroy())
		{
			// We got a connection.
			wxLogDebug(wxT("Server connection accepted."));

			//Start up the connection thread that verifies everything.
			wxRuleEvent event(shNetworkRuleServerConnection, 
				DataObject(mHandler, socket), true);
			mHandler->AddPendingEvent(event);

			// Add to the current game sockets.
			if(false == TestDestroy())
			{
				mGameThread->AddSocket(socket);
			}
		}		
	}

	return NULL;
}

void wxServerThread::OnExit()
{
	wxLogDebug(wxT("Server thread exiting."));

	if(NULL != mServer)
	{
		mServer->Destroy();
		mServer = NULL;
	}
}
