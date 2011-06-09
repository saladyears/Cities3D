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
#pragma once

#include "style.h"	//READ THIS BEFORE MAKING ANY CHANGES TO THIS FILE!!!
//---------------------------- SYSTEM INCLUDES  -----------------------------//
#include <map>

//---------------------------- USER INCLUDES    -----------------------------//
#include "LocalSingleton.h"

//---------------------------- TYPEDEFS         -----------------------------//
class IOptionUI;
typedef boost::shared_ptr<IOptionUI> OptionUIPtr;
typedef std::map<wxString, OptionUIPtr> OptionUIMap;

#ifndef __WIN32__
	#pragma GCC visibility push(hidden)
	class OptionDatabase;
	template class LocalSingleton<OptionDatabase>;
	#pragma GCC visibility pop
#endif

//---------------------------- CLASSES          -----------------------------//
class DLL_LOCAL OptionDatabase : public LocalSingleton<OptionDatabase>
{
public:
	const OptionUIMap &options() const {return mOptionUIMap;}

	void RegisterOptionUI(const wxString& ruleset, const OptionUIPtr& option);
	
private:
	OptionUIMap mOptionUIMap;

	friend class LocalSingleton<OptionDatabase>;
};

//---------------------------- DEFINES          -----------------------------//
#define IMPLEMENT_OPTION_UI(ruleset, option) \
namespace \
{ \
	struct sAutoRegister##option \
	{ \
		sAutoRegister##option(IOptionUI* ui) \
		{ \
			OptionDatabase::get().RegisterOptionUI(ruleset, \
				OptionUIPtr(ui)); \
		} \
	}; \
	sAutoRegister##option object##option(new option); \
}
			
//---------------------------- PROTOTYPES       -----------------------------//
