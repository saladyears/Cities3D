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
#include "OptionKeyMaps.h"
#include "BaseGrid.h"
#include "BaseStaticBox.h"
#include "BaseStaticText.h"
#include "BaseButton.h"
#include "IKeyMapEngine.h"
#include "GameEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	enum
	{
		ID_KEYMAP_GRID      = 0,
        ID_KEYMAP_ENTRY     = 1,
        ID_ASSIGN_BUTTON    = 2,
	};

	wxBaseGrid* spKeymapGrid = NULL;
	wxBaseStaticText* spActionText = NULL;
	wxBaseStaticText* spKeystrokeText = NULL;
    wxBaseStaticText* spAssignedToText = NULL;
    wxKeystrokeEntryTextCtrl* spKeystrokeEntryTextCtrl = NULL;
    wxBaseButton* spAssignButton = NULL;

    wxInt32 sLastSelectedRow = -1;
	wxInt32 sIgnoreCount = 0;
    wxKeyEvent sCurrentKeyEvent;
    
    void PopulateKeymapGrid()
    {
        // Fill in the grid.
	    spKeymapGrid->BeginBatch();
        spKeymapGrid->ClearGrid();
    	
	    wxInt32 row = 0;

	    IKeyMapEngine::EventMap eventMap;
        KEYMAP.GetEventMap(eventMap);

	    IKeyMapEngine::EventMap::const_iterator it, itEnd = eventMap.end();
	    for(it = eventMap.begin(); it != itEnd; ++it)
	    {
		    const IKeyMapEngine::KeyMap& keymap = it->second;
            const wxKeyEvent& event = keymap.defaultEvent;

		    spKeymapGrid->AppendRows();
            spKeymapGrid->SetReadOnly(row, 0);
            spKeymapGrid->SetReadOnly(row, 1);
		    spKeymapGrid->SetCellValue(row, 0, keymap.description);

            // Set the invisible row header to the name of the keymap.
            spKeymapGrid->SetRowLabelValue(row, keymap.name);

		    // Start with the modifiers.
            wxString keystroke;

            KEYMAP.GetKeyText(event, keystroke);

            spKeymapGrid->SetCellValue(row, 1, keystroke);

		    ++row;
	    }

	    spKeymapGrid->AutoSize();
	    spKeymapGrid->EndBatch();
    }

    void IsCurrentKeyEvent(const wxKeyEvent& event, bool& valid, bool& displayable)
    {
        displayable = true;
        valid = true;

        // If we got a full event (i.e. an acceptable character), update the 
        // keystroke label to the changed color and turn on the assign button.
        switch(event.GetKeyCode())
        {
        case 0:
        case WXK_COMMAND:
        case WXK_RETURN:
        case WXK_TAB:
		case WXK_ESCAPE:
		case WXK_WINDOWS_LEFT:
		case WXK_WINDOWS_RIGHT:
		case WXK_WINDOWS_MENU:
            displayable = false;

        // Intentional fall through.
        case WXK_CONTROL:
        case WXK_ALT:
        case WXK_SHIFT:
            valid = false;
        
            break;
        }
    }

    bool CheckForCollision(const wxString& keystroke, wxString& description)
    {
        bool ret = false;

        for(wxInt32 i = 0; i < spKeymapGrid->GetNumberRows(); ++i)
        {
            if(keystroke == spKeymapGrid->GetCellValue(i, 1))
            {
                description = spKeymapGrid->GetCellValue(i, 0);
                ret = true;
                break;
            }
        }

        return ret;
    }
}

