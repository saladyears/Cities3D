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
#include "ModelExport.h"

//---------------------------- DEFINES          -----------------------------//
#define NullPlayer	Player::Null()

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//
class wxDataInputStream;
class wxDataOutputStream;

//---------------------------------------------------------------------------//
// Class: Player
// 
// Data class that stores player data.  Player data consists of all data that 
// allows for unique identification of players in the system, such as ID and 
// name, as well as other data that specifies specific preferences and player-
// specific settings for that player.
//
// Derived From:
//     <Data>
//
// Project:
//     <ModelCore>
//
// Include:
//     Player.h
//
class MODEL_EXPORT_SPEC Player
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: Player
	//
	// The default Player constructor.  
	//
	Player();

	//-----------------------------------------------------------------------//
	// Constructor: Player
	//
	// The Player constructor.  
	//
	// Parameters
	//     name - The player's name.
	//
	Player(const wxString& name);

	//-----------------------------------------------------------------------//
	// Constructor: Player
	//
	// The Player constructor.  
	//
	// Parameters
	//     name - The player's name.
	//     id - The players' id.
	//
	Player(const wxString& name, wxInt32 id);
	
	//-----------------------------------------------------------------------//
	// Group: Static Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Null
	//
	// Returns the one and only null Player.  The null Player is a way of
	// saying "this player is not a player".
	//
	// Returns:
	//     A Player that is the null Player.
	//
	static const Player &Null();

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Id
	//
	// Returns the id of the player.
	//
	wxInt32 Id() const
	{
		return mId;
	}

	//-----------------------------------------------------------------------//
	// Function: Name
	//
	// Returns the name of the player.
	//
	const wxString& Name() const
	{
		return mName;
	}

	//-----------------------------------------------------------------------//
	// Group: Operators
	//

	//-----------------------------------------------------------------------//
	// Function: operator==
	//
	// Compares another Player to the Player to see if they are the same.  They
	// are the same if their IDs are the same.
	//
	// Parameters:
	//     player - The Player to compare.
	//
	// Returns:
	//     A bool that is true if the players are the same and false if they 
	//     are different.
	//
	bool operator ==(const Player &player) const;

	//-----------------------------------------------------------------------//
	// Function: operator!=
	//
	// Compares another Player to the Player to see if they are not the same.
	// They are not the same if their IDs are not the same.
	//
	// Parameters:
	//     player - The Player to compare.
	//
	// Returns:
	//     A bool that is true if the players are different, and false if they
	//     are the same.
	//
	bool operator !=(const Player &player) const;

	//-----------------------------------------------------------------------//
	// Function: operator==
	//
	// Compares the given id to the Player to see if they are the same.  They 
	// are the same if the Player's id is the same as the given id.
	//
	// Parameters:
	//     id - The id to compare.
	//
	// Returns:
	//     A bool that is true if the players are the same and false if they 
	//     are different.
	//
	bool operator ==(wxInt32 id) const;

	//-----------------------------------------------------------------------//
	// Function: operator<
	//
	// Compares the given player to the Player and returns if the Player is 
	// less than the given player.
	//
	// Parameters:
	//     player - The Player to compare.
	//
	// Returns:
	//     A bool that is true if the Player is less than the given Player and
	//     false if not.
	//
	bool operator <(const Player &player) const;

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Int: mId
	//
	// The id of the player.
	//
	wxInt32 mId;

	//-----------------------------------------------------------------------//
	// String: mName
	//
	// The name of the player.
	//
	wxString mName;

	friend MODEL_EXPORT_SPEC wxDataOutputStream& operator<<(
		wxDataOutputStream& out, const Player& player);
	friend MODEL_EXPORT_SPEC wxDataInputStream& operator>>(
		wxDataInputStream& in, Player& player);
};

//---------------------------- PROTOTYPES       -----------------------------//
MODEL_EXPORT_SPEC wxDataOutputStream& operator<<(wxDataOutputStream& out, 
	const Player& player);
MODEL_EXPORT_SPEC wxDataInputStream& operator>>(wxDataInputStream& in, 
	Player& player);
