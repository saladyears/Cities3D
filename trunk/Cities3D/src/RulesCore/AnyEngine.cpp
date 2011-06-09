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
#include "AnyEngine.h"
#include "IAnySerializer.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	WX_DECLARE_HASH_STRING_HASH_MAP(IAnySerializerPtr, AnyHash);
	AnyHash sAnyTypeHash;
	AnyHash sAnyTypeIDHash;
}

//---------------------------- PUBLIC           -----------------------------//
void AnyEngine::Stop()
{
	sAnyTypeHash.clear();
	sAnyTypeIDHash.clear();
}

void AnyEngine::AddAnySerializer(const IAnySerializerPtr &serializer)
{
	wxASSERT(serializer);

	sAnyTypeHash[serializer->Type()] = serializer;
	sAnyTypeIDHash[serializer->TypeID()] = serializer;
}

void AnyEngine::RemoveAnySerializer(const IAnySerializerPtr &serializer)
{
	wxASSERT(serializer);

	sAnyTypeHash.erase(serializer->Type());
	sAnyTypeIDHash.erase(serializer->TypeID());
}

const IAnySerializerPtr &AnyEngine::FindAnySerializerByType(
	const HashString &type) const
{
	wxASSERT(sAnyTypeHash[type]);

	return sAnyTypeHash[type];
}

const IAnySerializerPtr &AnyEngine::FindAnySerializerByTypeID(
	const HashString &typeID) const
{
	return sAnyTypeIDHash[typeID];
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//



