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
#include <map>
#include <list>
#include <boost/bind.hpp>
#include <boost/function.hpp>

//---------------------------- USER INCLUDES    -----------------------------//
#include "DatabaseExport.h"
#include "Singleton.h"

//---------------------------- DEFINES          -----------------------------//
#define TIME	TimerManager::get()

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: TimerManager
//
// Manages all timers in the game, grouping them by resolution to avoid
// ridiculous amounts of timers.
//
// Derived From:
//     <Singleton>
//     <wxEvtHandler>
//
// Project:
//     <DatabaseCore>
//
// Include:
//     TimerManager.h
//
class DATABASE_EXPORT_SPEC TimerManager 
	: public wxEvtHandler
	, public Singleton<TimerManager>
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: AddPeriodicFunction
	//
	// Adds a periodic function to the timer manager at the given resolution.
	//
	// Parameters:
	//     func - The function pointer.
	//     object - The class object.
	//     resolution - The desired timer resolution, in milliseconds.
	//
	template <typename R>
	void AddPeriodicFunction(void (R::*func)(), R* object, 
		const size_t resolution)
	{
		wxLogDebug(wxT("TimerManager: Adding periodic function with "
					wxT("resolution %d.")), resolution);

		// See if we have a timer already created for this resolution.
		TimerMap::const_iterator it = mTimerMap.find(resolution);
		if(mTimerMap.end() == it)
		{
			// Create the timer and start it up.
			TimerPtr timer = TimerPtr(new wxTimer(this, 0));
			mTimerMap[resolution] = timer;
			timer->Start(resolution, wxTIMER_CONTINUOUS);
		}

		// Add the function to the list at this resolution.
		mPeriodicMap[resolution].push_back(
			std::make_pair(false, 
			Event0(boost::bind(func, object), object)));
	}

	//-----------------------------------------------------------------------//
	// Function: RemovePeriodicFunction
	//
	// Removes a periodic function from the timer manager at the given 
	// resolution.
	//
	// Parameters:
	//     func - The function pointer.
	//     object - The class object.
	//     resolution - The desired timer resolution, in milliseconds.
	//
	template <typename R>
	void RemovePeriodicFunction(void (R::*func)(), R* object, 
		const size_t resolution)
	{
		wxLogDebug(wxT("TimerManager: Removing periodic function with "
					wxT("resolution %d.")), resolution);

		Event0 event(boost::bind(func, object), object);

		// Search for the event in the function list.
		PeriodicMap::iterator itMap = mPeriodicMap.find(resolution);

		if(mPeriodicMap.end() != itMap)
		{
			PeriodicFunctionList &periodics = itMap->second;
			PeriodicFunctionList::iterator it;
			for (it = periodics.begin(); it != periodics.end(); ++it)
			{
				if (it->second == event)
				{
					break;
				}
			}
			if(periodics.end() != it)
			{
				(*it).first = true;
			}
		}
	}

	//-----------------------------------------------------------------------//
	// Function: AddOneShotFunction
	//
	// Adds a oneshot function to the timer manager at the given resolution.
	//
	// Parameters:
	//     func - The function pointer.
	//     object - The class object.
	//     resolution - The desired timer resolution.
	//     removeCopies - Removes any already existing copy of the one-shot
	//                    function.
	//
	template <typename R>
	void AddOneShotFunction(void (R::*func)(), R* object, 
		const size_t resolution, bool removeCopies = true)
	{
		Event0 event(boost::bind(func, object), object);

		// Remove any currently existing one shot function that matches this
		// event and resolution.
		if(true == removeCopies)
		{
			RemoveOneShotFunction(event, resolution);
		}

		wxLogDebug(wxT("TimerManager: Adding one-shot function with "
					wxT("resolution %d.")), resolution);

		// Create the one-shot timer and fire it up.
		OneShotTimerPtr timer(new wxOneShotTimer(event));
		timer->Start(resolution, wxTIMER_ONE_SHOT);
		mOneShotList.push_back(timer);
	}

	//-----------------------------------------------------------------------//
	// Function: RemoveOneShotFunction
	//
	// Removes a oneshot function from the timer manager at the given 
	// resolution.
	//
	// Parameters:
	//     func - The function pointer.
	//     object - The class object.
	//     resolution - The desired timer resolution.
	//
	template <typename R>
	void RemoveOneShotFunction(void (R::*func)(), R* object, 
		const size_t resolution)
	{
		Event0 event(boost::bind(func, object), object);

		RemoveOneShotFunction(event, resolution);
	}

	//-----------------------------------------------------------------------//
	// Function: Stop
	//
	// Stops the timer manager completely, turning off all timers.
	//
	void Stop();

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Macro: DECLARE_EVENT_TABLE
	//
	// The wxWidgets event table declaration.
	//
	DECLARE_EVENT_TABLE()

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	typedef boost::function0<void> ReceiverFunc;

	// This class allows us to find functions after registering them.
	class Event0
	{
	public:
		explicit Event0(ReceiverFunc funcObject, void *object) :
			mFuncObject(funcObject), mObject(object) {};

		wxBOOL operator==(const Event0 &event)
		{
			return (mObject == event.mObject);
		}

		void operator()() const
		{
			mFuncObject();
		}
	
	private:
		ReceiverFunc mFuncObject;
		void *mObject;
	};

	// This class handles one shot timers.
	class wxOneShotTimer : public wxTimer
	{
	public:
		wxOneShotTimer(const Event0& event)
			: wxTimer()
			, mEvent(event)
		{}

		bool Match(const Event0& event, const wxInt32 resolution)
		{
			return (
				(mEvent == event) &&
				(resolution == GetInterval())
				);
		}

		virtual void Notify()
		{
			wxLogDebug(wxT("TimerManager: Firing one-shot function with "
				wxT("resolution %d.")), GetInterval());

			mEvent();
			TIME.RemoveOneShotFunction(mEvent, GetInterval());
		}

	private:
		Event0 mEvent;
	};

	friend void wxOneShotTimer::Notify();

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~TimerManager
	//
	// The TimerManager destructor.
	//
	~TimerManager();

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: RemoveOneShotFunction
	//
	// Removes the first one shot function that matches the given event and
	// resolution.
	//
	// Parameters:
	//     event - The event to match.
	//     resolution - The resolution to match.
	//
	void RemoveOneShotFunction(const Event0& event, const size_t resolution)
	{
		// Find the first one shot timer in the list that matches the criteria
		// and remove it.
		OneShotList::iterator it, itEnd = mOneShotList.end();

		for(it = mOneShotList.begin(); it != itEnd; ++it)
		{
			if(true == (*it)->Match(event, resolution))
			{
				wxLogDebug(wxT("TimerManager: Removing one-shot function with "
					wxT("resolution %d.")), resolution);

				mOneShotList.erase(it);
				break;
			}
		}
	}

	//-----------------------------------------------------------------------//
	// Group: Event Message Handling
	//

	//-----------------------------------------------------------------------//
	// Function: OnPeriodicTimer
	//
	// Handles timer events.
	//
	// Parameters:
	//     event - The timer event.
	//
	void OnPeriodicTimer(wxTimerEvent &event);

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	typedef std::list<std::pair<bool, Event0> > PeriodicFunctionList;
	typedef std::map<size_t, PeriodicFunctionList> PeriodicMap;

	//-----------------------------------------------------------------------//
	// Object: mPeriodicMap
	//
	// Maps timer ids to function lists.
	//
	PeriodicMap mPeriodicMap;

	typedef boost::shared_ptr<wxOneShotTimer> OneShotTimerPtr;
	typedef std::list<OneShotTimerPtr> OneShotList;
	OneShotList mOneShotList;

	typedef boost::shared_ptr<wxTimer> TimerPtr;
	typedef std::map<size_t, TimerPtr> TimerMap;

	//-----------------------------------------------------------------------//
	// Object: mTimerMap
	//
	// Maps timer ids to wxTimers.
	//
	TimerMap mTimerMap;

	friend class Singleton<TimerManager>;
};

//---------------------------- PROTOTYPES       -----------------------------//

