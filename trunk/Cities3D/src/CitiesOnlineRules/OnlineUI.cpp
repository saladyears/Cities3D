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
#include "OnlineUI.h"
#include "SkinManager.h"
#include "Controller.h"
#include "BaseStaticText.h"
#include "TimerManager.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	const wxSize sMinSizeOnline = wxSize(165, 90);

	wxBaseStaticText* spTimerText = NULL;

    wxBaseStaticText* spTurnTimersLeft = NULL;
    wxBaseStaticText* spTurnTimersLeftText = NULL;
    wxBaseStaticText* spPassiveQuitText = NULL;
    wxBaseStaticText* spMultiplier = NULL;
    wxBaseStaticText* spMultiplierText = NULL;

	bool sInTurnCountdown = false;
    bool sInPassiveCountdown = false;
	wxStopWatch sStopWatch;
	unsigned long sCountdownTime = 0;

    size_t sLastTick = 0;
}

//---------------------------- PUBLIC           -----------------------------//
wxOnlineUI::wxOnlineUI(wxWindow *parent) 
: wxBaseUI<wxPanel>(parent, sMinSizeOnline)
{
	Show(true);

	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	spTimerText = new wxBaseStaticText(this, wxID_ANY, swDashTime, 
		shDefaultText, shTurnTimerFont, wxDefaultSize, 
		wxALIGN_CENTRE_HORIZONTAL | wxST_NO_AUTORESIZE);

	sizer->AddStretchSpacer();
	sizer->Add(spTimerText, 0, wxEXPAND);
	sizer->AddStretchSpacer();

    wxSizer* horz = new wxBoxSizer(wxHORIZONTAL);
    spTurnTimersLeft = new wxBaseStaticText(this, wxID_ANY, 
        swSpace, shInfoText, shTurnTimerTextFont, wxDefaultSize);
    
    spTurnTimersLeftText = new wxBaseStaticText(this, wxID_ANY, 
        swSpace, shInfoText, shTurnTimerTextFont, wxDefaultSize);
    
    spPassiveQuitText = new wxBaseStaticText(this, wxID_ANY, 
        swSpace, shDefaultText, shTurnTimerTextFont, wxDefaultSize, 
        wxALIGN_CENTRE);

    spMultiplier = new wxBaseStaticText(this, wxID_ANY, 
        swSpace, shInfoText, shTurnTimerTextFont, wxDefaultSize);
    
    spMultiplierText = new wxBaseStaticText(this, wxID_ANY, swSpace, 
        shInfoText, shTurnTimerTextFont, wxDefaultSize);
    
    horz->Add(spTurnTimersLeft, 0, wxEXPAND | wxALIGN_LEFT | wxALL, 2);
    horz->Add(spTurnTimersLeftText, 0, wxEXPAND | wxALIGN_LEFT | wxALL, 2);
    horz->Add(spPassiveQuitText, 1, wxEXPAND | wxALIGN_CENTRE | wxALL, 2);
    horz->Add(spMultiplier, 0, wxEXPAND | wxALIGN_RIGHT | wxALL, 2);
    horz->Add(spMultiplierText, 0, wxEXPAND | wxALIGN_RIGHT | wxALL, 2);
    
    sizer->Add(horz, 0, wxEXPAND);

	SetBackgroundColour(SKIN.Element(shBackground));

	SetSizer(sizer);
	sizer->SetSizeHints(this);

	// Add a periodic timer for a once a second countdown.
	TIME.AddPeriodicFunction(&wxOnlineUI::OnTimer, this, 500);

	Controller::get().AddReceiver(shEventResetTurnTime, 
		&wxOnlineUI::OnResetTurnTime, this);
    Controller::get().AddReceiver(shEventResetPassiveTime, 
		&wxOnlineUI::OnResetPassiveTime, this);
    Controller::get().AddReceiver(shEventShutdownUI, 
        &wxOnlineUI::OnShutdownUI, this);
}

