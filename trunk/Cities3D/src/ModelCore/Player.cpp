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
#include "Player.h"
#include "define/defineHelper.h"
#include "define/defineFile.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	wxInt32 sNewId = (wxInt32) time(NULL);
}

wxDataOutputStream& operator<<(wxDataOutputStream& out, const Player& player)
{
	out << player.mId;
	out << player.mName;

	return out;
}

wxDataInputStream& operator>>(wxDataInputStream& in, Player& player)
{
	in >> player.mId;
	in >> player.mName;

	return in;
}

//---------------------------- PUBLIC           -----------------------------//
Player::Player()
{
	*this = Null();
}

Player::Player(const wxString& name)
: mId(sNewId++)
, mName(name)
{
}

Player::Player(const wxString& name, wxInt32 id)
: mId(id)
, mName(name)
{
}

const Player &Player::Null()
{
	static Player player(swEmpty, 0);
	return player;
}

bool Player::operator ==(const Player &player) const
{
	return mId == player.mId;
}

bool Player::operator !=(const Player &player) const
{
	return !this->operator==(player);
}

bool Player::operator ==(wxInt32 id) const
{
	return mId == id;
}

bool Player::operator <(const Player &player) const
{
	return mId < player.mId;
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//


