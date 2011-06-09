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

#ifdef _WIN32

//---------------------------- SYSTEM INCLUDES  -----------------------------//
#include <Iphlpapi.h>
#include <boost/scoped_ptr.hpp>

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
	ULONG size = sizeof(IP_ADAPTER_INFO);

	// Start with room for a single adapter.
	boost::scoped_ptr<char> buffer(new char[size]);
	IP_ADAPTER_INFO* info = reinterpret_cast<IP_ADAPTER_INFO*>(buffer.get());

attempt:
	DWORD err = GetAdaptersInfo(info, &size);

	// Determine if we have enough room for the adapter information.
	if(ERROR_BUFFER_OVERFLOW == err)
	{
		// They have more than one adapter, resize the buffer.
		buffer.reset(new char[size]);
		info = reinterpret_cast<IP_ADAPTER_INFO*>(buffer.get());
		goto attempt;
	}
	else if(NO_ERROR == err)
	{
		wxString hostIP;

		// Get the ip address of this machine.
		char hostname[1024];
		if(0 == gethostname(hostname, 1024))
		{
			hostent* host = gethostbyname(hostname);
			if(NULL != host)
			{
				char** addr_list = host->h_addr_list;
				while(NULL != *addr_list)
				{
					in_addr* addr = reinterpret_cast<in_addr*>(*addr_list);
					char* localIP = inet_ntoa(*addr);
				
					// This will use the last reported IP address to find the
					// network adapter.
					hostIP = wxString(localIP, wxConvLibc, strlen(localIP));

					++addr_list;
				}
			}
		}
		
		// We've got the adapter, use the information.
		IP_ADAPTER_INFO* adapter = info;
		
		while( (NULL != adapter) && (true == strMacAddress.IsEmpty()) )
		{
			// We need an ethernet adapter.
			if(MIB_IF_TYPE_ETHERNET == adapter->Type)
			{
				// Look for the address that windows reports as our own.
				_IP_ADDR_STRING* ip = &adapter->IpAddressList;

				while(NULL != ip)
				{
					wxString ipAddress = wxString(ip->IpAddress.String, 
						wxConvLibc, strlen(ip->IpAddress.String));

					// If we didn't get a host IP for some reason, just use the
					// first adapter.
					if( (true == hostIP.IsEmpty()) ||
						(hostIP == ipAddress))
					{
						for(unsigned int i = 0; i < adapter->AddressLength; ++i)
						{
							if(0 != i)
							{
								strMacAddress += swDash;
							}

							unsigned char c = adapter->Address[i];
							unsigned char c1 = c / 16;
							unsigned char c2 = c % 16;

							strMacAddress += (c1 < 10) ? '0' + c1 : 'A' - 10 + c1;
							strMacAddress += (c2 < 10) ? '0' + c2 : 'A' - 10 + c2;
						}

						break;
					}

					ip = ip->Next;
				}
			}

			adapter = adapter->Next;
		}
	}
	
	if(true == strMacAddress.IsEmpty())
	{
		// Things are hosed, just make up something.
		strMacAddress = swNoAdapter;
	}
}

void Network::GetIPAddresses(StringArray& i_addresses)
{
	// Get the ip address of this machine.
	char hostname[1024];
	if(0 == gethostname(hostname, 1024))
	{
		hostent* host = gethostbyname(hostname);
		if(NULL != host)
		{
			char** addr_list = host->h_addr_list;
			while(NULL != *addr_list)
			{
				in_addr* addr = reinterpret_cast<in_addr*>(*addr_list);
				char* localIP = inet_ntoa(*addr);

				i_addresses.push_back(wxString(localIP, wxConvLibc, 
					strlen(localIP)));

				++addr_list;
			}
		}
	}
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//

#endif // _WIN32

