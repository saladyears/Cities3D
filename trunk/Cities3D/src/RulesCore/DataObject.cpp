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
#include "DataObject.h"
#include "AnySerializer.h"
#include "GameEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
void DataObject::load(wxDataInputStream &input)
{
	wxUint32 size;
	input >> size;

	mData.reserve(size);

	for(wxUint32 i = 0; i < size; ++i)
	{
		//read the type in
		HashString type;
		input >> type;

		IAnySerializerPtr serializer = 
			ANYENGINE.FindAnySerializerByType(type);

		//sanity
		if(!serializer)
		{
			wxLogError(wxT(
				"Programmer Error: No AnySerializer for type %s."), 
				type.cwx_str());
			
			boost::throw_exception(bad_serializer());
		}

		boost::any any;
		serializer->Unserialize(any, input);

		mData.push_back(AnyPair(any, serializer));
	}
}

void DataObject::save(wxDataOutputStream &output) const
{
	//first, we write the number of items in the any array, then
	//write each item in the any array
	output << (wxUint32)mData.size();

	AnyArray::const_iterator it = mData.begin(), itEnd = mData.end();
	for(; it != itEnd; ++it)
	{
		const IAnySerializerPtr &serializer = it->second;

		//sanity
		if(!serializer)
		{
			wxLogError(wxT("Programmer Error: No AnySerializer during save."));
			
			boost::throw_exception(bad_serializer());
		}

		//use the boost::any object's serializer to write the data
		output << serializer->Type();

		serializer->Serialize(it->first, output);
	}
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//



