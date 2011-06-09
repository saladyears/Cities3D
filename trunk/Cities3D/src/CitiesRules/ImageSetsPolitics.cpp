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


#include "ImageSet.h"
#include "RuleSetDatabase.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//---------------------------- STATICS          -----------------------------//

// Politics Cards.
IMPLEMENT_IMAGE_SET_START(PlayerCards, Politics)
IMPLEMENT_IMAGE_SET_NAME(Arsonist)
IMPLEMENT_IMAGE_SET_NAME(Bishop)
IMPLEMENT_IMAGE_SET_NAME(Deserter)
IMPLEMENT_IMAGE_SET_NAME(Diplomat)
IMPLEMENT_IMAGE_SET_NAME(Intrigue)
IMPLEMENT_IMAGE_SET_NAME(Saboteur)
IMPLEMENT_IMAGE_SET_NAME(Spy)
IMPLEMENT_IMAGE_SET_NAME(Warlord)
IMPLEMENT_IMAGE_SET_NAME(Wedding)
IMPLEMENT_IMAGE_SET_END(CITIES, PlayerCards, Politics, uicardspolitics.tex)

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
