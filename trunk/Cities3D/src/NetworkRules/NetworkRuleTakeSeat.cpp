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
#include "Rule.h"
#include "RuleSetDatabase.h"
#include "ColorInfoData.h"
#include "HTML.h"
#include "Controller.h"
#include "NetworkPlayers.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: NetworkRuleTakeSeat
// 
// Examines the seats in the game to see if this player should take one of 
// them, either because the seat is empty and they are in the original player
// list, or the seat is taken, but they are higher in priority in the original
// player list than the current player.
//
// Derived From:
//     <Rule>
//
// Project:
//     <NetworkRules>
//
// RuleSet:
//     <Network>
//
// Rules Called:
//     <NetworkRuleRemoveSpectator>
//     <RuleGenerateSystemMessage>
//
// Transmits To:
//     eventNetworkRestartPlayers
//     eventSpectator
//     eventShutdownUI
//
class NetworkRuleTakeSeat : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(1 <= object.numItems());

		const Player &player = object.read<Player>();
		bool restart = true;

		if(2 <= object.numItems())
		{
			restart = object.read<bool>(1);
		}

		// Run through each of the seats in the game.  By rule, a player may 
		// only be listed in one original player list, so we can determine what
		// action to take if and when we find them in a list.
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			PlayerGame::TurnPlayerMap originals;
			playerGame(i).originalPlayers(originals);
			
			const Player &seatPlayer = playerGame(i).player();
			const ColorType color = playerGame(i).color();
			
			bool higherPriority = false;

			PlayerGame::TurnPlayerMap::const_iterator it, 
				itEnd = originals.end();
			for(it = originals.begin(); it != itEnd; ++it)
			{
				const Player &originalPlayer = it->second;

				// See if the current seat player is higher in the priority
				// list.
				if(originalPlayer == seatPlayer)
				{
					higherPriority = true;
				}
				// If they are in the list, see what needs to happen.
				else if(originalPlayer == player)
				{
					// If there is no player in the seat, this player gets it.
					if(NullPlayer == seatPlayer)
					{
						wxString str = wxString::Format(
							restart ? 
							stRejoinsAs.c_str() :
							stJoinsAs.c_str(), 
							player.Name().c_str(),
							ColorInfoData::Name(color).c_str());
						HTML::Color(str, color);

						RULE.Execute(shNetworkRulePlayerReplace,
							DataObject(player, i, str));
					}
					// Otherwise, if this player has a higher priority than the
					// current player, replace the current player.
					else if(false == higherPriority)
					{
						// The current player goes to the end of the spectator
						// list.
						RULE.Execute(shNetworkRuleAddSpectator, 
							DataObject(seatPlayer));

						wxString str = wxString::Format(
							stReplacesAs.c_str(), 
							player.Name().c_str(),
							seatPlayer.Name().c_str(),
							ColorInfoData::Name(color).c_str());
						HTML::Color(str, color);

						RULE.Execute(shNetworkRulePlayerReplace,
							DataObject(player, i, str));

						// Reset everything.
						Controller::get().Transmit(shEventShutdownUI, 0);
						RULE.Execute(shRuleRestartGame, DataObject());
					}

					break;
				}
			}
		}

		// Immediately update everything.
		Controller::get().Transmit(shEventNetworkRestartPlayers, GetGame());
		Controller::get().Transmit(shEventSpectator, 0);
	}
};

IMPLEMENT_RULE(NetworkRuleTakeSeat, NETWORK)
