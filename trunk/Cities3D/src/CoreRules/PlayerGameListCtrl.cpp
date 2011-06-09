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
#include "PlayerGameListCtrl.h"
#include "DataObject.h"
#include "GameEngine.h"
#include "Player.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
wxPlayerGameListCtrl::wxPlayerGameListCtrl(wxWindow* parent, wxWindowID id,
		const wxPoint &pos, const wxSize &size)
: wxBasePlayerGameListCtrl(parent, id, pos, size)
{
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void wxPlayerGameListCtrl::ChangeColor(const Player &player,
		const ColorType color)
{
	RULE.Execute(shRuleUIChangeColor, DataObject(player, color));
}

void wxPlayerGameListCtrl::RemovePlayer(const Player &player)
{
	RULE.Execute(shRuleUIRemovePlayer, DataObject(player, CR_SIZE));
}

void wxPlayerGameListCtrl::AddPlayer(const Player &player,
		const ColorType color)
{
	RULE.Execute(shRuleUIAddPlayer, DataObject(player, color));
}
