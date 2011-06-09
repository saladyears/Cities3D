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
#include "SideObject.h"
#include "Utility.h"
#include "UtilityGL.h"
#include "Game.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
SideObject::SideObject(wxInt32 owner, wxInt32 turn, const ColorType color, 
					   const GamePtr &game, const wxInt32 tile, 
					   const wxInt32 side)
: mOwner(owner)
, mTurn(turn)
, mTile1(std::make_pair(tile, side))
, mColor(color)
, mLongest(false)
{
	//set the game object ID
	mID = Utility::encodeSel(tile, side);

	if(game)
	{
		//determine other side
		wxInt32 tile2 = game->tile(tile)->read<IntArray>(shSides)[side];
		wxASSERT(-1 != tile2);

		wxInt32 corner2 = (side + 3) % 6;

		mTile2 = std::make_pair(tile2, corner2);
	
		//determine coordinates
		mCoords = game->tile(tile)->coords();

		//find the offset and rotation
		UtilityGL::sideCoords(mCoords, mRotation.y, side);
	}
}

bool SideObject::operator==(const SideObject &object) const
{
	return((mTile1 == object.mTile1) &&
		   (mTile2 == object.mTile2) &&
		   (mColor == object.mColor));
}

void SideObject::load(wxDataInputStream &stream)
{
	GameObject::load(stream);

	wxInt32 first, second;

	stream >> mOwner;
	stream >> mTurn;

	stream >> first;
	stream >> second;
	mTile1 = TileSide(first, second);

	stream >> first;
	stream >> second;
	mTile2 = TileSide(first, second);

	stream >> first;
	mColor = static_cast<ColorType>(first);

	stream >> first;
	mLongest = first || false;
}

void SideObject::save(wxDataOutputStream &stream) const
{
	GameObject::save(stream);

	stream << mOwner;
	stream << mTurn;

	stream << mTile1.first;
	stream << mTile1.second;

	stream << mTile2.first;
	stream << mTile2.second;

	stream << mColor;
	stream << static_cast<wxInt32>(mLongest);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//



