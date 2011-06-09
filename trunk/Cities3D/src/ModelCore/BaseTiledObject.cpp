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
#include "BaseTiledObject.h"
#include "TileObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
//standard flags
bool BaseTiledObject::isset(const ObjectFlag flag) const
{
	return (mDisplayFlags & (1 << flag)) || false;
}

void BaseTiledObject::set(const ObjectFlag flag)
{
	mDisplayFlags |= (1 << flag);
}

//custom flags
bool BaseTiledObject::isset(const wxInt32 flag) const
{
	wxASSERT(16 > (wxUint32) flag);

	return (mDisplayFlags & (1 << (16 + flag))) || false;
}

void BaseTiledObject::set(const wxInt32 flag)
{
	wxASSERT(16 > (wxUint32) flag);

	mDisplayFlags |= (1 << (16 + flag));
}

wxInt32 BaseTiledObject::id() const
{
	return mpTile->id();
}

const Vector &BaseTiledObject::coords() const
{
	return mpTile->coords();
}

void BaseTiledObject::load(wxDataInputStream &stream)
{
	stream >> mDisplayFlags;
}

void BaseTiledObject::save(wxDataOutputStream &stream) const
{
	stream << mDisplayFlags;
}

//---------------------------- PROTECTED        -----------------------------//
BaseTiledObject::BaseTiledObject(const TileObject * const pTile)
: mpTile(pTile)
, mDisplayFlags(0)
{
	wxASSERT(pTile);
}

BaseTiledObject::~BaseTiledObject()
{
}

//---------------------------- PRIVATE          -----------------------------//


