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
#include "TransientSpectateCommand.h"
#include "Controller.h"
#include "GameEngine.h"
#include "IPluginEngine.h"
#include "network/Network.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

// A spectate command needs to look something like this:
//
// -spectate <ip address> -spectateport <port> -spectateplayer <player:id> 
//
// Assumptions about player names:
//	 - one single word, no spaces
//   - no : characters
//   - id is not zero
//
class SpectateCommand : public ICommand
{
public:
	virtual ~SpectateCommand() {}

	virtual void PrepareParser(wxCmdLineParser& parser) const
	{
		parser.AddOption(swSpectate, wxEmptyString, wxEmptyString,
			wxCMD_LINE_VAL_STRING, wxCMD_LINE_NEEDS_SEPARATOR);
		parser.AddOption(swSpectatePort, wxEmptyString, wxEmptyString,
			wxCMD_LINE_VAL_STRING, wxCMD_LINE_NEEDS_SEPARATOR);
		parser.AddOption(swSpectatePlayer, wxEmptyString, wxEmptyString,
			wxCMD_LINE_VAL_STRING, wxCMD_LINE_NEEDS_SEPARATOR);
	}

	virtual void Execute(const wxCmdLineParser& parser, GamePtr& game, 
		bool inStartup)
	{
		// Spectators need the following to start up the game:
		//
		// 1.  Host IP address
		// 2.  Host port
		// 3.  Player name
		
		// Gather all of the information needed.
		wxString host, port, player;

		parser.Found(swSpectate, &host);
		parser.Found(swSpectatePort, &port);
		parser.Found(swSpectatePlayer, &player);
		
		wxString playerName;
		wxInt32 playerId = 0;

		// We must have a host ip, port, and player and game id.
		if( (true == Network::SplitNameAndId(player, playerName, playerId)) &&
			(false == host.IsEmpty()) &&
			(false == port.IsEmpty()) &&
			(false == playerName.IsEmpty()))
		{
			Player thisPlayer(playerName, playerId);

			wxIPV4address addr;
			addr.Hostname(host);
			addr.Service(port);

			// Make sure the network engine is loaded.
			PLUGIN.LoadRuleset(swNetworkRules, swNetwork);

			// Since we may be in startup and not the actual event loop, 
			// we need to wait until we get are fully up and running
			// before connecting.  
			TransientCommandPtr command(new TransientSpectateCommand(thisPlayer,
				addr));

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
		}
	}
};

REGISTER_COMMAND(SpectateCommand, spectate);

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
