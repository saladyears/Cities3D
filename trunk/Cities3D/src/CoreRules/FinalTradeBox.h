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
#pragma once

#include "style.h"	//READ THIS BEFORE MAKING ANY CHANGES TO THIS FILE!!!

//---------------------------- SYSTEM INCLUDES  -----------------------------//

//---------------------------- USER INCLUDES    -----------------------------//
#include "ColorInfoData.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxFinalTradeBox
// 
// Static box that contains the final trade offer between two players.
//
// Derived From:
//     <wxStaticBox>
//
// Project:
//     <Cities3D>
//
// Include:
//     TradeOfferBox.h
//
class wxFinalTradeBox : public wxStaticBox
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	enum State
	{
		StateNeutral		= 0,
		StateAcceptLeft		= 1,
		StateAcceptRight	= 2,
		StateRejectRight	= 4,
		StateRejectLeft		= 8,
	};

	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxFinalTradeBox
	//
	// The wxFinalTradeBox constructor.
	//
	// Parameters:
	//     parent - The parent window.  Must not be NULL.
	//     name - The player name for this window.
	//     color - The color of the player name.
	//
	wxFinalTradeBox(wxWindow* parent, const wxString& name, ColorType color);

	~wxFinalTradeBox();

	void SetState(State state);

	void SetFirstName(const wxString& name, ColorType color);
	void SetSecondName(const wxString& name, ColorType color);

#ifdef __WIN32__
protected:
	// paint the given rectangle with our background brush/colour
    virtual void PaintBackground(wxDC& dc, const struct tagRECT& rc);
    // paint the foreground of the static box
    virtual void PaintForeground(wxDC& dc, const struct tagRECT& rc);
#endif

private:
	void OnSkin(wxInt32=0);

	size_t mState;

	wxFont mFont;

	wxString mName;
	wxString mSecondName;

	wxColour mLeftPlayerColor;
	wxColour mRightPlayerColor;

	wxColour mLeftColor;
	wxColour mRightColor;
};

//---------------------------- PROTOTYPES       -----------------------------//

