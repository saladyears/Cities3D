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
#include "GameEngine.h"
#include "Controller.h"
#include "IPluginEngine.h"

#include "RollsUI.h"
#include "UpgradeUI.h"
#include "DataObject.h"
#include "Game.h"
#include "AqueductDialog.h"
#include "SelectProgressCardsDialog.h"
#include "DiscardProgressCardsDialog.h"
#include "PlayProgressCardsDialog.h"
#include "ResourceMonopolyDialog.h"
#include "TradeMonopolyDialog.h"
#include "MerchantFleetDialog.h"
#include "MasterMerchantPlayerDialog.h"
#include "MasterMerchantResourceDialog.h"
#include "CommercialHarborPlayerDialog.h"
#include "CommercialHarborResourceDialog.h"
#include "CommercialHarborCommodityDialog.h"
#include "LoseCardsDialog.h"
#include "WeddingDialog.h"
#include "SpyPlayerDialog.h"
#include "StealProgressCardsDialog.h"
#include "DeserterPlayerDialog.h"
#include "AlchemistDialog.h"
#include "SettlersGLCanvas.h"
#include "BarbarianUI.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
class CitiesUI : public UIPlugin
{
public:
	CitiesUI()
		: mCardsDialog(NULL)
	{
		Controller::get().AddReceiver(shEventAqueduct, &CitiesUI::OnAqueduct, 
			this);
		Controller::get().AddReceiver(shEventProgressCards, 
			&CitiesUI::OnProgressCards, this);
		Controller::get().AddReceiver(shEventDiscardProgressCards, 
			&CitiesUI::OnDiscardProgressCards, this);

		Controller::get().AddReceiver(shEventCards, 
			&CitiesUI::OnCards, this);
		Controller::get().AddReceiver(shEventShutdownUI, 
			&CitiesUI::OnShutdownUI, this);

		// Trade cards.
		Controller::get().AddReceiver(shEventResourceMonopoly, 
			&CitiesUI::OnResourceMonopoly, this);
		Controller::get().AddReceiver(shEventTradeMonopoly, 
			&CitiesUI::OnTradeMonopoly, this);
		Controller::get().AddReceiver(shEventMerchantFleet, 
			&CitiesUI::OnMerchantFleet, this);
		Controller::get().AddReceiver(shEventMasterMerchantPlayer,
			&CitiesUI::OnMasterMerchantPlayer, this);
		Controller::get().AddReceiver(shEventMasterMerchantResource,
			&CitiesUI::OnMasterMerchantResource, this);
		Controller::get().AddReceiver(shEventCommercialHarborPlayer,
			&CitiesUI::OnCommercialHarborPlayer, this);
		Controller::get().AddReceiver(shEventCommercialHarborResource,
			&CitiesUI::OnCommercialHarborResource, this);
		Controller::get().AddReceiver(shEventCommercialHarborCommodity,
			&CitiesUI::OnCommercialHarborCommodity, this);

		// Politics cards.
		Controller::get().AddReceiver(shEventArsonist, 
			&CitiesUI::OnArsonist, this);
		Controller::get().AddReceiver(shEventWedding, 
			&CitiesUI::OnWedding, this);
		Controller::get().AddReceiver(shEventSpyPlayer, 
			&CitiesUI::OnSpyPlayer, this);
		Controller::get().AddReceiver(shEventSpyCard, 
			&CitiesUI::OnSpyCard, this);
		Controller::get().AddReceiver(shEventDeserterPlayer, 
			&CitiesUI::OnDeserterPlayer, this);

		// Science cards.
		Controller::get().AddReceiver(shEventAlchemist, 
			&CitiesUI::OnAlchemist, this);
	}

	~CitiesUI()
	{
		Controller::get().RemoveReceiver(shEventAqueduct, 
			&CitiesUI::OnAqueduct, this);
		Controller::get().RemoveReceiver(shEventProgressCards, 
			&CitiesUI::OnProgressCards, this);
		Controller::get().RemoveReceiver(shEventDiscardProgressCards, 
			&CitiesUI::OnDiscardProgressCards, this);

		Controller::get().RemoveReceiver(shEventCards, 
			&CitiesUI::OnCards, this);
		Controller::get().RemoveReceiver(shEventShutdownUI, 
			&CitiesUI::OnShutdownUI, this);

		Controller::get().RemoveReceiver(shEventResourceMonopoly, 
			&CitiesUI::OnResourceMonopoly, this);
		Controller::get().RemoveReceiver(shEventTradeMonopoly, 
			&CitiesUI::OnTradeMonopoly, this);
		Controller::get().RemoveReceiver(shEventMerchantFleet, 
			&CitiesUI::OnMerchantFleet, this);
		Controller::get().RemoveReceiver(shEventMasterMerchantPlayer,
			&CitiesUI::OnMasterMerchantPlayer, this);
		Controller::get().RemoveReceiver(shEventMasterMerchantResource,
			&CitiesUI::OnMasterMerchantResource, this);
		Controller::get().RemoveReceiver(shEventCommercialHarborPlayer,
			&CitiesUI::OnCommercialHarborPlayer, this);
		Controller::get().RemoveReceiver(shEventCommercialHarborResource,
			&CitiesUI::OnCommercialHarborResource, this);
		Controller::get().RemoveReceiver(shEventCommercialHarborCommodity,
			&CitiesUI::OnCommercialHarborCommodity, this);

		Controller::get().RemoveReceiver(shEventArsonist, 
			&CitiesUI::OnArsonist, this);
		Controller::get().RemoveReceiver(shEventWedding, 
			&CitiesUI::OnWedding, this);
		Controller::get().RemoveReceiver(shEventSpyPlayer, 
			&CitiesUI::OnSpyPlayer, this);
		Controller::get().RemoveReceiver(shEventSpyCard, 
			&CitiesUI::OnSpyCard, this);
		Controller::get().RemoveReceiver(shEventDeserterPlayer, 
			&CitiesUI::OnDeserterPlayer, this);

		Controller::get().RemoveReceiver(shEventAlchemist, 
			&CitiesUI::OnAlchemist, this);
	}

