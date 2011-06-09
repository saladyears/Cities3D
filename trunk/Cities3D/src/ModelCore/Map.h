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
#include <wx/datstrm.h>
#include <list>
#include <boost/function.hpp>

//---------------------------- USER INCLUDES    -----------------------------//
#include "Data.h"
#include "TileObject.h"
#include "ModelExport.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class MapDatabase;

typedef boost::function1<void, wxInt32> TileCallback;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: Map
// 
// Data class for storing one game map.  Map holds all of the information that
// a single playable map requires in order to be played, such as points, 
// minimum and maximum players, stock amounts, allowed cards, as well as other 
// data like the map's name and creator, and any other data that is 
// map-specific.  Even more importantly, a Map also holds all of the 
// <TileObjects> that make up the particular layout of that map, and also 
// contains a list of all <RuleSets> that the Map requires to be loaded in 
// order for to be played. Maps are manipulated via the <MapDatabase>.
//
// Derived From:
//     <Data>
//
// Project:
//     <ModelCore>
//
// Include:
//     Map.h
//
class MODEL_EXPORT_SPEC Map : public Data
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: Map
	//
	// The Map constructor.  Initializes default data values
	// that all maps use.
	//
	Map();

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	typedef std::vector<TileObject> TileArray;

	typedef std::pair<wxString, wxString> StringPair;
	typedef std::vector<StringPair> StringPairArray;

	//-----------------------------------------------------------------------//
	// Function: name
	//
	// Returns the name of the map.
	//
	// Returns:
	//     The name of the map.
	//
	const wxString& name() const
	{
		return mName;
	}

	//-----------------------------------------------------------------------//
	// Function: internal_name
	//
	// Returns the internal name of the map.
	//
	// Returns:
	//     The internal name of the map.
	//
	const wxString& internal_name() const
	{
		return mInternalName;
	}

	//-----------------------------------------------------------------------//
	// Function: creator
	//
	// Returns the name of the creator of the map.
	//
	// Returns:
	//     The name of the creator of the map.
	//
	const wxString& creator() const
	{
		return mCreator;
	}
	
	
	//-----------------------------------------------------------------------//
	// Function: tiles_begin
	//
	// Provides read-only access to the start of the conatiner 
	// of <TileObjects> that make up the Map.
	//
	// Returns:
	//      A const_iterator to the start of <TileObject> container.
	//
	TileArray::const_iterator tiles_begin() const {return mTiles.begin();}

	//-----------------------------------------------------------------------//
	// Function: tiles_end
	//
	// Provides read-only access to the end of the conatiner 
	// of <TileObjects> that make up the Map.
	//
	// Returns:
	//      A const_iterator to the end of <TileObject> container.
	//
	TileArray::const_iterator tiles_end() const {return mTiles.end();}
	
	//-----------------------------------------------------------------------//
	// Function: rulesets
	//
	// Provides read-only access to the <RuleSet> names that the Map requires 
	// to be played.
	//
	// Returns:
	//      The rulesets.
	//
	const StringPairArray& rulesets() const
	{
		return mRuleSets;
	}

	//-----------------------------------------------------------------------//
	// Function: load
	//
	// Loads a Map from an input stream.
	//
	// Parameters:
	//     stream - The input stream.
	//
	virtual void load(wxDataInputStream &stream);

	//-----------------------------------------------------------------------//
	// Function: save
	//
	// Writes a Map to an output stream.
	//
	// Parameters:
	//     stream - The output stream.
	//
	virtual void save(wxDataOutputStream &stream) const;

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Static Functions
	//

	//-----------------------------------------------------------------------//
	// Function: CreateMap
	//
	// Creates a map of the given dimensions and sets all the
	// location and side information for each tile.  Calls the 
	// provided callback function (if any) to set tile-specific
	// data.
	//
	// Parameters:
	//     map - The Map to create.
	//     width - The number of tiles in the top row in the Map.
	//     rows - The number of rows in the Map.
	//     func - A callback function to allow tile specific data to be set.
	//
	static void CreateMap(Map &map, wxInt32 width, wxInt32 rows, 
		const TileCallback &func);

	//-----------------------------------------------------------------------//
	// Group: Variables
	//
	
	//-----------------------------------------------------------------------//
	// String: mName
	//
	// The name of the map.
	//
	wxString mName;

	//-----------------------------------------------------------------------//
	// String: mInternalName
	//
	// The internal name of the map.  Without spaces so the site can select it.
	//
	wxString mInternalName;

	//-----------------------------------------------------------------------//
	// String: mCreator
	//
	// The name of the map creator.
	//
	wxString mCreator;

	//-----------------------------------------------------------------------//
	// Object: mTiles
	//
	// The container of <TileObjects> that make up the Map.
	//
	TileArray mTiles;

	//-----------------------------------------------------------------------//
	// Object: mRuleSets
	//
	// The container of pairs of module/rule sets that the Map needs 
	// in order to be played.
	//
	StringPairArray mRuleSets;
};

//---------------------------- PROTOTYPES       -----------------------------//
