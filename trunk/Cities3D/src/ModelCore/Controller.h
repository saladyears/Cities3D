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
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <wx/hashmap.h>

//---------------------------- USER INCLUDES    -----------------------------//
#include "Singleton.h"
#include "Event.h"
#include "ModelExport.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: Controller
// 
// The communication mechanism between the View and the Model. The Controller 
// class is the go-between for the Model classes and the View, however the View
// is implemented.  By using the Observer design pattern, complete decoupling 
// is acheived between the visual representation of the game data and the game 
// data itself as neither knows anything about the other.  The View adds 
// receivers to the Controller by supplying a function and class as a receiver,
// and the name of the event to listen to.  The Model triggers events by 
// supplying the data to be transmitted and the name of the event to transmit 
// to.  There are safeguards in place to ensure that type-safety is maintained
// when transmitting data from the Model to the View. All receiver functions 
// that are registered must take exactly one parameter.
//
// Derived From:
//     <Singleton>
//
// Project:
//     <ModelCore>
//
// Include:
//     Controller.h
//
class MODEL_EXPORT_SPEC Controller : public Singleton<Controller>
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: AddReceiver
	//
	// Adds a receiver to the list of receivers for the specified
	// event.  If the receiver is the first for the given event name, a 
	// new <Event> is created, and the receiver is added to it.  If
	// other receivers have already been added, the new receiver is checked
	// to ensure that the type of its one and only parameter is the exact
	// same as all previously added receivers.
	//
	// Parameters:
	//     event - The name of the event the receiver will listen to.
	//     func - The function signature of the receiver.
	//     object - The class object the function signature belongs to.
	//
	template <typename T, typename R>
	void AddReceiver(const HashString &event, void (R::*func)(T), R* object)
	{
		//see if this event already exists in the map
		EventMap::iterator it(mEventMap.find(event));
		
		//if it does not exist, create a new event
		if(mEventMap.end() == it)
		{
			Event<T> *pEvent = new Event<T>;
			mEventMap[event] = std::make_pair(typeinfo<T>(), 
				static_cast<void *>(pEvent));

			wxLogDebug(wxT("Adding receiver %s to event %s."), 
				typeinfo<R>().cwx_str(), event.cwx_str());

			//add the listener
			pEvent->AddReceiver(func, object);
		}
		else
		{
			//otherwise, ensure that the types are the same for the same
			if(it->second.first != typeinfo<T>())
			{
				wxLogError(
					wxT("Programmer error: parameter type for %s was %s, not %s."), 
					event.c_str(), typeinfo<T>().cwx_str(), 
					it->second.first.c_str());
				return;
			}
			else
			//if they are the same, add this as a listener
			{
				Event<T> *pEvent = static_cast<Event<T> *>(it->second.second);
				wxASSERT(NULL != pEvent);
				pEvent->AddReceiver(func, object);
			}
		}
	}

	//-----------------------------------------------------------------------//
	// Function: RemoveReceiver
	//
	// Removes a receiver from the list of receivers for the specified
	// event.  If the receiver is currently executing, it will be safely
	// deleted after it has finished, thus preventing corruption in the
	// call stack.  If the receiver is the last listener to the <Event>, 
	// then the <Event> is destroyed.
	//
	// Parameters:
	//     event - The name of the event the receiver is listening to.
	//     func - The function signature of the receiver.
	//     object - The class object the function signature belongs to.
	//
	template <typename T, typename R>
	void RemoveReceiver(const HashString &event, void (R::*func)(T), R* object)
	{
		EventMap::iterator it(mEventMap.find(event));

		//this event already should be in the map, but just in case
		if(mEventMap.end() == it)
		{
			return;
		}

		if(it->second.first != typeinfo<T>())
		{
			return;
		}

		//remove the listener
		Event<T> *pEvent = static_cast<Event<T> *>(it->second.second);
		wxASSERT(NULL != pEvent);
		pEvent->RemoveReceiver(func, object);

		//if none left, kill, unless it's currently transmitting, in which
		//case let the transmit function deal with it
		if( (0 == pEvent->NumReceivers()) &&
			(false == pEvent->IsTransmitting()))
		{
			wxLogDebug(wxT("Removing receiver %s from event %s."), 
				typeinfo<R>().cwx_str(), event.cwx_str());

			delete pEvent;
			mEventMap.erase(it);
		}
	}

	//-----------------------------------------------------------------------//
	// Function: Transmit
	//
	// Calls all of the receivers for the given event with
	// the given object as the parameter.  If the type of the object
	// does not match the type of a receiver's stored parameter
	// type, an error is thrown to the screen.
	//
	// Parameters:
	//     event - The name of the event to trigger.
	//     object - The object to pass to the receivers.
	//
	template <typename T>
	void Transmit(const HashString &event, const T &object)
	{
		EventMap::iterator it(mEventMap.find(event));

		//if there's nothing listening to the event, there's nothing to trigger
		if(mEventMap.end() == it)
		{
			return;
		}

		//make sure the types are correct
		if(it->second.first != typeinfo<T>())
		{
			wxLogError(wxString::Format(
					wxT("Programmer error: transmit type for %s was %s, not %s."), 
					event.cwx_str(), typeinfo<T>().cwx_str(), 
					it->second.first.cwx_str()));

			return;
		}

		//transmit the event
		Event<T> *pEvent = static_cast<Event<T> *>(it->second.second);
		wxASSERT(NULL != pEvent);
		pEvent->Transmit(object);

		//if transmitting this event has removed all listeners, kill the 
		//event
		if(0 == pEvent->NumReceivers())
		{
			delete pEvent;

			// make sure the object is still in the map before erase()
			if(mEventMap.end() != (it = mEventMap.find(event)))
			{
				mEventMap.erase(it);
			}
		}
	}

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~Controller
	//
	// The Controller destructor.  Cleans up any Event objects
	// that have not been already destroyed.
	//
	~Controller();

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: typeinfo
	//
	// Returns the type information associated with the
	// template parameter.
	//
	// Returns:
	//     A string containing the type information of the template
	//     parameter.
	//
	template <typename T> HashString typeinfo()
	{
		HashString str;

		try
		{
			const std::type_info &info(typeid(T));
			str = info.name();
		}
		catch(std::bad_typeid &object)
		{
			(void) object;
		}

		return str;
	}

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//the event map
	typedef std::pair<HashString, void *> EventPair;
	WX_DECLARE_HASH_STRING_HASH_MAP(EventPair, EventMap);

	//-----------------------------------------------------------------------//
	// Object: mEventMap
	//
	// The map of event names to the <Events> themselves.  The <Event>
	// objects are stored as void * because they are template objects
	// and every instance of a template class is a brand new type, making
	// it is impossible to store them as Event *, for example.  To
	// get around this, the <Events> are changed to void * using static_cast<>
	// and stored in the map, then converted back into the proper Event<> type,
	// later when needed, using the stored type information for each <Event>.
	//
	EventMap mEventMap;

	friend class Singleton<Controller>;
};
//---------------------------- PROTOTYPES       -----------------------------//

