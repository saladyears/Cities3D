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
#include "DummySelectionObject.h"
#include "CornerObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleRestartVolcanoMetropolis
// 
// Restarts the game from players moving metropolises after losing a city to
// the volcano.
//
// Derived From:
//     <Rule>
//
// Project:
//     <VolcanoRules>
//
// RuleSet:
//     <VolcanoCities>
//
// Mixin To:
//     <RuleRestartGame>
//
class RuleRestartVolcanoMetropolis : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		bool done = true;
		bool first = true;
		bool thisPlayer = false;
		wxString thisName;
		wxString str;

		// See if any players need to lose a city.
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			if(TRUE == playerGameData<wxInt32>(shMoveMetropolis, i))
			{
                // This player is now in a blocking action.
                RULE.Execute(shRuleBeginBlockingAction, DataObject(i));

				// Shut everything down.
				Controller::get().Transmit(shEventBuildUI, 
					DataObject(false, GetGame()));
				Controller::get().Transmit(shEventControlsUI, 
					DataObject(false));

				// Add their name to the waiting list.
				if(true == first)
				{
					first = false;
				}
				else
				{
					str += swComma;
				}

				const wxString& name = playerGame(i).player().Name();
				str += name;

				done = false;

				// See if this player is moving a metropolis
				DataObject input(i), output;
				RULE.Decide(shLogicIsThisPlayer, input, output);
				if(true == output.read<bool>())
				{
					thisPlayer = true;
					thisName = name;

					// Store the current player so that each player sees their
					// text in their color.
					gameData<wxInt32>(shCurrentPlayer) = i;

					// Turn on all selectable cities for this player.
					DummySelectionObject* selection = new DummySelectionObject(
						shRuleSelectVolcanoMetropolis);

					Controller::get().Transmit(shEventSelectionObject, 
						SelectionObjectPtr(selection));

					RULE.Execute(shRuleResetObjectFlags, 
						DataObject(IGameObject::Dim));
					RULE.Execute(shRuleResetTileFlags, 
						DataObject(IGameObject::Dim));
					
					PlayerGame::CornerObjectArray cities = 
						playerGame(i).getCornerObjects(shCities);

					PlayerGame::CornerObjectArray::iterator it, 
						itEnd = cities.end();
					for(it = cities.begin(); it != itEnd; ++it)
					{
						CornerObjectPtr object = (*it);

						// Any city with a metropolis on it cannot be 
						// destroyed.
						if(!object->above().lock())
						{
							object->reset();
							object->set(IGameObject::Select);

							wxInt32 tile1 = object->tile1().first;
							wxInt32 tile2 = object->tile2().first;
							wxInt32 tile3 = object->tile3().first;

							tile(tile1)->reset();

							if(-1 != tile2)
							{
								tile(tile2)->reset();
							}

							if(-1 != tile3)
							{
								tile(tile3)->reset();
							}
						}
					}
				}
			}
		}

		if(true == done)
		{
			// After we're done, move on to the next part of the dice roll.
			RULE.Execute(shRuleMarkerPostDieRoll, DataObject());
		}
		else
		{
			wxString str1;

			if(true == thisPlayer)
			{
				str1 = wxString::Format(stMoveMetropolis, thisName.c_str());
			}
			else
			{
				str1 = wxString::Format(stWaitingMoveMetropolis, str.c_str());
			}

			Controller::get().Transmit(shEventMessageUI, DataObject(str1, 
				GetGame()));
		}
	}
};

IMPLEMENT_RULE_KEYED_MIXIN(RuleRestartVolcanoMetropolis, VolcanoMetropolis, 
						   RuleRestartGame, VOLCANO_CITIES)
