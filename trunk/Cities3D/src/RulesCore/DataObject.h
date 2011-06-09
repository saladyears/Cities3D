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
#include <boost/any.hpp>

//---------------------------- USER INCLUDES    -----------------------------//
#include "RulesExport.h"
#include "AnySerializer.h"
#include "GameEngine.h"
#include "IAnyEngine.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class wxDataInputStream;
class wxDataOutputStream;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: DataObject
// 
// A generic container for passing any kind of data around.
// DataObject makes use of the boost::any class and templated
// constructors to allow for any type of data to be stored in it.
// The data is stored in a std::vector of boost::any objects.  When
// an accessor wants to retrieve the data out of the DataObject,
// it must use the read<> function and supply an index into the 
// std::vector.  If the requested type does not match the type of
// the object stored at that location, a boost::bad_any_cast
// exception is thrown.
//
// DataObject is used to pass information from the UI to an <IRule> 
// without the type of the information being explicit, which would
// require a new type of Execute function for every kind of data
// that could be passed.  The strategy at work here allows the data
// type to be "hidden" while being passed around, but retrieved later.
// It is somewhat slower, but allows for great flexibility.
// 
//
// Project:
//     <RulesCore>
//
// Include:
//     DataObject.h
//
class RULES_EXPORT_SPEC DataObject
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: DataObject
	//
	// The default DataObject constructor.  Creates an
	// empty DataObject.
	DataObject() {}

	//-----------------------------------------------------------------------//
	// Constructor: DataObject
	//
	// The one-parameter DataObject constructor.  Takes the
	// data and appends it to the std::vector.
	//
	// Parameters:
	//     data - The piece of data to store.
	//
	template <typename T> DataObject(const T &data) 
	{
		mData.push_back(AnyPair(data, 
			ANYENGINE.FindAnySerializerByTypeID(typeid(T).name())));
	}

	//-----------------------------------------------------------------------//
	// Constructor: DataObject
	//
	// The two-parameter DataObject constructror.  Takes the
	// data and appends it to the std::vector.
	//
	// Parameters:
	//     data1 - The first piece of data to store.
	//     data2 - The second piece of data to store.
	//
	template <typename T, typename U> DataObject(const T &data1, 
		const U &data2)
	{
		mData.push_back(AnyPair(data1, 
			ANYENGINE.FindAnySerializerByTypeID(typeid(T).name())));
		mData.push_back(AnyPair(data2, 
			ANYENGINE.FindAnySerializerByTypeID(typeid(U).name())));
	}

	//-----------------------------------------------------------------------//
	// Constructor: DataObject
	//
	// The three-parameter DataObject constructror.  Takes the
	// data and appends it to the std::vector.
	//
	// Parameters:
	//     data1 - The first piece of data to store.
	//     data2 - The second piece of data to store.
	//     data3 - The third piece of data to store.
	//
	template <typename T, typename U, typename V> DataObject(const T &data1, 
		const U &data2, const V &data3)
	{
		mData.push_back(AnyPair(data1, 
			ANYENGINE.FindAnySerializerByTypeID(typeid(T).name())));
		mData.push_back(AnyPair(data2, 
			ANYENGINE.FindAnySerializerByTypeID(typeid(U).name())));
		mData.push_back(AnyPair(data3, 
			ANYENGINE.FindAnySerializerByTypeID(typeid(V).name())));
	}

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: numItems
	//
	// Returns how many items are in the DataObject.
	//
	// Returns:
	//     The number of items in the DataObject.
	//
	size_t numItems() const {return mData.size();}

	//-----------------------------------------------------------------------//
	// Function: reset
	//
	// Resets the DataObject to be empty.
	//
	void reset()
	{
		mData.clear();
	}

	//-----------------------------------------------------------------------//
	// Function: read
	//
	// Reads data out of the data store.  Uses the boost::any_cast
	// function to attempt to cast the data at the given index
	// in the data store to the requested type.  If the any_cast
	// fails, a boost::bad_any_cast exception is thrown, which signals
	// to the developer that the kind of data they were expecting in
	// that DataObject was not what they actually got.  Otherwise,
	// returns a read-only reference to the data object.
	//
	// Returns:
	//     A const reference to the data at the given index.
	//
	// Throws:
	//     A boost::bad_any_cast when the requested data type does
	//     not match the data at the given index in the data store.
	//
	template <typename T> const T &read(const wxUint32 index = 0) const
	{
		//sanity
		wxASSERT(index < mData.size());

		//return the boost::any_cast results; if an exception is thrown, it 
		//must be handled by something up the chain
		const T *value = boost::any_cast<T>(&(mData[index].first));

		if(NULL == value)
		{
			boost::throw_exception(boost::bad_any_cast());
		}

		return *value;
	}

	//-----------------------------------------------------------------------//
	// Function: load
	//
	// Reads a DataObject from an input stream.  Reads the
	// type information for each boost::any object that has
	// been serialized, looks up the serializer for that type
	// and then uses it to read the boost::any data from the
	// stream.
	//
	// Parameters:
	//     input - The input stream.
	//
	void load(wxDataInputStream &input);

	//-----------------------------------------------------------------------//
	// Function: save
	//
	// Writes a DataObject to an output stream.  Uses the
	// <IAnySerializer> for each boost::any object to handle
	// writing the data.
	//
	// Parameters:
	//     output - The output stream.
	//
	void save(wxDataOutputStream &output) const;

	class bad_serializer : public std::bad_typeid
    {
    public:
        virtual const char *what() const throw()
        {
            return "bad_serializer: "
                   "no serializer available for given type";
        }
    };

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	typedef std::pair<boost::any, IAnySerializerPtr> AnyPair;
	typedef std::vector<AnyPair> AnyArray;

	//-----------------------------------------------------------------------//
	// Object: mData
	//
	// The data store.  Stores boost::any objects in a
	// std::vector.
	//
	AnyArray mData;
};

//---------------------------- PROTOTYPES       -----------------------------//
