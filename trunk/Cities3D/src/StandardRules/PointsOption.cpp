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
	long sPoints = -1;

	enum
	{
		ID_POINTS_COMBO			= 0,
	};

	const wxString sOptions_Points[] = {sw3, sw4, sw5, sw6, sw7, sw8, sw9,
        sw10, sw11, sw12, sw13, sw14, sw15, sw16, sw17, sw18, sw19, sw20};

	wxBaseStaticText* spText_Points = NULL;
	wxBaseComboBox *spPointsCombo = NULL;
}

//---------------------------- PUBLIC           -----------------------------//
class PointsOption 
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
		return stChangeTheNumberOfPoints;
	}

	virtual wxInt32 priority() const
	{
		return 30;
	}

	virtual wxString short_description() const
	{
		wxString str = wxString::Format(stPlayToXPoints.c_str(), sPoints);

		return str;
	}

	virtual bool HasUI() const
	{
		return true;
	}

	virtual void CreateUI(wxWindow* parent, wxSizer* sizer)
	{
		if(-1 == sPoints)
		{
			sPoints = MAP.read<wxInt32>(shPoints);
		}

		wxString init = wxString::Format(swIntFormat.c_str(), sPoints);

		wxBoxSizer *vert = new wxBoxSizer(wxVERTICAL);
		wxBoxSizer *innerVert = new wxBoxSizer(wxVERTICAL);
		wxBoxSizer* innerHorz = new wxBoxSizer(wxHORIZONTAL);
		spText_Points = new wxBaseStaticText(parent, -1, 
			stPointsNeededToWin + swColon);
		spPointsCombo = new wxBaseComboBox(parent, ID_POINTS_COMBO, init, 
			wxDefaultPosition, wxDefaultSize, 18, sOptions_Points, 
			wxCB_READONLY);
		
		innerVert->AddStretchSpacer();
		innerVert->Add(spText_Points);
		innerVert->AddStretchSpacer();
		innerHorz->Add(innerVert, 1, wxEXPAND | wxLEFT, 7);
		innerHorz->Add(spPointsCombo, 0, wxEXPAND | wxLEFT | wxRIGHT, 7);
		vert->Add(innerHorz, 1, wxEXPAND);
		
		sizer->Add(vert, 0);

		parent->Connect(ID_POINTS_COMBO, ID_POINTS_COMBO, 
			wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(PointsOption::OnPointsCombo));
	}

	virtual void DestroyUI(wxWindow* parent, wxSizer* sizer)
	{
		parent->Disconnect(ID_POINTS_COMBO, ID_POINTS_COMBO, 
			wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(PointsOption::OnPointsCombo));

		sizer->Remove(0);

		spText_Points->Destroy();
		spText_Points = NULL;

		spPointsCombo->Destroy();
		spPointsCombo = NULL;
	}

	virtual void UpdateUI(const wxString& key, const wxString& value)
	{
		wxASSERT(swPoints == key);

		value.ToLong(&sPoints);
	}

	void OnPointsCombo(wxCommandEvent &)
	{
		wxInt32 sel = spPointsCombo->GetSelection();
		wxASSERT(-1 != sel);

		wxString str = spPointsCombo->GetString(sel);
		
		Controller::get().Transmit(shEventUpdateOption, 
			DataObject(swPoints, str));
	}

	virtual void SaveToXML(wxXmlNode* node)
	{
		wxXmlNode* child = new wxXmlNode(node, wxXML_ELEMENT_NODE,
			swPoints);
		child->AddProperty(swValue, wxString::Format(swIntFormat.c_str(), sPoints));
	}

	virtual void LoadFromXML(const wxString& option, wxXmlNode* node)
	{
		wxXmlNode* child = node->GetChildren();
		wxString value;

		if( (swPoints == child->GetName()) &&
			(true == child->GetPropVal(swValue, &value)))
		{
			RULE.Execute(shRuleSilentUpdateOption, DataObject(option,
				swPoints, value));
		}
	}
};

IMPLEMENT_OPTION_UI(POINTS, PointsOption);

//---------------------------------------------------------------------------//
// Class: RuleInitializePoints
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
//     <Points>
//
// Mixin To:
//     <RuleInitializeOptions>
//
class RuleInitializePoints : public Rule
{
public:
	virtual void Execute(const DataObject &)
	{
		gameData<wxInt32>(shPoints) = sPoints;
	}
};

IMPLEMENT_RULE_MIXIN(RuleInitializePoints, RuleInitializeOptions, POINTS)

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
