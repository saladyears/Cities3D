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
#include "ITransientCommand.h"
#include "Player.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: TransientSpectateCommand
//
// Handles actually connecting to a game, which must happen in the main event
// loop for sockets to work correctly.
// 
// Derived From:
//     <ITransientCommand>
//
// Project:
//     <CitiesOnlineRules>
//
// Include:
//     TransientSpectateCommand.h
//
class TransientSpectateCommand : public ITransientCommand
{
public:
	TransientSpectateCommand(const Player& player, const wxIPV4address& address)
		: mPlayer(player)
		, mAddress(address)
	{
	}

	virtual ~TransientSpectateCommand() {}

	virtual void Execute();

private:
	Player mPlayer;
	wxIPV4address mAddress;
};

//---------------------------- PROTOTYPES       -----------------------------//

