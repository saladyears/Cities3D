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
#include <map>

//---------------------------- USER INCLUDES    -----------------------------//
#include "RulesExport.h"
#include "Player.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
typedef std::pair<Player, wxBOOL> SpectatorPair;
typedef std::vector<SpectatorPair> SpectatorArray;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: NetworkPlayers
// 
// Stores all of the information about players in a network game.
//
// Project:
//     <RulesCore>
//
// Include:
//     NetworkPlayers.h
//
class RULES_EXPORT_SPEC NetworkPlayers
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//  

	//-----------------------------------------------------------------------//
	// Constructor: NetworkPlayers 
	//  
	// The default NetworkPlayers constructor.
	//  
	NetworkPlayers();

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Reset
	// 
	// Resets the NetworkPlayers.
	//
	void Reset();

	//
	//-----------------------------------------------------------------------//
	// Function: GetSpectators
	//
	// Returns the array of spectators in the network game.
	//
	// Returns:
	//     An array of <Players> that are the spectators in the network game.
	//
	const SpectatorArray &GetSpectators() const;

	//-----------------------------------------------------------------------//
	// Function: GetThisPlayer
	//
	// Returns the <Player> at this computer.
	//
	// Returns:
	//     A const reference to the <Player> at this computer.
	//
	const Player &GetThisPlayer();

	//-----------------------------------------------------------------------//
	// Function: GetHostPlayer
	//
	// Returns the host <Player>.
	//
	// Returns:
	//     A const reference to the host <Player>.
	//
	const Player &GetHostPlayer();

	//-----------------------------------------------------------------------//
	// Function: SetSpectators
	//
	// Sets the spectator array to the given array.  Calls eventSpectator.
	//
	// Parameters:
	//     spectators - The new spectator list.
	//
	void SetSpectators(const SpectatorArray &spectators);

	//-----------------------------------------------------------------------//
	// Function: SetThisPlayer
	//
	// Sets the player at this computer.
	//
	// Parameters:
	//     player - The <Player> at this computer.
	//
	void SetThisPlayer(const Player &player);

	//-----------------------------------------------------------------------//
	// Function: SetHostPlayer
	//
	// Sets the host player.
	//
	// Parameters:
	//     player - The <Player> that is the host player.
	//
	void SetHostPlayer(const Player &player);

	//-----------------------------------------------------------------------//
	// Function: IsHost
	//
	// Returns whether the player is the host player or not.
	//
	// Returns:
	//     True if the player is the host, false if not.
	//
	bool IsHost();

	//-----------------------------------------------------------------------//
	// Function: IsSpectator
	//
	// Returns whether the player is a spectator or not.
	//
	// Parameters:
	//     The player to check.  Defaults to the player at this computer.
	//
	// Returns:
	//     True if the player is a spectator, false if not.
	//
	bool IsSpectator(const Player& player = NullPlayer);

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Array: mSpectators
	//
	// The array of current spectators to the game.
	//
	SpectatorArray mSpectators;

	//-----------------------------------------------------------------------//
	// Object: mThisPlayer;
	//
	// The player at this computer in the network game.
	//
	Player mThisPlayer;

	//-----------------------------------------------------------------------//
	// Object: mHostPlayer
	//
	// The host player in the network game.
	//
	Player mHostPlayer;

	//-----------------------------------------------------------------------//
	// Object: mLock
	//
	// The recursive mutex that provides thread-safety.
	//
	mutable wxMutex mLock;
};

//---------------------------- PROTOTYPES       -----------------------------//

