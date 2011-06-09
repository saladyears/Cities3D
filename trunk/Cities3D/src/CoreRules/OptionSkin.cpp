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
#include "OptionSkin.h"
#include "ConfigDatabase.h"
#include "FilePath.h"
#include "SkinManager.h"
#include "ImageEngine.h"
#include "BaseStaticBox.h"
#include "FittedStaticText.h"
#include "BaseComboBox.h"
#include "Controller.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	enum
	{
		ID_SKIN_COMBO		= 0,
	};

	wxComboBox *spSkinCombo;
	
	void PopulateSkinCombo(wxWindow *parent)
	{
		wxArrayString dirs(FilePath::EnumDataSubDirs(swSkins));

		wxString current = CONFIG.read<wxString>(swConfigSectionGeneral, 
			swSkinFolder, swEmpty);

		if(true == current.IsEmpty())
		{
			current = swStandard;
		}

		//now that we've got a list, create the combo box with it
		spSkinCombo = new wxBaseComboBox(parent, ID_SKIN_COMBO, current, 
			wxDefaultPosition, wxSize(150, 25), 
			dirs.size(), &dirs[0], wxCB_READONLY);
	}
}

//---------------------------- PUBLIC           -----------------------------//
wxOptionSkin::wxOptionSkin(wxWindow* parent)
: wxOptionWindow(parent, swSkin)
{
	wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);

	wxBaseStaticBox *box = new wxBaseStaticBox(this, -1, stSkins, 
		wxDefaultPosition, wxSize(200, 50));
	wxStaticBoxSizer *boxSizer = new wxStaticBoxSizer(box, wxHORIZONTAL);
	
	wxBoxSizer *innerVert = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *innerHorz1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *innerHorz2 = new wxBoxSizer(wxHORIZONTAL);
	
	wxBaseStaticText *images = new wxBaseStaticText(this, -1, stSkins + swColon);
	PopulateSkinCombo(this);

	innerHorz1->Add(images, 0, wxTOP | wxLEFT, 7);
	innerHorz1->Add(spSkinCombo, 1, wxALL | wxEXPAND, 4);

	wxBaseStaticText *explainText = new wxFittedStaticText(this, -1, 
		stSelectTheSkin, shInfoText, shInfoFont,
		wxSize(200, 33), wxST_NO_AUTORESIZE | wxALIGN_LEFT);

	innerHorz2->Add(explainText, 1, wxEXPAND | wxLEFT | wxRIGHT, 7);

	innerVert->Add(innerHorz1, 1, wxEXPAND);
	innerVert->Add(innerHorz2, 1, wxEXPAND);
	boxSizer->Add(innerVert, 1, wxEXPAND);

	pSizer->Add(boxSizer, 1, wxEXPAND);

	SetSizer(pSizer);
	pSizer->SetSizeHints(this);
	pSizer->Layout();
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxOptionSkin, wxOptionWindow)
	EVT_COMBOBOX(ID_SKIN_COMBO, wxOptionSkin::OnSkinComboSel)
END_EVENT_TABLE()

void wxOptionSkin::OnSkinComboSel(wxCommandEvent &)
{
	wxInt32 sel = spSkinCombo->GetSelection();

	//sanity
	wxASSERT(-1 != sel);

	wxString str = spSkinCombo->GetString(sel);
	wxString current = CONFIG.read<wxString>(swConfigSectionGeneral, 
		swSkinFolder, swEmpty);

	//no need to do anything if they are the same
	if(str == current)
	{
		return;
	}

	//set the new name
	CONFIG.write<wxString>(swConfigSectionGeneral, swSkinFolder, str);

	// Fire the event.
	SKIN.LoadXML();
	IMAGE.IgnoreCache();
	Controller::get().Transmit(shEventSkin, 0);
}

