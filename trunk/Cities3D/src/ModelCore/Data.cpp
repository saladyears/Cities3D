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
#include "Data.h"
#include "define/defineHelper.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
Data::~Data()
{
}

void Data::load(wxDataInputStream &stream)
{
	wxUint32 i, size;
	HashString key;

	//ints
	mInts.clear();
	stream >> size;
	for(i = 0; i < size; ++i)
	{
		stream >> key;
		stream >> mInts[key];
	}

	//int arrays
	mIntArrays.clear();
	stream >> size;
	for(i = 0; i < size; ++i)
	{
		stream >> key;
		loadArray(stream, &(mIntArrays[key]));
	}

	//int2 arrays
	mInt2Arrays.clear();
	stream >> size;
	for(i = 0; i < size; ++i)
	{
		stream >> key;
		loadArrays(stream, &(mInt2Arrays[key]));
	}

	//int hashes
	mIntHashes.clear();
	wxUint32 hashSize;
	HashString hashKey;

	stream >> size;
	for(i = 0; i < size; ++i)
	{
		stream >> key;
		stream >> hashSize;
		for(wxUint32 j = 0; j < hashSize; ++j)
		{
			stream >> hashKey;
			stream >> mIntHashes[key][hashKey];
		}
	}

	//floats
	mFloats.clear();
	stream >> size;
	for(i = 0; i < size; ++i)
	{
		stream >> key;
		stream >> mFloats[key];
	}

	//strings
	mStrings.clear();
	stream >> size;
	for(i = 0; i < size; ++i)
	{
		stream >> key;
		stream >> mStrings[key];
	}

	//string arrays
	mHashStringArrays.clear();
	stream >> size;
	for(i = 0; i < size; ++i)
	{
		stream >> key;
		loadArray(stream, &(mHashStringArrays[key]));
	}

	//times
	mTimes.clear();
	stream >> size;
	for(i = 0; i < size; ++i)
	{
		stream >> key;
		loadDateTime(stream, mTimes[key]);
	}

	//timespans
	mTimeSpans.clear();
	stream >> size;
	for(i = 0; i < size; ++i)
	{
		stream >> key;
		loadTimeSpan(stream, mTimeSpans[key]);
	}
}

void Data::save(wxDataOutputStream &stream) const
{
	//ints
	stream << (wxUint32) mInts.size();
	for(IntHash::const_iterator it = mInts.begin(); it != mInts.end(); ++it)
	{
		stream << it->first;
		stream << it->second;
	}

	//int arrays
	stream << (wxUint32) mIntArrays.size();
	for(IntArrayHash::const_iterator it = mIntArrays.begin(); 
		it != mIntArrays.end(); ++it)
	{
		stream << it->first;
		saveArray(stream, &(it->second));
	}

	//int2 arrays
	stream << (wxUint32) mInt2Arrays.size();
	for(IntArray2Hash::const_iterator it = mInt2Arrays.begin(); 
		it != mInt2Arrays.end(); ++it)
	{
		stream << it->first;
		saveArrays(stream, &(it->second));
	}

	//int hashes
	stream << (wxUint32) mIntHashes.size();
	for(IntHashHash::const_iterator it = mIntHashes.begin(); 
		it != mIntHashes.end(); ++it)
	{
		stream << it->first;
		stream << (wxUint32) it->second.size();
		for(IntHash::const_iterator it2 = it->second.begin(); 
			it2 != it->second.end(); ++it2)
		{
			stream << it2->first;
			stream << it2->second;
		}
	}

	//floats
	stream << (wxUint32) mFloats.size();
	for(FloatHash::const_iterator it = mFloats.begin(); 
		it != mFloats.end(); ++it)
	{
		stream << it->first;
		stream << it->second;
	}

	//strings
	stream << (wxUint32) mStrings.size();
	for(StringHash::const_iterator it = mStrings.begin(); 
		it != mStrings.end(); ++it)
	{
		stream << it->first;
		stream << it->second;
	}

	//string arrays
	stream << (wxUint32) mHashStringArrays.size();
	for(HashStringArrayHash::const_iterator it = mHashStringArrays.begin(); 
		it != mHashStringArrays.end(); ++it)
	{
		stream << it->first;
		saveArray(stream, &(it->second));
	}

	//date/times
	stream << (wxUint32) mTimes.size();
	for(DateTimeHash::const_iterator it = mTimes.begin(); 
		it != mTimes.end(); ++it)
	{
		stream << it->first;
		saveDateTime(stream, it->second);
	}

	//timespans
	stream << (wxUint32) mTimeSpans.size();
	for(TimeSpanHash::const_iterator it = mTimeSpans.begin();
		it != mTimeSpans.end(); ++it)
	{
		stream << it->first;
		saveTimeSpan(stream, it->second);
	}
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//


