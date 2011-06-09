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
// Topic: TradeRoutes
//
// _TradeRoutes Overview_:
//
// Needs documentation.
//
// The official maps that use the TradeRoutes <RuleSet> are 
// Great Crossing (IV), and Great Crossing (VI).  The rules of trade routes are
// as follows:
//
// Initial Placement:
// 1 - Each player selects one of the map's islands as a "home island" with 
//     their first initial placement.  The user may select any of the islands 
//     as a home island as long as less than half of the players in the game 
//     have already selected it as their home island.
//
// 2 - The player's second initial placement must also be on their home island.
//
// Rules:
//     <RuleInitializeTradeRoutes>
//
// Initial Placement Rules:
//     <RulePlaceInitialTradeRoutes>
//
// Initial Placement Logic:
//     <LogicPlaceInitialTileHomeIsland>
//
