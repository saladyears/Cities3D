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
#include <wx/cmdline.h>

//---------------------------- USER INCLUDES    -----------------------------//
#include "CommandProcesser.h"
#include "GameEngine.h"
#include "ICommand.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
void CommandProcesser::RegisterCommand(const wxString& key, 
	const CommandPtr& command)
{
	mCommandMap[key] = command;
}

void CommandProcesser::ProcessCommand(const wxString& command, bool inStartup)
{
	wxCmdLineParser parser(command);

	// Add our options.
	CommandMap::iterator it, itEnd = mCommandMap.end();
	for(it = mCommandMap.begin(); it != itEnd; ++it)
	{
		const CommandPtr& command = it->second;
		command->PrepareParser(parser);
	}

	// Wrap this in NULL log output so no information is leaked.
	{
		wxLogNull log;
		parser.Parse(false);
	}

	wxString data;

	// Iterate through the commands, looking for any that were parsed.
	for(it = mCommandMap.begin(); it != itEnd; ++it)
	{
		const wxString& key = it->first;
		if(true == parser.Found(key, &data))
		{
			GameEngine::get().ExecuteCommand(parser, it->second, inStartup);
		}
	}
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
