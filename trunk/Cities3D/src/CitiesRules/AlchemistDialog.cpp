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
#include "AlchemistDialog.h"
#include "ISizer.h"
#include "BaseStaticBox.h"
#include "BaseButton.h"
#include "BaseStaticText.h"
#include "TradeLogic.h"
#include "SelectionImageStrip.h"
#include "ImageEngine.h"
#include "GameEngine.h"
#include "IRuleEngine.h"
#include "DataObject.h"

//---------------------------- USER INCLUDES    -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	enum
	{
		ID_OK_ALCHEMIST_BUTTON		= 0,
	};

	class AlchemistTradeLogic : public TradeLogic
	{
	public:
		AlchemistTradeLogic()
			: TradeLogic(GamePtr())
		{}

		virtual void Initialize()
		{
			wxInteractiveImageStrip* selection = GetResourcePool();
			selection->SetMaxImages(2);
		}

		virtual bool AllowDrop(wxInteractiveImageStrip* target, wxCoord, 
			wxCoord, const HashString&)
		{
			bool allow = false;

			wxInteractiveImageStrip* selection = GetResourcePool();
			
			// They can always drag from the selection to the left or right
			// side, or if the selection is the target.
			if( (true == selection->InDrag()) ||
				(target == selection))
			{
				allow = true;
			}

			return allow;
		}

		virtual void ExecuteDrop(wxInteractiveImageStrip*, wxCoord, 
			wxCoord, const HashString& res)
		{
			wxInteractiveImageStrip* selection = GetResourcePool();
			wxInteractiveImageStrip* left = GetLeftSide();
			wxInteractiveImageStrip* right = GetRightSide();

			// If we're dragging from the selection, just remove the item
			// from the selection.
			if(true == selection->InDrag())
			{
				ExecuteSelection(res);
			}
			else if(true == left->InDrag())
			{
				ExecuteLeftToSelection(res);
			}
			else if(true == right->InDrag())
			{
				ExecuteRightToSelection(res);
			}
		}

		virtual void HandleLeftClick(wxInteractiveImageStrip* origin,
			const HashString& res)
		{
			// Left-click works from left or right to selection and from
			// selection to anywhere.
			wxInteractiveImageStrip* selection = GetResourcePool();
			wxInteractiveImageStrip* left = GetLeftSide();
			wxInteractiveImageStrip* right = GetRightSide();

			if(origin == selection)
			{
				ExecuteSelection(res);
			}
			else if(origin == left)
			{
				ExecuteLeftToSelection(res);
			}
			else if(origin == right)
			{
				ExecuteRightToSelection(res);
			}
		}

		virtual void HandleRightClick(wxInteractiveImageStrip*, 
			const HashString&)
		{
			// Right click does nothing.
		}

	private:
		void ExecuteSelection(const HashString& res)
		{
			wxInteractiveImageStrip* selection = GetResourcePool();

			selection->Remove(res);

			GetButton()->Disable();
		}

		void ExecuteLeftToSelection(const HashString& res)
		{
			wxInteractiveImageStrip* selection = GetResourcePool();

			// Remove any existing normal die, then add this one.
			selection->Freeze();
			selection->SetKey(shDice, false);
			selection->Remove(sh1);
			selection->Remove(sh2);
			selection->Remove(sh3);
			selection->Remove(sh4);
			selection->Remove(sh5);
			selection->Remove(sh6);

			// This gigantic hack always keeps the non-red die on the left
			// side.  The right way to do this is to add an Insert function to
			// wxImageStrip, but that looks complicated and I'm lazy.
			HashString type;
			if(1 == selection->GetResourceCount())
			{
				wxImageStrip::ResourceMap map;
				selection->FillResourceMap(map);
				wxASSERT(1 == map.size());

				type = map.begin()->first;
				selection->Remove(type);
			}

			selection->Add(res, 1);

			if(false == type.empty())
			{
				selection->SetKey(shDiceRed, false);
				selection->Add(type, 1);
			}

			selection->Thaw();

			bool enable = (2 == selection->GetResourceCount());
			GetButton()->Enable(enable);
		}

		void ExecuteRightToSelection(const HashString& res)
		{
			wxInteractiveImageStrip* selection = GetResourcePool();

			// Remove any existing red die, then add this one.
			selection->SetKey(shDiceRed, false);
			selection->Remove(shRed1);
			selection->Remove(shRed2);
			selection->Remove(shRed3);
			selection->Remove(shRed4);
			selection->Remove(shRed5);
			selection->Remove(shRed6);
			selection->Add(res, 1);

			bool enable = (2 == selection->GetResourceCount());
			GetButton()->Enable(enable);
		}
	};

	TradeLogicPtr sLogic;
	wxInteractiveImageStrip* spSelection = NULL;

	class wxDiceImageStrip : public wxInteractiveImageStrip
	{
	public:
		wxDiceImageStrip(wxWindow* parent, IResourceLogic* logic, 
			const HashString& type)
			: wxInteractiveImageStrip(parent, -1, 6, 10, logic, type, 
				shDice == type ? sh1 : shRed1) 
		{
			// Add each of the dice to the strip.
			if(shDice == type)
			{
				Add(sh1, 1);
				Add(sh2, 1);
				Add(sh3, 1);
				Add(sh4, 1);
				Add(sh5, 1);
				Add(sh6, 1);
			}
			else
			{
				Add(shRed1, 1);
				Add(shRed2, 1);
				Add(shRed3, 1);
				Add(shRed4, 1);
				Add(shRed5, 1);
				Add(shRed6, 1);
			}

			SetPixelSpacing(10);
			SetFlags(FlagAlignCenter | FlagHideText);
		}
	};

	class wxDiceSelectionImageStrip : public wxInteractiveImageStrip
	{
	public:
		wxDiceSelectionImageStrip(wxWindow* parent, IResourceLogic* logic)
			: wxInteractiveImageStrip(parent, -1, 2, 10, logic, shDice, sh1) 
		{
			SetPixelSpacing(4);
			SetFlags(FlagOneOutline | FlagHideText);
		}
	};

	class Sizer_Alchemist : public ISizer
	{
	public:
		Sizer_Alchemist(wxWindow *window)
			: ISizer(window)
		{
			sLogic = TradeLogicPtr(new AlchemistTradeLogic);
		}

		void CreateLayout(wxBoxSizer *sizer)
		{
			wxBaseStaticBox *box = new wxBaseStaticBox(mpWindow, 0, 
				stAlchemist, wxDefaultPosition, wxDefaultSize, 
				shDefaultText, shTradeBoxFont);
			wxStaticBoxSizer *boxSizer = 
				new wxStaticBoxSizer(box, wxHORIZONTAL);
			
			spSelection = new wxDiceSelectionImageStrip(mpWindow, 
				sLogic.get());

			wxBaseButton *button = new wxBaseButton(mpWindow, ID_OK_ALCHEMIST_BUTTON, 
				stOK);
			button->Disable();

			wxBaseStaticText* text = new wxBaseStaticText(mpWindow, 0, 
				stSelectOneDieOfEachColor, shDefaultText, shInfoFont);

			wxBoxSizer *vertBox = new wxBoxSizer(wxVERTICAL);
			wxBoxSizer *horzBox1 = new wxBoxSizer(wxHORIZONTAL);

			horzBox1->Add(spSelection, 1, wxEXPAND | wxLEFT | wxTOP, 2);
			horzBox1->Add(button, 0, 
				wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT | wxBOTTOM, 4);

			wxBoxSizer *horzBox2 = new wxBoxSizer(wxHORIZONTAL);
			horzBox2->Add(text, 1, wxEXPAND | wxALL, 4);

			vertBox->Add(horzBox1, 1, wxEXPAND);
			vertBox->Add(horzBox2, 0, wxALIGN_CENTER_HORIZONTAL);

			boxSizer->Add(vertBox, 1, wxEXPAND);

			// Create the dice pools.
			wxInteractiveImageStrip* left = new wxDiceImageStrip(mpWindow,
				sLogic.get(), shDice);
			wxInteractiveImageStrip* right = new wxDiceImageStrip(mpWindow,
				sLogic.get(), shDiceRed);

			sLogic->SetParameters(left, right, spSelection, button);

			// Calculate offsets.
			wxSizeEvent event(wxSize(0, 0));
			left->ProcessEvent(event);
			right->ProcessEvent(event);
			spSelection->ProcessEvent(event);

			// Calculate the ratio needed to keep all cards the same size in
			// the window.
			wxInt32 leftX = left->GetMinSize().x;
			wxInt32 boxX = boxSizer->GetMinSize().x;

			// Determine buffers.
			wxInt32 x = ((boxX - leftX) / 2) + 19;

			wxBoxSizer *horz2 = new wxBoxSizer(wxHORIZONTAL);
			horz2->AddSpacer(x);
			horz2->Add(left, 1, wxEXPAND);
			horz2->AddSpacer(x);

			wxBoxSizer *horz3 = new wxBoxSizer(wxHORIZONTAL);
			horz3->AddSpacer(x);
			horz3->Add(right, 1, wxEXPAND);
			horz3->AddSpacer(x);

			wxBoxSizer *horz4 = new wxBoxSizer(wxHORIZONTAL);
			wxBaseStaticText *infoText = new wxBaseStaticText(mpWindow, -1,
				stDragAndDropDice, shDefaultText, shInfoFont);
			
			horz4->Add(infoText, 1, wxEXPAND | wxALL, 2);

			wxBoxSizer* vert = new wxBoxSizer(wxVERTICAL);
			vert->AddSpacer(6);
			vert->Add(boxSizer, 20, wxEXPAND | wxLEFT | wxRIGHT, 18);
			vert->Add(horz2, 10, wxEXPAND);
			vert->Add(horz3, 10, wxEXPAND);
			vert->Add(horz4, 0, wxALIGN_CENTER_HORIZONTAL);
			vert->AddSpacer(6);

			sizer->Add(vert, 1, wxEXPAND);
		}
	};
}

//---------------------------- PUBLIC           -----------------------------//
#ifdef __WIN32__
	//disables the 'this' : used in base member initializer list warning
	#pragma warning(disable:4355)
#endif
wxAlchemistDialog::wxAlchemistDialog(wxWindow *parent)
: wxBaseGameDialog(parent, ISizerPtr(new Sizer_Alchemist(this)), 
	stSelectDice, swSelectDice)
, mLogic(sLogic)
{
}

wxAlchemistDialog::~wxAlchemistDialog()
{
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxAlchemistDialog, wxBaseGameDialog)
	EVT_BUTTON(ID_OK_ALCHEMIST_BUTTON, wxAlchemistDialog::OnButton)
END_EVENT_TABLE()

void wxAlchemistDialog::OnButton(wxCommandEvent &)
{
	// Grab all of the resources in the selection strip and send it down with
	// the rule.
	wxImageStrip::ResourceMap resources;
	spSelection->FillResourceMap(resources);

	RULE.Execute(shRuleSelectAlchemistDice, DataObject(resources));

	Destroy();
}
