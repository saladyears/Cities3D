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
#include <boost/shared_array.hpp>

//---------------------------- USER INCLUDES    -----------------------------//
#include "Game.h"
#include "NetworkPlayers.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class wxCities3DSocket;
class wxMemoryOutputStream;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxServerConnectionThread
//
// A thread that handles the negotiation when a new client connects to ensure
// that the new client is a valid client.
//
// Derived From:
//     <wxThread>
//
// Project:
//     <RulesCore>
//
// Include:
//     ServerConnectionThread.h
//
class wxServerConnectionThread : public wxThread
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxServerConnectionThread
	//
	// The wxServerConnectionThread constructor.
	//
	// Parameters:
	//     handler - The wxEvtHandler to pass events to.
	//     socket - The <wxCities3DSocket> to negotiate with.
	//     game - The <Game> data to send.
	//     version - The current Cities3D version.
	//     spectators - The array of spectators to send.
	//     rng - The pool of random numbers.
	//     size - The size of the RNG pool.
	//     
	wxServerConnectionThread(wxEvtHandler * const handler, 
		wxCities3DSocket * const socket, const Game &game,
		const wxString &version, const SpectatorArray &spectators, 
		const boost::scoped_array<wxUint8> &rng, const size_t size);

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

	bool Cities3DCheck() const;
	void VersionCheck() const;
	
	void ReceiveSpectator() const;

	void SendSpectators() const;
	void SendGame() const;
	void SendRNG() const;
	

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
	// Pointer: mSocket
	//
	// The socket to communicate with.
	//
	wxCities3DSocket *mSocket;

	//-----------------------------------------------------------------------//
	// Object: mGame
	//
	// The <Game> data to send.
	//
	Game mGame;

	//-----------------------------------------------------------------------//
	// String: mVersion
	//
	// The current Cities3D version.
	//
	wxString mVersion;

	//-----------------------------------------------------------------------//
	// Array: mSpecatators
	//
	// The spectators in the game.
	//
	SpectatorArray mSpectators;

	//-----------------------------------------------------------------------//
	// Array: mRNG
	//
	// The pool of random numbers.
	//
	boost::scoped_array<wxUint8> mRNG;

	//-----------------------------------------------------------------------//
	// Int: mSize
	//
	// The size of the RNG pool.
	//
	size_t mSize;
};

//---------------------------- PROTOTYPES       -----------------------------//

