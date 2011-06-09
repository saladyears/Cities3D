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
//standard development card back image
IMPLEMENT_IMAGE_SET_START(Cards, DevCards)
IMPLEMENT_IMAGE_SET_NAME(DevCard)
IMPLEMENT_IMAGE_SET_END(DEVCARDS, Cards, DevCards, uidevcard.tex)

//standard soldiers played image
IMPLEMENT_IMAGE_SET_START(Points, DevCards)
IMPLEMENT_IMAGE_SET_NAME(Soldier)
IMPLEMENT_IMAGE_SET_END(DEVCARDS, Points, DevCards, uipointsdev.tex)

//standard development card build image
IMPLEMENT_IMAGE_SET_START(BuildDevCard, DevCards)
IMPLEMENT_IMAGE_SET_NAME(DevCard)
IMPLEMENT_IMAGE_SET_END(DEVCARDS, BuildDevCard, DevCards, uibuilddevcard.tex)

// Development Cards.
IMPLEMENT_IMAGE_SET_START(PlayerCards, DevCards)
IMPLEMENT_IMAGE_SET_NAME(VictoryPoint)
IMPLEMENT_IMAGE_SET_NAME(RoadBuilding)
IMPLEMENT_IMAGE_SET_NAME(YearOfPlenty)
IMPLEMENT_IMAGE_SET_NAME(Monopoly)
IMPLEMENT_IMAGE_SET_NAME(Soldier)
IMPLEMENT_IMAGE_SET_END(DEVCARDS, PlayerCards, DevCards, uicardsdev.tex)

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//


