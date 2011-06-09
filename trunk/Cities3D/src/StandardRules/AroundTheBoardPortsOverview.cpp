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
// Topic: AroundTheBoardPorts
//
// AroundTheBoardPorts Overview_:
//
// The AroundTheBoardPorts <RuleSet> adds one single <IRule> that generates 
// ports for maps of a certain shape, that require alternating port-ocean-port-
// ocean (etc.) around the outside of the map.  The official maps that use the 
// AroundTheBoardPorts <RuleSet> are Standard Settlers (III/IV), and Standard 
// Settlers(V/VI).  The rules of around the board ports are as follows:
//
// 1 - Starting with the first ocean tile, a port-ocean or ocean-port sequence
//     is repeated until all random ports have been placed.
//
// PreGame Rules:
//     <RuleGenerateAroundBoardPorts>
//
