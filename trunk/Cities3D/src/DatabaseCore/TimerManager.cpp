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
#include "TimerManager.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
void TimerManager::Stop()
{
	// Kill all timers.
	TimerMap::const_iterator it, itEnd = mTimerMap.end();
	for(it = mTimerMap.begin(); it != itEnd; ++it)
	{
		it->second->Stop();
	}

	// Clear everything.
	mTimerMap.clear();
	mPeriodicMap.clear();
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(TimerManager, wxEvtHandler)
	EVT_TIMER(0, TimerManager::OnPeriodicTimer)
END_EVENT_TABLE()

TimerManager::~TimerManager()
{
	Stop();
}

void TimerManager::OnPeriodicTimer(wxTimerEvent &event)
{
	// Get the resolution out of the event, which is the key to everything.
	size_t resolution = event.GetInterval();

	PeriodicMap::iterator itMap = mPeriodicMap.find(resolution);
	
	// Sanity check.
	if(mPeriodicMap.end() == itMap)
	{
		wxASSERT(false);
		mPeriodicMap.erase(itMap);

		mTimerMap[resolution]->Stop();
		mTimerMap.erase(mTimerMap.find(resolution));
	}
	else
	{
		PeriodicFunctionList &periodics = itMap->second;
			
		// Fire the events.
		PeriodicFunctionList::iterator it = periodics.begin(), itThis, 
			itEnd = periodics.end();
		while(it != itEnd)
		{
			itThis = it++;
			
			if ((*itThis).first)
			{
				it = periodics.erase(itThis);
			}
			else
			{
				(*itThis).second();
			}
		}

		// Remove the entry if there's nothing left.
		if(true == periodics.empty())
		{
			mPeriodicMap.erase(itMap);

			mTimerMap[resolution]->Stop();
			mTimerMap.erase(mTimerMap.find(resolution));
		}
	}
}
