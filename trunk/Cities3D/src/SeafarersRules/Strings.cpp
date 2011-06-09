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

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
    // Force all strings to be statically initialized first.
#ifdef __WIN32__
    #pragma warning(disable:4073)
    #pragma init_seg(lib)
    #pragma warning(default:4073)
#endif

	// All strings used in this project.

	//-----------------------------------------------------------------------//
	// Section: HashStrings
	//
    const HashString shChipObject = "ChipObject";
    const HashString shDummySelectionObject = "DummySelectionObject";
	const HashString shEmpty = "";
	const HashString shExtraObject = "ExtraObject";
    const HashString shLogicStringFormat = "Logic%s";
	const HashString shMoveShip = "MoveShip";
    const HashString shPirateObject = "PirateObject";
    const HashString shPortObject = "PortObject";
    const HashString shRuleStringFormat = "Rule%s";
    const HashString shShipObject = "ShipObject";
    const HashString shSideSelectionObject = "SideSelectionObject";
    const HashString shTileObject = "TileObject";

	// Data
    const HashString shAlwaysShow = "AlwaysShow";
    const HashString shAutoTradeRule = "AutoTradeRule";
    const HashString shBorder = "Border";
    const HashString shBonus = "Bonus";
    const HashString shChipless = "Chipless";
    const HashString shChiplessChipArray = "ChiplessChipArray";
    const HashString shChiplessChipIndex = "ChiplessChipIndex";
    const HashString shChiplessChips = "ChiplessChips";
    const HashString shChiplessResources = "ChiplessResources";
    const HashString shChiplessTile = "ChiplessTile";
    const HashString shChiplessTiles = "ChiplessTiles";
    const HashString shCities = "Cities";
    const HashString shCurrentPlayer = "CurrentPlayer";
    const HashString shExtra = "Extra";
    const HashString shFreeRoads = "FreeRoads";
    const HashString shHidden = "Hidden";
    const HashString shHiddenArray = "HiddenArray";
    const HashString shHiddenChipArray = "HiddenChipArray";
    const HashString shHiddenChipIndex = "HiddenChipIndex";
    const HashString shHiddenChips = "HiddenChips";
    const HashString shHiddenIndex = "HiddenIndex";
    const HashString shHiddenResources = "HiddenResources";
    const HashString shHiddenTiles = "HiddenTiles";
    const HashString shHomeIsland = "HomeIsland";
    const HashString shInitial = "Initial";
    const HashString shInitialEvaluate = "InitialEvaluate";
    const HashString shInitialEvaluateState = "InitialEvaluateState";
    const HashString shInitialPortCount = "InitialPortCount";
    const HashString shInitialPortOrientation = "InitialPortOrientation";
    const HashString shInitialPortPlacement = "InitialPortPlacement";
    const HashString shInitialPortTile = "InitialPortTile";
    const HashString shInitialShip = "InitialShip";
    const HashString shIsland = "Island";
    const HashString shOcean = "Ocean";
    const HashString shMovedShip = "MovedShip";
    const HashString shPirate = "Pirate";
    const HashString shPirateSteal = "PirateSteal";
    const HashString shPort1 = "Port1";
    const HashString shPort2 = "Port2";
    const HashString shPortRandom = "PortRandom";
    const HashString shRandomPorts = "RandomPorts";
    const HashString shReceivedBonus = "ReceivedBonus";
	const HashString shResources = "Resources";
    const HashString shRoad = "Road";
    const HashString shRoadBuiltFree = "RoadBuiltFree";
    const HashString shRobber = "Robber";
    const HashString shRoll = "Roll";
    const HashString shSecondary = "Secondary";
    const HashString shSettlements = "Settlements";
    const HashString shSheep = "Sheep";
    const HashString shShip = "Ship";
    const HashString shShipMoveSide = "ShipMoveSide";
    const HashString shShipMoveTile = "ShipMoveTile";
    const HashString shShipMoveTurn = "ShipMoveTurn";
    const HashString shShips = "Ships";
    const HashString shSides = "Sides";
    const HashString shState = "State";
    const HashString shStockShips = "StockShips";
    const HashString shTileType = "TileType";
	const HashString shTimber = "Timber";
    const HashString shTradePoints = "TradePoints";
    const HashString shTurn = "Turn";
    const HashString shTurnShipMove = "TurnShipMove";
    const HashString shTurnShipMovePlacement = "TurnShipMovePlacement";
    const HashString shTurnShipPlacement = "TurnShipPlacement";

	// Display
	const HashString shBackground = "Background";
	const HashString shDefaultFont = "DefaultFont";
	const HashString shDefaultText = "DefaultText";
    const HashString shPortType = "PortType";

	// Events
    const HashString shEventBuildUI = "eventBuildUI";
    const HashString shEventControlsUI = "eventControlsUI";
    const HashString shEventDirty = "eventDirty";
    const HashString shEventGameObjects = "eventGameObjects";
    const HashString shEventPlayerReorder = "eventPlayerReorder";
    const HashString shEventPlayerUI = "eventPlayerUI";
    const HashString shEventSelectionObject = "eventSelectionObject";
    const HashString shEventSkin = "eventSkin";
    const HashString shEventUpdateOptions = "eventUpdateOption";

	// Logic
    const HashString shLogicCanMoveShip = "LogicCanMoveShip";
    const HashString shLogicCanPlaceShip = "LogicCanPlaceShip";
    const HashString shLogicCanPurchase = "LogicCanPurchase";
    const HashString shLogicCanPurchaseShip = "LogicCanPurchaseShip";
    const HashString shLogicCostShip = "LogicCostShip";
    const HashString shLogicIsCornerSettleable = "LogicIsCornerSettleable";
    const HashString shLogicIsValidShipPlacement = "LogicIsValidShipPlacement";
    const HashString shLogicIsValidSidePlacement = "LogicIsValidSidePlacement";
    const HashString shLogicPirateStealPlayers = "LogicPirateStealPlayers";
    const HashString shLogicPlaceInitialRoad = "LogicPlaceInitialRoad";
    const HashString shLogicPlaceInitialShip = "LogicPlaceInitialShip";
    const HashString shLogicPlayerName = "LogicPlayerName";
    const HashString shLogicResourceName = "LogicResourceName";
    const HashString shLogicTileIsLand = "LogicTileIsLand";
    const HashString shLogicTileIsOcean = "LogicTileIsOcean";
    const HashString shLogicTileIsPort = "LogicTileIsPort";
    const HashString shLogicTileIsResource = "LogicTileIsResource";

	// Rules
    const HashString shRuleAdjustExtras = "RuleAdjustExtras";
    const HashString shRuleAdjustPoints = "RuleAdjustPoints";
    const HashString shRuleAnimatePlacement = "RuleAnimatePlacement";
    const HashString shRuleBankTransact = "RuleBankTransact";
	const HashString shRuleBeginBlockingAction = "RuleBeginBlockingAction";
    const HashString shRuleBreakUndo = "RuleBreakUndo";
    const HashString shRuleBuildShipCommon = "RuleBuildShipCommon";
    const HashString shRuleBuildShipCommonPlaces = "RuleBuildShipCommonPlaces";
    const HashString shRuleBuildShipTurn = "RuleBuildShipTurn";
	const HashString shRuleEndBlockingAction = "RuleEndBlockingAction";
    const HashString shRuleEvaluateInitialRoads = "RuleEvaluateInitialRoads";
    const HashString shRuleExaminePortPlacement = "RuleExaminePortPlacement";
    const HashString shRuleExecuteAutoTrade = "RuleExecuteAutoTrade";
    const HashString shRuleExecuteChiplessTile = "RuleExecuteChiplessTile";
    const HashString shRuleExecutePiratePlacement = "RuleExecutePiratePlacement";
    const HashString shRuleExecuteRobberPiratePlacement = "RuleExecuteRobberPiratePlacement";
    const HashString shRuleExecuteRobberPlacement = "RuleExecuteRobberPlacement";
    const HashString shRuleExecuteRobberSteal = "RuleExecuteRobberSteal";
    const HashString shRuleIncrementPlayer = "RuleIncrementPlayer";
    const HashString shRuleIncrementTurnsPlayed = "RuleIncrementTurnsPlayed";
    const HashString shRuleLocateInitialRoad = "RuleLocateInitialRoad";
    const HashString shRuleLongestRoadCheck = "RuleLongestRoadCheck";
    const HashString shRuleMoveShip = "RuleMoveShip";
	const HashString shRulePerformGameAction = "RulePerformGameAction";
    const HashString shRulePirateSteal = "RulePirateSteal";
    const HashString shRulePlacePortPlacement = "RulePlacePortPlacement";
    const HashString shRulePlacePirate = "RulePlacePirate";
    const HashString shRulePlaceShip = "RulePlaceShip";
    const HashString shRulePlaceShipCommon = "RulePlaceShipCommon";
    const HashString shRulePlaceShipMove = "RulePlaceShipMove";
    const HashString shRulePlaceShipTurn = "RulePlaceShipTurn";
    const HashString shRulePlaySound = "RulePlaySound";
    const HashString shRulePurchase = "RulePurchase";
	const HashString shRulePurchaseItem = "RulePurchaseItem";
    const HashString shRuleRandomizePlayers = "RuleRandomizePlayers";
    const HashString shRuleReplaceExtra = "RuleReplaceExtra";
    const HashString shRuleReplaceExtras = "RuleReplaceExtras";
    const HashString shRuleRequestInitialRoad = "RuleRequestInitialRoad";
    const HashString shRuleRequestPortOrientation = "RuleRequestPortOrientation";
    const HashString shRuleRequestPortPlacement = "RuleRequestPortPlacement";
    const HashString shRuleResetObjectFlags = "RuleResetObjectFlags";
    const HashString shRuleResetTileFlags = "RuleResetTileFlags";
    const HashString shRuleRestartOldState = "RuleRestartOldState";
    const HashString shRuleRestartPirateSteal = "RuleRestartPirateSteal";
    const HashString shRuleRestartTurn = "RuleRestartTurn";
    const HashString shRuleRestartTurnShipMovePlacement = "RuleRestartTurnShipMovePlacement";
    const HashString shRuleRewardChiplessTile = "RuleRewardChiplessTile";
    const HashString shRuleRewardHiddenTile = "RuleRewardHiddenTile";
    const HashString shRuleSelectChiplessTile = "RuleSelectChiplessTile";
    const HashString shRuleSelectPirateSteal = "RuleSelectPirateSteal";
    const HashString shRuleSelectShipMove = "RuleSelectShipMove";
    const HashString shRuleSetPortPlacements = "RuleSetPortPlacements";
    const HashString shRuleSilentUpdateOption = "RuleSilentUpdateOption";
	const HashString shRuleTakeAction = "RuleTakeAction";
    const HashString shRuleUpdateMessageUI = "RuleUpdateMessageUI";
    const HashString shRuleUpdateNetworkUI = "RuleUpdateNetworkUI";

	//-----------------------------------------------------------------------//
	// Section: wxStrings
	//
    const wxString sw5 = wxT("5");
    const wxString sw6 = wxT("6");
    const wxString sw7 = wxT("7");
    const wxString sw8 = wxT("8");
    const wxString sw9 = wxT("9");
    const wxString sw10 = wxT("10");
    const wxString sw11 = wxT("11");
    const wxString sw12 = wxT("12");
    const wxString sw13 = wxT("13");
    const wxString sw14 = wxT("14");
    const wxString sw15 = wxT("15");
    const wxString sw16 = wxT("16");
    const wxString sw17 = wxT("17");
    const wxString sw18 = wxT("18");
    const wxString sw19 = wxT("19");
    const wxString sw20 = wxT("20");
    const wxString swAlwaysShowBuild = wxT("AlwaysShowBuild");
    const wxString swColon = wxT(":");
    const wxString swConfigSectionGeneral = wxT("ConfigSectionGeneral");
    const wxString swConfigSectionOpenGL = wxT("ConfigSectionOpenGL");
    const wxString swConfigSectionUI = wxT("ConfigSectionUI");
	const wxString swEmpty = wxT("");
    const wxString swGlExtraTex = wxT("glextra.tex");
    const wxString swGlShipTex = wxT("glship.tex");
    const wxString swMipmap = wxT("Mipmap");
    const wxString swSeafarers = wxT("Seafarers");
    const wxString swSeafarersPorts = wxT("SeafarersPorts");
    const wxString swSeafarersRules = wxT("SeafarersRules");
    const wxString swShips = wxT("Ships");
    const wxString swValue = wxT("Value");

	// Formats
    const wxString swFloat6 = wxT("%.6f");
    const wxString swIntFormat = wxT("%d");
    const wxString swSlashStringSlashString = wxT("/%s/%s");
    const wxString swStringFormat = wxT("%s");

	//-----------------------------------------------------------------------//
	// Section: Translations
	//
    const wxString stBuildAShip = _("Build a ship");
    const wxString stChangeTheStartingStockShips = _("Change the starting stock amount of ships.");
    const wxString stChiplessTiles = _("Chipless tiles");
    const wxString stExtraPoint = _("Extra Point");
    const wxString stHiddenTiles = _("Hidden tiles");
    const wxString stMoveAShip = _("Move a ship");
    const wxString stNoInitialRoadsOnCoast = _("No initial roads along the coast");
    const wxString stOneThenTwoVPs = _("1, then 2 VPs for colonies");
    const wxString stOneVP = _("1 VP for colonies");
    const wxString stPirate = _("Pirate");
    const wxString stPlayersPlacePorts = _("Players place ports");
    const wxString stRandom = _("Random");
    const wxString stSeafarers = _("Seafarers");
    const wxString stShip = _("Ship");
    const wxString stShips = _("Ships");
    const wxString stStockShips = _("Stock ships");
    const wxString stTradeRoutes = _("Trade routes");

	// Formats
    const wxString stChooseAPlayerToStealFrom = _("%s, choose a player to steal from, or click the pirate to not steal.");
    const wxString stGainsOneTradeRoutePoint = _("%s gains 1 trade route point.");
    const wxString stGainsXTradeRoutePoints = _("%s gains %d trade route points.");
    const wxString stIntShips = _("%d ships");
    const wxString stLosesOneTradeRoutePoint = _("%s loses 1 trade route point.");
    const wxString stLosesXTradeRoutePoints = _("%s loses %d trade route points.");
    const wxString stMovesAShip = _("%s moves a ship.");
    const wxString stMoveYourShip = _("%s, move your ship.");
    const wxString stOrientThePortHarbor = _("%s, orient the port harbor.");
    const wxString stPlaceAPortHarbor = _("%s, place a port harbor.");
    const wxString stPlaceARoadOrShip = _("%s, place a road or a ship.");
    const wxString stPlaceAShip = _("%s, place a ship.");
    const wxString stPlacesAPortHarbor = _("%s places a port harbor.");
    const wxString stPlacesAShip = _("%s places a ship.");
    const wxString stPlaceThePirate = _("%s, place the pirate.");
    const wxString stPlaceTheRobberOrPirate = _("%s, place the robber or pirate.");
    const wxString stReceivesOneForColonizing = _("%s receives 1 bonus point for colonizing a new island.");
    const wxString stReceivesXForColonizing = _("%s receives %d bonus points for colonizing a new island.");
    const wxString stReceivesOneForDiscovering = _("%s receives 1 %s for discovering new land.");
    const wxString stSelectAChipFromTheMainland = _("%s, select a chip from the mainland.");
    const wxString stSelectAShipToMove = _("%s, select a ship to move.");
    const wxString stStealsNothing = _("%s steals nothing.");
    const wxString stTakesFromTheMainland = _("%s takes a %d from the mainland.");
    const wxString stUndoesAShip = _("%s undoes a ship.");
    const wxString stUndoesAShipMove = _("%s undoes a ship move.");
    const wxString stWaitingOrientThePortHarbor = _("Waiting for %s to orient the port harbor.");
    const wxString stWaitingPlaceAPortHarbor = _("Waiting for %s to place a port harbor.");
    const wxString stWaitingPlaceARoadOrShip = _("Waiting for %s to place a road or a ship.");
    const wxString stWaitingPlaceAShip = _("Waiting for %s to place a ship.");
    const wxString stWaitingPlaceThePirate = _("Waiting for %s to place the pirate.");
    const wxString stWaitingPlaceTheRobberOrPirate = _("Waiting for %s to place the robber or pirate.");
    const wxString stWaitingSelectAChipFromTheMainland = _("Waiting for %s to select a chip from the mainland.");
    const wxString stWaitingSelectAShipToMove = _("Waiting for %s to select a ship to move.");
    const wxString stWaitingToMoveAShip =  _("Waiting for %s to move a ship.");
    const wxString stWaitingToSteal = _("Waiting for %s to steal a card.");
    
    //-----------------------------------------------------------------------//
	// Section: Sounds
	//
    const wxString SOUND_PLACE_SHIP = wxT("PlaceShip.mp3");
    const wxString SOUND_MOVE_SHIP = wxT("MoveShip.mp3");
    const wxString SOUND_EXTRA_POINTS = wxT("EarnExtraPoints.mp3");
    const wxString SOUND_PLACE_PIRATE = wxT("PlacePirate.mp3");

    //-----------------------------------------------------------------------//
	// Section: Options
	//
    const wxString STOCK_AMOUNTS = wxT("StockAmounts");

    //-----------------------------------------------------------------------//
	// Section: Rulesets
	//
    const wxString CHIPLESS_TILES = wxT("ChiplessTiles");
	const wxString HIDDEN_TILES = wxT("HiddenTiles");
	const wxString INITIAL_SHIPS = wxT("InitialShips");
	const wxString PORT_PLACEMENT = wxT("PortPlacement");
	const wxString SEAFARERS = wxT("Seafarers");
	const wxString SINGLE_BONUS = wxT("SingleBonus");
	const wxString REPEATING_BONUS = wxT("RepeatingBonus");
    const wxString TRADE_ROUTES = wxT("TradeRoutes");
	
}

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
