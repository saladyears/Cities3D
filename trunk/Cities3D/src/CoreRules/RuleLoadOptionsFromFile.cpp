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
#include "Rule.h"
#include "RuleSetDatabase.h"
#include "GameEngine.h"
#include "Controller.h"
#include "IOptionEngine.h"
#include "IOptionUI.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleLoadOptionsFromFile
// 
// Loads all options from the given file, which is passed in as a string.
//
// Derived From:
//     <Rule>
//
// Project:
//     <CoreRules>
//
// RuleSet:
//     <Core>
//
class RuleLoadOptionsFromFile : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		const wxString& str = object.read<wxString>();

		// Write the string to a memory buffer, then read it into an
		// xml doc, then finally get the root node as the node to
		// parse.
		size_t size = str.Length();
		boost::shared_array<wxUint8> data;
		data.reset(new wxUint8[size]);
		memset(data.get(), 0, size);

		wxMemoryOutputStream mem(data.get(), size);
		mem.GetOutputStreamBuffer()->Write(str.c_str(), size);
		wxMemoryInputStream in(mem);

		// Now, read it into an xml document.
		wxXmlDocument doc(in);
		wxXmlNode* root = doc.GetRoot();
		
		if(NULL != root)
		{
			wxString module;
			wxString option;

			// Iterate through all the children of the root, loading the 
			// options as we go.
			for(wxXmlNode *node = root->GetChildren(); NULL != node; 
				node = node->GetNext())
			{
				if( (swOption == node->GetName()) &&
					(true == node->GetPropVal(swModule, &module)) &&
					(true == node->GetPropVal(swOption, &option)))
				{
					if( (false == OPTION.IsSelected(module, option)) &&
						(false == OPTION.IsExcluded(module, option)))
					{
						RULE.Execute(shRuleSelectOption, DataObject(module, 
							option,	false));

						OptionUIPtr optionUI = OPTION.GetOptionUI(module, 
							option);
						// Load any extra parts, if needed.
						if(optionUI && true == optionUI->HasUI())
						{
							optionUI->LoadFromXML(module + option, node);
						}
					}
				}
			}

			// Now that we're done, tell everything to refresh.
			Controller::get().Transmit(shEventResetReady, 0);
			Controller::get().Transmit(shEventPreGame, GetGame());
		}
	}
};

IMPLEMENT_RULE(RuleLoadOptionsFromFile, CORE)
