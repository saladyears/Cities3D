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
#include "TradeOfferBox.h"
#include "Controller.h"
#include "SkinManager.h"
#include "TimerManager.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	wxInt32 sCurveTradeOffer = 0;
	wxColour sNeutralColorTradeOffer;

	void ForwardMouseEvent(wxMouseEvent &event, wxWindow *window, 
		wxWindow *parent)
	{
		wxASSERT(NULL != window);
		wxASSERT(NULL != parent);

		// Set the point as parent coordinates.
		wxPoint pos = event.GetPosition();
		window->ClientToScreen(&pos.x, &pos.y);
		parent->ScreenToClient(&pos.x, &pos.y);
		event.m_x = pos.x;
		event.m_y = pos.y;

		parent->ProcessEvent(event);
	}

	const size_t TIMER_RESOLUTION = 500;
	const size_t FLASH_COUNT = 6;
	wxInt32 sCurrentFlash = 0;
}

//---------------------------- PUBLIC           -----------------------------//
wxTradeOfferBox::wxTradeOfferBox(wxWindow* parent, const wxString& name, 
	ColorType color, wxInt32 index, wxInt32 current, wxInt32 player, 
	const wxString& right)
: wxStaticBox(parent, -1, name)
, mIndex(index)
, mCurrentIndex(current)
, mPlayerIndex(player)
, mName(name)
, mRight(right)
, mState(StateNeutral)
{
	Controller::get().AddReceiver(shEventSkin, &wxTradeOfferBox::OnSkin, this);
	
	if(CR_SIZE == color)
	{
		mPlayerColor = wxColour(255, 255, 255);
	}
	else
	{
		mPlayerColor = ColorInfoData::UIColor(color);
	}
	
	// Grab the other information.
	OnSkin();

	SetState(StateNeutral);
}

wxTradeOfferBox::~wxTradeOfferBox()
{
	Controller::get().RemoveReceiver(shEventSkin, &wxTradeOfferBox::OnSkin, 
		this);
	TIME.RemoveOneShotFunction(&wxTradeOfferBox::OnTimer, this, 
		TIMER_RESOLUTION);
}

void wxTradeOfferBox::SetState(State state)
{
	wxString str;
	wxString str2;
	wxColour color;

	switch(state)
	{
	case StateNeutral:
		str = swStringFormat;
		color = sNeutralColorTradeOffer;
		break;
	case StateOffer:
		str = swStringFormat;
		color = mPlayerColor;

		// On a new offer, flash the offer.
		sCurrentFlash = FLASH_COUNT;
		TIME.AddOneShotFunction(&wxTradeOfferBox::OnTimer, this,
			TIMER_RESOLUTION);
		break;
	case StateAccept:
		if(mIndex == mPlayerIndex)
		{
			str = stYouAccept;
			mRight = swEmpty;
		}
		else
		{
			str = stStringAccepts;
			mRight = swEmpty;
		}
		color = mPlayerColor;
		break;
	case StateCounteroffer:
		if(mPlayerIndex == mCurrentIndex)
		{
			str2 = stStringOffers;
			str = stStringWants;
		}
		else if(mIndex == mPlayerIndex)
		{
			str = stYouOffer;
			str2 = stYouWant;
		}
		else
		{
			str = stStringOffers;
			str2 = stStringWants;
		}
		color = mPlayerColor;
		break;
	case StateReject:
		if(mIndex == mPlayerIndex)
		{
			str = stYouPass;
			mRight = swEmpty;
		}
		else
		{
			str = stStringPasses;
			mRight = swEmpty;
		}
		color = sNeutralColorTradeOffer;
		break;
	case StateWontTrade:
		if(mIndex == mPlayerIndex)
		{
			str = stYouWontTrade;
			mRight = swEmpty;
		}
		else
		{
			str = stStringWontTrade;
			mRight = swEmpty;
		}
		color = sNeutralColorTradeOffer;
		break;
	default:
		wxASSERT(false);
		break;
	};

	mState = state;
	mText = wxString::Format(str.c_str(), mName.c_str());

	if(false == str2.IsEmpty())
	{
		mRight = wxString::Format(str2.c_str(), mName.c_str());
	}

	mCurrentColor = color;

	Refresh();
}

