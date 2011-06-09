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
// Standard tile images.
IMPLEMENT_IMAGE_SET_START(TileType, Standard)
IMPLEMENT_IMAGE_SET_NAME(Timber)
IMPLEMENT_IMAGE_SET_NAME(Clay)
IMPLEMENT_IMAGE_SET_NAME(Sheep)
IMPLEMENT_IMAGE_SET_NAME(Wheat)
IMPLEMENT_IMAGE_SET_NAME(Ore)
IMPLEMENT_IMAGE_SET_NAME(Desert)
IMPLEMENT_IMAGE_SET_NAME(Ocean)
IMPLEMENT_IMAGE_SET_NAME(Port3)
IMPLEMENT_IMAGE_SET_NAME(PortTimber)
IMPLEMENT_IMAGE_SET_NAME(PortClay)
IMPLEMENT_IMAGE_SET_NAME(PortSheep)
IMPLEMENT_IMAGE_SET_NAME(PortWheat)
IMPLEMENT_IMAGE_SET_NAME(PortOre)
IMPLEMENT_IMAGE_SET_NAME(PortRandom)
IMPLEMENT_IMAGE_SET_END(STANDARD, TileType, Standard, gltilestandard.tex)

// Standard resource images.
IMPLEMENT_IMAGE_SET_START(Resources, Standard)
IMPLEMENT_IMAGE_SET_NAME(Sheep)
IMPLEMENT_IMAGE_SET_NAME(Ore)
IMPLEMENT_IMAGE_SET_NAME(Timber)
IMPLEMENT_IMAGE_SET_NAME(Clay)
IMPLEMENT_IMAGE_SET_NAME(Wheat)
IMPLEMENT_IMAGE_SET_NAME(Back)
IMPLEMENT_IMAGE_SET_NAME(AnyOffer)
IMPLEMENT_IMAGE_SET_NAME(AnyMatch)
IMPLEMENT_IMAGE_SET_END(STANDARD, Resources, Standard, uirescard.tex)

// Standard points images.
IMPLEMENT_IMAGE_SET_START(Points, Standard)
IMPLEMENT_IMAGE_SET_NAME(Points)
IMPLEMENT_IMAGE_SET_NAME(VictoryPoints)
IMPLEMENT_IMAGE_SET_NAME(Roads)
IMPLEMENT_IMAGE_SET_END(STANDARD, Points, Standard, uipoints.tex)

// Standard build images
IMPLEMENT_IMAGE_SET_START(Build, Standard)
IMPLEMENT_IMAGE_SET_NAME(road32)
IMPLEMENT_IMAGE_SET_NAME(road96)
IMPLEMENT_IMAGE_SET_NAME(settlement32)
IMPLEMENT_IMAGE_SET_NAME(settlement96)
IMPLEMENT_IMAGE_SET_NAME(city32)
IMPLEMENT_IMAGE_SET_NAME(city96)
IMPLEMENT_IMAGE_SET_END(STANDARD, Build, Standard, uibuild.tex)

// Standard dice.
IMPLEMENT_IMAGE_SET_START(Dice, Standard)
IMPLEMENT_IMAGE_SET_NAME(1)
IMPLEMENT_IMAGE_SET_NAME(2)
IMPLEMENT_IMAGE_SET_NAME(3)
IMPLEMENT_IMAGE_SET_NAME(4)
IMPLEMENT_IMAGE_SET_NAME(5)
IMPLEMENT_IMAGE_SET_NAME(6)
IMPLEMENT_IMAGE_SET_END(STANDARD, Dice, Standard, uidice.tex)

// Icons.
IMPLEMENT_IMAGE_SET_START(BuildUIIcons, Standard)
IMPLEMENT_IMAGE_SET_NAME(Trade)
IMPLEMENT_IMAGE_SET_NAME(Move)
IMPLEMENT_IMAGE_SET_NAME(Attack)
IMPLEMENT_IMAGE_SET_NAME(Expel)
IMPLEMENT_IMAGE_SET_END(STANDARD, BuildUIIcons, Standard, uiicons.tex)

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
