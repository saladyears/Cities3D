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
#include "NetworkPlayers.h"
#include "Controller.h"
#include "ColorInfoData.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: NetworkRuleChangeColor
// 
// Changes a players color and updates their ready flag.
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
//     <RuleChangeColor>
//
// Transmits To:
//     eventPreGame
//
class NetworkRuleChangeColor : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		const Player &player = object.read<Player>();
		const wxBOOL ready = object.read<wxInt32>(2);

		// First, see if this player is already assigned a color, because
		// they might actually be switching colors with someone else if the
		// host is doing it for them.
		DataObject output;
		RULE.Decide(shNetworkLogicPlayerColor, object, output);
		const ColorType color = output.read<ColorType>();

		// Now actually change the color.
		RULE.Execute(shRuleChangeColor, object);

		// Set their ready flag.
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			const Player &thisPlayer = playerGame(i).player();

			if(player == thisPlayer)
			{
				playerGameData<wxInt32>(shReady, i) = ready;
				break;
			}
		}

		//fire the update mechanism
		Controller::get().Transmit(shEventPreGame, GetGame());

		// Update the UI.
		RULE.Execute(shNetworkRuleChangeColorMessage, object);

		// Finally, if they had a previous color, see if someone else is
		// now that color, and if so, update the UI.
		if(CR_SIZE != color)
		{
			for(wxInt32 i = 0; i < numPlayers(); ++i)
			{
				if(playerGame(i).color() == color)
				{
					RULE.Execute(shNetworkRuleChangeColorMessage, 
						DataObject(playerGame(i).player(), color));
				}
			}
		}
	}
};

IMPLEMENT_RULE(NetworkRuleChangeColor, NETWORK)
