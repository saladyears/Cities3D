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
#include "Singleton.h"

//---------------------------- USER INCLUDES    -----------------------------//

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class wxPortForwardThread;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: PortForwardManager
// 
// Manages the port forward thread which sets up UPnP support for the game.
//
// Derived From:
//     <Singleton>
//
// Project:
//     <CitiesOnline>
//
// Include:
//     PortForwardManager.h
//
class PortForwardManager : public Singleton<PortForwardManager>
{
public:
	// This function blocks internally until the wxPortForwardThread is no
	// longer busy, and returns the port that the thread has opened, or
	// port 7176 as the default if the thread is unsuccessful.
	long GetPort() const;

	void Initialize();

	// This function blocks internally to give the wxPortForwardThread time to
	// unmap any mapping it has created.
	void Shutdown();

private:
	void SetPort(wxUint32 port);

	PortForwardManager();

	mutable long mPort;
	wxPortForwardThread* mpPortForwardThread;

	friend class wxPortForwardThread;
	friend class Singleton<PortForwardManager>;
};
//---------------------------- PROTOTYPES       -----------------------------//

