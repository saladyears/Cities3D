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
#include "Logic.h"
#include "RuleSetDatabase.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicCanBankTransact
// 
// Determines if the bank has enough resources to make a transaction.
//
// Derived From:
//     <Logic>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
class LogicCanBankTransact : public Logic
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		const HashString &resource = input.read<HashString>();
		wxInt32 num = input.read<wxInt32>(1);
		wxInt32 bank = gameData<Data::IntHash>(shBankResources)[resource];

		// If the bank amount is -1, it means there is an infinite amount of
		// it.
		bool transact = (bank >= num);

		if(INT_MIN == bank)
		{
			transact = true;
		}

		output = DataObject(transact);
	}
};

IMPLEMENT_LOGIC(LogicCanBankTransact, STANDARD)
