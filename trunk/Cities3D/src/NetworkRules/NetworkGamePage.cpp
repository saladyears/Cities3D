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
#include "NetworkGamePage.h"
#include "BaseButton.h"
#include "FittedStaticText.h"
#include "SpectatorDropListCtrl.h"
#include "NetworkPlayerGameListCtrl.h"
#include "MapTextCtrl.h"
#include "ConfigDatabase.h"
#include "GameEngine.h"
#include "INetworkEngine.h"
#include "SelectMapDialog.h"
#include "Controller.h"
#include "DataObject.h"
#include "ChatWindow.h"
#include "NetworkPlayers.h"
#include "Game.h"
#include "OptionsDialog.h"
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
		ID_SELECT_MAP_BUTTON		,
		ID_GAME_OPTIONS_BUTTON		,
		ID_ACTIONS_BUTTON			,
		ID_NOTREADY_BUTTON			,
	};

	wxNetworkPlayerGameListCtrl *spPlayerListCtrl;
	wxSpectatorDropListCtrl *spSpectatorList;
	wxChatWindow *spChatWindow;
	wxMapTextCtrl *spMapTextCtrl;
	wxBaseButton *spVariantsButton;
	wxBaseButton *spActionsButton;
	wxBaseButton *spNotReadyButton = NULL;

	bool sIsHost = false;
}

//---------------------------- PUBLIC           -----------------------------//
wxNetworkGamePage::wxNetworkGamePage(wxWindow* parent)
: wxPanel(parent, wxID_ANY, wxPoint(parent->GetClientSize().x, 
	parent->GetClientSize().y))
{
	this->SetBackgroundColour(parent->GetBackgroundColour());

	sIsHost = NET.Players().IsHost();

	wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer *horz1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *horz2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *horz3 = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer *innerVert1 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *innerVert2 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *innerVert3 = new wxBoxSizer(wxVERTICAL);

	//player lists
	spPlayerListCtrl = new wxNetworkPlayerGameListCtrl(this, -1, 
		wxDefaultPosition, wxSize(150, 150));
	spSpectatorList = new wxSpectatorDropListCtrl(this, -1, wxDefaultPosition, 
		wxSize(150, 150));

	wxString str = sIsHost ? stStartGame : stReady;
	spActionsButton = new wxBaseButton(this, ID_ACTIONS_BUTTON, str);
	
	spActionsButton->Enable(false);


	innerVert1->Add(spPlayerListCtrl, 1, wxEXPAND);
	innerVert2->Add(spSpectatorList, 1, wxEXPAND);
	innerVert3->Add(spActionsButton, 0, wxEXPAND | wxALL, 6);

	if(false == sIsHost)
	{
		spNotReadyButton = new wxBaseButton(this, ID_NOTREADY_BUTTON, stNotReady);
		spNotReadyButton->Enable(false);
		innerVert3->Add(spNotReadyButton, 0, wxEXPAND | wxALL, 6);
	}

	horz1->Add(innerVert1, 1, wxEXPAND | wxALL, 3);
	horz1->Add(innerVert2, 1, wxEXPAND | wxALL, 3);
	horz1->Add(innerVert3, 0, wxEXPAND);

	horz2->Add(new wxFittedStaticText(this, -1, 
	    stDragAndDropNames, shInfoText, shInfoFont, wxSize(425, 22)), 0, 
        wxEXPAND | wxLEFT | wxBOTTOM, 3);

	wxBoxSizer *innerVert10 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *innerVert11 = new wxBoxSizer(wxVERTICAL);

	//chat window
	spChatWindow = new wxChatWindow(this, -1, true, wxDefaultPosition, 
		wxSize(300, 150));
	innerVert10->Add(spChatWindow, 1, wxEXPAND);

	//map selection
	spMapTextCtrl = new wxMapTextCtrl(this, -1, wxDefaultPosition, 
		wxSize(125, 150));
	innerVert11->Add(spMapTextCtrl, 1, wxEXPAND);

	if(true == sIsHost)
	{
		wxBoxSizer *innerHorz1 = new wxBoxSizer(wxHORIZONTAL);
		wxBaseButton *button = new wxBaseButton(this, ID_SELECT_MAP_BUTTON, 
			stSelectMap);
		innerHorz1->Add(button, 0, wxALIGN_LEFT);
		innerHorz1->Add(5, 0, 1, wxEXPAND);
		spVariantsButton = new wxBaseButton(this, ID_GAME_OPTIONS_BUTTON, 
			stOptions);
		spVariantsButton->Enable(false);
		innerHorz1->Add(spVariantsButton, 0, wxALIGN_RIGHT);

		innerVert11->Add(innerHorz1, 0, wxEXPAND | wxTOP, 5);
	}

	horz3->Add(innerVert10, 3, wxEXPAND | wxALL, 3);
	horz3->Add(innerVert11, 2, wxEXPAND | wxALL, 3);

	pSizer->Add(horz1, 3, wxEXPAND);
	pSizer->Add(horz2, 0, wxEXPAND);
	pSizer->Add(horz3, 4, wxEXPAND | wxRIGHT, 3);

	SetSizer(pSizer);

	Controller::get().AddReceiver(shEventPreGame,
		&wxNetworkGamePage::OnPreGame, this);
	Controller::get().AddReceiver(shEventResetReady,
		&wxNetworkGamePage::OnResetReady, this);
}

