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
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- STATICS          -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicIsResourceBankTradeable
// 
// Mixin container for logic that determines whether a particular resource can
// be traded to the bank.
//
// Derived From:
//     <Logic>
//     <BaseMixin>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
class LogicIsResourceBankTradeable 
	: public Logic
	, public BaseMixin
{
	void Decide(const DataObject &input, DataObject &output)
	{
		// This is very similiar to LogicBooleanMixin, but the output value
		// must return the trade ratio, not just a boolean value.
		wxInt32 ratio = -1;
		bool canTrade = false;

		IRuleEngine::StringHash::const_iterator it, itEnd(hash_end());
		for(it = hash_begin(); it != itEnd; ++it)
		{
			//apply the logic
			output.reset();
			RULE.Decide(it->second, input, output);
			wxASSERT(2 <= output.numItems());
			
			if(true == output.read<bool>())
			{
				// Take the best ratio.
				wxInt32 thisRatio = output.read<wxInt32>(1);
				if( (thisRatio < ratio) ||
					(-1 == ratio))
				{
					ratio = thisRatio;
					canTrade = true;
				}
			}
		}

		output = DataObject(canTrade, ratio);
	}
};

IMPLEMENT_LOGIC(LogicIsResourceBankTradeable, STANDARD);
