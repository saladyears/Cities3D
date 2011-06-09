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
#include "AnimationPlacement.h"
#include "NetworkPlayers.h"
#include "Controller.h"
#include "INetworkEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleAnimatePlacementNetwork
// 
// Creates a placement animation for the given <IGameObject> and color.
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
// Mixin To:
//     <RuleAnimatePlacement>
//
class RuleAnimatePlacementNetwork : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(3 == object.numItems());

		const Player &player = object.read<Player>();

		// Only animate things placed by other players.
		if(NET.Players().GetThisPlayer() != player)
		{
			const GameObjectPtr &gameObject = object.read<GameObjectPtr>(1);
			const ColorType color = object.read<ColorType>(2);
			
			AnimationPtr animation(new AnimationPlacement(gameObject, color));
			Controller::get().Transmit(shEventAnimation, animation);
		}
	}
};

IMPLEMENT_RULE_KEYED_MIXIN(RuleAnimatePlacementNetwork, Network,
						   RuleAnimatePlacement, NETWORK)
