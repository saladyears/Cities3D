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
// Topic: AroundTheBoard
//
// _AroundTheBoard Overview_:
//
// The AroundTheBoard <RuleSet> adds one single <IRule> that generates the
// roll chips for a map with a certain shape, using a set sequence of
// roll chip values.  The official maps that use the AroundTheBoard <RuleSet>
// are Standard Settlers (III/IV), Standard Settlers(V/VI), New Shores,
// Greater Catan (IV), and GreaterCatan (V/VI).  The rules of around the board
// chip layout are as follows:
//
// 1 - A random starting corner on the board is selected.  A random rotation,
//     either clockwise or counter-clockwise is also selected.
//
// 2 - Starting from the initial corner, in the selected rotation, chips are
//     laid in that direction until either an ocean tile, or a tile with a chip
//     already on it, is reached.  When such a block is reached, the direction
//     of the chip laying is altered one hex side in the selected rotation and
//     the chip laying continues until all chips have been laid.
//
// 3 - The Desert tile does not have a chip laid on it.  Instead, the robber is
//     placed on the Desert tile.
//
// PreGame Rules:
//     <RuleGenerateChips>
//
