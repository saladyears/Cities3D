/*
 *  Cities3D - Copyright (C) 2001-2009 Jason Fugate (saladyears@gmail.com)
 * 
 *  This program is free software; you can redistribute it and/or modify it 
 *  under the terms of the GNU General Public License as published by the Free 
 *  Software Foundation; either version 2 of the License, or (at your option) 
 *  any later version.
 * 
 *  This program is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; ithout even the implied warranty of MERCHANTABILITY 
 *  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License 
 *  for more details.
 */
#include "stdwx.h"
#include "style.h"	//READ THIS BEFORE MAKING ANY CHANGES TO THIS FILE!!!

//---------------------------- SYSTEM INCLUDES  -----------------------------//

//---------------------------- USER INCLUDES    -----------------------------//
#include "BaseModalDialog.h"
#include "Controller.h"

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

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//
wxBaseModalDialog::wxBaseModalDialog(wxWindow *parent, const ISizerPtr &sizer, 
	const wxString &label, const wxString &config, long style)
: wxBaseDialog(parent, sizer, label, config, style | wxRESIZE_BORDER | 
			   wxCAPTION)
{
	Controller::get().AddReceiver(shEventKeyDown, 
		&wxBaseModalDialog::OnKeyDown, this);
}

wxBaseModalDialog::~wxBaseModalDialog()
{
	Controller::get().RemoveReceiver(shEventKeyDown, 
		&wxBaseModalDialog::OnKeyDown, this);
}

//default handlers, which may be overridden
void wxBaseModalDialog::Ok()
{
	EndModal(wxID_OK);
}

void wxBaseModalDialog::Cancel()
{
	EndModal(wxID_CANCEL);
}

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxBaseModalDialog, wxBaseDialog)
	EVT_BUTTON(wxID_OK, wxBaseModalDialog::OnOk)
	EVT_BUTTON(wxID_CANCEL, wxBaseModalDialog::OnCancel)
END_EVENT_TABLE()

void wxBaseModalDialog::OnOk(wxCommandEvent &)
{
	Ok();
}

void wxBaseModalDialog::OnCancel(wxCommandEvent &)
{
	Cancel();
}

void wxBaseModalDialog::OnKeyDown(const wxKeyEvent &event)
{
#ifdef __WIN32__
	if (!GetHWND())
		return;
#endif
	wxASSERT(TRUE == IsModal());

	if(true == IsActive())
	{
		// "Esc" works as an accelerator for the "Cancel" button, but it
		// shouldn't close the dialog which doesn't have any cancel button
		if(WXK_ESCAPE == event.m_keyCode)
		{
			wxWindow *window = FindWindow(wxID_CANCEL);

			if( (NULL != window) &&
				(true == window->IsEnabled()))
			{
				Cancel();
			}
		}
		// Same thing for Return keys.
		else if( (WXK_RETURN == event.m_keyCode) ||
				 (WXK_NUMPAD_ENTER == event.m_keyCode))
		{
			wxWindow *window = FindWindow(wxID_OK);

			if( (NULL != window) &&
				(true == window->IsEnabled()))
			{
				Ok();
			}
		}
	}
}
