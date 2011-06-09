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

//---------------------------- USER INCLUDES    -----------------------------//
#include "CornerObject.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
typedef boost::shared_ptr<CornerObject> CornerObjectPtr;

//---------------------------- CLASSES          -----------------------------//
class ExtraCornerObject : public CornerObject
{
public:
	ExtraCornerObject(wxInt32 owner, wxInt32 turn, const ColorType color, 
		const GamePtr &game, const wxInt32 tile, const wxInt32 corner,
		boost::weak_ptr<CornerObject> link)
	: CornerObject(owner, turn, color, game, tile, corner, link)
	, mOffset(0.0f)
	{
	}

	ExtraCornerObject() 
		: mOffset(0.0f)
	{}

	~ExtraCornerObject()
	{
		CornerObjectPtr object = above().lock();

		while(object)
		{
			Vector coords = object->coords();
			coords.y -= mOffset;
			object->coords(coords);

			object = object->above().lock();
		}
	}

	void offset(float offset)
	{
		mOffset = offset;

		CornerObjectPtr object = above().lock();

		while(object)
		{
			Vector coords = object->coords();
			coords.y += mOffset;
			object->coords(coords);

			object = object->above().lock();
		}
	}

	float offset() const
	{
		return mOffset;
	}

	virtual void load(wxDataInputStream& stream)
	{
		CornerObject::load(stream);

		stream >> mOffset;
	}

	virtual void save(wxDataOutputStream& stream) const
	{
		CornerObject::save(stream);

		stream << mOffset;
	}

private:
	float mOffset;
};

//---------------------------- PROTOTYPES       -----------------------------//
