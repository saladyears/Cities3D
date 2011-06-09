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
#include <list>
#include <map>

//---------------------------- USER INCLUDES    -----------------------------//
#include "Data.h"
#include "Player.h"
#include "ModelExport.h"
#include "ColorInfoData.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class CornerObject;
typedef boost::shared_ptr<CornerObject> CornerObjectPtr;

class SideObject;
typedef boost::shared_ptr<SideObject> SideObjectPtr;

class IGameObject;
typedef boost::shared_ptr<IGameObject> GameObjectPtr;

class IGameObjectEngine;
typedef boost::shared_ptr<IGameObjectEngine> GameObjectEnginePtr;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: PlayerGame
// 
// Data class that stores game data for one player.  Player game data
// consists of the <Player>, all data related to that player (such as points, 
// stock items, cards, etc.), their color, and any <CornerObjects> and
// <SideObjects> that they may have ownership of on the board.
//
// Derived From:
//     <Data>
//
// Project:
//     <ModelCore>
//
// Include:
//     PlayerGame.h
//
class MODEL_EXPORT_SPEC PlayerGame : public Data
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: PlayerGame
	//
	// The Game PlayerGame.  Initializes default data values
	// that all data for players in games use.
	//
	// Parameters:
	//     player - The <Player> the PlayerGame represents.
	//
	PlayerGame(const Player &player = Player(), 
		const ColorType color = CR_WHITE);
	
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: player
	//
	// Returns the <Player> in the PlayerGame.
	//
	// Returns:
	//     A const reference to the <Player>.
	//
	const Player &player() const {return mPlayer;}

	typedef std::map<Player, size_t> PlayerTurnMap;
	typedef std::map<size_t, Player, std::greater<size_t> > TurnPlayerMap;
	
	//-----------------------------------------------------------------------//
	// Function: originalPlayers
	//
	// Returns the map of original <Players> in the PlayerGame, sorted by
	// turns played.
	//
	// Parameters:
	//      originals - The sorted original players.
	//
	void originalPlayers(TurnPlayerMap &originals) const;

	//-----------------------------------------------------------------------//
	// Function: newPlayer
	//
	// Sets a new <Player> in the PlayerGame.  If the player is not the null
	// Player, they are appended to the list of original players (if they
	// aren't in it already).
	//
	// Parameters:
	//     player - The new player of this game.
	//
	void newPlayer(const Player &player);

	//-----------------------------------------------------------------------//
	// Function: incrementTurn
	//
	// Increments the turn count for the current <Player> in the original 
	// player map.
	//
	void incrementTurn();

	//-----------------------------------------------------------------------//
	// Function: color
	//
	// Returns the color of the player in the PlayerGame.
	//
	// Returns:
	//     The color of the player.
	//
	ColorType color() const {return mColor;}

	//-----------------------------------------------------------------------//
	// Function: color
	//
	// Sets the color of the player in the PlayerGame.
	//
	// Parameters:
	//     color - The color to set.
	//
	void color(const ColorType color) {mColor = color;}

	//-----------------------------------------------------------------------//
	// Function: getGameObjects
	//
	// Appends all <CornerObjects> and <SideObjects> the PlayerGame maintains
	// to the given list.
	//
	// Parameters:
	//     objects - The container of <GameObjects> to append to.
	//
	void getGameObjects(std::list<GameObjectPtr> &objects) const;

	typedef std::vector<CornerObjectPtr> CornerObjectArray;

	//-----------------------------------------------------------------------//
	// Function: getCornerObjects
	//
	// Returns a container of all <CornerObjects> in the hash
	// at the given key.
	//
	// Parameters:
	//     key - The key in the hash to look up.
	//
	// Returns:
	//     A const reference to the container of <CornerObjects> at the
	//     given key.
	//
	const CornerObjectArray &getCornerObjects(const HashString &key);

	//-----------------------------------------------------------------------//
	// Function: getAllCornerObjects
	//
	// Fills a container with all <CornerObjects> the player has.
	//
	// Parameters:
	//     objects - [out] The container to fill.
	//
	void getAllCornerObjects(CornerObjectArray &objects) const;

	//-----------------------------------------------------------------------//
	// Function: addCornerObject
	//
	// Appends the given <CornerObject> at the given key in the
	// hash of <CornerObjects>.
	//
	// Parameters:
	//     key - The key in the hash the <CornerObject> belongs at.
	//     object - The <CornerObject> to append.
	//
	void addCornerObject(const HashString &key, const CornerObjectPtr &object);

	//-----------------------------------------------------------------------//
	// Function: removeCornerObject
	//
	// Removes the given <CornerObject> from the given key in the
	// hash of <CornerObjects>.
	//
	// Parameters:
	//     key - The key in the hash the <CornerObject> is at.
	//     object - The <CornerObject> to remove.
	//
	void removeCornerObject(const HashString &key, const CornerObjectPtr &object);

	//-----------------------------------------------------------------------//
	// Function: countCornerObjects
	//
	// Returns the number of <CornerObjects> in the entire
	// <CornerObject> hash.
	//
	// Returns:
	//     The total number of <CornerObjects>.
	//
	size_t countCornerObjects() const;

	typedef std::vector<SideObjectPtr> SideObjectArray;

	//-----------------------------------------------------------------------//
	// Function: getSideObjects
	//
	// Returns a container of all <SideObjects> in the hash
	// at the given key.
	//
	// Parameters:
	//     key - The key in the hash to look up.
	//
	// Returns:
	//     A const reference to the container of <SideObjects> at the
	//     given key.
	//
	const SideObjectArray &getSideObjects(const HashString &key);

	//-----------------------------------------------------------------------//
	// Function: getAllSideObjects
	//
	// Fills a container with all <SideObjects> the player has.
	//
	// Parameters:
	//     objects - [out] The container to fill.
	//
	void getAllSideObjects(SideObjectArray &objects) const;

	//-----------------------------------------------------------------------//
	// Function: addSideObject
	//
	// Appends the given <SideObject> at the given key in the
	// hash of <SideObjects>.
	//
	// Parameters:
	//     key - The key in the hash the <SideObject> belongs at.
	//     object - The <SideObject> to append.
	//
	void addSideObject(const HashString &key, const SideObjectPtr &object);

	//-----------------------------------------------------------------------//
	// Function: removeSideObject
	//
	// Removes the given <SideObject> from the given key in the
	// hash of <SideObjects>.
	//
	// Parameters:
	//     key - The key in the hash the <SideObject> is at.
	//     object - The <SideObject> to remove.
	//
	void removeSideObject(const HashString &key, const SideObjectPtr &object);

	//-----------------------------------------------------------------------//
	// Function: load
	//
	// Loads the PlayerGame from the given input stream.  Uses the 
	// given <GameObjectEngine> to instantiate <GameObjects>, if provided.
	//
	// Parameters:
	//     stream - The input stream.
	//     engine - The <GameObjectEngine> used to instantiate loaded 
	//              <GameObjects>. May be empty.
	//
	void load(wxDataInputStream &stream, const GameObjectEnginePtr &engine);
	
	//-----------------------------------------------------------------------//
	// Function: load
	//
	// Loads the PlayerGame from the given input stream.
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
	// Saves the PlayerGame to the given output stream.
	//
	// Parameters:
	//     stream - The output stream.
	//
	virtual void save(wxDataOutputStream &stream) const;

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: WriteGameObjects
	//
	// Writes all of the <CornerObjects> and <SideObjects>
	// in the PlayerGame to the given stream.
	//
	// Parameters:
	//     stream - The output stream.
	//
	void WriteGameObjects(wxDataOutputStream &stream) const;

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Object: mPlayer
	//
	// The <Player> the PlayerGame represents.
	//
	Player mPlayer;

	//-----------------------------------------------------------------------//
	// Object: mOriginalPlayers
	//
	// Because a player may be replaced by a spectator in a game, each 
	// PlayerGame must remember all of its original players, and the number of
	// turns each has played, in case any of those players returns to the game 
	// to take back their spot.
	//
	PlayerTurnMap mOriginalPlayers;

	//-----------------------------------------------------------------------//
	// Object: mColor
	//
	// The color of the player in the PlayerGame.
	//
	ColorType mColor;

	WX_DECLARE_HASH_STRING_HASH_MAP(CornerObjectArray, CornerObjectHash);
	WX_DECLARE_HASH_STRING_HASH_MAP(SideObjectArray, SideObjectHash);

	//-----------------------------------------------------------------------//
	// Object: mCornerObjects
	//
	// The container of all <CornerObjects> that belong to 
	// the player.
	//
	CornerObjectHash mCornerObjects;

	//-----------------------------------------------------------------------//
	// Object: mSideObjects
	//
	// The container of all <SideObjects> that belong to 
	// the player.
	//
	SideObjectHash mSideObjects;

	friend class Access;
};

//---------------------------- PROTOTYPES       -----------------------------//
