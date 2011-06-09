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
#include "IUndoable.h"
#include "GameEngine.h"
#include "Controller.h"
#include "Utility.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- STATICS          -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleInitialResources
// 
// Gives players initial resources based on the location of their
// second placement.  Each of the three tiles at the given location
// are tested to see if they are resource providing.  If so, the
// resource is added to the player's current amount and subtracted from
// the bank's amount.  *Undoable*.
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
// Logic Called:
//     <LogicTileIsResource>
//
// Transmits To:
//     eventPlayerUI
//
class RuleInitialResources 
: public Rule
, public IUndoable
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(1 <= object.numItems());

		wxInt32 tile1, tile2, tile3;
		wxInt32 corner1, corner2, corner3;

		boost::tie(tile1, corner1) = 
			Utility::decodeSel(object.read<wxInt32>());

		//add cards for all tiles that are resource tiles
		Utility::otherTiles(GetGame(), tile1, corner1, tile2, corner2, tile3, 
			corner3);

		handleResource(tile1, true);
		handleResource(tile2, true);
		handleResource(tile3, true);

		Controller::get().Transmit(shEventPlayerUI, 
			DataObject(GetGame(), -1));
	}

	virtual void Unexecute(const DataObject &object)
	{
		wxASSERT(1 <= object.numItems());

		//undo is done in reverse order automatically

		wxInt32 tile1, tile2, tile3;
		wxInt32 corner1, corner2, corner3;

		boost::tie(tile1, corner1) = 
			Utility::decodeSel(object.read<wxInt32>());

		//remove cards for all tiles that are resource tiles
		Utility::otherTiles(GetGame(), tile1, corner1, tile2, corner2, tile3, 
			corner3);

		handleResource(tile1, false);
		handleResource(tile2, false);
		handleResource(tile3, false);

		//fire the update
		Controller::get().Transmit(shEventPlayerUI, 
			DataObject(GetGame(), -1));
	}

	virtual bool CanUndo() const
	{
		//initial resources are always undoable, since they are part of placing
		//the second placement, which is always undoable
		return true;
	}

private:
	void handleResource(wxInt32 thisTile, bool increment)
	{
		DataObject input(thisTile), output;
		RULE.Decide(shLogicTileIsResource, input, output);

		if(true == output.read<bool>())
		{
			const HashString &str = tile<HashString>(shTileType, thisTile);

			output.reset();
			RULE.Decide(shLogicResourceType, DataObject(str), output);

			HashString realResource = output.read<HashString>();

			// Now see if we're allowed to actually gather this resource.
			output.reset();
			RULE.Decide(shLogicIsInitialResource, DataObject(realResource),
				output);

			if(true == output.read<bool>())
			{
				if(true == increment)
				{
					RULE.Execute(shRuleBankTransact, DataObject(current(), 
						realResource, 1));
				}
				else
				{
					RULE.Execute(shRuleBankTransact, DataObject(current(), 
						realResource, -1));
				}
			}
		}
	}
};

IMPLEMENT_RULE(RuleInitialResources, STANDARD);
