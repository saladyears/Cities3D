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
#include "GoldDialog.h"
#include "SelectionLogic.h"
#include "InteractiveImageStrip.h"
#include "BaseButton.h"
#include "BaseStaticText.h"
#include "Game.h"
#include "DataObject.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	class GoldLogic : public SelectionLogic
	{
	public:
		GoldLogic(wxInt32 index, boost::weak_ptr<Game> game) 
			: SelectionLogic(game) 
			, mIndex(index)
		{}

		virtual void Initialize()
		{
			const GamePtr& game = GetGame();

			// Determine how much gold they have.
			const Data::IntHash& resources = 
				game->playerGame(mIndex).read<Data::IntHash>(shResources);
			
			wxInt32 size = resources.size();

			Data::IntHash::const_iterator it = resources.find(shGold);
			wxASSERT(resources.end() != it);

			mGold = it->second;

			// They are allowed up to their amount of gold selections.
			wxInteractiveImageStrip* selection = GetSelection();
			selection->SetMaxImages(std::min(mGold, size));

			wxBaseStaticText* text = GetText();
			wxString str = wxString::Format(stSelectXResourcesToGain.c_str(), 
				mGold);
			text->SetLabel(str);

			// It is possible that there are resources that are not allowed
			// in the gold dialog (i.e. C&K).  Therefore, go through each
			// resource type listed in the bank and see if it is allowed in
			// the gold dialog.  If not, remove it from the pool.
			{
				wxInteractiveImageStrip* pool = GetResourcePool();

				const Data::IntHash& bankResources = 
					game->read<Data::IntHash>(shBankResources);
				Data::IntHash::const_iterator it, itEnd = bankResources.end();
				for(it = bankResources.begin(); it != itEnd; ++it)
				{
					const HashString& type = it->first;
					DataObject input(type), output;
					RULE.Decide(shLogicIsResourceGoldable, input, output);

					if(false == output.read<bool>())
					{
						pool->Remove(type, true);
					}
				}
			}
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
			// They may go from pool to selection if the total resources is
			// their gold amount or less, and the bank has enough of the 
			// resources.
			else if(target == selection)
			{
				const Data::IntHash& resources = 
					game->read<Data::IntHash>(shBankResources);
				Data::IntHash::const_iterator it = resources.find(res);
				wxASSERT(resources.end() != it);
				
				wxInt32 bankAmount = it->second;
				wxInt32 targetAmount = selection->GetResourceCount();

				allow = (targetAmount + 1 <= bankAmount) &&
						(mGold > selection->GetResourceCount());
			}

			return allow;
		}

		virtual void ExecuteDrop(wxInteractiveImageStrip* target, wxCoord, 
			wxCoord, const HashString& res)
		{
			wxInteractiveImageStrip* selection = GetSelection();
			wxInteractiveImageStrip* pool = GetResourcePool();

			wxInt32 targetAmount = selection->GetResourceCount(res);
			wxBaseButton* button = GetButton();
			
			bool enable = false;

			// If the pool is the target, just remove the resource from the
			// selection.
			if(target == pool)
			{
				if(1 < targetAmount)
				{
					selection->SetResourceCount(res, targetAmount - 1);
				}
				else
				{
					selection->Remove(res);
				}
			}
			// If the selection is the target, increase the count if the
			// resource exists, or add it otherwise.
			else if(target == selection)
			{
				if(1 <= targetAmount)
				{
					selection->SetResourceCount(res, targetAmount + 1);
				}
				else
				{
					selection->Add(res, 1);
				}

				enable = (mGold == selection->GetResourceCount());
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
		wxInt32 mGold;
	};
}

//---------------------------- PUBLIC           -----------------------------//
wxGoldDialog::wxGoldDialog(wxWindow *parent, wxInt32 index, 
	boost::weak_ptr<Game> game)
: wxResourceSelectionDialog(parent, game, SelectionLogicPtr(
	new GoldLogic(index, game)), shRuleExecuteGold, stSpendYourGold, 
	index)
{
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
