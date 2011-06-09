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
#include "BuildUIPanel.h"
#include "GameEngine.h"
#include "IPluginEngine.h"
#include "BuildData.h"
#include "BuildUI.h"
#include "SkinManager.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
wxBuildUIPanel::wxBuildUIPanel(wxWindow* parent, wxSettlersGLCanvas* canvas)
: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0)
{
	SetBackgroundColour(SKIN.Element(shBackground));

	wxSizer* horz = new wxBoxSizer(wxHORIZONTAL);
	wxSizer* vert = new wxBoxSizer(wxVERTICAL);

    typedef std::vector<BuildDataPtr> BuildDataArray;
	BuildDataArray array;
	PLUGIN.LoadBuildData(array);

	// Create a map of prioritized build data to use.
	typedef std::multimap<wxInt32, BuildDataPtr> BuildDataMap;
	typedef std::map<wxInt32, BuildDataMap> RowMap;
	RowMap rowMap;

	BuildDataArray::const_iterator it = array.begin(), itEnd = array.end();
	for(; it != itEnd; ++it)
	{
		BuildDataPtr data = (*it);

		// Sort by row, then priority within the row.
		rowMap[data->mRow].insert(std::make_pair(data->mPriority, data));
	}

	typedef std::vector<wxSizer*> SizerArray;
	SizerArray sizers(rowMap.size(), 0);

	for(size_t i = 0; i < rowMap.size(); ++i)
	{
		sizers[i] = new wxBoxSizer(wxHORIZONTAL);
	}

	wxSize minSize = wxSize(0, 0);
	size_t index = 0;
	bool calcX = true;
	bool calcY = true;

	// Now that everything is in rows by priority, create the UIs.
    RowMap::const_iterator it2 = rowMap.begin(), it2End = rowMap.end();
    for(; it2 != it2End; ++it2)
    {
		// Go in reverse order here because sizers add left to right, but the
		// priority goes right to left.
        BuildDataMap::const_reverse_iterator it3 = it2->second.rbegin(),
			it3End = it2->second.rend();

		// If it's a new row, use the next sizer.
		wxSizer* sizer = sizers[index];
		++index;
		
        for(; it3 != it3End; ++it3)
        {
			wxBuildUI* ui = new wxBuildUI(this, it3->second, canvas);
            
			wxSize min = ui->GetMinSize();
			if(true == calcX)
			{
				minSize.x += min.x + 2;
			}
			if(true == calcY)
			{
				minSize.y += min.y + 2;
				calcY = false;
			}

			sizer->Add(ui, 1, wxEXPAND | wxALL | wxSHAPED, 2);
		}

		calcX = false;
		calcY = true;
		vert->Add(sizer, 1, wxEXPAND);
    }

	minSize += wxSize(2, 2);

	horz->Add(vert, 1, wxEXPAND | wxTOP | wxLEFT | wxBOTTOM, 2);

	SetMinSize(minSize);
	SetSizer(horz);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
