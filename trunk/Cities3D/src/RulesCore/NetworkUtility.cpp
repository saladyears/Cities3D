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
#include "NetworkUtility.h"
#include "DataObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
void NetworkUtility::WriteRuleToMemory(const HashString &rule, 
	const DataObject &object, boost::shared_array<wxUint8> &data, size_t &size)
{
	// Determine the size of the message.
	wxCountingOutputStream count;
	wxDataOutputStream counter(count);

	counter << rule;

	try
	{
		object.save(counter);
	}
	catch(const DataObject::bad_serializer &)
	{
		HashString str = HashString::Format("Network could not serialize %s.", 
			rule.c_str());
		throw std::runtime_error(str.c_str());
	}

	// Add 4 bytes for the message type.
	size = count.GetSize();

	// Write everything to a memory stream so it can be sent as a glob of 
	// memory.
	data.reset(new wxUint8[size]);
	wxMemoryOutputStream mem(data.get(), size);
	wxDataOutputStream output(mem);
	
	// Write to memory.
	output << rule;
	object.save(output);
}

void NetworkUtility::ReadRuleFromMemory(HashString &rule, DataObject &object, 
	const boost::shared_array<wxUint8> &data, const size_t size)
{
	wxMemoryInputStream memStream(data.get(), size);
	wxDataInputStream input(memStream);

	input >> rule;

	if(true == rule.empty())
	{
		throw std::runtime_error("Network received empty rule.");
	}

	try
	{
		object.load(input);
	}
	catch(const DataObject::bad_serializer &)
	{
		//do not try to execute the rule because it may crash...
		//the DataObject could not be loaded and may be empty or only
		//partially filled, which may cause the rule execution to
		//explode
		HashString str = HashString::Format("Network could not unserialize %s.", 
			rule.c_str());
		throw std::runtime_error(str.c_str());
	}
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
