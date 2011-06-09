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

// Barbarians overlay.
IMPLEMENT_IMAGE_SET_START(Barbarians, Cities)
IMPLEMENT_IMAGE_SET_NAME(Barbarians)
IMPLEMENT_IMAGE_SET_END(CITIES, Barbarians, Cities, glbarbarians.tex)

//cities resource images
IMPLEMENT_IMAGE_SET_START(Resources, Cities)
IMPLEMENT_IMAGE_SET_NAME(Paper)
IMPLEMENT_IMAGE_SET_NAME(Cloth)
IMPLEMENT_IMAGE_SET_NAME(Coin)
IMPLEMENT_IMAGE_SET_END(CITIES, Resources, Cities, uicomcard.tex)

//cities card images
IMPLEMENT_IMAGE_SET_START(Cards, Cities)
IMPLEMENT_IMAGE_SET_NAME(TradeCard)
IMPLEMENT_IMAGE_SET_NAME(PoliticsCard)
IMPLEMENT_IMAGE_SET_NAME(ScienceCard)
IMPLEMENT_IMAGE_SET_END(CITIES, Cards, Cities, uiprocard.tex)

//cities points images
IMPLEMENT_IMAGE_SET_START(Points, Cities)
IMPLEMENT_IMAGE_SET_NAME(DefenderPoints)
IMPLEMENT_IMAGE_SET_END(CITIES, Points, Cities, uipointsck.tex)

// Cities build images
IMPLEMENT_IMAGE_SET_START(Build, Cities)
IMPLEMENT_IMAGE_SET_NAME(walls32)
IMPLEMENT_IMAGE_SET_NAME(walls128)
IMPLEMENT_IMAGE_SET_NAME(knight132)
IMPLEMENT_IMAGE_SET_NAME(knight232)
IMPLEMENT_IMAGE_SET_NAME(knight332)
IMPLEMENT_IMAGE_SET_NAME(knight32_mask)
IMPLEMENT_IMAGE_SET_NAME(knight1128)
IMPLEMENT_IMAGE_SET_NAME(knight2128)
IMPLEMENT_IMAGE_SET_NAME(knight3128)
IMPLEMENT_IMAGE_SET_NAME(knight128_mask)
IMPLEMENT_IMAGE_SET_NAME(knight1128a)
IMPLEMENT_IMAGE_SET_NAME(knight132a)
IMPLEMENT_IMAGE_SET_NAME(Trade)
IMPLEMENT_IMAGE_SET_NAME(Politics)
IMPLEMENT_IMAGE_SET_NAME(Science)
IMPLEMENT_IMAGE_SET_NAME(CommercialHarbor)
IMPLEMENT_IMAGE_SET_NAME(Saboteur)
IMPLEMENT_IMAGE_SET_END(CITIES, Build, Cities, uibuildck.tex)

// Cities dice.
IMPLEMENT_IMAGE_SET_START(DiceRed, Cities)
IMPLEMENT_IMAGE_SET_NAME(Red1)
IMPLEMENT_IMAGE_SET_NAME(Red2)
IMPLEMENT_IMAGE_SET_NAME(Red3)
IMPLEMENT_IMAGE_SET_NAME(Red4)
IMPLEMENT_IMAGE_SET_NAME(Red5)
IMPLEMENT_IMAGE_SET_NAME(Red6)
IMPLEMENT_IMAGE_SET_END(CITIES, DiceRed, Cities, uidicered.tex)

IMPLEMENT_IMAGE_SET_START(DiceCK, Cities)
IMPLEMENT_IMAGE_SET_NAME(Barbarians)
IMPLEMENT_IMAGE_SET_NAME(Trade)
IMPLEMENT_IMAGE_SET_NAME(Politics)
IMPLEMENT_IMAGE_SET_NAME(Science)
IMPLEMENT_IMAGE_SET_END(CITIES, DiceCK, Cities, uidiceck.tex)

// Player icons.
IMPLEMENT_IMAGE_SET_START(PlayerIcon, Cities)
IMPLEMENT_IMAGE_SET_NAME(Trade)
IMPLEMENT_IMAGE_SET_NAME(Politics)
IMPLEMENT_IMAGE_SET_NAME(Science)
IMPLEMENT_IMAGE_SET_NAME(Merchant)
IMPLEMENT_IMAGE_SET_END(CITIES, PlayerIcon, Cities, uiplayerck.tex)

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
