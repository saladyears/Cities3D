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

//---------------------------- STATICS          -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
	HashString landArray[] = {
		shTimber, 
		shClay, 
		shSheep, 
		shWheat, 
		shOre,
		shDesert
	};

	HashString oceanArray[] = {
		shPort3, 
		shPortTimber, 
		shPortClay,
		shPortWheat, 
		shPortOre, 
		shPortSheep,
		shOcean
	};

	//base helper class
	class LogicTile : public Logic
	{
	protected:
		bool inArray(const DataObject &input, const HashString *array, 
			const wxInt32 size)
		{
			wxASSERT(1 <= input.numItems());

			//look up the tile type
			wxInt32 thisTile = input.read<wxInt32>();

			bool found = false;
			if(0 <= thisTile)
			{
				HashString type = tile<HashString>(shTileType, thisTile);
			
				//now see if it is in the array
				found = (&array[size] != std::find(&array[0], &array[size], type));
			}

			return found;
		}
	};
}

//---------------------------- LOGIC            -----------------------------//

//---------------------------------------------------------------------------//
// Class: LogicTileIsLandStandard
// 
// Allows a land tile to be Timber, Clay, Sheep, Wheat, Ore,
// or Desert.
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
// Mixin To:
//     <LogicTileIsLand>
//
class LogicTileIsLandStandard : public LogicTile
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		bool found = inArray(input, landArray, 6);
		output = DataObject(found);
	}
};

IMPLEMENT_LOGIC_MIXIN(LogicTileIsLandStandard, LogicTileIsLand, STANDARD);

//---------------------------------------------------------------------------//
// Class: LogicTileIsOceanStandard
// 
// Allows an ocean tile to be Port3, PortTimber, PortClay, 
// PortSheep, PortWheat, PortOre, or Ocean.
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
// Mixin To:
//     <LogicTileIsOcean>
//
class LogicTileIsOceanStandard : public LogicTile
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		bool found = inArray(input, oceanArray, 7);
		output = DataObject(found);
	}
};

IMPLEMENT_LOGIC_MIXIN(LogicTileIsOceanStandard, LogicTileIsOcean, STANDARD);

//---------------------------------------------------------------------------//
// Class: LogicTileIsPortStandard
// 
// Allows a port tile to be Port3, PortTimber, PortClay, 
// PortSheep, PortWheat, or PortOre.
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
// Mixin To:
//     <LogicTileIsPort>
//
class LogicTileIsPortStandard : public LogicTile
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		bool found = inArray(input, oceanArray, 6);
		output = DataObject(found);
	}
};

IMPLEMENT_LOGIC_MIXIN(LogicTileIsPortStandard, LogicTileIsPort, STANDARD);

//---------------------------------------------------------------------------//
// Class: LogicTileIsResourceStandard
// 
// Allows a resource tile to be Timber, Clay, Sheep, Wheat, 
// or Ore.
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
// Mixin To:
//     <LogicTileIsResource>
//
class LogicTileIsResourceStandard : public LogicTile
{
public:
	virtual void Decide(const DataObject &input, DataObject &output)
	{
		bool found = inArray(input, landArray, 5);
		output = DataObject(found);
	}
};

IMPLEMENT_LOGIC_MIXIN(LogicTileIsResourceStandard, LogicTileIsResource, STANDARD);
