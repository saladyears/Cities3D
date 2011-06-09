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
struct IEnumVARIANT;
struct IStaticPortMappingCollection;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxPortForwardThread
// 
// Handles UPnP for the application.
//
// Derived From:
//     wxThread
//
// Project:
//     <CitiesOnline>
//
// Include:
//     PortForwardThread.h
//
class wxPortForwardThread : public wxThread
{
public:
	wxPortForwardThread();

	// Blocks until the thread is no longer busy.  Returns -1 if no valid
	// port has been forwarded.
	long GetPort() const;

	virtual void* Entry();

private:
	typedef std::pair<wxString, wxString> StringPair;

	struct PortMapping
	{
		// For use in std::find.
		bool operator==(const StringPair& pair) const
		{
			return (pair.first == description) && 
				(pair.second == internalClient);
		}
		
		bool operator==(const wxString& localIP) const
		{
			return localIP == internalClient;
		}

		bool operator==(long port) const
		{
			return port == externalPort;
		}

		wxString externalAddress;
		wxString protocol;
		wxString internalClient;
		wxString description;
		long externalPort;
		long internalPort;
		bool enabled;
	};

	void ForwardPort();
	
	void UpdateMappings(IStaticPortMappingCollection*& collection);
	bool GetNextMapping(IEnumVARIANT* piEnumerator, PortMapping& mapping);

	bool DetermineLocalIP(wxString& i_ip);

	void AddPortMapping(IStaticPortMappingCollection* collection, 
		const wxString& i_localIp);
	long FindOpenPort();
	
	// Our current list of mappings.
	typedef std::vector<PortMapping> PortMappingArray;
	PortMappingArray mPortMappings;

	long mPort;
	bool mIsBusy;
};
//---------------------------- PROTOTYPES       -----------------------------//

