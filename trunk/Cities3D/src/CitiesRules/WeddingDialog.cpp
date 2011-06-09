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
#include "WeddingDialog.h"
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
	class LogicWedding : public SelectionLogic
	{
	public:
		LogicWedding(wxInt32 index, boost::weak_ptr<Game> game) 
			: SelectionLogic(game) 
			, mIndex(index)
		{
			wxASSERT(-1 != index);
		}

		virtual void Initialize()
		{
			wxInteractiveImageStrip* selection = GetSelection();
			wxInteractiveImageStrip* pool = GetResourcePool();

			// Hide zero count cards in the pool to preserve the display order.
			pool->SetFlags(wxImageStrip::FlagHideZeroes);

			// Assign numbers to what they actually have in their hand.
			const GamePtr& game = GetGame();

			// Determine how many cards they have to lose.
			wxInt32 total = 0;
			const Data::IntHash& resources = 
				game->playerGame(mIndex).read<Data::IntHash>(shResources);
			wxInt32 size = resources.size();

			Data::IntHash::const_iterator it, itEnd = resources.end();
			for(it = resources.begin(); it != itEnd; ++it)
			{
				const HashString& res = it->first;
				wxInt32 count = it->second;

				total += count;

				// Set that number in the pool.
				if(0 < count)
				{
					pool->SetResourceCount(res, count);
				}
				else
				{
					pool->Remove(res, true);
					--size;
				}
			}

			// They lose 1 or 2, depending on how many cards they have in hand.
			mLoss = std::min(total, 2);

			// The most resources they can select is the min of their amount
			// to lose or the resources available.
			selection->SetMaxImages(mLoss);

			wxBaseStaticText* text = GetText();
			wxString str;
			if(1 == mLoss)
			{
				str = stSelectOneCardToGive;
			}
			else
			{
				str = stSelectTwoCardsToGive;
			}
			
			text->SetLabel(str);
		}

		virtual bool AllowDrop(wxInteractiveImageStrip* target, wxCoord, 
			wxCoord, const HashString& res)
		{
			wxInteractiveImageStrip* selection = GetSelection();
			wxInteractiveImageStrip* pool = GetResourcePool();

			bool allow = false;

			// They can drop from the pool to the selection if they have not
			// assigned all their resources and they have that resource left
			// to assign..
			if(target == selection)
			{
				wxInt32 count = selection->GetResourceCount();
				wxInt32 poolAmount = pool->GetResourceCount(res);

				if( (count < mLoss) &&
					(0 < poolAmount))
				{
					allow = true;
				}
			}
			// They can always drag from selection to pool.
			else if(target == pool)
			{
				allow = true;
			}

			return allow;
		}

		virtual void ExecuteDrop(wxInteractiveImageStrip* target, wxCoord, 
			wxCoord, const HashString& res)
		{
			wxInteractiveImageStrip* selection = GetSelection();
			wxInteractiveImageStrip* pool = GetResourcePool();
			wxBaseButton* button = GetButton();

			wxInteractiveImageStrip* other = (target == pool) ? selection : 
				pool;

			wxInt32 targetAmount = target->GetResourceCount(res);
			wxInt32 otherAmount = other->GetResourceCount(res);

			// Adjust both counts.
			if(0 < targetAmount)
			{
				target->SetResourceCount(res, targetAmount + 1);
			}
			else
			{
				if(target == pool)
				{
					target->SetResourceCount(res, 1);
				}
				else
				{
					target->Add(res, 1);
				}
			}

			if(1 == otherAmount)
			{
				// Allow for zero cards in the pool to preserve the order.
				if(target == pool)
				{
					other->Remove(res);
				}
				else
				{
					other->SetResourceCount(res, 0);
				}
			}
			else
			{
				other->SetResourceCount(res, otherAmount - 1);
			}

			
			wxInt32 count = selection->GetResourceCount();
			button->Enable(count == mLoss);
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
		wxInt32 mLoss;
	};
}

//---------------------------- PUBLIC           -----------------------------//
wxWeddingDialog::wxWeddingDialog(wxWindow *parent, wxInt32 index, 
	boost::weak_ptr<Game> game)
: wxResourceSelectionDialog(parent, game, 
	SelectionLogicPtr(new LogicWedding(index, game)), shRuleExecuteWedding, 
	stCardsToGive, index)
{
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
