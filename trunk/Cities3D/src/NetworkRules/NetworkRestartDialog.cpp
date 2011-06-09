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
#include "NetworkRestartDialog.h"
#include "ISizer.h"
#include "NetworkRestartListCtrl.h"
#include "SpectatorRestartListCtrl.h"
#include "BaseStaticText.h"
#include "BaseButton.h"
#include "TimerManager.h"
#include "Game.h"
#include "NetworkPlayers.h"
#include "GameEngine.h"
#include "DataObject.h"
#include "INetworkEngine.h"
#include "IRuleEngine.h"
#include "Controller.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	const size_t FINAL_SECONDS = 10;
	const size_t PLAYER_SECONDS = 30;

	wxBaseStaticText *spText;
	wxBaseButton *spPlayButton;
	wxBaseButton *spNoThanksButton;

	enum
	{
		ID_PLAY_BUTTON			= 0,
		ID_NO_THANKS_BUTTON		,
	};

	class Sizer_NetworkRestart : public ISizer
	{
	public:
		Sizer_NetworkRestart(wxWindow *window, bool restart)	
			: ISizer(window)
			, mRestart(restart)
		{}

		void CreateLayout(wxBoxSizer *sizer)
		{
			wxBoxSizer *vert = new wxBoxSizer(wxVERTICAL);

			// Player lists and buttons.
			wxSizer *horz1 = new wxBoxSizer(wxHORIZONTAL);

			wxSizer *vert1 = new wxBoxSizer(wxVERTICAL);
			wxSizer *vert2 = new wxBoxSizer(wxVERTICAL);
			wxSizer *vert3 = new wxBoxSizer(wxVERTICAL);

			wxBaseListCtrl *network = new wxNetworkRestartListCtrl(mpWindow, 
				-1, wxDefaultPosition, wxSize(150, 200));
			vert1->Add(network, 1, wxEXPAND | wxALL, 3);

			wxBaseListCtrl *spectators = new wxSpectatorRestartListCtrl(
				mpWindow, -1, wxDefaultPosition, wxSize(150, 200));
			vert2->Add(spectators, 1, wxEXPAND | wxALL, 3);

			spPlayButton = new wxBaseButton(mpWindow, ID_PLAY_BUTTON, 
				stIllPlay);
			spPlayButton->Enable(false);
			spNoThanksButton = new wxBaseButton(mpWindow, ID_NO_THANKS_BUTTON,
				stNoThanks);
			spNoThanksButton->Enable(false);

			vert3->Add(spPlayButton, 0, wxALL, 3);
			vert3->Add(spNoThanksButton, 0, wxALL, 3);

			horz1->Add(vert1, 1, wxEXPAND);
			horz1->Add(vert2, 1, wxEXPAND);
			horz1->Add(vert3, 0, wxEXPAND);

			// Spectators aren't allowed to join starts.
			if(false == mRestart)
			{
				spPlayButton->Hide();
				spNoThanksButton->Hide();
			}

			// Text.
			wxSizer *horz2 = new wxBoxSizer(wxHORIZONTAL);

			wxString str = mRestart ? 
				stWaitingForPlayersToRejoin :
				stWaitingForPlayersToJoin ;

			spText = new wxBaseStaticText(mpWindow, -1, str);
			horz2->Add(spText, 1, wxEXPAND | wxALL, 3);

			vert->Add(horz1, 1, wxEXPAND);
			vert->Add(horz2, 0, wxEXPAND);

			sizer->Add(vert, 1, wxEXPAND);
		}

	private:
		bool mRestart;
	};
}

enum Mode
{
	ModeNone				= 0,
	ModeFinalCountdown		,
	ModePlayerCountdown		,
};

struct wxNetworkRestartDialog::Impl
{
	Impl(bool restart) 
		: mMode(ModeNone)
		, mHasTimer(false) 
		, mRestart(restart)
	{}

	Mode mMode;
	wxStopWatch mStopWatch;

	bool mHasTimer;
	bool mRestart;
};

//---------------------------- PUBLIC           -----------------------------//
#ifdef __WIN32__
	//disables the 'this' : used in base member initializer list warning
	#pragma warning(disable:4355)
#endif
wxNetworkRestartDialog::wxNetworkRestartDialog(wxWindow *parent, bool restart)
: wxBaseGameDialog(parent, ISizerPtr(new Sizer_NetworkRestart(this, restart)), 
	restart ? stNetworkGameRestart : stOnlineGameRestart, swNetworkGameRestart)
, mpImpl(new Impl(restart))
{
	Controller::get().AddReceiver(shEventNetworkRestartPlayers, 
		&wxNetworkRestartDialog::OnUpdatePlayers, this);
	Controller::get().AddReceiver(shEventNetworkAllSeatsFull,
		&wxNetworkRestartDialog::OnAllSeatsFull, this);
	Controller::get().AddReceiver(shEventSpectator,
		&wxNetworkRestartDialog::OnSpectator, this);
}

