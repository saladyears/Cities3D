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
#include "SelectLanguageDialog.h"
#include "BaseModalSizer.h"
#include "ConfigDatabase.h"
#include "LanguageDatabase.h"
#include "BaseComboBox.h"
#include "BaseButton.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	wxBaseComboBox *spLanguageComboBox;

	//initial sizer creation
	class Sizer : public BaseModalSizer
	{
	public:
		Sizer(wxWindow *window) : BaseModalSizer(window) {}

		virtual void CreateSizers(wxBoxSizer *pSizer)
		{
			wxBoxSizer *vert1 = new wxBoxSizer(wxVERTICAL);

			const LanguageDatabase::LanguageInfoArray &languages =
				LanguageDatabase::get().GetLanguageInfo();

			// Prepare the list of languages.
			StringArray choices;

			LanguageDatabase::LanguageInfoArray::const_iterator it, 
				itEnd = languages.end();
			for(it = languages.begin(); it != itEnd; ++it)
			{
				choices.push_back(it->Description);
			}

			spLanguageComboBox = new wxBaseComboBox(mpWindow, -1, 
				swEnglishUS, wxDefaultPosition, wxSize(150, 20), 
				choices.size(), &choices[0], wxCB_READONLY);

			vert1->Add(spLanguageComboBox, 1, wxEXPAND | wxALL, 3);
			
			//add it
			pSizer->Add(vert1, 1, wxALIGN_TOP | wxEXPAND | wxALL, 3);
		}

		virtual void CreateButtons(wxBoxSizer *pSizer)
		{
            static const wxString stOK = _("OK");

			// Only use an OK button.
			wxBaseButton *ok = new wxBaseButton(mpWindow, wxID_OK, stOK);
			pSizer->Add(ok, 0, wxALL, 3);
		}
	};
}

//---------------------------- PUBLIC           -----------------------------//
#ifdef __WIN32__
	//disables the 'this' : used in base member initializer list warning
	#pragma warning(disable:4355)
#endif
wxSelectLanguageDialog::wxSelectLanguageDialog(wxWindow *parent)
: wxBaseModalDialog(parent, ISizerPtr(new Sizer(this)), _("Select a Language"),
	swSelectALanguage, 0)
{
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void wxSelectLanguageDialog::Ok()
{
	wxString str = spLanguageComboBox->GetValue();

	// Store their language selection.
	CONFIG.write<wxString>(swConfigSectionGeneral, swLanguage, str);

	wxBaseModalDialog::Ok();
}
