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
#include <wx/socket.h>

//---------------------------- USER INCLUDES    -----------------------------//
#include "Player.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxCities3DSocket
//
// A socket that also contains information about whether negotiations with the
// player are complete and what <Player> is associated with this socket.
//
// Derived From:
//     <wxSocketBase>
//
// Project:
//     <RulesCore>
//
// Include:
//     Cities3DSocket.h
//
class wxCities3DSocket : public wxSocketBase
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxCities3DSocket
	//
	// The wxCities3DSocket constructor.
	//
	wxCities3DSocket();

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: IsComplete
	//
	// Returns whether the socket has completed initial negotiations.
	//
	// Returns:
	//     A bool that is true if the socket has completed negotiations, and
	//     false if not.
	//
	const bool IsComplete() const {return mIsComplete;}

	//-----------------------------------------------------------------------//
	// Function: HasJoined
	//
	// Returns whether the <Player> at this socket has officially joined the 
	// game or not.
	//
	// Returns:
	//     A bool that is true if the <Player> has joined the game, and false
	//     if not.
	//
	const bool HasJoined() const {return mHasJoined;}

	//-----------------------------------------------------------------------//
	// Function: Join
	//
	// Marks the <Player> at this socket as having officially joined the game.
	//
	void Join() {mHasJoined = true;}

	//-----------------------------------------------------------------------//
	// Function: Player
	//
	// Returns the <Player> associated with the socket, if any.
	//
	// Returns:
	//     The <Player> associated with the socket, which may be the NullPlayer
	//     if the actual player hasn't been determined yet.
	//
	const Player &GetPlayer() const {return mPlayer;}

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Bool: mIsComplete
	//
	// Whether the socket has completed negotiations or not.
	//
	bool mIsComplete;

	//-----------------------------------------------------------------------//
	// Bool: mHasJoined
	//
	// Whether the <Player> at this socket has officially joined the game or 
	// not.
	//
	bool mHasJoined;

	//-----------------------------------------------------------------------//
	// Object: mPlayer
	//
	// The <Player> associated with this socket.
	//
	Player mPlayer;

	// This avoids any public set methods.
	friend class wxServerConnectionThread;
};

//---------------------------- PROTOTYPES       -----------------------------//

