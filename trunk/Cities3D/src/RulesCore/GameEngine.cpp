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
#include "stdwx.h"
#include "style.h"	//READ THIS BEFORE MAKING ANY CHANGES TO THIS FILE!!!

//---------------------------- SYSTEM INCLUDES  -----------------------------//

//---------------------------- USER INCLUDES    -----------------------------//
#include "GameEngine.h"
#include "Game.h"
#include "Map.h"
#include "Access.h"
#include "DataObject.h"
#include "PluginEngine.h"
#include "RuleEngine.h"
#include "ImageEngine.h"
#include "GameObjectEngine.h"
#include "DrawObjectEngine.h"
#include "SoundEngine.h"
#include "NetworkEngine.h"
#include "StatEngine.h"
#include "AnyEngine.h"
#include "KeyMapEngine.h"
#include "MapDatabase.h"
#include "GameDatabase.h"
#include "Controller.h"
#include "RNG.h"
#include "TimerManager.h"
#include "PlayerDatabase.h"
#include "UIEngine.h"
#include "OptionEngine.h"
#include "ICommand.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	// The Game Version.
	const wxInt32 C3D_VERSION_MAJOR		= 0;
	const wxInt32 C3D_VERSION_MINOR		= 9;
	const wxInt32 C3D_VERSION_PATCH		= 7;
}

struct GameEngine::Impl
{
	Impl() 
		: mPluginEngine(mRuleEngine, mGameObjectEngine, mDrawObjectEngine,
			mUIEngine, mOptionEngine, mKeyMapEngine)
		, mMapIndex(-1)
		, mLoadedGameIndex(-1)
		, mInStartup(false)
	{}

	RuleEngine mRuleEngine;
	GameObjectEngine mGameObjectEngine;
	DrawObjectEngine mDrawObjectEngine;
	SoundEngine mSoundEngine;
	AnyEngine mAnyEngine;
	KeyMapEngine mKeyMapEngine;
	NetworkEngine mNetworkEngine;
	UIEngine mUIEngine;
	OptionEngine mOptionEngine;
	PluginEngine mPluginEngine;
	StatEngine mStatEngine;

	// The index of the currently loaded map.
	wxInt32 mMapIndex;

	// The index of the currently loaded game.
	wxInt32 mLoadedGameIndex;

	// The currently loaded map.
	Map mMap;

	// The one and only game.
	GamePtr mpGame;

	// A flag that is true if we are looking for plugins.
	bool mInStartup;

	// A flag that is true if the app is closing.
	bool mInShutdown;

	void OnSkin(wxInt32)
	{
		// Force an update to all PlayerUIs to make sure they get new skin
		// colors.
		Controller::get().Transmit(shEventPlayerUI, DataObject(mpGame, -1));
	}
};

struct no_delete_engine
{
	void operator()(void *){}
};

//---------------------------- PUBLIC           -----------------------------//
IPluginEngine &GameEngine::plugin() const
{
	return mpImpl->mPluginEngine;
}

IRuleEngine &GameEngine::rule() const
{
	return mpImpl->mRuleEngine;
}

IGameObjectEngine &GameEngine::gameobject() const
{
	return mpImpl->mGameObjectEngine;
}

IDrawObjectEngine &GameEngine::drawobject() const
{
	return mpImpl->mDrawObjectEngine;
}

ISoundEngine &GameEngine::sound() const
{
	return mpImpl->mSoundEngine;
}

IAnyEngine &GameEngine::any() const
{
	return mpImpl->mAnyEngine;
}

IKeyMapEngine &GameEngine::keymap() const
{
	return mpImpl->mKeyMapEngine;
}

INetworkEngine &GameEngine::network() const
{
	return mpImpl->mNetworkEngine;
}

IStatEngine &GameEngine::stat() const
{
    return mpImpl->mStatEngine;
}

IUIEngine& GameEngine::ui() const
{
	return mpImpl->mUIEngine;
}

IOptionEngine& GameEngine::option() const
{
	return mpImpl->mOptionEngine;
}

const Map &GameEngine::map() const
{
	return mpImpl->mMap;
}

wxInt32 GameEngine::mapIndex() const
{
	return mpImpl->mMapIndex;
}

bool GameEngine::InStartup() const
{
	return mpImpl->mInStartup;
}

bool GameEngine::InShutdown() const
{
	return mpImpl->mInShutdown;
}

void GameEngine::ExecuteCommand(const wxCmdLineParser& parser, 
	const CommandPtr& command, bool inStartup)
{
	wxASSERT(command);

	command->Execute(parser, mpImpl->mpGame, inStartup);
}

void GameEngine::LoadMap(const wxInt32 index)
{
	//always reset the map when loading a new one
	mpImpl->mMap = Map();

	if(-1 != index)
	{
		MapDatabase::get().LoadMap(index, mpImpl->mMap);
	}

	mpImpl->mMapIndex = index;
}

void GameEngine::StartEngine()
{
	//first, stop anything that's currently going on
	StopEngine();

	// Create a new game.
	mpImpl->mpGame = GamePtr(new Game);
	
	// Set the game data for all Access derived objects.
	Access::SetGame(mpImpl->mpGame);

	// Make sure the game and databases are initialized.
	GameDatabase::get();
	MapDatabase::get();

	// Start the plugin database.
	mpImpl->mInStartup = true;
	mpImpl->mPluginEngine.Start();
	mpImpl->mInStartup = false;

    // Apply overrides in the keymap engine.
    mpImpl->mKeyMapEngine.Start();

	// Repool the RNG.
	RAND.repool();
}

