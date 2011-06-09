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

#if defined(__MACH__) && defined(__APPLE__)

//---------------------------- SYSTEM INCLUDES  -----------------------------//

//---------------------------- USER INCLUDES    -----------------------------//
#include "network/Network.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
void Network::GetMacAddress(wxString& strMacAddress)
{
	if(false == strMacAddress.IsEmpty())
	{
		// we have already got the mac address
		return;
	}

   // FIXME: implement me!

	strMacAddress = wxT("no_adapter");
}

void Network::GetIPAddresses(StringArray& i_addresses)
{
	// FIXME: this is probably not what we want
	wxIPV4address addr;
	addr.Hostname(wxGetFullHostName());
	i_addresses.push_back(addr.IPAddress());
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//

#endif // __MACH__ && __APPLE__

