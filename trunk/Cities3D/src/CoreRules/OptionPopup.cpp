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
#include "OptionPopup.h"
#include "ConfigDatabase.h"
#include "BaseStaticBox.h"
#include "BaseRadioButton.h"
#include "BaseComboBox.h"
#include "FittedStaticText.h"
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
		ID_ALWAYS_RADIO			= 0,
		ID_HIDE_RADIO			,
		ID_TIME_COMBO			,
	};

	const wxString sOptions[] = 
	{
		sw1, sw2, sw3, sw4, sw5, sw6, sw7, sw8, sw9, sw10
	}; 
	
	wxBaseRadioButton *spAlwaysRadio;
	wxBaseRadioButton *spHideRadio;
	wxBaseComboBox* spTimeCombo;
}

//---------------------------- PUBLIC           -----------------------------//
wxOptionPopup::wxOptionPopup(wxWindow* parent)
: wxOptionWindow(parent, swPopup)
{
	wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);

	wxBaseStaticBox *box = new wxBaseStaticBox(this, -1, stPopups, 
		wxDefaultPosition, wxSize(200, 25));
	wxStaticBoxSizer *boxSizer = new wxStaticBoxSizer(box, wxHORIZONTAL);

	wxBoxSizer *innerVert = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *innerHorz1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *innerHorz2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *innerHorz3 = new wxBoxSizer(wxHORIZONTAL);

	bool always = CONFIG.read<wxInt32>(swConfigSectionUI, swPopups, TRUE) 
		|| false;

	spAlwaysRadio = new wxBaseRadioButton(this, ID_ALWAYS_RADIO, 
		stShowPopups, shDefaultText, shDefaultFont, wxDefaultPosition, 
		wxDefaultSize, wxRB_GROUP);

	spHideRadio = new wxBaseRadioButton(this, ID_HIDE_RADIO, 
		stHidePopups);

	if(true == always)
	{
		spAlwaysRadio->SetValue(true);
	}
	else
	{
		spHideRadio->SetValue(true);
	}

	innerHorz1->Add(spAlwaysRadio, 1, wxTOP | wxLEFT | wxRIGHT, 7);

	wxBaseStaticText *explainText = new wxFittedStaticText(this, -1, 
		stRecommendedForPlayers, shInfoText, shInfoFont,
		wxSize(200, 22), wxST_NO_AUTORESIZE | wxALIGN_LEFT);

	innerHorz2->Add(explainText, 1, wxEXPAND | wxLEFT | wxRIGHT, 7);
	
	innerHorz3->Add(spHideRadio, 1, wxEXPAND | wxLEFT | wxRIGHT, 7);

	innerVert->Add(innerHorz1, 1, wxEXPAND);
	innerVert->Add(innerHorz2, 1, wxEXPAND);
	innerVert->Add(innerHorz3, 1, wxEXPAND);
	boxSizer->Add(innerVert, 1, wxEXPAND);

	wxBaseStaticBox *box2 = new wxBaseStaticBox(this, -1, stOptions, 
		wxDefaultPosition, wxSize(200, 25));
	wxStaticBoxSizer *boxSizer2 = new wxStaticBoxSizer(box2, wxHORIZONTAL);

	wxString time = CONFIG.read<wxString>(swConfigSectionUI, swPopupTimeout, sw3);

	wxBoxSizer *innerVert2 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *innerVert3 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* innerHorz5 = new wxBoxSizer(wxHORIZONTAL);
	wxBaseStaticText* text = new wxBaseStaticText(this, -1, 
		stSecondsBeforePopups + swColon);
	spTimeCombo = new wxBaseComboBox(this, ID_TIME_COMBO, time, 
		wxDefaultPosition, wxDefaultSize, 10, sOptions, wxCB_READONLY);
	
	innerVert2->Add(innerHorz5, 1, wxEXPAND);
	innerVert3->AddStretchSpacer();
	innerVert3->Add(text);
	innerVert3->AddStretchSpacer();
	innerHorz5->Add(innerVert3, 1, wxEXPAND | wxLEFT, 7);
	innerHorz5->Add(spTimeCombo, 0, wxEXPAND | wxLEFT | wxRIGHT, 7);
	boxSizer2->Add(innerVert2, 1, wxEXPAND);

	pSizer->Add(boxSizer, 0, wxEXPAND);
	pSizer->Add(boxSizer2, 0, wxEXPAND | wxTOP, 7);
	
	SetSizer(pSizer);
	pSizer->SetSizeHints(this);
	pSizer->Layout();

	Controller::get().AddReceiver(shEventPopupToggle, 
		&wxOptionPopup::OnPopupToggle, this);
}

wxOptionPopup::~wxOptionPopup()
{
	Controller::get().RemoveReceiver(shEventPopupToggle, 
		&wxOptionPopup::OnPopupToggle, this);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxOptionPopup, wxOptionWindow)
	EVT_RADIOBUTTON(ID_ALWAYS_RADIO, wxOptionPopup::OnAlwaysRadio)
	EVT_RADIOBUTTON(ID_HIDE_RADIO, wxOptionPopup::OnHideRadio)
	EVT_COMBOBOX(ID_TIME_COMBO, wxOptionPopup::OnTimeCombo)
END_EVENT_TABLE()

void wxOptionPopup::OnPopupToggle(bool flag)
{
	if(true == flag)
	{
		spHideRadio->SetValue(false);
		spAlwaysRadio->SetValue(true);
	}
	else
	{
		spAlwaysRadio->SetValue(false);
		spHideRadio->SetValue(true);
	}

	Refresh();
}

void wxOptionPopup::OnAlwaysRadio(wxCommandEvent &)
{
	bool check = spAlwaysRadio->GetValue();

	if(true == check)
	{
		CONFIG.write(swConfigSectionUI, swPopups, TRUE);
	}
}

void wxOptionPopup::OnHideRadio(wxCommandEvent &)
{
	bool check = spHideRadio->GetValue();

	if(true == check)
	{
		CONFIG.write(swConfigSectionUI, swPopups, FALSE);
	}
}

void wxOptionPopup::OnTimeCombo(wxCommandEvent &)
{
	wxInt32 sel = spTimeCombo->GetSelection();
	wxASSERT(-1 != sel);

	wxString str = spTimeCombo->GetString(sel);

	wxString current = CONFIG.read<wxString>(swConfigSectionUI, swPopupTimeout, 
		sw3);

	if(str != current)
	{
		//set the new name
		CONFIG.write<wxString>(swConfigSectionUI, swPopupTimeout, str);
	}
}
