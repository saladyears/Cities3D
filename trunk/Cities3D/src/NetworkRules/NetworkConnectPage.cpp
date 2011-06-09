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
#include "NetworkConnectPage.h"
#include "BaseStaticBox.h"
#include "BaseRadioButton.h"
#include "FittedStaticText.h"
#include "PlayerListCtrl.h"
#include "BaseButton.h"
#include "BaseMessageBox.h"
#include "Player.h"
#include "PlayerDatabase.h"
#include "AddPlayerDialog.h"
#include "ConfigDatabase.h"
#include "SkinManager.h"
#include "DataObject.h"
#include "GameEngine.h"
#include "Controller.h"
#include "INetworkEngine.h"
#include "NetworkPlayers.h"
#include "NetworkConnectToDialog.h"
#include "NetworkSettingsDialog.h"
#include "Game.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	wxRadioButton *spHostRadio;
	wxRadioButton *spClientRadio;
	wxBaseStaticText *spPlayerName;
	wxBaseStaticText *spIPAddress;
	wxBaseButton *spActionButton;
	wxBaseButton *spSettingsButton;
	wxPlayerListCtrl *spPlayerList;
	wxBaseButton *spDeleteButton;

	Player sPlayer;

	bool sHost = false;
	bool sInGame = false;

	enum
	{
		ID_HOST_RADIO			= 0,
		ID_CLIENT_RADIO			,
		
		ID_PLAYER_NAME			,
		ID_IP_ADDRESS			,
		ID_ACTION_BUTTON		,
		ID_SETTINGS_BUTTON		,

		ID_PLAYER_LIST			,

		ID_PLAYER_ADD			,
		ID_PLAYER_DELETE		,
	};

	void EnableActions(const wxIPV4address &address)
	{
		spIPAddress->Freeze();

		if(true == sHost)
		{
			if(NET.IsConnected())
			{
				spActionButton->SetLabel(stStopServer);
				spIPAddress->SetLabel(
					wxString::Format(swIPFormat.c_str(), 
					address.IPAddress().c_str(), address.Service(),
					stStarted.c_str()));
				spIPAddress->SetForegroundColour(
					SKIN.Element(shNetworkTextOn));
				spSettingsButton->Enable(false);
			}
			else
			{
				//set the action button text properly
				spActionButton->SetLabel(stStartServer);
				spIPAddress->SetLabel(
					wxString::Format(swIPFormat.c_str(), 
					address.IPAddress().c_str(), address.Service(), 
					stStopped.c_str()));
				spIPAddress->SetForegroundColour(
					SKIN.Element(shNetworkTextOff));
				spSettingsButton->Enable(true);
			}

			spSettingsButton->Show(true);
		}
		else
		{
			if(NET.IsConnected())
			{
				spActionButton->SetLabel(stDisconnect);
				spIPAddress->SetLabel(
					wxString::Format(swIPFormat.c_str(), 
					address.IPAddress().c_str(), address.Service(),
					stConnected.c_str()));
				spIPAddress->SetForegroundColour(
					SKIN.Element(shNetworkTextOn));
			}
			else
			{
				spActionButton->SetLabel(stConnect);
				spIPAddress->SetLabel(stNotConnected);
				spIPAddress->SetForegroundColour(
					SKIN.Element(shNetworkTextOff));
			}

			spSettingsButton->Show(false);
		}

		spIPAddress->Thaw();
	}

	void EnableNames()
	{
		wxInt32 index = spPlayerList->GetSelectedItem();
		
		if(-1 != index)
		{
			wxInt32 playerIndex = spPlayerList->GetItemData(index);

			sPlayer = PlayerDatabase::get().GetPlayer(playerIndex);

			spPlayerName->SetLabel(sPlayer.Name());
			spPlayerName->SetForegroundColour(SKIN.Element(shNetworkTextOn));
			spDeleteButton->Enable(true);

			spActionButton->Enable(true);
		}
		else
		{
			spPlayerName->SetLabel(stUnselected);
			spPlayerName->SetForegroundColour(SKIN.Element(shNetworkTextOff));
			spDeleteButton->Enable(false);

			spActionButton->Enable(false);
		}
	}

	bool HostShutdownCheck()
	{
		bool ret = true;

		// If players are actually connected, ask if they really want
		// to shut it down.
		if(true == NET.DoesServerHaveClients())
		{
			wxBaseMessageBox dialog(NULL, stQuestion, 
				stThereArePlayersConnectedToYou, MessageBoxYesNo);

			if(wxNO == dialog.ShowModal())
			{
				ret = false;
			}
			else
			{
				RULE.Execute(shNetworkRuleQuit, 
                    DataObject(NET.Players().GetThisPlayer()));
			}
		}

		return ret;
	}

	bool ClientShutdownCheck()
	{
		bool ret = true;

		// Ask them if they really want to quit, since they are already
		// connected.
		wxBaseMessageBox dialog(NULL, stQuestion, 
			stYouAreCurrentlyConnectedToAServer, MessageBoxYesNo);

		if(wxNO == dialog.ShowModal())
		{
			ret = false;
		}
		else
		{
			RULE.Execute(shNetworkRuleQuit, 
                DataObject(NET.Players().GetThisPlayer()));
		}

		return ret;
	}
}

