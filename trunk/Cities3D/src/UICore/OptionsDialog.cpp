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
#include "OptionsDialog.h"
#include "BaseModalSizer.h"
#include "BaseTreeCtrl.h"
#include "BaseButton.h"
#include "BaseStaticBox.h"
#include "FittedStaticText.h"
#include "GameEngine.h"
#include "IOptionUI.h"
#include "SkinManager.h"
#include "DataObject.h"
#include "IOptionEngine.h"
#include "IPluginEngine.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	enum
	{
		ID_OPTIONS_TREE			= 0,
		ID_SELECT_BUTTON		,
		ID_UNSELECT_BUTTON		,
		ID_CLEAR_BUTTON			,
		ID_SAVE_BUTTON			,
		ID_LOAD_BUTTON			,
	};

	wxBaseTreeCtrl *spTreeCtrl;
	wxBoxSizer *spOptionsSizer;
	wxBaseButton* spSelectButton;
	wxBaseButton* spUnselectButton;

	wxBaseButton* spClearButton;
	wxBaseButton* spSaveButton;

	wxStaticBoxSizer* spDescriptionSizer;
	wxBaseStaticBox* spDescriptionBox;
	wxFittedStaticText* spDescriptionText;

	wxStaticBoxSizer* spSettingsSizer;
	wxBaseStaticBox* spSettingsBox;

	class wxOptionTreeItemData : public wxTreeItemData
	{
	public:
		wxOptionTreeItemData(const wxString& ruleset)
			: mRuleset(ruleset)
		{}

		const wxString& ruleset() const
		{
			return mRuleset;
		}

	private:
		wxString mRuleset;
	};

	class Sizer_OptionsDialog : public BaseModalSizer
	{
	public:
		Sizer_OptionsDialog(wxWindow *window) : BaseModalSizer(window) {}

		virtual void CreateSizers(wxBoxSizer *pSizer)
		{
			spTreeCtrl = new wxBaseTreeCtrl(mpWindow, ID_OPTIONS_TREE, 
				wxDefaultPosition, wxSize(220, 300), wxTR_HIDE_ROOT | 
				wxTR_HAS_BUTTONS | wxTR_LINES_AT_ROOT | wxTR_FULL_ROW_HIGHLIGHT | 
				wxTR_SINGLE);

			wxTreeItemId root;
			root = spTreeCtrl->AddRoot(swRoot);

			// Get all options from the option engine and add them to the tree.
			IOptionEngine::RulesetPriorityMap options;

			OPTION.GetOptions(options);
			IOptionEngine::RulesetPriorityMap::const_iterator it, 
				itEnd = options.end();

			wxString rootModule;
			wxTreeItemId current = root;

			for(it = options.begin(); it != itEnd; ++it)
			{
				const IOptionEngine::RulesetOptionMap& map = it->second;
				IOptionEngine::RulesetOptionMap::const_iterator itMap,
					itMapEnd = map.end();

				for(itMap = map.begin(); itMap != itMapEnd; ++itMap)
				{
					const wxString& module = itMap->first;
					const IOptionEngine::OptionPriorityMap& opts = 
						itMap->second;

					IOptionEngine::OptionPriorityMap::const_iterator itOpt, 
						itOptEnd = opts.end();
					for(itOpt = opts.begin(); itOpt != itOptEnd; ++itOpt)
					{
						const wxString& option = itOpt->second;

						// See if we need to add a new root.
						if(module != rootModule)
						{
							if(current != root)
							{
								spTreeCtrl->Expand(current);
							}

							const wxString& name = 
								PLUGIN.GetModuleName(module);
							wxOptionTreeItemData* data = 
								new wxOptionTreeItemData(module);

							current = spTreeCtrl->AppendItem(root, name);
							spTreeCtrl->SetItemData(current, data);
							rootModule = module;
						}

						const wxString& name = 
							PLUGIN.GetRulesetName(module, option);
						wxOptionTreeItemData* data = 
							new wxOptionTreeItemData(option);

						wxTreeItemId item = spTreeCtrl->AppendItem(current, 
							name);
						spTreeCtrl->SetItemData(item, data);
					}
				}
			}

			if(current != root)
			{
				spTreeCtrl->Expand(current);
			}

			wxBoxSizer *left = new wxBoxSizer(wxVERTICAL);
			
			wxBoxSizer *buttons = new wxBoxSizer(wxHORIZONTAL);

            static const wxString stSelect = _("Select");
	        static const wxString stUnselect = _("Unselect");
	
			spSelectButton = new wxBaseButton(mpWindow, ID_SELECT_BUTTON, 
				stSelect);
			spSelectButton->Disable();
			spUnselectButton = new wxBaseButton(mpWindow, ID_UNSELECT_BUTTON, 
				stUnselect);
			spUnselectButton->Disable();

			buttons->Add(spSelectButton);
			buttons->AddStretchSpacer();
			buttons->Add(spUnselectButton);

			left->Add(spTreeCtrl, 1, wxEXPAND | wxBOTTOM, 3);
			left->Add(buttons, 0, wxEXPAND | wxTOP | wxBOTTOM, 3);

			spOptionsSizer = new wxBoxSizer(wxVERTICAL);
			spOptionsSizer->SetMinSize(wxSize(225, 200));

            static const wxString stDescription = _("Description");

			spDescriptionBox = new wxBaseStaticBox(mpWindow, -1, 
				stDescription, wxDefaultPosition, wxSize(200, 25));
			spDescriptionSizer = new wxStaticBoxSizer(spDescriptionBox, 
				wxHORIZONTAL);
			spDescriptionSizer->Add(0, 0);
			spDescriptionBox->Hide();

            static const wxString stSettings = _("Settings");
	
			spSettingsBox = new wxBaseStaticBox(mpWindow, -1, stSettings,
				wxDefaultPosition, wxSize(200, 25));
			spSettingsSizer = new wxStaticBoxSizer(spSettingsBox,
				wxHORIZONTAL);
			spSettingsBox->Hide();

			spOptionsSizer->Add(spDescriptionSizer, 0, wxEXPAND);
			spOptionsSizer->Add(spSettingsSizer, 0, wxEXPAND | wxTOP, 7);

			pSizer->Add(left, 0, wxEXPAND | wxALL, 3);
			pSizer->Add(spOptionsSizer, 1, wxEXPAND | wxALL, 3);
		}

		virtual void CreateButtons(wxBoxSizer *pSizer)
		{
            static const wxString stDone = _("Done");
	
			wxBaseButton *done = new wxBaseButton(mpWindow, wxID_OK, 
				stDone);
			pSizer->Add(done, 0, wxALL, 3);
			pSizer->AddStretchSpacer();
			
            static const wxString stClear = _("Clear");
	        static const wxString stLoad = _("Load");
            static const wxString stSave = _("Save");
	
			spClearButton = new wxBaseButton(mpWindow, ID_CLEAR_BUTTON,
				stClear);
			spClearButton->Disable();
			pSizer->Add(spClearButton, 0, wxALL, 3); 
			spSaveButton = new wxBaseButton(mpWindow, ID_SAVE_BUTTON, 
				stSave);
			spSaveButton->Disable();
			pSizer->Add(spSaveButton, 0, wxALL, 3);
			wxBaseButton* load = new wxBaseButton(mpWindow, ID_LOAD_BUTTON,
				stLoad);
			pSizer->Add(load, 0, wxALL, 3);
		}
	};
}

