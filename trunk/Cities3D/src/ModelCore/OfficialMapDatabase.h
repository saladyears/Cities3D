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
#include <map>

//---------------------------- USER INCLUDES    -----------------------------//
#include "ModelExport.h"
#include "Singleton.h"

//---------------------------- DEFINES          -----------------------------//
#define MAPS_BUILD_MAPS			0	//turn this to 1 to rebuild the database

//---------------------------- TYPEDEFS         -----------------------------//
class Map;

//---------------------------- CLASSES          -----------------------------//
class MODEL_EXPORT_SPEC OfficialMapDatabase : 
	public Singleton<OfficialMapDatabase>
{
public:
	void AddMap(const Map &map);

	void WriteMaps();

private:
	typedef std::map<wxInt32, Map> MapMap;
	MapMap mMaps;

	friend class Singleton<OfficialMapDatabase>;
};

//---------------------------- PROTOTYPES       -----------------------------//
#if MAPS_BUILD_MAPS
#include "Map.h"

#define IMPLEMENT_OFFICIAL_MAP(T) \
namespace \
{ \
	struct sAutoRegister##T \
	{ \
		sAutoRegister##T(const Map &map) \
			{OfficialMapDatabase::get().AddMap(map);} \
	}; \
    T map##T; \
    sAutoRegister##T object##T(map##T); \
}
#else
#define IMPLEMENT_OFFICIAL_MAP(T)
#endif