//---------------------------- PUBLIC           -----------------------------//
wxNetworkConnectPage::wxNetworkConnectPage(wxWindow* parent)
: wxPanel(parent, wxID_ANY)
{
	this->SetBackgroundColour(parent->GetBackgroundColour());

	sInGame = false;

	mAddress.AnyAddress();
	mAddress.Service(7176);

	wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *pTopBox = new wxBoxSizer(wxHORIZONTAL);

	wxBaseStaticBox *box1 = new wxBaseStaticBox(this, -1, stConnection, 
		wxDefaultPosition, wxSize(100, 80));
	wxStaticBoxSizer *boxSizer1 = new wxStaticBoxSizer(box1, wxVERTICAL);

	wxBaseStaticBox *box2 = new wxBaseStaticBox(this, -1, stStatus, 
		wxDefaultPosition, wxSize(300, 80));
	wxStaticBoxSizer *boxSizer2 = new wxStaticBoxSizer(box2, wxVERTICAL);

	sHost = CONFIG.read<wxInt32>(swConfigSectionNetwork, swHost, FALSE) || false;
	spHostRadio = new wxBaseRadioButton(this, ID_HOST_RADIO, stHost, 
		shDefaultText, shDefaultFont, wxDefaultPosition, wxDefaultSize, 
		wxRB_GROUP);
	spClientRadio = new wxBaseRadioButton(this, ID_CLIENT_RADIO, 
		stConnectTo);

	boxSizer1->Add(spHostRadio, 0, wxALL, 5);
	boxSizer1->Add(spClientRadio, 0, wxALL, 5);

	if(true == sHost)
	{
		RULE.Execute(shNetworkRuleStoreIP, 
			DataObject(mAddress.IPAddress(), mAddress.Service()));

		spHostRadio->SetValue(true);
	}
	else
	{
		RULE.Execute(shNetworkRuleClearIP, DataObject());

		spClientRadio->SetValue(true);
	}

	wxBaseStaticText *player = new wxBaseStaticText(this, -1, 
        stPlayer + swColon);

	spPlayerName = new wxBaseStaticText(this, ID_PLAYER_NAME, swEmpty, 
		shNetworkTextOff, shBoldFont);

	wxBoxSizer *innerHorz1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *innerHorz2 = new wxBoxSizer(wxHORIZONTAL);

	innerHorz1->Add(player, 0, wxEXPAND | wxALL, 5);
	innerHorz1->Add(spPlayerName, 0, wxEXPAND | wxALL, 5);

	wxBaseStaticText *address = new wxBaseStaticText(this, -1, 
        stIPAddress + swColon);
	spIPAddress = new wxBaseStaticText(this, ID_IP_ADDRESS, swEmpty, 
		shNetworkTextOff, shBoldFont, wxSize(200, 11), wxST_NO_AUTORESIZE | 
		wxALIGN_LEFT);

	innerHorz2->Add(address, 0, wxEXPAND | wxLEFT, 5);
	innerHorz2->Add(spIPAddress, 0, wxEXPAND | wxLEFT, 5);

	boxSizer2->Add(innerHorz1, 0, wxEXPAND | wxALL, 5);
	boxSizer2->Add(innerHorz2, 0, wxEXPAND | wxALL, 5);

	wxBoxSizer *innerVert1 = new wxBoxSizer(wxVERTICAL);
	spActionButton = new wxBaseButton(this, ID_ACTION_BUTTON, stConnect);
	spActionButton->Enable(false);
	spSettingsButton = new wxBaseButton(this, ID_SETTINGS_BUTTON, 
		stSettings);
	innerVert1->Add(spActionButton, 0, wxALL, 6);
	innerVert1->Add(spSettingsButton, 0, wxALL, 6);

	pTopBox->Add(boxSizer1, 0, wxEXPAND);
	pTopBox->Add(boxSizer2, 0, wxEXPAND | wxLEFT | wxRIGHT, 6);
	pTopBox->Add(innerVert1, 0, wxEXPAND | wxLEFT, 6);

	wxBoxSizer *boxSizer10 = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer *innerVert10 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *innerVert11 = new wxBoxSizer(wxVERTICAL);

	spPlayerList = new wxPlayerListCtrl(this, ID_PLAYER_LIST, wxDefaultPosition,
		wxSize(406, 300));

	wxBaseStaticText *explainText = new wxFittedStaticText(this, -1, 
		stUseTheConnectionSettings, shInfoText, shInfoFont, wxSize(406, 10));

	innerVert10->Add(spPlayerList, 0, wxEXPAND | wxTOP | wxRIGHT | 
		wxBOTTOM, 6);
	innerVert10->Add(explainText, 0, wxEXPAND | wxLEFT | wxBOTTOM, 2);

	wxBaseButton *button = new wxBaseButton(this, ID_PLAYER_ADD, stAdd);
	spDeleteButton = new wxBaseButton(this, ID_PLAYER_DELETE, stDelete);
	spDeleteButton->Enable(false);

	innerVert11->Add(button, 0, wxEXPAND | wxALL, 6);
	innerVert11->Add(spDeleteButton, 0, wxEXPAND | wxALL, 6);

	boxSizer10->Add(innerVert10, 0, wxEXPAND);
	boxSizer10->Add(innerVert11, 0, wxEXPAND);	

	pSizer->Add(pTopBox, 0, wxEXPAND | wxLEFT, 7);
	pSizer->Add(boxSizer10, 0, wxEXPAND | wxTOP | wxLEFT | wxBOTTOM, 7);
	
	SetSizer(pSizer);

	EnableNames();
	EnableActions(mAddress);

	Controller::get().AddReceiver(shEventGameConnect, 
		&wxNetworkConnectPage::OnGameConnect, this);
}