//---------------------------- PUBLIC           -----------------------------//
wxOptionKeyMaps::wxOptionKeyMaps(wxWindow* parent)
: wxOptionWindow(parent, swKeyMaps)
{
	wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);

	wxBaseStaticBox *box = new wxBaseStaticBox(this, -1, stKeyBindings, 
		wxDefaultPosition, wxSize(200, 0));
	wxStaticBoxSizer *boxSizer = new wxStaticBoxSizer(box, wxHORIZONTAL);

	// Create the grid.
	spKeymapGrid = new wxBaseGrid(this, ID_KEYMAP_GRID, wxDefaultPosition,
		wxSize(250, 135));
	spKeymapGrid->CreateGrid(0, 2, wxGrid::wxGridSelectRows);
	spKeymapGrid->SetColLabelValue(0, stAction);
	spKeymapGrid->SetColLabelValue(1, stKeystroke);
    spKeymapGrid->SetRowLabelSize(0);
	spKeymapGrid->DisableDragColSize();
    spKeymapGrid->DisableDragRowSize();
    spKeymapGrid->SetColLabelSize(wxGRID_AUTOSIZE);

	PopulateKeymapGrid();

	boxSizer->Add(spKeymapGrid, 1, wxEXPAND);

	pSizer->Add(boxSizer, 1, wxEXPAND);

	wxBoxSizer* innerHorz1 = new wxBoxSizer(wxHORIZONTAL);
	wxBaseStaticText* text = new wxBaseStaticText(this, -1, 
		stAction + swColon);
	spActionText = new wxBaseStaticText(this, -1, swEmpty,
		shDefaultText, shBoldFont, wxSize(70, 0));

	innerHorz1->Add(text, 0, wxEXPAND);
	innerHorz1->Add(spActionText, 1, wxEXPAND | wxLEFT, 7);

	wxBoxSizer* innerHorz2 = new wxBoxSizer(wxHORIZONTAL);
	text = new wxBaseStaticText(this, -1, stKeystroke + swColon);

	spKeystrokeText = new wxBaseStaticText(this, -1, swEmpty,
		shDefaultText, shBoldFont, wxSize(70, 0));

	innerHorz2->Add(text, 0, wxEXPAND);
	innerHorz2->Add(spKeystrokeText, 1, wxEXPAND | wxLEFT, 7);

    wxBoxSizer* innerHorz3 = new wxBoxSizer(wxHORIZONTAL);
    spAssignedToText = new wxBaseStaticText(this, -1,
        swEmpty, shInfoText, shInfoFont, wxSize(0, 22));
    innerHorz3->Add(spAssignedToText, 1, wxEXPAND);

    wxBoxSizer* innerHorz4 = new wxBoxSizer(wxHORIZONTAL);
	text = new wxBaseStaticText(this, -1, stNewKeystroke + swColon);
    spKeystrokeEntryTextCtrl = new wxKeystrokeEntryTextCtrl(this, 
        ID_KEYMAP_ENTRY, this, wxDefaultPosition, wxSize(25, 20));
    spKeystrokeEntryTextCtrl->Disable();
    spAssignButton = new wxBaseButton(this, ID_ASSIGN_BUTTON, stAssign);
    spAssignButton->Disable();
  
    innerHorz4->Add(text, 0, wxALIGN_CENTRE_VERTICAL);
    innerHorz4->Add(spKeystrokeEntryTextCtrl, 0, wxALIGN_CENTRE_VERTICAL | wxLEFT, 7);
    innerHorz4->AddStretchSpacer();
    innerHorz4->Add(spAssignButton, 0, wxEXPAND);

	pSizer->Add(innerHorz1, 0, wxEXPAND | wxALL, 7);
	pSizer->Add(innerHorz2, 0, wxEXPAND | wxRIGHT | wxLEFT | wxBOTTOM, 7);
    pSizer->Add(innerHorz3, 0, wxEXPAND | wxRIGHT | wxLEFT | wxBOTTOM, 7);
    pSizer->Add(innerHorz4, 0, wxEXPAND | wxRIGHT | wxLEFT | wxBOTTOM, 7);

	SetSizer(pSizer);
	pSizer->SetSizeHints(this);
	pSizer->Layout();
}

