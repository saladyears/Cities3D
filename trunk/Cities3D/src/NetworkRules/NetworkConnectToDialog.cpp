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
#include "NetworkConnectToDialog.h"
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
	wxBaseTextCtrl *spNameText;
	wxBaseTextCtrl *spPortConnectTo;

	//initial sizer creation
	class Sizer_NetworkConnectTo : public BaseModalSizer
	{
	public:
		Sizer_NetworkConnectTo(wxWindow *window) : BaseModalSizer(window) {}

		virtual void CreateSizers(wxBoxSizer *pSizer)
		{
			const wxFont &font = SKIN.Font(shDefaultFont);
			const wxColour color = SKIN.Element(shDefaultText);

			wxBoxSizer *vert1 = new wxBoxSizer(wxVERTICAL);
			wxBoxSizer *horz1 = new wxBoxSizer(wxHORIZONTAL);

			//static texts
			wxBoxSizer *innerVert1 = new wxBoxSizer(wxVERTICAL);

			wxStaticText *text1 = new wxBaseStaticText(mpWindow, -1, 
				stConnectTo + swColon);
			wxStaticText *text2 = new wxBaseStaticText(mpWindow, -1, 
				wxString(stPort) + swColon);
			
			innerVert1->Add(text1, 0, wxALIGN_RIGHT | wxTOP | wxBOTTOM, 3);
			innerVert1->Add(text2, 0, wxALIGN_RIGHT | wxTOP, 10);			

			//text entry
			wxBoxSizer *innerVert2 = new wxBoxSizer(wxVERTICAL);

			spNameText = new wxBaseTextCtrl(mpWindow, -1, wxDefaultPosition, 
				wxSize(175, 20));
			spNameText->SetFont(font);
			spNameText->SetForegroundColour(color);
			
			spPortConnectTo = new wxBaseTextCtrl(mpWindow, -1, wxDefaultPosition, 
				wxSize(50, 20));
			spPortConnectTo->SetFont(font);
			spPortConnectTo->SetForegroundColour(color);

			innerVert2->Add(spNameText, 0, wxRIGHT, 3);
			innerVert2->Add(spPortConnectTo, 0, wxTOP, 6);

			wxBaseStaticText *explain = new wxFittedStaticText(mpWindow, -1, 
				stEnterAnIPAddress, shInfoText, shInfoFont, 
				wxSize(225, 33), wxST_NO_AUTORESIZE | wxALIGN_LEFT);

			horz1->Add(innerVert1, 0, wxEXPAND | wxALL, 3);
			horz1->Add(innerVert2, 1, wxEXPAND | wxALL, 3);

			//add it
			vert1->Add(horz1, 1, wxEXPAND | wxALL, 3);
			vert1->Add(explain, 0, wxEXPAND | wxALL, 3);

			pSizer->Add(vert1, 1, wxEXPAND | wxALL, 3);
		}
	};
}

//---------------------------- PUBLIC           -----------------------------//
#ifdef __WIN32__
	//disables the 'this' : used in base member initializer list warning
	#pragma warning(disable:4355)
#endif
wxNetworkConnectToDialog::wxNetworkConnectToDialog(wxWindow *parent)
: wxBaseModalDialog(parent, ISizerPtr(new Sizer_NetworkConnectTo(this)), 
	stConnectToAGame, swConnectToAGame)
{
	wxString str = CONFIG.read<wxString>(swConfigSectionNetwork, 
		swLastConnect, swEmpty);
	wxInt32 port = CONFIG.read<wxInt32>(swConfigSectionNetwork, 
		swLastPort, 7176);

	spPortConnectTo->SetValue(wxString::Format(swIntFormat.c_str(), port));
	spPortConnectTo->SetSelection(-1, -1);

	spNameText->SetValue(str);
	spNameText->SetSelection(-1, -1);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void wxNetworkConnectToDialog::Ok()
{
	//get player name to start
	wxString str = spNameText->GetValue().Strip(wxString::both);

	//sanity check
	if(true == str.IsEmpty())
	{
		Controller::get().Transmit(shEventMessageBox, 
            stYouMustEnterAnIPAddress);
		return;
	}

	wxString port = spPortConnectTo->GetValue().Strip(wxString::both);
	long num;
	port.ToLong(&num);

	if( (true == port.IsEmpty()) ||
		(false == port.IsNumber()) ||
		(0 > num) ||
		(65535 < num))
	{
		Controller::get().Transmit(shEventMessageBox,
			stThePortMustBeAValidNumber);
		return;
	}

	CONFIG.write(swConfigSectionNetwork, swLastConnect, str);
	CONFIG.write(swConfigSectionNetwork, swLastPort, static_cast<wxInt32>(num));

	mAddress.Hostname(str);
	mAddress.Service(num);

	wxBaseModalDialog::Ok();
}


