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

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleEndTurnCities
// 
// Extension to RuleEndTurn for knight movement handling.
//
// Derived From:
//     <Rule>
//
// Project:
//     <CitiesRules>
//
// RuleSet:
//     <Cities>
//
// Mixin To:
//     <RuleEndTurn>
//
class RuleEndTurnCities : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		// At the end of the turn, the merchat fleet card is no longer valid.
		playerGameData<HashStringArray>(shMerchantFleetCards).clear();
		
		// No more free stuff either.
		playerGameData<wxInt32>(shFreeKnightUpgrades) = 0;
		playerGameData<wxInt32>(shFreeWalls) = 0;
		playerGameData<wxInt32>(shFreeCityUpgrades) = 0;

		playerGameData<wxInt32>(shAlchemist) = 0;
		gameData<wxInt32>(shAlchemistDie1) = 0;
		gameData<wxInt32>(shAlchemistDie2) = 0;

		// Get rid of any knight move undo information.
		eraseGameData<wxInt32>(shMoveKnightTile);
		eraseGameData<wxInt32>(shMoveKnightCorner);
		eraseGameData<wxInt32>(shMoveKnightTurn);
		eraseGameData<wxInt32>(shMoveKnightActive);
		eraseGameData<wxInt32>(shMoveKnightStrength);
		eraseGameData<wxInt32>(shMoveKnightUpgradeTurn);

		// As well as attack information.
		eraseGameData<wxInt32>(shAttackKnightTile);
		eraseGameData<wxInt32>(shAttackKnightCorner);
		eraseGameData<wxInt32>(shAttackKnightTurn);
		eraseGameData<wxInt32>(shAttackKnightStrength);
		eraseGameData<wxInt32>(shAttackKnightUpgradeTurn);

		// And deserter information.
		eraseGameData<wxInt32>(shDeserterKnightStrength);
		eraseGameData<wxInt32>(shDeserterKnightUpgrade);
		eraseGameData<wxInt32>(shDeserterKnightActive);

		eraseGameData<wxInt32>(shOriginalPlayer);
		
		eraseGameData<wxInt32>(shExpelTile);
		eraseGameData<wxInt32>(shExpelCorner);

		eraseGameData<wxInt32>(shMasterMerchantVictim);
		eraseGameData<wxInt32>(shCommercialHarborVictim);
		eraseGameData<HashString>(shCommercialHarborResource);
		eraseGameData<HashString>(shCommercialHarborCommodity);

		eraseGameData<wxInt32>(shSpyVictim);
		eraseGameData<HashString>(shDiplomatType);

		eraseGameData<wxInt32>(shKnightUpgradeFree);
		eraseGameData<wxInt32>(shWallsFree);
		eraseGameData<wxInt32>(shCityUpgradeFree);
		eraseGameData<wxInt32>(shInventorChip);
		
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			playerGameData<wxInt32>(shCommercialHarbor, i) = FALSE;
		}
	}
};

IMPLEMENT_RULE_MIXIN(RuleEndTurnCities, RuleEndTurn, CITIES)
