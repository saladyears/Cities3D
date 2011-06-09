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
// Topic: PortPlacement
//
// _PortPlacement Overview_:
//
// The PortPlacement <RuleSet> adds <IRules> that allow players to manually
// set a fixed number of ports on the map before the game starts.  The port 
// types themselves are not determined until the all ports have been placed.  
// The number of ports is determined by the map, specifically the values in the
// RandomPorts IntHash data.  The official maps that use the PortPlacement 
// <RuleSet> are A New World (III), A New World (IV), A New World (V), and A 
// New World (VI). The rules of initial port placement are as follows:
//
// 1 - The players are randomly ordered, and a random starting player is 
//     selected to go first.
// 2 - Starting with the first player, and proceeding in order, each player 
//     selects any ocean tile that borders a land tile, and does not already 
//     have a port on it, to be a port tile.  If more than one land tile 
//     borders the selected port tile, the player then selects which specific 
//     land tile the port will border. As each port placed, it is drawn on 
//     screen as an unknown port (with a ? mark as its type).
// 3 - When the number of ports in the map have been placed, all ports are 
//     randomly changed from the unknown type to one of the known port types 
//     (i.e. timber, clay, 3 for 1, etc.).  The players are then reordered 
//     randomly, and play proceeds into initial settlement placing with the 
//     first player.
//
// Rules:
//     <RuleExaminePortPlacement>,
//     <RuleInitializePortPlacement>,
//     <RulePlacePortPlacement>,
//     <RuleRequestPortOrientation>,
//     <RuleRequestPortPlacement>,
//     <RuleSetPortPlacements>,
//     <RuleStartPortPlacement>
//
// Restart Rules:
//     <RuleRestartInitialPortOrientation>,
//     <RuleRestartInitialPortPlacement>
//
// Logic:
//     <LogicTileIsPortPortPlacement>
//
// Branch Logic:
//     <LogicBranchPortPlacement>
//
// Rule Chain:
//     RuleMarkerStartInitial - -> <RuleStartPortPlacement>, *100*
//
//     RuleStartPortPlacement - -> <RuleRequestPortPlacement>
//
//     RulePlacePortPlacement - -> <RuleSetPortPlacements>, or 
//          <RuleRequestPortPlacement>
//
//     RuleSetPortPlacements - -> <RuleRequestFirstSettlement>
//

