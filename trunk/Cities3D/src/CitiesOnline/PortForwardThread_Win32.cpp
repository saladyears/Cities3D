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
#include <Rpcsal.h>
#include <Natupnp.h>
#include <UPnP.h>
#include <AtlConv.h>

//---------------------------- USER INCLUDES    -----------------------------//
#include "PortForwardThread.h"
#include "Network.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	IUPnPNAT* spNAT = NULL;
	INATEventManager* spEventManager = NULL;
	bool sRouterUpdate = false;

	interface IDerivedNATNumberOfEntriesCallback : public INATNumberOfEntriesCallback
	{
		IDerivedNATNumberOfEntriesCallback() 
			: mRefs(0) 
		{};

		virtual HRESULT STDMETHODCALLTYPE NewNumberOfEntries( long lNewNumberOfEntries )
		{
			sRouterUpdate = true;

			return S_OK;
		}

		virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void ** ppvObject)
		{
			HRESULT hr = S_OK;
			*ppvObject = NULL;

			if ( iid == IID_IUnknown ||	iid == IID_INATNumberOfEntriesCallback )
			{
				*ppvObject = this;
				AddRef();
				hr = NOERROR;
			}
			else
			{
				hr = E_NOINTERFACE;
			}

			return hr;
		}

		virtual ULONG STDMETHODCALLTYPE AddRef()  
		{ 
			return ++mRefs; 
		}

		virtual ULONG STDMETHODCALLTYPE Release()
		{
			DWORD refs = --mRefs;

			if(refs == 0)
			{
				delete this;
			}

			return refs;
		}

		DWORD mRefs;
	};

	void AddListener()
	{
		if( (SUCCEEDED(CoCreateInstance(__uuidof(UPnPNAT), NULL, CLSCTX_ALL, 
			__uuidof(IUPnPNAT), (void **)&spNAT))) &&
			(NULL != spNAT))
		{
			// Get the INATEventManager interface

			if( (SUCCEEDED(spNAT->get_NATEventManager(&spEventManager))) &&
				(NULL != spEventManager))
			{
				spEventManager->put_NumberOfEntriesCallback( 
					new IDerivedNATNumberOfEntriesCallback);
			}
		}
	}

	void RemoveListener()
	{
		if(NULL != spEventManager)
		{
			spEventManager->Release();
			spEventManager = NULL;
		}

		if(NULL != spNAT)
		{
			spNAT->Release();
			spNAT = NULL;
		}
	}	
}

//---------------------------- PUBLIC           -----------------------------//
wxPortForwardThread::wxPortForwardThread()
: mIsBusy(false)
, mPort(-1)
{
}

long wxPortForwardThread::GetPort() const
{
	// As this function is called from another thread, it is OK to sleep here,
	// we're not sleeping this thread.
	while(true == mIsBusy)
	{
		Sleep(100);
	}

	return mPort;
}

