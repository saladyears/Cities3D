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
#include "TradeOffer.h"
#include "TradeOfferBox.h"
#include "ImageStrip.h"
#include "BaseStaticText.h"
#include "GameEngine.h"
#include "IRuleEngine.h"
#include "DataObject.h"
#include "ImageStrip.h"
#include "BaseStaticText.h"
#include "ConfigDatabase.h"
#include "Controller.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	TradeOffer::ResourceMap sCurrentLeft;
	TradeOffer::ResourceMap sCurrentRight;
}

void TradeOffer::AcceptOffer(wxInt32 playerIndex)
{
	RULE.Execute(shRuleOfferTrade, DataObject(sCurrentLeft, sCurrentRight, 
		playerIndex));
}

//---------------------------- PUBLIC           -----------------------------//
TradeOffer::TradeOffer(wxInt32 index, bool trader, wxWindow* window, 
	wxSizer* sizer, wxTradeOfferBox* box)
: mPlayerIndex(index)
, mTrader(trader)
, mBox(box)
{
	mBox->SetBackgroundColour(window->GetBackgroundColour());
	mSizer = new wxStaticBoxSizer(mBox, wxHORIZONTAL);

	mLeft = new wxImageStrip(window, -1, 4, 10);
	mLeft->SetPixelSpacing(4);
	mLeft->SetFlags(wxImageStrip::FlagReverseOrder |
		wxImageStrip::FlagForwardMouseEvents | 
		wxImageStrip::FlagAlignCenter);
	mRight = new wxImageStrip(window, -1, 4, 10);
	mRight->SetPixelSpacing(4);
	mRight->SetFlags(wxImageStrip::FlagForwardMouseEvents |
		wxImageStrip::FlagAlignCenter);

	mText = new wxBaseStaticText(window, -1, stFor);
	mText->Hide();

	// See if we need to force max size.
	wxInt32 width = CONFIG.read(swConfigSectionUI, swStockyardSlash + swWidth, -1);
	wxInt32 height = CONFIG.read(swConfigSectionUI, swStockyardSlash + swHeight, -1);

	wxInt32 initial = 0;

	if( (-1 == width) ||
		(-1 == height))
	{
		initial = -1;
	}

	wxSizeEvent event(wxSize(initial, initial));

	mLeft->ProcessEvent(event);
	mRight->ProcessEvent(event);

	mSizer->Add(mLeft, 1, wxEXPAND | wxLEFT | wxTOP, 2);
	mSizer->Add(mText, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
	mSizer->Add(mRight, 1, wxEXPAND | wxRIGHT | wxTOP, 2);

	sizer->Add(mSizer, 1, wxEXPAND | wxALL, 4);

	Controller::get().AddReceiver(shEventTrade, &TradeOffer::OnTrade, this);
	Controller::get().AddReceiver(shEventCurrentOffer, 
		&TradeOffer::OnCurrentOffer, this);
	Controller::get().AddReceiver(shEventPlayerOffer, 
		&TradeOffer::OnPlayerOffer, this);
	Controller::get().AddReceiver(shEventRejectOffer, 
		&TradeOffer::OnRejectOffer, this);
	Controller::get().AddReceiver(shEventWontTrade, &TradeOffer::OnWontTrade, 
		this);
}

TradeOffer::~TradeOffer()
{
	Controller::get().RemoveReceiver(shEventTrade, &TradeOffer::OnTrade, this);
	Controller::get().RemoveReceiver(shEventCurrentOffer, 
		&TradeOffer::OnCurrentOffer, this);
	Controller::get().RemoveReceiver(shEventPlayerOffer, 
		&TradeOffer::OnPlayerOffer, this);
	Controller::get().RemoveReceiver(shEventRejectOffer, 
		&TradeOffer::OnRejectOffer, this);
	Controller::get().RemoveReceiver(shEventWontTrade, &TradeOffer::OnWontTrade, 
		this);
}

bool TradeOffer::CompareOffer(const TradeOffer& other) const
{
	ResourceMap left, otherLeft;
	ResourceMap right, otherRight;

	mLeft->FillResourceMap(left);
	mRight->FillResourceMap(right);

	other.mLeft->FillResourceMap(otherLeft);
	other.mRight->FillResourceMap(otherRight);

	return (left == otherLeft) && (right == otherRight);
}

void TradeOffer::MatchOffer(wxInt32 playerIndex)
{
	// Players may only match offers of other players.
	if(-2 != mPlayerIndex)
	{
		ResourceMap left;
		ResourceMap right;

		mLeft->FillResourceMap(left);
		mRight->FillResourceMap(right);

		DataObject input(left, right, playerIndex), output;
		RULE.Decide(shLogicCanAcceptTrade, input, output);

		// If they can do it, make the offer.
		if(true == output.read<bool>())
		{
			RULE.Execute(shRuleOfferTrade, DataObject(left, right, 
				mPlayerIndex));
		}
	}
}

void TradeOffer::FillOffer(DataObject& object) const
{
	ResourceMap left, right;

	mLeft->FillResourceMap(left);
	mRight->FillResourceMap(right);

	object = DataObject(left, right, mPlayerIndex);		
}

wxInt32 TradeOffer::TestClick(const wxPoint &pos, wxInt32 playerIndex, 
	bool& canTrade) const
{
	wxInt32 ret = -1;
	wxRect rect(mSizer->GetPosition(), mSizer->GetSize());

	if( (true == rect.Contains(pos)) &&
		(-1 != playerIndex))
	{
		// See if they can actually accept this trade.
		ResourceMap left, right;
		mLeft->FillResourceMap(left);
		mRight->FillResourceMap(right);

		// There must be resources to accept.
		if(	(false == left.empty()) &&
			(false == right.empty()) &&
			((true == mTrader) ||
			(playerIndex != mPlayerIndex)))
		{
			DataObject input(left, right, playerIndex), output;
			RULE.Decide(shLogicCanAcceptTrade, input, output);
			canTrade = output.read<bool>();

			ret = mPlayerIndex;
		}
	}

	return ret;
}

void TradeOffer::SetBackgroundColour(wxColour color)
{
	mBox->SetBackgroundColour(color);
	mLeft->SetBackgroundColour(color);
	mRight->SetBackgroundColour(color);
	mText->SetBackgroundColour(color);
}

wxRect TradeOffer::GetSizerRect() const
{
	wxRect rect(mSizer->GetPosition(), mSizer->GetSize());
	return rect;
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void TradeOffer::SetOffer(const ResourceMap& left, const ResourceMap& right)
{
	// Clear and fill the current image strips.
	mLeft->Clear();
	mRight->Clear();

	mLeft->SetResourceMap(left);
	mRight->SetResourceMap(right);

	mText->Show();
	mSizer->Layout();
}

void TradeOffer::ClearOffer(wxTradeOfferBox::State state =
							wxTradeOfferBox::StateNeutral)
{
	mLeft->Clear();
	mRight->Clear();
	mText->Hide();

	mBox->SetState(state);
}

void TradeOffer::OnTrade(wxInt32 partner)
{
	// Clear the current offer and the person that traded.  In addition, any
	// players that rejected that last offer should be cleared.
	if( (-2 == mPlayerIndex) ||
		(partner == mPlayerIndex) ||
		(wxTradeOfferBox::StateReject == mBox->GetState()))
	{
		ClearOffer();
	}
}

void TradeOffer::OnCurrentOffer(const DataObject& object)
{
	wxASSERT(2 <= object.numItems());

	ResourceMap left, right;

	// Switch the order for tradees so that what they give up is always on the
	// left side.
	if(true == mTrader)
	{
		left = object.read<ResourceMap>();
		right = object.read<ResourceMap>(1);
	}
	else
	{
		left = object.read<ResourceMap>(1);
		right = object.read<ResourceMap>();
	}

	// If this is the current offer, set the values.
	if(-2 == mPlayerIndex)
	{
		SetOffer(left, right);
		mBox->SetState(wxTradeOfferBox::StateOffer);

		sCurrentLeft = left;
		sCurrentRight = right;
	}
	// Reset any player offers marked as reject to neutral.  Any player marked
	// as accept or counteroffer should have their trade remain on the table,
	// in case the trader decides to take it.  Any player marked as won't trade
	// should remain in the won't trade state, as by definition, they do not
	// care about new trade offers.
	else if(wxTradeOfferBox::StateReject == mBox->GetState())
	{
		ClearOffer();
	}
}

void TradeOffer::OnPlayerOffer(const DataObject& object)
{
	wxASSERT(3 <= object.numItems());

	wxInt32 player = object.read<wxInt32>(2);

	if(mPlayerIndex == player)
	{
		ResourceMap left, right;

		// If this is the trader, flip the display so they show up the way the
		// player sees them.
		if(true == mTrader)
		{
			left = object.read<ResourceMap>(1);
			right = object.read<ResourceMap>();
		}
		else
		{
			left = object.read<ResourceMap>();
			right = object.read<ResourceMap>(1);
		}

		SetOffer(left, right);
	
		// If the offer matches the current offer, they are accepting the
		// trade.
		if( (left == sCurrentLeft) &&
			(right == sCurrentRight))
		{
			RULE.AsynchExecute(shRulePlaySound, DataObject(SOUND_TRADE_ACCEPT));

			mBox->SetState(wxTradeOfferBox::StateAccept);
		}
		// Otherwise, they are making a counteroffer.
		else
		{
			RULE.AsynchExecute(shRulePlaySound, DataObject(SOUND_TRADE_COUNTER));

			mBox->SetState(wxTradeOfferBox::StateCounteroffer);
		}
	}
}

void TradeOffer::OnRejectOffer(wxInt32 player)
{
	if(player == mPlayerIndex)
	{
		ClearOffer(wxTradeOfferBox::StateReject);
	}
}

void TradeOffer::OnWontTrade(wxInt32 player)
{
	// If we match this player, clear any existing offer and set the box state.
	if(player == mPlayerIndex)
	{
		ClearOffer(wxTradeOfferBox::StateWontTrade);
	}
}
