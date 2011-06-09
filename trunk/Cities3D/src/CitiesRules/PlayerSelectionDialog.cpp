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
#include "PlayerSelectionDialog.h"
#include "ISizer.h"
#include "BaseButton.h"
#include "BaseListCtrl.h"
#include "GameEngine.h"
#include "IRuleEngine.h"
#include "DataObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	enum
	{
		ID_OK_PLAYER_BUTTON		= 0,
		ID_PLAYER_LIST			,
	};

	wxString sFields[] = {swPlayer};
	wxBaseListCtrl* spPlayerList = NULL;
	wxBaseButton* spButton = NULL;

	class Sizer_PlayerSelection : public ISizer
	{
	public:
		Sizer_PlayerSelection(wxWindow *window, boost::weak_ptr<Game> game,
			wxPlayerSelectionDialog::PopulateListFunc func)	
			: ISizer(window)
			, mGame(game)
			, mPopulateFunc(func)
		{}

		void CreateLayout(wxBoxSizer *sizer)
		{
			wxBoxSizer *vert1 = new wxBoxSizer(wxVERTICAL);
			wxBoxSizer* horz1 = new wxBoxSizer(wxHORIZONTAL);

			spPlayerList = new wxBaseListCtrl(mpWindow, ID_PLAYER_LIST, 
				sFields, _countof(sFields), shPlayerSelectionMainFont, 
				shPlayerSelectionSubFont, wxDefaultPosition, wxSize(350, 200));

			wxASSERT(mPopulateFunc);
			mPopulateFunc(spPlayerList, mGame);

			spButton = new wxBaseButton(mpWindow, ID_OK_PLAYER_BUTTON, stSelect);
			spButton->Disable();

			horz1->AddStretchSpacer();
			horz1->Add(spButton);
			horz1->AddStretchSpacer();

			vert1->Add(spPlayerList, 1, wxEXPAND | wxALL, 2);
			vert1->Add(horz1, 0, wxEXPAND | wxALL, 2);

			sizer->Add(vert1, 1, wxEXPAND);
		}

	private:
		boost::weak_ptr<Game> mGame;
		wxPlayerSelectionDialog::PopulateListFunc mPopulateFunc;
	};
}

//---------------------------- PUBLIC           -----------------------------//
#ifdef __WIN32__
	//disables the 'this' : used in base member initializer list warning
	#pragma warning(disable:4355)
#endif
wxPlayerSelectionDialog::wxPlayerSelectionDialog(wxWindow *parent,
	boost::weak_ptr<Game> game, const HashString& rule, const wxString& text, 
	PopulateListFunc func)
: wxBaseGameDialog(parent, ISizerPtr(new Sizer_PlayerSelection(this, game, 
	func)), text, swSelectPlayer)
, mRule(rule)
{

}

wxPlayerSelectionDialog::~wxPlayerSelectionDialog()
{
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxPlayerSelectionDialog, wxBaseGameDialog)
	EVT_BUTTON(ID_OK_PLAYER_BUTTON, wxPlayerSelectionDialog::OnButton)
	EVT_LIST_ITEM_SELECTED(ID_PLAYER_LIST, 
	wxPlayerSelectionDialog::OnPlayerSelected)
	EVT_LIST_ITEM_DESELECTED(ID_PLAYER_LIST, 
	wxPlayerSelectionDialog::OnPlayerSelected)
END_EVENT_TABLE()

void wxPlayerSelectionDialog::OnButton(wxCommandEvent &)
{
	wxInt32 index = spPlayerList->GetSelectedItem();
	wxASSERT(-1 != index);

	// Get the index of the player being selected.
	wxInt32 playerIndex = spPlayerList->GetItemData(index);

	RULE.Execute(mRule, DataObject(playerIndex));

	Destroy();
}

void wxPlayerSelectionDialog::OnPlayerSelected(wxListEvent&)
{
	wxInt32 index = spPlayerList->GetSelectedItem();
	
	bool enable = (-1 != index);
	
	spButton->Enable(enable);
}