	virtual bool ProvidesUI() const
	{
		return true;
	}

	virtual void CreateUI(const GamePtr& game, wxAuiManager& manager)
	{
		wxWindow* frame = GetMainFrame();

		// Tell the main frame to create a new side pane for us.
		Controller::get().Transmit(shEventNewSidePane, 
			DataObject(stCitiesAndKnights, swCities));

		// Dice rolls.
		wxRollsUI* rolls = new wxRollsUI(frame, game, true);

		// Barbarians.
		wxBarbarianUI* barbarians = new wxBarbarianUI(frame, game);

		// City Upgrades.
		wxUpgradeUI* upgrades = new wxUpgradeUI(frame, game);

		Controller::get().Transmit(shEventAddSidePane,
			DataObject(static_cast<wxWindow*>(rolls), stResources));

		Controller::get().Transmit(shEventAddSidePane,
			DataObject(static_cast<wxWindow*>(barbarians), stBarbarians));

		Controller::get().Transmit(shEventAddSidePane,
			DataObject(static_cast<wxWindow*>(upgrades), stCityUpgrades));
	}
private:
	void OnAqueduct(const DataObject& object)
	{
		wxInt32 player = object.read<wxInt32>();

		DataObject input(player), output;
		RULE.Decide(shLogicIsThisPlayer, input, output);

		if(true == output.read<bool>())
		{
			const GamePtr& game = object.read<GamePtr>(1);
			wxAqueductDialog* dialog = new wxAqueductDialog(GetGLCanvas(), 
				player, game);
			dialog->Show();
		}
	}

	void OnProgressCards(const DataObject& object)
	{
		wxInt32 player = object.read<wxInt32>();

		DataObject input(player), output;
		RULE.Decide(shLogicIsThisPlayer, input, output);

		if(true == output.read<bool>())
		{
			const GamePtr& game = object.read<GamePtr>(1);
			wxSelectProgressCardsDialog* dialog = 
				new wxSelectProgressCardsDialog(GetGLCanvas(), player, game);
			dialog->Show();
		}
	}

	void OnDiscardProgressCards(const DataObject& object)
	{
		wxInt32 player = object.read<wxInt32>();

		DataObject input(player), output;
		RULE.Decide(shLogicIsThisPlayer, input, output);

		if(true == output.read<bool>())
		{
			const GamePtr& game = object.read<GamePtr>(1);
			wxDiscardProgressCardsDialog* dialog = 
				new wxDiscardProgressCardsDialog(GetGLCanvas(), game, 
				player);
			dialog->Show();
		}
	}

