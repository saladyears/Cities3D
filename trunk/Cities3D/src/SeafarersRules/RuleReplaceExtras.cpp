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
#include "Utility.h"
#include "ExtraObject.h"
#include "AggregatorObject.h"
#include "Controller.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleReplaceExtras
// 
// Replaces the extra objects in a obj/settlement switch.
//
// Derived From:
//     <Rule>
//
// Project:
//     <SeafarersRules>
//
// RuleSet:
//     <RepeatingBonus>
//
class RuleReplaceExtras : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxASSERT(2 <= object.numItems());

		wxInt32 tile, corner;
		boost::tie(tile, corner) = Utility::decodeSel(object.read<wxInt32>());
		wxInt32 player = object.read<wxInt32>(1);
		const HashString& type = object.read<HashString>(2);

		wxInt32 count = 0;

		// First, count how many extras there are.
		PlayerGame::CornerObjectArray extras =
			playerGame(player).getCornerObjects(shExtra);
		PlayerGame::CornerObjectArray::const_iterator it, itEnd = extras.end();
		for(it = extras.begin(); it != itEnd; ++it)
		{
			CornerObjectPtr extra = (*it);
			if(true == extra->onTileCorner(std::make_pair(tile, corner)))
			{
				// Remove it.
				playerGame(player).removeCornerObject(shExtra, extra);
				++count;
			}
		}

		if(0 < count)
		{
			// If they extras, find the object that was built.
			CornerObjectPtr obj;
			const PlayerGame::CornerObjectArray& objects =
				playerGame(player).getCornerObjects(type);
			itEnd = objects.end();
			for(it = objects.begin(); it != itEnd; ++it)
			{
				CornerObjectPtr object = (*it);
				if(true == object->onTileCorner(std::make_pair(tile, corner)))
				{
					obj = object;
					break;
				}
			}
			wxASSERT(obj);

			// Add a new extras to replace the old ones.
			wxInt32 turn = gameData<wxInt32>(shTurn);
			AggregatorObjectPtr aggregate(new AggregatorObject);
			CornerObjectPtr above = obj;

			for(wxInt32 i = 0; i < count; ++i)
			{
				CornerObjectPtr extra(new ExtraObject(player, turn, CR_SIZE,
					GetGame(), tile, corner, above));
				above->below(extra);
				playerGame().addCornerObject(shExtra, extra);

				aggregate->add(extra);
				above = extra;
			}

			Controller::get().Transmit(shEventGameObjects, aggregate);
		}
	}
};

IMPLEMENT_RULE(RuleReplaceExtras, REPEATING_BONUS);
