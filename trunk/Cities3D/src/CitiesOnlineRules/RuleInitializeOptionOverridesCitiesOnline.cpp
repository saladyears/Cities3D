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
#include "CommandProcesser.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------------------------------------------------------//
// Class: RuleInitializeOptionOverridesCitiesOnline
// 
// Initialize option overrides for a Cities Online game.
//
// Derived From:
//     <Rule>
//
// Project:
//     <CitiesOnlineRules>
//
// RuleSet:
//     <CitiesOnline>
//
// Mixin To:
//     <RuleInitializeOptionOverrides>
//
class RuleInitializeOptionOverridesCitiesOnline : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		// Retrieve any UI values set by the host and set them up.
        const StringPairArray& settings = CommandProcesser::get().GetUISettings();
        StringPairArray::const_iterator it, itEnd = settings.end();
        for(it = settings.begin(); it != itEnd; ++it)
        {
            const wxString& element = it->first;
            const wxString& value = it->second;

            // The element has to be stored as a HashString
            HashString e(element);
            long v;
            value.ToLong(&v);

            gameData<wxInt32>(e) = static_cast<wxInt32>(v);
        }

        // Nuke the settings, we don't need them any more.
        CommandProcesser::get().ClearUISettings();
	}
};

IMPLEMENT_RULE_MIXIN(RuleInitializeOptionOverridesCitiesOnline, 
                     RuleInitializeOptionOverrides, CITIESONLINE)
