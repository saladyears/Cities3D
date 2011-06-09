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
#include "RulesExport.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class wxCmdLineParser;
class Map;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: ICommand
// 
// Interface for a command to be executed by the game engine.  A command is
// basically a wrapper for CitiesOnline site commands.
//
// Project:
//     <RulesCore>
//
// Include:
//     ICommand.h
//
class RULES_EXPORT_SPEC ICommand
{
public:
	virtual ~ICommand()=0;

	virtual void PrepareParser(wxCmdLineParser& parser) const=0;

	virtual void Execute(const wxCmdLineParser& parser, 
		GamePtr& game, bool inStartup)=0;
};

//---------------------------- PROTOTYPES       -----------------------------//
inline ICommand::~ICommand() {}
