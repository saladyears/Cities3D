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
#include "GameSetupPane.h"
#include "PlayerDropListCtrl.h"
#include "PlayerGameListCtrl.h"
#include "MapTextCtrl.h"
#include "FittedStaticText.h"
#include "BaseButton.h"
#include "AddPlayerDialog.h"
#include "SelectMapDialog.h"
#include "BaseModalSizer.h"
#include "GameEngine.h"
#include "Controller.h"
#include "Game.h"
#include "DataObject.h"
#include "SkinManager.h"
#include "BaseMessageBox.h"
#include "PlayerDatabase.h"
#include "OptionsDialog.h"
#include "IRuleEngine.h"
#include "Map.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	enum
	{
		ID_ADD_PLAYER_BUTTON		,
		ID_DELETE_PLAYER_BUTTON		,
		ID_SELECT_MAP_BUTTON		,
		ID_GAME_OPTIONS_BUTTON		,
	};

	//controls
	wxBaseButton *spDeleteButtonGame;
	wxBaseButton *spVariantsButton;
	wxPlayerDropListCtrl *spPlayerList;
	wxMapTextCtrl *spMapTextCtrl;
	wxBaseButton *spOKGame;

	void CreatePlayers(wxBoxSizer *sizer, wxWindow* parent)
	{
		//left side (player list, and add/edit buttons)
		wxBoxSizer *horzSizer1 = new wxBoxSizer(wxHORIZONTAL);
		spPlayerList = new wxPlayerDropListCtrl(parent, ID_PLAYER_LIST, 
			wxDefaultPosition, wxSize(125, 200));
		horzSizer1->Add(spPlayerList, 1, wxEXPAND);

		//map selection and variants buttons
		wxBoxSizer *horzSizer2 = new wxBoxSizer(wxHORIZONTAL);
		wxBaseButton *button = new wxBaseButton(parent, 
			ID_ADD_PLAYER_BUTTON, stAdd);
		horzSizer2->Add(button, 0, wxALIGN_LEFT | wxTOP, 2);
		horzSizer2->Add(5, 0, 1, wxEXPAND);
		spDeleteButtonGame = new wxBaseButton(parent, ID_DELETE_PLAYER_BUTTON, 
			stDelete);
		spDeleteButtonGame->Enable(false);
		horzSizer2->Add(spDeleteButtonGame, 0, wxALIGN_RIGHT | wxTOP, 2);

		wxBoxSizer *vertSizer1 = new wxBoxSizer(wxVERTICAL);
		vertSizer1->Add(horzSizer1, 0, wxEXPAND | wxALL, 1);
		vertSizer1->Add(horzSizer2, 0, wxEXPAND | wxALL, 1);

		//right side (game list, message text)
		wxBoxSizer *horzSizer3 = new wxBoxSizer(wxHORIZONTAL);
		horzSizer3->Add(new wxPlayerGameListCtrl(parent, -1, 
			wxDefaultPosition, wxSize(125, 200)), 1, wxEXPAND);

		wxBoxSizer *horzSizer4 = new wxBoxSizer(wxHORIZONTAL);
		wxStaticText *text = new wxFittedStaticText(parent, -1, 
			stDragAndDropNames, shInfoText, shInfoFont, wxSize(155, 22), 
			wxST_NO_AUTORESIZE | wxALIGN_LEFT);
		horzSizer4->Add(text, 1, wxEXPAND | wxTOP, 2);

		wxBoxSizer *vertSizer2 = new wxBoxSizer(wxVERTICAL);
		vertSizer2->Add(horzSizer3, 0, wxEXPAND | wxALL, 1);
		vertSizer2->Add(horzSizer4, 0, wxEXPAND | wxALL, 1);

		wxBoxSizer *interSizer1 = new wxBoxSizer(wxHORIZONTAL);
		interSizer1->Add(vertSizer1, 1, wxEXPAND);
		interSizer1->Add(5, 0, 0, wxEXPAND);
		interSizer1->Add(vertSizer2, 1, wxEXPAND);
		sizer->Add(interSizer1, 1, wxEXPAND);
	}

	void CreateMap(wxBoxSizer *sizer, wxWindow* parent)
	{
		//set up the map section
		wxBoxSizer *interSizer1 = new wxBoxSizer(wxHORIZONTAL);
		wxBoxSizer *vertSizer1 = new wxBoxSizer(wxVERTICAL);

		//map information
		wxBoxSizer *horzSizer1 = new wxBoxSizer(wxHORIZONTAL);
		spMapTextCtrl = new wxMapTextCtrl(parent, -1, wxDefaultPosition, 
			wxSize(125, 200));
		horzSizer1->Add(spMapTextCtrl, 1, wxEXPAND);

		//map selection and variants buttons
		wxBoxSizer *horzSizer2 = new wxBoxSizer(wxHORIZONTAL);
		wxBaseButton *button = new wxBaseButton(parent, 
			ID_SELECT_MAP_BUTTON, stSelectMap);
		horzSizer2->Add(button, 0, wxALIGN_LEFT | wxTOP, 2);
		horzSizer2->Add(5, 0, 1, wxEXPAND);
		spVariantsButton = new wxBaseButton(parent, 
			ID_GAME_OPTIONS_BUTTON, stOptions);
		spVariantsButton->Enable(false);
		horzSizer2->Add(spVariantsButton, 0, wxALIGN_RIGHT | wxTOP, 2);

		vertSizer1->Add(horzSizer1, 0, wxEXPAND | wxALL, 1);
		vertSizer1->Add(horzSizer2, 0, wxEXPAND | wxALL, 1);

		interSizer1->Add(vertSizer1, 1, wxEXPAND);
		sizer->Add(interSizer1, 1, wxEXPAND);
	}

	void CreateButtons(wxBoxSizer *sizer, wxWindow* parent)
	{
		//add OK/Cancel buttons
		spOKGame = new wxBaseButton(parent, wxID_OK, stOK);
		
		sizer->Add(spOKGame, 0, wxALL, 3);
	}

	void CreatePaneGameSetup(wxWindow* window)
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		wxBoxSizer *playerSizer = new wxBoxSizer(wxVERTICAL);
		wxBoxSizer *mapSizer = new wxBoxSizer(wxVERTICAL);

		// Create the sections.
		CreatePlayers(playerSizer, window);
		CreateMap(mapSizer, window);

		// Add the top level sizers.
		sizer->Add(playerSizer, 8, wxALIGN_LEFT | wxALIGN_TOP | wxEXPAND | 
			wxALL, 3);
		sizer->Add(mapSizer, 5, wxALIGN_RIGHT | wxALIGN_TOP | wxEXPAND | 
			wxALL, 3);

		// Then tack on the buttons.
		wxBoxSizer *buttonSizer = new wxBoxSizer(wxVERTICAL);
		CreateButtons(buttonSizer, window);
		sizer->Add(buttonSizer, 0, wxEXPAND | wxALIGN_RIGHT | wxALL, 3);

		window->SetSizer(sizer);
		sizer->SetSizeHints(window);
	}
}

