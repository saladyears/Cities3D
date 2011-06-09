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
#include "CommercialHarborCommodityDialog.h"
#include "SelectionLogic.h"
#include "InteractiveImageStrip.h"
#include "BaseButton.h"
#include "BaseStaticText.h"
#include "Game.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	class LogicCommercialHarborCommodity : public SelectionLogic
	{
	public:
		LogicCommercialHarborCommodity(boost::weak_ptr<Game> game) 
			: SelectionLogic(game) {}

		virtual void Initialize()
		{
			// They are only allowed one selection.
			wxInteractiveImageStrip* selection = GetSelection();
			selection->SetMaxImages(1);
			
			wxBaseStaticText* text = GetText();

			const GamePtr& game = GetGame();
			wxInt32 index = game->read<wxInt32>(shCurrentPlayer);
			const wxString& name = game->playerGame(index).player().Name();

			wxString str = wxString::Format(stSelectOneCommodityToGive.c_str(), 
                name.c_str());
			text->SetLabel(str);

			// Fill the pool with only what the player has.
			index = game->read<wxInt32>(shCommercialHarborVictim);

			wxInteractiveImageStrip* pool = GetResourcePool();
			pool->SetFlags(wxInteractiveImageStrip::FlagHideZeroes);

			// We only want commodities in the pool.
			pool->SetMaxImages(3);
			pool->Add(shCloth, 1);
			pool->Add(shCoin, 1);
			pool->Add(shPaper, 1);

			Data::IntHash resources = 
				game->playerGame(index).read<Data::IntHash>(shResources);
			
			SetCount(shCloth, resources[shCloth]);
			SetCount(shCoin, resources[shCoin]);
			SetCount(shPaper, resources[shPaper]);
		}

		virtual bool AllowDrop(wxInteractiveImageStrip*, wxCoord, wxCoord, 
			const HashString&)
		{
			// They can always drag and drop between the pool and selection.
			return true;
		}

		virtual void ExecuteDrop(wxInteractiveImageStrip* target, wxCoord, 
			wxCoord, const HashString& res)
		{
			wxInteractiveImageStrip* selection = GetSelection();
			wxInteractiveImageStrip* pool = GetResourcePool();
			wxBaseButton* button = GetButton();

			// If the target is the pool, just remove it from the selection
			// and disable the button.
			if(target == pool)
			{
				selection->Remove(res);

				wxInt32 count = pool->GetResourceCount(res);
				pool->SetResourceCount(res, count + 1);

				button->Disable();
			}
			// If the target is the selection, remove any current resource
			// and add this one.
			else if(target == selection)
			{
				selection->Clear();
				selection->Add(res, 1);

				wxInt32 count = pool->GetResourceCount(res);
				pool->SetResourceCount(res, count - 1);

				button->Enable();
			}
		}

		virtual void HandleLeftClick(wxInteractiveImageStrip* origin,
			const HashString& res)
		{
			// The target is the opposite strip.
			wxInteractiveImageStrip* selection = GetSelection();
			wxInteractiveImageStrip* pool = GetResourcePool();

			wxInteractiveImageStrip* target = (origin == selection) ?
				pool : selection;

			// Just use drop logic.
			ExecuteDrop(target, 0, 0, res);
		}

		virtual void HandleRightClick(wxInteractiveImageStrip*, 
			const HashString&)
		{
			// Right double-click does nothing.
		}

	private:
		void SetCount(const HashString& res, wxInt32 amount)
		{
			wxInteractiveImageStrip* pool = GetResourcePool();

			if(0 < amount)
			{
				pool->SetResourceCount(res, amount);
			}
			else
			{
				pool->Remove(res, true);
			}
		}
	};
}

//---------------------------- PUBLIC           -----------------------------//
wxCommercialHarborCommodityDialog::wxCommercialHarborCommodityDialog(
	wxWindow *parent, boost::weak_ptr<Game> game)
: wxResourceSelectionDialog(parent, game, SelectionLogicPtr(new LogicCommercialHarborCommodity(game)),
	shRuleSelectCommercialHarborCommodity, stCommercialHarbor)
{
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
