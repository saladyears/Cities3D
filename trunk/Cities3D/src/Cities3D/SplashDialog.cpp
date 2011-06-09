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
#include "SplashDialog.h"
#include "ImageEngine.h"
#include "GameEngine.h"
#include "Controller.h"
#include "BaseStaticText.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	typedef boost::scoped_array<wxImage> ImageArray;
	ImageArray sSplash;

	wxInt32 sWidth = 0;

	wxBaseStaticText* spText = NULL;
}

//---------------------------- PUBLIC           -----------------------------//
void wxSplashDialog::Create(wxWindow *parent)
{
	// Load up the splash image.
	sSplash.reset(new wxImage[1]);
	IMAGE.ReadImages(1, sSplash.get(), swUISpashTex);

	sWidth = sSplash[0].GetWidth();
	wxSize size(sWidth, sSplash[0].GetHeight());

	wxDialog::Create(parent, -1, swEmpty, wxDefaultPosition, wxDefaultSize, 0);

	SetBackgroundColour(wxColour(0, 0, 0));

	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	// Set the image.
	wxStaticBitmap* bitmap = new wxStaticBitmap(this, 0, wxBitmap(sSplash[0]), 
		wxPoint(0, 0), size);
	sizer->Add(bitmap, 0, wxEXPAND);

	// Add the load text.
    static const wxString stFindingPlugins = _("Finding plugins...");
	spText = new wxBaseStaticText(this, 0, stFindingPlugins);
	sizer->Add(spText, 0, wxEXPAND | wxLEFT | wxBOTTOM, 2);

	SetSizer(sizer);
	sizer->SetSizeHints(this);
	sizer->Layout();

	CenterOnScreen();
	Show(true);
	Update();
	wxYieldIfNeeded();

	Controller::get().AddReceiver(shEventFindPlugin, 
		&wxSplashDialog::OnFindPlugin, this);
}

wxSplashDialog::~wxSplashDialog()
{
	sSplash.reset();

	Controller::get().RemoveReceiver(shEventFindPlugin, 
		&wxSplashDialog::OnFindPlugin, this);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void wxSplashDialog::OnFindPlugin(const wxString& str)
{
	spText->SetSize(-1, -1, sWidth, spText->GetSize().y);
	spText->SetLabel(str);
	Update();
}
