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
#include "BaseGameDialog.h"
#include "Controller.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//
wxBaseGameDialog::wxBaseGameDialog(wxWindow *parent, const ISizerPtr &sizer, 
	const wxString &label, const wxString &config, long style)
: wxBaseDialog(parent, sizer, label, config, style | wxRESIZE_BORDER | 
	wxCAPTION)
{
	Controller::get().AddReceiver(shEventShutdownUI, 
		&wxBaseGameDialog::OnShutdownUI, this);
}

wxBaseGameDialog::~wxBaseGameDialog()
{
	Controller::get().RemoveReceiver(shEventShutdownUI, 
		&wxBaseGameDialog::OnShutdownUI, this);
	Controller::get().Transmit(shEventDirty, 0);
}

//---------------------------- PRIVATE          -----------------------------//
void wxBaseGameDialog::OnShutdownUI(wxInt32)
{
	Destroy();
}
