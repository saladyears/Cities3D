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

//---------------------------- USER INCLUDES    -----------------------------//
#include "ExtraCornerObject.h"
#include "RuleSetDatabase.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//
class MetropolisObject : public ExtraCornerObject
{
	DECLARE_GAME_OBJECT(MetropolisObject);

public:
	enum Type
	{
		Trade			= 0,
		Politics		,
		Science			,
	};

	MetropolisObject(wxInt32 owner, wxInt32 turn, Type type, 
		const GamePtr &game, wxInt32 tile, wxInt32 corner)
	: ExtraCornerObject(owner, turn, CR_WHITE, game, tile, corner, 
		boost::weak_ptr<CornerObject>())
	, mType(type)
	{
	}

	MetropolisObject() 
		: ExtraCornerObject(-1, -1, CR_WHITE, GamePtr(), 0, 0, 
			boost::weak_ptr<CornerObject>())
		, mType(Trade)
	{}

	Type type() const
	{
		return mType;
	}

	virtual Vector uicoords() const
	{
		return Vector(0.0f, -1.00f, 0.0f);
	}

	virtual void load(wxDataInputStream &stream)
	{
		ExtraCornerObject::load(stream);

		wxInt32 type;
		stream >> type;

		mType = static_cast<Type>(type);
	}

	virtual void save(wxDataOutputStream &stream) const
	{
		ExtraCornerObject::save(stream);

		stream << static_cast<wxInt32>(mType);
	}

protected:
	void type(Type type)
	{
		mType = type;
	}

private:

	Type mType;
};

class TradeMetropolisObject : public MetropolisObject
{
public:
	TradeMetropolisObject(wxInt32 owner, wxInt32 turn, const GamePtr &game, 
		wxInt32 tile, wxInt32 corner) 
		: MetropolisObject(owner, turn, Trade, game, tile, corner) 
	{
	}

	TradeMetropolisObject() : MetropolisObject() 
	{
		type(Trade);
	}
};

class PoliticsMetropolisObject : public MetropolisObject
{
public:
	PoliticsMetropolisObject(wxInt32 owner, wxInt32 turn, const GamePtr &game, 
		wxInt32 tile, wxInt32 corner) 
		: MetropolisObject(owner, turn, Politics, game, tile, corner) 
	{
	}

	PoliticsMetropolisObject() : MetropolisObject() 
	{
		type(Politics);
	}
};

class ScienceMetropolisObject : public MetropolisObject
{
public:
	ScienceMetropolisObject(wxInt32 owner, wxInt32 turn, const GamePtr &game, 
		wxInt32 tile, wxInt32 corner) 
		: MetropolisObject(owner, turn, Science, game, tile, corner) 
	{
	}

	ScienceMetropolisObject() : MetropolisObject() 
	{
		type(Science);
	}
};

//---------------------------- PROTOTYPES       -----------------------------//
IMPLEMENT_GAME_OBJECT(MetropolisObject, CITIES);