//---------------------------- PUBLIC           -----------------------------//
#ifdef __WIN32__
	//disables the 'this' : used in base member initializer list warning
	#pragma warning(disable:4355)
#endif
wxOptionsDialog::wxOptionsDialog(wxWindow *parent)
: wxBaseModalDialog(parent, ISizerPtr(new Sizer_OptionsDialog(this)), 
	_("Game Options"), swGameOptions)
{
	UpdateTreeColors();

	Controller::get().AddReceiver(shEventUpdateOption, 
		&wxOptionsDialog::OnUpdateOption, this);
	Controller::get().AddReceiver(shEventPreGame, &wxOptionsDialog::OnPreGame, 
		this);
}

wxOptionsDialog::~wxOptionsDialog()
{
	Controller::get().RemoveReceiver(shEventUpdateOption, 
		&wxOptionsDialog::OnUpdateOption, this);
	Controller::get().RemoveReceiver(shEventPreGame, 
		&wxOptionsDialog::OnPreGame, this);

	spDescriptionSizer = NULL;
	spDescriptionBox = NULL;
	spDescriptionText = NULL;

	spSettingsSizer = NULL;
	spSettingsBox = NULL;
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxOptionsDialog, wxBaseModalDialog)
	EVT_TREE_SEL_CHANGED(ID_OPTIONS_TREE, wxOptionsDialog::OnTreeSelection)
	EVT_TREE_ITEM_ACTIVATED(ID_OPTIONS_TREE, wxOptionsDialog::OnTreeActivate)
	EVT_BUTTON(ID_SELECT_BUTTON, wxOptionsDialog::OnSelectButton)
	EVT_BUTTON(ID_UNSELECT_BUTTON, wxOptionsDialog::OnUnselectButton)
	EVT_BUTTON(ID_CLEAR_BUTTON, wxOptionsDialog::OnClearButton)
	EVT_BUTTON(ID_SAVE_BUTTON, wxOptionsDialog::OnSaveButton)
	EVT_BUTTON(ID_LOAD_BUTTON, wxOptionsDialog::OnLoadButton)
