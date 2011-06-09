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
#include "OptionDatabase.h"
#include "IOptionUI.h"

#include "Rule.h"
#include "RuleSetDatabase.h"

#include "IRuleEngine.h"
#include "DataObject.h"
#include "BaseStaticText.h"
#include "BaseComboBox.h"
#include "Map.h"
#include "IOptionEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	long sAllowedCards = -1;

	enum
	{
		ID_ALLOWED_CARDS_COMBO			= 0,
	};

	const wxString sOptions_Allowed[] = {sw3, sw4, sw5, sw6, sw7, sw8, sw9,
        sw10, sw11, sw12, sw13, sw14, sw15, sw16, sw17, sw18, sw19, sw20};

	wxBaseStaticText* spText_Allowed = NULL;
	wxBaseComboBox *spAllowedCardsCombo = NULL;
}

//---------------------------- PUBLIC           -----------------------------//
class AllowedCardsOption 
	: public wxEvtHandler
	, public IOptionUI
{
public:
	virtual void dependencies(StringPairArray& ) const
	{
	}

	virtual void silent_dependencies(StringPairArray&) const
	{
	}

	virtual void exclusions(StringPairArray& ) const
	{
	}

	virtual const wxString& description() const
	{
		return stChangeTheNumberOfCards;
	}

	virtual wxInt32 priority() const
	{
		return 20;
	}

	virtual wxString short_description() const
	{
		if(-1 == sAllowedCards)
		{
			sAllowedCards = MAP.read<wxInt32>(shAllowedCards);
		}

		wxString str = wxString::Format(stIntCardsAllowedInHand.c_str(), 
			sAllowedCards);

		return str;
	}

	virtual bool HasUI() const
	{
		return true;
	}

	virtual void CreateUI(wxWindow* parent, wxSizer* sizer)
	{
		if(-1 == sAllowedCards)
		{
			sAllowedCards = MAP.read<wxInt32>(shAllowedCards);
		}

		wxString init = wxString::Format(swIntFormat.c_str(), sAllowedCards);

		wxBoxSizer *vert = new wxBoxSizer(wxVERTICAL);
		wxBoxSizer *innerVert = new wxBoxSizer(wxVERTICAL);
		wxBoxSizer* innerHorz = new wxBoxSizer(wxHORIZONTAL);
		spText_Allowed = new wxBaseStaticText(parent, -1, 
            stCardsAllowedInHand + swColon);
		spAllowedCardsCombo = new wxBaseComboBox(parent, 
			ID_ALLOWED_CARDS_COMBO, init, wxDefaultPosition, wxDefaultSize, 
			_countof(sOptions_Allowed), sOptions_Allowed, wxCB_READONLY);
		
		innerVert->AddStretchSpacer();
		innerVert->Add(spText_Allowed);
		innerVert->AddStretchSpacer();
		innerHorz->Add(innerVert, 1, wxEXPAND | wxLEFT, 7);
		innerHorz->Add(spAllowedCardsCombo, 0, wxEXPAND | wxLEFT | wxRIGHT, 7);
		vert->Add(innerHorz, 1, wxEXPAND);
		
		sizer->Add(vert, 0);

		parent->Connect(ID_ALLOWED_CARDS_COMBO, ID_ALLOWED_CARDS_COMBO, 
			wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(AllowedCardsOption::OnAllowedCardsCombo));
	}

	virtual void DestroyUI(wxWindow* parent, wxSizer* sizer)
	{
		parent->Disconnect(ID_ALLOWED_CARDS_COMBO, ID_ALLOWED_CARDS_COMBO, 
			wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(AllowedCardsOption::OnAllowedCardsCombo));

		sizer->Remove(0);

		spText_Allowed->Destroy();
		spText_Allowed = NULL;

		spAllowedCardsCombo->Destroy();
		spAllowedCardsCombo = NULL;
	}

	virtual void UpdateUI(const wxString& key, const wxString& value)
	{
		wxASSERT(swAllowedCards == key);

		value.ToLong(&sAllowedCards);
	}

	void OnAllowedCardsCombo(wxCommandEvent &)
	{
		wxInt32 sel = spAllowedCardsCombo->GetSelection();
		wxASSERT(-1 != sel);

		wxString str = spAllowedCardsCombo->GetString(sel);
		
		Controller::get().Transmit(shEventUpdateOption, 
            DataObject(swAllowedCards, str));
	}

	virtual void SaveToXML(wxXmlNode* node)
	{
		wxXmlNode* child = new wxXmlNode(node, wxXML_ELEMENT_NODE,
			swAllowedCards);
		child->AddProperty(swValue, wxString::Format(swIntFormat.c_str(), 
			sAllowedCards));
	}

	virtual void LoadFromXML(const wxString& option, wxXmlNode* node)
	{
		wxXmlNode* child = node->GetChildren();
		wxString value;

		if( (swAllowedCards == child->GetName()) &&
			(true == child->GetPropVal(swValue, &value)))
		{
			RULE.Execute(shRuleSilentUpdateOption, DataObject(option,
				swAllowedCards, value));
		}
	}
};

IMPLEMENT_OPTION_UI(ALLOWED_CARDS, AllowedCardsOption);

//---------------------------------------------------------------------------//
// Class: RuleInitializeAllowedCards
// 
// Initializes allowed cards data.
//
// Derived From:
//     <Rule>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <AllowedCards>
//
// Mixin To:
//     <RuleInitializeOptions>
//
class RuleInitializeAllowedCards : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			playerGameData<wxInt32>(shAllowedCards, i) = sAllowedCards;
		}
	}
};

IMPLEMENT_RULE_MIXIN(RuleInitializeAllowedCards, RuleInitializeOptions, 
					 ALLOWED_CARDS)

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