void GameEngine::StopEngine(bool shutdown)
{
	mpImpl->mInShutdown = shutdown;

	// Stop the network engine.
	mpImpl->mNetworkEngine.Stop();

	// Stop all timers.
	TIME.Stop();

	// Clear the game data for all Access derived objects.
	Access::SetGame(GamePtr());

	// Reset the game and map data.
	mpImpl->mpGame.reset();
	mpImpl->mMap = Map();

	mpImpl->mMapIndex = -1;
	mpImpl->mLoadedGameIndex = -1;
	
	// Clear the image engine.
	IMAGE.Clear();

	// Clear the rest of the engines.
	mpImpl->mGameObjectEngine.Stop();
	mpImpl->mDrawObjectEngine.Stop();
	mpImpl->mRuleEngine.Stop();
	mpImpl->mStatEngine.Stop();
	mpImpl->mUIEngine.Stop();
	mpImpl->mOptionEngine.Stop();
	mpImpl->mAnyEngine.Stop();
	mpImpl->mPluginEngine.Stop();
}

void GameEngine::LoadGame(wxInt32 index)
{
	mpImpl->mLoadedGameIndex = index;

	GameDatabase::get().LoadGame(index, *(mpImpl->mpGame.get()), 
		GameObjectEnginePtr(&(mpImpl->mGameObjectEngine), 
		no_delete_engine()), mpImpl->mStatEngine);

	//load up the map that this game is using
	mpImpl->mMapIndex = MapDatabase::get().LoadMapByID(
		mpImpl->mpGame->read<wxInt32>(shMapID), mpImpl->mMap);

	Controller::get().Transmit(shEventResetCamera, false);

	// Network games require a different execution path.
	if(shNetwork == mpImpl->mpGame->read<HashString>(shMode))
	{
		// Find the host player and set ourselves as them.
		wxInt32 id = mpImpl->mpGame->read<wxInt32>(shHostID);

		bool found = false;
		Player player;
		size_t numPlayers = PlayerDatabase::get().NumPlayers();
		for(size_t i = 0; i < numPlayers; ++i)
		{
			const Player &thisPlayer = PlayerDatabase::get().GetPlayer(i);
			if(thisPlayer == id)
			{
				player = thisPlayer;
				found = true;
				break;
			}
		}

		if(true == found)
		{
			HashString ip = mpImpl->mpGame->read<HashString>(shHostIP);
			wxInt32 port = mpImpl->mpGame->read<wxInt32>(shHostPort);

			wxIPV4address addr;
			addr.Hostname(ip.wx_str());
			addr.Service(port);
			mpImpl->mNetworkEngine.StartServer(addr, player);

			mpImpl->mRuleEngine.Execute(shNetworkRuleRestartGame, 
				DataObject());
		}
		else
		{
			wxLogError(wxT("Unable to find host player in player database."));
		}
	}
	else
	{
		// Fire up the UI and restart the game.
		mpImpl->mRuleEngine.Execute(shRulePrepareUI, DataObject());
		mpImpl->mRuleEngine.Execute(shRuleRestartGame, DataObject());	
	}
}

void GameEngine::SaveGame()
{
	wxASSERT(mpImpl->mpGame);

	// If the game is complete, remove it from the incomplete game database.
	if(shComplete == mpImpl->mpGame->read<HashString>(shState))
	{
		if(-1 != mpImpl->mLoadedGameIndex)
		{
			GameDatabase::get().DeleteGame(mpImpl->mLoadedGameIndex);
		}
	}
	else
	{
		// Get all the rulesets that are loaded.
		PluginEngine::StringPairArray array;
		mpImpl->mPluginEngine.GetLoadedRuleSets(array);

		StringPairArray options;
		mpImpl->mOptionEngine.GetSelectedOptions(options);

		// If this is the first time this game has been saved (meaning,
		// the current load game index is -1, and not an actual index),
		// then just save the game to the database.
		if(-1 == mpImpl->mLoadedGameIndex)
		{
			mpImpl->mLoadedGameIndex = 
				GameDatabase::get().SaveGame(*(mpImpl->mpGame.get()), array, 
				options, mpImpl->mStatEngine);
		}
		// Otherwise, update the game in the database at that index.
		else
		{
			mpImpl->mLoadedGameIndex = 
				GameDatabase::get().UpdateGame(mpImpl->mLoadedGameIndex, array,
				options, mpImpl->mStatEngine, *(mpImpl->mpGame.get()));
		}
	}
}

const wxString GameEngine::Version()
{
	return wxString::Format(swVersion.c_str(), C3D_VERSION_MAJOR, 
		C3D_VERSION_MINOR, C3D_VERSION_PATCH);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
GameEngine::GameEngine()
: mpImpl(new Impl)
{
	Controller::get().AddReceiver(shEventResetGame, 
		&GameEngine::OnResetGame, this);

	Controller::get().AddReceiver(shEventSkin, &GameEngine::Impl::OnSkin,
		mpImpl.get());
}

GameEngine::~GameEngine()
{
	Controller::get().RemoveReceiver(shEventResetGame, 
		&GameEngine::OnResetGame, this);

	Controller::get().RemoveReceiver(shEventSkin, &GameEngine::Impl::OnSkin,
		mpImpl.get());
}

void GameEngine::OnResetGame(wxInt32)
{
	//create a new game and current map
	mpImpl->mpGame = GamePtr(new Game);
	mpImpl->mMap = Map();

	mpImpl->mMapIndex = -1;
	mpImpl->mLoadedGameIndex = -1;

	//set the game data for all Access derived objects
	Access::SetGame(mpImpl->mpGame);
}
