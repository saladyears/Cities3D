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
class wxServerGameThread;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxServerThread
//
// A thread that listens on the given address and creates <wxConnectionThreads>
// in response to clients connecting.
//
// Derived From:
//     <wxThread>
//
// Project:
//     <RulesCore>
//
// Include:
//     ServerThread.h
//
class wxServerThread : public wxThread
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxServerThread
	//
	// The wxServerThread constructor.  Starts a server socket listning at the
	// given address.
	//
	// Parameters:
	//     port - The port to listen on.
	//     handler - The event handler to pass events to.
	//     thread - The <wxServerGameThread> responsible for game-related 
	//              socket communication.
	//
	wxServerThread(const size_t port, wxEvtHandler * const handler, 
		wxServerGameThread *thread);

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
	virtual void *Entry();

	//-----------------------------------------------------------------------//
	// Function: OnExit
	//
	// Handles cleanup when the thread exits.
	//
	virtual void OnExit();

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Pointer: mServer
	//
	// The socket server.
	//
	wxSocketServer *mServer;

	//-----------------------------------------------------------------------//
	// Int: mPort
	//
	// The port the server should listen on.
	//
	size_t mPort;

	//-----------------------------------------------------------------------//
	// Pointer: mHandler
	//
	// The event handler to pass events to.
	//
	wxEvtHandler *mHandler;

	//-----------------------------------------------------------------------//
	// Pointer: mGameThread
	//
	// The thread responsible for game-related socket communication.
	//
	wxServerGameThread *mGameThread;
};

//---------------------------- PROTOTYPES       -----------------------------//