END_EVENT_TABLE()

void wxOptionsDialog::Ok()
{
	if(mOptionUI)
	{
		mOptionUI->DestroyUI(this, spSettingsSizer);
	}

	wxBaseModalDialog::Ok();
}

void wxOptionsDialog::UpdateTreeColors()
{
	wxInt32 selections = 0;

	// Iterate through the tree, setting colors to show selected and
	// excluded items.
	wxTreeItemId id = spTreeCtrl->GetRootItem();
	wxTreeItemIdValue cookie;

	wxTreeItemId child = spTreeCtrl->GetFirstChild(id, cookie);

	while(true == child.IsOk())
	{
		wxTreeItemIdValue leafCookie;
		wxTreeItemId leaf = spTreeCtrl->GetFirstChild(child, leafCookie);

		wxOptionTreeItemData* data = static_cast<wxOptionTreeItemData*>(
				spTreeCtrl->GetItemData(child));
		const wxString &module = data->ruleset();

		while(true == leaf.IsOk())
		{
			wxOptionTreeItemData* data = static_cast<wxOptionTreeItemData*>(
				spTreeCtrl->GetItemData(leaf));
			const wxString &option = data->ruleset();
			
			bool bold = false;
			wxColour color = SKIN.Element(shOptionUnselected);

			if(true == OPTION.IsSelected(module, option))
			{
				// Make selected options bold and colored.
				bold = true;
				color = SKIN.Element(shOptionSelected);

				++selections;
			}
			else if(true == OPTION.IsExcluded(module, option))
			{
				color = SKIN.Element(shOptionExcluded);
			}

			spTreeCtrl->SetItemBold(leaf, bold);
			spTreeCtrl->SetItemTextColour(leaf, color);

			leaf = spTreeCtrl->GetNextChild(child, leafCookie);
		}

		child = spTreeCtrl->GetNextChild(id, cookie);
	}

	spSaveButton->Enable(selections || false);
}

void wxOptionsDialog::UpdateSettings(const wxString& module, 
	const wxString& option, bool selected)
{
	bool show = false;

	// Destroy any existing option UI
	if(mOptionUI && (true == mOptionUI->HasUI()))
	{
		mOptionUI->DestroyUI(this, spSettingsSizer);
		mOptionUI = OptionUIPtr();
	}

	if(false == module.IsEmpty())
	{
		const OptionUIPtr &optionUI = OPTION.GetOptionUI(module, option);
		wxASSERT(optionUI);

		// See if it has UI that needs to be shown.
		if( (true == selected) &&
			(true == optionUI->HasUI()))
		{
			optionUI->CreateUI(this, spSettingsSizer);
			spSettingsBox->Show();

			mOptionUI = optionUI;

			show = true;
		}
	}

	spSettingsBox->Show(show);

	GetSizer()->Layout();
}

void wxOptionsDialog::OnPreGame(const GamePtr&)
{
	UpdateTreeColors();
}

void wxOptionsDialog::OnUpdateOption(const DataObject& object)
{
	const wxString& key = object.read<wxString>();
	const wxString& value = object.read<wxString>(1);

	// Create the option name out of the currently selected option.
	wxTreeItemId id = spTreeCtrl->GetSelection();
	wxASSERT(false == spTreeCtrl->ItemHasChildren(id));

	wxOptionTreeItemData* data = static_cast<wxOptionTreeItemData*>(
		spTreeCtrl->GetItemData(id));
	const wxString &option = data->ruleset();
	wxTreeItemId parent = spTreeCtrl->GetItemParent(id);

	data = static_cast<wxOptionTreeItemData*>(
			spTreeCtrl->GetItemData(parent));
	const wxString& module = data->ruleset();

	RULE.Execute(shRuleUpdateOption, DataObject(module + option, key, value));
}

