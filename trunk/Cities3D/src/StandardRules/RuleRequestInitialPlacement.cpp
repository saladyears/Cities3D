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
#include "LogicHashMixin.h"
#include "RuleSetDatabase.h"
#include "GameEngine.h"
#include "Utility.h"
#include "UtilityGL.h"
#include "AggregatorObject.h"
#include "CornerSelectionObject.h"
#include "Controller.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- STATICS          -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleRequestInitialPlacement
// 
// Mixin container for finding available initial placement
// positions on the board.  Looks up the current <Game> 
// State in its <ILogic> hash and calls that <ILogic> if 
// one exists.  The hashed <ILogic> must provide the text
// for the <MessageUI> in the first output <DataObject> position.
// Creates a <CornerSelectionObject> with all available
// corners discovered by <LogicPlaceInitialTile> and
// <LogicPlaceInitialCorner>.
//
// Derived From:
//     <Rule>,
//     <LogicHashMixin>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
// Rules Called:
//     <RuleUpdateMessageUI>,
//     <RuleResetTileFlags>
//
// Logic Called:
//     <LogicPlaceInitialTile>,
//     <LogicPlaceInitialCorner>
//
// Selection Rule:
//     <RulePlaceInitialPlacement>
//
// Transmits To:
//     eventControlsUI
//     eventSelectionObject
//
class RuleRequestInitialPlacement 
: public Rule
, public LogicHashMixin
{
public:
	virtual void Execute(const DataObject &)
	{
        // The current player is now in a blocking action.
        RULE.Execute(shRuleBeginBlockingAction, DataObject(current()));

		Controller::get().Transmit(shEventControlsUI, DataObject(true));

		//retrieve the text to put in the MessageUI
		DataObject input(0), output;
		
		const HashString& state = gameData<HashString>(shState);
		if(false == DecideHash(state, input, output))
		{
			wxLogError(
				wxT("Programmer Error: No State %s in RuleRequestInitialPlacement"), 
				state.cwx_str());

			return;
		}

		const wxString& str1 = output.read<wxString>();
		const wxString& str2 = output.read<wxString>(1);
		RULE.Execute(shRuleUpdateMessageUI, DataObject(str1, str2));

		RULE.Execute(shRuleResetTileFlags, DataObject());

		ColorType color = playerGame().color();

		CornerSelectionObject *pObject;
		pObject = new CornerSelectionObject(
			shRulePlaceInitialPlacement, color);

		//go through the board data and add all the possible locations
		for(wxInt32 i = 0; i < numTiles(); ++i)
		{
			DataObject input(i), output;
			RULE.Decide(shLogicPlaceInitialTile, input, output);

			//skip whole tiles that fail
			if(false == output.read<bool>())
			{
				continue;
			}

			//look at each side
			for(wxInt32 j = 0; j < 6; ++j)
			{
				//apply all of the logic rules against this side
				DataObject input(std::make_pair(i, j)), output;
				RULE.Decide(shLogicPlaceInitialCorner, input, output);
				
				//if we're using this side, add it to the selection object
				if(true == output.read<bool>())
				{
					Vector coords = tile(i)->coords();
					UtilityGL::cornerCoords(coords, j);

					pObject->add(CornerSelectionObject::CornerSelectionPair(
						Utility::encodeSel(i, j), coords));
				}
			}
		}

		//pass the selection object along
		Controller::get().Transmit(shEventSelectionObject, 
			SelectionObjectPtr(pObject));
	}
};

IMPLEMENT_RULE(RuleRequestInitialPlacement, STANDARD);
