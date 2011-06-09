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
#include <wx/cmdline.h>

//---------------------------- USER INCLUDES    -----------------------------//
#include "ICommand.h"
#include "CommandProcesser.h"
#include "MapDatabase.h"
#include "Game.h"
#include "Player.h"
#include "Map.h"
#include "Controller.h"
#include "GameEngine.h"
#include "IRuleEngine.h"
#include "INetworkEngine.h"
#include "IPluginEngine.h"
#include "IOptionEngine.h"
#include "DataObject.h"
#include "Access.h"
#include "network/Network.h"
#include "json_spirit_reader.h"

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- STATICS          -----------------------------//
namespace
{
	const wxString sPlayers[] = 
	{
		swHostPlayer, 
		swOpponent1, 
        swOpponent2, 
        swOpponent3, 
        swOpponent4, 
        swOpponent5, 
	};

	// A host command needs to look something like this:
	//
	// -host <ip address> -hostport <port> -map <internal map name> 
	//   -hostplayer <player:id> -opponent1 <player:id> -opponent2 <player:id>
	//   -opponent3 <player:id> -opponent4 <player:id> -opponent5 <player:id>
	//   -options <option string>
	//
	// Assumptions about player names:
	//	 - one single word, no spaces
	//   - no : characters
	//   - id is not zero
}

