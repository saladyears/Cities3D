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
#include "LogicHashMixin.h"
#include "RuleSetDatabase.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicIsResourceAqueductable
// 
// A mixin container for determining if a resource can be shown in the aqueduct
// dialog.
//
// Derived From:
//     <Logic>
//     <LogicHashMixin>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <Standard>
//
class LogicIsResourceAqueductable 
	: public Logic
	, public LogicHashMixin
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		const HashString& res = input.read<HashString>();
		
		bool found = DecideHash(res, input, output);
		if(false == found)
		{
			// Default to true.
			output = DataObject(true);
		}
	}
};

IMPLEMENT_LOGIC(LogicIsResourceAqueductable, CITIES)
