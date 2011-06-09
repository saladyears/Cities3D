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
#include "ResourceSelectionDialog.h"
#include "ISizer.h"
#include "SelectionImageStrip.h"
#include "ResourcePoolImageStrip.h"
#include "BaseStaticText.h"
#include "BaseStaticBox.h"
#include "BaseButton.h"
#include "SelectionLogic.h"
#include "GameEngine.h"
#include "DataObject.h"
#include "ConfigDatabase.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	enum
	{
		ID_OK_BUTTON		= 0,
	};

	class Sizer_ResourceSelectionDialog : public ISizer
	{
	public:
		Sizer_ResourceSelectionDialog(wxWindow *window, 
			boost::weak_ptr<Game> game, SelectionLogicPtr logic, 
			const wxString& boxText, wxInteractiveImageStrip*& selection)	
			: ISizer(window)
			, mGame(game)
			, mLogic(logic)
			, mBoxText(boxText)
			, mpSelection(selection)
		{}

		void CreateLayout(wxBoxSizer *sizer)
		{
			wxBoxSizer *vert1 = new wxBoxSizer(wxVERTICAL);

			wxBaseStaticBox *box = new wxBaseStaticBox(mpWindow, 0, 
				mBoxText, wxDefaultPosition, wxDefaultSize, 
				shDefaultText, shTradeBoxFont);
			wxStaticBoxSizer *boxSizer = 
				new wxStaticBoxSizer(box, wxHORIZONTAL);
			
			// Create the selection image strip.
			mpSelection = new wxSelectionImageStrip(mpWindow, 0, 0, 10, 
				mLogic.get());

            static const wxString stOK = _("OK");
	
			wxBaseButton *button = new wxBaseButton(mpWindow, ID_OK_BUTTON, 
				stOK);
			button->Disable();

			wxBaseStaticText* text = new wxBaseStaticText(mpWindow, 0, swEmpty,
				shDefaultText, shInfoFont);

			wxBoxSizer *vertBox = new wxBoxSizer(wxVERTICAL);
			wxBoxSizer *horzBox1 = new wxBoxSizer(wxHORIZONTAL);

			horzBox1->Add(mpSelection, 1, wxEXPAND | wxLEFT | wxTOP, 2);
			horzBox1->Add(button, 0, 
				wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT | wxBOTTOM, 4);

			wxBoxSizer *horzBox2 = new wxBoxSizer(wxHORIZONTAL);
			horzBox2->Add(text, 1, wxEXPAND | wxALL, 4);

			vertBox->Add(horzBox1, 1, wxEXPAND);
			vertBox->Add(horzBox2, 0, wxALIGN_CENTER_HORIZONTAL);

			boxSizer->Add(vertBox, 1, wxEXPAND);

			wxInteractiveImageStrip* pool = new wxResourcePoolImageStrip(
				mpWindow, 0, mGame, mLogic.get());

			mLogic->SetParameters(mpSelection, pool, button, text);

			wxBoxSizer *horz2 = new wxBoxSizer(wxHORIZONTAL);
	
			// See if we need to force max size.
			wxInt32 width = CONFIG.read(swConfigSectionUI, swSelectResourcesSlash + swWidth, -1);
			wxInt32 height = CONFIG.read(swConfigSectionUI, swSelectResourcesSlash + swHeight, -1);

			wxInt32 initial = 0;

			if( (-1 == width) ||
				(-1 == height))
			{
				initial = -1;
			}

			wxSizeEvent event(wxSize(initial, initial));
			mpSelection->ProcessEvent(event);
			pool->ProcessEvent(event);

			// Calculate the ratio needed to keep all cards the same size in
			// the window.
			wxInt32 poolX = pool->GetMinSize().x;
			wxInt32 boxX = boxSizer->GetMinSize().x;

			bool top = (boxX > poolX);
			wxInt32 x = (abs((poolX - boxX)) / 2) + 11;

			wxBoxSizer* horz1 = new wxBoxSizer(wxHORIZONTAL);

			if(false == top)
			{
				horz1->AddSpacer(x);
			}
			horz1->Add(boxSizer, 1, wxEXPAND);
			if(false == top)
			{
				horz1->AddSpacer(x);
			}

			if(true == top)
			{
				horz2->AddSpacer(x);
			}
			horz2->Add(pool, 1, wxEXPAND);
			if(true == top)
			{
				horz2->AddSpacer(x);
			}

            static const wxString stDragAndDropToMakeASelection = _("Drag and drop cards to make a selection.");
	
			wxBoxSizer *horz3 = new wxBoxSizer(wxHORIZONTAL);
			text = new wxBaseStaticText(mpWindow, -1,
				stDragAndDropToMakeASelection, shDefaultText, shInfoFont);

			horz3->Add(text, 1, wxEXPAND | wxALL, 2);

			vert1->Add(horz1, 10, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 6);
			vert1->AddSpacer(4);
			vert1->Add(horz2, 10, wxEXPAND);
			vert1->Add(horz3, 0, wxALIGN_CENTER_HORIZONTAL);

			sizer->Add(vert1, 1, wxEXPAND);
		}

	private:
		boost::weak_ptr<Game> mGame;
		SelectionLogicPtr mLogic;
		wxString mBoxText;

		// OK, this is a little crazy, but you can't make anonymous namespace
		// classes as friends, so store a reference to the selection image
		// strip pointer so it can be set by the layout.
		wxInteractiveImageStrip *&mpSelection;
	};
}

//---------------------------- PUBLIC           -----------------------------//
#ifdef __WIN32__
	//disables the 'this' : used in base member initializer list warning
	#pragma warning(disable:4355)
#endif
wxResourceSelectionDialog::wxResourceSelectionDialog(wxWindow *parent, 
	boost::weak_ptr<Game> game, SelectionLogicPtr logic, const HashString& rule,
	const wxString& boxText, wxInt32 index)
: wxBaseGameDialog(parent, ISizerPtr(new Sizer_ResourceSelectionDialog(this, 
	game, logic, boxText, mpSelection)), _("Select Resources"), swSelectResources)
, mLogic(logic)
, mRule(rule)
, mIndex(index)
{
}

wxResourceSelectionDialog::~wxResourceSelectionDialog()
{
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxResourceSelectionDialog, wxBaseGameDialog)
	EVT_BUTTON(ID_OK_BUTTON, wxResourceSelectionDialog::OnButton)
END_EVENT_TABLE()

void wxResourceSelectionDialog::OnButton(wxCommandEvent &)
{
	// Grab all of the resources in the selection strip and send it down with
	// the rule.
	wxImageStrip::ResourceMap resources;
	mpSelection->FillResourceMap(resources);

	RULE.Execute(mRule, DataObject(resources, mIndex));

	Destroy();
}