class HostCommand 
	: public ICommand
	, public Access
{
public:
	virtual ~HostCommand() {}

	virtual void PrepareParser(wxCmdLineParser& parser) const
	{
		parser.AddOption(swHost, wxEmptyString, wxEmptyString,
			wxCMD_LINE_VAL_STRING, wxCMD_LINE_NEEDS_SEPARATOR);
		parser.AddOption(swHostPort, wxEmptyString, wxEmptyString,
			wxCMD_LINE_VAL_STRING, wxCMD_LINE_NEEDS_SEPARATOR);
		parser.AddOption(swMap, wxEmptyString, wxEmptyString,
			wxCMD_LINE_VAL_STRING, wxCMD_LINE_NEEDS_SEPARATOR);
		parser.AddOption(swHostGameID, wxEmptyString, wxEmptyString,
			wxCMD_LINE_VAL_STRING, wxCMD_LINE_NEEDS_SEPARATOR);

		for(size_t i = 0; i < MAX_PLAYERS; ++i)
		{
			parser.AddOption(sPlayers[i], wxEmptyString, wxEmptyString,
				wxCMD_LINE_VAL_STRING, wxCMD_LINE_NEEDS_SEPARATOR);
		}

		parser.AddOption(swOptions, wxEmptyString, wxEmptyString,
			wxCMD_LINE_VAL_STRING, wxCMD_LINE_NEEDS_SEPARATOR);

		// Timer options.
		parser.AddOption(swTurnTime, wxEmptyString, wxEmptyString,
			wxCMD_LINE_VAL_STRING, wxCMD_LINE_NEEDS_SEPARATOR);

		parser.AddOption(swPassiveTime, wxEmptyString, wxEmptyString,
			wxCMD_LINE_VAL_STRING, wxCMD_LINE_NEEDS_SEPARATOR);

		parser.AddOption(swExtraTimers, wxEmptyString, wxEmptyString,
			wxCMD_LINE_VAL_STRING, wxCMD_LINE_NEEDS_SEPARATOR);


	}

	virtual void Execute(const wxCmdLineParser& parser, GamePtr& game, 
		bool inStartup)
	{
		// Hosts need the following to start up the game:
		//
		// 1.  Host IP address
		// 2.  Host port
		// 3.  Map
		// 4.  Game ID
		// 5.  Player name
		// 6.  Opponent names
		// 7.  Options

		// Gather all of the information needed.
		wxString host, port, mapName, gameID, player, opponent1, opponent2, 
			opponent3, opponent4, opponent5, options, turnTime, passiveTime,
			extraTimers;
		
		parser.Found(swHost, &host);
		parser.Found(swHostPort, &port);
		parser.Found(swMap, &mapName);
		parser.Found(swHostGameID, &gameID);
		parser.Found(swOptions, &options);
		parser.Found(swTurnTime, &turnTime);
		parser.Found(swPassiveTime, &passiveTime);
		parser.Found(swExtraTimers, &extraTimers);

		// Read in and create players.
		std::vector<Player> players;

		bool foundHost = false;

		for(wxInt32 i = 0; i < MAX_PLAYERS; ++i)
		{
			wxString value;
			if(true == parser.Found(sPlayers[i], &value))
			{
				wxString name;
				wxInt32 id;

				if(true == Network::SplitNameAndId(value, name, id))
				{
					// Create the player.
					Player player(name, id);
					players.push_back(player);

					if(0 == i)
					{
						foundHost = true;
					}
				}
			}
		}

		// We must have a host ip, port, map, and player.
		if( (false == host.IsEmpty()) &&
			(false == port.IsEmpty()) &&
			(false == mapName.IsEmpty()) &&
			(false == gameID.IsEmpty()) &&
			(true == foundHost))
		{
			// Load the map and reset the game.
			wxInt32 mapIndex = MapDatabase::get().FindMapIndexByName(mapName);
            GameEngine::get().LoadMap(mapIndex);

			game = GamePtr(new Game);
			Access::SetGame(game);

			// Set the internal game id.
			long id;
			gameID.ToLong(&id);
			gameData<wxInt32>(shID) = id;

			// This is a CitiesOnline game.
			gameData<wxInt32>(shCitiesOnline) = TRUE;

			// Make sure the network engine is loaded.
			PLUGIN.LoadRuleset(swNetworkRules, swNetwork);

			// If we have any options, tell the option engine about them.
			if(false == options.IsEmpty())
			{
				// Replace any %20s in the string with spaces.
				Network::FixNetworkString(options, options);

				// Parse the JSON string.
				std::string str(options.char_str());

                using namespace json_spirit;

                Value value;
                read(str, value);
                const Object& top = value.get_obj();

                for(Object::const_iterator it = top.begin(); it != top.end(); ++it)
                {
                    const Pair& pair = *it;
                    const std::string& m = pair.name_;
                    wxString module(m.c_str(), wxConvLibc, m.size());
					wxString ruleset;
                    wxString element;
                    wxString value;

                    // If the second part is an object, we have something with UI.
                    if( obj_type == pair.value_.type() )
                    {
                        const Object& obj = pair.value_.get_obj();
                        wxASSERT(1 == obj.size());

                        // Get the ruleset.
                        const Pair& val = *obj.begin();
                        const std::string& r = val.name_;
                        ruleset = wxString(r.c_str(), wxConvLibc, r.size());

                        // Get the UI setting.
                        const Object& ui = val.value_.get_obj();
                        wxASSERT(1 == ui.size());

                        // Get the UI value.
                        const Pair& u = *ui.begin();
                        const std::string& e = u.name_;
                        element = wxString(e.c_str(), wxConvLibc, e.size());

                        const std::string& v = u.value_.get_str();
                        value = wxString(v.c_str(), wxConvLibc, v.size());
                    }
                    else
                    {
                        const std::string& r = pair.value_.get_str();
                        ruleset = wxString(r.c_str(), wxConvLibc, r.size());
                    }

					OPTION.SelectOption(module, ruleset);

                    if( (false == element.IsEmpty()) &&
                        (false == value.IsEmpty()))
                    {
                        // Since we have no UI, and we're not going to load any
                        // for options, we need to store off this element and
                        // value for assignment during RuleInitialize.
                        CommandProcesser::get().StoreUISetting(element, value);
                    }
                }
			}

			// Set the colors for the game.
			std::vector<ColorType> colors;
			colors.resize(CR_SIZE);
			for(wxInt32 i = 0; i < CR_SIZE; ++i)
			{
				colors[i] = static_cast<ColorType>(i);
			}

			// Randomize them.
			srand(time(NULL));
			std::random_shuffle(colors.begin(), colors.end());

			// Add each of the players.
			for(size_t i = 0; i < players.size(); ++i)
			{
				// Assign a random color.
				ColorType color = colors[i];
					
				// Add to the game.
				PlayerGame playerGame(players[i]);
				playerGame.color(color);
				playerGames().push_back(playerGame);

				// Immediately set the player as the null player for the
				// purposes of joining the game over the network.
				game->playerGame(i).newPlayer(NullPlayer);
			}

			long turn_time = -1;
			long passive_time = -1;
			long extra_timers = 0;

			if(false == turnTime.IsEmpty())
			{
				turnTime.ToLong(&turn_time);
			}
			
			if(false == passiveTime.IsEmpty())
			{
				passiveTime.ToLong(&passive_time);
			}

			if(false == extraTimers.IsEmpty())
			{
				extraTimers.ToLong(&extra_timers);
			}

			// Set up game values.
			gameData<wxInt32>(shTurnTime) = turn_time;
			gameData<wxInt32>(shPassiveTime) = passive_time;
			gameData<wxInt32>(shIgnoreExtraTimers) = FALSE;

			for(wxInt32 i = 0; i < numPlayers(); ++i)
			{
				playerGameData<wxInt32>(shExtraTimerStep, i) = 1;
				playerGameData<wxInt32>(shExtraTimers, i) = extra_timers;
				playerGameData<wxInt32>(shPassiveTurnTimer, i) = 0;
				playerGameData<wxInt32>(shPassiveQuit, i) = 0;
			}

			const Player& hostPlayer = players[0];

			// Start the network engine.
			wxIPV4address addr;
			addr.Hostname(host);
			addr.Service(port);
			NET.StartServer(addr, hostPlayer);

			RULE.Execute(shRuleStartHostCommandGame, DataObject(hostPlayer));

			// Send the synchronize command to the site to tell it we got the 
			// host command.  It's easiest to format this as a wxString and 
			// then convert it back to std::string for the command, since we
			// know there are no Unicode characters in it.
			HashString command = HashString::Format(shStartCmd.c_str(), id);
			Controller::get().Transmit(shEventSynchronize, command);
		}
	}
};

REGISTER_COMMAND(HostCommand, host);

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