void* wxPortForwardThread::Entry()
{
	// Start up COM.
	CoInitialize(NULL);

	// Add a listener to the router for changes.
	AddListener();
	
	// Attempt to set up the port forward.
	ForwardPort();

	while(false == TestDestroy())
	{
		// Listen for router changes.
		if(true == sRouterUpdate)
		{
			ForwardPort();
			sRouterUpdate = false;
		}

		Sleep(100);
	}

	// Clean up.
	RemoveListener();

	// We're done with COM.
	CoUninitialize();

	return NULL;
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void wxPortForwardThread::ForwardPort()
{
	mIsBusy = true;

	IStaticPortMappingCollection* collection = NULL;

	// First, retrieve all our mappings from the router.
	UpdateMappings(collection);

	// Determine our local IP address.
	wxString localIP;
	if(true == DetermineLocalIP(localIP))
	{
		StringPair pair(swCitiesOnlineTCP, localIP);

		// Now see if we have a mapping already for our game.
		PortMappingArray::const_iterator it = std::find(mPortMappings.begin(), 
			mPortMappings.end(), pair);
		
		// If we did not find it, we need to add it.
		if(mPortMappings.end() == it)
		{
			AddPortMapping(collection, localIP);
		}
		// Otherwise, our port is the port specified in the mapping.
		else
		{
			mPort = it->externalPort;
		}
	}

	mIsBusy = false;
}

void wxPortForwardThread::UpdateMappings(IStaticPortMappingCollection*& collection)
{
	// Reset the array.
	mPortMappings.clear();
	collection = NULL;

	// Create the COM instance of IUPnPNAT.
	IUPnPNAT* piNAT = NULL;
	PortMapping mapping;

	if(FAILED(CoCreateInstance(__uuidof(UPnPNAT), NULL, CLSCTX_ALL, 
		__uuidof(IUPnPNAT), (void **)&piNAT)))
	{
		return;
	}

	// Get the collection of forwarded ports.
	if(FAILED(piNAT->get_StaticPortMappingCollection(&collection)))
	{
		return;
	}

	// It is possible for get_StaticPortMappingCollection to return S_OK and
	// still have the mapping collection as NULL.  On my machine, this was
	// because I needed to enable the "Show icons for networked UPnP devices
	// in My Network Places".
	if(NULL == collection)
	{
		return;
	}

	// Get the mapping enumerator and reset the list of mappings
	IUnknown* piUnk = NULL;

	if(FAILED(collection->get__NewEnum(&piUnk)))
	{
		return;
	}

	IEnumVARIANT* piEnumerator = NULL;

	if(FAILED(piUnk->QueryInterface(IID_IEnumVARIANT, 
		(void **)&piEnumerator)))
	{
		return;
	}

	if(FAILED(piEnumerator->Reset()))
	{
		return;
	}

	// Retrieve all of the mappings.
	while(true == GetNextMapping(piEnumerator, mapping))
	{
		mPortMappings.push_back(mapping);
	}
}

bool wxPortForwardThread::GetNextMapping(IEnumVARIANT* piEnumerator, 
	PortMapping& mapping)
{
	bool ret = true;
	IStaticPortMapping* piMapping = NULL;

	VARIANT currentMapping;
	VariantInit(&currentMapping);

	if( (FAILED(piEnumerator->Next( 1, &currentMapping, NULL))) ||
		(VT_EMPTY == currentMapping.vt))
	{
		ret = false;
		goto done;
	}

	IDispatch* piDispMap = V_DISPATCH(&currentMapping);
	if(FAILED(piDispMap->QueryInterface(IID_IStaticPortMapping, 
		(void **)&piMapping)))
	{
		ret = false;
		goto done;
	}

	// Retrieve external address.
	BSTR bStr = NULL;

	if(FAILED(piMapping->get_ExternalIPAddress(&bStr)))
	{
		ret = false;
		goto done;
	}
	
	if(NULL != bStr)
	{
		mapping.externalAddress = wxString(OLE2T(bStr));
	}
	SysFreeString(bStr);
	bStr = NULL;

	// Retrieve external port.
	if(FAILED(piMapping->get_ExternalPort(&mapping.externalPort)))
	{	
		ret = false;
		goto done;
	}

	// Retrieve internal port.
	if(FAILED(piMapping->get_InternalPort(&mapping.internalPort)))
	{	
		ret = false;
		goto done;
	}

	// Retrieve protocol.
	if(FAILED(piMapping->get_Protocol(&bStr)))
	{
		ret = false;
		goto done;
	}

	if(NULL != bStr)
	{
		mapping.protocol = wxString(OLE2T(bStr));
	}
	SysFreeString(bStr);
	bStr = NULL;

	// Retrieve internal client.
	if(FAILED(piMapping->get_InternalClient(&bStr)))
	{
		ret = false;
		goto done;
	}

	if(NULL != bStr)
	{
		mapping.internalClient = wxString(OLE2T(bStr));
	}
	SysFreeString(bStr);
	bStr = NULL;

	// Retrieve enabled.
	VARIANT_BOOL enabled = VARIANT_FALSE;
	
	if(FAILED(piMapping->get_Enabled( &enabled )))
	{
		ret = false;
		goto done;
	}

	mapping.enabled = (enabled == VARIANT_TRUE);

	// Retrieve description.
	if(FAILED(piMapping->get_Description(&bStr)))
	{
		ret = false;
		goto done;
	}

	if(NULL != bStr)
	{
		mapping.description = wxString(OLE2T(bStr));
	}
	SysFreeString(bStr);
	bStr = NULL;

done:
	if(NULL != piMapping)
	{
		piMapping->Release();
		piMapping = NULL;
	}

	return ret;
}

bool wxPortForwardThread::DetermineLocalIP(wxString& i_ip)
{
	bool ret = true;
	i_ip.Empty();

	StringArray addresses;
	Network::GetIPAddresses(addresses);

	// If we only have one, we're golden.
	size_t size = addresses.size();
	if(1 == size)
	{
		i_ip = addresses[0];
	}
	// If we have more than one, we need to determine which one is the
	// best to use.
	else if(1 < size)
	{
		size_t internalCount = 0;
		std::vector<bool> internalArray(size, false);

		// Go through our existing port forwards and see if any of them
		// are already in use.  If so, use it.
		for(size_t i = 0; i < size; ++i)
		{
			const wxString& ip = addresses[i];

			PortMappingArray::const_iterator it = std::find(
				mPortMappings.begin(), mPortMappings.end(), ip);

			// We found it.
			if(mPortMappings.end() != it)
			{
				i_ip = ip;
				break;
			}

			// Keep track of any that are 192.168.0.x or 192.168.1.x
			if( (-1 != ip.Find(sw1921681)) ||
				(-1 != ip.Find(sw1921680)))
			{
				++internalCount;
				internalArray[i] = true;
			}
		}

		// If we did not find one, we'll have to guess.
		if(true == i_ip.IsEmpty())
		{
			size_t index = 0;

			if(0 < internalCount)
			{
				// Use the first one.
				index = std::distance(internalArray.begin(), 
					std::find(internalArray.begin(), internalArray.end(), 
					true));
			}

			i_ip = addresses[index];
		}

	}
	// If we have none, something is horribly wrong.
	else
	{
		ret = false;
	}

	return ret;
}

void wxPortForwardThread::AddPortMapping(IStaticPortMappingCollection* collection,
	const wxString& i_localIp)
{
	long port = -1;
	long attempt = FindOpenPort();
	
	if( (NULL != collection) &&
		(-1 != attempt))
	{
		PortMapping mapping;
		mapping.protocol = swTCP;
		mapping.internalClient = i_localIp;
		mapping.description = swCitiesOnlineTCP;
		mapping.externalPort = attempt;
		mapping.internalPort = attempt;
		mapping.enabled = true;

		IStaticPortMapping* portMapping = NULL;

		if(SUCCEEDED(collection->Add(
			mapping.externalPort,
			T2OLE((LPTSTR)mapping.protocol.c_str()),
			mapping.internalPort,
			T2OLE((LPTSTR)mapping.internalClient.c_str()),
			mapping.enabled ? VARIANT_TRUE : VARIANT_FALSE,
			T2OLE((LPTSTR)mapping.description.c_str()),
			&portMapping))
			)
		{
			port = attempt;
		}
	}

	mPort = port;
}

long wxPortForwardThread::FindOpenPort()
{
	long value = 7176;
	long ret = -1;

	// This is completely arbritrary
	while(value < 8000)
	{
		// Skip 7177 because that is the port Cities Online uses internally.
		if(7177 != value)
		{
			PortMappingArray::const_iterator it = std::find(
				mPortMappings.begin(), mPortMappings.end(), value);

			// Found an open one.
			if(mPortMappings.end() == it)
			{
				ret = value;
				break;
			}
		}

		++value;
	}
	
	return ret;
}

#endif

