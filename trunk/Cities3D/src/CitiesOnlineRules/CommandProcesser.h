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

//---------------------------- USER INCLUDES    -----------------------------//
#include "Singleton.h"

//---------------------------- TYPEDEFS         -----------------------------//
class ICommand;
typedef boost::shared_ptr<ICommand> CommandPtr;

typedef std::pair<wxString, wxString> StringPair;
typedef std::vector<StringPair> StringPairArray;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: CommandProcesser
// 
// Processes commands from CitiesOnline.
//
// Derived From:
//     <Singleton>
//
// Project:
//     <CitiesOnlineRules>
//
// Include:
//     CommandProcesser.h
//
class CommandProcesser : public Singleton<CommandProcesser>
{
public:
	void RegisterCommand(const wxString& key, const CommandPtr& command);
	void ProcessCommand(const wxString& command, bool inStartup);

    void StoreUISetting(const wxString& element, const wxString& value)
    {
        mUISettings.push_back(StringPair(element, value));
    }
    
    const StringPairArray& GetUISettings() const
    {
        return mUISettings;
    }

    void ClearUISettings()
    {
        mUISettings.clear();
    }

private:
	typedef std::map<wxString, CommandPtr> CommandMap;
	CommandMap mCommandMap;

    StringPairArray mUISettings;
};

//---------------------------- PROTOTYPES       -----------------------------//

//---------------------------- DEFINES          -----------------------------//
#define REGISTER_COMMAND(Command, Key) \
namespace \
{ \
	struct sAutoRegister##Command \
	{ \
		sAutoRegister##Command(const wxString& key) \
		{ \
			CommandProcesser::get().RegisterCommand(key, CommandPtr(new Command)); \
		} \
	}; \
	sAutoRegister##Command object##Command(wxT(#Key)); \
}