wxOnlineUI::~wxOnlineUI()
{
	TIME.RemovePeriodicFunction(&wxOnlineUI::OnTimer, this, 500);

	Controller::get().RemoveReceiver(shEventResetTurnTime, 
		&wxOnlineUI::OnResetTurnTime, this);
    Controller::get().RemoveReceiver(shEventResetPassiveTime, 
		&wxOnlineUI::OnResetPassiveTime, this);
    Controller::get().RemoveReceiver(shEventShutdownUI, 
        &wxOnlineUI::OnShutdownUI, this);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void wxOnlineUI::OnResetTurnTime(const DataObject& object)
{
    // It's possible to go directly from a passive timer directly to a turn
    // timer from the PreRoll state.
    sInTurnCountdown = true;
    sInPassiveCountdown = false;

    if(true == ResetCountdown(object))
    {
        // Restart the timer.
	    sStopWatch.Start();
    }
}

void wxOnlineUI::OnResetPassiveTime(wxInt32 seconds)
{
    // It's possible that we go from a turn timer directly to a passive
    // timer.
    sInPassiveCountdown = true;
    sInTurnCountdown = false;

    if(true == ResetCountdown(DataObject(seconds, -1, -1)))
    {
        // Restart the timer.
	    sStopWatch.Start();
    }
}

void wxOnlineUI::OnShutdownUI(wxInt32)
{
	Freeze();

    sInTurnCountdown = false;
    sInPassiveCountdown = false;
    spTimerText->SetLabel(swDashTime);
    spTimerText->SetTextColor(shPlayerUINeutral);

    spTurnTimersLeft->SetLabel(swSpace);
    spTurnTimersLeftText->SetLabel(swSpace);
    spMultiplier->SetLabel(swSpace);
    spMultiplierText->SetLabel(swSpace);
    spPassiveQuitText->SetLabel(swSpace);
    Layout();

	Thaw();
}

void wxOnlineUI::OnTimer()
{
	if(true == sInTurnCountdown || true == sInPassiveCountdown)
	{
		unsigned long milliseconds = sStopWatch.Time();

		// See if we're done.
		if((sCountdownTime * 1000) <= milliseconds)
		{
			spTimerText->SetLabel(swZeroTime);

            bool inTurn = sInTurnCountdown;

            sInTurnCountdown = false;
            sInPassiveCountdown = false;

            // If we're out of time, asynchronously shut everything down 
            // immediately, before the user has a chance to do anything.
            DataObject input(-1), output;
            RULE.Decide(shLogicIsOutOfTime, input, output);
            if(true == output.read<bool>())
            {
                Controller::get().Transmit(shEventShutdownUI, 0);
            }

            if(true == inTurn)
            {
			    RULE.Execute(shRuleTurnTimerDone, DataObject());
            }
            else
            {
                // Get the index of the player quitting.
                DataObject input(-1), output;
                RULE.Decide(shLogicIsThisPlayer, input, output);
                wxInt32 actualIndex = output.read<wxInt32>(1);

                RULE.Execute(shRulePassiveTimerDone, DataObject(actualIndex));
            }
		}
		else
		{
			wxTimeSpan timeSpan(0, 0, 0, (sCountdownTime * 1000) - milliseconds);
			size_t seconds = static_cast<size_t>(timeSpan.GetSeconds().ToLong());
			size_t minutes = seconds / 60;
			seconds -= (minutes * 60);

            if( (0 == minutes) && (10 >= seconds) )
            {
                spTimerText->SetTextColor(shPlayerUINegative);

                // Play the ominous ticking sound if we're at 10 seconds or less.
                if( (9 >= seconds) && 
                    (seconds != sLastTick))
                {
                    RULE.Execute(shRulePlaySound, DataObject(SOUND_LOW_ON_TIME));
                    sLastTick = seconds;
                }
            }
            else if( (0 == minutes) && (30 >= seconds) )
            {
                spTimerText->SetTextColor(shPlayerUIWarning);
            }
            else
            {
                spTimerText->SetTextColor(shPlayerUIPositive);
            }
			
			spTimerText->SetLabel(wxString::Format(swMinutesSeconds.c_str(), minutes, seconds));
		}
	}
}

bool wxOnlineUI::ResetCountdown(const DataObject& object)
{
    sCountdownTime = object.read<wxInt32>();

    // See if we're no longer in countdown.
    if(-1 == sCountdownTime)
    {
        OnShutdownUI();
        return false;
    }

    Freeze();

    if(true == sInTurnCountdown)
    {
        wxInt32 extraTimers = object.read<wxInt32>(1);
        wxInt32 timerStep = object.read<wxInt32>(2);

        spTurnTimersLeft->SetLabel(stTurnTimersLeft + swColon);
        spTurnTimersLeftText->SetLabel(wxString::Format(swIntFormat.c_str(), 
            extraTimers));
        spMultiplier->SetLabel(stMultiplier + swColon);
        spMultiplierText->SetLabel(wxString::Format(swXIntFormat.c_str(), 
            timerStep));
        spPassiveQuitText->SetLabel(swSpace);
        Layout();
        
        // Set colors.
        if(1 == timerStep)
        {
            spMultiplierText->SetTextColor(shPlayerUIPositive);
        }
        else if(2 == timerStep)
        {
            spMultiplierText->SetTextColor(shPlayerUIWarning);
        }
        else
        {
            spMultiplierText->SetTextColor(shPlayerUINegative);
        }

        if(1 >= extraTimers)
        {
            spTurnTimersLeftText->SetTextColor(shPlayerUINegative);
        }
        else if(4 >= extraTimers)
        {
            spTurnTimersLeftText->SetTextColor(shPlayerUIWarning);
        }
        else
        {
            spTurnTimersLeftText->SetTextColor(shPlayerUIPositive);
        }
    }
    else
    {
        wxASSERT(true == sInPassiveCountdown);

        spTurnTimersLeft->SetLabel(swSpace);
        spTurnTimersLeftText->SetLabel(swSpace);
        spMultiplier->SetLabel(swSpace);
        spMultiplierText->SetLabel(swSpace);
        spPassiveQuitText->SetLabel(stActToAvoid);
        Layout();
    }

    Thaw();

    return true;
}