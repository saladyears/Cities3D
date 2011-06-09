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
// Class: NetworkLogicAllowColorChange
// 
// Mixin to <LogicUIAllowColorChange> that contrains assigning colors to either
// the host or the player at this computer assigining themself.
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
//     <LogicUIAllowColorChange>
//
class NetworkLogicAllowColorChange : public Logic
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		const Player &player = input.read<Player>();
		const Player &thisPlayer = NET.Players().GetThisPlayer();
		bool host = NET.Players().IsHost();

		// They must be trying to change the player at this computer, or the
		// host.
		bool val = (thisPlayer == player) || host;

		output = DataObject(val);
	}
};

IMPLEMENT_LOGIC_MIXIN(NetworkLogicAllowColorChange, LogicUIAllowColorChange, 
					  NETWORK);
