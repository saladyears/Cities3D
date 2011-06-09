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
#include "PortForwardManager.h"
#include "PortForwardThread.h"

//---------------------------- USER INCLUDES    -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
long PortForwardManager::GetPort() const
{
#ifdef _WIN32 
	long port = -1;
	if(NULL != mpPortForwardThread)
	{
		port = mpPortForwardThread->GetPort();
	}

	if(-1 != port)
	{
		mPort = port;
	}
#endif // _WIN32

	return mPort;
}

void PortForwardManager::Initialize()
{
#ifdef _WIN32
	// Create and start up the thread.
	mpPortForwardThread = new wxPortForwardThread;

	if(wxTHREAD_NO_ERROR == mpPortForwardThread->Create())
	{
		mpPortForwardThread->Run();
	}
	else
	{
		delete mpPortForwardThread;
		mpPortForwardThread = NULL;
	}
#endif // _WIN32 
}

void PortForwardManager::Shutdown()
{
#ifdef _WIN32
	if(NULL != mpPortForwardThread)
	{
		mpPortForwardThread->Delete();
	}
#endif // _WIN32 
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
PortForwardManager::PortForwardManager()
: mPort(7176)
, mpPortForwardThread(NULL)
{
}