wxNetworkConnectPage::~wxNetworkConnectPage()
{
	Controller::get().RemoveReceiver(shEventGameConnect,
		&wxNetworkConnectPage::OnGameConnect, this);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxNetworkConnectPage, wxWindow)
	EVT_RADIOBUTTON(ID_HOST_RADIO, wxNetworkConnectPage::OnHostRadio)
	EVT_RADIOBUTTON(ID_CLIENT_RADIO, wxNetworkConnectPage::OnClientRadio)
	EVT_BUTTON(ID_ACTION_BUTTON, wxNetworkConnectPage::OnActionButton)
	EVT_BUTTON(ID_SETTINGS_BUTTON, wxNetworkConnectPage::OnSettingsButton)
	EVT_BUTTON(ID_PLAYER_ADD, wxNetworkConnectPage::OnAddPlayer)
	EVT_BUTTON(ID_PLAYER_DELETE, wxNetworkConnectPage::OnDeletePlayer)
	EVT_LIST_ITEM_SELECTED(ID_PLAYER_LIST, 
	wxNetworkConnectPage::OnPlayerSelected)
	EVT_LIST_ITEM_DESELECTED(ID_PLAYER_LIST, 
	wxNetworkConnectPage::OnPlayerSelected)
END_EVENT_TABLE()

