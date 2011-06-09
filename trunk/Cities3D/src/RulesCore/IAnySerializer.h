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

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class wxDataOutputStream;
class wxDataInputStream;

class IAnySerializer;
typedef boost::shared_ptr<IAnySerializer> IAnySerializerPtr;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: IAnySerializer
// 
// Needs documentation.
//
// Project:
//     <RulesCore>
//
// Include:
//     IAnySerializer.h
//
class RULES_EXPORT_SPEC IAnySerializer
{
public:
	virtual ~IAnySerializer()=0;

	virtual const HashString& Type() const=0;

	virtual const HashString& TypeID() const=0;

	virtual void Serialize(const boost::any &data, 
		wxDataOutputStream &output)=0;

	virtual void Unserialize(boost::any &data, wxDataInputStream &input)=0;
};

//---------------------------- PROTOTYPES       -----------------------------//
