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

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	struct PoliticsCard
	{
		wxInt32 id;
		wxBaseStaticText* text;
		wxBaseComboBox* combo;
		long value;
		long original;
		wxString type;
		wxString name;
	};

	enum
	{
		Arsonist			= 0,
		Bishop				,
		Constitution		,
		Deserter			,
		Diplomat			,
		Intrigue			,
		Saboteur			,
		Spy					,
		Warlord				,
		Wedding				,

		NumPoliticsCards	,
	};

	const wxString sOptions_PoliticsCards[] = {sw0, sw1, sw2, sw3, sw4, sw5,
        sw6, sw7, sw8, sw9, sw10};

	PoliticsCard sPoliticsCards[] =
	{
		{Arsonist, 0, 0, 2, 2, swArsonist, stArsonist},
		{Bishop, 0, 0, 2, 2, swBishop, stBishop},
		{Constitution, 0, 0, 1, 1, swConstitution, stConstitution},
		{Deserter, 0, 0, 2, 2, swDeserter, stDeserter},
		{Diplomat, 0, 0, 2, 2, swDiplomat, stDiplomat},
		{Intrigue, 0, 0, 2, 2, swIntrigue, stIntrigue},
		{Saboteur, 0, 0, 0, 0, swSaboteur, stSaboteur},
		{Spy, 0, 0, 3, 3, swSpy, stSpy},
		{Warlord, 0, 0, 2, 2, swWarlord, stWarlord},
		{Wedding, 0, 0, 2, 2, swWedding, stWedding},
	};
}

//---------------------------- PUBLIC           -----------------------------//
class PoliticsCardsOption 
	: public wxEvtHandler
	, public IOptionUI
{
public:
	virtual void dependencies(StringPairArray& array) const
	{
		array.push_back(std::make_pair(swCitiesRules, swCities));
	}

	virtual void silent_dependencies(StringPairArray&) const
	{
	}

	virtual void exclusions(StringPairArray&) const
	{
	}

	virtual const wxString& description() const
	{
		return stChangeThePoliticsDeck;
	}

	virtual wxInt32 priority() const
	{
		return 60;
	}

	virtual wxString short_description() const
	{
		wxString str;
		bool first = true;

		for(size_t i = 0; i < NumPoliticsCards; ++i)
		{
			if(sPoliticsCards[i].value != sPoliticsCards[i].original)
			{
				if(false == first)
				{
					str += swCommaSpace;
				}

				first = false;

				str += wxString::Format(swIntStringFormat.c_str(), 
                    sPoliticsCards[i].value, sPoliticsCards[i].name.c_str());
			}
		}

        return stPoliticsCards + swColon + swSpace + str;
	}

	virtual bool HasUI() const
	{
		return true;
	}

	virtual void CreateUI(wxWindow* parent, wxSizer* sizer)
	{
		wxBoxSizer *vert = new wxBoxSizer(wxVERTICAL);
		
		for(size_t i = 0; i < NumPoliticsCards; ++i)
		{
			wxBoxSizer *innerVert1 = new wxBoxSizer(wxVERTICAL);
			wxBoxSizer* innerHorz1 = new wxBoxSizer(wxHORIZONTAL);
			sPoliticsCards[i].text = new wxBaseStaticText(parent, -1, sPoliticsCards[i].name + 
				swColon);

			wxString init = wxString::Format(swIntFormat.c_str(), sPoliticsCards[i].value);
			sPoliticsCards[i].combo = new wxBaseComboBox(parent, sPoliticsCards[i].id,
				init, wxDefaultPosition, wxDefaultSize, _countof(sOptions_PoliticsCards), 
				sOptions_PoliticsCards, wxCB_READONLY);
		
			innerVert1->AddStretchSpacer();
			innerVert1->Add(sPoliticsCards[i].text);
			innerVert1->AddStretchSpacer();
			innerHorz1->Add(innerVert1, 1, wxEXPAND | wxLEFT, 7);
			innerHorz1->Add(sPoliticsCards[i].combo, 0, wxEXPAND | wxLEFT | 
				wxRIGHT, 7);
			vert->Add(innerHorz1, 1, wxEXPAND);
		}

		sizer->Add(vert, 0);

		parent->Connect(sPoliticsCards[0].id, sPoliticsCards[NumPoliticsCards - 1].id, 
			wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(PoliticsCardsOption::OnCombo));
	}

	virtual void DestroyUI(wxWindow* parent, wxSizer* sizer)
	{
		parent->Disconnect(sPoliticsCards[0].id, sPoliticsCards[NumPoliticsCards - 1].id, 
			wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(PoliticsCardsOption::OnCombo));

		sizer->Remove(0);

		for(size_t i = 0; i < NumPoliticsCards; ++i)
		{
			sPoliticsCards[i].text->Destroy();
			sPoliticsCards[i].text = NULL;

			sPoliticsCards[i].combo->Destroy();
			sPoliticsCards[i].combo = NULL;
		}
	}

	virtual void UpdateUI(const wxString& key, const wxString& value)
	{
		size_t i = 0;
		for(; i < NumPoliticsCards; ++i)
		{
			if(sPoliticsCards[i].type == key)
			{
				value.ToLong(&sPoliticsCards[i].value);
				break;
			}
		}

		wxASSERT(i != NumPoliticsCards);
	}

	virtual void SaveToXML(wxXmlNode* node)
	{
		for(size_t i = 0; i < NumPoliticsCards; ++i)
		{
			wxXmlNode* child = new wxXmlNode(NULL, wxXML_ELEMENT_NODE,
				sPoliticsCards[i].type);
			child->AddProperty(swValue, wxString::Format(swIntFormat.c_str(), 
				sPoliticsCards[i].value));
			node->AddChild(child);
		}
	}

	virtual void LoadFromXML(const wxString& option, wxXmlNode* node)
	{
		wxXmlNode* child = node->GetChildren();
		
		size_t i = 0;
		wxString value;
		
		for(; child != NULL; child = child->GetNext())
		{
			if( (sPoliticsCards[i].type == child->GetName()) &&
				(true == child->GetPropVal(swValue, &value)))
			{
				RULE.Execute(shRuleSilentUpdateOption, DataObject(option,
					sPoliticsCards[i].type, value));
			}

			++i;
		}
	}

private:
	void OnCombo(wxCommandEvent &command)
	{
		size_t id = command.GetId();

		wxInt32 sel = sPoliticsCards[id].combo->GetSelection();
		wxASSERT(-1 != sel);
		
		wxString str = sPoliticsCards[id].combo->GetString(sel);
		Controller::get().Transmit(shEventUpdateOption, 
			DataObject(sPoliticsCards[id].type, str));
	}
};

IMPLEMENT_OPTION_UI(POLITICS_CARDS, PoliticsCardsOption);

//---------------------------------------------------------------------------//
// Class: RuleInitializePoliticsCards
// 
// Initializes politics cards data.
//
// Derived From:
//     <Rule>
//
// Project:
//     <CitieRules>
//
// RuleSet:
//     <PoliticsCards>
//
// Mixin To:
//     <RuleInitializeOptions>
//
class RuleInitializePoliticsCards : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		for(size_t i = 0; i < NumPoliticsCards; ++i)
		{
			gameData<Data::IntHash>(shBankPoliticsCards)[HashString(sPoliticsCards[i].type)] =
				sPoliticsCards[i].value;
		}
	}
};

IMPLEMENT_RULE_MIXIN(RuleInitializePoliticsCards, RuleInitializeOptions, 
					 POLITICS_CARDS)

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
