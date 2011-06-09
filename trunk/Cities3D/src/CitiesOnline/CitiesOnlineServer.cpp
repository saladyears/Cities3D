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
#include "CitiesOnlineServer.h"
#include "LocalServerThread.h"
#include "HostCheckServerThread.h"
#include "fnv.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	// Maximum time to wait for a connection to the server instance.
	const size_t sSecondsToConnect = 10;

	const wxInt32 sClientPort = 7177;
}

//---------------------------- PUBLIC           -----------------------------//
wxCitiesOnlineServer::wxCitiesOnlineServer()
: mpHostCheckServerThread(NULL)
{
	// Start up the server thread.
	mpLocalServerThread = new wxLocalServerThread;

	if(wxTHREAD_NO_ERROR == mpLocalServerThread->Create())
	{
		mpLocalServerThread->Run();
	}
	else
	{
		delete mpLocalServerThread;
		mpLocalServerThread = NULL;
	}
}

wxCitiesOnlineServer::~wxCitiesOnlineServer()
{
	if(NULL != mpLocalServerThread)
	{
		mpLocalServerThread->Delete();
	}
}

bool wxCitiesOnlineServer::SendMessage(const wxString& message)
{
	bool ret = false;

	// Create a client socket, attempt to attach to localhost, and send
	// the message.
	wxIPV4address addr;
	addr.Hostname(sw127001);
	addr.Service(sClientPort);

	boost::scoped_ptr<wxSocketClient> socket(new wxSocketClient);
	
	socket->SetFlags(wxSOCKET_BLOCK | wxSOCKET_REUSEADDR_FLAG);

	// Give it a certain amount of time to connect.
	socket->Connect(addr, true);
	socket->WaitOnConnect(sSecondsToConnect);

	// If we connected, great, send the message.
	if(true == socket->IsConnected())
	{
		wxSocketOutputStream stream(*socket);
		wxDataOutputStream out(stream);

		out.WriteString(message);

		// Now that we've sent the string, validate it.
		size_t inHash;
		
		// Calculate the hash value using the FNV hash.
		size_t outHash = fnv_32_buf(message.c_str(), message.Length() * 
			sizeof(wxChar), 0);

		socket->Read(&inHash, sizeof(size_t));
		socket->Write(&outHash, sizeof(size_t));

		// If they are the same, we have successfully sent the message.
		if(inHash == outHash)
		{
			ret = true;
		}

		socket->Destroy();
	}

	return ret;
}

void wxCitiesOnlineServer::Hostcheck(long port)
{
	// Only handle one host check at a time.
	if(NULL == mpHostCheckServerThread)
	{
		mpHostCheckServerThread = new wxHostCheckServerThread(this, port);

		if(wxTHREAD_NO_ERROR == mpHostCheckServerThread->Create())
		{
			mpHostCheckServerThread->Run();
		}
		else
		{
			delete mpHostCheckServerThread;
			mpHostCheckServerThread = NULL;
		}
	}
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
