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
#include "OptionAnimation.h"
#include "ConfigDatabase.h"
#include "BaseStaticBox.h"
#include "BaseCheckBox.h"
#include "FittedStaticText.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	enum
	{
		ID_BOARD_CHECK			 = 0,
	};

	wxBaseCheckBox *spBoardCheck;
}

//---------------------------- PUBLIC           -----------------------------//
wxOptionAnimation::wxOptionAnimation(wxWindow* parent)
: wxOptionWindow(parent, swAnimation)
{
	wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);

	wxBaseStaticBox *box = new wxBaseStaticBox(this, -1, stAnimation, 
		wxDefaultPosition, wxSize(200, 25));
	wxStaticBoxSizer *boxSizer = new wxStaticBoxSizer(box, wxHORIZONTAL);

	wxBoxSizer *innerVert = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *innerHorz1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *innerHorz2 = new wxBoxSizer(wxHORIZONTAL);

	bool board = CONFIG.read<wxInt32>(swConfigSectionUI, swAnimationBoard, TRUE) 
		|| false;

	spBoardCheck = new wxBaseCheckBox(this, ID_BOARD_CHECK, stPlayBoardAnimations);
	spBoardCheck->SetValue(board);
	
	innerHorz1->Add(spBoardCheck, 1, wxTOP | wxLEFT | wxRIGHT, 7);

	wxBaseStaticText *explainText = new wxFittedStaticText(this, -1, 
		stBoardAnimationsShow, shInfoText, shInfoFont,
		wxSize(200, 22), wxST_NO_AUTORESIZE | wxALIGN_LEFT);

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
BEGIN_EVENT_TABLE(wxOptionAnimation, wxOptionWindow)
	EVT_CHECKBOX(ID_BOARD_CHECK, wxOptionAnimation::OnBoardAnimation)
END_EVENT_TABLE()

void wxOptionAnimation::OnBoardAnimation(wxCommandEvent &)
{
	bool check = spBoardCheck->GetValue();

	CONFIG.write(swConfigSectionUI, swAnimationBoard, check ? TRUE : FALSE);
}