	void OnCards(const DataObject& object)
	{
		if(true == PLUGIN.IsLoaded(swCitiesRules, CITIES))
		{
			wxASSERT(2 <= object.numItems());
			const GamePtr& game = object.read<GamePtr>();
			bool show = object.read<bool>(1);

			if( (NULL == mCardsDialog) &&
				(true == show))
			{
				mCardsDialog = new wxPlayProgressCardsDialog(
					GetGLCanvas(), game);
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

	void OnResourceMonopoly(const GamePtr& game)
	{
		DataObject input(-1), output;
		RULE.Decide(shLogicIsThisPlayer, input, output);

		if(true == output.read<bool>())
		{
			wxResourceMonopolyDialog *dialog = new wxResourceMonopolyDialog(
				GetGLCanvas(), game);
			dialog->Show();
		}
	}

	void OnTradeMonopoly(const GamePtr& game)
	{
		DataObject input(-1), output;
		RULE.Decide(shLogicIsThisPlayer, input, output);

		if(true == output.read<bool>())
		{
			wxTradeMonopolyDialog *dialog = new wxTradeMonopolyDialog(
				GetGLCanvas(), game);
			dialog->Show();
		}
	}

	void OnMerchantFleet(const GamePtr& game)
	{
		DataObject input(-1), output;
		RULE.Decide(shLogicIsThisPlayer, input, output);

		if(true == output.read<bool>())
		{
			wxMerchantFleetDialog *dialog = new wxMerchantFleetDialog(
				GetGLCanvas(), game);
			dialog->Show();
		}
	}

	void OnMasterMerchantPlayer(const GamePtr& game)
	{
		DataObject input(-1), output;
		RULE.Decide(shLogicIsThisPlayer, input, output);

		if(true == output.read<bool>())
		{
			wxMasterMerchantPlayerDialog *dialog = 
				new wxMasterMerchantPlayerDialog(GetGLCanvas(), game);
			dialog->Show();
		}
	}

	void OnMasterMerchantResource(const GamePtr& game)
	{
		DataObject input(-1), output;
		RULE.Decide(shLogicIsThisPlayer, input, output);

		if(true == output.read<bool>())
		{
			wxMasterMerchantResourceDialog *dialog = 
				new wxMasterMerchantResourceDialog(GetGLCanvas(), game);
			dialog->Show();
		}
	}

	void OnCommercialHarborPlayer(const GamePtr& game)
	{
		DataObject input(-1), output;
		RULE.Decide(shLogicIsThisPlayer, input, output);

		if(true == output.read<bool>())
		{
			wxCommercialHarborPlayerDialog *dialog = 
				new wxCommercialHarborPlayerDialog(GetGLCanvas(), game);
			dialog->Show();
		}
	}

	void OnCommercialHarborResource(const GamePtr& game)
	{
		DataObject input(-1), output;
		RULE.Decide(shLogicIsThisPlayer, input, output);

		if(true == output.read<bool>())
		{
			wxCommercialHarborResourceDialog *dialog = 
				new wxCommercialHarborResourceDialog(GetGLCanvas(), game);
			dialog->Show();
		}
	}

	void OnCommercialHarborCommodity(const GamePtr& game)
	{
		wxInt32 index = game->read<wxInt32>(shCommercialHarborVictim);

		DataObject input(index), output;
		RULE.Decide(shLogicIsThisPlayer, input, output);

		if(true == output.read<bool>())
		{
			wxCommercialHarborCommodityDialog *dialog = 
				new wxCommercialHarborCommodityDialog(GetGLCanvas(), game);
			dialog->Show();
		}
	}

	void OnArsonist(const DataObject& object)
	{
		wxInt32 player = object.read<wxInt32>();

		DataObject input(player), output;
		RULE.Decide(shLogicIsThisPlayer, input, output);

		if(true == output.read<bool>())
		{
			const GamePtr& game = object.read<GamePtr>(1);
			wxLoseCardsDialog *dialog = 
				new wxLoseCardsDialog(GetGLCanvas(), player, game,
				shRuleExecuteArsonist);
			dialog->Show();
		}
	}

	void OnWedding(const DataObject& object)
	{
		wxInt32 player = object.read<wxInt32>();

		DataObject input(player), output;
		RULE.Decide(shLogicIsThisPlayer, input, output);

		if(true == output.read<bool>())
		{
			const GamePtr& game = object.read<GamePtr>(1);
			wxWeddingDialog *dialog = 
				new wxWeddingDialog(GetGLCanvas(), player, game);
			dialog->Show();
		}
	}

	void OnSpyPlayer(const GamePtr& game)
	{
		DataObject input(-1), output;
		RULE.Decide(shLogicIsThisPlayer, input, output);

		if(true == output.read<bool>())
		{
			wxSpyPlayerDialog *dialog = 
				new wxSpyPlayerDialog(GetGLCanvas(), game);
			dialog->Show();
		}
	}

	void OnSpyCard(const GamePtr& game)
	{
		DataObject input(-1), output;
		RULE.Decide(shLogicIsThisPlayer, input, output);

		if(true == output.read<bool>())
		{
			wxInt32 victim = game->read<wxInt32>(shSpyVictim);
			wxStealProgressCardsDialog* dialog = 
				new wxStealProgressCardsDialog(GetGLCanvas(), game, victim);
			dialog->Show();
		}
	}

	void OnDeserterPlayer(const GamePtr& game)
	{
		DataObject input(-1), output;
		RULE.Decide(shLogicIsThisPlayer, input, output);

		if(true == output.read<bool>())
		{
			wxDeserterPlayerDialog *dialog = 
				new wxDeserterPlayerDialog(GetGLCanvas(), game);
			dialog->Show();
		}
	}

	void OnAlchemist(wxInt32)
	{
		DataObject input(-1), output;
		RULE.Decide(shLogicIsThisPlayer, input, output);

		if(true == output.read<bool>())
		{
			wxAlchemistDialog* dialog = new wxAlchemistDialog(GetGLCanvas());
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

IMPLEMENT_UI_PLUGIN(CITIES, CitiesUI)

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
