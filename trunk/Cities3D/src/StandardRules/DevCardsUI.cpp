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
#include "UIPlugin.h"
#include "RuleSetDatabase.h"
#include "Controller.h"
#include "GameEngine.h"
#include "DataObject.h"
#include "IRuleEngine.h"
#include "IPluginEngine.h"

#include "RollsUI.h"
#include "DevCardsDialog.h"
#include "MonopolyDialog.h"
#include "YearOfPlentyDialog.h"
#include "SettlersGLCanvas.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
class DevCardsUI : public UIPlugin
{
public:
	DevCardsUI()
		: mCardsDialog(NULL)
	{
		Controller::get().AddReceiver(shEventCards, 
			&DevCardsUI::OnCards, this);
		Controller::get().AddReceiver(shEventMonopoly, 
			&DevCardsUI::OnMonopoly, this);
		Controller::get().AddReceiver(shEventYearOfPlenty, 
			&DevCardsUI::OnYearOfPlenty, this);
		Controller::get().AddReceiver(shEventShutdownUI, 
			&DevCardsUI::OnShutdownUI, this);
	}

	~DevCardsUI()
	{
		Controller::get().RemoveReceiver(shEventCards, 
			&DevCardsUI::OnCards, this);
		Controller::get().RemoveReceiver(shEventMonopoly, 
			&DevCardsUI::OnMonopoly, this);
		Controller::get().RemoveReceiver(shEventYearOfPlenty, 
			&DevCardsUI::OnYearOfPlenty, this);
		Controller::get().RemoveReceiver(shEventShutdownUI, 
			&DevCardsUI::OnShutdownUI, this);
	}

	virtual bool ProvidesUI() const
	{
		return true;
	}

	virtual void CreateUI(const GamePtr& game, wxAuiManager& manager)
	{
		// Tell the main frame to create a new side pane for us.
		Controller::get().Transmit(shEventNewSidePane, 
			DataObject(stDiceRolls, swDiceRolls));

		wxWindow* frame = GetMainFrame();

		// Dice rolls.
		wxRollsUI* rolls = new wxRollsUI(frame, game, false);

		Controller::get().Transmit(shEventAddSidePane,
			DataObject(static_cast<wxWindow*>(rolls), stResources));
	}

private:
	void OnCards(const DataObject& object)
	{
		if(true == PLUGIN.IsLoaded(swStandardRules, DEVCARDS))
		{
			wxASSERT(2 <= object.numItems());
			const GamePtr& game = object.read<GamePtr>();
			bool show = object.read<bool>(1);

			if( (NULL == mCardsDialog) &&
				(true == show))
			{
				mCardsDialog = new wxDevCardsDialog(GetGLCanvas(), game);
				mCardsDialog->Show();
			}
			else if((NULL != mCardsDialog) &&
					(false == show))
			{
				mCardsDialog->Destroy();
				mCardsDialog = NULL;
			}
		}
	}

	void OnMonopoly(const GamePtr& game)
	{
		DataObject input(-1), output;
		RULE.Decide(shLogicIsThisPlayer, input, output);

		if(true == output.read<bool>())
		{
			wxMonopolyDialog *dialog = new wxMonopolyDialog(GetGLCanvas(), 
				game);
			dialog->Show();
		}
	}

	void OnYearOfPlenty(const GamePtr& game)
	{
		DataObject input(-1), output;
		RULE.Decide(shLogicIsThisPlayer, input, output);

		if(true == output.read<bool>())
		{
			wxYearOfPlentyDialog *dialog = new wxYearOfPlentyDialog(
				GetGLCanvas(), game);
			dialog->Show();
		}
	}

	void OnShutdownUI(wxInt32)
	{
		mCardsDialog = NULL;
	}

private:
	wxCardsDialog* mCardsDialog;
};

IMPLEMENT_UI_PLUGIN(DEVCARDS, DevCardsUI)

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
