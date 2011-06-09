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
#include "MonopolyDialog.h"
#include "SelectionLogic.h"
#include "InteractiveImageStrip.h"
#include "BaseButton.h"
#include "BaseStaticText.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	class MonopolyLogic : public SelectionLogic
	{
	public:
		MonopolyLogic(boost::weak_ptr<Game> game) : SelectionLogic(game) {}

		virtual void Initialize()
		{
			// They are only allowed one selection.
			wxInteractiveImageStrip* selection = GetSelection();
			selection->SetMaxImages(1);
			selection->SetFlags(wxImageStrip::FlagHideText);

			wxBaseStaticText* text = GetText();
			text->SetLabel(stSelectOneResourceTypeToSteal);
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
				button->Disable();
			}
			// If the target is the selection, remove any current resource
			// and add this one.
			else if(target == selection)
			{
				selection->Clear();
				selection->Add(res, 1);
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
	};
}

//---------------------------- PUBLIC           -----------------------------//
wxMonopolyDialog::wxMonopolyDialog(wxWindow *parent, 
	boost::weak_ptr<Game> game)
: wxResourceSelectionDialog(parent, game, SelectionLogicPtr(
	new MonopolyLogic(game)), shRuleExecuteMonopolyCard, stMonopoly)
{
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
