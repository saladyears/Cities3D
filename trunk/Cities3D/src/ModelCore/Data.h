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
#include <wx/datstrm.h>
#include <wx/hashmap.h>
#include <wx/log.h>

//---------------------------- USER INCLUDES    -----------------------------//
#include "ModelExport.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: Data
// 
// Stores data of many different kinds in string-based hash
// maps.  The Data class provides public read-access to data, but gives no
// public method for changing data, which must be provided by
// derived classes.  By using hash maps with strings as keys, any class with
// write access can add new data fields dynamically and instantly, with little
// overhead.  Data provides data hashes for the following types: integer,
// integer arrays, two-dimensional integer arrays, integer hashes,
// floats, strings, string arrays, and date/times.
//
// Project:
//     <ModelCore>
//
// Include:
//     Data.h
//
class MODEL_EXPORT_SPEC Data
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: read
	//
	// Reads a keyed value from a hash map.  If the key does not exist
	// in the hash map, an error is raised in a popup window specifying
	// the key and the type of data that was attempting to be read.
	//
	// Parameters:
	//     key - The key to look up in the data hash.
	//
	// Returns:
	//     A const reference to the data corresponding to the key.
	//     It is read-only.
	//
	template <typename T> const T& read(const HashString &key) const
	{
		//since using [] creates data if it doesn't exist, wxHashMap doesn't 
		//provide a const version of the [] operator; we have to cast away the 
		//const-ness of the this pointer first and then return the non-const 
		//version of the request, which gets converted to const on the way out
		//of this function
		Data *pThis = const_cast<Data *>(this);
		return pThis->data<T>(key, true);
	}

	//this definition has to be public so readers of the data can
	//get access to hashes of IntHash
	WX_DECLARE_HASH_STRING_HASH_MAP(wxInt32, IntHash);

	//-----------------------------------------------------------------------//
	// Function: read
	//
	// Provides specialized read-only access for integer hash
	// data.  Because a wxWidgets hash map does not have a const []
	// operator, it is unsafe to just look up specific integer hash
	// data without a key, so this function is necessary.
	//
	// Parameters:
	//     key - The key that finds an integer hash.
	//     hashKey - The key to look up in the integer hash.
	//
	// Returns:
	//     The data corresponding to the key.
	//
	wxInt32 read(const HashString &key, const HashString &hashKey)
	{
		IntHash::const_iterator it(read<IntHash>(key).find(hashKey));
		if(read<IntHash>(key).end() == it)
		{
			wxLogError(wxString::Format(wxT("Programmer Error: Unable to ")
				wxT("find key %s in IntHash %s data."), hashKey.cwx_str(), 
				key.cwx_str()));
			return 0;
		}

		return it->second;
	}

	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: load
	//
	// Reads data from an input stream.
	//
	// Parameters:
	//     stream - The input stream.
	//
	virtual void load(wxDataInputStream &stream);

	//-----------------------------------------------------------------------//
	// Function: save
	//
	// Writes data to an output stream.
	//
	// Parameters:
	//     stream - The output stream.
	//
	virtual void save(wxDataOutputStream &stream) const;

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~Data
	//
	// The Data destructor.  It is pure virtual because
	// this class must be derived from to be used.
	//
	virtual ~Data()=0;

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: data
	//
	// Gives direct access to data in a hash map.  If the type of the data
	// does not match one of the hash maps, the code will not compile.
	//
	// Parameters:
	//     key - The key to access in the hash.
	//     safety - A flag to ensure the key exists.  If true, the key is first
	//              searched for before writing to it, and if the key does not
	//              exist, an error message is shown on the screen and the 
	//              default constructed value for that data type.
	//
	// Returns:
	//     A reference to the data corresponding to the key.  It is writable.
	//
	template <typename T> T& data(const HashString &key, bool safety = false)
	{
		//by doing a void return here, we make it so that any developer who
		//tries to access a data type that this class does not provide will get
		//a compile-time error saying that this function does not return a 
		//value; this works because template declarations are never 
		//instantiated until they are actually required
		return;
	}

	//-----------------------------------------------------------------------//
	// Function: erase
	//
	// Erases the given key from the hash map.
	//
	// Parameters:
	//     key - The key to erase in the hash.
	//
	template <typename T> void erase(const HashString &key)
	{
		//see comments for data
		return;
	}

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//by using hash maps instead of some other container type, it becomes 
	//trivial for rule plugins to extend the data of the game, by simply using 
	//another string for a data type
	WX_DECLARE_HASH_STRING_HASH_MAP(IntArray, IntArrayHash);
	WX_DECLARE_HASH_STRING_HASH_MAP(IntArray2, IntArray2Hash);
	WX_DECLARE_HASH_STRING_HASH_MAP(IntHash, IntHashHash);
	WX_DECLARE_HASH_STRING_HASH_MAP(float, FloatHash);
	WX_DECLARE_HASH_STRING_HASH_MAP(HashString, StringHash);
	WX_DECLARE_HASH_STRING_HASH_MAP(HashStringArray, HashStringArrayHash);
	WX_DECLARE_HASH_STRING_HASH_MAP(wxDateTime, DateTimeHash);
	WX_DECLARE_HASH_STRING_HASH_MAP(wxTimeSpan, TimeSpanHash);

	//-----------------------------------------------------------------------//
	// Hash: mInts
	//
	// The hash of integers.
	//
	IntHash mInts;

	//-----------------------------------------------------------------------//
	// Hash: mIntArrays
	//
	// The hash of arrays of integers.
	//
	IntArrayHash mIntArrays;

	//-----------------------------------------------------------------------//
	// Hash: mInt2Arrays
	//
	// The hash of two-dimensional arrays of integers.
	//
	IntArray2Hash mInt2Arrays;

	//-----------------------------------------------------------------------//
	// Hash: mIntHashes
	//
	// The hash of hashes of integers.
	//
	IntHashHash mIntHashes;

	//-----------------------------------------------------------------------//
	// Hash: mFloats
	//
	// The hash of floats.
	//
	FloatHash mFloats;

	//-----------------------------------------------------------------------//
	// Hash: mStrings
	//
	// The hash of HashStrings.
	//
	StringHash mStrings;

	//-----------------------------------------------------------------------//
	// Hash: mHashStringArrays
	//
	// The hash of arrays of HashStrings.
	//
	HashStringArrayHash mHashStringArrays;

	//-----------------------------------------------------------------------//
	// Hash: mTimes
	//
	// The hash of date/times.
	//
	DateTimeHash mTimes;

	//-----------------------------------------------------------------------//
	// Hash: mTimeSpans
	//
	// The hash of timespans.
	//
	TimeSpanHash mTimeSpans;

	friend class Map;
};

