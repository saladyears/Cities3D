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
#include "ChipObject.h"
#include "TileObject.h"
#include "RNG.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
ChipObject::ChipObject(const TileObject * const pTile)
: BaseTiledObject(pTile)
, mRotation(0.0, 0.0, 0.0)
{
	wxASSERT(pTile);
}

wxInt32 ChipObject::roll() const
{
	return mpTile->read<wxInt32>(shRoll);
}

void ChipObject::randomizeRotation()
{
	//the chip rotation can be any rotation from 0 to 360.0;
	mRotation.y = RAND.unpooled(360);
}

void ChipObject::load(wxDataInputStream &stream)
{
	BaseTiledObject::load(stream);
	
	stream >> mRotation.x;
	stream >> mRotation.y;
	stream >> mRotation.z;
}

void ChipObject::save(wxDataOutputStream &stream) const
{
	BaseTiledObject::save(stream);

	stream << mRotation.x;
	stream << mRotation.y;
	stream << mRotation.z;
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//


