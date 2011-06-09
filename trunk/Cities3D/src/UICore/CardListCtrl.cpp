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
#include "CardListCtrl.h"
#include "NetworkPlayers.h"
#include "Controller.h"
#include "DataObject.h"
#include "INetworkEngine.h"
#include "Game.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	wxString sFields_CardListCtrl[] = {swCard};
}

//---------------------------- PUBLIC           -----------------------------//
wxCardListCtrl::wxCardListCtrl(wxWindow* parent, wxWindowID id, 
	boost::weak_ptr<Game> game, PopulateFunc func, 
	const wxPoint& pos, const wxSize& size)
: wxBaseListCtrl(parent, id, sFields_CardListCtrl, 
	_countof(sFields_CardListCtrl), shBoldFont, shBoldFont, pos, size)
, mPlayer(-1)
, mPopulateFunc(func)
{
	const GamePtr& thisGame = game.lock();
	wxASSERT(thisGame);

	wxASSERT(mPopulateFunc);

	UpdatePlayer(thisGame);

    OnPlayerUI(DataObject(thisGame, mPlayer));
	
	Controller::get().AddReceiver(shEventPlayerUI, &wxCardListCtrl::OnPlayerUI, 
		this);
}

wxCardListCtrl::~wxCardListCtrl()
{
	Controller::get().RemoveReceiver(shEventPlayerUI, 
		&wxCardListCtrl::OnPlayerUI, this);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void wxCardListCtrl::UpdatePlayer(const GamePtr& game)
{
	// If this is a hotseat game, fill it with whoever the current player is,
	// otherwise if it is a network game, it is always the player at this
	// computer.
	const HashString& mode = game->read<HashString>(shMode);

	// The player only needs to be set once in network mode.
	if( (shNetwork == mode) &&
		(-1 == mPlayer))
	{
		const Player& thisPlayer = NET.Players().GetThisPlayer();
		wxInt32 players = game->numPlayers();
		for(wxInt32 i = 0; i < players; ++i)
		{
			if(game->player(i) == thisPlayer)
			{
				mPlayer = i;
				break;
			}
		}
	}
	else if(shHotseat == mode)
	{
		mPlayer = game->read<wxInt32>(shCurrentPlayer);
	}
}

void wxCardListCtrl::PopulateList()
{
	Freeze();

    wxInt32 sel = GetSelectedItem();
    wxString text = GetItemText(sel, swCard);
    wxInt32 newIndex = -1;

    DeleteAllItems();

    for(size_t i = 0; i < mCards.size(); ++i)
    {
        const Card& card = mCards[i];

        size_t index = AppendItem(card.name);
        SetItemColor(index, card.color);
        SetItemData(index, card.playable);

        if( (-1 == newIndex) &&
            (text == card.name))
        {
            newIndex = i;
        }
    }
    
    // Reselect the card they had.
    if(-1 != newIndex)
    {
        SetSelectedItem(newIndex);
    }
    else
    {
        wxListEvent event(wxEVT_COMMAND_LIST_ITEM_DESELECTED, GetId());
	    ProcessEvent(event);
    }
 
    Thaw();

    Refresh();
}

void wxCardListCtrl::OnPlayerUI(const DataObject &object)
{
	wxASSERT(2 <= object.numItems());

	const GamePtr &game = object.read<GamePtr>();
	
	UpdatePlayer(game);
    
    // Retrieve all cards to display.
    mCards.clear();
    mPopulateFunc(game, mPlayer, mCards);

    // If the card list is empty, then close down.  This can happen in a 
    // hotseat game when switching between players.
    if(true == mCards.empty())
    {
        Controller::get().Transmit(shEventCards, DataObject(game, false));
        return;
    }

    PopulateList();
}
