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
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "UIPlugin.h"
#include "RuleSetDatabase.h"
#include "Controller.h"
#include "GameEngine.h"
#include "DataObject.h"

#include "INetworkEngine.h"
#include "NetworkPlayers.h"
#include "BaseMessageBox.h"
#include "OnlineUI.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
    bool sIsCitiesOnlineGame = false;
}

//---------------------------- PUBLIC           -----------------------------//
class CitiesOnlineUI : public UIPlugin
{
public:
	CitiesOnlineUI()
	{
        Controller::get().AddReceiver(shEventPlayerReorder, 
            &CitiesOnlineUI::OnPlayerReorder, this);
	}

	~CitiesOnlineUI()
	{
        Controller::get().RemoveReceiver(shEventPlayerReorder, 
            &CitiesOnlineUI::OnPlayerReorder, this);
	}

	virtual QuitHandler GetQuitHandler(wxInt32& o_priority)
	{
		o_priority = 2;
		return boost::bind(&CitiesOnlineUI::QuitHandler, this, _1);
	}

private:
    virtual bool ProvidesUI() const
	{
		return sIsCitiesOnlineGame;
	}

    virtual void CreateUI(const GamePtr& game, wxAuiManager& manager)
	{
        wxASSERT(game);

        sIsCitiesOnlineGame = game->read<wxInt32>(shCitiesOnline) ? true : false;

        // Only create the UI if this is a Cities Online game.
        if(true == sIsCitiesOnlineGame)
        {
		    wxWindow* frame = GetMainFrame();

            // Cities Online window.
            wxOnlineUI* online = new wxOnlineUI(frame);
		    wxSize minSize = online->GetMinSize();
    		
		    manager.AddPane(online, wxAuiPaneInfo().Caption(swCitiesOnline)
			    .MinSize(minSize).CloseButton(false).MaximizeButton(false)
			    .Dockable(true).Bottom().Name(swCitiesOnlineNoSpaces));
        }
    }

    void OnPlayerReorder(const GamePtr& game)
    {
        // If the players get reordered (i.e. A New World maps), then tell the
        // site about it so it can reset the order.

        // See if this player is actually in the game.
        DataObject input(-1), output;
        RULE.Decide(shLogicIsThisPlayer, input, output);

        wxInt32 actualIndex = output.read<wxInt32>(1);
        
        // Only report the connection if we are the player in the game and in
        // a Cities Online game.
		if( (TRUE == game->read<wxInt32>(shCitiesOnline)) &&
			(-1 != actualIndex) )
		{
            const Player& player = NET.Players().GetThisPlayer();
            const wxInt32 id = game->read<wxInt32>(shID);

            ColorType type = game->playerGame(actualIndex).color();
			HashString color = HashString(ColorInfoData::Name(type));

            HashString command = HashString::Format(shConnectCmd.c_str(), id, player.Id(), 
                actualIndex + 1, color.c_str());
			Controller::get().Transmit(shEventSynchronize, command);
        }
    }
    
	bool QuitHandler(wxWindow* window)
	{
		// First, see if we are in a Cities Online game.  If we are, we have
		// special handling.  Otherwise, if we aren't, default to the standard
		// network handling.
		DataObject input(-1), output;
		RULE.Decide(shLogicIsCitiesOnline, input, output);

		bool ret = true;

		if(true == output.read<bool>())
		{
			ret = CitiesOnlineQuitHandling(window);
		}
		else
		{
			ret = NetworkQuitHandling(window);
		}

		return ret;
	}

	bool CitiesOnlineQuitHandling(wxWindow* window)
	{
		bool ret = true;

		if(true == NET.IsConnected())
		{
			// Just ask spectators if they are sure.
			if(true == NET.Players().IsSpectator())
			{
				wxBaseMessageBox dialog(window, stQuestion, stYouAreCurrentlyWatching, 
					MessageBoxYesNo);
				
				if(wxNO == dialog.ShowModal())
				{
					ret = false;
				}
			}
			// Tell players of the potential consequences.
			else
			{
				// TODO: Potentially differentiate based on rated vs. unrated games.
				wxBaseMessageBox dialog(window, stQuestion, stYouAreCurrentlyInAGame, 
					MessageBoxYesNo);

				if(wxNO == dialog.ShowModal())
				{
					ret = false;
				}

				if(true == ret)
				{
					RULE.Execute(shNetworkRuleQuit, 
						DataObject(NET.Players().GetThisPlayer()));

					// Give the network time to send it.
					wxMilliSleep(1000);
				}
			}
		}

		return ret;
	}

	bool NetworkQuitHandling(wxWindow* window)
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

IMPLEMENT_UI_PLUGIN(CITIESONLINE, CitiesOnlineUI)

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
