/*
 *  Cities3D - Copyright (C) 2001-2004 Jason Fugate (saladyears@gmail.com)
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

//---------------------------- DEFINES          -----------------------------//
namespace
{
	//data files
	const wxString FILE_MAP			= wxT("maps.db");
	const wxString FILE_MAP_KEYS	= wxT("mapkey.db");
	const wxString FILE_PLAYERS		= wxT("players.db");
	const wxString FILE_GAMES		= wxT("games.db");
	const wxString FILE_GAME_KEYS	= wxT("gamekey.db");
	
	const HashString TEX_FLAG		= "Tex3D";
}

//---------------------------- TYPEDEFS         -----------------------------//
//DataFileType is an enumeration of the data files that Cities3D uses to store 
//its database information, like games, players, maps, etc.
enum DataFileType
{
	DataFileMaps					= 0,
	DataFileMapKeys					,
	DataFilePlayers					,
	DataFileGames					,
	DataFileGameKeys				,
	
	DataFileSize					,
	DataFileDword					= 0x7fffffff
};

//---------------------------- CLASSES          -----------------------------//

//---------------------------- PROTOTYPES       -----------------------------//
