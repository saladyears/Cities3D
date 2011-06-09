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
#include "BaseCloseableDialog.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//
class DataObject;
class wxListEvent;
class wxCardListCtrl;
class Game;

//---------------------------------------------------------------------------//
// Class: wxCardsDialog
// 
// Handles player cards.
//
// Derived From:
//     <wxBaseCloseableDialog>
//
// Project:
//     <Cities3D>
//
// Include:
//     CardsDialog.h
//
class UI_EXPORT_SPEC wxCardsDialog : public wxBaseCloseableDialog
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	struct CardListCreator
	{
		virtual wxCardListCtrl* CreateCardList(wxWindow* parent, wxWindowID id,
			boost::weak_ptr<Game> game, const wxPoint& pos, 
			const wxSize& size)=0;
		virtual ~CardListCreator() { }
	};

	//-----------------------------------------------------------------------//
	// Constructor: wxCardsDialog
	//
	// The wxCardsDialog constructor.
	//
	// Parameters:
	//     parent - The parent window.
	//     game - The current <Game>.
	//     buttonText - The text for the button.
	//     titleText - The text for the dialog title bar.
	//     creator - The <wxCardListCtrl> creator this dialog uses.
	//     rule - The rule to call after the card is selected.
	//
	wxCardsDialog(wxWindow *parent, boost::weak_ptr<Game> game, 
		const wxString& buttonText, const wxString& titleText,
		boost::shared_ptr<CardListCreator> creator, 
		const HashString& rule = shRulePlayCard, wxInt32 index = -1);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxCardsDialog
	//
	// The wxCardsDialog destructor.
	//
	virtual ~wxCardsDialog()=0;

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Macro: DECLARE_EVENT_TABLE
	//
	// The wxWidgets event table declaration.
	//
	DECLARE_EVENT_TABLE()

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: ResetImage
	//
	// Resets the image and text on the right side of the window.
	//
	// Parameters:
	//     card - The card to display (if any).
	//
	void ResetImage(const HashString& card);

	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Close
	//
	// Closes the window.
	//
	virtual void Close();

	//-----------------------------------------------------------------------//
	// Function: OnPlayCard
	//
	// Allows manual override when a card is played.
	//
	virtual void OnPlayCard()
	{
	}

	//-----------------------------------------------------------------------//
	// Group: Game Event Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnImageUpdate
	//
	// Handles the user changing the images used in the game.
	//
	void OnImageUpdate(bool);

	//-----------------------------------------------------------------------//
	// Group: Event Message Handling
	//

	//-----------------------------------------------------------------------//
	// Function: OnCardSelected
	//
	// Handles a selection in the <wxCardListCtrl>.
	//
	// Parameters
	//     event - The list event.
	//
	void OnCardSelected(wxListEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnPlayCardButton
	//
	// Handles a click on the play card button, which plays the card.
	//
	// Parameters:
	//     event - The button click event.
	//
	void OnPlayCardButton(wxCommandEvent &event);

	HashString mRule;
	wxInt32 mIndex;

	HashString mCurrentCard;
};

//---------------------------- PROTOTYPES       -----------------------------//

