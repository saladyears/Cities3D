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
#include "OfficialMapDatabase.h"
#include "FilePath.h"
#include "Map.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
void OfficialMapDatabase::AddMap(const Map &map)
{
	mMaps[map.read<wxInt32>(shID)] = map;
}

void OfficialMapDatabase::WriteMaps()
{
	//save the existing map database

	//map file
	wxFile mapdb;
	mapdb.Open(FilePath::Data(DataFileMaps), wxFile::write);
	wxFileOutputStream x(mapdb);
	wxDataOutputStream maps(x);

	//key file
	wxFile keydb;
	keydb.Open(FilePath::Data(DataFileMapKeys), wxFile::write);
	wxFileOutputStream y(keydb);
	wxDataOutputStream keys(y);

	MapMap::const_iterator it = mMaps.begin(), itEnd = mMaps.end();
	for(; it != itEnd; ++it)
	{
		//get the starting position
		wxUint32 pos = mapdb.Tell();

		//write out the map
		it->second.save(maps);

		//write the offset
		keys << pos;
	}

	//close the files
	mapdb.Close();
	keydb.Close();
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//

