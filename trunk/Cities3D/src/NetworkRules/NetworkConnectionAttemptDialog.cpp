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
#include "NetworkConnectionAttemptDialog.h"
#include "ISizer.h"
#include "FittedStaticText.h"
#include "BaseButton.h"
#include "Controller.h"
#include "GameEngine.h"
#include "INetworkClient.h"
#include "TimerManager.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	wxBaseStaticText* spAttemptText = NULL;
	wxBaseButton* spOKButton = NULL;
	wxBaseStaticText* spEventText = NULL;

	// Try this many times before giving up.
	wxInt32 sAttempt = 0;
	wxInt32 sMaxAttempts = 5;

	class Sizer_NetworkConnectionAttempt : public ISizer
	{
	public:
		Sizer_NetworkConnectionAttempt(wxWindow *window, 
			const wxIPV4address& address) 
			: ISizer(window)
			, mAddress(address)
		{}

		virtual void CreateLayout(wxBoxSizer *pSizer)
		{
			wxBoxSizer* vert = new wxBoxSizer(wxVERTICAL);

			spAttemptText = new wxFittedStaticText(mpWindow, wxID_ANY,
				stAttemptOf.c_str(), shDefaultText, shDefaultFont, 
				wxSize(250, 22), wxST_NO_AUTORESIZE | wxALIGN_CENTER);

			wxString str = wxString::Format(
				stAttemptingToConnectTo.c_str(),
				mAddress.IPAddress().c_str(), mAddress.Service());

			wxBaseStaticText *text = new wxFittedStaticText(mpWindow, wxID_ANY, 
				str, shDefaultText, shDefaultFont, wxSize(250, 22), 
				wxST_NO_AUTORESIZE | wxALIGN_CENTER);

			vert->Add(spAttemptText, 1, wxEXPAND | wxALL | wxALIGN_CENTER, 4);
			vert->Add(text, 1, wxEXPAND | wxALL | wxALIGN_CENTER, 4);

			spEventText = new wxBaseStaticText(mpWindow, wxID_ANY, stWaiting,
				shDefaultText, shDefaultFont, wxSize(300, 11), wxALIGN_CENTER);

			vert->Add(spEventText, 1, wxEXPAND | wxALL | wxALIGN_CENTER, 4);

			wxBoxSizer* horz = new wxBoxSizer(wxHORIZONTAL);
			horz->AddStretchSpacer();

			spOKButton = new wxBaseButton(mpWindow, wxID_OK, stOK);
			horz->Add(spOKButton, 0);
			spOKButton->Disable();

			horz->AddStretchSpacer();
			vert->Add(horz, 0, wxEXPAND | wxALL, 4);

			pSizer->Add(vert, 1, wxEXPAND);
		}

	private:
		wxIPV4address mAddress;
	};
}

//---------------------------- PUBLIC           -----------------------------//
#ifdef __WIN32__
//disables the 'this' : used in base member initializer list warning
#pragma warning(disable:4355)
#endif
wxNetworkConnectionAttemptDialog::wxNetworkConnectionAttemptDialog(
	wxWindow *parent, const wxIPV4address& address, INetworkClient* client)
: wxBaseModalDialog(parent, ISizerPtr(new Sizer_NetworkConnectionAttempt(
	this, address)), stConnectingToAGame, swConnectingToAGame)
, mpNetworkClient(client)
, mAddress(address)
{
	wxASSERT(NULL != mpNetworkClient);

	Controller::get().AddReceiver(shEventNetworkConnectionEvent, 
		&wxNetworkConnectionAttemptDialog::OnNetworkConnectionEvent, this);

	// Disable the close button until we get an event.
	EnableCloseButton(false);

	// Add the one shot callback so we get a modal dialog.
	TimerManager::get().AddOneShotFunction(
		&wxNetworkConnectionAttemptDialog::OnTimer, this, 1);
}

wxNetworkConnectionAttemptDialog::~wxNetworkConnectionAttemptDialog()
{
	Controller::get().RemoveReceiver(shEventNetworkConnectionEvent, 
		&wxNetworkConnectionAttemptDialog::OnNetworkConnectionEvent, this);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void wxNetworkConnectionAttemptDialog::OnNetworkConnectionEvent(
	const wxString& str)
{
	wxString text = wxString::Format(stAttempt.c_str(), sAttempt + 1, 
		str.c_str());
	spEventText->SetLabel(text);
	Layout();
}

void wxNetworkConnectionAttemptDialog::OnTimer()
{
	bool success = false;

	for(sAttempt = 0; sAttempt < sMaxAttempts; ++sAttempt)
	{
		wxString str = wxString::Format(stAttemptOf.c_str(), sAttempt + 1, 
			sMaxAttempts);
		spAttemptText->SetLabel(str);

		// Immediately attempt to connect.
		success = mpNetworkClient->Connect(mAddress);

		if(true == success)
		{
			break;
		}
	}

	// If we successfully connected, immediately close.
	if(true == success)
	{
		EndModal(wxID_OK);
	}
	// Otherwise, let the user read the message and close.
	else
	{
		// If we get here, we tried a lot and couldn't connect.
		EnableCloseButton(true);
		spOKButton->Enable();
	}
}
