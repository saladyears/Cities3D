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


#include "BuildData.h"
#include "RuleSetDatabase.h"
#include "DrawKnightObject.h"
#include "DrawWallsObject.h"
#include "DrawMetropolisObject.h"
#include "KnightObject.h"
#include "WallsObject.h"
#include "MetropolisObject.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

// Walls
IMPLEMENT_BUILD_OPENGL_DATA(CITIES, 0, 40, LogicCanBuildWalls, 
							RuleBuildWalls, WallsObject, DrawWallsObject, 
							walls128, no_mask, Trade, stBuildACityWalls, 
							LogicCostWalls, BuildCityWalls);

// City Upgrades
IMPLEMENT_BUILD_IMAGE_DATA(CITIES, 1, 10, LogicCanUpgradeScience, 
						   RuleUpgradeScience, Build, Science, Trade,
						  stUpgradeScience, LogicCostUpgradeScience,
						  UpgradeScience);

IMPLEMENT_BUILD_IMAGE_DATA(CITIES, 1, 20, LogicCanUpgradePolitics, 
						   RuleUpgradePolitics, Build, Politics, Trade,
						  stUpgradePolitics, LogicCostUpgradePolitics,
						  UpgradePolitics);

IMPLEMENT_BUILD_IMAGE_DATA(CITIES, 1, 30, LogicCanUpgradeTrade, 
						   RuleUpgradeTrade, Build, Trade, Trade,
						  stUpgradeTrade, LogicCostUpgradeTrade,
						  UpgradeTrade);

// Knights
IMPLEMENT_BUILD_OPENGL_DATA(CITIES, 0, 50, LogicCanBuildKnight, 
							RuleBuildKnight, KnightObject, DrawKnightObject, 
							knight1128, knight128_mask, Trade, 
							stBuildABasicKnight, LogicCostKnight, BuildBasicKnight);

IMPLEMENT_BUILD_OPENGL_DATA(CITIES, 0, 60, LogicCanUpgrade2Knight,
							RuleUpgrade2Knight, Knight2Object, 
							DrawKnightObject, knight2128, knight128_mask,
							Trade, stBuildAStrongKnight,
							LogicCostKnightUpgrade, BuildStrongKnight);

IMPLEMENT_BUILD_OPENGL_DATA(CITIES, 0, 70, LogicCanUpgrade3Knight,
							RuleUpgrade3Knight, Knight3Object, 
							DrawKnightObject, knight3128, knight128_mask,
							Trade, stBuildAMightyKnight,
							LogicCostKnightUpgrade, BuildMightyKnight);

IMPLEMENT_BUILD_OPENGL_DATA(CITIES, 1, 40, LogicCanActivateKnight, 
							RuleActivateKnight, ActiveKnightObject, 
							DrawKnightObject, knight1128a, knight128_mask, 
							Trade, stActivateAKnightUI, 
							LogicCostActivateKnight, ActivateKnight);

IMPLEMENT_BUILD_OPENGL_DATA(CITIES, 1, 50, LogicCanMoveKnight, 
							RuleMoveKnight, ActiveKnightObject, 
							DrawKnightObject, knight1128, knight128_mask, 
							Move, stMoveAKnight, 
							LogicCostNothing, MoveKnight);

IMPLEMENT_BUILD_OPENGL_DATA(CITIES, 1, 60, LogicCanAttackKnight, 
							RuleAttackKnight, ActiveKnightObject, 
							DrawKnightObject, knight1128, knight128_mask, 
							Attack, stAttackAnotherKnight, 
							LogicCostNothing, AttackKnight);

IMPLEMENT_BUILD_OPENGL_DATA(CITIES, 1, 70, LogicCanExpelKnight, 
							RuleExpelKnight, ActiveKnightObject, 
							DrawKnightObject, knight1128, knight128_mask, 
							Expel, stExpelCriminals, 
							LogicCostNothing, ExpelCriminals);

// Progress cards.
IMPLEMENT_BUILD_IMAGE_DATA(CITIES, 1, 80, LogicCanCommercialHarbor, 
						   RuleCommercialHarbor, Build, CommercialHarbor, 
						   Trade, stUseCommercialHarbor, 
						   LogicCostNothing, UseHarbor);

IMPLEMENT_BUILD_IMAGE_DATA(CITIES, 0, 80, LogicCanRepairCity, 
						   RuleRepairCity, Build, Saboteur, 
						   Trade, stRepairACityUI, 
						   LogicCostRepairCity, RepairCity);

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
