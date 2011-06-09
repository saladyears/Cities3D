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
#include <wx/datstrm.h>

//---------------------------- USER INCLUDES    -----------------------------//
#include "Data.h"
#include "Player.h"
#include "PlayerGame.h"
#include "TileObject.h"
#include "ModelExport.h"

//---------------------------- DEFINES          -----------------------------//
namespace
{
	// max number of players in one game
	const wxInt32 MAX_PLAYERS = 6;
}

//---------------------------- TYPEDEFS         -----------------------------//
class AggregatorObject;
typedef boost::shared_ptr<AggregatorObject> AggregatorObjectPtr;

class HexObject;
typedef boost::shared_ptr<HexObject> HexObjectPtr;

typedef std::vector<TileObjectPtr> Board;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: Game
// 
// Data class that stores game data.  Game data consists of the <PlayerGames>
// for the players in the game, and the board, made up of <TileObjects>. The
// Game data also stores a hash of <HexObjects> that are special <GameObjects>
// that reside directly on the board, such as the robber, pirate, and merchant.
//
// Derived From:
//     <Data>
//
// Project:
//     <ModelCore>
//
// Include:
//     Game.h
//
class MODEL_EXPORT_SPEC Game : public Data
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: Game
	//
	// The Game constructor.  Initializes default data values
	// that all games use.
	//
	Game();
	
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: numPlayers
	//
	// Returns the number of <Players> in the game.
	//
	// Returns:
	//     The number of <Players> in the game.
	//
	inline wxInt32 numPlayers() const {return mPlayers.size();}

	//-----------------------------------------------------------------------//
	// Function: player
	//
	// Returns the <Player> in the <PlayerGame> at the given
	// index.
	//
	// Parameters:
	//     player - The index of the player to return.
	//
	// Returns:
	//     A const reference to the <Player> at the given index.
	//
	inline const Player &player(const wxInt32 player) const;

	//-----------------------------------------------------------------------//
	// Function: playerGame
	//
	// Returns the <PlayerGame> at the given index.
	//
	// Parameters:
	//     player - The index of the player to return.
	//
	// Returns:
	//     A const reference to the <PlayerGame> at the given index.
	//
	inline const PlayerGame &playerGame(const wxInt32 player) const;

	//-----------------------------------------------------------------------//
	// Function: playerGame
	//
	// Returns the <PlayerGame> at the given index.
	//
	// Parameters:
	//     player - The index of the player to return.
	//
	// Returns:
	//     A reference to the <PlayerGame> at the given index.
	//
	inline PlayerGame &playerGame(const wxInt32 player);

	//-----------------------------------------------------------------------//
	// Function: numTiles
	//
	// Returns the number of <TileObjects> in the board.
	//
	// Returns:
	//     The number of <TileObjects> in the board.
	//
	inline wxInt32 numTiles() const {return mBoard.size();}

	//-----------------------------------------------------------------------//
	// Function: tile
	//
	// Returns the <TileObject> at the given index.
	//
	// Parameters:
	//     tile - The index of the <TileObject> to return.
	//
	// Returns:
	//     A const reference to the <TileObject> at the given index.
	//
	inline const TileObjectPtr &tile(const wxInt32 tile) const;

	//-----------------------------------------------------------------------//
	// Function: tile
	//
	// Returns the <TileObject> at the given index.
	//
	// Parameters:
	//     tile - The index of the <TileObject> to return.
	//
	// Returns:
	//     A reference to the <TileObject> at the given index.
	//
	inline TileObjectPtr &tile(const wxInt32 tile);

	//-----------------------------------------------------------------------//
	// Function: buildBoard
	//
	// Clears the board array of <TileObjects> and copies all <TileObjects> 
	// within the iterator range into the board array.
	//
	// Parameters:
	//     itBegin - The start iterator of the <TileObjects> to copy.
	//     itEnd - The end iterator of the <TileObjects> to copy.
	//
	void buildBoard(std::vector<TileObject>::const_iterator itBegin, 
		std::vector<TileObject>::const_iterator itEnd);

	//-----------------------------------------------------------------------//
	// Function: getGameObjects
	//
	// Appends all <TileObjects> and <HexObjects> the Game maintains
	// to the given list.
	//
	// Parameters:
	//     objects - The container of <GameObjects> to append to.
	//
	void getGameObjects(std::list<GameObjectPtr> &objects) const;

	//-----------------------------------------------------------------------//
	// Function: setHexObject
	//
	// Sets the given <HexObject> at the given key in the
	// hash of <HexObjects>.
	//
	// Parameters:
	//     key - The key in the hash the <HexObject> belongs at.
	//     object - The <HexObject> to set.
	//
	void setHexObject(const HashString &key, const HexObjectPtr &object);

	//-----------------------------------------------------------------------//
	// Function: getHexObject
	//
	// Returns the <HexObject> at the given key.  Logs an error
	// to the screen if no <HexObject> exists at the key.
	//
	// Parameters:
	//     key - The key in the hash to look up.
	//
	// Returns:
	//     A const reference to the <HexObject>.  May be empty if no
	//     <HexObject> exists at the given key.
	//
	const HexObjectPtr &getHexObject(const HashString &key);

	typedef std::vector<HexObjectPtr> HexObjectArray;

	//-----------------------------------------------------------------------//
	// Function: getAllHexObjects
	//
	// Fills a container with all <HexObjects> the game has.
	//
	// Parameters:
	//     objects - [out] The container to fill.
	//
	void getAllHexObjects(HexObjectArray &objects) const;

	//-----------------------------------------------------------------------//
	// Function: load
	//
	// Loads the Game from the given input stream.  Uses the 
	// given <GameObjectEngine> to instantiate <GameObjects>, if provided.
	//
	// Parameters:
	//     stream - The input stream.
	//     engine - The <GameObjectEngine> used to instantiate loaded 
	//              <GameObjects>.  May be empty.
	//
	void load(wxDataInputStream &stream, const GameObjectEnginePtr &engine);

	//-----------------------------------------------------------------------//
	// Function: load
	//
	// Loads the Game from the given input stream.
	//
	// Parameters:
	//     stream - The input stream.
	//
	virtual void load(wxDataInputStream &stream) 
	{
		load(stream, GameObjectEnginePtr());
	}

	//-----------------------------------------------------------------------//
	// Function: save
	//
	// Saves the Game to the given output stream.
	//
	// Parameters:
	//     stream - The output stream.
	//
	virtual void save(wxDataOutputStream &stream) const;

	typedef std::vector<PlayerGame> PlayerGameArray;

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Section: Variables
	//

	//-----------------------------------------------------------------------//
	// Object: mPlayers
	//
	// The container of <PlayerGames> that make up the <Players>
	// in the Game.
	//
	PlayerGameArray mPlayers;

	//-----------------------------------------------------------------------//
	// Object: mBoard
	//
	// The container of <TileObjects> that make up the board for
	// the Game.
	//
	Board mBoard;

	WX_DECLARE_HASH_STRING_HASH_MAP(HexObjectPtr, HexObjectHash);

	//-----------------------------------------------------------------------//
	// Object: mHexObjects
	//
	// The hash container of <HexObjects> that store what <GameObjects>
	// are located on hexes on the board.
	//
	HexObjectHash mHexObjects;

	//allow access to one class only
	friend class Access;
};

//---------------------------- PROTOTYPES       -----------------------------//
inline const Player &Game::player(const wxInt32 player) const
{
	wxASSERT(mPlayers.size() > (wxUint32) player);

	return mPlayers[player].player();
}

inline const PlayerGame &Game::playerGame(const wxInt32 player) const
{
	wxASSERT(mPlayers.size() > (wxUint32) player);

	return mPlayers[player];
}

inline PlayerGame &Game::playerGame(const wxInt32 player)
{
	wxASSERT(mPlayers.size() > (wxUint32) player);

	return mPlayers[player];
}

inline const TileObjectPtr &Game::tile(const wxInt32 tile) const
{
	wxASSERT(numTiles() > tile);

	return mBoard[tile];
}

inline TileObjectPtr &Game::tile(const wxInt32 tile)
{
	wxASSERT(numTiles() > tile);

	return mBoard[tile];
}
