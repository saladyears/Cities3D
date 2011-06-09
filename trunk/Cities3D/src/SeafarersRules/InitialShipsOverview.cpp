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

//---------------------------------------------------------------------------//
// Topic: InitialShips
//
// _InitialShips Overview_:
//
// The InitialShips <RuleSet> adds one single <ILogic> that alters the way
// players can place their initial roads at the start of the game.  The 
// official maps that use the InitialShips <RuleSet> are Four Islands (III), 
// Four Islands (IV), Five Islands (V), and Six Islands (VI).  The rules of 
// initial ships are as follows:
//
// 1 - If the player builds an initial placement on a corner where and one or 
//     more of their ensuing road or ship placements are along a coast 
//     (meaning, one tile is land, the other is ocean), the player may not 
//     build a road for those coastal placements, but must build a ship 
//     instead.  The exception to the rule is if the ocean tile along the 
//     coast is also a border tile of the map.  In that case, the player 
//     may build a road.  Note that the player may always build a road for any 
//     placement that is between two land tiles.  This rule only applies to 
//     coastal placements.
//
// Initial Placement Logic:
//     <LogicPlaceInitialRoadNoCoasts>
//
