/*
 *  Cities3D - Copyright (C) 2001-2006 Jason Fugate (saladyears@gmail.com)
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

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: LocalSingleton
// 
// A template class that provides an implementation of the
// LocalSingleton pattern.  Any class may derive from LocalSingleton to
// become a LocalSingleton.  This pattern should not be abused.
//
// LocalSingleton differs from Singleton in only one way. Singletons are
// globally visible across all modules (plugins), while LocalSingletons are
// only locally visible within a particular module (plugin).
//
// Project:
//     <DatabaseCore>
//
// Include:
//     LocalSingleton.h
//
// Usage:
// (start example)
//     class MyLocalSingleton : public LocalSingleton<MyLocalSingleton>
//     {
//     public:
//         /* ... */
//     private:
//         MyLocalSingleton();
//
//         //since the constructor is private, LocalSingleton must be a friend
//         //so that this class can be instantiated.
//         friend class LocalSingleton<MyLocalSingleton>;
//     };
// ·
// (end)
//
template <typename T>
class LocalSingleton
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Static Functions
	//

	//-----------------------------------------------------------------------//
	// Variable: theLocalSingleton
	//
	// The static instance of the LocalSingleton class.
	//
	static T* theLocalSingleton;

	//-----------------------------------------------------------------------//
	// Function: get
	//
	// The one and only one method of accessing a LocalSingleton. The get method
	// creates a static instance of the LocalSingleton class the first time it
	// is called, and then returns this instance every subsequent time.
	//
	// Returns:
	//     The LocalSingleton object.
	//
	static T &get()
	{
		if (NULL == theLocalSingleton)
			theLocalSingleton = new T();
		return *theLocalSingleton;
	}

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: LocalSingleton
	// 
	// The LocalSingleton constructor.  It is protected so that the LocalSingleton cannot 
	// be instantiated unless it is derived from.
	//
	LocalSingleton()
	{
	}

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~LocalSingleton
	// 
	// The LocalSingleton destructor.  It is protected so that the LocalSingleton cannot 
	// be instantiated unless it is derived from.
	//
	virtual ~LocalSingleton()
	{
	}

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Copy Constructor
	//

	//-----------------------------------------------------------------------//
	// Constructor: LocalSingleton
	// 
	// The LocalSingleton copy constructor.  It is private and never instantiated so 
	// that any developer trying to make a copy of a singleton object will get 
	// a compiler error.
	//
	LocalSingleton(const LocalSingleton &);

	//-----------------------------------------------------------------------//
	// Group: Operators
	//

	//-----------------------------------------------------------------------//
	// Function: operator=
	// 
	// The LocalSingleton assignment operator.  It is private and never instantiated
	// so that any developer trying to create a LocalSingleton by assigning it will 
	// get a compiler error.
	//
	LocalSingleton &operator=(const LocalSingleton &);
};

//---------------------------- PROTOTYPES       -----------------------------//

	//-----------------------------------------------------------------------//
	// Variable: theLocalSingleton
	//
	// The static instance of the LocalSingleton class.
	//
	template <typename T> T* LocalSingleton<T>::theLocalSingleton = NULL;
