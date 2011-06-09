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

//---------------------------- USER INCLUDES    -----------------------------//
#include "synchronization/Synchronization.h"
#include "network/Network.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	void CreateSynchronizationPath(wxString& strPath, const wxString& base)
	{
		// Create the name of the synchronization file based on the mac address.
		static wxString macAddress;

		Network::GetMacAddress(macAddress);

		wxString str = wxString::Format(base, macAddress.c_str());

		// Now that we have the file name, create the path.
		wxString dir = wxStandardPaths::Get().GetUserConfigDir();

		wxFileName fileName(dir, str);
		strPath = fileName.GetFullPath();
	}
}

//---------------------------- PUBLIC           -----------------------------//
void Synchronization::CreateSynchronizationPathToCities3D(wxString& strPath)
{
	CreateSynchronizationPath(strPath, swSynchronizeSyn);
}

void Synchronization::CreateSynchronizationPathToCitiesOnline(wxString& strPath)
{
	CreateSynchronizationPath(strPath, swSynchronizeNys);
}
//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
