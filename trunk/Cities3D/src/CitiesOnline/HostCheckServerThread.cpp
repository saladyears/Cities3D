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
#include "HostCheckServerThread.h"
#include "CitiesOnlineServer.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	const size_t sSleepTime = 250;
	const size_t sWaitTime = 10000;		// Ten seconds of checking.
	const size_t sAttempts = sWaitTime / sSleepTime;
}

//---------------------------- PUBLIC           -----------------------------//
wxHostCheckServerThread::wxHostCheckServerThread(wxCitiesOnlineServer* server, 
	long port)
: mPort(port)
, mServer(server)
{
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void* wxHostCheckServerThread::Entry()
{
	wxIPV4address addr;
	addr.AnyAddress();
	addr.Service(mPort);

	boost::scoped_ptr<wxSocketServer> server(new wxSocketServer(addr, wxSOCKET_REUSEADDR_FLAG));
	size_t attempts = sAttempts;

	if(true == server->IsOk())
	{
		// Loop, testing for connections and our count.
		while( (false == TestDestroy()) &&
			   (0 < attempts))
		{
			attempts--;

			wxSocketBase socket;
			server->AcceptWith(socket, false);

			if(true == socket.IsConnected())
			{
				socket.SetFlags(wxSOCKET_REUSEADDR_FLAG | wxSOCKET_BLOCK 
					| wxSOCKET_WAITALL);

				wxUint32 number = 0;

				socket.Read(&number, sizeof(wxUint32));

				// All site traffic comes in big endian format.
				wxUint32 compare = wxUINT32_SWAP_ON_LE('SITE');

				if(number == compare)
				{
					wxUint32 magic = wxUINT32_SWAP_ON_LE('CONL');
					socket.Write(&magic, sizeof(wxUint32));
				}

				attempts = 0;
			}

			Sleep(sSleepTime);
		}
	}

	// Signal the server that we're done.
	mServer->SetHostThreadDone();

	return NULL;
}
