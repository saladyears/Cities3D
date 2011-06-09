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

#ifdef __linux__ 

//---------------------------- SYSTEM INCLUDES  -----------------------------//
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <net/if.h>

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

	int sock = socket(AF_INET, SOCK_DGRAM, 0);

	if(-1 != sock)
	{
		struct ifreq ifr;
		struct ifconf ifc;
		struct ifreq *IFR;
		char buf[1024];

		ifc.ifc_len = sizeof(buf);
		ifc.ifc_buf = buf;
		ioctl(sock, SIOCGIFCONF, &ifc);

		IFR = ifc.ifc_req;
		for(int i = ifc.ifc_len / sizeof(struct ifreq); --i >= 0; IFR++)
		{
			strcpy(ifr.ifr_name, IFR->ifr_name);
			if(0 == ioctl(sock, SIOCGIFFLAGS, &ifr))
			{
				if(ifr.ifr_flags & IFF_LOOPBACK)
				{
					// we don't need the loopback device
					continue;
				}

				if(0 == ioctl(sock, SIOCGIFHWADDR, &ifr))
				{
					unsigned char addr[7];

					addr[6] = '\0';
					bcopy(ifr.ifr_hwaddr.sa_data, &addr, 6);

					for(unsigned int c = 0; c < 6; ++c)
					{
						if(0 != c)
						{
							strMacAddress += wxT("-");
						}

						unsigned char c1 = addr[c] & 0xF;
						unsigned char c2 = addr[c] >> 4;

						strMacAddress += (c1 < 10) ? '0' + c1 : 'A' - 10 + c1;
						strMacAddress += (c2 < 10) ? '0' + c2 : 'A' - 10 + c2;
					}

					break;
				}
			}
		}

		close(sock);
	}

	if(true == strMacAddress.IsEmpty())
	{
		// Things are hosed, just make up something.
		strMacAddress = wxT("no_adapter");
	}
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

#endif // __linux__

