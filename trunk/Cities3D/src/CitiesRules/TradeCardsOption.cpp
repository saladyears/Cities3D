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
	struct TradeCard
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
		CommercialHarbor			= 0,
		MasterMerchant				,
		Merchant					,
		MerchantFleet				,
		ResourceMonopoly			,
		TradeMonopoly				,

		NumTradeCards				,
	};

	const wxString sOptions_TradeCards[] = {sw0, sw1, sw2, sw3, sw4, sw5,
        sw6, sw7, sw8, sw9, sw10};

	TradeCard sTradeCards[] = 
	{
		{CommercialHarbor, 0, 0, 2, 2, swCommercialHarbor, stCommercialHarbor},
		{MasterMerchant, 0, 0, 2, 2, swMasterMerchant, stMasterMerchant},
		{Merchant, 0, 0, 6, 6, swMerchant, stMerchant},
		{MerchantFleet, 0, 0, 2, 2, swMerchantFleet, stMerchantFleet},
		{ResourceMonopoly, 0, 0, 4, 4, swResourceMonopoly, stResourceMonopoly},
		{TradeMonopoly, 0, 0, 2, 2, swTradeMonopoly, stTradeMonopoly}
	};
}

//---------------------------- PUBLIC           -----------------------------//
class TradeCardsOption 
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
		return stChangeTheTradeDeck;
	}

	virtual wxInt32 priority() const
	{
		return 50;
	}

	virtual wxString short_description() const
	{
		wxString str;
		bool first = true;

		for(size_t i = 0; i < NumTradeCards; ++i)
		{
			if(sTradeCards[i].value != sTradeCards[i].original)
			{
				if(false == first)
				{
					str += swCommaSpace;
				}

				str += wxString::Format(swIntStringFormat.c_str(), 
                    sTradeCards[i].value, sTradeCards[i].name.c_str());
				first = false;
			}
		}

        return stTradeCards + swColon + swSpace + str;
	}

	virtual bool HasUI() const
	{
		return true;
	}

	virtual void CreateUI(wxWindow* parent, wxSizer* sizer)
	{
		wxBoxSizer *vert = new wxBoxSizer(wxVERTICAL);
		
		for(size_t i = 0; i < NumTradeCards; ++i)
		{
			wxBoxSizer *innerVert1 = new wxBoxSizer(wxVERTICAL);
			wxBoxSizer* innerHorz1 = new wxBoxSizer(wxHORIZONTAL);
			sTradeCards[i].text = new wxBaseStaticText(parent, -1, sTradeCards[i].name + 
				swColon);

			wxString init = wxString::Format(swIntFormat.c_str(), sTradeCards[i].value);
			sTradeCards[i].combo = new wxBaseComboBox(parent, sTradeCards[i].id,
				init, wxDefaultPosition, wxDefaultSize, _countof(sOptions_TradeCards), 
				sOptions_TradeCards, wxCB_READONLY);
		
			innerVert1->AddStretchSpacer();
			innerVert1->Add(sTradeCards[i].text);
			innerVert1->AddStretchSpacer();
			innerHorz1->Add(innerVert1, 1, wxEXPAND | wxLEFT, 7);
			innerHorz1->Add(sTradeCards[i].combo, 0, wxEXPAND | wxLEFT | 
				wxRIGHT, 7);
			vert->Add(innerHorz1, 1, wxEXPAND);
		}

		sizer->Add(vert, 0);

		parent->Connect(sTradeCards[0].id, sTradeCards[NumTradeCards - 1].id, 
			wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(TradeCardsOption::OnCombo));
	}

	virtual void DestroyUI(wxWindow* parent, wxSizer* sizer)
	{
		parent->Disconnect(sTradeCards[0].id, sTradeCards[NumTradeCards - 1].id, 
			wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(TradeCardsOption::OnCombo));

		sizer->Remove(0);

		for(size_t i = 0; i < NumTradeCards; ++i)
		{
			sTradeCards[i].text->Destroy();
			sTradeCards[i].text = NULL;

			sTradeCards[i].combo->Destroy();
			sTradeCards[i].combo = NULL;
		}
	}

	virtual void UpdateUI(const wxString& key, const wxString& value)
	{
		size_t i = 0;
		for(; i < NumTradeCards; ++i)
		{
			if(sTradeCards[i].type == key)
			{
				value.ToLong(&sTradeCards[i].value);
				break;
			}
		}

		wxASSERT(i != NumTradeCards);
	}

	virtual void SaveToXML(wxXmlNode* node)
	{
		for(size_t i = 0; i < NumTradeCards; ++i)
		{
			wxXmlNode* child = new wxXmlNode(NULL, wxXML_ELEMENT_NODE,
				sTradeCards[i].type);
			child->AddProperty(swValue, wxString::Format(swIntFormat.c_str(), 
				sTradeCards[i].value));
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
			if( (sTradeCards[i].type == child->GetName()) &&
				(true == child->GetPropVal(swValue, &value)))
			{
				RULE.Execute(shRuleSilentUpdateOption, DataObject(option,
					sTradeCards[i].type, value));
			}

			++i;
		}
	}

private:
	void OnCombo(wxCommandEvent &command)
	{
		size_t id = command.GetId();

		wxInt32 sel = sTradeCards[id].combo->GetSelection();
		wxASSERT(-1 != sel);
		
		wxString str = sTradeCards[id].combo->GetString(sel);
		Controller::get().Transmit(shEventUpdateOption, 
			DataObject(sTradeCards[id].type, str));
	}
};

IMPLEMENT_OPTION_UI(TRADE_CARDS, TradeCardsOption);

//---------------------------------------------------------------------------//
// Class: RuleInitializeTradeCards
// 
// Initializes trade cards data.
//
// Derived From:
//     <Rule>
//
// Project:
//     <CitieRules>
//
// RuleSet:
//     <TradeCards>
//
// Mixin To:
//     <RuleInitializeOptions>
//
class RuleInitializeTradeCards : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		for(size_t i = 0; i < NumTradeCards; ++i)
		{
			gameData<Data::IntHash>(shBankTradeCards)[HashString(sTradeCards[i].type)] =
				sTradeCards[i].value;
		}
	}
};

IMPLEMENT_RULE_MIXIN(RuleInitializeTradeCards, RuleInitializeOptions, 
					 TRADE_CARDS)

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
