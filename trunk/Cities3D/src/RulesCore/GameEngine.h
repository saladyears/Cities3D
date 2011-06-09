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
#include "RulesExport.h"

//---------------------------- DEFINES          -----------------------------//
#define PLUGIN		GameEngine::get().plugin()
#define RULE		GameEngine::get().rule()
#define GAMEOBJECT	GameEngine::get().gameobject()
#define DRAWOBJECT	GameEngine::get().drawobject()
#define SOUND		GameEngine::get().sound()
#define ANYENGINE	GameEngine::get().any()
#define KEYMAP		GameEngine::get().keymap()
#define NET			GameEngine::get().network()
#define MAP			GameEngine::get().map()
#define MAPINDEX	GameEngine::get().mapIndex()
#define STAT        GameEngine::get().stat()
#define GUI			GameEngine::get().ui()
#define OPTION		GameEngine::get().option()
#define GAMEVERSION	GameEngine::get().Version()

//---------------------------- TYPEDEFS         -----------------------------//
class Map;

class Game;
typedef boost::shared_ptr<Game> GamePtr;

class IPluginEngine;
class IRuleEngine;
class IGameObjectEngine;
class IDrawObjectEngine;
class ISoundEngine;
class IAnyEngine;
class IKeyMapEngine;
class INetworkEngine;
class IStatEngine;
class IUIEngine;
class IOptionEngine;

class wxCmdLineParser;
class ICommand;
typedef boost::shared_ptr<ICommand> CommandPtr;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: GameEngine
// 
// The engine that drives the game.  The <Singleton> class GameEngine
// manages the <Game> itself.  It provides several sub-engines that
// outside objects have access to (through interfaces) that are
// responsible for different parts of the system.  GameEngine is the one
// single command and control center for running the game.  The application
// is only responsible for starting and stopping the GameEngine at
// application load and exit, and the GameEngine handles all
// other initialization and cleanup of <Game> and <Map> data and all 
// sub-engines.
//
// GameEngine only returns interfaces to the sub-engines by design.  This 
// allows the actual implementations of the interfaces used internally
// by GameEngine to change dramatically without having to recompile the
// entire project.
//
// Derived From:
//     <Singleton>
//
// Project:
//     <RulesCore>
//
// Include:
//     GameEngine.h
//