//---------------------------- PROTOTYPES       -----------------------------//

//explicit specializations to return specific data

//ints
template <> inline wxInt32 &Data::data(const HashString &key, bool safety)
{
	//if the safety is on, look for the key first and if it is not found,
	//report an error
	if( (true == safety) &&
		(mInts.end() == mInts.find(key)))
	{
		wxLogError(wxString::Format(wxT("Programmer Error: Unable to find %s ")
			wxT("in integer data."), key.cwx_str()));
		mInts[key] = 0;
	}

	return mInts[key];
}

//int arrays
template <> inline IntArray &Data::data(const HashString &key, bool safety)
{
	//if the safety is on, look for the key first and if it is not found,
	//report an error
	if( (true == safety) &&
		(mIntArrays.end() == mIntArrays.find(key)))
	{
		wxLogError(wxString::Format(wxT("Programmer Error: Unable to find %s ")
			wxT("in integer array data."), key.cwx_str()));
		mIntArrays[key].resize(0, 0);
	}

	return mIntArrays[key];
}

//int2 arrays
template <> inline IntArray2 &Data::data(const HashString &key, bool safety)
{
	//if the safety is on, look for the key first and if it is not found,
	//report an error
	if( (true == safety) &&
		(mInt2Arrays.end() == mInt2Arrays.find(key)))
	{
		wxLogError(wxString::Format(wxT("Programmer Error: Unable to find %s ")
			wxT("in integer array2 data."), key.cwx_str()));
		mInt2Arrays[key].resize(0);
	}

	return mInt2Arrays[key];
}

//int hashes
template <> inline Data::IntHash &Data::data(const HashString &key, bool safety)
{
	//if the safety is on, look for the key first and if it is not found,
	//report an error
	if( (true == safety) &&
		(mIntHashes.end() == mIntHashes.find(key)))
	{
		wxLogError(wxString::Format(wxT("Programmer Error: Unable to find %s ")
			wxT("in integer hash data."), key.cwx_str()));
	}

	return mIntHashes[key];
}

