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
#include "LoginDialog.h"
#include "BaseTextCtrl.h"
#include "BaseStaticText.h"
#include "BaseModalSizer.h"
#include "BaseButton.h"
#include "SkinManager.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	wxBaseTextCtrl* spEntryText;

	class Sizer : public BaseModalSizer
	{
	public:
		Sizer(wxWindow *window, const wxString& userName) 
			: BaseModalSizer(window) 
			, mUserName(userName)
		{}

		virtual void CreateSizers(wxBoxSizer *sizer)
		{
			wxBoxSizer* vert = new wxBoxSizer(wxVERTICAL);

			// Two horizontal sizers, one for each entry.
			wxBoxSizer* horz1 = new wxBoxSizer(wxHORIZONTAL);
			wxBoxSizer* horz2 = new wxBoxSizer(wxHORIZONTAL);

            static const wxString stUsername = _("Username");
			horz1->Add(new wxBaseStaticText(mpWindow, wxID_ANY,
				wxString(stUsername) + swColon), 0, wxRIGHT | wxTOP, 3);
			
			wxBaseTextCtrl* usernameText = new wxBaseTextCtrl(mpWindow,
				wxID_ANY, wxDefaultPosition, wxSize(150, 20));
			usernameText->SetValue(mUserName);
			usernameText->Disable();
			horz1->AddStretchSpacer();
			horz1->Add(usernameText, 0, wxRIGHT, 3);
			
            static const wxString stPassword = _("Password");
			horz2->Add(new wxBaseStaticText(mpWindow, wxID_ANY,
				wxString(stPassword) + swColon), 0, 
				wxRIGHT | wxTOP, 3);
			
			spEntryText = new wxBaseTextCtrl(mpWindow,
				wxID_ANY, wxDefaultPosition, wxSize(150, 20), wxTE_PASSWORD);
			horz2->AddStretchSpacer();
			horz2->Add(spEntryText, 0, wxRIGHT, 3);
			
			vert->Add(horz1, 1, wxEXPAND);
			vert->Add(horz2, 1, wxEXPAND);

			sizer->Add(vert, 1, wxEXPAND | wxALL, 3);
		}

		virtual void CreateButtons(wxBoxSizer *pSizer)
		{
            static const wxString stOK = _("OK");
			wxBaseButton *done = new wxBaseButton(mpWindow, wxID_OK, 
				stOK);
			pSizer->Add(done, 0, wxALL, 3);
			pSizer->AddStretchSpacer();
		}

	private:
		wxString mUserName;
	};
}

//---------------------------- PUBLIC           -----------------------------//
#ifdef __WIN32__
	//disables the 'this' : used in base member initializer list warning
	#pragma warning(disable:4355)
#endif
wxLoginDialog::wxLoginDialog(const wxString& userName)
: wxBaseModalDialog(NULL, ISizerPtr(new Sizer(this, userName)), 
	_("Enter Your Password"), swEnterYourPassword)
{
}

wxString wxLoginDialog::GetUserEntry() const
{
	return spEntryText->GetValue();
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
