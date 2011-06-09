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
// Class: wxTradeOfferBox
// 
// Static box that contains trade offer windows.
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
class wxTradeOfferBox : public wxStaticBox
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	enum State
	{
		StateNeutral		= 0,
		StateOffer			,
		StateAccept			,
		StateCounteroffer	,
		StateReject			,
		StateWontTrade		,
	};

	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxTradeOfferBox
	//
	// The wxTradeOfferBox constructor.
	//
	// Parameters:
	//     parent - The parent window.  Must not be NULL.
	//     name - The player name for this window.
	//     color - The color of the player.
	//     index - The index of the player this box represents.
	//     current - The current player index.
	//     player - This player at this computer's index.
	//     right - Any text ont he right side of the box.
	//
	wxTradeOfferBox(wxWindow* parent, const wxString& name, ColorType color,
		wxInt32 index, wxInt32 current, wxInt32 player, 
		const wxString& right = swEmpty);

	~wxTradeOfferBox();

	void SetState(State state);

	State GetState() const
	{
		return mState;
	}

#ifdef __WIN32__
protected:
	// paint the given rectangle with our background brush/colour
    virtual void PaintBackground(wxDC& dc, const struct tagRECT& rc);
    // paint the foreground of the static box
    virtual void PaintForeground(wxDC& dc, const struct tagRECT& rc);
#endif

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Macro: DECLARE_EVENT_TABLE
	//
	// The wxWidgets event table declaration.
	//
	DECLARE_EVENT_TABLE()

private:
	//-----------------------------------------------------------------------//
	// Group: Event Message Handling
	//

	//-----------------------------------------------------------------------//
	// Function: OnMouseMove
	//
	// Forwards mouse move events to the parent.
	//
	// Parameters:
	//     event - The mouse event.
	//
	virtual void OnMouseMove(wxMouseEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnLeftDown
	//
	// Handles the left mouse down event.
	// 
	// Parameters:
	//     event - The mouse event.
	//
	virtual void OnLeftDown(wxMouseEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnLeftUp
	//
	// Handles the left mouse up event.
	// 
	// Parameters:
	//     event - The mouse event.
	//
	virtual void OnLeftUp(wxMouseEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnRightUp
	//
	// Handles the right mouse up event.
	// 
	// Parameters:
	//     event - The mouse event.
	//
	virtual void OnRightUp(wxMouseEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnLeftDoubleClick
	//
	// Handles the left mouse double-click event.
	// 
	// Parameters:
	//     event - The mouse event.
	//
	virtual void OnLeftDoubleClick(wxMouseEvent &event);

	void OnSkin(wxInt32=0);

	void OnTimer();

	State mState;

	wxFont mFont;

	wxInt32 mIndex;
	wxInt32 mCurrentIndex;
	wxInt32 mPlayerIndex;

	wxString mName;
	wxString mText;
	wxString mRight;

	wxColour mPlayerColor;
	wxColour mCurrentColor;
};

//---------------------------- PROTOTYPES       -----------------------------//

