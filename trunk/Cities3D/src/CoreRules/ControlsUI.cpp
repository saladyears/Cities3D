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
#include "ControlsUI.h"
#include "DataObject.h"
#include "Game.h"
#include "Controller.h"
#include "GameEngine.h"
#include "BaseButton.h"
#include "SkinManager.h"
#include "ConfigDatabase.h"
#include "GLExport.h"
#include "TimerManager.h"
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
		ID_UNDO_BUTTON	= 0,
	};

	const int NUM_BUTTONS_CONTROLS = 6;

	const wxSize sMinButtonSize = wxSize(75, 20);
	const wxSize sMinSizeControls = wxSize(((sMinButtonSize.x + 1) * NUM_BUTTONS_CONTROLS) + 1, 
		sMinButtonSize.y + 1);

	struct no_delete_controls
	{
		void operator()(void *){}
	};
}

//---------------------------- PUBLIC           -----------------------------//
wxControlsUI::wxControlsUI(wxWindow *parent)
: wxBaseUI<wxPanel>(parent, sMinSizeControls)
{
	wxSizer* horz = new wxBoxSizer(wxHORIZONTAL);

	//create the buttons
	mButtons.resize(NUM_BUTTONS_CONTROLS);

	wxColour background = SKIN.Element(shBackground);
	SetBackgroundColour(background);

	for(wxInt32 i = 0; i < NUM_BUTTONS_CONTROLS; i++)
	{
		wxString caption;
		switch(i)
		{
		case 0: caption = stUndo; break;
		case 1: caption = stTrade; break;
		case 2: caption = stBank; break;
		case 3: caption = stCards; break;
        case 4: caption = stRollDice; break;
		case 5: caption = stEndTurn; break;
		default: wxASSERT(false); break;
		}

		wxBaseButton* button = new wxBaseButton(this, ID_UNDO_BUTTON + i, 
			caption);
		button->SetSize(sMinButtonSize);
		button->SetMinSize(sMinButtonSize);
		button->Enable(FALSE);
		button->Show();

		mButtons[i] = boost::shared_ptr<wxBaseButton>(button, no_delete_controls());	
		
		horz->Add(button, 0, wxTOP | wxBOTTOM | wxLEFT, 1);
	}

	SetSizer(horz);
	horz->SetSizeHints(this);
	horz->Layout();

	SetSize(sMinSizeControls);
	SetMinSize(sMinSizeControls);

	// Register event handlers.
	Controller::get().AddReceiver(shEventControlsUI, 
		&wxControlsUI::OnUpdate, this);
	Controller::get().AddReceiver(shEventUpdateUndo, 
		&wxControlsUI::OnUpdateUndo, this);
	Controller::get().AddReceiver(shEventShutdownUI, &wxControlsUI::OnShutdownUI,
		this);
	Controller::get().AddReceiver(shEventPreRollCard, 
		&wxControlsUI::OnPreRollCard, this);
	
	// KeyMap events.
	Controller::get().AddReceiver(shEventControlsUndo, 
		&wxControlsUI::OnControlsUndo, this);
	Controller::get().AddReceiver(shEventControlsTrade, 
		&wxControlsUI::OnControlsTrade, this);
	Controller::get().AddReceiver(shEventControlsBank, 
		&wxControlsUI::OnControlsBank, this);
	Controller::get().AddReceiver(shEventControlsPlayCard, 
		&wxControlsUI::OnControlsPlayCard, this);
	Controller::get().AddReceiver(shEventControlsRollDice, 
		&wxControlsUI::OnControlsRollDice, this);
	Controller::get().AddReceiver(shEventControlsEndTurn, 
		&wxControlsUI::OnControlsEndTurn, this);
}

wxControlsUI::~wxControlsUI()
{
	//remove event handler
	Controller::get().RemoveReceiver(shEventControlsUI, 
		&wxControlsUI::OnUpdate, this);
	Controller::get().RemoveReceiver(shEventUpdateUndo, 
		&wxControlsUI::OnUpdateUndo, this);
	Controller::get().RemoveReceiver(shEventShutdownUI, 
		&wxControlsUI::OnShutdownUI, this);
	Controller::get().RemoveReceiver(shEventPreRollCard, 
		&wxControlsUI::OnPreRollCard, this);

	// KeyMap events.
	Controller::get().RemoveReceiver(shEventControlsUndo, 
		&wxControlsUI::OnControlsUndo, this);
	Controller::get().RemoveReceiver(shEventControlsTrade, 
		&wxControlsUI::OnControlsTrade, this);
	Controller::get().RemoveReceiver(shEventControlsBank, 
		&wxControlsUI::OnControlsBank, this);
	Controller::get().RemoveReceiver(shEventControlsPlayCard, 
		&wxControlsUI::OnControlsPlayCard, this);
	Controller::get().RemoveReceiver(shEventControlsRollDice, 
		&wxControlsUI::OnControlsRollDice, this);
	Controller::get().RemoveReceiver(shEventControlsEndTurn, 
		&wxControlsUI::OnControlsEndTurn, this);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxControlsUI, wxWindow)
	EVT_COMMAND_RANGE(ID_UNDO_BUTTON, ID_UNDO_BUTTON + NUM_BUTTONS_CONTROLS, 
	wxEVT_COMMAND_BUTTON_CLICKED, wxControlsUI::OnButton)
END_EVENT_TABLE()

