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
#include "MapDatabase.h"
#include "FilePath.h"
#include "Map.h"
#include "define/defineFile.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	//offsets into the map database file
	UintArray sOffsets;

	//data for all of the maps in the database
	typedef std::vector<MapDatabase::MapData> MapDataArray;
	MapDataArray sMapData;
}

//---------------------------- PUBLIC           -----------------------------//
wxUint32 MapDatabase::NumMaps() const 
{
	return sMapData.size();
}

void MapDatabase::LoadMap(const wxInt32 index, Map &map)
{
	wxASSERT(sOffsets.size() > (wxUint32) index);

	wxFileInputStream x(FilePath::Data(DataFileMaps));
	wxDataInputStream maps(x);

	//seek to this position in the game file
	x.SeekI(sOffsets[index]);

	map.load(maps);
}

wxInt32 MapDatabase::LoadMapByID(wxInt32 id, Map &map)
{
	wxInt32 ret = -1;

	MapDataArray::const_iterator it;

	it = std::find(sMapData.begin(), sMapData.end(), id);

	if(sMapData.end() != it)
	{
		wxInt32 index = it - sMapData.begin();
		LoadMap(index, map);
		ret = index;
	}

	return ret;
}

wxInt32 MapDatabase::FindMapIndexByName(const wxString& name)
{
	wxInt32 ret = -1;

	MapDataArray::const_iterator it;

	it = std::find(sMapData.begin(), sMapData.end(), name);

	if(sMapData.end() != it)
	{
		wxInt32 index = it - sMapData.begin();
		ret = index;
	}

	return ret;
}

const MapDatabase::MapData &MapDatabase::MapInfo(const size_t index)
{
	wxASSERT(sMapData.size() > index);

	return sMapData[index];
}

void MapDatabase::MapInfoString(const size_t index, StringArray& pieces)
{
	wxASSERT(sMapData.size() > index);

    static const wxString stPlayers = _("players");
    static const wxString stPoints = _("points");

	const MapData &data = sMapData[index];

	//map name
	pieces.push_back(data.name.wx_str());

	//number of players
	if(data.min_players == data.max_players)
	{
		pieces.push_back(wxString::Format(swHTMLListIntString.c_str(), 
            data.min_players, stPlayers.c_str()));
	}
	else
	{
		pieces.push_back(wxString::Format(swHTMLListIntIntString.c_str(), 
            data.min_players, data.max_players, stPlayers.c_str()));
	}

	//number of points
	pieces.push_back(wxString::Format(swHTMLListIntString.c_str(), 
        data.points, stPoints.c_str()));
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
MapDatabase::MapDatabase()
{
	InitializeDatabase();
}

void MapDatabase::InitializeDatabase()
{
	wxInt32 i;

	//reset the array of game IDs
	sMapData.clear();
	sOffsets.clear();

	//key file
	wxFile keydb;
	keydb.Open(FilePath::Data(DataFileMapKeys));
	wxFileInputStream y(keydb);
	wxDataInputStream keys(y);

	//sanity
	if(false == keydb.IsOpened())
	{
		return;
	}

	wxUint32 keyLength;
	wxInt32 size;

	//calculate the length of the key file
	keyLength = keydb.Length();

	//determine how many maps there are
	size = keyLength / sizeof(wxInt32);

	//reset the offsets
	sOffsets.resize(size);

	//read them in
	for(i = 0; i < size; i++)
	{
		keys >> sOffsets[i];
	}

	//now actually read all the map IDs
	for(i = 0; i < size; i++)
	{
		Map map;
		LoadMap(i, map);

		sMapData.push_back(
			MapData(map.read<wxInt32>(shID),
					map.name(),
					map.internal_name(),
					map.read<wxInt32>(shMinPlayers),
					map.read<wxInt32>(shMaxPlayers),
					map.read<wxInt32>(shPoints)));
	}
}


