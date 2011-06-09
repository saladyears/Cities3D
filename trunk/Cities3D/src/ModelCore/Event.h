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
#include <boost/type_traits.hpp>
#include <list>

//---------------------------- USER INCLUDES    -----------------------------//

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

namespace
{
	// This handy typedef removes any const, volatile, or references from the
	// T type, which allows us to accept functions that take const references
	// to T as their parameter type.  If we didn't do this, the compiler would
	// be trying to call Transmit(const const T &&object), and since you
	// can't have a reference to a reference, it would not compile.  With this
	// typedef, the Transmit call becomes Transmit(const T &object), which is
	// exactly what we want.
	template<typename T>
	struct no_qualifiers
	{
		typedef 
		typename boost::remove_cv<typename boost::remove_reference<T>::type>::type 
		type;
	};

	// This typedef chooses either a const T or a const T reference based on
	// the boolean passed as second template argument
	template<typename T, bool B>
	struct is_fund_switch
	{
		typedef
		const typename no_qualifiers<T>::type&
		type;
	};
	template<typename T>
	struct is_fund_switch<T, true>
	{
		typedef
		const typename no_qualifiers<T>::type
		type;
	};

	// This typedef provides the final version of the parameter type that we
	// want to use in our function objects.  It passes fundamental types (ints,
	// floats, void) as const T, which is the right way to pass simple types,
	// and passes all other types as const T &, the right way to pass complex
	// types.
	template<typename T>
	struct parameter
	{
		typedef 
		typename is_fund_switch<T, boost::is_fundamental<T>::value>::type
		type;
	};
}

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: Event
// 
// A generic implementation of the Observer design pattern.
// The Event class allows a generic event handler to be created.  The template 
// parameter of the class is the type of the object in the one and only 
// parameter of the receivers.  Each time a function is added to an Event, a 
// function object is created for that function and class object using the 
// boost::bind function.  The function object is then stored in the list of 
// receivers for the Event.  When the Event is notified to transmit data, all 
// receivers for the Event are called with the given object as their one and
// only parameter.  Any particular class object may store only one function in
// the Event's receiver list--any other functions it tries to add will not be 
// stored.
//
// Project:
//     <ModelCore>
//
// Include:
//     Event.h
//

template <typename T>
class Event
{
	typedef typename parameter<T>::type parameter_type;

	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructor
	//

	//-----------------------------------------------------------------------//
	// Constructor: Event
	//
	// The Event constructor.  Initializes the in transmit flag to
	// false.
	//
	Event() : mInTransmit(false) {};
	
	//the mini-class that hides all of the implementation details of receiver 
	//function objects
	class Receiver
	{
	public:
		typedef boost::function1<void, parameter_type> ReceiverFunc;

		explicit Receiver(ReceiverFunc funcObject, void *object) :
			mFuncObject(funcObject), mObject(object) {};

		wxBOOL operator==(const Receiver &recv)
		{
			return (mObject == recv.mObject);
		}

		void CallReceiver(parameter_type object1) const
		{
			mFuncObject(object1);
		}
	
	private:
		Receiver();

		ReceiverFunc mFuncObject;
		void *mObject;
	};

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: AddReceiver
	//
	// Adds a new function object to the receiver list for this Event.
	//
	// Parameters:
	//     func - The function signature of the receiver.
	//     object - The class object the function signature belongs to.
	//
	template <typename R>
	void AddReceiver(void (R::*func)(T), R* object)
	{
		Receiver receiver = MakeReceiver(func, object);

		typename ReceiverList::iterator it;
		it = std::find(mReceiverList.begin(), mReceiverList.end(), receiver);
		if(mReceiverList.end() == it)
		{
			mReceiverList.push_back(receiver);
		}
	}

