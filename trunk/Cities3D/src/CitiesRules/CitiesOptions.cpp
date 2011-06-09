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
class CitiesOptions : public IOptionUI
{
public:
	virtual void dependencies(StringPairArray&) const
	{
	}

	virtual void silent_dependencies(StringPairArray&) const
	{
	}

	virtual void exclusions(StringPairArray& array) const
	{
		array.push_back(std::make_pair(swStandardRules, swDevCards));
		array.push_back(std::make_pair(swStandardRules, swDevelopmentCards));
	}

	virtual const wxString& description() const
	{
		return stCitiesAndKnightsIsAWholeNewWay;
	}

	virtual wxInt32 priority() const
	{
		return 0;
	}

	virtual wxString short_description() const
	{
		return stCitiesAndKnightsPlus3;
	}

	virtual bool HasUI() const
	{
		return false;
	}
};

IMPLEMENT_OPTION_UI(CITIES, CitiesOptions);

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
