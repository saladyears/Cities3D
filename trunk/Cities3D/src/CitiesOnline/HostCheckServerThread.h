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

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class wxSocketServer;
class wxCitiesOnlineServer;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxHostCheckServerThread
// 
// A server worker thread for handling connections from the site for host
// checks.
//
// Derived From:
//     wxThread
//
// Project:
//     <CitiesOnline>
//
// Include:
//     HostCheckServerThread.h
//
class wxHostCheckServerThread : public wxThread
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxHostCheckServerThread
	//
	// The wxHostCheckServerThread constructor.
	//
	// Parameters:
	//     port - The port to listen on.
	//
	wxHostCheckServerThread(wxCitiesOnlineServer* server, long port);

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Entry
	//
	// The entry point of the thread where it does all of its work.
	//
	// Returns:
	//     A NULL pointer.
	//
	virtual void* Entry();

	//-----------------------------------------------------------------------//
	// Group: Variables
	//
	
	//-----------------------------------------------------------------------//
	// Int: mPort
	//
	// The port to listen on.
	//
	long mPort;

	//-----------------------------------------------------------------------//
	// Pointer: mServer
	//
	// The pointer to the server to signal our completion.
	//
	wxCitiesOnlineServer* mServer;
};

//---------------------------- PROTOTYPES       -----------------------------//

