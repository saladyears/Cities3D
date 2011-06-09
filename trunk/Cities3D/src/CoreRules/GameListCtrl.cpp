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
#include "GameListCtrl.h"
#include "GameDatabase.h"
#include "MapDatabase.h"
#include "Game.h"
#include "Map.h"
#include "GameEngine.h"
#include "ISerialize.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	wxString sFields_Game[] = {swMap, swDate, swPlayers};
}

//---------------------------- PUBLIC           -----------------------------//
wxGameListCtrl::wxGameListCtrl(wxWindow* parent, wxWindowID id, 
	const wxPoint& pos, const wxSize& size)
: wxBaseListCtrl(parent, id, sFields_Game, _countof(sFields_Game), 
	shGameListMainFont, shGameListSubFont, pos, size)
{
	ImportGames();
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void wxGameListCtrl::ImportGames()
{
	Lock();

	size_t size = GameDatabase::get().NumGames();

	struct FakeSerialize : public ISerialize
	{
		void load(wxDataInputStream &stream) {}
		void save(wxDataOutputStream &stream) const {}
	};

	FakeSerialize serialize;

	for(size_t i = 0; i < size; ++i)
	{
		Game game;
		GameDatabase::get().LoadGame(i, game, GameObjectEnginePtr(), serialize);

		Map map;
		MapDatabase::get().LoadMapByID(game.read<wxInt32>(shMapID), map);
		const wxString& mapName = map.name();

		wxInt32 index = AppendItem(mapName);
		
		// Date/Time
		wxDateTime time = game.read<wxDateTime>(shTimeStart);
		wxString str = time.Format(swTimeFormat);
		SetItemText(index, swDate, str);
		
		// Players
		wxInt32 numPlayers = game.numPlayers();
		
		str = swEmpty;
		for(wxInt32 j = 0; j < numPlayers; ++j)
		{
			if(0 != j)
			{
				str += swCommaSpace;
			}
			
			str += game.player(j).Name();
		}

		SetItemText(index, swPlayers, str);
	}

	Unlock();
}


