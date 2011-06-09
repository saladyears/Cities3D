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

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: ITransientCommand
// 
// Interface for a transient command to be executed by the synchronizer. 
// Transient commands exist because a command may need to wait until the full
// application message loop is established before executing.
//
// Project:
//     <CitiesOnlineRules>
//
// Include:
//     ITransientCommand.h
//
class ITransientCommand
{
public:
	virtual ~ITransientCommand()=0;

	virtual void Execute()=0;
};

//---------------------------- PROTOTYPES       -----------------------------//
inline ITransientCommand::~ITransientCommand() {}

typedef boost::shared_ptr<ITransientCommand> TransientCommandPtr;
