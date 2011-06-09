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
#include "CardsDialog.h"
#include "ISizer.h"
#include "CardListCtrl.h"
#include "BaseButton.h"
#include "BaseStaticBox.h"
#include "FittedStaticText.h"
#include "Controller.h"
#include "ImageEngine.h"
#include "DataObject.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	enum
	{
		ID_PLAY_CARD_BUTTON			= 0,
		ID_CARD_LIST				,
	};

	wxCardListCtrl *spCardList = NULL;
	wxBaseButton* spButton = NULL;
	wxStaticBitmap *spBitmap = NULL;
	wxBaseStaticBox *spBox = NULL;
	wxFittedStaticText *spText = NULL;
	wxImage sBlankImage;

	class Sizer_CardsDialog : public ISizer
	{
	public:
		Sizer_CardsDialog(wxWindow *window, boost::weak_ptr<Game> game, 
			boost::shared_ptr<wxCardsDialog::CardListCreator> creator,
			const wxString& text)
			: ISizer(window)
			, mGame(game)
			, mCreator(creator)
			, mText(text)
		{}

		void CreateLayout(wxBoxSizer *sizer)
		{
			wxBoxSizer *horz = new wxBoxSizer(wxHORIZONTAL);
			
			wxBoxSizer* vert1 = new wxBoxSizer(wxVERTICAL);
			spCardList = mCreator->CreateCardList(mpWindow, ID_CARD_LIST,
				mGame, wxDefaultPosition, wxSize(150, 200));

			spButton = new wxBaseButton(mpWindow, ID_PLAY_CARD_BUTTON, mText);
			spButton->Disable();

			wxBoxSizer* horz1 = new wxBoxSizer(wxHORIZONTAL);
			horz1->AddStretchSpacer();
			horz1->Add(spButton, 0);
			horz1->AddStretchSpacer();

			vert1->Add(spCardList, 1, wxEXPAND | wxRIGHT | wxLEFT | wxTOP, 4);
			vert1->Add(horz1, 0, wxEXPAND | wxALL, 2);

			// Get the minimum size needed for our bitmap window.
			wxASSERT(0 < spCardList->GetItemCount());
			const HashString& card = spCardList->GetCardName(0);

			// Grab the image at its normal size.
			wxImage image = IMAGE.GetImage(shPlayerCards, card);
			wxSize size(image.GetWidth(), image.GetHeight());
			sBlankImage = wxImage(image.GetWidth(), image.GetHeight());

			spBitmap = new wxStaticBitmap(mpWindow, 0, wxBitmap(sBlankImage), 
				wxDefaultPosition, size);

			wxBoxSizer* vert2 = new wxBoxSizer(wxVERTICAL);
			wxBoxSizer* horzBox = new wxBoxSizer(wxHORIZONTAL);
			
			spBox = new wxBaseStaticBox(mpWindow, 0, swEmpty, wxDefaultPosition, 
				wxDefaultSize, shDefaultText, shCardHeaderFont);
			wxStaticBoxSizer* boxSizer = new wxStaticBoxSizer(spBox, wxVERTICAL);
			spText = new wxFittedStaticText(mpWindow, 0, swEmpty, shDefaultText, 
				shInfoFont, wxSize(size.GetWidth(), 0), wxST_NO_AUTORESIZE | 
				wxALIGN_CENTER);
			boxSizer->Add(spBitmap, 0, wxEXPAND | wxSHAPED | wxALL, 2);
			boxSizer->Add(spText, 0, wxEXPAND | wxALL, 2);
			horzBox->Add(boxSizer, 1, wxEXPAND | wxRIGHT | wxTOP | wxBOTTOM, 4);

			vert2->Add(horzBox, 0, wxEXPAND);
			vert2->AddStretchSpacer();

			horz->Add(vert1, 1, wxEXPAND);
			horz->Add(vert2, 0, wxEXPAND);

			sizer->Add(horz, 1, wxEXPAND);
		}

	private:
		boost::weak_ptr<Game> mGame;
		boost::shared_ptr<wxCardsDialog::CardListCreator> mCreator;
		wxString mText;
	};
}

//---------------------------- PUBLIC           -----------------------------//
#ifdef __WIN32__
	//disables the 'this' : used in base member initializer list warning
	#pragma warning(disable:4355)
