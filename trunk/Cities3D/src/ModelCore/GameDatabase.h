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
#include "Game.h"
#include "ModelExport.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class IGameObjectEngine;
typedef boost::shared_ptr<IGameObjectEngine> GameObjectEnginePtr;

class ISerialize;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: GameDatabase
// 
// The GameDatabase is a Singleton class allows the system to save and load 
// incomplete games from disk.
//
// Derived From:
//     <Singleton>
//
// Project:
//     <ModelCore>
//
// Include:
//     GameDatabase.h
//
class MODEL_EXPORT_SPEC GameDatabase : public Singleton<GameDatabase>
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//
	
	//-----------------------------------------------------------------------//
	// Function: NumGames
	//
	// Returns the number of incomplete games in the database.
	//
	// Returns:
	//     The number of games in the incomplete game database.
	//
	size_t NumGames() const;
	
	typedef std::pair<wxString, wxString> StringPair;
	typedef std::vector<StringPair> StringPairArray;

	//-----------------------------------------------------------------------//
	// Function: SaveGame
	//
	// Saves the given <Game> to the incomplete game database.
	//
	// Parameters:
	//     game - The <Game> to save.
	//     rulesets - The container of module/ruleset pairs that are the
	//                given <Game> is being played with.
	//     options - The options the game is played with.
	//     stats - The game's stats.
	//     reload - A flag that is true if the database should be reloaded 
	//              after the delete and false if not.
	//
	// Returns:
	//     The index in the game database where the <Game> is saved.
	//
	size_t SaveGame(const Game &game, const StringPairArray &rulesets,
		const StringPairArray& options, const ISerialize& stats, const bool reload = true);

	//-----------------------------------------------------------------------//
	// Function: UpdateGame
	//
	// Updates a <Game> currently in the incomplete game database.
	//
	// Parameters:
	//     index - The current index of the <Game> in the incomplete game
	//             database.
	//     rulesets - The container of module/ruleset pairs that are the
	//                given <Game> is being played with.
	//     options - The options the game is played with.
	//     stats - The game's stats.
	//     game - The <Game> data to save over the old <Game>.
	//
	// Returns:
	//     The new index in the incomplete game database where the <Game>
	//     is saved.
	//
	size_t UpdateGame(const size_t index, const StringPairArray &rulesets, 
		const StringPairArray& options, const ISerialize& stats, const Game &game);

	//-----------------------------------------------------------------------//
	// Function: LoadGame
	//
	// Loads a <Game> from the disk from the incomplete game database.
	//
	// Parameters:
	//     index - The index of the <Game> to load from the incomplete game
	//             database.
	//     game - The <Game> to store the loaded data in.
	//     engine - The <IGameObjectEngine> that handles creating any 
	//              <GameObjects> needed by the loaded <Game>.  May be empty.
	//     stats - The <ISerialize> interface used to load the game's stats.
	//
	void LoadGame(const size_t index, Game &game, 
		const GameObjectEnginePtr &engine, ISerialize& stats);

	//-----------------------------------------------------------------------//
	// Function: DeleteGame
	//
	// Removes the <Game> at the given index from the incomplete game database.
	//
	// Parameters:
	//     index - The index of the <Game> to remove from the database.
	//     reload - A flag that is true if the database should be reloaded 
	//              after the delete and false if not.
	//
	void DeleteGame(const size_t index, const bool reload = true);

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: GameDatabase
	//
	// The GameDatabase constructor.  Calls <InitializeDatabase> to cache 
	// database information for faster lookup in the future.
	//
	GameDatabase();
	
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: InitializeDatabase
	//
	// Creates a cache of the database information for quick lookup in the 
	// future.
	//
	void InitializeDatabase();

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Array: mGames
	//
	// The array of incomplete game IDs in the database.
	//
	IntArray mGames;

	//-----------------------------------------------------------------------//
	// Array: mOffsets
	//
	// The array of offsets into the database file of incomplete games.
	//
	IntArray mOffsets;

	friend class Singleton<GameDatabase>;
};

//---------------------------- PROTOTYPES       -----------------------------//
