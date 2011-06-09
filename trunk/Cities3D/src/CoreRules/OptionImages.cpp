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
#include "OptionImages.h"
#include "Controller.h"
#include "ConfigDatabase.h"
#include "FilePath.h"
#include "SkinManager.h"
#include "BaseStaticBox.h"
#include "FittedStaticText.h"
#include "BaseComboBox.h"
#include "BaseCheckBox.h"
#include "WaitCursor.h"
#include "GLExport.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	enum
	{
		ID_IMAGE_COMBO		= 0,
		ID_MIPMAP_CHECK		,
	};

	wxComboBox *spImageCombo;
	wxCheckBox *spMipmapCheck;

	void PopulateImageCombo(wxWindow *parent)
	{
		wxArrayString dirs(FilePath::EnumDataSubDirs(swTex));

		wxString current = CONFIG.read<wxString>(swConfigSectionGeneral, 
			swTexFoler, swEmpty);

		if(true == current.IsEmpty())
		{
			current = swStandard;
		}

		//now that we've got a list, create the combo box with it
		spImageCombo = new wxBaseComboBox(parent, ID_IMAGE_COMBO, current, 
			wxDefaultPosition, wxSize(150, 25), 
			dirs.size(), &dirs[0], wxCB_READONLY);
	}
}

//---------------------------- PUBLIC           -----------------------------//
wxOptionImages::wxOptionImages(wxWindow* parent)
: wxOptionWindow(parent, swImages)
{
	wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);

	wxBaseStaticBox *box = new wxBaseStaticBox(this, -1, stGameImages, 
		wxDefaultPosition, wxSize(200, 50));
	wxStaticBoxSizer *boxSizer = new wxStaticBoxSizer(box, wxHORIZONTAL);
	
	wxBoxSizer *innerVert = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *innerHorz1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *innerHorz2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *innerHorz3 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *innerHorz4 = new wxBoxSizer(wxHORIZONTAL);

	wxBaseStaticText *images = new wxBaseStaticText(this, -1, stImages + 
		swColon);
	PopulateImageCombo(this);

	innerHorz1->Add(images, 0, wxTOP | wxLEFT, 7);
	innerHorz1->Add(spImageCombo, 1, wxALL | wxEXPAND, 4);

	wxBaseStaticText *explainText = new wxFittedStaticText(this, -1, 
		stSelectTheSetOfImages, shInfoText, shInfoFont,
		wxSize(200, 22), wxST_NO_AUTORESIZE | wxALIGN_LEFT);

	innerHorz2->Add(explainText, 1, wxEXPAND | wxLEFT | wxRIGHT, 7);

	wxBOOL mipmap = CONFIG.read<wxInt32>(swConfigSectionOpenGL, swMipmap, TRUE);
	spMipmapCheck = new wxBaseCheckBox(this, ID_MIPMAP_CHECK, 
		stSoftenTiles);
	spMipmapCheck->SetValue(mipmap || false);

	innerHorz3->Add(spMipmapCheck, 1, wxEXPAND | wxLEFT | wxRIGHT, 7);

	wxBaseStaticText *explainText2 = new wxFittedStaticText(this, -1, 
		stForMostImageSets, shInfoText, shInfoFont, wxSize(200, 33), 
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
BEGIN_EVENT_TABLE(wxOptionImages, wxOptionWindow)
	EVT_COMBOBOX(ID_IMAGE_COMBO, wxOptionImages::OnImageComboSel)
	EVT_CHECKBOX(ID_MIPMAP_CHECK, wxOptionImages::OnMipmapCheck)
END_EVENT_TABLE()

void wxOptionImages::OnImageComboSel(wxCommandEvent &)
{
	wxInt32 sel = spImageCombo->GetSelection();

	//sanity
	wxASSERT(-1 != sel);

	wxString str = spImageCombo->GetString(sel);
	wxString current = CONFIG.read<wxString>(swConfigSectionGeneral, swTexFoler, 
		swEmpty);

	//no need to do anything if they are the same
	if(str == current)
	{
		return;
	}

	//set the new name
	CONFIG.write<wxString>(swConfigSectionGeneral, swTexFoler, str);

	//update everything
	wxWaitCursor cursor(this);
	Controller::get().Transmit(shEventImageUpdate, false);
}

void wxOptionImages::OnMipmapCheck(wxCommandEvent &)
{
	bool check = spMipmapCheck->GetValue();
	bool mipmap = CONFIG.read<wxInt32>(swConfigSectionOpenGL, swMipmap, 
		TRUE) || false;

	if(check == mipmap)
	{
		return;
	}

	CONFIG.write<wxInt32>(swConfigSectionOpenGL, swMipmap, check ? TRUE : FALSE);

	wxWaitCursor cursor(this);
	Controller::get().Transmit(shEventImageUpdate, false);
}


