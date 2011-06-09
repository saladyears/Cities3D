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

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleOfferTrade
// 
// Handles a trade offer from one person to others.  This works in two 
// directions: from the trader to the tradees, and from a single tradee back
// towards the trader.
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
class RuleOfferTrade : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(2 <= object.numItems());

		wxInt32 offerPlayer = object.read<wxInt32>(2);

		DataObject input(-1), output;
		RULE.Decide(shLogicIsThisPlayer, input, output);
		wxInt32 thisPlayer = output.read<wxInt32>(1);
		
		// Going from trader to tradees.
		if(offerPlayer == current())
		{
			RULE.Execute(shRulePlaySound, DataObject(SOUND_TRADE_OFFER));

			// Handle spectators.
			if(-1 == thisPlayer)
			{
				thisPlayer = -2;
			}

			// Launch the tradee stuff if needed.
			Controller::get().Transmit(shEventOthersTrade,
				DataObject(GetGame(), thisPlayer));

			// The current trade offer is always updated.
			Controller::get().Transmit(shEventCurrentOffer, object);

			// Going from trader to tradees.
			RULE.Execute(shRuleCurrentOffer, object);

            // The user performed a game action.
            RULE.Execute(shRulePerformGameAction, DataObject());
		}
		// Going from tradee to trader.
		else
		{
			Controller::get().Transmit(shEventPlayerOffer, object);
		}
	}
};

IMPLEMENT_RULE(RuleOfferTrade, STANDARD)
