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
#include "TileObject.h"
#include "ChipObject.h"
#include "PortObject.h"
#include "RNG.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
TileObject::TileObject(wxInt32 index)
{
	//set initial data
	data<wxInt32>(shIndex) = index;
	data<wxInt32>(shRoll) = -1;
	data<wxInt32>(shPort1) = -1;
	data<wxInt32>(shPort2) = -1;
	data<wxInt32>(shPortSide) = -1;
	data<wxInt32>(shBorder) = FALSE;

	data<IntArray>(shSides).resize(6, -1);

	data<HashString>(shTileType) = shOcean;
}

void TileObject::randomizeRotation()
{
	//multiply by 60 to get the tile rotation
	if(-1 == data<wxInt32>(shPortSide))
	{
		mRotation.y = static_cast<float>(RAND.pooled(6) * 60);
	}
}

void TileObject::createChipObject(bool show)
{
	//always reset
	mChipObject.reset();

	//create a new chip object if we need to show one
	if(true == show)
	{
		mChipObject = ChipObjectPtr(new ChipObject(this));
	}
}

void TileObject::createPortObject(bool show)
{
	//always reset
	mPortObject.reset();

	//create a new port object if we need to show one
	if(true == show)
	{
		mPortObject = PortObjectPtr(new PortObject(this));
	}
}

void TileObject::reset()
{
	GameObject::reset();

	if(mChipObject)
	{
		mChipObject->reset();
	}

	if(mPortObject)
	{
		mPortObject->reset();
	}
}

void TileObject::set(const ObjectFlag flag)
{
	GameObject::set(flag);

	if(mChipObject)
	{
		mChipObject->set(flag);
	}

	if(mPortObject)
	{
		mPortObject->set(flag);
	}
}

void TileObject::load(wxDataInputStream &stream)
{
	Data::load(stream);
	GameObject::load(stream);

	//load the chip object, if it exists
	wxBOOL object;
	stream >> object;

	if(TRUE == object)
	{
		mChipObject = ChipObjectPtr(new ChipObject(this));
		mChipObject->load(stream);
	}

	stream >> object;

	if(TRUE == object)
	{
		mPortObject = PortObjectPtr(new PortObject(this));
		mPortObject->load(stream);
	}
}

void TileObject::save(wxDataOutputStream &stream) const
{
	Data::save(stream);
	GameObject::save(stream);

	//save the chip object, if it exists
	if(mChipObject)
	{
		stream << TRUE;
		mChipObject->save(stream);
	}
	else
	{
		stream << FALSE;
	}

	//save the port object, if it exists
	if(mPortObject)
	{
		stream << TRUE;
		mPortObject->save(stream);
	}
	else
	{
		stream << FALSE;
	}
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//



