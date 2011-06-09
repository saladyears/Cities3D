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
#include "TimerManager.h"
#include "ConfigDatabase.h"
#include "IRuleEngine.h"

#include "BankTradeDialog.h"
#include "OthersTradeDialog.h"
#include "StockyardDialog.h"
#include "SettlersView.h"
#include "BaseMessageBox.h"

#include "MessageUI.h"
#include "ControlsUI.h"
#include "BuildUIPanel.h"
#include "PlayerUIPanel.h"
#include "BankUI.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
class CoreUI : public UIPlugin
{
public:
	CoreUI()
		: mBankTradeDialog(NULL)
		, mOthersTradeDialog(NULL)
		, mStockyardDialog(NULL)
		, mSettlersView(NULL)
	{
		Controller::get().AddReceiver(shEventBankTrade, &CoreUI::OnBankTrade, 
			this);
		Controller::get().AddReceiver(shEventOthersTrade, 
			&CoreUI::OnOthersTrade, this);
		Controller::get().AddReceiver(shEventShowResources,	
			&CoreUI::OnShowResources, this);
		Controller::get().AddReceiver(shEventShutdownModelessUI, 
			&CoreUI::OnShutdownModelessUI, this);
		Controller::get().AddReceiver(shEventShutdownUI, 
			&CoreUI::OnShutdownUI, this);
		Controller::get().AddReceiver(shEventCreateGameWindow, 
			&CoreUI::OnCreateGameWindow, this);
		Controller::get().AddReceiver(shEventCloseGameWindow, 
			&CoreUI::OnCloseGameWindow, this);
		Controller::get().AddReceiver(shEventMessageBox,
			&CoreUI::OnMessageBox, this);
	}

	~CoreUI()
	{
		Controller::get().RemoveReceiver(shEventBankTrade,
			&CoreUI::OnBankTrade, this);
		Controller::get().RemoveReceiver(shEventOthersTrade,
			&CoreUI::OnOthersTrade, this);
		Controller::get().RemoveReceiver(shEventShowResources,
			&CoreUI::OnShowResources, this);
		Controller::get().RemoveReceiver(shEventShutdownModelessUI,
			&CoreUI::OnShutdownModelessUI, this);
		Controller::get().RemoveReceiver(shEventShutdownUI,
			&CoreUI::OnShutdownUI, this);
		Controller::get().RemoveReceiver(shEventCreateGameWindow, 
			&CoreUI::OnCreateGameWindow, this);	
		Controller::get().RemoveReceiver(shEventCloseGameWindow, 
			&CoreUI::OnCloseGameWindow, this);	
		Controller::get().RemoveReceiver(shEventMessageBox,
			&CoreUI::OnMessageBox, this);

		// Always remove any existing timer.
		TIME.RemoveOneShotFunction(&CoreUI::OnHideResources, this, 
			5000);
	}

	virtual bool ProvidesUI() const
	{
		return true;
	}

	virtual void CreateUI(const GamePtr& game, wxAuiManager& manager)
	{
		wxWindow* frame = GetMainFrame();
		wxSettlersGLCanvas* canvas = GetGLCanvas();

		// Create all of the UI common to a game.  Dice rolls are created in
		// DevCardsUI so that they do not clash with CitiesUI since they are
		// mutually exclusive.

		// Status window.
		wxMessageUI* message = new wxMessageUI(frame);
		wxSize minSize = message->GetMinSize();
		
		manager.AddPane(message, wxAuiPaneInfo().Caption(stStatus)
			.MinSize(minSize).CloseButton(false).MaximizeButton(false)
			.Dockable(true).Bottom().Name(swStatus));

		// Controls.
		wxControlsUI* controls = new wxControlsUI(frame);
		minSize = controls->GetMinSize();

		manager.AddPane(controls, wxAuiPaneInfo().Caption(stControls)
			.MinSize(minSize).CloseButton(false).MaximizeButton(false)
			.Dockable(true).Bottom().Name(swControls));

		// Build options.
		wxBuildUIPanel* panel = new wxBuildUIPanel(frame, canvas);
		minSize = panel->GetMinSize();

		manager.AddPane(panel, wxAuiPaneInfo().Caption(stBuildOptions)
			.MinSize(minSize).CloseButton(false).MaximizeButton(false)
			.Dockable(true).Top().Name(swBuildOptions));

		// Players.
		wxPlayerUIPanel* players = new wxPlayerUIPanel(frame, canvas, game);
		minSize = players->GetMinSize();

		manager.AddPane(players, wxAuiPaneInfo().Caption(stPlayers)
			.MinSize(minSize).CloseButton(false).MaximizeButton(false)
			.Dockable(true).Left().Name(swPlayers));

		// Bank.
		wxBankUI* bank = new wxBankUI(frame, game, canvas);
		minSize = bank->GetMinSize();

		manager.AddPane(bank, wxAuiPaneInfo().Caption(stBank)
			.MinSize(minSize).CloseButton(false).MaximizeButton(false)
			.Dockable(true).Left().Name(swBank));
	}

private:
	void OnBankTrade(const GamePtr& game)
	{
		DataObject input(-1), output;
		RULE.Decide(shLogicIsThisPlayer, input, output);

		if( (NULL == mBankTradeDialog) &&
			(true == output.read<bool>())
			)
		{
			mBankTradeDialog = new wxBankTradeDialog(GetGLCanvas(), game);
			mBankTradeDialog->Show();
		}
		else
		{
			OnShutdownModelessUI();
		}
	}

