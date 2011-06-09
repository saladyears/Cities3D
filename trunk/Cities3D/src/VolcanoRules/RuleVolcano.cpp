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
#include "Controller.h"
#include "IRuleEngine.h"
#include "RNG.h"
#include "CornerObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleVolcano
// 
// Handles volcanic eruptions.
//
// Derived From:
//     <Rule>
//
// Project:
//     <VolcanoRules>
//
// RuleSet:
//     <Volcano>
//
class RuleVolcano : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		// The first that that happens is that we pick a random corner out of 
		// six to feel the heat.
		wxInt32 corner = RAND.pooled(6);

		// Set the game data for the stats to use.
		gameData<wxInt32>(shVolcanoDie) = corner + 1;

		// Play the sound.
		RULE.Execute(shRulePlaySound, DataObject(SOUND_VOLCANIC_ERUPTION));

		// Send this up to the UI.
		Controller::get().Transmit(shEventVolcanoRoll, corner + 1);

		wxInt32 die1 = gameData<wxInt32>(shDie1);
		wxInt32 die2 = gameData<wxInt32>(shDie2);
		wxInt32 roll = die1 + die2;
		wxInt32 tiles = numTiles();

		// It's possible that we may need to move to a different rule.
		gameData<HashString>(shNextVolcanoRule) = shRuleMarkerPostDieRoll;

		for(wxInt32 i = 0; i < tiles; ++i)
		{
			if( (roll == tile<wxInt32>(shRoll, i)) &&
				(shVolcano == tile<HashString>(shTileType, i)))
			{
				Erupt(i, corner);
			}
		}

		HashString rule = gameData<HashString>(shNextVolcanoRule);
		eraseGameData<HashString>(shNextVolcanoRule);

		RULE.Execute(rule, DataObject());
	}

private:
	void Erupt(wxInt32 index, wxInt32 corner)
	{
		// When the volcano erupts, there are four possibilities:
		// 
		// 1.  Nothing happens.
		// 2.  A player loses a settlement.
		// 3.  A player has a city reduced to a settlement.
		// 4.  A player has a city destroyed, if they have no settlements with
		//     which to replace the city.
		//
		// It is also possible that multiple volcanoes erupt at the same time,
		// in which case these things may happen multiple times.
		
		// Because the tile is rotated, the Cities3D ordering, which starts with
		// zero in the upper-left corner of the hexagon, is not going to be 
		// the way the tile appears in the board, so we must unrotate the tile
		// in order to get the Cities3D tile corner.
		float rotation = tile(index)->rotation().y;
		wxInt32 rotateIndex = static_cast<wxInt32>(rotation / 60.0f);

		corner = (corner + rotateIndex) % 6;

		CornerObject::TileCorner tileCorner(index, corner);

		// Now see if anything is on this corner.
		wxInt32 players = numPlayers();

		// Keep track of what happens to every player.
		StringArray results;
		results.resize(players);

		// Start with settlements.
		for(wxInt32 i = 0; i < players; ++i)
		{
			bool onVolcano = false;
			wxInt32 lostSettlements = 0;
			wxInt32 lostCities = 0;
			wxInt32 reducedCities = 0;

			PlayerGame::CornerObjectArray objects = 
				playerGame(i).getCornerObjects(shSettlements);
			
			PlayerGame::CornerObjectArray::const_iterator it,
				itEnd = objects.end();
			for(it = objects.begin(); it != itEnd; ++it)
			{
				const CornerObjectPtr& object = (*it);

				// If it's on the tile, it gets an entry in the display, 
				// whether it is actually hit or not.
				if(true == object->onTile(index))
				{
					onVolcano = true;

					// If the settlement is on this corner, it's toast.
					if(true == object->onTileCorner(tileCorner))
					{
						RULE.Execute(shRuleCornerDestroyedVolcano, 
							DataObject(object, shRuleRemoveSettlement));
						++lostSettlements;
					}
				}
			}

			objects = playerGame(i).getCornerObjects(shCities);
			
			itEnd = objects.end();
			for(it = objects.begin(); it != itEnd; ++it)
			{
				const CornerObjectPtr& object = (*it);

				// A city is reduced to a settlement, unless the player
				// has no settlements in stock, in which case the city is
				// destroyed.
				if(true == object->onTile(index))
				{
					onVolcano = true;

					if(true == object->onTileCorner(tileCorner))
					{
						if(0 < playerGameData<wxInt32>(shStockSettlements, i))
						{
							RULE.Execute(shRuleCityReducedVolcano, 
								DataObject(object));
							++reducedCities;
						}
						else
						{
							RULE.Execute(shRuleCornerDestroyedVolcano, 
								DataObject(object, shRuleRemoveCity));
							++lostCities;
						}
					}
				}
			}

			// If they have anything on a volcano, they get an entry in the 
			// results.
			if(true == onVolcano)
			{
				// If they lost anything create the text for it.
				if(lostSettlements | lostCities | reducedCities)
				{
					wxString str;
					bool first = true;

					if(0 < lostSettlements)
					{
						if(1 == lostSettlements)
						{
							str = stLoses1Settlement;
						}
						else
						{
							str = wxString::Format(stLosesXSettlements.c_str(),
								lostSettlements);
						}
						
						first = false;
					}

					if(0 < lostCities)
					{
						if(false == first)
						{
							str += swComma;
						}
					
						if(1 == lostCities)
						{
							str += stLoses1City;
						}
						else
						{
							str += wxString::Format(stLosesXCities.c_str(),
								lostCities);
						}

						first = false;
					}

					if(0 < reducedCities)
					{
						if(false == first)
						{
							str += swComma;
						}

						if(1 == reducedCities)
						{
							str += stHas1CityReduced;
						}
						else
						{
							str += wxString::Format(stHasXCitiesReduced.c_str(),
								reducedCities);
						}
					}

					results[i] = wxString::Format(swStringStringExclamation.c_str(),
						playerGame(i).player().Name().c_str(),
						str.c_str());
				}
				else
				{
					results[i] = wxString::Format(stEscapesUnscathed.c_str(),
						playerGame(i).player().Name().c_str());
				}
			}
		}

		// Send the information up to the UI.
		Controller::get().Transmit(shEventVolcanoResult, results);
	}
};

IMPLEMENT_RULE(RuleVolcano, VOLCANO)