void wxOptionKeyMaps::HandleKeyEvent(const wxKeyEvent& event, bool keydown)
{
	wxLogDebug(wxT("%d"), event.GetKeyCode());

	// Check if we're ignoring this event.
	if( (0 < sIgnoreCount) &&
		(false == keydown))
	{
		--sIgnoreCount;
		return;
	}

    bool displayable = true;
    bool valid = true;
    sCurrentKeyEvent.m_keyCode = 0;
    wxString assignedTo = swEmpty;

    // Inspect the validity of our key event.
    IsCurrentKeyEvent(event, valid, displayable);

    if(true == displayable)
    {
        wxString keystroke;
        KEYMAP.GetKeyText(event, keystroke);
        
		// Change the color of the keystroke if it's different from the current
		// keystroke.
		wxString currentKeystroke = spKeymapGrid->GetCellValue(sLastSelectedRow, 1);
		if(currentKeystroke == keystroke)
		{
			spKeystrokeText->SetTextColor(shDefaultText);
            spAssignButton->Disable();
		}
		else
		{
			spKeystrokeText->SetTextColor(shUserChangeableText);
			spAssignButton->Enable(valid);

            // We also need to check for clashes.
            wxString description;
            if( (true == valid) &&
                (true == CheckForCollision(keystroke, description)))
            {
                assignedTo = wxString::Format(
                    stIsCurrentlyAssignedTo.c_str(), keystroke.c_str(),
                    description.c_str());
            }
		}

		// If it's valid keystroke, we need to ignore the next X keystrokes
		// for the amount of modifiers on the stroke.
		if(true == valid)
		{
			sIgnoreCount = 1;
			sIgnoreCount += event.ControlDown();
			sIgnoreCount += event.AltDown();
			sIgnoreCount += event.ShiftDown();

            sCurrentKeyEvent = event;
		}

		spKeystrokeText->SetLabel(keystroke);		
    }

    spAssignedToText->SetLabel(assignedTo);
    spKeystrokeEntryTextCtrl->Clear();
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxOptionKeyMaps, wxOptionWindow)
    EVT_BUTTON(ID_ASSIGN_BUTTON, wxOptionKeyMaps::OnAssign)
	EVT_GRID_CMD_CELL_LEFT_CLICK(ID_KEYMAP_GRID, wxOptionKeyMaps::OnGridCellLeftClick)
	EVT_GRID_CMD_LABEL_LEFT_CLICK(ID_KEYMAP_GRID, wxOptionKeyMaps::OnGridLabelLeftClick)
END_EVENT_TABLE()

void wxOptionKeyMaps::OnAssign(wxCommandEvent&)
{
    wxString keystroke;
    KEYMAP.GetKeyText(sCurrentKeyEvent, keystroke);

    // Update the keymap.
    wxString collision;
    wxString name = spKeymapGrid->GetRowLabelValue(sLastSelectedRow);
    KEYMAP.OverrideKeyEvent(sCurrentKeyEvent, name, collision);

    // Update the grid.
    wxASSERT(0 <= sLastSelectedRow && spKeymapGrid->GetNumberRows() > sLastSelectedRow);
    spKeymapGrid->SetCellValue(sLastSelectedRow, 1, keystroke);

    // Update any collision.
    if(false == collision.IsEmpty())
    {
        // Find this row in the grid.
        for(wxInt32 i = 0; i < spKeymapGrid->GetNumberRows(); ++i)
        {
            if(collision == spKeymapGrid->GetRowLabelValue(i))
            {
                spKeymapGrid->SetCellValue(i, 1, swEmpty);
                break;
            }
        }
    }

    // Update the UI.
    spKeystrokeText->SetLabel(keystroke);
	spKeystrokeText->SetTextColor(shDefaultText);
    spAssignedToText->SetLabel(swEmpty);

    // Disable the button.
    spAssignButton->Disable();
}

void wxOptionKeyMaps::OnGridCellLeftClick(wxGridEvent& event)
{
	wxInt32 row = event.GetRow();

    if(row != sLastSelectedRow)
    {
	    wxString description = spKeymapGrid->GetCellValue(row, 0);
	    wxString keystroke = spKeymapGrid->GetCellValue(row, 1);

	    spActionText->SetLabel(description);
	    spKeystrokeText->SetLabel(keystroke);
		spKeystrokeText->SetTextColor(shDefaultText);
        spAssignedToText->SetLabel(swEmpty);

	    spKeymapGrid->SelectRow(row);

        spKeystrokeEntryTextCtrl->Enable();
        spAssignButton->Disable();

		sLastSelectedRow = row;
    }
}

void wxOptionKeyMaps::OnGridLabelLeftClick(wxGridEvent& event)
{
	spActionText->SetLabel(swEmpty);
	spKeystrokeText->SetLabel(swEmpty);
	spKeystrokeText->SetTextColor(shDefaultText);
    
    sLastSelectedRow = -1;
    spKeystrokeEntryTextCtrl->Disable();
    spAssignButton->Disable();
    spAssignedToText->SetLabel(swEmpty);
}
