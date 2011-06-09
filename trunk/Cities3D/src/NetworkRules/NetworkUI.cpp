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
#include <wx/aui/aui.h>

//---------------------------- USER INCLUDES    -----------------------------//
#include "UIPlugin.h"
#include "RuleSetDatabase.h"
#include "Controller.h"
#include "GameEngine.h"
#include "DataObject.h"

#include "NetworkPane.h"
#include "NetworkRestartDialog.h"
#include "NetworkConnectionAttemptDialog.h"
#include "NetworkPlayers.h"
#include "BaseMessageBox.h"
#include "INetworkEngine.h"
#include "INetworkClient.h"
#include "IUIEngine.h"
#include "IRuleEngine.h"
#include "IPluginEngine.h"
#include "SettlersGLCanvas.h"
#include "ChatUI.h"
#include "EventUI.h"
#include "SpectatorUI.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
class NetworkUI : public UIPlugin
{
public:
	NetworkUI()
	{
		wxSocketBase::Initialize();
	
		Controller::get().AddReceiver(shEventNetworkRestart,
			&NetworkUI::OnNetworkRestart, this);
		Controller::get().AddReceiver(shEventNetworkConnectionAttempt,
			&NetworkUI::OnNetworkConnectionAttempt, this);

		GUI.RegisterMainMenuItem(MODULE, NETWORK, swPlay, 200, 
			stLANNetwork, shNetwork, swNetwork);
	}

	~NetworkUI()
	{
		Controller::get().RemoveReceiver(shEventNetworkRestart,
			&NetworkUI::OnNetworkRestart, this);
		Controller::get().RemoveReceiver(shEventNetworkConnectionAttempt,
			&NetworkUI::OnNetworkConnectionAttempt, this);
	}

	virtual QuitHandler GetQuitHandler(wxInt32& o_priority)
	{
		o_priority = 1;
		return boost::bind(&NetworkUI::QuitHandler, this, _1);
	}

	virtual void HandleMainMenuClick(const wxString& key)
	{
		wxASSERT(swNetwork == key);
		
		// Create the wxAuiNotebook that will house all of the network 
		// connection information.
		wxWindow* frame = GetMainFrame();

		wxNetworkPane* pane = new wxNetworkPane(frame);

		Controller::get().Transmit(shEventChangeMainPane, 
			DataObject(static_cast<PaneMixin*>(pane), stLANNetworkSetup));
	}

	virtual bool ProvidesUI() const
	{
		return true;
	}

	virtual void CreateUI(const GamePtr& game, wxAuiManager& manager)
	{
		// Create all of the UI needed for network games.
		wxWindow* frame = GetMainFrame();

		wxSize minSize = wxSize(165, 150);

		// Create the notebook off screen to avoid flicker.
		wxSize size = frame->GetClientSize();

		wxAuiNotebook* notebook = new wxAuiNotebook(frame, wxID_ANY,
			wxPoint(size.x, size.y), minSize, wxAUI_NB_TOP | 
			wxAUI_NB_TAB_SPLIT |wxAUI_NB_TAB_MOVE | wxAUI_NB_SCROLL_BUTTONS |
			wxAUI_NB_TAB_EXTERNAL_MOVE | wxNO_BORDER);

		// Chat window.
		wxChatUI* chat = new wxChatUI(notebook);
		notebook->AddPage(chat, stChat);

        // Event window.
        wxEventUI* event = new wxEventUI(notebook);
        notebook->AddPage(event, stEvents);
		
		// Spectator window.
		wxSpectatorUI* spectators = new wxSpectatorUI(notebook);
		notebook->AddPage(spectators, stSpectators);

		manager.AddPane(notebook, wxAuiPaneInfo().MinSize(minSize)
			.Caption(stNetwork).CloseButton(false)
			.MaximizeButton(false).Dockable(true).Right()
			.Name(swNetwork));
	}

private:
	void OnNetworkRestart(bool restart)
	{
		wxNetworkRestartDialog *dialog = new wxNetworkRestartDialog(
			GetGLCanvas(), restart);
		dialog->Show();
	}

	void OnNetworkConnectionAttempt(const DataObject& object)
	{
		wxASSERT(2 <= object.numItems());

		const wxIPV4address& address = object.read<wxIPV4address>();
		INetworkClient* client = object.read<INetworkClient*>(1);

		wxNetworkConnectionAttemptDialog dialog(GetMainFrame(), address, 
			client);
		dialog.ShowModal();
	}

	bool QuitHandler(wxWindow* window)
	{
		bool isHost = NET.Players().IsHost();
		bool isConnected = false;
		bool quit = true;

		if( (true == isHost) &&
			(true == NET.DoesServerHaveClients())
			)
		{
			isConnected = true;

			wxBaseMessageBox dialog(window, stQuestion,
				stThereArePlayersConnectedToYou, MessageBoxYesNo);

			if(wxNO == dialog.ShowModal())
			{
				quit = false;
			}
		}
		else if((false == isHost) &&
				(true == NET.IsConnected()))
		{
			isConnected = true;

			wxBaseMessageBox dialog(window, stQuestion,
				stYouAreCurrentlyConnectedToAServer, MessageBoxYesNo);

			if(wxNO == dialog.ShowModal())
			{
				quit = false;
			}
		}

		if( (true == quit) &&
			(true == isConnected))
		{
			RULE.Execute(shNetworkRuleQuit, 
				DataObject(NET.Players().GetThisPlayer()));
		}

		return quit;
	}
};

IMPLEMENT_UI_PLUGIN(NETWORK, NetworkUI)

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