//floats
template <> inline float &Data::data(const HashString &key, bool safety)
{
	//if the safety is on, look for the key first and if it is not found,
	//report an error
	if( (true == safety) &&
		(mFloats.end() == mFloats.find(key)))
	{
		wxLogError(wxString::Format(wxT("Programmer Error: Unable to find %s ")
			wxT("in float data."), key.cwx_str()));
		mFloats[key] = 0.0f;
	}

	return mFloats[key];
}

//strings
template <> inline HashString &Data::data(const HashString &key, bool safety)
{
	//if the safety is on, look for the key first and if it is not found,
	//report an error
	if( (true == safety) &&
		(mStrings.end() == mStrings.find(key)))
	{
		wxLogError(wxString::Format(wxT("Programmer Error: Unable to find %s ")
			wxT("in string data."), key.cwx_str()));
		mStrings[key] = shEmpty;
	}

	return mStrings[key];
}

//string arrrays
template <> inline HashStringArray &Data::data(const HashString &key, 
	bool safety)
{
	//if the safety is on, look for the key first and if it is not found,
	//report an error
	if( (true == safety) &&
		(mHashStringArrays.end() == mHashStringArrays.find(key)))
	{
		wxLogError(wxString::Format(wxT("Programmer Error: Unable to find %s ")
			wxT("in string array data."), key.cwx_str()));
	}

	return mHashStringArrays[key];
}

//date/times
template <> inline wxDateTime &Data::data(const HashString &key, bool safety)
{
	//if the safety is on, look for the key first and if it is not found,
	//report an error
	if( (true == safety) &&
		(mTimes.end() == mTimes.find(key)))
	{
		wxLogError(wxString::Format(wxT("Programmer Error: Unable to find %s ")
			wxT("in date/time data."), key.cwx_str()));
		mTimes[key] = wxDateTime::UNow();
	}

	return mTimes[key];
}

//timespans
template <> inline wxTimeSpan &Data::data(const HashString &key, bool safety)
{
	//if the safety is on, look for the key first and if it is not found,
	//report an error
	if( (true == safety) &&
		(mTimeSpans.end() == mTimeSpans.find(key)))
	{
		wxLogError(wxString::Format(wxT("Programmer Error: Unable to find %s ")
			wxT("in timepsan data."), key.cwx_str()));
		mTimeSpans[key] = wxTimeSpan();
	}

	return mTimeSpans[key];
}

//explicit specializations to erase keys

//ints
template <> inline void Data::erase<wxInt32>(const HashString &key)
{
	IntHash::iterator it = mInts.find(key);
	if(mInts.end() != it)
	{
		mInts.erase(it);
	}
}

//int arrays
template <> inline void Data::erase<IntArray>(const HashString &key)
{
	IntArrayHash::iterator it = mIntArrays.find(key);
	if(mIntArrays.end() != it)
	{
		mIntArrays.erase(it);
	}
}

//int2 arrays
template <> inline void Data::erase<IntArray2>(const HashString &key)
{
	IntArray2Hash::iterator it = mInt2Arrays.find(key);
	if(mInt2Arrays.end() != it)
	{
		mInt2Arrays.erase(it);
	}
}

//int hashes
template <> inline void Data::erase<Data::IntHash>(const HashString &key)
{
	IntHashHash::iterator it = mIntHashes.find(key);
	if(mIntHashes.end() != it)
	{
		mIntHashes.erase(it);
	}
}


//floats
template <> inline void Data::erase<float>(const HashString &key)
{
	FloatHash::iterator it = mFloats.find(key);
	if(mFloats.end() != it)
	{
		mFloats.erase(it);
	}
}

//strings
template <> inline void Data::erase<HashString>(const HashString &key)
{
	StringHash::iterator it = mStrings.find(key);
	if(mStrings.end() != it)
	{
		mStrings.erase(it);
	}
}

//string arrrays
template <> inline void Data::erase<HashStringArray>(const HashString &key)
{
	HashStringArrayHash::iterator it = mHashStringArrays.find(key);
	if(mHashStringArrays.end() != it)
	{
		mHashStringArrays.erase(it);
	}
}

//date/times
template <> inline void Data::erase<wxDateTime>(const HashString &key)
{
	DateTimeHash::iterator it = mTimes.find(key);
	if(mTimes.end() != it)
	{
		mTimes.erase(it);
	}
}

//timespans
template <> inline void Data::erase<wxTimeSpan>(const HashString &key)
{
	TimeSpanHash::iterator it = mTimeSpans.find(key);
	if(mTimeSpans.end() != it)
	{
		mTimeSpans.erase(it);
	}
}