wxNetworkGamePage::~wxNetworkGamePage()
{
	Controller::get().RemoveReceiver(shEventPreGame,
		&wxNetworkGamePage::OnPreGame, this);
	Controller::get().RemoveReceiver(shEventResetReady,
		&wxNetworkGamePage::OnResetReady, this);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxNetworkGamePage, wxWindow)
	EVT_BUTTON(ID_SELECT_MAP_BUTTON, wxNetworkGamePage::OnSelectMap)
	EVT_BUTTON(ID_GAME_OPTIONS_BUTTON, wxNetworkGamePage::OnGameOptions)
	EVT_BUTTON(ID_ACTIONS_BUTTON, wxNetworkGamePage::OnActionButton)
	EVT_BUTTON(ID_NOTREADY_BUTTON, wxNetworkGamePage::OnNotReadyButton)
END_EVENT_TABLE()

void wxNetworkGamePage::OnPreGame(const GamePtr &game)
{
	bool enable = false;
	bool notReady = false;

	// If this is the host, see if everyone in the game is ready, in which
	// case we can enable the action button.
	if(true == sIsHost)
	{
		// If there aren't any players in the game, the button should not be
		// enabled.
		wxInt32 numPlayers = game->numPlayers();
		enable = (numPlayers > 0);

		for(wxInt32 i = 0; i < numPlayers; ++i)
		{
			if(FALSE == game->playerGame(i).read<wxInt32>(shReady))
			{
				enable = false;
				break;
			}
		}

		// There must also be a map selected.
		if(-1 == MAPINDEX)
		{
			enable = false;
		}
	}
	// If this is a client, their button only be enabled if they are in the
	// game and not ready.
	else
	{
		const Player &thisPlayer = NET.Players().GetThisPlayer();

		for(wxInt32 i = 0; i < game->numPlayers(); ++i)
		{
			const PlayerGame &playerGame = game->playerGame(i);
			if(thisPlayer == playerGame.player())
			{
				enable = !(playerGame.read<wxInt32>(shReady)) || false;

				// Their not ready button should be the opposite of this one.
				notReady = !enable;
			}
		}
	}

	spActionsButton->Enable(enable);

	if(NULL != spNotReadyButton)
	{
		spNotReadyButton->Enable(notReady);
	}
}

void wxNetworkGamePage::OnResetReady(wxInt32)
{
	RULE.Execute(shNetworkRuleResetReady, DataObject());
}

void wxNetworkGamePage::OnSelectMap(wxCommandEvent &)
{	
	wxInt32 oldIndex = MAPINDEX;

	wxSelectMapDialog dlg(this, oldIndex);

	if(wxID_OK == dlg.ShowModal())
	{
		spVariantsButton->Enable(true);

		wxInt32 index = dlg.GetIndex();
		wxASSERT(-1 != index);

		// If the map or CK changed from its previous state, reset everyone's
		// player state.
		if(oldIndex != index)
		{
			// Load the new map if needed.
			if(oldIndex != index)
			{
				RULE.Execute(shNetworkRuleLoadMap, DataObject(index));
			}

			RULE.Execute(shNetworkRuleResetReady, DataObject());
		}
	}
}

void wxNetworkGamePage::OnGameOptions(wxCommandEvent &)
{
	wxOptionsDialog dialog(this);
	dialog.ShowModal();
}

void wxNetworkGamePage::OnActionButton(wxCommandEvent &)
{
	if(true == sIsHost)
	{
		RULE.Execute(shNetworkRuleLaunchGame, DataObject());
	}
	else
	{
		RULE.Execute(shNetworkRulePlayerReady, 
			DataObject(NET.Players().GetThisPlayer()));
	}
}

void wxNetworkGamePage::OnNotReadyButton(wxCommandEvent &)
{
	RULE.Execute(shNetworkRulePlayerNotReady, 
			DataObject(NET.Players().GetThisPlayer()));
}
