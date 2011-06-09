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
#include "Player.h"
#include "TransientConnectCommand.h"
#include "Controller.h"
#include "GameEngine.h"
#include "IPluginEngine.h"
#include "network/Network.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

// A connect command needs to look something like this:
//
// -connect <ip address> -connectport <port> -connectplayer <player:id> 
//   -connectgameid <game id>
//
// Assumptions about player names:
//	 - one single word, no spaces
//   - no : characters
//   - id is not zero
//
class ConnectCommand : public ICommand
{
public:
	virtual ~ConnectCommand() {}

	virtual void PrepareParser(wxCmdLineParser& parser) const
	{
		parser.AddOption(swConnect, wxEmptyString, wxEmptyString,
			wxCMD_LINE_VAL_STRING, wxCMD_LINE_NEEDS_SEPARATOR);
		parser.AddOption(swConnectPort, wxEmptyString, wxEmptyString,
			wxCMD_LINE_VAL_STRING, wxCMD_LINE_NEEDS_SEPARATOR);
		parser.AddOption(swConnectPlayer, wxEmptyString, wxEmptyString,
			wxCMD_LINE_VAL_STRING, wxCMD_LINE_NEEDS_SEPARATOR);
		parser.AddOption(swConnectGameID, wxEmptyString, wxEmptyString,
			wxCMD_LINE_VAL_STRING, wxCMD_LINE_NEEDS_SEPARATOR);
	}

	virtual void Execute(const wxCmdLineParser& parser, GamePtr& game, 
		bool inStartup)
	{
		// Clients need the following to start up the game:
		//
		// 1.  Host IP address
		// 2.  Host port
		// 3.  Player name
		// 4.  Game ID
		
		// Gather all of the information needed.
		wxString host, port, player, gameID;

		parser.Found(swConnect, &host);
		parser.Found(swConnectPort, &port);
		parser.Found(swConnectPlayer, &player);
		parser.Found(swConnectGameID, &gameID);

		wxString playerName;
		wxInt32 playerId = 0;

        // We must have a host ip, port, and player and game id.
		if( (true == Network::SplitNameAndId(player, playerName, playerId)) &&
			(false == host.IsEmpty()) &&
			(false == port.IsEmpty()) &&
			(false == playerName.IsEmpty()) &&
			(false == gameID.IsEmpty()))
		{
			Player thisPlayer(playerName, playerId);

			wxIPV4address addr;
			addr.Hostname(host);
			addr.Service(port);

			// Make sure the network engine is loaded.
			PLUGIN.LoadRuleset(swNetworkRules, swNetwork);

			long id;
			gameID.ToLong(&id);

			// Since we may be in startup and not the actual event loop, 
			// we need to wait until we get are fully up and running
			// before connecting.  
			TransientCommandPtr command(new TransientConnectCommand(thisPlayer,
				addr, id));

			// If we're in startup, we need to pass the transient command off
			// to the synchronizer to be handled in the main message loop.
			if(true == inStartup)
			{
				Controller::get().Transmit(shEventTransientCommand,
					command);
			}
			// Otherwise, just execute it immediately.
			else
			{
				command->Execute();
			}

			// Send the synchronize command to the site to tell it we got the 
			// connect command.  It's easiest to format this as a wxString and 
			// then convert it back to std::string for the command, since we
			// know there are no Unicode characters in it.
			HashString cmd = HashString::Format(shStartCmd.c_str(), id);
			Controller::get().Transmit(shEventSynchronize, cmd);
		}
	}
};

REGISTER_COMMAND(ConnectCommand, connect);

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
