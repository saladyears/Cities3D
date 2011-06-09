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

// Science Cards.
IMPLEMENT_IMAGE_SET_START(PlayerCards, Science)
IMPLEMENT_IMAGE_SET_NAME(Alchemist)
IMPLEMENT_IMAGE_SET_NAME(Crane)
IMPLEMENT_IMAGE_SET_NAME(Engineer)
IMPLEMENT_IMAGE_SET_NAME(Inventor)
IMPLEMENT_IMAGE_SET_NAME(Irrigation)
IMPLEMENT_IMAGE_SET_NAME(Medicine)
IMPLEMENT_IMAGE_SET_NAME(Mining)
IMPLEMENT_IMAGE_SET_NAME(RoadBuilding)
IMPLEMENT_IMAGE_SET_NAME(Smith)
IMPLEMENT_IMAGE_SET_END(CITIES, PlayerCards, Science, uicardsscience.tex)

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
