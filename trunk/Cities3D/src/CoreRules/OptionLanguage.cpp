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
#include "OptionLanguage.h"
#include "ConfigDatabase.h"
#include "LanguageDatabase.h"
#include "SkinManager.h"
#include "BaseStaticBox.h"
#include "FittedStaticText.h"
#include "BaseComboBox.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	enum
	{
		ID_LANGUAGE_COMBO		= 0,
	};

	wxComboBox *spLanguageCombo;
	
	void PopulateLanguageCombo(wxWindow *parent)
	{
		const LanguageDatabase::LanguageInfoArray &languages =
			LanguageDatabase::get().GetLanguageInfo();

		// Prepare the list of languages.
		StringArray choices;

		LanguageDatabase::LanguageInfoArray::const_iterator it, 
			itEnd = languages.end();
		for(it = languages.begin(); it != itEnd; ++it)
		{
			choices.push_back(it->Description);
		}

		wxString current = CONFIG.read<wxString>(swConfigSectionGeneral, swLanguage,
			swEmpty);

		// This should never happen, but be safe.
		if(true == current.IsEmpty())
		{
			current = swEnglishUS;
		}

		spLanguageCombo = new wxBaseComboBox(parent, ID_LANGUAGE_COMBO, 
			current, wxDefaultPosition, wxSize(150, 25), choices.size(), 
			&choices[0], wxCB_READONLY);
	}
}

//---------------------------- PUBLIC           -----------------------------//
wxOptionLanguage::wxOptionLanguage(wxWindow* parent)
: wxOptionWindow(parent, swLanguage)
{
	wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);

	wxBaseStaticBox *box = new wxBaseStaticBox(this, -1, 
		stSelectALanguage, wxDefaultPosition, wxSize(200, 50));
	wxStaticBoxSizer *boxSizer = new wxStaticBoxSizer(box, wxHORIZONTAL);
	
	wxBoxSizer *innerVert = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *innerHorz1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *innerHorz2 = new wxBoxSizer(wxHORIZONTAL);
	
	wxBaseStaticText *text = new wxBaseStaticText(this, -1, stLanguage + swColon);
	PopulateLanguageCombo(this);

	innerHorz1->Add(text, 0, wxTOP | wxLEFT, 7);
	innerHorz1->Add(spLanguageCombo, 1, wxALL | wxEXPAND, 4);

	wxBaseStaticText *explainText = new wxFittedStaticText(this, -1, 
		stSelectTheLanguage, shInfoText, shInfoFont, wxSize(200, 33), 
		wxST_NO_AUTORESIZE | wxALIGN_LEFT);

	innerHorz2->Add(explainText, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 7);

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
BEGIN_EVENT_TABLE(wxOptionLanguage, wxOptionWindow)
	EVT_COMBOBOX(ID_LANGUAGE_COMBO, wxOptionLanguage::OnLanguageComboSel)
END_EVENT_TABLE()

void wxOptionLanguage::OnLanguageComboSel(wxCommandEvent &)
{
	wxInt32 sel = spLanguageCombo->GetSelection();

	//sanity
	wxASSERT(-1 != sel);

	wxString str = spLanguageCombo->GetString(sel);
	wxString current = CONFIG.read<wxString>(swConfigSectionGeneral, 
		swLanguage, swEmpty);

	//no need to do anything if they are the same
	if(str == current)
	{
		return;
	}

	//set the new name
	CONFIG.write<wxString>(swConfigSectionGeneral, swLanguage, str);
}
