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

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	long sRoads = 15;
	long sSettlements = 5;
	long sCities = 4;

	enum
	{
		ID_ROADS_COMBO			= 0,
		ID_SETTLEMENTS_COMBO	,
		ID_CITIES_COMBO,
	};

	const wxString sRoadOptions[] = {sw5, sw6, sw7, sw8, sw9, sw10, sw11, sw12, 
        sw13, sw14, sw15, sw16, sw17, sw18, sw19, sw20};
	const wxString sSettlementOptions[] = {sw2, sw3, sw4, sw5, sw6, sw7, sw8, 
        sw9, sw10};
	const wxString sCityOptions[] = {sw0, sw1, sw2, sw3, sw4, sw5, sw6, sw7, 
        sw8, sw9, sw10};

	wxBaseStaticText* spText1 = NULL;
	wxBaseStaticText* spText2 = NULL;
	wxBaseStaticText* spText3 = NULL;
	wxBaseComboBox *spRoadsCombo = NULL;
	wxBaseComboBox *spSettlementsCombo = NULL;
	wxBaseComboBox *spCitiesCombo = NULL;
}

//---------------------------- PUBLIC           -----------------------------//
class StockOption 
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
		return stChangeTheStartingStock;
	}

	virtual wxInt32 priority() const
	{
		return 40;
	}

	virtual wxString short_description() const
	{
		wxString str = wxString::Format(stXRoads.c_str(), sRoads);
		str += swCommaSpace;
		str += wxString::Format(stXSettlements.c_str(), sSettlements);
		str += swCommaSpace;
		
		if(1 == sCities)
		{
			str += stOneCity;
		}
		else
		{
			str += wxString::Format(stXCities.c_str(), sCities);
		}

		return str;
	}

	virtual bool HasUI() const
	{
		return true;
	}

	virtual void CreateUI(wxWindow* parent, wxSizer* sizer)
	{
		wxBoxSizer *vert = new wxBoxSizer(wxVERTICAL);
		
		// Roads
		wxString init = wxString::Format(swIntFormat.c_str(), sRoads);

		wxBoxSizer *innerVert1 = new wxBoxSizer(wxVERTICAL);
		wxBoxSizer* innerHorz1 = new wxBoxSizer(wxHORIZONTAL);
		spText1 = new wxBaseStaticText(parent, -1, 
			stRoads + swColon);
		spRoadsCombo = new wxBaseComboBox(parent, ID_ROADS_COMBO, init, 
			wxDefaultPosition, wxDefaultSize, _countof(sRoadOptions), 
			sRoadOptions, wxCB_READONLY);
		
		innerVert1->AddStretchSpacer();
		innerVert1->Add(spText1);
		innerVert1->AddStretchSpacer();
		innerHorz1->Add(innerVert1, 1, wxEXPAND | wxLEFT, 7);
		innerHorz1->Add(spRoadsCombo, 0, wxEXPAND | wxLEFT | wxRIGHT, 7);
		vert->Add(innerHorz1, 1, wxEXPAND);
	
		// Settlements
		init = wxString::Format(swIntFormat.c_str(), sSettlements);

		wxBoxSizer *innerVert2 = new wxBoxSizer(wxVERTICAL);
		wxBoxSizer* innerHorz2 = new wxBoxSizer(wxHORIZONTAL);
		spText2 = new wxBaseStaticText(parent, -1, 
			stSettlements + swColon);
		spSettlementsCombo = new wxBaseComboBox(parent, ID_SETTLEMENTS_COMBO, 
			init, wxDefaultPosition, wxDefaultSize, 
			_countof(sSettlementOptions), sSettlementOptions, wxCB_READONLY);
		
		innerVert2->AddStretchSpacer();
		innerVert2->Add(spText2);
		innerVert2->AddStretchSpacer();
		innerHorz2->Add(innerVert2, 1, wxEXPAND | wxLEFT, 7);
		innerHorz2->Add(spSettlementsCombo, 0, wxEXPAND | wxLEFT | wxRIGHT, 7);
		vert->Add(innerHorz2, 1, wxEXPAND | wxTOP, 7);

		// Cities
		init = wxString::Format(swIntFormat.c_str(), sCities);

		wxBoxSizer *innerVert3 = new wxBoxSizer(wxVERTICAL);
		wxBoxSizer* innerHorz3 = new wxBoxSizer(wxHORIZONTAL);
		spText3 = new wxBaseStaticText(parent, -1, 
			stCities + swColon);
		spCitiesCombo = new wxBaseComboBox(parent, ID_CITIES_COMBO, 
			init, wxDefaultPosition, wxDefaultSize, 
			_countof(sCityOptions), sCityOptions, wxCB_READONLY);
		
		innerVert3->AddStretchSpacer();
		innerVert3->Add(spText3);
		innerVert3->AddStretchSpacer();
		innerHorz3->Add(innerVert3, 1, wxEXPAND | wxLEFT, 7);
		innerHorz3->Add(spCitiesCombo, 0, wxEXPAND | wxLEFT | wxRIGHT, 7);
		vert->Add(innerHorz3, 1, wxEXPAND | wxTOP, 7);

		sizer->Add(vert, 0);

		parent->Connect(ID_ROADS_COMBO, ID_ROADS_COMBO, 
			wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(StockOption::OnRoadsCombo));
		parent->Connect(ID_SETTLEMENTS_COMBO, ID_SETTLEMENTS_COMBO, 
			wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(StockOption::OnSettlementsCombo));
		parent->Connect(ID_CITIES_COMBO, ID_CITIES_COMBO, 
			wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(StockOption::OnCitiesCombo));
	}

	virtual void DestroyUI(wxWindow* parent, wxSizer* sizer)
	{
		parent->Disconnect(ID_ROADS_COMBO, ID_ROADS_COMBO, 
			wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(StockOption::OnRoadsCombo));
		parent->Disconnect(ID_SETTLEMENTS_COMBO, ID_SETTLEMENTS_COMBO, 
			wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(StockOption::OnSettlementsCombo));
		parent->Disconnect(ID_CITIES_COMBO, ID_CITIES_COMBO, 
			wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(StockOption::OnCitiesCombo));

		sizer->Remove(0);

		spText1->Destroy();
		spText1 = NULL;
		spText2->Destroy();
		spText2 = NULL;
		spText3->Destroy();
		spText3 = NULL;

		spRoadsCombo->Destroy();
		spRoadsCombo = NULL;
		spSettlementsCombo->Destroy();
		spSettlementsCombo = NULL;
		spCitiesCombo->Destroy();
		spCitiesCombo = NULL;
	}

	virtual void UpdateUI(const wxString& key, const wxString& value)
	{
		if(swRoads == key)
		{
			value.ToLong(&sRoads);
		}
		else if(swSettlements == key)
		{
			value.ToLong(&sSettlements);
		}
		else if(swCities == key)
		{
			value.ToLong(&sCities);
		}
		else
		{
			wxASSERT(false);
		}
	}

	void OnRoadsCombo(wxCommandEvent &)
	{
		wxInt32 sel = spRoadsCombo->GetSelection();
		wxASSERT(-1 != sel);

		wxString str = spRoadsCombo->GetString(sel);
		
		Controller::get().Transmit(shEventUpdateOption, 
			DataObject(swRoads, str));
	}

	void OnSettlementsCombo(wxCommandEvent &)
	{
		wxInt32 sel = spSettlementsCombo->GetSelection();
		wxASSERT(-1 != sel);

		wxString str = spSettlementsCombo->GetString(sel);
		
		Controller::get().Transmit(shEventUpdateOption, 
			DataObject(swSettlements, str));
	}

	void OnCitiesCombo(wxCommandEvent &)
	{
		wxInt32 sel = spCitiesCombo->GetSelection();
		wxASSERT(-1 != sel);

		wxString str = spCitiesCombo->GetString(sel);
		
		Controller::get().Transmit(shEventUpdateOption, 
			DataObject(swCities, str));
	}

	virtual void SaveToXML(wxXmlNode* node)
	{
		wxXmlNode* child = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, swRoads);
		child->AddProperty(swValue, wxString::Format(swIntFormat.c_str(), sRoads));
		node->AddChild(child);

		child = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, swSettlements);
		child->AddProperty(swValue, wxString::Format(swIntFormat.c_str(), sSettlements));
		node->AddChild(child);

		child = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, swCities);
		child->AddProperty(swValue, wxString::Format(swIntFormat.c_str(), sCities));
		node->AddChild(child);
	}

	virtual void LoadFromXML(const wxString& option, wxXmlNode* node)
	{
		wxXmlNode* child = node->GetChildren();
		wxString value;

		for(; child != NULL; child = child->GetNext())
		{
			if( (swRoads == child->GetName()) &&
				(true == child->GetPropVal(swValue, &value)))
			{
				RULE.Execute(shRuleSilentUpdateOption, DataObject(option,
					swRoads, value));
			}

			if( (swSettlements == child->GetName()) &&
				(true == child->GetPropVal(swValue, &value)))
			{
				RULE.Execute(shRuleSilentUpdateOption, DataObject(option,
					swSettlements, value));
			}

			if( (swCities == child->GetName()) &&
				(true == child->GetPropVal(swValue, &value)))
			{
				RULE.Execute(shRuleSilentUpdateOption, DataObject(option,
					swCities, value));
			}
		}
	}
};

IMPLEMENT_OPTION_UI(STOCK_AMOUNTS, StockOption);

//---------------------------------------------------------------------------//
// Class: RuleInitializeStock
// 
// Initializes points data.
//
// Derived From:
//     <Rule>
//
// Project:
//     <StandardRules>
//
// RuleSet:
//     <StockAmounts>
//
// Mixin To:
//     <RuleInitializeOptions>
//
class RuleInitializeStock : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			playerGameData<wxInt32>(shStockRoads, i) = 
				sRoads;
			playerGameData<wxInt32>(shStockSettlements, i) = 
				sSettlements;
			playerGameData<wxInt32>(shStockCities, i) = 
				sCities;
		}
	}
};

IMPLEMENT_RULE_MIXIN(RuleInitializeStock, RuleInitializeOptions, STOCK_AMOUNTS)

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
