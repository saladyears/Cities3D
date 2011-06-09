/*
 *  Cities3D - CopymRight (C) 2001-2009 Jason Fugate (saladyears@gmail.com)
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
#include <map>

//---------------------------- USER INCLUDES    -----------------------------//
#include "TradeOfferBox.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class wxImageStrip;
class wxBaseStaticText;
class DataObject;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: TradeOffer
// 
// Encapsulates all of the UI elements for one trade offer in the 
// <wxStockyardDialog>.
//
// Project:
//     <CoreRules>
//
// Include:
//     TradeOffer.h
//
class TradeOffer
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: TradeOffer
	//
	// The default TradeOffer constructor.
	//
	// Parameters:
	//     index - The index of the player the TradeOffer belongs to, or -2
	//             if it is the current trade offer from the trader.
	//     trader - A bool that is true if the person is the trader and false
	//              if not.
	//     window - The parent window.
	//     sizer - The sizer to add the elements to.
	//     box - The <wxTradeOfferBox>.
	//
	TradeOffer(wxInt32 index, bool trader, wxWindow* window, wxSizer* sizer, 
		wxTradeOfferBox* box);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~TradeOffer
	//
	// The TradeOffer destructor.
	//
	~TradeOffer();

	//-----------------------------------------------------------------------//
	// Group: Static Functions
	//

	//-----------------------------------------------------------------------//
	// Function: AcceptOffer
	//
	// Accepts the current offer for the given player index.
	//
	static void AcceptOffer(wxInt32 playerIndex);

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: CompareOffer
	//
	// Compares this offer against the given offer to see if they are the same.
	//
	// Parameters:
	//     other - The offer to compare to.
	//
	// Returns:
	//     A bool that is true if the offers are the same and false if not.
	//
	bool CompareOffer(const TradeOffer& other) const;

	//-----------------------------------------------------------------------//
	// Function: MatchOffer
	//
	// Matches this offer and makes the offer if the given player is able to
	// do so.
	//
	// Parameters:
	//     playerIndex - The player attempting to match the offer.
	//
	void MatchOffer(wxInt32 playerIndex);

	//-----------------------------------------------------------------------//
	// Function: FillOffer
	//
	// Fills the given <DataObject> with the trade offer.
	//
	// Parameters:
	//     object - [out] The outgoing <DataObject> filled with this trade 
	//             offer.
	//
	void FillOffer(DataObject& object) const;

	//-----------------------------------------------------------------------//
	// Function: TestClick
	//
	// Tests the offer to see if given mouse coords are over the offer, and if
	// so, if the given player is able to make the offer.
	//
	// Parameters:
	//     pos - The position of the mouse.
	//     playerIndex - The player attempting to make the offer.
	//     canTrade - [out] A bool that is true if the player can make the offer
	//                and false if not.
	//
	// Returns:
	//     The index of the offer if the mouse is over the offer or -1 if not.
	//
	wxInt32 TestClick(const wxPoint &pos, wxInt32 playerIndex, 
		bool& canTrade) const;
	
	//-----------------------------------------------------------------------//
	// Function: SetBackgroundColour
	//
	// Sets the background color of all windows in the trade offer.
	//
	// Parameters:
	//     color - The new background color.
	//
	void SetBackgroundColour(wxColour color);

	//-----------------------------------------------------------------------//
	// Function: GetSizerRect
	//
	// Returns the rectangle contained by the TradeOffer sizer.
	//
	// Returns:
	//     The TradeOffer sizer rectangle.
	//
	wxRect GetSizerRect() const;
	
	//-----------------------------------------------------------------------//
	// Section: Private
	//
public:
	typedef std::map<HashString, wxInt32> ResourceMap;

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: SetOffer
	//
	// Sets the TradeOffer to the given values.
	//
	// Parameters:
	//     left - The left side of the trade offer.
	//     right - The right side of the trade offer.
	//
	void SetOffer(const ResourceMap& left, const ResourceMap& right);
	
	//-----------------------------------------------------------------------//
	// Function: ClearOffer
	//
	// Clears the trade offer to the given state.
	//
	// Parameters:
	//     state - The state to clear the TradeOffer to.
	//
	void ClearOffer(wxTradeOfferBox::State state);

	//-----------------------------------------------------------------------//
	// Group: Game Event Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnTrade
	//
	// Handles a trade being executed between the current player and a trade
	// partner.
	//
	// Parameters:
	//     partner - The trade partner.
	//
	void OnTrade(wxInt32 partner);
	
	//-----------------------------------------------------------------------//
	// Function: OnCurrentOffer
	//
	// Handles a new current offer from the trader.
	//
	// Parameters:
	//     object - The <DataObject> containing the trade offer information.
	//
	void OnCurrentOffer(const DataObject& object);
	
	//-----------------------------------------------------------------------//
	// Function: OnPlayerOffer
	//
	// Handles a new player offer from a tradee.
	//
	// Parameters:
	//     object - The <DataObject> containing the trade offer information.
	//
	void OnPlayerOffer(const DataObject& object);
	
	//-----------------------------------------------------------------------//
	// Function: OnRejectOffer
	//
	// Handles a player rejecting the current trade offer.
	//
	// Parameters:
	//     player - The index of the player rejecting the offer.
	//
	void OnRejectOffer(wxInt32 player);
	
	//-----------------------------------------------------------------------//
	// Function: OnClearOffer
	//
	// Handles a player backing out of an accepted trade.
	//
	// Parameters:
	//     player - The index of the player backing out of the trade.
	//
	void OnWontTrade(wxInt32 player);
	
	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Int: mPlayerIndex
	//
	// The index of the player this TradeOffer describes.
	//
	wxInt32 mPlayerIndex;

	//-----------------------------------------------------------------------//
	// Bool: mTrader
	//
	// True if the player at this computer is the trader, false if not.
	//
	bool mTrader;

	//-----------------------------------------------------------------------//
	// Pointer: mBox
	//
	// The box around the image strips.
	//
	wxTradeOfferBox* mBox;

	//-----------------------------------------------------------------------//
	// Pointer: mLeft
	//
	// The left side image strip.
	//
	wxImageStrip* mLeft;

	//-----------------------------------------------------------------------//
	// Pointer: mRight
	//
	// The right side image strip.
	//
	wxImageStrip* mRight;

	//-----------------------------------------------------------------------//
	// Pointer: mText
	//
	// The center text.
	//
	wxBaseStaticText* mText;

	//-----------------------------------------------------------------------//
	// Pointer: mSizer
	//
	// The containing sizer.
	//
	wxSizer* mSizer;
};

//---------------------------- PROTOTYPES       -----------------------------//