	//-----------------------------------------------------------------------//
	// Function: RemoveReceiver
	//
	// Removes a function object from the receiver list for this Event.  If
	// the receiver is currently being trasmitted to, it is set aside
	// in a special list marked for deletion later.
	//
	// Parameters:
	//     func - The function signature of the receiver.
	//     object - The class object the function signature belongs to.
	//
	template <typename R>
	void RemoveReceiver(void (R::*func)(T), R* object)
	{
		Receiver receiver = MakeReceiver(func, object);

		//if we're current transmitting, don't remove a receiver out
		//from under the transmit; instead, add it to a list of 
		//receivers to delete after the transmit is done
		if(true == IsTransmitting())
		{
			mRemoveList.push_back(receiver);
		}
		//otherwise, just remove it
		else
		{
			DeleteReceiver(receiver);
		}
	}

	//-----------------------------------------------------------------------//
	// Function: Transmit
	//
	// Calls all of the function objects for this Event.
	// The given object is passed to the receiver function objects
	// as the one and only parameter.
	//
	// Parameters:
	//     object - The object to pass to the receivers.
	//
	void Transmit(parameter_type object)
	{
		mInTransmit = true;

		typename ReceiverList::const_iterator it, itEnd;
		itEnd = mReceiverList.end();

		for(it = mReceiverList.begin(); it != itEnd; it++)
		{
			(*it).CallReceiver(object);
		}

		mInTransmit = false;

		//if some receivers removed themselves during the transmit,
		//commit the remove now
		if(0 < mRemoveList.size())
		{
			std::for_each(mRemoveList.begin(), mRemoveList.end(), 
				boost::bind(&Event<T>::DeleteReceiver, this, _1));
			mRemoveList.clear();
		}
	}

	//-----------------------------------------------------------------------//
	// Function: NumReceivers
	//
	// Returns how many receivers this Event has.
	//
	// Returns:
	//     The number of receivers in this Event.
	//
	const wxUint32 NumReceivers() {return mReceiverList.size();}

	//-----------------------------------------------------------------------//
	// Function: IsTransmitting
	//
	// Indicates whether the Event is transmitting to receivers
	// right now.
	//
	// Returns:
	//     A boolean giving whether the Event is transmitting.  The value
	//     is true if transmitting, false if not.
	//
	const bool IsTransmitting() {return mInTransmit;}

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: MakeReceiver
	//
	// Turns a function signature and class object into
	// a function object.
	//
	// Parameters:
	//     func - The function signature of the receiver.
	//     object - The class object the function signature belongs to.
	//
	// Returns:
	//     A function object wrapping the function signature and class object.
	//
	template <typename R>
	Receiver MakeReceiver(void (R::*func)(T), R* object) const
	{
		typename Receiver::ReceiverFunc funcObject;
		funcObject = boost::bind(func, object, _1);
		Receiver receiver(funcObject, static_cast<void *>(object));

		return receiver;
	}

	//-----------------------------------------------------------------------//
	// Function: DeleteReceiver
	//
	// Deletes a function object from the receiver list.
	//
	// Parameters:
	//     receiver - The function object to delete.
	//
	void DeleteReceiver(const Receiver &receiver)
	{
		typename ReceiverList::iterator it;
		it = std::find(mReceiverList.begin(), mReceiverList.end(), receiver);
		if(mReceiverList.end() != it)
		{
			mReceiverList.erase(it);
		}
	}

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Bool: mInTransmit
	//
	// Whether the Event is transmitting to receivers right now.
	//
	bool mInTransmit;

	typedef std::list<Receiver> ReceiverList;

	//-----------------------------------------------------------------------//
	// Object: mReceiverList
	//
	// The list of receivers for this Event.
	//
	ReceiverList mReceiverList;

	//-----------------------------------------------------------------------//
	// Object: mRemoveList
	//
	// The list of receivers to be deleted when this Event stops
	// transmitting.
	//
	ReceiverList mRemoveList;
};

//---------------------------- PROTOTYPES       -----------------------------//
