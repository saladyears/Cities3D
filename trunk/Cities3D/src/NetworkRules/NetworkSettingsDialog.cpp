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
#include "NetworkSettingsDialog.h"
#include "BaseTextCtrl.h"
#include "FittedStaticText.h"
#include "BaseModalSizer.h"
#include "ConfigDatabase.h"
#include "SkinManager.h"
#include "INetworkEngine.h"
#include "Controller.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	//controls
	wxBaseTextCtrl *spHostname;
	wxBaseTextCtrl *spPortSettings;

	//initial sizer creation
	class Sizer_NetworkSettings : public BaseModalSizer
	{
	public:
		Sizer_NetworkSettings(wxWindow *window) : BaseModalSizer(window) {}

		virtual void CreateSizers(wxBoxSizer *pSizer)
		{
			wxBoxSizer *vert1 = new wxBoxSizer(wxVERTICAL);

			// IP Address
			wxBoxSizer *horz1 = new wxBoxSizer(wxHORIZONTAL);
			horz1->Add(new wxBaseStaticText(mpWindow, -1, 
				wxString(stHostname) + swColon),
				0, wxRIGHT | wxTOP, 3);
			spHostname = new wxBaseTextCtrl(mpWindow, -1, wxDefaultPosition, 
				wxSize(150, 20));
			horz1->Add(spHostname, 0, wxRIGHT, 3);
			
			// Port
			wxBoxSizer *horz2 = new wxBoxSizer(wxHORIZONTAL);
			horz2->Add(new wxBaseStaticText(mpWindow, -1, 
				wxString(stPort) + swColon), 0, 
				wxRIGHT | wxTOP, 3);
			spPortSettings = new wxBaseTextCtrl(mpWindow, -1, wxDefaultPosition, 
				wxSize(50, 20));
			horz2->Add(spPortSettings, 0, wxRIGHT, 3);

			wxBaseStaticText *explain = new wxFittedStaticText(mpWindow, -1, 
				stEnterAValidHostname, shInfoText, shInfoFont,	
                wxSize(200, 22), wxST_NO_AUTORESIZE | wxALIGN_LEFT);

			vert1->Add(horz1, 1, wxEXPAND | wxALL, 3);
			vert1->Add(horz2, 1, wxEXPAND | wxALL, 3);
			vert1->Add(explain, 0, wxEXPAND | wxALL, 3);

			//add it
			pSizer->Add(vert1, 1, wxALIGN_TOP | wxEXPAND | wxALL, 3);
		}
	};
}

//---------------------------- PUBLIC           -----------------------------//
#ifdef __WIN32__
	//disables the 'this' : used in base member initializer list warning
	#pragma warning(disable:4355)
#endif
wxNetworkSettingsDialog::wxNetworkSettingsDialog(wxWindow *parent)
: wxBaseModalDialog(parent, ISizerPtr(new Sizer_NetworkSettings(this)), 
	stServerSettings, swServerSettings)
{
	wxInt32 port = CONFIG.read<wxInt32>(swConfigSectionNetwork, swHostPort, 7176);
	wxString str = CONFIG.read<wxString>(swConfigSectionNetwork, swHostAddress, 
		swEmpty);

	if(true == str.IsEmpty())
	{
		// Try and find a default name.
		mAddress.AnyAddress();
		if(swZeroIP == mAddress.IPAddress())
		{
			// dont try to resolve a NULL hostname
			str = mAddress.IPAddress();
		}
		else
		{
			str = mAddress.Hostname();
		}
	}

	spHostname->SetValue(str);
	spHostname->SetSelection(-1, -1);

    spPortSettings->SetValue(wxString::Format(swIntFormat.c_str(), port));
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void wxNetworkSettingsDialog::Ok()
{
	wxString str = spHostname->GetValue().Strip(wxString::both);
	
	// Attempt to set it in the outgoing address.
	if( (true == str.IsEmpty()) ||
		(false == mAddress.Hostname(str)))
	{
		Controller::get().Transmit(shEventMessageBox,
			stYouMustEnterAValidHostname);
		return;
	}

	wxString hostname = str;

	str = spPortSettings->GetValue().Strip(wxString::both);

	//sanity check
	if( (true == str.IsEmpty()) ||
		(false == str.IsNumber()))
	{
		Controller::get().Transmit(shEventMessageBox, stYouMustEnterAPort);
		return;
	}

	long port;
	str.ToLong(&port);
	
	// Set it in the outgoing address.
	mAddress.Service(port);

	CONFIG.write<wxInt32>(swConfigSectionNetwork, swHostPort, port);
	CONFIG.write(swConfigSectionNetwork, swHostAddress, hostname);

	wxBaseModalDialog::Ok();
}


