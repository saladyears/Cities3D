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
#include "ResourcePoolImageStrip.h"
#include "ImageEngine.h"
#include "Game.h"
#include "DataObject.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
wxResourcePoolImageStrip::wxResourcePoolImageStrip(wxWindow* parent, 
	wxWindowID id, boost::weak_ptr<Game> game, IResourceLogic* logic, bool any)
: wxInteractiveImageStrip(parent, id, 0, 10, logic)
{
	DataObject input(false), output;
	RULE.Decide(shLogicResourceOrder, input, output);
	
	typedef std::map<wxInt32, HashString> OrderMap;
	const OrderMap& map = output.read<OrderMap>();

	size_t size = map.size();

	if(true == any)
	{
		++size;
	}

	SetMaxImages(size);

	OrderMap::const_iterator it, itEnd = map.end();

	for(it = map.begin(); it != itEnd; ++it)
	{
		Add(it->second, 0);
	}

	// If we're using the AnyResource card, add it.
	if(true == any)
	{
		Add(shAnyOffer, 0);
	}

	SetPixelSpacing(10);
	SetFlags(FlagAlignCenter);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
