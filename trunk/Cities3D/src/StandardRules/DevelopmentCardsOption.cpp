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

#include "Map.h"
#include "GameEngine.h"
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
	struct Card
	{
		wxInt32 id;
		wxBaseStaticText* text;
		wxBaseComboBox* combo;
		long value;
		long original3;
		long original5;
		wxString type;
		wxString name;
	};

	enum
	{
		VictoryPoint		= 0,
		RoadBuilding		,
		YearOfPlenty		,
		Monopoly			,
		Soldier				,

		NumDevelopmentCards	,
	};

	const wxString sOptions_DevelopmentCards[] = {sw0, sw1, sw2, sw3, sw4, 
        sw5, sw6, sw7, sw8, sw9, sw10, sw11, sw12, sw13, sw14, sw15, sw16, 
        sw17, sw18, sw19, sw20};

	Card sCards[] = 
	{
		{VictoryPoint, 0, 0, 5, 5, 5, swVictoryPoint, stVictoryPoint},
		{RoadBuilding, 0, 0, 2, 2, 3, swRoadBuilding, stRoadBuilding},
		{YearOfPlenty, 0, 0, 2, 2, 3, swYearOfPlenty, stYearOfPlenty},
		{Monopoly, 0, 0, 2, 2, 3, swMonopoly, stMonopoly},
		{Soldier, 0, 0, 14, 14, 20, swSoldier, stSoldier},
	};
}

//---------------------------- PUBLIC           -----------------------------//
class DevelopmentCardsOption 
	: public wxEvtHandler
	, public IOptionUI
{
public:
	virtual void dependencies(StringPairArray&) const
	{
	}

	virtual void silent_dependencies(StringPairArray&) const
	{
	}

	virtual void exclusions(StringPairArray&) const
	{
	}

	virtual const wxString& description() const
	{
		return stChangeTheDeck;
	}

	virtual wxInt32 priority() const
	{
		return 50;
	}

	virtual wxString short_description() const
	{
		wxString str;
		bool first = true;

		bool b34 = (5 > MAP.read<wxInt32>(shMinPlayers));

		for(size_t i = 0; i < NumDevelopmentCards; ++i)
		{
			wxInt32 val = b34 ? sCards[i].original3 : sCards[i].original5;
			if(sCards[i].value != val)
			{
				if(false == first)
				{
					str += swCommaSpace;
				}

				first = false;

				str += wxString::Format(swIntString.c_str(), sCards[i].value, 
					sCards[i].name.c_str());
			}
		}

        wxString ret = stDevelopmentCards + swColon + swSpace + str;
		return ret;
	}

	virtual bool HasUI() const
	{
		return true;
	}

	virtual void CreateUI(wxWindow* parent, wxSizer* sizer)
	{
		wxBoxSizer *vert = new wxBoxSizer(wxVERTICAL);
		
		for(size_t i = 0; i < NumDevelopmentCards; ++i)
		{
			wxBoxSizer *innerVert1 = new wxBoxSizer(wxVERTICAL);
			wxBoxSizer* innerHorz1 = new wxBoxSizer(wxHORIZONTAL);
			sCards[i].text = new wxBaseStaticText(parent, -1, sCards[i].name + 
				swColon);

			wxString init = wxString::Format(swIntFormat.c_str(), sCards[i].value);
			sCards[i].combo = new wxBaseComboBox(parent, sCards[i].id,
				init, wxDefaultPosition, wxDefaultSize, 
				_countof(sOptions_DevelopmentCards), 
				sOptions_DevelopmentCards, wxCB_READONLY);
		
			innerVert1->AddStretchSpacer();
			innerVert1->Add(sCards[i].text);
			innerVert1->AddStretchSpacer();
			innerHorz1->Add(innerVert1, 1, wxEXPAND | wxLEFT, 7);
			innerHorz1->Add(sCards[i].combo, 0, wxEXPAND | wxLEFT | 
				wxRIGHT, 7);
			vert->Add(innerHorz1, 1, wxEXPAND);
		}

		sizer->Add(vert, 0);

		parent->Connect(sCards[0].id, sCards[NumDevelopmentCards - 1].id, 
			wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(DevelopmentCardsOption::OnCombo));
	}

	virtual void DestroyUI(wxWindow* parent, wxSizer* sizer)
	{
		parent->Disconnect(sCards[0].id, sCards[NumDevelopmentCards - 1].id, 
			wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(DevelopmentCardsOption::OnCombo));

		sizer->Remove(0);

		for(size_t i = 0; i < NumDevelopmentCards; ++i)
		{
			sCards[i].text->Destroy();
			sCards[i].text = NULL;

			sCards[i].combo->Destroy();
			sCards[i].combo = NULL;
		}
	}

	virtual void UpdateUI(const wxString& key, const wxString& value)
	{
		size_t i = 0;
		for(; i < NumDevelopmentCards; ++i)
		{
			if(sCards[i].type == key)
			{
				value.ToLong(&sCards[i].value);
				break;
			}
		}

		wxASSERT(i != NumDevelopmentCards);
	}

	virtual void SaveToXML(wxXmlNode* node)
	{
		for(size_t i = 0; i < NumDevelopmentCards; ++i)
		{
			wxXmlNode* child = new wxXmlNode(NULL, wxXML_ELEMENT_NODE,
				sCards[i].type);
			child->AddProperty(swValue, wxString::Format(swIntFormat.c_str(), 
				sCards[i].value));
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
			if( (sCards[i].type == child->GetName()) &&
				(true == child->GetPropVal(swValue, &value)))
			{
				RULE.Execute(shRuleSilentUpdateOption, DataObject(option,
					sCards[i].type, value));
			}

			++i;
		}
	}

private:
	void OnCombo(wxCommandEvent &command)
	{
		size_t id = command.GetId();

		wxInt32 sel = sCards[id].combo->GetSelection();
		wxASSERT(-1 != sel);
		
		wxString str = sCards[id].combo->GetString(sel);
		Controller::get().Transmit(shEventUpdateOption, 
			DataObject(sCards[id].type, str));
	}
};

IMPLEMENT_OPTION_UI(DEVELOPMENT_CARDS, DevelopmentCardsOption);

//---------------------------------------------------------------------------//
// Class: RuleInitializeDevelopmentCards
// 
// Initializes development cards data.
//
// Derived From:
//     <Rule>
//
// Project:
//     <CitieRules>
//
// RuleSet:
//     <DevelopmentCards>
//
// Mixin To:
//     <RuleInitializeOptions>
//
class RuleInitializeDevelopmentCards : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		for(size_t i = 0; i < NumDevelopmentCards; ++i)
		{
			gameData<Data::IntHash>(shBankDevCards)[HashString(sCards[i].type)] =
				sCards[i].value;
		}
	}
};

IMPLEMENT_RULE_MIXIN(RuleInitializeDevelopmentCards, RuleInitializeOptions, 
					 DEVELOPMENT_CARDS)

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