//---------------------------- PROTECTED        -----------------------------//
#ifdef __WIN32__
void wxTradeOfferBox::PaintBackground(wxDC& dc, const struct tagRECT& rc)
{
	wxColour background = GetBackgroundColour();

	dc.SetPen(wxPen(background));

	wxBrush brush(background);

	dc.SetBrush(brush);
	dc.DrawRectangle(0, 0, rc.right, rc.bottom);

	// Won't trade has a special black out.
	if(StateWontTrade == mState)
	{
		wxInt32 x, y;
		dc.GetTextExtent(mText, &x, &y);

		brush.SetColour(mCurrentColor);
		brush.SetStyle(wxBDIAGONAL_HATCH);

		dc.SetBrush(brush);
		dc.DrawRectangle(0, y / 2, rc.right, rc.bottom - (y / 2));
	}
}

void wxTradeOfferBox::PaintForeground(wxDC& dc, const struct tagRECT& rc)
{
	wxColour background = GetBackgroundColour();

	dc.SetFont(mFont);
	dc.SetTextForeground(mPlayerColor);
	dc.SetTextBackground(background);

	wxInt32 x, y;
	dc.GetTextExtent(mText, &x, &y);

	// Draw border.
	dc.SetPen(wxPen(mCurrentColor));
	
	wxBrush brush(background);

	if(StateWontTrade == mState)
	{
		brush.SetColour(mCurrentColor);
		brush.SetStyle(wxBDIAGONAL_HATCH);
	}

	wxInt32 yStart = (y / 2);

	dc.SetBrush(brush);

	// Draw a double line for accept.
	if( (StateAccept == mState) ||
		(sCurrentFlash % 2))
	{
		yStart--;
		dc.DrawRoundedRectangle(0, yStart, rc.right, rc.bottom - yStart, 
			sCurveTradeOffer);
		yStart += 2;

		dc.DrawRoundedRectangle(2, yStart, rc.right - 4, 
			rc.bottom - yStart - 2, sCurveTradeOffer);
	}
	else
	{
		dc.DrawRoundedRectangle(0, yStart, rc.right, rc.bottom - yStart, 
			sCurveTradeOffer);
	}

	// Black out area for text.
	dc.SetPen(wxPen(background));
	dc.SetBrush(wxBrush(background));
	dc.DrawRectangle(7, 0, x + 4, y);

	// Draw text.	
	dc.DrawText(mText, 9, 0);

	// Draw second label if it exists.
	if(false == mRight.IsEmpty())
	{
		dc.GetTextExtent(mRight, &x, &y);

		// Black out area for text.
		dc.SetPen(wxPen(background));
		dc.SetBrush(wxBrush(background));
		dc.DrawRectangle(rc.right - x - 11, 0, x + 4, y);

		dc.DrawText(mRight, rc.right - x - 8, 0);
	}
}
#endif

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxTradeOfferBox, wxStaticBox)
	EVT_MOTION(wxTradeOfferBox::OnMouseMove)
	EVT_LEFT_DOWN(wxTradeOfferBox::OnLeftDown)
	EVT_LEFT_UP(wxTradeOfferBox::OnLeftUp)
	EVT_RIGHT_UP(wxTradeOfferBox::OnRightUp)
	EVT_LEFT_DCLICK(wxTradeOfferBox::OnLeftDoubleClick)
END_EVENT_TABLE()

void wxTradeOfferBox::OnSkin(wxInt32)
{
	mFont = SKIN.Font(shTradeBoxFont);
	
	sCurveTradeOffer = SKIN.Size(shFrameCurve);
	sNeutralColorTradeOffer = SKIN.Element(shGameUIOutline);

	Refresh();
}

void wxTradeOfferBox::OnTimer()
{
	--sCurrentFlash;

	if(0 < sCurrentFlash)
	{
		TIME.AddOneShotFunction(&wxTradeOfferBox::OnTimer, this,
			TIMER_RESOLUTION, false);
	}

	Refresh();
}

void wxTradeOfferBox::OnMouseMove(wxMouseEvent &event)
{
	ForwardMouseEvent(event, this, GetParent());
}

void wxTradeOfferBox::OnLeftDown(wxMouseEvent &event)
{
	ForwardMouseEvent(event, this, GetParent());

	event.Skip();
}

void wxTradeOfferBox::OnLeftUp(wxMouseEvent &event)
{
	ForwardMouseEvent(event, this, GetParent());
}

void wxTradeOfferBox::OnRightUp(wxMouseEvent &event)
{
	ForwardMouseEvent(event, this, GetParent());
}

void wxTradeOfferBox::OnLeftDoubleClick(wxMouseEvent &event)
{
	ForwardMouseEvent(event, this, GetParent());
}
