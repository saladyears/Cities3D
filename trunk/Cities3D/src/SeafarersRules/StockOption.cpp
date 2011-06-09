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
	long sShips = 15;

	enum
	{
		ID_SHIPS_COMBO			= 0,
	};

	const wxString sShipOptions[] = {sw5, sw6, sw7, sw8, sw9, sw10, sw11, sw12,
        sw13, sw14, sw15, sw16, sw17, sw18, sw19, sw20};
		
	wxBaseStaticText* spText1 = NULL;
	wxBaseComboBox *spShipsCombo = NULL;
}

//---------------------------- PUBLIC           -----------------------------//
class StockOption 
	: public wxEvtHandler
	, public IOptionUI
{
public:
	virtual void dependencies(StringPairArray& array) const
	{
		array.push_back(std::make_pair(swSeafarersRules, swSeafarers));
	}

	virtual void silent_dependencies(StringPairArray&) const
	{
	}

	virtual void exclusions(StringPairArray& ) const
	{
	}

	virtual const wxString& description() const
	{
		return stChangeTheStartingStockShips;
	}

	virtual wxInt32 priority() const
	{
		return 20;
	}

	virtual wxString short_description() const
	{
		wxString str = wxString::Format(stIntShips.c_str(), sShips);

		return str;
	}

	virtual bool HasUI() const
	{
		return true;
	}

	virtual void CreateUI(wxWindow* parent, wxSizer* sizer)
	{
		wxBoxSizer *vert = new wxBoxSizer(wxVERTICAL);
		
		// Ships
		wxString init = wxString::Format(swIntFormat, sShips);

		wxBoxSizer *innerVert1 = new wxBoxSizer(wxVERTICAL);
		wxBoxSizer* innerHorz1 = new wxBoxSizer(wxHORIZONTAL);
		spText1 = new wxBaseStaticText(parent, -1, 
			stShips + swColon);
		spShipsCombo = new wxBaseComboBox(parent, ID_SHIPS_COMBO, init, 
			wxDefaultPosition, wxDefaultSize, _countof(sShipOptions), 
			sShipOptions, wxCB_READONLY);
		
		innerVert1->AddStretchSpacer();
		innerVert1->Add(spText1);
		innerVert1->AddStretchSpacer();
		innerHorz1->Add(innerVert1, 1, wxEXPAND | wxLEFT, 7);
		innerHorz1->Add(spShipsCombo, 0, wxEXPAND | wxLEFT | wxRIGHT, 7);
		vert->Add(innerHorz1, 1, wxEXPAND);
	
		
		sizer->Add(vert, 0);

		parent->Connect(ID_SHIPS_COMBO, ID_SHIPS_COMBO, 
			wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(StockOption::OnShipsCombo));
	}

	virtual void DestroyUI(wxWindow* parent, wxSizer* sizer)
	{
		parent->Disconnect(ID_SHIPS_COMBO, ID_SHIPS_COMBO, 
			wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(StockOption::OnShipsCombo));

		sizer->Remove(0);

		spText1->Destroy();
		spText1 = NULL;

		spShipsCombo->Destroy();
		spShipsCombo = NULL;
	}

	virtual void UpdateUI(const wxString& key, const wxString& value)
	{
		wxASSERT(swShips == key);

		value.ToLong(&sShips);
	}

	void OnShipsCombo(wxCommandEvent &)
	{
		wxInt32 sel = spShipsCombo->GetSelection();
		wxASSERT(-1 != sel);

		wxString str = spShipsCombo->GetString(sel);
		
		Controller::get().Transmit(shEventUpdateOptions, 
			DataObject(swShips, str));
	}

	virtual void SaveToXML(wxXmlNode* node)
	{
		wxXmlNode* child = new wxXmlNode(node, wxXML_ELEMENT_NODE,
			swShips);
        child->AddProperty(swValue, wxString::Format(swIntFormat.c_str(), 
            sShips));
	}

	virtual void LoadFromXML(const wxString& option, wxXmlNode* node)
	{
		wxXmlNode* child = node->GetChildren();
		wxString value;

		if( (swShips == child->GetName()) &&
			(true == child->GetPropVal(swValue, &value)))
		{
			RULE.Execute(shRuleSilentUpdateOption, DataObject(option,
				swShips, value));
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
class RuleInitializeStockShips : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			playerGameData<wxInt32>(shStockShips, i) = 
				sShips;
		}
	}
};

IMPLEMENT_RULE_MIXIN(RuleInitializeStockShips, RuleInitializeOptions, 
					 STOCK_AMOUNTS)

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
