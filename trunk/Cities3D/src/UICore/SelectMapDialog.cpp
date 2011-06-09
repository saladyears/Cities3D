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
#include "SelectMapDialog.h"
#include "BaseModalSizer.h"
#include "MapListCtrl.h"
#include "BaseStaticText.h"
#include "BaseStaticBox.h"
#include "BaseCheckBox.h"
#include "BaseButton.h"
#include "Controller.h"
#include "Game.h"
#include "DataObject.h"
#include "GameEngine.h"
#include "MapDatabase.h"
#include "SkinManager.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	enum
	{
		ID_MAP_LIST		= 0,
	};

	//controls
	wxMapListCtrl *spMapList;
	wxBaseButton *spOK;

	//initial sizer creation
	class Sizer_SelectMapDialog : public BaseModalSizer
	{
	public:
		Sizer_SelectMapDialog(wxWindow *window) : BaseModalSizer(window) {}

		virtual void CreateSizers(wxBoxSizer *pSizer)
		{
			wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

			spMapList = new wxMapListCtrl(mpWindow, ID_MAP_LIST, 
				wxDefaultPosition, wxSize(250, 250));
			sizer->Add(spMapList, 1, wxEXPAND | wxALL, 3);
			pSizer->Add(sizer, 1, wxEXPAND);
		}

		virtual void CreateButtons(wxBoxSizer *pSizer)
		{
            static const wxString stOK = _("OK");
            static const wxString stCancel = _("Cancel");
	
			//add OK/Cancel buttons
			spOK = new wxBaseButton(mpWindow, wxID_OK, stOK);
			wxBaseButton *cancel = new wxBaseButton(mpWindow, wxID_CANCEL, 
				stCancel);

			pSizer->Add(spOK, 0, wxALL, 3);
			pSizer->Add(cancel, 0, wxALL, 3);
		}
	};
}

//---------------------------- PUBLIC           -----------------------------//
#ifdef __WIN32__
	//disables the 'this' : used in base member initializer list warning
	#pragma warning(disable:4355)
#endif
wxSelectMapDialog::wxSelectMapDialog(wxWindow *parent, const wxInt32 index)
: wxBaseModalDialog(parent, ISizerPtr(new Sizer_SelectMapDialog(this)), 
	_("Select A Map"), swSelectAMap)
, mIndex(index)
{
	spOK->Enable(false);

	if(-1 != mIndex)
	{
		spMapList->SetSelectedItem(mIndex);
	}
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxSelectMapDialog, wxBaseModalDialog)
	EVT_LIST_ITEM_SELECTED(ID_MAP_LIST, wxSelectMapDialog::OnMapSelected)
	EVT_LIST_ITEM_DESELECTED(ID_MAP_LIST, wxSelectMapDialog::OnMapSelected)
	EVT_LIST_ITEM_ACTIVATED(ID_MAP_LIST, wxSelectMapDialog::OnMapDoubleClick)
END_EVENT_TABLE()

void wxSelectMapDialog::Ok()
{
	wxInt32 index = spMapList->GetSelectedItem();

	//sanity
	if(-1 != index)
	{
		mIndex = spMapList->GetItemData(index);
		
		//now leave
		wxBaseModalDialog::Ok();
	}
}

void wxSelectMapDialog::OnMapSelected(wxListEvent &)
{
	wxInt32 index = spMapList->GetSelectedItem();

	spOK->Enable(-1 != index);
}

void wxSelectMapDialog::OnMapDoubleClick(wxListEvent &)
{
	Ok();
}
