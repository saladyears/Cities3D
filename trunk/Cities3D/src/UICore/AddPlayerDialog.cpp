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
#include "AddPlayerDialog.h"
#include "BaseTextCtrl.h"
#include "BaseStaticText.h"
#include "BaseModalSizer.h"
#include "Controller.h"
#include "Player.h"
#include "PlayerDatabase.h"
#include "SkinManager.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	//current player
	Player sPlayer;

	//controls
	wxBaseTextCtrl *spNameText;

	//initial sizer creation
	class Sizer_AddPlayerDialog : public BaseModalSizer
	{
	public:
		Sizer_AddPlayerDialog(wxWindow *window) : BaseModalSizer(window) {}

		virtual void CreateSizers(wxBoxSizer *pSizer)
		{
			wxBoxSizer *infoSizer = new wxBoxSizer(wxVERTICAL);

			//create the sections
			createName(infoSizer);

			pSizer->Add(infoSizer, 1, wxEXPAND | wxALL, 3);
		}

		void createName(wxBoxSizer *pSizer)
		{
			const wxFont &font = SKIN.Font(shDefaultFont);
			const wxColour color = SKIN.Element(shDefaultText);

            static const wxString stName = _("Name");
	
			//horizontal sizer for handle
			wxBoxSizer *handle = new wxBoxSizer(wxHORIZONTAL);
			handle->Add(new wxBaseStaticText(mpWindow, -1, 
				wxString(stName) + swColon), 0, wxRIGHT | 
				wxTOP, 3);
			spNameText = new wxBaseTextCtrl(mpWindow, -1, wxDefaultPosition, 
				wxSize(150, 20));
			spNameText->SetFont(font);
			spNameText->SetForegroundColour(color);
			handle->Add(spNameText, 0, wxRIGHT, 3);

			//add it
			pSizer->Add(handle, 1, wxALIGN_TOP | wxEXPAND | wxALL, 3);
		};
	};
}

//---------------------------- PUBLIC           -----------------------------//
#ifdef __WIN32__
	//disables the 'this' : used in base member initializer list warning
	#pragma warning(disable:4355)
#endif
wxAddPlayerDialog::wxAddPlayerDialog(wxWindow *parent)
	: wxBaseModalDialog(parent, ISizerPtr(new Sizer_AddPlayerDialog(this)), 
	_("Add A Player"), swAddAPlayer)
{
	sPlayer = Player();
}

const Player &wxAddPlayerDialog::GetPlayer()
{
	return sPlayer;
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void wxAddPlayerDialog::Ok()
{
	wxString str;

	//get player name to start
	str = spNameText->GetValue().Strip(wxString::both);

	//sanity check
	if(true == str.IsEmpty())
	{
        static const wxString stYourNameCannotBeBlank = _("Your name cannot be blank or all spaces.");
	
		Controller::get().Transmit(shEventMessageBox, stYourNameCannotBeBlank);
		return;
	}

	if(20 < str.Length())
	{
        static const wxString stYourNameMustBe20OrLess = _("Your name must be 20 characters or less.");

		Controller::get().Transmit(shEventMessageBox, stYourNameMustBe20OrLess);
		return;
	}

	sPlayer = Player(str);

	//add them to the database
	PlayerDatabase::get().AddPlayer(sPlayer);

	wxBaseModalDialog::Ok();
}
