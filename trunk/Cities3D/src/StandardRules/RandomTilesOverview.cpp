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
// Topic: RandomTiles
//
// _RandomTiles Overview_:
//
// The RandomTiles <RuleSet> adds one single <IRule> that generates data
// at the start of the game for <TileObjects> that are marked as random 
// in the current <Map>.  The official maps that use the RandomTiles <RuleSet> 
// are Standard Settlers (III/IV), Standard Settlers (V/VI), New Shores,
// A New World (III), A New World (IV), A New World (V), A New World (VI),
// Greater Catan (IV), and Greater Catan (V/VI).  The rules of random tiles
// are as follows:
//
// 1 - All tiles marked as random are assigned a type corresponding to one
//     of the random types available for the map.
//
// 2 - All resource-producing random tiles are then randomly assigned a roll 
//     chip.  No roll chips with a value of 6 or 8 may be on adjacent tiles.  
//     If such a situation exists, one of the 6 or 8 chips is randomly switched
//     with another tile, until the rule is satisfied.
//
// PreGame Rules:
//     <RuleGenerateRandom>
//
