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
#include "RuleSetDatabase.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//
class KnightObject : public CornerObject
{
	DECLARE_GAME_OBJECT(KnightObject);

public:
	KnightObject(wxInt32 owner, wxInt32 turn, const ColorType color, 
		const GamePtr &game, const wxInt32 tile, const wxInt32 corner)
	: CornerObject(owner, turn, color, game, tile, corner)
	, mStrength(1)
	, mActiveTurn(-1)
	, mActive(false)
	{
	}

	KnightObject() 
		: CornerObject(-1, -1, CR_WHITE, GamePtr(), 0, 0)
		, mStrength(1)
		, mActive(false)
	{}

	virtual Vector uicoords() const
	{
		return Vector(0.0f, -0.3333f, 0.0f);
	}

	virtual bool bridge() const
	{
		return false;
	}

	wxInt32 strength() const 
	{
		return mStrength;
	}

	void strength(wxInt32 strength) 
	{
		mStrength = strength;
	}

	bool active() const 
	{
		return mActive;
	}

	void active(bool active, wxInt32 turn) 
	{
		mActive = active;
		mActiveTurn = turn;
	}

	wxInt32 active_turn() const
	{
		return mActiveTurn;
	}

	void upgrade(bool upgrade, wxInt32 turn)
	{
		if(true == upgrade)
		{
			++mStrength;
			wxASSERT(3 >= mStrength);
		}
		else
		{
			--mStrength;
			wxASSERT(0 <= mStrength);
		}
		
		mUpgradeTurn = turn;
	}

	wxInt32 upgrade_turn() const
	{
		return mUpgradeTurn;
	}

	virtual void load(wxDataInputStream &stream)
	{
		CornerObject::load(stream);

		wxBOOL active;

		stream >> mStrength;
		stream >> mActiveTurn;
		stream >> mUpgradeTurn;
		stream >> active;

		mActive = active ? true : false;
	}

	virtual void save(wxDataOutputStream &stream) const
	{
		CornerObject::save(stream);

		wxBOOL active = mActive ? TRUE : FALSE;

		stream << mStrength;
		stream << mActiveTurn;
		stream << mUpgradeTurn;
		stream << active;
	}

private:
	wxInt32 mStrength;
	wxInt32 mActiveTurn;
	wxInt32 mUpgradeTurn;
	bool mActive;
};

//use some fake classes to set up initial 2nd and 3rd level knights
class Knight2Object : public KnightObject
{
public:
	Knight2Object(wxInt32 owner, wxInt32 turn, const ColorType color, 
		const GamePtr &game, const wxInt32 tile, const wxInt32 corner) 
		: KnightObject(owner, turn, color, game, tile, corner) 
	{
		strength(2);
	}

	Knight2Object() : KnightObject() 
	{
		strength(2);
	}
};

class Knight3Object : public KnightObject
{
public:
	Knight3Object(wxInt32 owner, wxInt32 turn, const ColorType color, 
		const GamePtr &game, const wxInt32 tile, const wxInt32 corner) 
	: KnightObject(owner, turn, color, game, tile, corner) 
	{
		strength(3);
	}

	Knight3Object() : KnightObject() 
	{
		strength(3);
	}
};

class ActiveKnightObject : public KnightObject
{
public:
	ActiveKnightObject(wxInt32 owner, wxInt32 turn, const ColorType color, 
		const GamePtr &game, const wxInt32 tile, const wxInt32 corner) 
	: KnightObject(owner, turn, color, game, tile, corner) 
	{
		active(true, -1);
	}

	ActiveKnightObject() : KnightObject() 
	{
		active(true, -1);
	}
};

//---------------------------- PROTOTYPES       -----------------------------//
IMPLEMENT_GAME_OBJECT(KnightObject, CITIES);
