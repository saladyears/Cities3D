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
#include "ChipObject.h"
#include "HexObject.h"
#include "CornerObject.h"
#include "Controller.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleGatherResources
// 
// Gathers resources for the players based on the dice roll.
//
// Derived From:
//     <Rule>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
// Rules Called:
//     <RuleBankTransact>
//     <RuleGenerateSystemMessage>
//
// Logic Called:
//     <LogicGatherSettlement>
//     <LogicGatherCity>
//
// Transmits To:
//     eventPlayerUI
//     eventShowResources
//     eventPlayerResources
//
class RuleGatherResources : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		// Get the roll.
		wxInt32 roll = gameData<wxInt32>(shDie1) + gameData<wxInt32>(shDie2);

		// Determine which tile the robber is on.
		const HexObjectPtr &robber = GetGame()->getHexObject(shRobber);
		wxASSERT(robber);

		wxInt32 robberTile = robber->tile();

		// If the robber is hidden, he does not affect resources.
		if(true == robber->isset(IGameObject::Hidden))
		{
			robberTile = -1;
		}

		IntArray tiles;

		// Gather up all of the tiles that this roll affects.
		for(wxInt32 i = 0; i < numTiles(); ++i)
		{
			const ChipObjectPtr &chip = tile(i)->chip();
			if( (chip) && 
				(chip->roll() == roll))
			{
				tiles.push_back(i);
			}
		}

		// Now that we have the list of tiles, time to start gathering
		// resources.
		typedef std::vector<ResourceMap> ResourceMapArray;

		// We need a list of overall resources being acquired, and one for
		// each player.
		ResourceMap totalResources;
		ResourceMapArray playerResources(numPlayers());
		ResourceMapArray blockedResources(numPlayers());

		// First, traverse through each player and see if they have any
		// cities or settlements on these tiles.
		for(size_t i = 0; i < tiles.size(); ++i)
		{
			const wxInt32 tileNum = tiles[i];
			
			for(wxInt32 j = 0; j < numPlayers(); ++j)
			{
				PlayerGame &game = playerGame(j);
				ResourceMap &resources = playerResources[j];
				ResourceMap &blocked = blockedResources[j];
				
				// Settlements.
				const PlayerGame::CornerObjectArray &settlements =
					game.getCornerObjects(shSettlements);

				for(size_t h = 0; h < settlements.size(); ++h)
				{
					CalculateResources(settlements[h], shLogicGatherSettlement, 
						tileNum, totalResources, resources, blocked, j, robberTile);
				}

				// Cities.
				const PlayerGame::CornerObjectArray &cities =
					game.getCornerObjects(shCities);

				for(size_t h = 0; h < cities.size(); ++h)
				{	
					CalculateResources(cities[h], shLogicGatherCity, tileNum,
						totalResources, resources, blocked, j, robberTile);
				}
			}
		}

		// Now that we have put together who gets what, see if the bank allows
		// us to pass out that many resource and if so, execute it.
		ResourceMap::const_iterator it, itEnd = totalResources.end();
		for(it = totalResources.begin(); it != itEnd; ++it)
		{
			const HashString &resource = it->first;
			const wxInt32 total = it->second;

			// See if the bank will allow this transaction.
			DataObject input(resource, total), output;
			RULE.Decide(shLogicCanBankTransact, input, output);

			if(true == output.read<bool>())
			{
				// If yes, then run the transactions on this type for all
				// the players that have it.
				for(wxInt32 i = 0; i < numPlayers(); ++i)
				{
					wxInt32 total = playerResources[i][resource];

					if(0 < total)
					{
						RULE.Execute(shRuleBankTransact, DataObject(i, 
							resource, total));
					}
				}
			}
			else
			{
				// Put out a system message describing what happened.
				RULE.Execute(shRuleGenerateSystemMessage, DataObject(
					wxString::Format(stThereIsNotEnough.c_str(), resource.c_str())));

				// Clear out this resource for everyone.
				for(wxInt32 i = 0; i < numPlayers(); ++i)
				{
					wxInt32 total = playerResources[i][resource];

					// Record the resource being blocked.
					if(0 < total)
					{
						RULE.Execute(shRuleBlockBankResources, DataObject(i, total, resource));
					}

					playerResources[i][resource] = 0;
				}
			}
		}

		// Finally, store the total number of resources each player got with
		// their data, for use in later rules.
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			wxInt32 total = 0;
			const ResourceMap& resources = playerResources[i];

			ResourceMap::const_iterator it, itEnd = resources.end();
			for(it = resources.begin(); it != itEnd; ++it)
			{
				wxInt32 amount = it->second;
				total += amount;
			}

            playerGameData<wxInt32>(shResourcesGained, i) = total;

			// Additionally, report all resources blocked for each player to
			// the stat engine.
			const ResourceMap& blocked = blockedResources[i];

			itEnd = blocked.end();
			for(it = blocked.begin(); it != itEnd; ++it)
			{
				const HashString& res = it->first;
				wxInt32 amount = it->second;
				
				RULE.Execute(shRuleBlockResources, DataObject(i, amount, res));
			}
		}

		// Update the UI.
		Controller::get().Transmit(shEventPlayerUI, DataObject(GetGame(), -1));
		Controller::get().Transmit(shEventShowResources, 
			DataObject(true, roll));
		Controller::get().Transmit(shEventPlayerResources, playerResources);

		// Continue on post roll.
		RULE.Execute(shRuleMarkerPostDiceRoll, DataObject());
	}

private:
	typedef std::map<HashString, wxInt32> ResourceMap;
		
	void CalculateResources(const CornerObjectPtr &object, 
		const HashString &logic, const wxInt32 tileNum, ResourceMap &total, 
		ResourceMap &player, ResourceMap& blocked, wxInt32 index, wxInt32 robber)
	{
		if( (object->tile1().first == tileNum) ||
			(object->tile2().first == tileNum) ||
			(object->tile3().first == tileNum))
		{
			const HashString &resource = tile<HashString>(shTileType, tileNum);

			// Get the actual type of the resource.
			DataObject output;
			RULE.Decide(shLogicResourceType, DataObject(resource), output);

			HashString realResource = output.read<HashString>();

			// We've got an object.
			output.reset();
			RULE.Decide(logic, DataObject(realResource, object->id(), index), 
				output);

			// Add to total and player resources.
			const ResourceMap &resources = output.read<ResourceMap>();
			ResourceMap::const_iterator it, itEnd = resources.end();
			
			for(it = resources.begin(); it != itEnd; ++it)
			{
				const HashString &resource = it->first;
				const wxInt32 num = it->second;

				if(tileNum == robber)
				{
					blocked[resource] += num;
				}
				else
				{
					total[resource] += num;
					player[resource] += num;
				}
			}
		}
	}
};

IMPLEMENT_RULE(RuleGatherResources, STANDARD)
