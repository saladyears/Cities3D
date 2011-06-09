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
#include "RuleExtensionMixin.h"
#include "IUndoable.h"
#include "RuleSetDatabase.h"
#include "GameEngine.h"
#include "Controller.h"
#include "AggregatorObject.h"
#include "ChipObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleRewardHiddenTile
// 
// Rewards a player for discovering a hidden tile.
//
// Derived From:
//     <Rule>
//     <IUndoable>
//
// Project:
//     <SeafarersRules>
//
// RuleSet:
//     <Hidden>
//
class RuleRewardHiddenTile 
	: public Rule
	, public IUndoable
	, public RuleExtensionMixin
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxInt32 hiddenTile = object.read<wxInt32>();

		wxInt32 &hiddenIndex = gameData<wxInt32>(shHiddenIndex);
		HashStringArray &hiddenArray = gameData<HashStringArray>(shHiddenArray);

		if(hiddenIndex >= (wxInt32) hiddenArray.size())
		{
			wxLogError(wxT("Programmer Error: No hidden tiles available in HiddenArray"));
			return;
		}

		HashString tileType = hiddenArray[hiddenIndex++];
		tile<HashString>(shTileType, hiddenTile) = tileType;

		//check for resource
		DataObject input(hiddenTile), output;
		RULE.Decide(shLogicTileIsResource, input, output);
		if(true == output.read<bool>())
		{
			RULE.Execute(shRuleBankTransact, DataObject(current(), tileType, 1));

			// Write out some text.
			DataObject output;
			RULE.Decide(shLogicResourceName, DataObject(tileType), output);
			const wxString& name = output.read<wxString>();

			wxString str = wxString::Format(stReceivesOneForDiscovering.c_str(), 
				swStringFormat.c_str(), name.c_str());
			RULE.Execute(shRuleUpdateNetworkUI, DataObject(str));
			
			wxInt32 &chipIndex = gameData<wxInt32>(shHiddenChipIndex);
			HashStringArray &chipArray = 
				gameData<HashStringArray>(shHiddenChipArray);

			if(chipIndex >= (wxInt32) chipArray.size())
			{
				wxLogError(wxT("Programmer Error: No hidden chips available in HiddenChipArray"));
				return;
			}

			//set the tile's roll value
			wxInt32 rollVal = 
				boost::lexical_cast<wxInt32>(chipArray[chipIndex++]);
			
			tile<wxInt32>(shRoll, hiddenTile) = rollVal;

			//create a new chip object
			AggregatorObjectPtr aggregate(new AggregatorObject);
			tile(hiddenTile)->createChipObject(true);
			tile(hiddenTile)->chip()->randomizeRotation();
			aggregate->add(tile(hiddenTile)->chip());

			//notify the display engine
			Controller::get().Transmit(shEventGameObjects, aggregate);
		}

		ExecuteExtensions(object);
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

IMPLEMENT_RULE(RuleRewardHiddenTile, HIDDEN_TILES);
