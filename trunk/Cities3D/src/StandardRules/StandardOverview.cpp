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
// Topic: Standard
//
// _Standard Overview_:
//
// Needs documentation.
//
// Rules:
//     <RulePlaceCity>,
//     <RulePlaceRoad>,
//     <RulePlaceRobber>,
//     <RulePlaceSettlement>,
//     <RulePlaySettlementSound>
//
// PreGame Rules:
//     <RuleInitializeStandard>
//
// Initial Placement Rules:
//     <RuleContinueInitialPlacements>,
//     <RuleDecreaseInitialPlayer>,
//     <RuleIncreaseInitialPlayer>,
//     <RuleInitialResources>,
//     <RuleLocateInitialRoad>,
//     <RulePlaceFirstSettlement>,
//     <RulePlaceInitialExtensions>,
//     <RulePlaceInitialPlacement>,
//     <RulePlaceInitialRoad>,
//     <RulePlaceSecondSettlement>,
//     <RuleRequestInitialPlacement>,
//     <RuleRequestInitialRoad>,
//     <RuleStartInitialPlacements>
//
// Build Rules:
//     <RuleBuildCity>,
//     <RuleBuildRoad>,
//     <RuleBuildSettlement>
//
// Restart Rules:
//     <RuleRestartFirstRoad>,
//     <RuleRestartFirstSettlement>,
//     <RuleRestartSecondRoad>,
//     <RuleRestartSecondSettlement>
//
// Tile Logic:
//     <LogicTileIsLand>,
//     <LogicTileIsOcean>,
//     <LogicTileIsPort>,
//     <LogicTileIsResource>,
//     <LogicTileIsLandStandard>,
//     <LogicTileIsOceanStandard>,
//     <LogicTileIsPortStandard>,
//     <LogicTileIsResourceStandard>
//
// Controls Logic:
//     <LogicCanShowCard>,
//     <LogicCanTradeBank>,
//     <LogicCanTradeOthers>,
//     <LogicCanEndTurn>
//
// Initial Placement Logic:
//     <LogicPlaceInitialBlock>,
//     <LogicPlaceInitialBlockSettlements>,
//     <LogicPlaceInitialRoad>,
//     <LogicPlaceInitialRoadNoOceans>,
//     <LogicPlaceInitialTile>,
//     <LogicPlaceInitialTileLand>,
//     <LogicRequestInitialPlacement>,
//     <LogicRequestInitialRoad>,
//     <LogicPlaceFirstRoad>,
//     <LogicPlaceSecondRoad>
//
// Build Logic:
//     <LogicCanBuildCity>,
//     <LogicCanBuildRoad>,
//     <LogicCanBuildSettlement>
//
// Initial Placement Rule Chain:
//     RuleMarkerStartInitial - -> <RuleStartInitialPlacements>
//
//     RuleStartInitialPlacements - -> <RuleRequestInitialPlacement>
//
//     RulePlaceInitialPlacement - -> <RuleMarkerInitialRoad>
//
//     RuleMarkerInitialRoad - -> <RuleRequestInitialRoad>
//
//     RuleMarkerMiddleInitial - -> <RuleContinueInitialPlacements>
//
//     RuleContinueInitialPlacements - -> <RuleRequestInitialPlacement>

