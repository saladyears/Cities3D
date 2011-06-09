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

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleBarbarianAttack
// 
// Handles the consequences of a barbarian attack.
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
//
class RuleBarbarianAttack : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		// Immediately see if we have winners or losers.
		DataObject input(0), output;
		RULE.Decide(shLogicKnightCounts, input, output);

		typedef std::set<wxInt32> IntSet;
		bool winners = output.read<bool>();
		const IntSet& results = output.read<IntSet>(1);

		HashString rule = shRuleMarkerPostDiceRoll;
		DataObject object;

		// Now that the barbarians are guaranteed too have attacked, the robber
		// is allowed to be placed.
		gameData<wxInt32>(shAllowRobberPlace) = TRUE;
		
		if(true == winners)
		{
			RULE.Execute(shRuleGenerateSystemMessage, 
                DataObject(stTheBarbariansAttackAndAreRepelled));

			RULE.Execute(shRulePlaySound, DataObject(SOUND_BARBARIAN_DEFEND));

			// If we have one single winner, they get a defender point.
			if(1 == results.size())
			{
				rule = shRuleDefenderPoint;
				object = DataObject(*results.begin());
			}
			// Multiple winners means that we have to select progress card
			// types.
			else if(false == results.empty())
			{
				IntSet::const_iterator it, itEnd = results.end();
				for(it = results.begin(); it != itEnd; ++it)
				{
					wxInt32 index = (*it);
					playerGameData<wxInt32>(shGainProgressCard, index) = TRUE;
				}

				gameData<HashString>(shState) = shProgressCards;
				rule = shRuleSelectProgressCards;
			}
		}
		// Losers have to select a city to destroy.
		else if(false == results.empty())
		{
			RULE.Execute(shRuleGenerateSystemMessage, 
                DataObject(stTheBarbariansAttackAndLayWaste));

			RULE.Execute(shRulePlaySound, DataObject(SOUND_BARBARIAN_PILLAGE));

			gameData<wxInt32>(shOriginalPlayer) = current();
			gameData<HashString>(shState) = shReduceCities;
			rule = shRuleRestartReduceCities;

			// Set the lose city flag for all players that need to.
			IntSet::const_iterator it, itEnd = results.end();
			for(it = results.begin(); it != itEnd; ++it)
			{
				wxInt32 player = (*it);

				playerGameData<wxInt32>(shLoseCity, player) = TRUE;
			}
		}

		RULE.Execute(rule, object);
	}
};

IMPLEMENT_RULE(RuleBarbarianAttack, CITIES)
