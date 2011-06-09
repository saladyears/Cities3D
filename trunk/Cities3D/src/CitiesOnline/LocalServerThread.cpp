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
#include "LocalServerThread.h"
#include "Launcher.h"
#include "CitiesOnlineApp.h"
#include "fnv.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	// Should never have a command line over this length.  This is a
	// safe-guard against buffer overrun attacks.
	const size_t sMaximumMessageLength = 2048;

	// FIXME: Should this be customizable?
	const wxInt32 sLocalPort = 7177;

	const size_t sLocalSleepTime = 250;

	void ProcessInput(wxSocketBase& socket)
	{
		socket.SetFlags(wxSOCKET_REUSEADDR_FLAG | wxSOCKET_BLOCK 
			| wxSOCKET_WAITALL);
		
		wxSocketInputStream stream(socket);
		wxDataInputStream in(stream);

		wxString url = in.ReadString();
		
		// Validate the string.
		size_t size = url.Length();
		size_t inHash = 0;
		size_t outHash = 0;

		// Only handle valid strings.
		if( (0 < size) &&
			(sMaximumMessageLength >= size))
		{
			// Calculate the hash value using the FNV hash.
			outHash = fnv_32_buf(url.c_str(), url.Length() * 
				sizeof(wxChar), 0);
		}

		// Write the validation.
		socket.Write(&outHash, sizeof(size_t));

		// Read the validation.
		socket.Read(&inHash, sizeof(size_t));

		// If they are the same, process the url.
		if(inHash == outHash)
		{
			// We can't launch dialogs from a child thread so we need to post
			// to the main thread.
			wxTheCitiesOnlineApp->PostUrl(url);
		}
	}
}

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void* wxLocalServerThread::Entry()
{
	// Defaults to localhost, which we want for local connections.
	wxIPV4address addr;
	addr.Hostname(sw127001);
	addr.Service(sLocalPort);

	// Create the socket.
	mpLocalServer.reset(new wxSocketServer(addr, wxSOCKET_REUSEADDR_FLAG));

	// If this fails, any secondary instances will fail to connect and
	// should shut down this instance.
	if(true == mpLocalServer->IsOk())
	{
		// Now loop, testing for connections.
		while(false == TestDestroy())
		{
			wxSocketBase socket;
			mpLocalServer->AcceptWith(socket, false);

			// If we got a connection, do stuff, otherwise sleep and try again.
			if(true == socket.IsConnected())
			{
				// Process connection and end.
				ProcessInput(socket);
			}

			Sleep(sLocalSleepTime);
		}
	}

	return NULL;
}

void wxLocalServerThread::OnExit()
{
	mpLocalServer.reset();
}
