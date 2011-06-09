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
#ifdef __WIN32__
#define wxSOCKET_REUSEADDR_FLAG 0
#else 
#define wxSOCKET_REUSEADDR_FLAG wxSOCKET_REUSEADDR
#endif

//---------------------------- TYPEDEFS         -----------------------------//
class wxLocalServerThread;
class wxHostCheckServerThread;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxCitiesOnlineServer
// 
// A small socket server that handles interprocess communication between 
// CitesOnline instances, as well as host checks from the site.
//
// Derived From:
//     wxEvtHandler
//
// Project:
//     <CitiesOnline>
//
// Include:
//     CitiesOnlineServer.h
//
class wxCitiesOnlineServer : public wxEvtHandler
{
public:
	wxCitiesOnlineServer();
	~wxCitiesOnlineServer();

	static bool SendMessage(const wxString& message);

	void SetHostThreadDone()
	{
		mpHostCheckServerThread = NULL;
	}

	void Hostcheck(long port);

private:
	wxLocalServerThread* mpLocalServerThread;
	wxHostCheckServerThread* mpHostCheckServerThread;
};

//---------------------------- PROTOTYPES       -----------------------------//