wxNetworkRestartDialog::~wxNetworkRestartDialog()
{
	Controller::get().RemoveReceiver(shEventNetworkRestartPlayers, 
		&wxNetworkRestartDialog::OnUpdatePlayers, this);
	Controller::get().RemoveReceiver(shEventNetworkAllSeatsFull,
		&wxNetworkRestartDialog::OnAllSeatsFull, this);
	Controller::get().RemoveReceiver(shEventSpectator,
		&wxNetworkRestartDialog::OnSpectator, this);

	// Remove any timer that might be firing.
	TIME.RemovePeriodicFunction(&wxNetworkRestartDialog::OnTimer, this, 1000);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxNetworkRestartDialog, wxBaseGameDialog)
	EVT_BUTTON(ID_PLAY_BUTTON, wxNetworkRestartDialog::OnPlay)
	EVT_BUTTON(ID_NO_THANKS_BUTTON, wxNetworkRestartDialog::OnNoThanks)
END_EVENT_TABLE()

void wxNetworkRestartDialog::OnTimer()
{
	switch(mpImpl->mMode)
	{
	case ModeFinalCountdown:
		{
			// See how much time has passed.
			unsigned long milliseconds = mpImpl->mStopWatch.Time();
			wxTimeSpan timeSpan(0, 0, 0, ((FINAL_SECONDS + 1) * 1000) - 
				milliseconds);
			
			wxString str = wxString::Format(mpImpl->mRestart ? 
				stTheGameWillRestartIn.c_str() :
				stTheGameWillStartIn.c_str(), 
				timeSpan.GetSeconds().GetValue());
			spText->SetLabel(str);

			 // Only the host can make the final call.
			if( (true == NET.Players().IsHost() &&
				(FINAL_SECONDS * 1000) < milliseconds)
				)
			{
				RULE.Execute(shNetworkRuleAllSeatsFull, DataObject());
			}
		}
		break;
	case ModePlayerCountdown:
		{
			// Only countdown if we're not starting a new online game.
			if(true == mpImpl->mRestart)
			{
				// Tick off one second from the time span for the player.
				RULE.AsynchExecute(shNetworkRuleTickCountdown, DataObject());
			}
		}
		break;
	default:
		{
			wxString str = mpImpl->mRestart ?
				stWaitingForPlayersToRejoin :
				stWaitingForPlayersToJoin ;
			spText->SetLabel(str);
		}
		break;
	}
}

void wxNetworkRestartDialog::OnUpdatePlayers(const GamePtr &game)
{
	bool restart = true;
	bool timer = false;

	Impl &impl = *mpImpl;

	// Look at all the players in the game.  If none of them are marked as
	// NullPlayer, we're ready to restart.
	for(wxInt32 i = 0; i < game->numPlayers(); ++i)
	{
		if(NullPlayer == game->playerGame(i).player())
		{
			// Check to see if this color has started a countdown until they
			// can be replaced.
			ColorType color = game->playerGame(i).color();
			RULE.AsynchExecute(shNetworkRuleStartCountdown, DataObject(
				color, PLAYER_SECONDS));
			timer = true;
			impl.mMode = ModePlayerCountdown;

			restart = false;
			break;
		}
	}

	if(true == restart)
	{
		impl.mMode = ModeFinalCountdown;
		impl.mStopWatch.Start();
		timer = true;
	}

	if( (true == timer) &&
		(false == impl.mHasTimer))
	{
		TIME.AddPeriodicFunction(&wxNetworkRestartDialog::OnTimer, this, 1000);
		impl.mHasTimer = true;
	}
}

void wxNetworkRestartDialog::OnAllSeatsFull(wxInt32)
{
	Destroy();
}

void wxNetworkRestartDialog::OnSpectator(wxInt32)
{
	bool play = false;
	bool noplay = false;

	// A button is only enabled if the player at this computer is a spectator.
	const SpectatorArray &spectators = NET.Players().GetSpectators();
	const Player &player = NET.Players().GetThisPlayer();

	SpectatorArray::const_iterator it, itEnd = spectators.end();
	for(it = spectators.begin(); it != itEnd; ++it)
	{
		if(it->first == player)
		{
			noplay = it->second || false;
			play = !noplay;
		}
	}

	spPlayButton->Enable(play);
	spNoThanksButton->Enable(noplay);
}

void wxNetworkRestartDialog::OnPlay(wxCommandEvent &)
{
	RULE.Execute(shNetworkRuleSpectatorState, 
		DataObject(NET.Players().GetThisPlayer(), TRUE));
}

void wxNetworkRestartDialog::OnNoThanks(wxCommandEvent &)
{
	RULE.Execute(shNetworkRuleSpectatorState, 
		DataObject(NET.Players().GetThisPlayer(), FALSE));
}