void wxNetworkConnectPage::OnGameConnect(const DataObject& object)
{
	bool val = object.read<bool>();

	// Check to see if we're actually still pre-game.
	if(2 <= object.numItems())
	{
		const GamePtr& game = object.read<GamePtr>(1);
		const HashString &state = game->read<HashString>(shState);

		if(shNew != state)
		{
			sInGame = true;
		}
	}

	EnableActions(mAddress);

	// Players cannot change who they are once connected.
	spPlayerList->Enable(!val);
}

void wxNetworkConnectPage::OnHostRadio(wxCommandEvent &)
{
	if(false == sHost)
	{
		if(true == NET.IsConnected())
		{
			if(false == ClientShutdownCheck())
			{
				spClientRadio->SetValue(true);
				return;
			}
		}

		sHost = true;

		// Reset the game data.
		Controller::get().Transmit(shEventResetGame, 0);
		
		//stop any current network connection
		NET.Stop();

		CONFIG.write<wxInt32>(swConfigSectionNetwork, swHost, TRUE);

		if(false == mAddress.AnyAddress())
		{
			wxLogError(wxT("Unable to determine local IP address."));
			return;
		}

		mAddress.Service(7176);			

		RULE.Execute(shNetworkRuleStoreIP, 
				DataObject(mAddress.IPAddress(), mAddress.Service()));

		EnableActions(mAddress);

		Layout();

		//SetLastFocus(NULL);
	}
}

void wxNetworkConnectPage::OnClientRadio(wxCommandEvent &)
{
	if(true == sHost)
	{		
		// If players are actually connected, ask if they really want
		// to shut it down.
		if( (true == NET.DoesServerHaveClients()) &&
			(false == HostShutdownCheck())
			)
		{
			spHostRadio->SetValue(true);
			return;
		}

		sHost = false;

		// Reset the game data.
		Controller::get().Transmit(shEventResetGame, 0);

		//stop any current network connection
		NET.Stop();

		CONFIG.write<wxInt32>(swConfigSectionNetwork, swHost, FALSE);

		RULE.Execute(shNetworkRuleClearIP, DataObject());

		EnableActions(mAddress);
	}
}

void wxNetworkConnectPage::OnActionButton(wxCommandEvent &)
{
	//host
	if(true == spHostRadio->GetValue())
	{
		if(NET.IsConnected())
		{
			if(false == HostShutdownCheck())
			{
				return;
			}

			NET.Stop();
		}
		else
		{
			NET.StartServer(mAddress, sPlayer);
		}
	}
	//client
	else
	{
		if(NET.IsConnected())
		{
			if(false == ClientShutdownCheck())
			{
				return;
			}

			NET.Stop();
		}
		else
		{
			wxNetworkConnectToDialog dialog(this);

			if(wxID_OK == dialog.ShowModal())
			{
				mAddress = dialog.GetAddress();

				spIPAddress->Freeze();
				spIPAddress->SetLabel(
					wxString::Format(swIPFormat.c_str(), 
					mAddress.IPAddress().c_str(), mAddress.Service(), 
					stConnecting.c_str()));
				spIPAddress->SetForegroundColour(
					SKIN.Element(shNetworkTextWait));
				spIPAddress->Thaw();

				NET.StartClient(mAddress, sPlayer);
			}
		}
	}

	EnableActions(mAddress);

	if(true == sInGame)
	{
		Controller::get().Transmit(shEventPrepareNetworkGame, false);
	}
}

void wxNetworkConnectPage::OnSettingsButton(wxCommandEvent &)
{
	wxNetworkSettingsDialog dialog(this);

	if(wxID_OK == dialog.ShowModal())
	{
		mAddress = dialog.GetAddress();

		RULE.Execute(shNetworkRuleStoreIP, 
			DataObject(mAddress.IPAddress(), mAddress.Service()));

		EnableActions(mAddress);
	}
}

void wxNetworkConnectPage::OnAddPlayer(wxCommandEvent &)
{
	wxAddPlayerDialog dlg(this);
	if(wxID_OK == dlg.ShowModal())
	{
		//add the player to the current player list
		spPlayerList->AddPlayer(dlg.GetPlayer().Name());

		EnableNames();
	}
}

void wxNetworkConnectPage::OnDeletePlayer(wxCommandEvent &)
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

		EnableNames();
	}
}

void wxNetworkConnectPage::OnPlayerSelected(wxListEvent &)
{
	EnableNames();
}