//---------------------------- PUBLIC           -----------------------------//
wxGameSetupPane::wxGameSetupPane(wxWindow *parent)
: wxPanel(parent, wxID_ANY, wxPoint(parent->GetClientSize().x, 
	parent->GetClientSize().y))
, PaneMixin(this)
{
	CreatePaneGameSetup(this);

	//OK button is disabled until game state is ready
	spOKGame->Enable(false);

	Controller::get().AddReceiver(shEventPreGame, 
		&wxGameSetupPane::OnPreGame, this);
}

wxGameSetupPane::~wxGameSetupPane()
{
	Controller::get().RemoveReceiver(shEventPreGame, 
		&wxGameSetupPane::OnPreGame, this);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxGameSetupPane, wxPanel)
	EVT_BUTTON(ID_ADD_PLAYER_BUTTON, wxGameSetupPane::OnAddPlayer)
	EVT_BUTTON(ID_DELETE_PLAYER_BUTTON, wxGameSetupPane::OnDeletePlayer)
	EVT_BUTTON(ID_SELECT_MAP_BUTTON, wxGameSetupPane::OnSelectMap)
	EVT_BUTTON(ID_GAME_OPTIONS_BUTTON, wxGameSetupPane::OnGameOptions)
	EVT_BUTTON(wxID_OK, wxGameSetupPane::OnOK)
	EVT_LIST_ITEM_SELECTED(ID_PLAYER_LIST, 
	wxGameSetupPane::OnPlayerSelected)
	EVT_LIST_ITEM_DESELECTED(ID_PLAYER_LIST, 
	wxGameSetupPane::OnPlayerSelected)
END_EVENT_TABLE()

void wxGameSetupPane::OnPreGame(const GamePtr &game)
{
	wxInt32 players = game->numPlayers();

	//the ok button can only be enabled if there are players in the game
	//and they have selected a map
	spOKGame->Enable((0 < players) && (-1 != MAPINDEX));

	//they should not be allowed to add more than the maximum number of players
	spPlayerList->Enable(MAX_PLAYERS > players);

	wxListEvent event;
	OnPlayerSelected(event);
}

void wxGameSetupPane::OnAddPlayer(wxCommandEvent &)
{
	wxAddPlayerDialog dlg(this);
	if(wxID_OK == dlg.ShowModal())
	{
		//add the player to the current player list
		spPlayerList->AddPlayer(dlg.GetPlayer().Name());
	}
}

void wxGameSetupPane::OnDeletePlayer(wxCommandEvent &)
{
	wxInt32 index = spPlayerList->GetSelectedItem();

	if(-1 != index)
	{
		//sanity check
		wxBaseMessageBox dialog(this, stQuestion, 
			wxString::Format(stAreYouSureYouWantToDelete.c_str(), 
			spPlayerList->GetItemText(index, swPlayer).c_str()), 
			MessageBoxYesNo);

		if(wxNO == dialog.ShowModal())
		{
			return;
		}

		wxInt32 playerIndex = spPlayerList->GetItemData(index);

		//remove the player from the list and the player database
		PlayerDatabase::get().DeletePlayer(playerIndex);
		spPlayerList->DeletePlayer(index);
	}
}

void wxGameSetupPane::OnSelectMap(wxCommandEvent &)
{
	wxSelectMapDialog dlg(this, MAPINDEX);
	
	if(wxID_OK == dlg.ShowModal())
	{
		spVariantsButton->Enable(true);

		wxInt32 index = dlg.GetIndex();
		wxASSERT(-1 != index);

		//load the requested map
		RULE.Execute(shRuleLoadMap, DataObject(index));
	}
}

void wxGameSetupPane::OnGameOptions(wxCommandEvent &)
{
	wxOptionsDialog dialog(this);
	dialog.ShowModal();
}

void wxGameSetupPane::OnPlayerSelected(wxListEvent &)
{
	wxInt32 index = spPlayerList->GetSelectedItem();

	spDeleteButtonGame->Enable(-1 != index);
}

void wxGameSetupPane::OnOK(wxCommandEvent&)
{
	// Start the game.  The first thing to do is randomize the players.
	RULE.Execute(shRuleRandomizePlayers, DataObject(-1));

	const Map::StringPairArray& rulesets = MAP.rulesets();
	Controller::get().Transmit(shEventLoadRulesets, rulesets);
	Controller::get().Transmit(shEventStartGame, true);

	//start the game
	RULE.Execute(shRule0, DataObject());
}
