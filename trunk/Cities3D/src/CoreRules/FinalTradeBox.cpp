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
#include "FinalTradeBox.h"
#include "Controller.h"
#include "SkinManager.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	wxInt32 sCurveFinalTrade = 0;
	wxColour sNeutralColorFinalTrade;
	wxColour sAcceptColor;
	wxColour sRejectColor;
}

//---------------------------- PUBLIC           -----------------------------//
wxFinalTradeBox::wxFinalTradeBox(wxWindow* parent, const wxString& name, 
	ColorType color)
: wxStaticBox(parent, -1, name)
, mName(name)
, mState(StateNeutral)
{
	Controller::get().AddReceiver(shEventSkin, &wxFinalTradeBox::OnSkin, this);
	
	SetFirstName(name, color);
	
	// Grab the other information.
	OnSkin();

	SetState(StateNeutral);
}

wxFinalTradeBox::~wxFinalTradeBox()
{
	Controller::get().RemoveReceiver(shEventSkin, &wxFinalTradeBox::OnSkin, 
		this);
}

void wxFinalTradeBox::SetState(State state)
{
	switch(state)
	{
	case StateNeutral:
		mState = state;
		mLeftColor = sNeutralColorFinalTrade;
		mRightColor = sNeutralColorFinalTrade;
		break;
	case StateAcceptLeft:
		mState |= state;
		mLeftColor = sAcceptColor;
		break;
	case StateAcceptRight:
		mState |= state;
		mRightColor = sAcceptColor;
		break;
	case StateRejectRight:
		mState |= state;
		mRightColor = sRejectColor;
		break;
	case StateRejectLeft:
		mState |= state;
		mLeftColor = sRejectColor;
		break;
	default:
		wxASSERT(false);
		break;
	}

	Refresh();
}

void wxFinalTradeBox::SetFirstName(const wxString& name, ColorType color)
{
	mName = name;

	if(CR_SIZE == color)
	{
		mLeftPlayerColor = wxColour(255, 255, 255);
	}
	else
	{
		mLeftPlayerColor = ColorInfoData::UIColor(color);
	}

	Refresh();
}

void wxFinalTradeBox::SetSecondName(const wxString& name, ColorType color)
{
	mSecondName = name;

	if(CR_SIZE == color)
	{
		mRightPlayerColor = wxColour(255, 255, 255);
	}
	else
	{
		mRightPlayerColor = ColorInfoData::UIColor(color);
	}

	Refresh();
}

//---------------------------- PROTECTED        -----------------------------//
#ifdef __WIN32__
void wxFinalTradeBox::PaintBackground(wxDC& dc, const struct tagRECT& rc)
{
	wxColour background = GetBackgroundColour();

	dc.SetPen(wxPen(background));
	dc.SetBrush(wxBrush(background));

	dc.DrawRectangle(0, 0, rc.right, rc.bottom);
}

void wxFinalTradeBox::PaintForeground(wxDC& dc, const struct tagRECT& rc)
{
	wxColour background = GetBackgroundColour();

	dc.SetFont(mFont);
	dc.SetTextForeground(mLeftPlayerColor);
	dc.SetTextBackground(GetBackgroundColour());

	wxInt32 x, y;
	dc.GetTextExtent(mName, &x, &y);

	// Draw left border.
	dc.SetPen(wxPen(mLeftColor));
	dc.SetBrush(wxBrush(background));

	wxInt32 halfX = rc.right / 2;

	dc.SetClippingRegion(0, 0, halfX, rc.bottom);
	dc.DrawRoundedRectangle(0, y / 2, rc.right, rc.bottom - (y / 2), sCurveFinalTrade);

	// Draw right border.
	dc.SetPen(wxPen(mRightColor));

	dc.DestroyClippingRegion();
	dc.SetClippingRegion(halfX, 0, halfX, rc.bottom);
	dc.DrawRoundedRectangle(0, y / 2, rc.right, rc.bottom - (y / 2), sCurveFinalTrade);

	dc.DestroyClippingRegion();

	// Black out area for text.
	dc.SetPen(wxPen(background));
	dc.SetBrush(wxBrush(background));
	dc.DrawRectangle(7, 0, x + 4, y);

	// Draw text.	
	dc.DrawText(mName, 9, 0);

	// Draw second label if it exists.
	if(false == mSecondName.IsEmpty())
	{
		dc.GetTextExtent(mSecondName, &x, &y);

		dc.SetTextForeground(mRightPlayerColor);

		// Black out area for text.
		dc.SetPen(wxPen(background));
		dc.SetBrush(wxBrush(background));
		dc.DrawRectangle(rc.right - x - 11, 0, x + 4, y);

		dc.DrawText(mSecondName, rc.right - x - 8, 0);
	}
}
#endif

//---------------------------- PRIVATE          -----------------------------//
void wxFinalTradeBox::OnSkin(wxInt32)
{
	SetBackgroundColour(SKIN.Element(shBackground));
	mFont = SKIN.Font(shTradeBoxFont);
	
	sCurveFinalTrade = SKIN.Size(shFrameCurve);
	sNeutralColorFinalTrade = SKIN.Element(shGameUIOutline);
	sAcceptColor = SKIN.Element(shPlayerUIPositive);
	sRejectColor = SKIN.Element(shPlayerUINegative);

	Refresh();
}

