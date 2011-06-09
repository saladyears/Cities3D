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
#include "GameEngine.h"
#include "IUndoable.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RulePlaceSettlement
// 
// Places a settlement on the board at the given tile and corner
// for the current player.
//
// Derived From:
//     <Rule>,
//     <IUndoable>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
// Rules Called:
//     <RulePlaySettlementSound>
//     <RuleUpdateNetworkUI>
//
// Transmits To:
//     eventGameObjects
//
class RulePlaceSettlement 
	: public Rule
	, public IUndoable
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(1 <= object.numItems());

		// Update the UI.
		RULE.Execute(shRuleUpdateNetworkUI, DataObject(stPlacesASettlement));

		//play the correct sound
		RULE.Execute(shRulePlaySettlementSound, object);

		RULE.Execute(shRulePlaceSettlementCommon, object);
	}

	virtual void Unexecute(const DataObject &)
	{
		// Update the UI.
		RULE.Execute(shRuleUpdateNetworkUI, DataObject(stUndoesASettlement));
	}

	virtual bool CanUndo() const
	{
		//default to true for settlement placements, allow
		//container rules to decide
		return true;
	}
};

IMPLEMENT_RULE(RulePlaceSettlement, STANDARD)
