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
#include "LogicBooleanMixin.h"
#include "RuleSetDatabase.h"

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- STATICS          -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicPlaceInitialBlock
// 
// Mixin container that determines if an initial placement location is blocked 
// by another corner object within two corners.
//
// Derived From:
//     <LogicBooleanMixin>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
// Mixin To:
//     <LogicPlaceInitialCorner>
//
class LogicPlaceInitialBlock : public LogicBooleanMixin<false>
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		wxASSERT(1 <= input.numItems());

		// Since we are asking our mixins whether this corner is blocked, but
		// we mixin to LogicPlaceInitialCorner, which is asking if the corner
		// can be built on, we have to reverse the decision of the mixin
		// logic.
		LogicBooleanMixin<false>::Decide(input, output);

		output = DataObject(!output.read<bool>());
	}
};

IMPLEMENT_LOGIC_MIXIN(LogicPlaceInitialBlock, LogicPlaceInitialCorner, 
					  STANDARD);
