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
#include "SettingsPane.h"
#include "BaseModalSizer.h"
#include "BaseTreeCtrl.h"
#include "OptionLanguage.h"
#include "OptionImages.h"
#include "OptionSounds.h"
#include "OptionSkin.h"
#include "OptionAnimation.h"
#include "OptionPopup.h"
#include "OptionAdvanced.h"
#include "OptionKeyMaps.h"
#include "BaseButton.h"
#include "ConfigDatabase.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef __WIN32__
#ifdef max
#undef max
#endif
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	enum
	{
		ID_OPTIONS_TREE		= 0,
	};

	wxBaseTreeCtrl *spTreeCtrl;
	wxBoxSizer *spOptionsSizer;

	class wxOptionsItemData : public wxTreeItemData
	{
	public:
		wxOptionsItemData(wxOptionWindow *pWindow) : window(pWindow) {}
		wxOptionWindow *window;
	};

	const wxSize FindMinSize()
	{
		wxSize size(0, 0);

		// Iterate over the tree control, find all the child option windows,
		// store the greatest common minimum size.
		wxTreeItemId id = spTreeCtrl->GetFirstVisibleItem();

		do
		{
			wxOptionsItemData *option = 
				dynamic_cast<wxOptionsItemData *>(spTreeCtrl->GetItemData(id));
			if(NULL != option)
			{
				wxWindow *window = option->window;
				if(NULL != window)
				{
					wxSize minSize = window->GetMinSize();

					if(size.x < minSize.x)
					{
						size.x = minSize.x;
					}
					if(size.y < minSize.y)
					{
						size.y = minSize.y;
					}
				}
			}

			id = spTreeCtrl->GetNextVisible(id);
		}
		while(true == id.IsOk());

		// Need a 2 pixel buffer for the outlines around the boxes.
		return size + wxSize(2, 2);
	}

	void CreateLayout(wxSizer* sizer, wxWindow* window)
	{
		spTreeCtrl = new wxBaseTreeCtrl(window, ID_OPTIONS_TREE, 
			wxDefaultPosition, wxSize(150, 250), wxTR_HIDE_ROOT | 
			wxTR_HAS_BUTTONS | wxTR_LINES_AT_ROOT | wxTR_FULL_ROW_HIGHLIGHT | 
			wxTR_SINGLE);

		//set up the tree
		wxTreeItemId root;
		root = spTreeCtrl->AddRoot(swRoot);

		// Display settings
		wxTreeItemId display;
		display = spTreeCtrl->AppendItem(root, stDisplay);
		spTreeCtrl->AppendItem(display, stImages, -1, -1, 
			new wxOptionsItemData(new wxOptionImages(window)));
		spTreeCtrl->AppendItem(display, stSkin, -1, -1, 
			new wxOptionsItemData(new wxOptionSkin(window)));
		spTreeCtrl->AppendItem(display, stAnimation, -1, -1, 
			new wxOptionsItemData(new wxOptionAnimation(window)));
		spTreeCtrl->AppendItem(display, stPopups, -1, -1, 
			new wxOptionsItemData(new wxOptionPopup(window)));
		spTreeCtrl->AppendItem(display, stAdvanced, -1, -1,
			new wxOptionsItemData(new wxOptionAdvanced(window)));
		spTreeCtrl->Expand(display);

		// Sound settings
		spTreeCtrl->AppendItem(root, stSounds, -1, -1,
			new wxOptionsItemData(new wxOptionSounds(window)));

		// Language settings
		spTreeCtrl->AppendItem(root, stLanguage, -1, -1,
			new wxOptionsItemData(new wxOptionLanguage(window)));

		// Key Map settings
		spTreeCtrl->AppendItem(root, stKeyboard, -1, -1,
			new wxOptionsItemData(new wxOptionKeyMaps(window)));

		//left and right-side sizers
		wxBoxSizer *tree = new wxBoxSizer(wxVERTICAL);
		tree->Add(spTreeCtrl, 0, wxEXPAND);

		spOptionsSizer = new wxBoxSizer(wxVERTICAL);

		// Find the minimum size required for our options section.
		wxSize minSize = FindMinSize();
		spOptionsSizer->SetMinSize(minSize);
		spOptionsSizer->Add(0, 0);

		sizer->Add(tree, 0, wxEXPAND | wxALL, 3);
		sizer->Add(spOptionsSizer, 1, wxEXPAND | wxALL, 3);
	}

	void CreatePaneSettings(wxWindow* window)
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		// Create the sections.
		CreateLayout(sizer, window);
		
		window->SetSizer(sizer);
		sizer->SetSizeHints(window);
	}

	void SetCurrentTreeSelection()
	{
		// Get the key of the last open window.
		wxString key = CONFIG.read<wxString>(swConfigSectionGeneral, 
			swLastOptionOpen, swEmpty);

		// Iterate over the tree control until we find an item that matches the
		// given key.
		wxTreeItemId id = spTreeCtrl->GetFirstVisibleItem();
		
		do
		{
			wxOptionsItemData *option = 
				dynamic_cast<wxOptionsItemData *>(spTreeCtrl->GetItemData(id));
			if(NULL != option)
			{
				wxOptionWindow *optionWindow = option->window;
				if( (NULL != optionWindow) &&
					(key == optionWindow->GetKey())
					)
				{
					spTreeCtrl->SelectItem(id);
					break;
				}
			}

			id = spTreeCtrl->GetNextVisible(id);
		}
		while(true == id.IsOk());
	}
}

//---------------------------- PUBLIC           -----------------------------//
wxSettingsPane::wxSettingsPane(wxWindow *parent, wxInt32 id)
: wxPanel(parent, wxID_ANY, wxPoint(parent->GetClientSize().x, 
	parent->GetClientSize().y))
, PaneMixin(this, id)
, mpCurrentWindow(NULL)
{
	CreatePaneSettings(this);

	SetCurrentTreeSelection();
}


//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxSettingsPane, wxPanel)
	EVT_TREE_SEL_CHANGED(ID_OPTIONS_TREE, wxSettingsPane::OnTreeSelection)
END_EVENT_TABLE()

void wxSettingsPane::OnTreeSelection(wxTreeEvent &event)
{
	wxTreeItemId id = event.GetItem();

	//see if this item has an attached option window to display
	wxOptionsItemData *pOption = 
		dynamic_cast<wxOptionsItemData *>(spTreeCtrl->GetItemData(id));

	//always remove the currently displayed window
	if(NULL != mpCurrentWindow)
	{
		mpCurrentWindow->Show(false);
		spOptionsSizer->Detach(mpCurrentWindow);
	}

	//show the new window if there is one
	if(NULL != pOption)
	{
		mpCurrentWindow = pOption->window;
		wxASSERT(NULL != mpCurrentWindow);

		wxSizer *sizer = mpCurrentWindow->GetSizer();
		if(NULL != sizer)
		{
			wxSize size = sizer->GetMinSize();
			sizer->SetDimension(0, 0, size.x, size.y);
		}
		mpCurrentWindow->Show(true);
		spOptionsSizer->Add(mpCurrentWindow, 1, wxEXPAND);
		spOptionsSizer->Layout();
	}
}


