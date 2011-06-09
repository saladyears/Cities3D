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
#include "Logic.h"
#include "RuleSetDatabase.h"
#include "NetworkPlayers.h"
#include "INetworkEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: NetworkLogicIsThisPlayer
// 
// Mixin logic for <LogicIsThisPlayer> that determines the player at this
// computer is the current player in a network game.
//
// Derived From:
//     <Logic>
//
// Project:
//     <NetworkRules>
//
// RuleSet:
//     <Network>
//
// Mixin To:
//     <LogicIsThisPlayer>
//
class NetworkLogicIsThisPlayer : public Logic
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		wxInt32 index = input.read<wxInt32>();
		wxInt32 actualIndex = -1;

		if(-1 == index)
		{
			index = current();
		}

		const Player &player = NET.Players().GetThisPlayer();

		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			const Player &currentPlayer = playerGame(i).player();
			if(currentPlayer == player)
			{
				actualIndex = i;
				break;
			}
		}

		bool equal = 
			(actualIndex == index) ||
			(shComplete == gameData<HashString>(shState));
		output = DataObject(equal, actualIndex);
	}
};

IMPLEMENT_LOGIC_KEYED_MIXIN(NetworkLogicIsThisPlayer, Network, 
							LogicIsThisPlayer, NETWORK)
