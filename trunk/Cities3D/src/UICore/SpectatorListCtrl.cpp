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
#include "SpectatorListCtrl.h"
#include "Controller.h"
#include "Player.h"
#include "GameEngine.h"
#include "NetworkPlayers.h"
#include "ColorInfoData.h"
#include "INetworkEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	wxString sFields_SpectatorListCtrl[] = {swSpectator};
}

//---------------------------- PUBLIC           -----------------------------//
wxSpectatorListCtrl::wxSpectatorListCtrl(wxWindow* parent, wxWindowID id, 
										 const wxPoint& pos, 
										 const wxSize& size)
: wxBaseListCtrl(parent, id, sFields_SpectatorListCtrl, 
	_countof(sFields_SpectatorListCtrl), shSpectatorListMainFont, 
	shSpectatorListSubFont, pos, size)
{
	Controller::get().AddReceiver(shEventSpectator, 
		&wxSpectatorListCtrl::OnUpdateSpectators, this);
}

wxSpectatorListCtrl::~wxSpectatorListCtrl()
{
	Controller::get().RemoveReceiver(shEventSpectator, 
		&wxSpectatorListCtrl::OnUpdateSpectators, this);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void wxSpectatorListCtrl::OnUpdateSpectators(wxInt32)
{
	Lock();

	//clear existing spectators
	DeleteAllItems();

	const SpectatorArray &spectators = NET.Players().GetSpectators();

	SpectatorArray::const_iterator it, itEnd = spectators.end();
	for(it = spectators.begin(); it != itEnd; ++it)
	{
		size_t index = AppendItem(it->first.Name());
		SetSpectatorColor(index);
	}

	Unlock();
}
