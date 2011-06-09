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
#include "RuleExtensionMixin.h"
#include "RuleSetDatabase.h"
#include "Controller.h"
#include "IRuleEngine.h"
#include "CornerObject.h"
#include "Utility.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleCornerDestroyedVolcano
// 
// Handles a corner being destroyed by a volcano.
//
// Derived From:
//     <Rule>
//     <RuleExtensionMixin>
//
// Project:
//     <VolcanoRules>
//
// RuleSet:
//     <Volcano>
//
class RuleCornerDestroyedVolcano 
	: public Rule
	, public RuleExtensionMixin
{
public:
	virtual void Execute(const DataObject &object)
	{
		const CornerObjectPtr& obj = object.read<CornerObjectPtr>();
		const HashString& rule = object.read<HashString>(1);

		// Determine who owns this settlement and remove it from them.
		wxInt32 owner = obj->owner();
		wxInt32 id = Utility::encodeSel(obj->tile1().first, 
			obj->tile1().second);

		RULE.Execute(rule, DataObject(id, owner));

		// Play the sound.
		RULE.Execute(shRulePlaySound, 
			DataObject(SOUND_VOLCANIC_DESTRUCTION));

		// Call specialized extensions.
		ExecuteExtensions(object);

		Controller::get().Transmit(shEventPlayerUI, 
			DataObject(GetGame(), owner));
	}
};

IMPLEMENT_RULE(RuleCornerDestroyedVolcano, VOLCANO)
