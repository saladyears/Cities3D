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
#include "StealProgressCardsDialog.h"
#include "StealProgressCardListCtrl.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	struct CreatorSteal : public wxCardsDialog::CardListCreator
	{
		CreatorSteal(wxInt32 victim)
			: mVictim(victim)
		{}

		virtual ~CreatorSteal()
		{}

		virtual wxCardListCtrl* CreateCardList(wxWindow* parent, wxWindowID id,
			boost::weak_ptr<Game> game, const wxPoint& pos, 
			const wxSize& size)
		{
			return new wxStealProgressCardListCtrl(parent, id, game, mVictim, 
				pos, size);
		}

		wxInt32 mVictim;
	};
}

//---------------------------- PUBLIC           -----------------------------//
wxStealProgressCardsDialog::wxStealProgressCardsDialog(wxWindow *parent, 
	boost::weak_ptr<Game> game, wxInt32 victim)
: wxCardsDialog(parent, game,  stStealCard, stSpyCards,
	boost::shared_ptr<CardListCreator>(new CreatorSteal(victim)), 
	shRuleExecuteSpy)
{
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
