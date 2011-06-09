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
#include "OptionSounds.h"
#include "ConfigDatabase.h"
#include "FilePath.h"
#include "SkinManager.h"
#include "BaseStaticBox.h"
#include "FittedStaticText.h"
#include "BaseComboBox.h"
#include "BaseCheckBox.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	enum
	{
		ID_SOUNDS_COMBO		= 0,
		ID_SOUNDS_CHECK		,
	};

	wxComboBox *spSoundsCombo;
	wxCheckBox *spSoundsCheck;
	
	void PopulateSoundCombo(wxWindow *parent)
	{
		wxArrayString dirs(FilePath::EnumDataSubDirs(swSounds));

		wxString current = CONFIG.read<wxString>(swConfigSectionGeneral, 
			swSoundFolder, swEmpty);

		if(true == current.IsEmpty())
		{
			current = swStandard;
		}

		//now that we've got a list, create the combo box with it
		spSoundsCombo = new wxBaseComboBox(parent, ID_SOUNDS_COMBO, current, 
			wxDefaultPosition, wxSize(150, 25), 
			dirs.size(), &dirs[0], wxCB_READONLY);
	}
}

//---------------------------- PUBLIC           -----------------------------//
wxOptionSounds::wxOptionSounds(wxWindow* parent)
: wxOptionWindow(parent, swSoundsCaps)
{
	wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);

	wxBaseStaticBox *box = new wxBaseStaticBox(this, -1, stGameSounds, 
		wxDefaultPosition, wxSize(200, 0));
	wxStaticBoxSizer *boxSizer = new wxStaticBoxSizer(box, wxHORIZONTAL);
	
	wxBoxSizer *innerVert = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *innerHorz1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *innerHorz2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *innerHorz3 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *innerHorz4 = new wxBoxSizer(wxHORIZONTAL);
	
	wxBaseStaticText *images = new wxBaseStaticText(this, -1, stSounds + swColon);
	PopulateSoundCombo(this);

	innerHorz1->Add(images, 0, wxTOP | wxLEFT, 7);
	innerHorz1->Add(spSoundsCombo, 1, wxALL | wxEXPAND, 4);

	wxBaseStaticText *explainText = new wxFittedStaticText(this, -1, 
		stSelectTheSetOfSounds, shInfoText, shInfoFont,
		wxSize(200, 22), wxST_NO_AUTORESIZE | wxALIGN_LEFT);

	innerHorz2->Add(explainText, 1, wxEXPAND | wxLEFT | wxRIGHT, 7);

	wxBOOL sounds = CONFIG.read<wxInt32>(swConfigSectionGeneral, swPlaySounds, 
		TRUE);
	spSoundsCheck = new wxBaseCheckBox(this, ID_SOUNDS_CHECK, 
		stPlaySounds);
	spSoundsCheck->SetValue(sounds || false);

	innerHorz3->Add(spSoundsCheck, 1, wxEXPAND | wxLEFT | wxRIGHT, 7);

	wxBaseStaticText *explainText2 = new wxFittedStaticText(this, -1, 
		stTurningSoundsOff, shInfoText, shInfoFont, wxSize(200, 33), 
		wxST_NO_AUTORESIZE | wxALIGN_LEFT);

	innerHorz4->Add(explainText2, 1, wxEXPAND | wxLEFT | wxRIGHT | 
		wxBOTTOM, 7);

	innerVert->Add(innerHorz1, 1, wxEXPAND);
	innerVert->Add(innerHorz2, 1, wxEXPAND);
	innerVert->Add(innerHorz3, 1, wxEXPAND);
	innerVert->Add(innerHorz4, 1, wxEXPAND);
	boxSizer->Add(innerVert, 1, wxEXPAND);

	pSizer->Add(boxSizer, 1, wxEXPAND);

	SetSizer(pSizer);
	pSizer->SetSizeHints(this);
	pSizer->Layout();
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxOptionSounds, wxOptionWindow)
	EVT_COMBOBOX(ID_SOUNDS_COMBO, wxOptionSounds::OnSoundsComboSel)
	EVT_CHECKBOX(ID_SOUNDS_CHECK, wxOptionSounds::OnSoundsCheck)
END_EVENT_TABLE()

void wxOptionSounds::OnSoundsComboSel(wxCommandEvent &)
{
	wxInt32 sel = spSoundsCombo->GetSelection();

	//sanity
	wxASSERT(-1 != sel);

	wxString str = spSoundsCombo->GetString(sel);
	wxString current = CONFIG.read<wxString>(swConfigSectionGeneral, 
		swSoundFolder, swEmpty);

	//no need to do anything if they are the same
	if(str == current)
	{
		return;
	}

	//set the new name
	CONFIG.write<wxString>(swConfigSectionGeneral, swSoundFolder, str);
}

void wxOptionSounds::OnSoundsCheck(wxCommandEvent &)
{
	bool check = spSoundsCheck->GetValue();
	bool sounds = CONFIG.read<wxInt32>(swConfigSectionGeneral, 
		swPlaySounds, TRUE) || false;

	if(check == sounds)
	{
		return;
	}

	CONFIG.write<wxInt32>(swConfigSectionGeneral, swPlaySounds, 
		check ? TRUE : FALSE);
}