	void OnOthersTrade(const DataObject& object)
	{
		const GamePtr& game = object.read<GamePtr>();
		wxInt32 index = object.read<wxInt32>(1);
		
		DataObject input(-1), output;
		RULE.Decide(shLogicIsThisPlayer, input, output);
		bool thisPlayer = output.read<bool>();
		wxInt32 player = output.read<wxInt32>(1);

		// If this is a click from the ControlsUI (index = -1), then we either
		// launch the windows or close them.  We also allow spectators.
		if( (NULL == mOthersTradeDialog) &&
			((true == thisPlayer) ||
			(-1 != index)))
		{
			mOthersTradeDialog = new wxOthersTradeDialog(GetGLCanvas(), 
				game, player);
			
			// If this is the first time it's ever been run, make sure the
			// stockyard window doesn't overlap the trade window.

			wxInt32 x, y;
			x = CONFIG.read(swConfigSectionUI, swTradeWithOthersX, -1);
			y = CONFIG.read(swConfigSectionUI, swTradeWithOthersY, -1);

			mStockyardDialog = new wxStockyardDialog(GetGLCanvas(), 
				game, player);
			
			if( (-1 == x) &&
				(-1 == y))
			{
				wxRect othersRect = mOthersTradeDialog->GetRect();
				wxRect stockRect = mStockyardDialog->GetRect();
				wxSize displaySize = wxGetDisplaySize();

				wxInt32 width = othersRect.width + stockRect.width;

				// Lay them out perfectly centered.
				wxPoint pos = wxPoint((displaySize.x - width) / 2, 
					(displaySize.y - othersRect.height) / 2);
				mOthersTradeDialog->SetPosition(pos);

				pos.x += othersRect.width;
				pos.y = (displaySize.y - stockRect.height) / 2;
				mStockyardDialog->SetPosition(pos);
			}

			mOthersTradeDialog->Show();
		}
		else if((NULL != mOthersTradeDialog) &&
				(-1 == index))
		{
			OnShutdownModelessUI();
		}
	}

	void OnShowResources(const DataObject &data)
	{
		TIME.RemoveOneShotFunction(&CoreUI::OnHideResources, this, 5000);

		wxASSERT(1 <= data.numItems());

		if(true == data.read<bool>())
		{
			wxASSERT(2 <= data.numItems());
			wxInt32 roll = data.read<wxInt32>(1);
			RULE.Execute(shRuleShowResources, DataObject(roll));

			// Start up the timer that will turn the display off.
			if(7 != roll)
			{
				TIME.AddOneShotFunction(&CoreUI::OnHideResources, this, 
					5000);
			}
		}
		else
		{
			// Clear things back to normal.
			RULE.AsynchExecute(shRuleResetTileFlags, DataObject());
			RULE.AsynchExecute(shRuleResetObjectFlags, DataObject());
		}

		Controller::get().Transmit(shEventDirty, 0);
	}

	void OnShutdownModelessUI(wxInt32 = 0)
	{
		if(mBankTradeDialog)
		{
			mBankTradeDialog->Destroy();
			mBankTradeDialog = NULL;
		}
		if(mOthersTradeDialog)
		{
			mOthersTradeDialog->Destroy();
			mOthersTradeDialog = NULL;
		}
		if(mStockyardDialog)
		{
			mStockyardDialog->Destroy();
			mStockyardDialog = NULL;
		}
	}

	void OnShutdownUI(wxInt32)
	{
		// When all UI is shut down, all dialog windows destroy themselves, which
		// causes all our pointers to become invalid, so NULL them out.
		mBankTradeDialog = NULL;
		mOthersTradeDialog = NULL;
		mStockyardDialog = NULL;
	}

	void OnHideResources()
	{
		wxLogDebug(wxT("Timer: OnHideResources"));

		OnShowResources(DataObject(false));
	}

	void OnCreateGameWindow(wxAuiManager* manager)
	{
		wxInt32 depthSize = CONFIG.read<wxInt32>(swConfigSectionOpenGL, 
			swDepthBuffer,	::wxDisplayDepth());

		wxInt32 attrib[] = {
			WX_GL_RGBA,
			WX_GL_DEPTH_SIZE, 
			depthSize,
			WX_GL_DOUBLEBUFFER,
			0,
		};

		wxWindow* window = GetMainFrame();
		wxSize size = window->GetSize();

		mSettlersView = new wxSettlersView(GetMainFrame(), manager, -1, 
			wxDefaultPosition, size, 0, swEmpty, &attrib[0]);
	}

	void OnCloseGameWindow(wxInt32)
	{
		if(NULL != mSettlersView)
		{
			mSettlersView->Destroy();
			mSettlersView = NULL;
		}
	}

	void OnMessageBox(const wxString &str)
	{
		wxBaseMessageBox dialog(GetMainFrame(), swCitiesOnline, str);
		dialog.ShowModal();
	}

	wxBankTradeDialog* mBankTradeDialog;
	wxOthersTradeDialog* mOthersTradeDialog;
	wxStockyardDialog* mStockyardDialog;
	wxSettlersView* mSettlersView;
};

IMPLEMENT_UI_PLUGIN(CORE, CoreUI)

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
