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
#include "Singleton.h"
#include "Player.h"
#include "ModelExport.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: PlayerDatabase
// 
// Singleton class that manages the player database on disk.  The
// player database stores all <Players> the user may choose to play as.
// The PlayerDatabase provides functions for loading, modifying, and 
// deleting <Players>.
//
// Derived From:
//     <Singleton>
//
// Project:
//     <ModelCore>
//
// Include:
//     PlayerDatabase.h
//
class MODEL_EXPORT_SPEC PlayerDatabase : public Singleton<PlayerDatabase>
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: NumPlayers
	//
	// Returns the number of <Players> in the player database.
	//
	// Returns:
	//     The number of <Players> in the player database.
	//
	size_t NumPlayers();

	//-----------------------------------------------------------------------//
	// Function: LoadMap
	//
	// Retrieves the <Player> at the given index from the map
	// database.
	//
	// Parameters:
	//     index - The index in the player database of the <Player>
	//             to load.
	//
	// Returns:
	//     A const reference to <Player> to retrieve.
	//
	const Player &GetPlayer(const size_t index) const;
	
	//-----------------------------------------------------------------------//
	// Function: AddPlayer
	//
	// Adds the given <Player> to the PlayerDatabase.
	//
	// Parameters:
	//     player - The <Player> to add.
	//
	// Returns:
	//     The index in the players database where the <Player> is saved.
	//
	wxUint32 AddPlayer(const Player &player);

	//-----------------------------------------------------------------------//
	// Function: UpdatePlayer
	//
	// Updates the <Player> at the given index in the player
	// database with the given data.
	//
	// Parameters:
	//     index - The index in the player database of the <Player>
	//             to update.
	//     player - The <Player> data to update with.
	//
	void UpdatePlayer(const size_t index, const Player &player);

	//-----------------------------------------------------------------------//
	// Function: DeletePlayer
	//
	// Removes the <Player> at the given index in the player
	// database.
	//
	// Parameters:
	//     index - The index in the player database of the <Player>
	//             to delete.
	//
	void DeletePlayer(const size_t index);

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: PlayerDatabase
	//
	// The PlayerDatabase constructor.  Calls <InitializeDatabase>
	// to prepare the player database cache.
	//
	PlayerDatabase();

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: InitializeDatabase
	//
	// Creates a cache of the player database information for
	// quick lookup in the future.
	//
	void InitializeDatabase();

	friend class Singleton<PlayerDatabase>;
};

//---------------------------- PROTOTYPES       -----------------------------//
