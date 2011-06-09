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
#include "RuleHashMixin.h"
#include "Controller.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleUpdateNetworkUI
// 
// Updates the <NetworkUI> with the given text.  Looks in its <ILogic>
// hash for an <ILogic> that can handle the given <Game> Mode.  The hashed
// <ILogic> should provide the wrapper text appropriate for that game mode.
//
// Derived From:
//     <Rule>
//     <RuleHashMixin>
//
// Project:
//     <RuleUpdateNetworkUI>
//
// RuleSet:
//     <CoreRules>
//
// Mixin To:
//     <Core>
//
class RuleUpdateNetworkUI 
: public Rule
, public RuleHashMixin
{
public:
	virtual void Execute(const DataObject &object)
	{
		HashString mode = gameData<HashString>(shMode);

		if(false == ExecuteHash(mode, object))
		{
			wxLogError(wxT("Programmer Error: No Mode %s in ")
				wxT("RuleUpdateNetworkUI."), mode.c_str());
		}
	}
};

IMPLEMENT_RULE(RuleUpdateNetworkUI, CORE)
