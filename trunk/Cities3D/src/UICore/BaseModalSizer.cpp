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
#include "BaseModalSizer.h"
#include "BaseButton.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
void BaseModalSizer::CreateLayout(wxBoxSizer *sizer)
{
	// Create the body of the dialog first.
	CreateSizers(sizer);

	// Then tack on the modal buttons.
	wxBoxSizer *buttonSizer = new wxBoxSizer(wxVERTICAL);
	CreateButtons(buttonSizer);
	sizer->Add(buttonSizer, 0, wxEXPAND | wxALIGN_RIGHT | wxALL, 3);
}
//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void BaseModalSizer::CreateButtons(wxBoxSizer *pSizer)
{
    static const wxString stOK = _("OK");
    static const wxString stCancel = _("Cancel");
	
	//add OK/Cancel buttons
	wxBaseButton *ok = new wxBaseButton(mpWindow, wxID_OK, stOK);
	wxBaseButton *cancel = new wxBaseButton(mpWindow, wxID_CANCEL, 
		stCancel);

	pSizer->Add(ok, 0, wxALL, 3);
	pSizer->Add(cancel, 0, wxALL, 3);
}
