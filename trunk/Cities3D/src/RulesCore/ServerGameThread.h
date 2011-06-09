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
#include <deque>
#include <boost/shared_array.hpp>

//---------------------------- USER INCLUDES    -----------------------------//
#include "Player.h"
#include "NetworkEngine.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class wxCities3DSocket;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxServerGameThread
// 
// The main networking thread that handles all game-related socket 
// communication.
//
// Derived From:
//     <wxThread>
//
// Project:
//     <RulesCore>
//
// Include:
//     ServerGameThread.h
//
class wxServerGameThread : public wxThread
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxServerGameThread
	//
	// The wxServerGameThread constructor.
	//
	// Parameters:
	//     handler - The event handler to pass events to.
	//     hostPlayer - The host player at the server.
	//
	wxServerGameThread(wxEvtHandler * const handler, const Player &hostPlayer);

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: AddSocket
	//
	// Adds a new socket to the internal socket map.
	//
	// Parameters:
	//     socket - The socket to add.
	//
	void AddSocket(wxCities3DSocket * const socket);

	//-----------------------------------------------------------------------//
	// Function: HasSockets
	//
	// Returns whether the game thread has connected sockets.
	//
	// Returns:
	//     A bool that is true if the game thread has connect sockets, and 
	//     false if not.
	//
	bool HasSockets() const;

	//-----------------------------------------------------------------------//
	// Function: SendRule
	//
	// Sends a rule to every connected socket.
	//
	// Parameters:
	//     data - The rule data to send.
	//     size - The size of the data.
	//
	void SendRule(const boost::shared_array<wxUint8> &data, 
		const size_t size);

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
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: HandleDisconnects
	//
	// Looks for disconnects in the socket map.
	//
	// Returns:
	//     A bool that is true if any disconnect was found, and false if not.
	//
	bool HandleDisconnects();

	//-----------------------------------------------------------------------//
	// Function: HandleJoins
	//
	// Looks for sockets that have just been completed.
	//
	// Returns:
	//     A bool that is true if any join was found, and false if not.
	//
	bool HandleJoins();

	//-----------------------------------------------------------------------//
	// Function: HandleIO
	//
	// Handles socket i/o.
	//
	void HandleIO();

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Pointer: mHandler
	//
	// The event handler to pass events to.
	//
	wxEvtHandler *mHandler;

	//-----------------------------------------------------------------------//
	// Object: mLock
	//
	// The recursive mutex that provides thread safety with <wxServerThread>.
	//
	mutable wxMutex mLock;

	//-----------------------------------------------------------------------//
	// Object: mHostPlayer
	//
	// The host player, an optimization to not have to look it up while in the
	// thread.
	//
	Player mHostPlayer;

	//-----------------------------------------------------------------------//
	// Object: mNullPlayer
	//
	// The NullPlayer, an optimization to not have to look it up in every loop.
	//
	Player mNullPlayer;

	// Structs for reading and writing.
	struct ReadSocketBuffer
	{
		ReadSocketBuffer() : mode(WaitingForData), size(0) {}

		enum Mode
		{
			WaitingForData			= 0,
			WaitingForRule			,
		};

		Mode mode;
		boost::shared_array<wxUint8> data;
		size_t size;
	};

	struct WriteSocketBuffer
	{
		WriteSocketBuffer() : type(NetworkMessageRule), mode(SendingType) {}

		enum Mode
		{
			SendingType				= 0,
			SendingSize				,
			SendingData				,
		};

		NetworkMessageType type;
		Mode mode;
		boost::shared_array<wxUint8> data;
		size_t size;
		size_t sent;
	};

	typedef std::deque<WriteSocketBuffer> WriteArray;

	// Each socket gets a buffer for reading and for writing.
	typedef std::pair<WriteArray, ReadSocketBuffer> SocketStream;

	typedef std::map<wxCities3DSocket * const, SocketStream> SocketMap;

	//-----------------------------------------------------------------------//
	// Object: mSocketMap
	//
	// The map of sockets currently connected.
	//
	SocketMap mSocketMap;
};

//---------------------------- PROTOTYPES       -----------------------------//

