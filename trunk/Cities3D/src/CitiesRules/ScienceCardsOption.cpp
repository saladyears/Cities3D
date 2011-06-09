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
	struct ScienceCard
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
		Alchemist			= 0,
		Crane				,
		Engineer			,
		Inventor			,
		Irrigation			,
		Medicine			,
		Mining				,
		Printer				,
		RoadBuilding		,
		Smith				,

		NumScienceCards		,
	};

	const wxString sOptions_ScienceCards[] = {sw0, sw1, sw2, sw3, sw4, sw5,
        sw6, sw7, sw8, sw9, sw10};

	ScienceCard sScienceCards[] = 
	{
		{Alchemist, 0, 0, 2, 2, swAlchemist, stAlchemist},
		{Crane, 0, 0, 2, 2, swCrane, stCrane},
		{Engineer, 0, 0, 2, 2, swEngineer, stEngineer},
		{Inventor, 0, 0, 1, 1, swInventor, stInventor},
		{Irrigation, 0, 0, 2, 2, swIrrigation, stIrrigation},
		{Medicine, 0, 0, 2, 2, swMedicine, stMedicine},
		{Mining, 0, 0, 2, 2, swMining, stMining},
		{Printer, 0, 0, 1, 1, swPrinter, stPrinter},
		{RoadBuilding, 0, 0, 2, 2, swRoadBuilding, stRoadBuilding},
		{Smith, 0, 0, 2, 2, swSmith, stSmith},
	};
}

//---------------------------- PUBLIC           -----------------------------//
class ScienceCardsOption 
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
		return stChangeTheScienceDeck;
	}

	virtual wxInt32 priority() const
	{
		return 70;
	}

	virtual wxString short_description() const
	{
		wxString str;
		bool first = true;

		for(size_t i = 0; i < NumScienceCards; ++i)
		{
			if(sScienceCards[i].value != sScienceCards[i].original)
			{
				if(false == first)
				{
					str += swCommaSpace;
				}

				first = false;

				str += wxString::Format(swIntStringFormat.c_str(), 
                    sScienceCards[i].value, sScienceCards[i].name.c_str());
			}
		}

        return stScienceCards + swColon + swSpace + str;
	}

	virtual bool HasUI() const
	{
		return true;
	}

	virtual void CreateUI(wxWindow* parent, wxSizer* sizer)
	{
		wxBoxSizer *vert = new wxBoxSizer(wxVERTICAL);
		
		for(size_t i = 0; i < NumScienceCards; ++i)
		{
			wxBoxSizer *innerVert1 = new wxBoxSizer(wxVERTICAL);
			wxBoxSizer* innerHorz1 = new wxBoxSizer(wxHORIZONTAL);
			sScienceCards[i].text = new wxBaseStaticText(parent, -1, sScienceCards[i].name + 
				swColon);

			wxString init = wxString::Format(swIntFormat.c_str(), sScienceCards[i].value);
			sScienceCards[i].combo = new wxBaseComboBox(parent, sScienceCards[i].id,
				init, wxDefaultPosition, wxDefaultSize, _countof(sOptions_ScienceCards), 
				sOptions_ScienceCards, wxCB_READONLY);
		
			innerVert1->AddStretchSpacer();
			innerVert1->Add(sScienceCards[i].text);
			innerVert1->AddStretchSpacer();
			innerHorz1->Add(innerVert1, 1, wxEXPAND | wxLEFT, 7);
			innerHorz1->Add(sScienceCards[i].combo, 0, wxEXPAND | wxLEFT | 
				wxRIGHT, 7);
			vert->Add(innerHorz1, 1, wxEXPAND);
		}

		sizer->Add(vert, 0);

		parent->Connect(sScienceCards[0].id, sScienceCards[NumScienceCards - 1].id, 
			wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(ScienceCardsOption::OnCombo));
	}

	virtual void DestroyUI(wxWindow* parent, wxSizer* sizer)
	{
		parent->Disconnect(sScienceCards[0].id, sScienceCards[NumScienceCards - 1].id, 
			wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(ScienceCardsOption::OnCombo));

		sizer->Remove(0);

		for(size_t i = 0; i < NumScienceCards; ++i)
		{
			sScienceCards[i].text->Destroy();
			sScienceCards[i].text = NULL;

			sScienceCards[i].combo->Destroy();
			sScienceCards[i].combo = NULL;
		}
	}

	virtual void UpdateUI(const wxString& key, const wxString& value)
	{
		size_t i = 0;
		for(; i < NumScienceCards; ++i)
		{
			if(sScienceCards[i].type == key)
			{
				value.ToLong(&sScienceCards[i].value);
				break;
			}
		}

		wxASSERT(i != NumScienceCards);
	}

	virtual void SaveToXML(wxXmlNode* node)
	{
		for(size_t i = 0; i < NumScienceCards; ++i)
		{
			wxXmlNode* child = new wxXmlNode(NULL, wxXML_ELEMENT_NODE,
				sScienceCards[i].type);
			child->AddProperty(swValue, wxString::Format(swIntFormat.c_str(), 
				sScienceCards[i].value));
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
			if( (sScienceCards[i].type == child->GetName()) &&
				(true == child->GetPropVal(swValue, &value)))
			{
				RULE.Execute(shRuleSilentUpdateOption, DataObject(option,
					sScienceCards[i].type, value));
			}

			++i;
		}
	}

private:
	void OnCombo(wxCommandEvent &command)
	{
		size_t id = command.GetId();

		wxInt32 sel = sScienceCards[id].combo->GetSelection();
		wxASSERT(-1 != sel);
		
		wxString str = sScienceCards[id].combo->GetString(sel);
		Controller::get().Transmit(shEventUpdateOption, 
			DataObject(sScienceCards[id].type, str));
	}
};

IMPLEMENT_OPTION_UI(SCIENCE_CARDS, ScienceCardsOption);

//---------------------------------------------------------------------------//
// Class: RuleInitializeScienceCards
// 
// Initializes science cards data.
//
// Derived From:
//     <Rule>
//
// Project:
//     <CitieRules>
//
// RuleSet:
//     <ScienceCards>
//
// Mixin To:
//     <RuleInitializeOptions>
//
class RuleInitializeScienceCards : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		for(size_t i = 0; i < NumScienceCards; ++i)
		{
			gameData<Data::IntHash>(shBankScienceCards)[HashString(sScienceCards[i].type)] =
				sScienceCards[i].value;
		}
	}
};

IMPLEMENT_RULE_MIXIN(RuleInitializeScienceCards, RuleInitializeOptions, 
					 SCIENCE_CARDS)

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
