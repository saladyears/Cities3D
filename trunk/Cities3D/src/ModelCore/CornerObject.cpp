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
#include "CornerObject.h"
#include "Utility.h"
#include "UtilityGL.h"
#include "Game.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
CornerObject::CornerObject(wxInt32 owner, wxInt32 turn, const ColorType color, 
						   const GamePtr &game, const wxInt32 tile, 
						   const wxInt32 corner, 
						   boost::weak_ptr<CornerObject> above)
: mOwner(owner)
, mTurn(turn)
, mColor(color)
, mAbove(above)
{
	tileCorner(tile, corner, game);
}

CornerObject::CornerObject()
: mOwner(-1)
, mTurn(-1)
, mTile1(TileCorner(0, 0))
, mTile2(TileCorner(0, 0))
, mTile3(TileCorner(0, 0))
, mColor(CR_WHITE)
{
}

CornerObject::~CornerObject()
{
	CornerObjectPtr above = mAbove.lock();
	CornerObjectPtr below = mBelow.lock();

	// Set pointers.
	if(above)
	{
		above->below(below);
	}
	if(below)
	{
		below->above(above);
	}
}

bool CornerObject::operator==(const CornerObject &object) const
{
	return((mTile1 == object.mTile1) &&
		   (mTile2 == object.mTile2) &&
		   (mTile3 == object.mTile3) &&
		   (mColor == object.mColor));
}

void CornerObject::tileCorner(wxInt32 tile, wxInt32 corner, 
	const GamePtr& game)
{
	mTile1 = std::make_pair(tile, corner);

	//set the game object ID
	mID = Utility::encodeSel(tile, corner);

	//determine other sides
	wxInt32 tile2 = 0, tile3 = 0;
	wxInt32 corner2 = 0, corner3 = 0;

	if(game)
	{
		Utility::otherTiles(game, tile, corner, tile2, corner2, tile3, 
			corner3);

		//determine coordinates
		Vector tileCoords = game->tile(tile)->coords();

		//find the offset
		UtilityGL::cornerCoords(tileCoords, corner);

		coords(tileCoords);
	}

	mTile2 = std::make_pair(tile2, corner2);
	mTile3 = std::make_pair(tile3, corner3);
}

bool CornerObject::onTile(wxInt32 tile)
{
	return( (mTile1.first == tile) ||
			(mTile2.first == tile) ||
			(mTile3.first == tile));
}

bool CornerObject::onTileCorner(const TileCorner tile)
{
	return( (mTile1 == tile) ||
			(mTile2 == tile) ||
			(mTile3 == tile));
}

wxInt32 CornerObject::links() const
{
	wxInt32 count = 0;

	CornerObjectPtr below = mBelow.lock();
	while(below)
	{
		below = below->below().lock();
		++count;
	}

	return count;
}

void CornerObject::load(wxDataInputStream &stream)
{
	GameObject::load(stream);

	wxInt32 first, second;

	stream >> mOwner;
	stream >> mTurn;

	stream >> first;
	stream >> second;
	mTile1 = TileCorner(first, second);

	stream >> first;
	stream >> second;
	mTile2 = TileCorner(first, second);

	stream >> first;
	stream >> second;
	mTile3 = TileCorner(first, second);

	stream >> first;
	mColor = static_cast<ColorType>(first);
}

void CornerObject::save(wxDataOutputStream &stream) const
{
	GameObject::save(stream);

	stream << mOwner;
	stream << mTurn;

	//the tile objects
	stream << mTile1.first;
	stream << mTile1.second;

	stream << mTile2.first;
	stream << mTile2.second;

	stream << mTile3.first;
	stream << mTile3.second;

	stream << mColor;
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//