class RULES_EXPORT_SPEC GameEngine : public Singleton<GameEngine>
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: plugin
	//
	// Provides access to the <PluginEngine>.  A calling
	// object should use the PLUGIN macro as a shortcut
	// to this function.
	//
	// Returns:
	//     A reference to the <IPluginEngine> interface.
	//
	IPluginEngine &plugin() const;

	//-----------------------------------------------------------------------//
	// Function: rule
	//
	// Provides access to the <RuleEngine>.  A calling
	// object should use the RULE macro as a shortcut to
	// this function.
	//
	// Returns:
	//     A reference to the <IRuleEngine> interface.
	//
	IRuleEngine &rule() const;

	//-----------------------------------------------------------------------//
	// Function: gameobject
	//
	// Provides access to the <GameObjectEngine>.  A calling
	// object should use the GAMEOBJECT macro as a shortcut to
	// this function.
	//
	// Returns:
	//     A reference to the <IGameObjectEngine> interface.
	//
	IGameObjectEngine &gameobject() const;

	//-----------------------------------------------------------------------//
	// Function: drawobject
	//
	// Provides access to the <DrawObjectEngine>.  A calling
	// object should use the DRAWOBJECT macro as a shortcut to
	// this function.
	//
	// Returns:
	//     A reference to the <IDrawObjectEngine> interface.
	//
	IDrawObjectEngine &drawobject() const;

	//-----------------------------------------------------------------------//
	// Function: sound
	//
	// Provides access to the <SoundEngine>.  A calling
	// object should use the SOUND macro as a shortcut to
	// this function.
	//
	// Returns:
	//     A reference to the <ISoundEngine> interface.
	//
	ISoundEngine &sound() const;

	//-----------------------------------------------------------------------//
	// Function: any
	//
	// Provides access to the <AnyEngine>.
	//
	// Returns:
	//    A reference to the <IAnyEngine> interface.
	//
	IAnyEngine &any() const;

	//-----------------------------------------------------------------------//
	// Function: keymap
	//
	// Provides access to the <KeyMapEngine>.
	//
	// Returns:
	//     A reference to the <IKeyMapEngine> interface.
	//
	IKeyMapEngine &keymap() const;

	//-----------------------------------------------------------------------//
	// Function: network
	//
	// Provides access to the <NetworkEngine>.
	//
	// Returns:
	//     A reference to the <INetworkEngine> interface.
	//
	INetworkEngine &network() const;

	//-----------------------------------------------------------------------//
	// Function: stat
	//
	// Provides access to the <StatEngine>.
	//
	// Returns:
	//     A reference to the <IStatEngine> interface.
	//
	IStatEngine &stat() const;

	//-----------------------------------------------------------------------//
	// Function: ui
	//
	// Provides access to the <UIEngine>.
	//
	// Returns:
	//     A reference to the <IUIEngine> interface.
	//
	IUIEngine& ui() const;

	//-----------------------------------------------------------------------//
	// Function: option
	//
	// Provides access to the <OptionEngine>.
	//
	// Returns:
	//     A reference to the <IOptionEngine> interface.
	//
	IOptionEngine& option() const;

	//-----------------------------------------------------------------------//
	// Function: map
	//
	// Provides read-only access to the current <Map>.  A 
	// calling object should use the MAP macro as a shortcut
	// to this function.
	//
	// Returns:
	//     A const reference to the currently loaded <Map>.
	//
	const Map &map() const;

	//-----------------------------------------------------------------------//
	// Function: mapIndex
	//
	// Returns the index in the <MapDatabase> of the currently
	// loaded <Map>.
	//
	// Returns:
	//     The index in the <MapDatabase> of the currently loaded
	//     <Map>.  May be -1 if no <Map> is currently loaded.
	//
	wxInt32 mapIndex() const;

	//-----------------------------------------------------------------------//
	// Function: InStartup
	//
	// Returns if the application is in startup or not.  This is only set to
	// true while looking for plugins.
	//
	// Returns:
	//     A bool that is true if the application is in startup and false if 
	//     not.
	//
	bool InStartup() const;

	//-----------------------------------------------------------------------//
	// Function: InShutdown
	//
	// Returns if the application is in shutdown or not.  This is only set to
	// true when the application is exiting.
	//
	// Returns:
	//     A bool that is true if the application is in shutdown and false if 
	//     not.
	//
	bool InShutdown() const;

	//-----------------------------------------------------------------------//
	// Function: ExecuteCommand
	//
	// Executes the given command.
	//
	// Parameters:
	//     parser - The command line parser that contains command info.
	//     command - The command to execute.
	//     inStartup - A flag that is true if the application is in startup.
	//
	void ExecuteCommand(const wxCmdLineParser& parser, 
		const CommandPtr& command, bool inStartup);

	//-----------------------------------------------------------------------//
	// Function: LoadMap
	//
	// Loads the <Map> at the given <MapDatabase> index into
	// memory.
	//
	// Parameters:
	//     index - The index in the <MapDatabase> of the map to load.
	//
	void LoadMap(const wxInt32 index);

	//-----------------------------------------------------------------------//
	// Function: StartEngine
	//
	// Starts the GameEngine.  First calls <StopEngine> to
	// shut down anything currently happening.  Creates new
	// <Game> and <Map> objects (though no map is loaded from
	// the <MapDatabase>).  Sets the <Access> data to the
	// newly created <Game> object.  Starts the <PluginEngine>.
	//
	void StartEngine();

	//-----------------------------------------------------------------------//
	// Function: StopEngine
	//
	// Stops the GameEngine.  Resets the <Game> data in <Access> 
	// to empty.  Resets the internal implementation.
	//
	// Parameters:
	//     shutdown - A bool that is true if the application is shutting down.
	//
	void StopEngine(bool shutdown = false);
	
	//-----------------------------------------------------------------------//
	// Function: LoadGame
	//
	// Loads a <Game> from disk.  Because a certain ordering of
	// events must take place (i.e. the <RuleSets> must be loaded first,
	// then the OpenGL window created, then the <Game> data be loaded),
	// the <Event> eventLoadGameCallback is called during the load sequence.
	// This callback <Event> function will be called when the
	// <RuleSet> names for the game have been loaded from disk but before
	// the actual <Game> data is loaded.  In this way, the calling function
	// can do whatever <RuleSet> loading and OpenGL initialization is
	// required before the <Game> data is loaded.  Once the <Game> is loaded,
	// the proper <Map> for the <Game> is loaded via the <MapDatabase>.  Then
	// the eventResetCamera <Event> is triggered to readjust the View, and 
	// finally, the <RuleRestartGame> <IRule> is executed to get things going 
	// again.
	//
	// Parameters:
	//     index - The index in the <GameDatabase> of the <Game> to load.
	//
	void LoadGame(wxInt32 index);

	//-----------------------------------------------------------------------//
	// Function: SaveGame
	//
	// Saves the currently played <Game> to disk.  If the
	// <Game>'s State field is set to "Completed", the <Game> is
	// saved in the complete game database, otherwise it is
	// saved in the incomplete game database.
	//
	void SaveGame();

	//-----------------------------------------------------------------------//
	// Function: Version
	//
	// Returns the version number of the game.
	//
	// Returns:
	//     A string that is the version number of the game in 0.0.0 format.
	//
	const wxString Version();

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: GameEngine
	//
	// The GameEngine constructor.  Creates the private,
	// static <RuleEngine>, <DrawObjectEngine>, <ImageEngine>,
	// <SoundEngine>, and <PluginEngine>.  Seeds the random
	// number generator with the current timestamp.
	//
	GameEngine();

	//-----------------------------------------------------------------------//
	// Group: Destructors
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~GameEngine
	//
	// The GameEngine destructor.  Resets the private
	// static <RuleEngine>, <DrawObjectEngine>, <ImageEngine>,
	// <SoundEngine>, and <PluginEngine>.
	//
	~GameEngine();

	//-----------------------------------------------------------------------//
	// Group: Game Event Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnResetGame
	//
	// Resets the game data when needed.
	//
	void OnResetGame(wxInt32);

	//-----------------------------------------------------------------------//
	// Group: Variables
	//
	
	struct Impl;

	//-----------------------------------------------------------------------//
	// Pointer: mpImpl
	//
	// The internal implementation of the GameEngine.
	//
	boost::scoped_ptr<Impl> mpImpl;

	friend class Singleton<GameEngine>;
};

//---------------------------- PROTOTYPES       -----------------------------//

