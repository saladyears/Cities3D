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
// Topic: Core
//
// _Core Overview_:
//
// The Core <RuleSet> provides <IRules> and <ILogic> that are foundational to 
// the playing of the game and which belong to all <RuleSets>, not any one in 
// particular.  They are divided up into the categories below.
//
// General Rules:
//     <RuleDecrementPlayer>,
//     <RuleIncrementPlayer>,
//     <RuleMarkBranch>,
//     <RulePlaySound>,
//     <RulePrepareUI>,
//     <RuleResetTileFlags>,
//     <RuleRestartGame>,
//     <RuleUpdateMessageUI>
//
// PreGame Rules:
//     <Rule0>,
//     <RuleFindPorts>,
//     <RuleGenerateBoard>,
//     <RuleGeneratePorts>,
//     <RuleInitializeData>,
//     <RuleLoadMap>,
//     <RuleRandomizePlayers>
//
// UI Rules:
//     <RuleUIAddPlayer>,
//     <RuleUIChangeColor>,
//     <RuleUIRemovePlayer>
//
// Rule Markers:
//     <RuleMarkerEndInitial>,
//     <RuleMarkerInitialRoad>,
//     <RuleMarkerMiddleInitial>,
//     <RuleMarkerStartInitial>
//
// Logic:
//     <LogicMarkBranch>,
//     <LogicUpdateMessageUIHotseat>
//