void wxControlsUI::OnButton(wxCommandEvent &event)
{
	HashString rule;
    DataObject object;

	switch(event.GetId())
	{
	case (ID_UNDO_BUTTON + 0):
		rule = shRuleUndo;
		break;
	//play card button
	case (ID_UNDO_BUTTON + 1):
		rule = shRuleTradeOthers;
		break;
	//bank button
	case (ID_UNDO_BUTTON + 2):
		rule = shRuleTradeBank;
		break;
	//trade button
	case (ID_UNDO_BUTTON + 3):
        {
            // Also check to see if it's even our turn.
	        DataObject input(-1), output;
	        RULE.Decide(shLogicIsThisPlayer, input, output);

            wxInt32 playerIndex = output.read<wxInt32>(1);
            object = DataObject(playerIndex);

		    rule = shRuleShowCards;
        }
		break;
	//roll dice button
	case (ID_UNDO_BUTTON + 4):
        OnPreRollCard(false);
	    rule = shRuleRollDice;
        break;
    case (ID_UNDO_BUTTON + 5):
		rule = shRuleMarkerEndTurn;

		// Disable the button so we don't get a double-click.
		mButtons[5]->Disable();
		break;
	default:
		wxASSERT(0);
		break;
	}

	RULE.Execute(rule, object);
}

void wxControlsUI::OnUpdate(const DataObject &object)
{
	GetSizer()->Layout();

	wxASSERT(1 == object.numItems());

	bool enable = object.read<bool>();

	// Also check to see if it's even our turn.
	DataObject input(-1), output;
	RULE.Decide(shLogicIsThisPlayer, input, output);

    bool thisPlayer = output.read<bool>();
    wxInt32 playerIndex = output.read<wxInt32>(1);
    input = DataObject(playerIndex);
	
	//undo button
	mButtons[0]->Enable(enable && thisPlayer && RULE.CanUndo());
	
	//play button
	output.reset();
	RULE.Decide(shLogicCanTradeOthers, input, output);
	mButtons[1]->Enable(enable && thisPlayer && output.read<bool>());

	//trade bank button
	output.reset();
	RULE.Decide(shLogicCanTradeBank, input, output);
	mButtons[2]->Enable(enable && thisPlayer && output.read<bool>());

	//show card button, enabled even if it's not their turn
	output.reset();
	RULE.Decide(shLogicCanShowCard, input, output);
	mButtons[3]->Enable(output.read<bool>());

	//end turn button
	output.reset();
	RULE.Decide(shLogicCanEndTurn, input, output);
	mButtons[5]->Enable(enable && thisPlayer && output.read<bool>());
}

void wxControlsUI::OnUpdateUndo(wxInt32)
{
	bool canUndo = false;

	// Ensure that it is even our turn.
	DataObject input(-1), output;
	RULE.Decide(shLogicIsThisPlayer, input, output);

	if(true == output.read<bool>())
	{
		canUndo = RULE.CanUndo();
	}

	mButtons[0]->Enable(canUndo);
}

void wxControlsUI::OnShutdownUI(wxInt32)
{
	std::for_each(mButtons.begin(), mButtons.end(), 
		boost::bind(&wxBaseButton::Enable, _1, false));

	Refresh();
}

void wxControlsUI::OnPreRollCard(bool show)
{
	// Ensure that it is even our turn.
	DataObject input(-1), output;
	RULE.Decide(shLogicIsThisPlayer, input, output);

	if(true == output.read<bool>())
	{
		if(true == show)
		{
            wxInt32 playerIndex = output.read<wxInt32>(1);

			std::for_each(mButtons.begin(), mButtons.end(), 
				boost::bind(&wxBaseButton::Enable, _1, false));

			DataObject input(playerIndex), output;
			RULE.Decide(shLogicCanShowCard, input, output);
			mButtons[3]->Enable(output.read<bool>());

			output.reset();
			RULE.Decide(shLogicRollText, input, output);
			const wxString& text = output.read<wxString>();

			mButtons[4]->Enable(true);
			mButtons[4]->SetLabel(text);
		}
        else
        {
            mButtons[4]->Enable(false);
            mButtons[4]->SetLabel(stRollDice);
        }

		Refresh();
	}
}

void wxControlsUI::OnControlsUndo(wxInt32)
{
	if(true == mButtons[0]->IsEnabled())
	{
		wxCommandEvent event;
		event.SetId(ID_UNDO_BUTTON);
		OnButton(event);
	}
}

void wxControlsUI::OnControlsTrade(wxInt32)
{
	if(true == mButtons[1]->IsEnabled())
	{
		wxCommandEvent event;
		event.SetId(ID_UNDO_BUTTON + 1);
		OnButton(event);
	}
}

void wxControlsUI::OnControlsBank(wxInt32)
{
	if(true == mButtons[2]->IsEnabled())
	{
		wxCommandEvent event;
		event.SetId(ID_UNDO_BUTTON + 2);
		OnButton(event);
	}
}

void wxControlsUI::OnControlsPlayCard(wxInt32)
{
	if(true == mButtons[3]->IsEnabled())
	{
		wxCommandEvent event;
		event.SetId(ID_UNDO_BUTTON + 3);
		OnButton(event);
	}
}

void wxControlsUI::OnControlsRollDice(wxInt32)
{
	if(true == mButtons[4]->IsEnabled())
	{
		wxCommandEvent event;
		event.SetId(ID_UNDO_BUTTON + 4);
		OnButton(event);
	}
}

void wxControlsUI::OnControlsEndTurn(wxInt32)
{
	if(true == mButtons[5]->IsEnabled())
	{
		wxCommandEvent event;
		event.SetId(ID_UNDO_BUTTON + 5);
		OnButton(event);
	}
}
