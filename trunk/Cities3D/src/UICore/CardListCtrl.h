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
#include <boost/function.hpp>

//---------------------------- USER INCLUDES    -----------------------------//
#include "BaseListCtrl.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class DataObject;

class Game;
typedef boost::shared_ptr<Game> GamePtr;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxCardListCtrl
//
// Shows a list of all playable and purchased card in the player's hand.  Cards
// that are not playable are grayed out.
//
// Derived From:
//     <wxBaseListCtrl>
//
// Project:
//     <Cities3D>
//
// Include:
//     CardListCtrl.h
//
class UI_EXPORT_SPEC wxCardListCtrl : public wxBaseListCtrl
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
    struct Card
    {
        Card(const wxString& n, const HashString& i, wxColour c, bool b)
            : name(n), internalName(i), color(c), playable(b)
        {}

        wxString name;
        HashString internalName;
        wxColour color;
        bool playable;
    };

    typedef std::vector<Card> CardArray;

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: GetCardName
	//
	// Returns the untranslated name of the card at the given index.
	//
	// Parameters:
	//     index - The index of the card to look up.
	//
	// Returns:
	//     The untranslated name of the card.
	//
	const HashString& GetCardName(size_t index) const
	{
		wxASSERT(index < mCards.size());
		return mCards[index].internalName;
	}

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	typedef boost::function3<void, const GamePtr&, wxInt32, CardArray&>
		PopulateFunc;

	//-----------------------------------------------------------------------//
	// Constructor: wxCardListCtrl
	// 
	// The wxCardListCtrl constructor.  Creates the columns for the list 
	// control.
	//
	// Parameters:
	//     parent - The parent window.
	//     id - The message handling ID.
	//     game - The current <Game>.
	//     func - The control population function.
	//     pos - The list control position in window coordinates.
	//     size - The list control size.
	//
	wxCardListCtrl(wxWindow* parent, wxWindowID id, boost::weak_ptr<Game> game,
		PopulateFunc func,
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxCardListCtrl
	//
	// The wxCardListCtrl destructor.
	//
	~wxCardListCtrl();

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: UpdatePlayer
	//
	// Sets our current player index.
	//
	// Parameters:
	//     game - The current <Game>.
	//
	void UpdatePlayer(const GamePtr& game);

	//-----------------------------------------------------------------------//
	// Function: PopulateList
	//
	// Populates the list with the available cards.
	//
	void PopulateList();

    //-----------------------------------------------------------------------//
	// Group: Game Event Handling
	//

	//-----------------------------------------------------------------------//
	// Function: OnPlayerUI
	//
	// Resets the card window.
	//
	void OnPlayerUI(const DataObject &object);

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	PopulateFunc mPopulateFunc;

	//-----------------------------------------------------------------------//
	// Int: mPlayer
	//
	// The player we are showing cards for.
	//
	wxInt32 mPlayer;

	//-----------------------------------------------------------------------//
	// Object: mCards
	//
	// The cards in the list.
	//
    CardArray mCards;
};

//---------------------------- PROTOTYPES       -----------------------------//

