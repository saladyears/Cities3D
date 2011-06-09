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
#include "SelectProgressCardsDialog.h"
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
	class LogicSelectProgressCards : public SelectionLogic
	{
	public:
		LogicSelectProgressCards(wxInt32 index, boost::weak_ptr<Game> game) 
			: SelectionLogic(game) 
			, mIndex(index)
		{}

		virtual void Initialize()
		{
			// They are allowed to get 1 resource.
			wxInteractiveImageStrip* selection = GetSelection();
			selection->SetKey(shCards);
			selection->SetMaxImages(1);
			selection->SetFlags(wxImageStrip::FlagHideText);

			wxBaseStaticText* text = GetText();
			text->SetLabel(stSelectOneProgressCard);

			// The resource pool is full of resources, so get rid of it and
			// add cards.
			wxInteractiveImageStrip* pool = GetResourcePool();
			pool->SetKey(shCards);
			pool->SetMaxImages(3);
			pool->SetFlags(wxImageStrip::FlagHideText);
			pool->Add(shTradeCard, 1);
			pool->Add(shPoliticsCard, 1);
			pool->Add(shScienceCard, 1);
		}

		virtual bool AllowDrop(wxInteractiveImageStrip* target, wxCoord, 
			wxCoord, const HashString& res)
		{
			wxInteractiveImageStrip* selection = GetSelection();
			wxInteractiveImageStrip* pool = GetResourcePool();
			const GamePtr& game = GetGame();

			bool allow = false;

			// They are always allowed to go from selection to pool.
			if(target == pool)
			{
				allow = true;
			}
			// They may go from pool to selection if there are enough cards
			// left in the deck to do so.
			else if(target == selection)
			{
				HashString name = HashString::Format(shBankString.c_str(), 
                    res.c_str());

				const HashStringArray& deck = game->read<HashStringArray>(name);

				wxInt32 targetAmount = selection->GetResourceCount();

				allow = (0 == targetAmount) && (false == deck.empty());
			}

			return allow;
		}

		virtual void ExecuteDrop(wxInteractiveImageStrip* target, wxCoord, 
			wxCoord, const HashString& res)
		{
			wxInteractiveImageStrip* selection = GetSelection();
			wxInteractiveImageStrip* pool = GetResourcePool();

			wxBaseButton* button = GetButton();
			
			bool enable = false;

			// If the pool is the target, just remove the resource from the
			// selection.
			if(target == pool)
			{
				selection->Remove(res);
			}
			// If the selection is the target, increase the count if the
			// resource exists, or add it otherwise.
			else if(target == selection)
			{
				selection->Add(res, 1);
				enable = true;
			}

			button->Enable(enable);
		}

		virtual void HandleLeftClick(wxInteractiveImageStrip* origin,
			const HashString& res)
		{
			// The target is the opposite strip.
			wxInteractiveImageStrip* selection = GetSelection();
			wxInteractiveImageStrip* pool = GetResourcePool();

			wxInteractiveImageStrip* target = (origin == selection) ?
				pool : selection;

			// Just use drop logic if allowed.
			if(true == AllowDrop(target, 0, 0, res))
			{
				ExecuteDrop(target, 0, 0, res);
			}
		}

		virtual void HandleRightClick(wxInteractiveImageStrip*, 
			const HashString&)
		{
			// Right double-click does nothing.
		}

	private:
		wxInt32 mIndex;
	};
}

//---------------------------- PUBLIC           -----------------------------//
wxSelectProgressCardsDialog::wxSelectProgressCardsDialog(wxWindow *parent, 
	wxInt32 index, boost::weak_ptr<Game> game)
: wxResourceSelectionDialog(parent, game, SelectionLogicPtr(
	new LogicSelectProgressCards(index, game)), shRuleExecuteProgressCard, 
	stSelectAProgressCardUI, index)
{
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
