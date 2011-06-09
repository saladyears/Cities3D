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
#include "RuleHashMixin.h"
#include "RuleSetDatabase.h"
#include "ConfigDatabase.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleAnimatePlacement
// 
// A mixin class that allows for a new object placement to have an animation
// that increases its visibility.
//
// Derived From:
//     <Rule>
//     <RuleHashMixin>
//
// Project:
//     <CoreRules>
//
// RuleSet:
//     <Core>
//
class RuleAnimatePlacement 
	: public Rule
	, public RuleHashMixin
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxBOOL animate = CONFIG.read<wxInt32>(swConfigSectionUI, swAnimationBoard, 
			TRUE);

		if(TRUE == animate)
		{
			HashString state = gameData<HashString>(shMode);
			if(false == ExecuteHash(state, object))
			{
				wxLogError(
					wxT("Programmer Error: No Mode %s in RuleAnimatePlacement"), 
					state.c_str());
			}
		}
	}
};

IMPLEMENT_RULE(RuleAnimatePlacement, CORE)
