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
// Namespace: Network
// 
// Network utility functions.
//
// Include:
//     Network.h
//
namespace Network
{
	// Retrieves the mac address of the user's computer.
	void GetMacAddress(wxString& strMacAddress);

	// Retrieves all IP addresses associated with the user's computer.
	void GetIPAddresses(StringArray& i_addresses);

	// Fixes any special characters the site puts into the player name
	// for transmission (i.e. %20 for space)
	inline void FixNetworkString(const wxString& i_name, wxString& o_name)
	{
		o_name = i_name;
		o_name.Replace(swPercent20, swSpace);
		o_name.Replace(swPercent22, swQuotation);
	}

	// Splits the user name and id from the given string.
	inline bool SplitNameAndId(const wxString& i_str, wxString& o_name, 
		wxInt32& o_id)
	{
		bool ret = false;

		int index = i_str.Find(':');
		if(-1 != index)
		{
			// Split the string on the ':' character to get name and id.
			o_name = i_str.Left(index);
			FixNetworkString(o_name, o_name);
			wxString id = i_str.Right(i_str.Length() - index - 1);
			long idVal;
			id.ToLong(&idVal);
			o_id = idVal;
			ret = true;
		}

		return ret;
	}
}

//---------------------------- PROTOTYPES       -----------------------------//

