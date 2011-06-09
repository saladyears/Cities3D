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
#include "BaseMessageBox.h"
#include "ISizer.h"
#include "FittedStaticText.h"
#include "BaseButton.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	class Sizer_BaseMessageBox : public ISizer
	{
	public:
		Sizer_BaseMessageBox(wxWindow *window, const wxString& text, 
			MessageBoxType type) 
			: ISizer(window)
			, mText(text)
			, mType(type)
		{}

		virtual void CreateLayout(wxBoxSizer *pSizer)
		{
			wxBoxSizer* vert = new wxBoxSizer(wxVERTICAL);

			wxBaseStaticText *text = new wxFittedStaticText(mpWindow, -1, 
				mText, shDefaultText, shDefaultFont, wxSize(250, 22), 
				wxST_NO_AUTORESIZE | wxALIGN_CENTER);

			vert->Add(text, 1, wxEXPAND | wxALL | wxALIGN_CENTER, 4);

			wxBoxSizer* horz = new wxBoxSizer(wxHORIZONTAL);
			horz->AddStretchSpacer();

			// Add buttons depending on type.
			switch(mType)
			{
			case MessageBoxOK:
				{
                    static const wxString stOK = _("OK");
	
					wxBaseButton *button = new wxBaseButton(mpWindow, wxID_OK, 
						stOK);
					horz->Add(button, 0);
				}

				break;
			case MessageBoxYesNo:
				{
                    static const wxString stYes = _("Yes");
                    static const wxString stNo = _("No");
	
					wxBaseButton *yes = new wxBaseButton(mpWindow, wxID_YES, 
						stYes);
					horz->Add(yes, 0, wxRIGHT, 4);

					wxBaseButton *no = new wxBaseButton(mpWindow, wxID_NO, 
						stNo);
					horz->Add(no, 0, wxLEFT, 4);
				}
				break;
			default:
				wxASSERT(false);
				break;
			}

			horz->AddStretchSpacer();
			vert->Add(horz, 0, wxEXPAND | wxALL, 4);

			pSizer->Add(vert, 1, wxEXPAND);
		}

	private:
		wxString mText;
		MessageBoxType mType;
	};
}

//---------------------------- PUBLIC           -----------------------------//
#ifdef __WIN32__
	//disables the 'this' : used in base member initializer list warning
	#pragma warning(disable:4355)
#endif
wxBaseMessageBox::wxBaseMessageBox(wxWindow *parent, const wxString& label, 
	const wxString& text, MessageBoxType type)
: wxBaseModalDialog(parent, ISizerPtr(new Sizer_BaseMessageBox(this, text, 
	type)), label, swMessageBox)
, mType(type)
{

}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxBaseMessageBox, wxBaseModalDialog)
	EVT_BUTTON(wxID_YES, wxBaseMessageBox::OnYes)
	EVT_BUTTON(wxID_NO, wxBaseMessageBox::OnNo)
END_EVENT_TABLE()

void wxBaseMessageBox::OnYes(wxCommandEvent&)
{
	EndModal(wxYES);
}

void wxBaseMessageBox::OnNo(wxCommandEvent&)
{
	EndModal(wxNO);
}
