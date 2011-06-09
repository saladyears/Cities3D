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
//seafarers port images
IMPLEMENT_IMAGE_SET_START(PortType, Seafarers)
IMPLEMENT_IMAGE_SET_NAME(Port3)
IMPLEMENT_IMAGE_SET_NAME(PortTimber)
IMPLEMENT_IMAGE_SET_NAME(PortWheat)
IMPLEMENT_IMAGE_SET_NAME(PortOre)
IMPLEMENT_IMAGE_SET_NAME(PortClay)
IMPLEMENT_IMAGE_SET_NAME(PortSheep)
IMPLEMENT_IMAGE_SET_NAME(PortRandom)
IMPLEMENT_IMAGE_SET_END(SEAFARERS, PortType, Seafarers, glseaports.tex)

// Seafarers build images
IMPLEMENT_IMAGE_SET_START(Build, Seafarers)
IMPLEMENT_IMAGE_SET_NAME(ship32)
IMPLEMENT_IMAGE_SET_NAME(ship96)
IMPLEMENT_IMAGE_SET_END(SEAFARERS, Build, Seafarers, uibuildship.tex)

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