void wxOptionsDialog::OnTreeSelection(wxTreeEvent &event)
{
	wxTreeItemId id = event.GetItem();

	// Enable / disable the buttons.  If the node has children, it is not an
	// option node, but one of the parents, so all buttons are disabled.
	if(true == spTreeCtrl->ItemHasChildren(id))
	{
		spSelectButton->Disable();
		spUnselectButton->Disable();

		spDescriptionBox->Hide();

		if(NULL != spDescriptionText)
		{
			spDescriptionText->Destroy();
			spDescriptionText = NULL;
		}

		UpdateSettings(swEmpty, swEmpty, false);
	}
	else
	{
		wxOptionTreeItemData* data = static_cast<wxOptionTreeItemData*>(
			spTreeCtrl->GetItemData(id));
		const wxString &option = data->ruleset();

		wxTreeItemId parent = spTreeCtrl->GetItemParent(id);
		
		data = static_cast<wxOptionTreeItemData*>(
			spTreeCtrl->GetItemData(parent));
		const wxString& module = data->ruleset();

		bool selected = OPTION.IsSelected(module, option);

		// If it's selected, the unselect is enabled.  If it's not selected,
		// check to see if it is excluded.  If it is not excluded, the
		// select is enabled.
		if(true == selected)
		{
			spSelectButton->Disable();
			spUnselectButton->Enable();
		}
		else
		{
			spUnselectButton->Disable();
			bool excluded = OPTION.IsExcluded(module, option);
			spSelectButton->Enable(!excluded);
		}

		// Set up the UI.
		const OptionUIPtr &optionUI = OPTION.GetOptionUI(module, option);
		wxASSERT(optionUI);

		const wxString& description = optionUI->description();

		// Set the description.
		if(NULL != spDescriptionText)
		{
			spDescriptionText->Destroy();
		}
		wxSize size = spDescriptionBox->GetSize();

		spDescriptionText = new wxFittedStaticText(this, -1, 
			description, shDefaultText, shDefaultFont, wxSize(size.x - 15, 0));
		size = spDescriptionText->GetMinSize();
		size.y += 5;
		spDescriptionText->SetMinSize(size);
		spDescriptionBox->Show();
		spDescriptionSizer->Add(spDescriptionText, 0, wxEXPAND | wxALL, 5);
		spDescriptionSizer->Layout();

		UpdateSettings(module, option, selected);
	}
}

void wxOptionsDialog::OnTreeActivate(wxTreeEvent &)
{
	wxCommandEvent event;

	if(true == spSelectButton->IsEnabled())
	{
		OnSelectButton(event);
	}
	else if(true == spUnselectButton->IsEnabled())
	{
		OnUnselectButton(event);
	}
}

void wxOptionsDialog::OnSelectButton(wxCommandEvent& )
{
	wxTreeItemId id = spTreeCtrl->GetSelection();
	wxASSERT(false == spTreeCtrl->ItemHasChildren(id));

	wxOptionTreeItemData* data = static_cast<wxOptionTreeItemData*>(
		spTreeCtrl->GetItemData(id));
	const wxString &option = data->ruleset();
	wxTreeItemId parent = spTreeCtrl->GetItemParent(id);

	data = static_cast<wxOptionTreeItemData*>(
			spTreeCtrl->GetItemData(parent));
	const wxString& module = data->ruleset();

	RULE.Execute(shRuleSelectOption, DataObject(module, option));

	spClearButton->Enable();
	spSelectButton->Disable();
	spUnselectButton->Enable();

	UpdateSettings(module, option, true);
}

void wxOptionsDialog::OnUnselectButton(wxCommandEvent& )
{
	wxTreeItemId id = spTreeCtrl->GetSelection();
	wxASSERT(false == spTreeCtrl->ItemHasChildren(id));

	wxOptionTreeItemData* data = static_cast<wxOptionTreeItemData*>(
		spTreeCtrl->GetItemData(id));
	const wxString &option = data->ruleset();
	wxTreeItemId parent = spTreeCtrl->GetItemParent(id);

	data = static_cast<wxOptionTreeItemData*>(
		spTreeCtrl->GetItemData(parent));
	const wxString& module = data->ruleset();

	RULE.Execute(shRuleUnselectOption, DataObject(module, option));

	bool optionsSelected = OPTION.HasSelectedOptions();
	spClearButton->Enable(optionsSelected);
	
	spSelectButton->Enable();
	spUnselectButton->Disable();

	UpdateSettings(module, option, false);
}

void wxOptionsDialog::OnClearButton(wxCommandEvent&)
{
	RULE.Execute(shRuleClearOptions, DataObject());

	UpdateSettings(swEmpty, swEmpty, false);

	spClearButton->Disable();
	spSaveButton->Disable();
}

void wxOptionsDialog::OnSaveButton(wxCommandEvent&)
{
    static const wxString stEnterAFileName = _("Enter a file name");
	
	wxFileDialog dialog(this, stEnterAFileName, swOptions, swEmpty,
		swXMLFiles, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if(wxID_OK == dialog.ShowModal())
	{
		wxString path = dialog.GetPath();

		OPTION.SaveOptionsToFile(path);
	}
}

void wxOptionsDialog::OnLoadButton(wxCommandEvent&)
{
    static const wxString stSelectAFile = _("Select a file");
	
	wxFileDialog dialog(this, stSelectAFile, swOptions, swEmpty,
		swXMLFiles, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if(wxID_OK == dialog.ShowModal())
	{
		wxString path = dialog.GetPath();

		OPTION.LoadOptionsFromFile(path);

		spClearButton->Enable();
	}
}
