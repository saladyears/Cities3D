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
// Class: RuleExecuteTrade
// 
// Executes a trade between the current player and a trade partner.
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
class RuleExecuteTrade : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(3 <= object.numItems());

		typedef std::map<HashString, wxInt32> ResourceMap;

		const ResourceMap& left = object.read<ResourceMap>();
		const ResourceMap& right = object.read<ResourceMap>(1);
		wxInt32 partner = object.read<wxInt32>(2);

		wxASSERT(false == left.empty());
		wxASSERT(false == right.empty());
		wxASSERT(-1 != partner);

		wxString str1;

		// Go through the left side, subtracting it from the current player
		// and adding it to the partner.
		ResourceMap::const_iterator it, itEnd = left.end();
		for(it = left.begin(); it != itEnd; ++it)
		{
			const HashString& res = it->first;
			const wxInt32 amount = it->second;

			RULE.Execute(shRuleAdjustResources, DataObject(current(), -amount, res));
			RULE.Execute(shRuleAdjustResources, DataObject(partner, amount, res));

			if(it != left.begin())
			{
				str1 += swCommaSpace;
			}

			// Get the translated name of the resource.
			DataObject input(res), output;
			RULE.Decide(shLogicResourceName, input, output);
			const wxString& name = output.read<wxString>();

			str1 += wxString::Format(swIntString.c_str(), amount, name.c_str());
		}

		const wxString& name = playerGame(partner).player().Name();

		wxString str2;

		// Go through the right side, adding it to the current player and
		// subtracting it from the partner.
		itEnd = right.end();
		for(it = right.begin(); it != itEnd; ++it)
		{
			const HashString& res = it->first;
			const wxInt32 amount = it->second;

			RULE.Execute(shRuleAdjustResources, DataObject(current(), amount, res));
			RULE.Execute(shRuleAdjustResources, DataObject(partner, -amount, res));

			if(it != right.begin())
			{
				str2 += swCommaSpace;
			}

			// Get the translated name of the resource.
			DataObject input(res), output;
			RULE.Decide(shLogicResourceName, input, output);
			const wxString& name = output.read<wxString>();

			str2 += wxString::Format(swIntString.c_str(), amount, name.c_str());
		}

		wxString str = wxString::Format(stTradesTo.c_str(), swStringFormat.c_str(), 
            str1.c_str(), name.c_str(), str2.c_str());

		// Update these two player's UIs.
		Controller::get().Transmit(shEventPlayerUI, DataObject(GetGame(), 
			current()));
		Controller::get().Transmit(shEventPlayerUI, DataObject(GetGame(), 
			partner));

		// They have new resources, other bank trades may be available.
		Controller::get().Transmit(shEventControlsUI, DataObject(true));

		Controller::get().Transmit(shEventBuildUI, 
			DataObject(true, GetGame()));

		Controller::get().Transmit(shEventTrade, partner);

		RULE.Execute(shRuleUpdateNetworkUI, DataObject(str));

		RULE.Execute(shRulePlaySound, DataObject(SOUND_TRADE_BANK));

        // The user performed a game action.
        RULE.Execute(shRulePerformGameAction, DataObject());
	}
};

IMPLEMENT_RULE(RuleExecuteTrade, STANDARD)
