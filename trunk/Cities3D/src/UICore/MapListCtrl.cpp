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
#include "MapListCtrl.h"
#include "MapDatabase.h"
#include "Map.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	wxString sFields_MapListCtrl[] = {swMap, swPoints, swPlayers};
}

//---------------------------- PUBLIC           -----------------------------//
wxMapListCtrl::wxMapListCtrl(wxWindow* parent, wxWindowID id, 
							 const wxPoint& pos, const wxSize& size)
: wxBaseListCtrl(parent, id, sFields_MapListCtrl, 
	_countof(sFields_MapListCtrl), shMapListMainFont, shMapListSubFont, pos, 
	size, 2)
{
	ImportMaps();
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void wxMapListCtrl::ImportMaps()
{
	size_t size = MapDatabase::get().NumMaps();
	
	for(size_t i = 0; i < size; ++i)
	{
		const MapDatabase::MapData &data = MapDatabase::get().MapInfo(i);

		//fill in the information
		wxInt32 index = AppendItem(data.name);

		wxString numPlayers;
		if(data.min_players == data.max_players)
		{
            static const wxString stXPlayers = _("%d players");
	
			numPlayers = wxString::Format(stXPlayers.c_str(), data.min_players);
		}
		else
		{
            static const wxString stXtoXPlayers = _("%d-%d players");
	
			numPlayers = wxString::Format(stXtoXPlayers.c_str(), data.min_players, 
				data.max_players);
		}

		SetItemText(index, swPlayers, numPlayers);

        static const wxString stXPoints = _("%d points");
	
		wxString points = wxString::Format(stXPoints.c_str(), data.points);

		SetItemText(index, swPoints, points);
	}
}
