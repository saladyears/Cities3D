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
#include "RulesExport.h"
#include "Game.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: Access
// 
// Provides read/write access to <Data>-derived classes. The Access class 
// allows for derived classes to access the internal data fields of the main 
// data classes that make up a game and board: <Game>, <PlayerGame>, and
// <TileObject>.  By using a common base class that abstracts the access, it is 
// possible to totally encapsulate the data from the View, so that the View has
// not even the slightest possibility of write-access to the main game data.  
// Only classes derived from Access (such as <Rule>, and <Logic>) are able to 
// alter the game data.
// 
// Project:
//     <RulesCore>
//
// Include:
//     Access.h
//
class RULES_EXPORT_SPEC Access
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Static Functions
	//

	//-----------------------------------------------------------------------//
	// Function: SetGame
	//
	// Sets the <Game> data all Access-derived classes
	// will use.
	//
	// Parameters:
	//     game - The <Game> data to use.
	//
	static void SetGame(const GamePtr &game);
	
	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: Access
	//
	// The Access destructor.  It is pure virtual because
	// this class must be derived from to be used.
	//
	virtual ~Access()=0;

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: numPlayers
	//
	// Returns the number of players in the <Game>.
	//
	// Returns:
	//     The number of players in the <Game>.
	//
	wxInt32 numPlayers() const
	{
		return GetGame()->numPlayers();
	}

	//-----------------------------------------------------------------------//
	// Function: numTiles
	//
	// Returns the number of tiles on the board in the <Game>.
	//
	// Returns:
	//     The number of tiles on the board in the <Game>.
	//
	wxInt32 numTiles() const
	{
		return GetGame()->numTiles();
	}

	//-----------------------------------------------------------------------//
	// Function: current
	//
	// Returns the index in the <Game>'s array of
	// <PlayerGames> of the current player.
	//
	// Returns:
	//     The index of the current player.
	//
	wxInt32 current() const
	{
		return GetGame()->data<wxInt32>(shCurrentPlayer);
	}

	//-----------------------------------------------------------------------//
	// Function: playerGame
	//
	// Returns the <PlayerGame> data of the player at the
	// given player index.  If the index is -1, the data for
	// the current player is returned.
	//
	// Parameters:
	//     player - The index of the player to retrieve.  If
	//              -1, the current player index is used.
	//
	// Returns:
	//     A reference to the <PlayerGame> data for the player
	//     at that index.
	//
	PlayerGame &playerGame(wxInt32 player = -1)
	{
		//use current player if desired
		if(-1 == player)
		{
			player = current();
		}

		return GetGame()->playerGame(player);
	}

	//-----------------------------------------------------------------------//
	// Function: playerGames
	//
	// Returns the std::vector of <PlayerGames> data of all players
	// in the game.  
	//
	// Returns:
	//     A reference to the std::vector of <PlayerGame> data
	//     stored in the <Game>.
	//
	Game::PlayerGameArray &playerGames()
	{
		return GetGame()->mPlayers;
	}

	//-----------------------------------------------------------------------//
	// Function: gameData
	//
	// Returns the data field at the given key within the <Data> of the <Game>.
	// Because it is returned as a reference, the data can be read from or 
	// written to.
	//
	// *NOTE*: The function calls the data<> member function of the <Data> 
	// class with no safety, meaning that if the given key does not exist, it 
	// will be created by this call to it.
	//
	// Parameters:
	//     key - The hash key of the field in the <Data> class.
	//
	// Returns:
	//     A reference to the data itself.  It may be read to
	//     or written from.
	//
	template <typename T> T& gameData(const HashString &key)
	{
		return GetGame()->template data<T>(key);
	}

	//-----------------------------------------------------------------------//
	// Function: playerGameData
	//
	// Returns the data field at the given key within the <Data> of the 
	// <PlayerGame> for the given player index.  If the index is -1, the data 
	// for the current player is used.  Because it is returned as a reference, 
	// the data can be read from or written to.  The playerGameData function is
	// a shortcut for direct access to a player's game data within the <Game>.
	//
	// *NOTE*: The function calls the data<> member function of the <Data> 
	// class with no safety, meaning that if the given key does not exist, it 
	// will be created by this call to it.
	//
	// Parameters:
	//     key - The hash key of the field in the <Data> class.
	//     player - The index of the player to retrieve.  If
	//              -1, the current player index is used.
	//
	// Returns:
	//     A reference to the data itself.  It may be read to
	//     or written from.
	//
	template <typename T> T& playerGameData(const HashString &key, 
		wxInt32 player = -1)
	{
		//use current player if desired
		if(-1 == player)
		{
			player = GetGame()->template data<wxInt32>(shCurrentPlayer);
		}

		return GetGame()->playerGame(player).template data<T>(key);
	}

	//-----------------------------------------------------------------------//
	// Function: tile
	//
	// Returns the data field for the given key within the
	// <Data> of the <TileObject> at the given tile index.
	// Because it is returned as a reference, the  data can 
	// be read from or written to.  The tile function
	// is a shortcut for direct access to a <TileObject>'s data
	// within the <Game>.
	//
	// Parameters:
	//     key - The hash key of the field in the <Data> class.
	//     tile - The index of the tile to retrieve.
	//
	// Returns:
	//     A reference to the data itself.  It may be read to
	//     or written from.
	//
	template <typename T> T& tile(const HashString &key, const wxInt32 tile)
	{
		return GetGame()->tile(tile)->template data<T>(key);
	}

	//-----------------------------------------------------------------------//
	// Function: tile
	//
	// Returns the <TileObject> at the given index.
	//
	// Parameters:
	//     tile - The index of the tile to retrieve.
	//
	// Returns:
	//     A reference to the <TileObject>.
	//
	TileObjectPtr &tile(const wxInt32 tile)
	{
		return GetGame()->tile(tile);
	}

	//-----------------------------------------------------------------------//
	// Function: eraseGameData
	//
	// Erases the given key from the <Game> data.
	//
	// Parameters:
	//     key - The key to erase.
	//
	template <typename T> void eraseGameData(const HashString &key)
	{
		GetGame()->template erase<T>(key);
	}

	//-----------------------------------------------------------------------//
	// Function: erasePlayerGameData
	//
	// Erases the given key from the <PlayerGame> data.
	//
	// Parameters:
	//     key - The key to erase.
	//     player - The player to erase it from.
	//
	template <typename T> void erasePlayerGameData(const HashString &key,
		wxInt32 player = -1)
	{
		if(-1 == player)
		{
			player = GetGame()->template data<wxInt32>(shCurrentPlayer);
		}

		GetGame()->playerGame(player).template erase<T>(key);
	}

	//-----------------------------------------------------------------------//
	// Function: GetGame
	//
	// Returns the current <Game> data.
	//
	// Returns:
	//     The current <Game> data.
	//
	GamePtr &GetGame() const;
};

//---------------------------- PROTOTYPES       -----------------------------//
inline Access::~Access() {}
