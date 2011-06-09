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
#include "YearOfPlentyDialog.h"
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
	class YearOfPlentyLogic : public SelectionLogic
	{
	public:
		YearOfPlentyLogic(boost::weak_ptr<Game> game) : SelectionLogic(game) {}

		virtual void Initialize()
		{
			// They are allowed two selections, though they may choose two
			// of the same type.
			wxInteractiveImageStrip* selection = GetSelection();
			selection->SetMaxImages(2);

			wxBaseStaticText* text = GetText();
			text->SetLabel(stSelectTwoResourcesToGain);
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
			// two or less, and the bank has enough of the resources.
			else if(target == selection)
			{
				const Data::IntHash& resources = 
					game->read<Data::IntHash>(shBankResources);
				Data::IntHash::const_iterator it = resources.find(res);
				wxASSERT(resources.end() != it);
				
				wxInt32 bankAmount = it->second;
				wxInt32 targetAmount = selection->GetResourceCount();

				allow = (targetAmount + 1 <= bankAmount) &&
						(2 > selection->GetResourceCount());
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
				wxASSERT( (1 == targetAmount) || (2 == targetAmount));
				
				if(2 == targetAmount)
				{
					selection->SetResourceCount(res, 1);
				}
				else if(1 == targetAmount)
				{
					selection->Remove(res);
				}
			}
			// If the selection is the target, increase the count if the
			// resource exists, or add it otherwise.
			else if(target == selection)
			{
				if(1 == targetAmount)
				{
					selection->SetResourceCount(res, 2);
				}
				else
				{
					selection->Add(res, 1);
				}

				enable = (2 == selection->GetResourceCount());
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
	};
}

//---------------------------- PUBLIC           -----------------------------//
wxYearOfPlentyDialog::wxYearOfPlentyDialog(wxWindow *parent, 
	boost::weak_ptr<Game> game)
: wxResourceSelectionDialog(parent, game, SelectionLogicPtr(
	new YearOfPlentyLogic(game)), shRuleExecuteYearOfPlentyCard, 
	stYearOfPlenty)
{
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
