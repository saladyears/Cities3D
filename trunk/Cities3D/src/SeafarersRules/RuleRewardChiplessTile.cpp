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
#include "IUndoable.h"
#include "RuleSetDatabase.h"
#include "GameEngine.h"
#include "Controller.h"
#include "AggregatorObject.h"
#include "ChipObject.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleRewardChiplessTile
// 
// Assigns a chipless tile for a player for discovering a chipless tile.
//
// Derived From:
//     <Rule>
//     <IUndoable>
//
// Project:
//     <SeafarersRules>
//
// RuleSet:
//     <Chipless>
//
class RuleRewardChiplessTile 
	: public Rule
	, public IUndoable
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxInt32 chiplessTile = object.read<wxInt32>();

		wxInt32 &chipIndex = gameData<wxInt32>(shChiplessChipIndex);
		HashStringArray &chipArray = 
			gameData<HashStringArray>(shChiplessChipArray);

		if(chipIndex >= (wxInt32) chipArray.size())
		{
			gameData<wxInt32>(shChiplessTile) = chiplessTile;
			gameData<HashString>(shState) = shChiplessTile;
			
			RULE.Execute(shRuleSelectChiplessTile, DataObject());
		}
		else
		{
			//set the tile's roll value
			wxInt32 rollVal = 
				boost::lexical_cast<wxInt32>(chipArray[chipIndex++]);
			
			tile<wxInt32>(shRoll, chiplessTile) = rollVal;
			tile<wxInt32>(shChipless, chiplessTile) = FALSE;

			//create a new chip object
			AggregatorObjectPtr aggregate(new AggregatorObject);
			tile(chiplessTile)->createChipObject(true);
			tile(chiplessTile)->chip()->randomizeRotation();
			aggregate->add(tile(chiplessTile)->chip());

			//notify the display engine
			Controller::get().Transmit(shEventGameObjects, aggregate);
		}
	}

	virtual void Unexecute(const DataObject&)
	{
		// This should never happen.
		wxASSERT(false);
	}
	
	virtual bool CanUndo() const
	{
		// Revealing tiles always breaks the undo chain.
		return false;
	}
};

IMPLEMENT_RULE(RuleRewardChiplessTile, CHIPLESS_TILES);
