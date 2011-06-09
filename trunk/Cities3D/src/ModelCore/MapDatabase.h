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
#pragma once

#include "style.h"	//READ THIS BEFORE MAKING ANY CHANGES TO THIS FILE!!!

//---------------------------- SYSTEM INCLUDES  -----------------------------//

//---------------------------- USER INCLUDES    -----------------------------//
#include "Singleton.h"
#include "TileObject.h"
#include "ModelExport.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
typedef void (MapDatabase::*LPNMAPCOORDS)(const wxUint32, TileObject &);

class Map;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: MapDatabase
// 
// Singleton class for managing the map database on disk.  The
// map database stores all <Maps> available to the system for play.
// MapDatabase provides functions for loading the <Maps> and obtaining
// formatted text information about a given <Map>.
//
// Derived From:
//     <Singleton>
//
// Project:
//     <ModelCore>
//
// Include:
//     MapDatabase.h
//
class MODEL_EXPORT_SPEC MapDatabase : public Singleton<MapDatabase>
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: NumMaps
	//
	// Returns the number of <Maps> in the map database.
	//
	// Returns:
	//     The number of <Maps> in the map database.
	//
	wxUint32 NumMaps() const;
	
	//-----------------------------------------------------------------------//
	// Function: LoadMap
	//
	// Loads the <Map> at the given index from the map
	// database.
	//
	// Parameters:
	//     index - The index in the map database of the
	//             <Map> to load.
	//     map - The <Map> to store the loaded data in.
	//
	void LoadMap(const wxInt32 index, Map &map);

	//-----------------------------------------------------------------------//
	// Function: LoadMapByID
	//
	// Loads the <Map> with the given id from the map database.
	//
	// Parameters:
	//     id - The id of the <Map> to load.
	//     map - The <Map> to store the loaded data in.
	//
	// Returns:
	//     The index in the map database of the loaded <Map>, or
	//     -1 if no <Map> with the given id was found.
	//
	wxInt32 LoadMapByID(const wxInt32 id, Map &map);

	//-----------------------------------------------------------------------//
	// Function: FindMapIndexByName
	//
	// Finds the <Map> with the given name from the map database.
	//
	// Parameters:
	//     name - The name of the <Map> to load.
	//
	// Returns:
	//     The index in the map database of the loaded <Map>, or
	//     -1 if no <Map> with the given id was found.
	//
	wxInt32 FindMapIndexByName(const wxString& name);
	
	struct MapData
	{
		MapData(const wxInt32 _id, const wxString &_name, 
			const wxString& _internal_name, const wxInt32 _min, 
			const wxInt32 _max, const wxInt32 _points)
		: id(_id)
		, name(_name)
		, internal_name(_internal_name)
		, min_players(_min)
		, max_players(_max)
		, points(_points) 
		{}

		bool operator==(const wxInt32 _id) const
		{
			return (id == _id);
		}

		bool operator==(const wxString& _name) const
		{
			return (internal_name == _name);
		}	

		wxInt32 id;
		wxString name;
		wxString internal_name;
		wxInt32 min_players;
		wxInt32 max_players;
		wxInt32 points;
	};

	//-----------------------------------------------------------------------//
	// Function: MapInfo
	//
	// Returns a struct containing information about the <Map> at
	// the given index in the map database.
	//
	// Parameters:
	//     index - The index in the map database of the <Map> to look up.
	//
	// Returns:
	//     A const reference to a struct containing data about the
	//     map at the given index.
	//
	const MapData &MapInfo(const size_t index);

	//-----------------------------------------------------------------------//
	// Function: MapInfoString
	//
	// Provides a formatted text description of the <Map> at the 
	// given index in the map database, giving details like map name, 
	// type, # of players, points, map creator, etc.
	//
	// Parameters:
	//     index - The index in the map database of the <Map> to look up.
	//     pieces - The parts of the map description.
	//
	void MapInfoString(const size_t index, StringArray& pieces);

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: MapDatabase
	//
	// The MapDatabase constructor.  Calls <InitializeDatabase>
	// to prepare the map database cache.
	//
	MapDatabase();

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: InitializeDatabase
	//
	// Creates a cache of the map database information for
	// quick lookup in the future.
	//
	void InitializeDatabase();

	friend class Singleton<MapDatabase>;
};

//---------------------------- PROTOTYPES       -----------------------------//
