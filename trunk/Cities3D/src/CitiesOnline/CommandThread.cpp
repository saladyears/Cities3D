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
#include <wx/url.h>

//---------------------------- USER INCLUDES    -----------------------------//
#include "CommandThread.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	const size_t COMMAND_RESOLUTION = 250;
}

//---------------------------- PUBLIC           -----------------------------//
wxCommandThread::wxCommandThread()
: mLock(wxMUTEX_RECURSIVE)
{
}

void wxCommandThread::SendCommands(const StringArray& commands)
{
	// Lock the commands so they can't be processed until they are safely
	// copied.
	wxMutexLocker lock(mLock);

	std::copy(commands.begin(), commands.end(), std::back_inserter(mCommands));
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void* wxCommandThread::Entry()
{
	// Run until told not to.
	while(false == TestDestroy())
	{
		// If we have commands, lock the critical section and process them.
		if(false == mCommands.empty())
		{
			wxMutexLocker lock(mLock);

			StringList::const_iterator it, itEnd = mCommands.end();
			for(it = mCommands.begin(); it != itEnd; ++it)
			{
				// Call the site to start it going.
				wxURL url(*it);
				if(true == url.IsOk())
				{
					// Set the timeout to a second, so we don't waste time
					// waiting for page contents.
					wxProtocol& protocol = url.GetProtocol();
					protocol.SetTimeout(1);

					// Get the input stream, which calls the page.  Since we 
					// only need to call the page, we care nothing about any 
					// contents of the stream.
					url.GetInputStream();
				}
			}

			// Now that we've processed them all, clear them out.
			mCommands.clear();
		}

		Sleep(COMMAND_RESOLUTION);
	}	

	return NULL;
}

