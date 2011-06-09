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
#include "IRuleEngine.h"
#include "Utility.h"
#include "CornerObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleCornerDestroyedVolcanoCities
// 
// Handles dealing with metropolises when corners are destroyed.
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
//     <RuleCornerDestroyedVolcano>
//
class RuleCornerDestroyedVolcanoCities : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		const CornerObjectPtr& obj = object.read<CornerObjectPtr>();

		wxInt32 owner = obj->owner();
		wxInt32 id = Utility::encodeSel(obj->tile1().first, 
			obj->tile1().second);

		// Call the common rule.
		RULE.Execute(shRuleReduceCityVolcanoCities, DataObject(id, owner));
	}
};

IMPLEMENT_RULE_MIXIN(RuleCornerDestroyedVolcanoCities, 
					 RuleCornerDestroyedVolcano, VOLCANO_CITIES)
