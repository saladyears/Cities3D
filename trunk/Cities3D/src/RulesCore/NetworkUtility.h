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
#include <boost/shared_array.hpp>

//---------------------------- USER INCLUDES    -----------------------------//

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class DataObject;
class wxSocketBase;

//---------------------------- CLASSES          -----------------------------//
namespace NetworkUtility
{
	void WriteRuleToMemory(const HashString &rule, const DataObject &object,
		boost::shared_array<wxUint8> &data, size_t &size);

	void ReadRuleFromMemory(HashString &rule, DataObject &object, 
		const boost::shared_array<wxUint8> &data, const size_t size);
}

//---------------------------- PROTOTYPES       -----------------------------//