#endif
wxCardsDialog::wxCardsDialog(wxWindow *parent, boost::weak_ptr<Game> game,
	const wxString& buttonText, const wxString& titleText, 
	boost::shared_ptr<CardListCreator> creator, const HashString& rule,
	wxInt32 index)
: wxBaseCloseableDialog(parent, ISizerPtr(new Sizer_CardsDialog(this, game, 
	creator, buttonText)), titleText, swYourCards)
, mRule(rule)
, mIndex(index)
{
	Controller::get().AddReceiver(shEventImageUpdate, 
		&wxCardsDialog::OnImageUpdate, this);

	// Select the first item as a nicety.
	spCardList->SetSelectedItem(0);
}

wxCardsDialog::~wxCardsDialog()
{
	Controller::get().RemoveReceiver(shEventImageUpdate, 
		&wxCardsDialog::OnImageUpdate, this);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxCardsDialog, wxBaseCloseableDialog)
	EVT_LIST_ITEM_SELECTED(ID_CARD_LIST, wxCardsDialog::OnCardSelected)
	EVT_LIST_ITEM_DESELECTED(ID_CARD_LIST, wxCardsDialog::OnCardSelected)
	EVT_BUTTON(ID_PLAY_CARD_BUTTON, wxCardsDialog::OnPlayCardButton)
END_EVENT_TABLE()

void wxCardsDialog::ResetImage(const HashString& card)
{
	wxString displayText;
	wxString description;

	wxImage image = sBlankImage;

	if(false == card.empty())
	{
		wxInt32 index = spCardList->GetSelectedItem();

		displayText = spCardList->GetItemText(index, swCard);

		DataObject input(card), output;
		RULE.Decide(shLogicCardText, input, output);
		description = output.read<wxString>();
        description += sw10Spaces;
		
		image = IMAGE.GetImage(shPlayerCards, card);

		sBlankImage = wxImage(image.GetWidth(), image.GetHeight());
	}

	spBitmap->SetSize(wxSize(image.GetWidth(), image.GetHeight()));
	spBitmap->SetBitmap(wxBitmap(image));

	spBox->Freeze();
	spBox->SetLabel(displayText);
	spBox->Thaw();

	// Here we have to do some chicanery.
	wxSizer* sizer = spText->GetContainingSizer();
	sizer->Detach(spText);
	spText->Destroy();
	spText = new wxFittedStaticText(this, 0, description, 
		shDefaultText, shInfoFont, 
		wxSize(spBitmap->GetSize().GetWidth(), 0), wxST_NO_AUTORESIZE | 
		wxALIGN_CENTER);
	sizer->Add(spText, 0, wxEXPAND | wxALL, 2);
	
	sizer = GetSizer();
	wxSize minSize = sizer->GetMinSize();
	wxSize size = GetClientSize();
	if(size.y < minSize.y)
	{
		wxSize min = GetMinSize();
		min.y += (minSize.y - size.y);
		SetMinSize(min);

		SetClientSize(wxSize(size.x, minSize.y));
	}

	sizer->Layout();
}

void wxCardsDialog::Close()
{
	Controller::get().Transmit(shEventCards, DataObject(GamePtr(), false));
}

void wxCardsDialog::OnImageUpdate(bool)
{
	// If they update the images, we need to regrab.
	ResetImage(mCurrentCard);
}

void wxCardsDialog::OnCardSelected(wxListEvent &)
{
	wxInt32 index = spCardList->GetSelectedItem();
	HashString card;

	bool enable = false;
	if(-1 != index)
	{
		card = spCardList->GetCardName(index);

		// See if the card can be played.
		if(TRUE == spCardList->GetItemData(index))
		{
			// They have to be the current player as well.
			DataObject input(mIndex), output;
			RULE.Decide(shLogicIsThisPlayer, input, output);
			enable = output.read<bool>();
		}
	}

	spButton->Enable(enable);

	if(mCurrentCard != card)
	{
		ResetImage(card);

		mCurrentCard = card;
	}
}

void wxCardsDialog::OnPlayCardButton(wxCommandEvent &)
{
	// Close things down and play the card.
	Controller::get().Transmit(shEventCards, DataObject(GamePtr(), false));

	wxASSERT(false == mCurrentCard.empty());
	RULE.Execute(mRule, DataObject(mCurrentCard, mIndex));

	OnPlayCard();
}
