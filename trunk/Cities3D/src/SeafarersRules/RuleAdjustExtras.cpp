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
#include "ExtraObject.h"
#include "Controller.h"
#include "AggregatorObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleAdjustExtras
// 
// Handles setting extra points for trade routes.
//
// Derived From:
//     <Rule>
//     <IUndoable>
//
// Project:
//     <SeafarersRules>
//
// RuleSet:
//     <TradeRoutes>
//
class RuleAdjustExtras 
	: public Rule
	, public IUndoable
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(2 <= object.numItems());

		CornerObjectPtr corner = object.read<CornerObjectPtr>();
		wxInt32 owner = corner->owner();
		wxInt32 tile1 = corner->tile1().first;
		wxInt32 corner1 = corner->tile1().second;
		wxInt32 adjust = object.read<wxInt32>(1);

		wxInt32 links = corner->links();
		CornerObjectPtr above = corner;
		
		PlayerGame::CornerObjectArray objects = 
			playerGame(owner).getCornerObjects(shExtra);

		wxInt32 turn = gameData<wxInt32>(shTurn);

		CornerObject::TileCorner tileCorner(tile1, corner1);

		if(0 < adjust)
		{
			// If the adjustment is positive, find the item with 0 links, and
			// add new Extras with that as the link.
			
			if(0 < links)
			{
				for(size_t i = 0; i < objects.size(); ++i)
				{
					CornerObjectPtr thisObject = objects[i];

					if( (true == thisObject->onTileCorner(tileCorner)) &&
						(0 == thisObject->links()))
					{
						above = thisObject;
						break;
					}
				}
			}
	
			AggregatorObjectPtr aggregate(new AggregatorObject);

			for(wxInt32 i = 0; i < adjust; ++i)
			{
				CornerObjectPtr extra(new ExtraObject(current(), turn, CR_SIZE,
					GetGame(), tile1, corner1, above));
				above->below(extra);
				playerGame(owner).addCornerObject(shExtra, extra);
				above = extra;

				aggregate->add(extra);
			}

			Controller::get().Transmit(shEventGameObjects, aggregate);
		}
		else
		{
			// Otherwise, just remove extra objects with link count of zero
			// on this
			for(wxInt32 i = 0; i < -adjust; ++i)
			{
				for(size_t j = 0; j < objects.size(); ++j)
				{
					CornerObjectPtr thisObject = objects[j];

					if( (true == thisObject->onTileCorner(tileCorner)) &&
						(0 == thisObject->links()))
					{
						playerGame(owner).removeCornerObject(shExtra, 
							thisObject);
						break;
					}
				}
			}
		}
	}

	virtual void Unexecute(const DataObject &)
	{
		wxASSERT(false);
	}

	virtual bool CanUndo() const
	{
		// This can never be undone.
		return false;
	}
};

IMPLEMENT_RULE(RuleAdjustExtras, TRADE_ROUTES)
