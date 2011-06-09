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
#include "OptionDatabase.h"
#include "IOptionUI.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
class VolcanoOption : public IOptionUI
{
public:
	virtual void dependencies(StringPairArray&) const
	{
	}

	virtual void silent_dependencies(StringPairArray& array) const
	{
		array.push_back(std::make_pair(swStandardRules, swGold));
		array.push_back(std::make_pair(swVolcanoRules, swVolcano));
	}

	virtual void exclusions(StringPairArray&) const
	{
	}

	virtual const wxString& description() const
	{
		return stVolcanoLongDescription;
	}

	virtual wxInt32 priority() const
	{
		return 0;
	}

	virtual wxString short_description() const
	{
		return stDesertsAreVolcanoes;
	}

	virtual bool HasUI() const
	{
		return false;
	}
};

IMPLEMENT_OPTION_UI(VOLCANO_DESERT, VolcanoOption);

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
