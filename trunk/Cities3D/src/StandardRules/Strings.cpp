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
	const HashString shAction = "Action";
	const HashString shChipObject = "ChipObject";
    const HashString shCityObject = "CityObject";
    const HashString shCornerSelectionObject = "CornerSelectionObject";
    const HashString shDummySelectionObject = "DummySelectionObject";
	const HashString shEmpty = "";
    const HashString shIntFormat = "%d";
    const HashString shLogicString = "Logic%s";
	const HashString shPurchased = "Purchased";
    const HashString shRoadObject = "RoadObject";
    const HashString shRobberObject = "RobberObject";
    const HashString shRuleString = "Rule%s";
    const HashString shSideSelectionObject = "SideSelectionObject";
    const HashString shSettlementObject = "SettlementObject";
    const HashString shTileObject = "TileObject";

	// Data
    const HashString shAllowedCards = "AllowedCards";
    const HashString shAnyMatch = "AnyMatch";
    const HashString shAutoTradeComplete = "AutoTradeComplete";
    const HashString shAutoTradeLogic = "AutoTradeLogic";
    const HashString shAutoTradeRule = "AutoTradeRule";
    const HashString shBankDevCards = "BankDevCards";
    const HashString shBankResources = "BankResources";
    const HashString shBorder = "Border";
    const HashString shCities = "Cities";
	const HashString shCity = "City";
    const HashString shCityBuiltFree = "CityBuiltFree";
    const HashString shClay = "Clay";
    const HashString shComplete = "Complete";
    const HashString shCurrentPlayer = "CurrentPlayer";
    const HashString shDesert = "Desert";
    const HashString shDev = "Dev";
    const HashString shDevCard = "DevCard";
    const HashString shDevCards = "DevCards";
	const HashString shDie1 = "Die1";
    const HashString shDie2 = "Die2";
    const HashString shFirstRoad = "FirstRoad";
    const HashString shFirstSettlement = "FirstSettlement";
    const HashString shFreeCities = "FreeCities";
    const HashString shFreeRoads = "FreeRoads";
    const HashString shGold = "Gold";
    const HashString shInitial = "Initial";
    const HashString shLandTiles = "LandTiles";
    const HashString shLargestArmyPlayer = "LargestArmyPlayer";
    const HashString shLongestRoadPlayer = "LongestRoadPlayer";
    const HashString shLoseCards = "LoseCards";
    const HashString shMinPlayers = "MinPlayers";
    const HashString shMode = "Mode";
    const HashString shMonopoly = "Monopoly";
    const HashString shNo7Rounds = "No7Rounds";
    const HashString shOcean = "Ocean";
    const HashString shOldState = "OldState";
    const HashString shOre = "Ore";
    const HashString shOriginalBankResources = "OriginalBankResources";
    const HashString shOriginalDevCards = "OriginalDevCards";
    const HashString shPlaceInitialRoad = "PlaceInitialRoad";
    const HashString shPlayableCards = "PlayableCards";
    const HashString shPlayedCard = "PlayedCard";
    const HashString shPlayedCards = "PlayedCards";
    const HashString shPoints = "Points";
    const HashString shPort1 = "Port1";
    const HashString shPort2 = "Port2";
    const HashString shPort3 = "Port3";
    const HashString shPortClay = "PortClay";
    const HashString shPortOre = "PortOre";
    const HashString shPorts = "Ports";
    const HashString shPortSheep = "PortSheep";
    const HashString shPortString = "Port%s";
    const HashString shPortTimber = "PortTimber";
    const HashString shPortWheat = "PortWheat";
    const HashString shPostDiceRoll = "PostDiceRoll";
    const HashString shPreRoll = "PreRoll";
    const HashString shPurchasedCards = "PurchasedCards";
    const HashString shRandom = "Random";
    const HashString shRandomChipArray = "RandomChipArray";
    const HashString shRandomChips = "RandomChips";
    const HashString shRandomPorts = "RandomPorts";
    const HashString shRandomResources = "RandomResources";
    const HashString shRandomTiles = "RandomTiles";
	const HashString shResources = "Resources";
    const HashString shResourcesGained = "ResourcesGained";
	const HashString shRoad = "Road";
    const HashString shRoads = "Roads";
    const HashString shRoadBuilding = "RoadBuilding";
    const HashString shRoadBuiltFree = "RoadBuiltFree";
    const HashString shRoadLength = "RoadLength";
    const HashString shRobber = "Robber";
    const HashString shRobberSteal = "RobberSteal";
    const HashString shRoll = "Roll";
    const HashString shRollPlaces = "RollPlaces";
    const HashString shSecondary = "Secondary";
    const HashString shSecondRoad = "SecondRoad";
    const HashString shSecondSettlement = "SecondSettlement";
	const HashString shSettlement = "Settlement";
    const HashString shSettlements = "Settlements";
    const HashString shSheep = "Sheep";
    const HashString shSides = "Sides";
    const HashString shSoldier = "Soldier";
    const HashString shState = "State";
    const HashString shStockCities = "StockCities";
    const HashString shStockRoads = "StockRoads";
    const HashString shStockSettlements = "StockSettlements";
    const HashString shTileType = "TileType";
	const HashString shTimber = "Timber";
    const HashString shTurn = "Turn";
    const HashString shTurnCityPlacement = "TurnCityPlacement";
    const HashString shTurnLose7 = "TurnLose7";
    const HashString shTurnMonopoly = "TurnMonopoly";
    const HashString shTurnRoadPlacement = "TurnRoadPlacement";
    const HashString shTurnSettlementPlacement = "TurnSettlementPlacement";
    const HashString shTurnYearOfPlenty = "TurnYearOfPlenty";
    const HashString shWheat = "Wheat";
    const HashString shVictoryPoint = "VictoryPoint";
    const HashString shVictoryPoints = "VictoryPoints";
    const HashString shYearOfPlenty = "YearOfPlenty";

    // TODO: Move into Jungles DLL
    const HashString shJungle = "Jungle";
    const HashString shJungles = "Jungles";
    
	// Display
	const HashString shBackground = "Background";
	const HashString shDefaultFont = "DefaultFont";
	const HashString shDefaultText = "DefaultText";
    const HashString shPlayerUINegative = "PlayerUINegative";
    const HashString shPlayerUINeutral = "PlayerUINeutral";
    const HashString shPlayerUIPositive = "PlayerUIPositive";

	// Events
    const HashString shEventAcceptOffer = "eventAcceptOffer";
    const HashString shEventAddSidePane = "eventAddSidePane";
    const HashString shEventBankTrade = "eventBankTrade";
    const HashString shEventBuildUI = "eventBuildUI";
    const HashString shEventCards = "eventCards";
    const HashString shEventClearOffer = "eventClearOffer";
    const HashString shEventClickOffer = "eventClickOffer";
    const HashString shEventControlsUI = "eventControlsUI";
    const HashString shEventCurrentOffer = "eventCurrentOffer";
    const HashString shEventDiceRoll = "eventDiceRoll";
    const HashString shEventDirty = "eventDirty";
    const HashString shEventGameObjects = "eventGameObjects";
    const HashString shEventGold = "eventGold";
    const HashString shEventLose7 = "eventLose7";
    const HashString shEventMonopoly = "eventMonopoly";
    const HashString shEventMessageUI = "eventMessageUI";
    const HashString shEventNewSidePane = "eventNewSidePane";
    const HashString shEventOthersTrade = "eventOthersTrade";
    const HashString shEventPlayerOffer = "eventPlayerOffer";
    const HashString shEventPlayerResources = "eventPlayerResources";
    const HashString shEventPlayerUI = "eventPlayerUI";
    const HashString shEventPreRollCard = "eventPreRollCard";
    const HashString shEventRejectOffer = "eventRejectOffer";
    const HashString shEventSelectionObject = "eventSelectionObject";
    const HashString shEventShowResources = "eventShowResources";
	const HashString shEventSkin = "eventSkin";
    const HashString shEventShutdownModelessUI = "eventShutdownModelessUI";
    const HashString shEventShutdownUI = "eventShutdownUI";
    const HashString shEventTrade = "eventTrade";
    const HashString shEventUpdateOption = "eventUpdateOption";
    const HashString shEventWontTrade = "eventWontTrade";
    const HashString shEventYearOfPlenty = "eventYearOfPlenty";

	// Logic
    const HashString shLogicCanAutoTrade = "LogicCanAutoTrade";
    const HashString shLogicCanBankTransact = "LogicCanBankTransact";
    const HashString shLogicCanPlaceRoad = "LogicCanPlaceRoad";
    const HashString shLogicCanPlaceRobber = "LogicCanPlaceRobber";
    const HashString shLogicCanPlaceSettlement = "LogicCanPlaceSettlement";
    const HashString shLogicCanPurchase = "LogicCanPurchase";
    const HashString shLogicCanPurchaseCity = "LogicCanPurchaseCity";
    const HashString shLogicCanPurchaseDevCard = "LogicCanPurchaseDevCard";
    const HashString shLogicCanPurchaseRoad = "LogicCanPurchaseRoad";
    const HashString shLogicCanPurchaseSettlement = "LogicCanPurchaseSettlement";
    const HashString shLogicCanTradeBank = "LogicCanTradeBank";
    const HashString shLogicCardName = "LogicCardName";
    const HashString shLogicCostCity = "LogicCostCity";
    const HashString shLogicCostDevCard = "LogicCostDevCard";
    const HashString shLogicCostRoad = "LogicCostRoad";
    const HashString shLogicCostSettlement = "LogicCostSettlement";
    const HashString shLogicGatherCity = "LogicGatherCity";
    const HashString shLogicGatherSettlement = "LogicGatherSettlement";
    const HashString shLogicHasPreRollAction = "LogicHasPreRollAction";
    const HashString shLogicHasPreRollCard = "LogicHasPreRollCard";
    const HashString shLogicIsCardPlayable = "LogicIsCardPlayable";
    const HashString shLogicIsCornerBlocked = "LogicIsCornerBlocked";
    const HashString shLogicIsCornerReachable = "LogicIsCornerReachable";
    const HashString shLogicIsCornerSettleable = "LogicIsCornerSettleable";
    const HashString shLogicIsInitialResource = "LogicIsInitialResource";
    const HashString shLogicIsResourceBankTradeable = "LogicIsResourceBankTradeable";
    const HashString shLogicIsResourceGoldable = "LogicIsResourceGoldable";
    const HashString shLogicIsThisPlayer = "LogicIsThisPlayer";
    const HashString shLogicIsValidDiceRoll = "LogicIsValidDiceRoll";
    const HashString shLogicIsValidRoadPlacement = "LogicIsValidRoadPlacement";
    const HashString shLogicIsValidSettlementPlacement = "LogicIsValidSettlementPlacement";
    const HashString shLogicIsValidSidePlacement = "LogicIsValidSidePlacement";
    const HashString shLogicPlaceInitialCorner = "LogicPlaceInitialCorner";
    const HashString shLogicPlaceInitialTile = "LogicPlaceInitialTile";
    const HashString shLogicPlayerName = "LogicPlayerName";
    const HashString shLogicRequestInitialPlacement = "LogicRequestInitialPlacement";
    const HashString shLogicRequestInitialRoad = "LogicRequestInitialRoad";
    const HashString shLogicResourceName = "LogicResourceName";
    const HashString shLogicResourceType = "LogicResourceType";
    const HashString shLogicRoadLength = "LogicRoadLength";
    const HashString shLogicRobberStealPlayers = "LogicRobberStealPlayers";
    const HashString shLogicTileIsLand = "LogicTileIsLand";
    const HashString shLogicTileIsOcean = "LogicTileIsOcean";
    const HashString shLogicTileIsPort = "LogicTileIsPort";
    const HashString shLogicTileIsResource = "LogicTileIsResource";
    const HashString shLogicWin = "LogicWin";

	// Rules
    const HashString shRuleAdjustPoints = "RuleAdjustPoints";
    const HashString shRuleAdjustResources = "RuleAdjustResources";
    const HashString shRuleAnimatePlacement = "RuleAnimatePlacement";
    const HashString shRuleAutoSave = "RuleAutoSave";
    const HashString shRuleBankTransact = "RuleBankTransact";
    const HashString shRuleBeginBlockingAction = "RuleBeginBlockingAction";
    const HashString shRuleBeginTurn = "RuleBeginTurn";
	const HashString shRuleBlockBankResources = "RuleBlockBankResources";
    const HashString shRuleBlockResources = "RuleBlockResources";
    const HashString shRuleBreakUndo = "RuleBreakUndo";
    const HashString shRuleBuildCityTurn = "RuleBuildCityTurn";
    const HashString shRuleBuildRoadCommon = "RuleBuildRoadCommon";
    const HashString shRuleBuildRoadTurn = "RuleBuildRoadTurn";
    const HashString shRuleBuildSettlementTurn = "RuleBuildSettlementTurn";
    const HashString shRuleBuyDevCardTurn = "RuleBuyDevCardTurn";
    const HashString shRuleCurrentOffer = "RuleCurrentOffer";
    const HashString shRuleDecreaseInitialPlayer = "RuleDecreaseInitialPlayer";
    const HashString shRuleDecrementPlayer = "RuleDecrementPlayer";
    const HashString shRuleEndBlockingAction = "RuleEndBlockingAction";
    const HashString shRuleExecuteAutoTrade = "RuleExecuteAutoTrade";
    const HashString shRuleExecuteBankTrade = "RuleExecuteBankTrade";
    const HashString shRuleExecuteGold = "RuleExecuteGold";
    const HashString shRuleExecuteLose7 = "RuleExecuteLose7";
    const HashString shRuleExecuteMonopolyCard = "RuleExecuteMonopolyCard";
    const HashString shRuleExecuteRobberPlacement = "RuleExecuteRobberPlacement";
    const HashString shRuleExecuteRobberSteal = "RuleExecuteRobberSteal";
    const HashString shRuleExecuteYearOfPlentyCard = "RuleExecuteYearOfPlentyCard";
    const HashString shRuleGainLargestArmy = "RuleGainLargestArmy";
    const HashString shRuleGainLongestRoad = "RuleGainLongestRoad";
    const HashString shRuleGatherResources = "RuleGatherResources";
    const HashString shRuleGenerateSystemMessage = "RuleGenerateSystemMessage";
    const HashString shRuleGold = "RuleGold";
    const HashString shRuleHandle7 = "RuleHandle7";
    const HashString shRuleIncreaseInitialPlayer = "RuleIncreaseInitialPlayer";
    const HashString shRuleIncrementPlayer = "RuleIncrementPlayer";
    const HashString shRuleIncrementTurnsPlayed = "RuleIncrementTurnsPlayed";
    const HashString shRuleInitialResources = "RuleInitialResources";
    const HashString shRuleLargestArmyCheck = "RuleLargestArmyCheck";
    const HashString shRuleLocateInitialRoad = "RuleLocateInitialRoad";
    const HashString shRuleLongestRoadCheck = "RuleLongestRoadCheck";
    const HashString shRuleLoseLargestArmy = "RuleLoseLargestArmy";
    const HashString shRuleLoseLongestRoad = "RuleLoseLongestRoad";
    const HashString shRuleMarkerEndInitial = "RuleMarkerEndInitial";
    const HashString shRuleMarkerMiddleInitial = "RuleMarkerMiddleInitial";
    const HashString shRuleMarkerPostDiceRoll = "RuleMarkerPostDiceRoll";
    const HashString shRuleNothing = "RuleNothing";
    const HashString shRulePerformGameAction = "RulePerformGameAction";
    const HashString shRulePlaceCity = "RulePlaceCity";
    const HashString shRulePlaceCityTurn = "RulePlaceCityTurn";
    const HashString shRulePlaceInitialExtensions = "RulePlaceInitialExtensions";
    const HashString shRulePlaceInitialPlacement = "RulePlaceInitialPlacement";
    const HashString shRulePlaceRoad = "RulePlaceRoad";
    const HashString shRulePlaceRoadTurn = "RulePlaceRoadTurn";
    const HashString shRulePlaceRobber = "RulePlaceRobber";
    const HashString shRulePlaceSettlement = "RulePlaceSettlement";
    const HashString shRulePlaceSettlementCommon = "RulePlaceSettlementCommon";
    const HashString shRulePlaceSettlementReduced = "RulePlaceSettlementReduced";
    const HashString shRulePlaceSettlementTurn = "RulePlaceSettlementTurn";
	const HashString shRulePlayCard = "RulePlayCard";
    const HashString shRulePlayCardDevCards = "RulePlayCardDevCards";
    const HashString shRulePlaySettlementSound = "RulePlaySettlementSound";
    const HashString shRulePlaySound = "RulePlaySound";
    const HashString shRulePurchase = "RulePurchase";
	const HashString shRulePurchaseItem = "RulePurchaseItem";
    const HashString shRuleReduceCityCommon = "RuleReduceCityCommon";
    const HashString shRuleRejectOffer = "RuleRejectOffer";
    const HashString shRuleRemoveCity = "RuleRemoveCity";
    const HashString shRuleRemoveSettlement = "RuleRemoveSettlement";
    const HashString shRuleRequestInitialPlacement = "RuleRequestInitialPlacement";
    const HashString shRuleRequestInitialRoad = "RuleRequestInitialRoad";
    const HashString shRuleResetObjectFlags = "RuleResetObjectFlags";
    const HashString shRuleResetTileFlags = "RuleResetTileFlags";
    const HashString shRuleRestartGame = "RuleRestartGame";
    const HashString shRuleRestartOldState = "RuleRestartOldState";
    const HashString shRuleRestartPreRoll = "RuleRestartPreRoll";
    const HashString shRuleRestartRobber = "RuleRestartRobber";
    const HashString shRuleRestartRobberSteal = "RuleRestartRobberSteal";
    const HashString shRuleRestartTurn = "RuleRestartTurn";
    const HashString shRuleRestartTurnLose7 = "RuleRestartTurnLose7";
    const HashString shRuleRestartTurnMonopoly = "RuleRestartTurnMonopoly";
    const HashString shRuleRestartTurnYearOfPlenty = "RuleRestartTurnYearOfPlenty";
    const HashString shRuleRobberSteal = "RuleRobberSteal";
    const HashString shRuleRollDice = "RuleRollDice";
    const HashString shRuleRollDiceText = "RuleRollDiceText";
    const HashString shRuleSelectRobberSteal = "RuleSelectRobberSteal";
    const HashString shRuleSignalTurn = "RuleSignalTurn";
    const HashString shRuleSilentUpdateOption = "RuleSilentUpdateOption";
    const HashString shRuleStartRobber = "RuleStartRobber";
    const HashString shRuleStartRobberCommon = "RuleStartRobberCommon";
    const HashString shRuleUpdateMessageUI = "RuleUpdateMessageUI";
    const HashString shRuleUpdateNetworkUI = "RuleUpdateNetworkUI";

	//-----------------------------------------------------------------------//
	// Section: wxStrings
	//
    const wxString sw0 = wxT("0");
    const wxString sw1 = wxT("1");
    const wxString sw2 = wxT("2");
    const wxString sw3 = wxT("3");
    const wxString sw4 = wxT("4");
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
    const wxString swAllowedCards = wxT("AllowedCards");
    const wxString swBaseGLCanvas = wxT("wxBaseGLCanvas");
    const wxString swBlock = wxT("Block");
    const wxString swBridge = wxT("Bridge");
    const wxString swCities = wxT("Cities");
    const wxString swColon = wxT(":");
    const wxString swCommaSpace = wxT(", ");
    const wxString swConfigSectionOpenGL = wxT("ConfigSectionOpenGL");
	const wxString swCoreGLCanvas = wxT("wxCoreGLCanvas");
    const wxString swDiceRolls = wxT("DiceRolls");
    const wxString swEmpty = wxT("");
    const wxString swFrameworkGLCanvas = wxT("wxFrameworkGLCanvas");
    const wxString swGlChipsTex = wxT("glchips.tex");
    const wxString swGlCityTex = wxT("glcity.tex");
    const wxString swGlRoadTex = wxT("glroad.tex");
    const wxString swGlSettlementTex = wxT("glsettlement.tex");
    const wxString swMipmap = wxT("Mipmap");
    const wxString swMonopoly = wxT("Monopoly");
    const wxString swPoints = wxT("Points");
    const wxString swRoadBuilding = wxT("RoadBuilding");
    const wxString swRoads = wxT("Roads");
    const wxString swRobberGouraud = wxT("RobberGouraud");
    const wxString swRounds = wxT("Rounds");
    const wxString swSettlements = wxT("Settlements");
    const wxString swSettlersGLCanvas = wxT("wxSettlersGLCanvas");
    const wxString swSoldier = wxT("Soldier");
    const wxString swSpace = wxT(" ");
    const wxString swStandard = wxT("Standard");
    const wxString swStandardRules = wxT("StandardRules");
    const wxString swValue = wxT("Value");
    const wxString swVictoryPoint = wxT("VictoryPoint");
    const wxString swYearOfPlenty = wxT("YearOfPlenty");

	// Formats
    const wxString swFloat6 = wxT("%.6f");
    const wxString swFloatPercent = wxT("%.1f%%");
    const wxString swIntFormat = wxT("%d");
    const wxString swIntString = wxT("%d %s");
    const wxString swSlashStringSlashString = wxT("/%s/%s");
    const wxString swStringFormat = wxT("%s");

	//-----------------------------------------------------------------------//
	// Section: Translations
	//
    const wxString stBuildACity = _("Build a city");
    const wxString stBuildARoad = _("Build a road");
    const wxString stBuildASettlement = _("Build a settlement");
    const wxString stBuyADevelopmentCard = _("Buy a development card");
    const wxString stCardsAllowed = _("Cards allowed");
    const wxString stCardsAllowedInHand = _("Cards allowed in hand:");
    const wxString stChangeTheNumberOfCards = _("Change the number of cards each player may hold without losing half on a 7.");
    const wxString stChangeTheNumberOfPoints = _("Change the number of points needed to win the game.");
    const wxString stChangeTheDeck = _("Change the deck of Development cards.");
    const wxString stChangeTheStartingStock = _("Change the starting stock amounts of roads, settlements, and cities.");
    const wxString stChooseOneResourceType = _("Choose one resource type.  All other players must give you all their resource cards of that type.");
    const wxString stCity = _("City");
    const wxString stCities = _("Cities");
    const wxString stClay = _("Clay");
    const wxString stDesert = _("Desert");
    const wxString stDevelopmentCards = _("Development cards");
    const wxString stDiceRolls = _("Dice Rolls");
    const wxString stGold = _("Gold");
    const wxString stInThisTurnTakeTwoCards = _("In this turn, take any two resource cards from the bank.  They may be used immediately.");
    const wxString stInThisTurnBuildTwoRoads = _("In this turn, build either two roads, two ships, or one of each for free.");
    const wxString stMonopoly = _("Monopoly");
    const wxString stMoveTheRobberAndSteal = _("Move the robber and steal one resource card from the owner of an adjacent city or settlement.  Or, move the pirate and steal from the owner of an adjacent ship.");
    const wxString stNo7sForOneRound = _("No 7s for 1 round");
    const wxString stNoEarly7s = _("No early 7s");
    const wxString stOcean = _("Ocean");
    const wxString stOdds = _("Odds");
    const wxString stOneCity = _("1 city");
    const wxString stOre = _("Ore");
    const wxString stPlayCard = _("Play Card");
    const wxString stPoints = _("Points");
    const wxString stPointsNeededToWin = _("Points needed to win");
    const wxString stPrevent7s = _("Prevent 7s from rolling for one or two rounds.  This lets players get their feet on the ground before the robber comes calling.");
    const wxString stRandomChipLayout = _("Random chip layout");
    const wxString stRandomPortLayout = _("Random port layout");
    const wxString stRandomTileLayout = _("Random tile layout");
    const wxString stReceiveOneVictoryPoint = _("Receive 1 Victory Point!");
    const wxString stResourceCards = _("Resource cards");
    const wxString stResources = _("Resources");
    const wxString stRoadBuilding = _("Road Building");
    const wxString stRoadLength = _("Road length");
    const wxString stRoad = _("Road");
    const wxString stRoads = _("Roads");
    const wxString stRobber = _("Robber");
    const wxString stRollDice = _("Roll Dice");
    const wxString stRoundsBeforeA7IsRolled = _("Rounds before a 7 is rolled");
    const wxString stSelectOneResourceTypeToSteal = _("Select 1 resource type to steal.");
    const wxString stSelectTwoResourcesToGain = _("Select 2 resources to gain.");
    const wxString stSettlement = _("Settlement");
    const wxString stSettlements = _("Settlements");
    const wxString stSheep = _("Sheep");
    const wxString stShowResourcesTab = _("Show Resources tab");
    const wxString stSoldier = _("Soldier");
    const wxString stSoldiers = _("Soldiers");
    const wxString stSpendYourGold = _("Spend Your Gold");
    const wxString stStandard = _("Standard");
    const wxString stStockAmounts = _("Stock amounts");
    const wxString stThreeToOne = _("3-1 Port");
    const wxString stTimber = _("Timber");
    const wxString stTwoToOneClay = _("2-1 Clay Port");
    const wxString stTwoToOneOre = _("2-1 Ore Port");
    const wxString stTwoToOneSheep = _("2-1 Sheep Port");
    const wxString stTwoToOneTimber = _("2-1 Timber Port");
    const wxString stTwoToOneWheat = _("2-1 Wheat Port");
    const wxString stVictoryPoint = _("Victory Point");
    const wxString stVictoryPoints = _("Victory points");
    const wxString stWheat = _("Wheat");
    const wxString stYearOfPlenty = _("Year Of Plenty");
    const wxString stYourCards = _("Your Cards");

	// Formats
    const wxString stBuysADevelopmentCard = _("%s buys a development card.");
    const wxString stChooseAPlayerToStealFrom = _("%s, choose a player to steal from, or click the robber to not steal.");
    const wxString stEndsTheTurn = _("%s ends the turn.");
    const wxString stHasACityReduced = _("%s has a city reduced to a settlement.");
    const wxString stHasWonTheGame = _("%s has won the game.");
    const wxString stIntCardsAllowedInHand = _("%d cards allowed in hand");
    const wxString stIntStringFromString = _("%d %s from %s");
    const wxString stItIsYourTurn = _("%s, it is your turn.");
    const wxString stLoses = _("%s loses %s.");
    const wxString stLosesLargestArmy = _("%s loses Largest Army.");
    const wxString stLosesLongestRoad = _("%s loses Longest Road.");
    const wxString stNo7sForXRounds = _("No 7s for %d rounds");
    const wxString stPlaceACity = _("%s, place a city.");
    const wxString stPlaceARoad = _("%s, place a road.");
    const wxString stPlaceASettlement = _("%s, place a settlement.");
    const wxString stPlacesACity = _("%s places a city.");
    const wxString stPlacesASettlement = _("%s places a settlement.");
    const wxString stPlacesARoad = _("%s places a road.");
    const wxString stPlaceTheRobber = _("%s, place the robber.");
    const wxString stPlayACardOrRoll = _("%s, play a card or roll the dice.");
    const wxString stPlayACardSelectAnAction = _("%s, play a card, select an action, or roll the dice.");
    const wxString stPlaysACard = _("%s plays a %s card.");
    const wxString stPlayToXPoints = _("Play to %d points");
    const wxString stRollsA = _("%s rolls a %d.");
    const wxString stSelectAnActionOrRoll = _("%s, select an action or roll the dice.");
    const wxString stSelectAResourceToSteal = _("%s, select a resource to steal.");
    const wxString stSelects = _("%s selects %s.");
    const wxString stSelectResourcesToLose = _("%s, select resources to lose.");
    const wxString stSelectTwoResources = _("%s, select two resources.");
    const wxString stSelectXResourcesToGain = _("Select %d resources to gain.");
    const wxString stStealsNothing = _("%s steals nothing.");
    const wxString stSteals = _("%s steals %s.");
    const wxString stStealsOneFromYou = _("%s steals 1 %s from you.");
    const wxString stStealsOneResourceFrom = _("%s steals 1 resource from %s.");
    const wxString stStringSpendYourGold = _("%s, spend your gold.");
    const wxString stTakesLargestArmy = _("%s takes Largest Army.");
    const wxString stTakesLongestRoad = _("%s takes Longest Road.");
    const wxString stThereIsNotEnough = _("There is not enough %s in the bank. No one receives any.");
    const wxString stTradesTo = _("%s trades %s to %s for %s.");
    const wxString stTradesToTheBank = _("%s trades %s to the bank for %s.");
    const wxString stUndoesABankTrade = _("%s undoes a bank trade.");
    const wxString stUndoesACity = _("%s undoes a city.");
    const wxString stUndoesARoad = _("%s undoes a road.");
    const wxString stUndoesASettlement = _("%s undoes a settlement.");
    const wxString stYouReceiveACard = _("You receive a %s card.");
    const wxString stYouStealOne = _("You steal 1 %s from %s.");
    const wxString stWaitingFinishTurn = _("Waiting for %s to finish the turn.");
    const wxString stWaitingLoseResources = _("Waiting for %s to lose resources.");
    const wxString stWaitingPlaceACity = _("Waiting for %s to place a city.");
    const wxString stWaitingPlaceARoad = _("Waiting for %s to place a road.");
    const wxString stWaitingPlaceASettlement = _("Waiting for %s to place a settlement.");
    const wxString stWaitingPlaceTheRobber = _("Waiting for %s to place the robber.");
    const wxString stWaitingPlayACardOrRoll = _("Waiting for %s to play a card or roll the dice.");
    const wxString stWaitingPlayACardSelectAnAction = _("Waiting for %s to play a card, select an action, or roll the dice.");
    const wxString stWaitingSelectAnActionOrRoll = _("Waiting for %s to select an action or roll the dice.");
    const wxString stWaitingSelectAResourceToSteal = _("Waiting for %s to select a resource to steal.");
    const wxString stWaitingSelectTwoResources = _("Waiting for %s to select two resources.");
    const wxString stWaitingSpendGold = _("Waiting for %s to spend gold.");
    const wxString stWaitingToSteal = _("Waiting for %s to steal a card.");
    const wxString stWinsTheGame = _("%s wins the game!");
    const wxString stXCities = _("%d cities");
    const wxString stXRoads = _("%d roads");
    const wxString stXSettlements = _("%d settlements");

    //-----------------------------------------------------------------------//
	// Section: Sounds
	//
    const wxString SOUND_BUY_DEV_CARD = wxT("BuyDevelopmentCard.mp3");
    const wxString SOUND_CITY_REDUCED = wxT("CityReduced.mp3");
    const wxString SOUND_END_TURN = wxT("EndTurn.mp3");
    const wxString SOUND_LARGEST_ARMY = wxT("LargestArmy.mp3");
    const wxString SOUND_LONGEST_ROAD = wxT("LongestRoad.mp3");
    const wxString SOUND_LOSE_GAME = wxT("LoseGame.mp3");
    const wxString SOUND_LOSE_ON_7 = wxT("LoseCardsOn7.mp3");
    const wxString SOUND_NO_STEAL = wxT("NoSteal.mp3");
    const wxString SOUND_PLACE_CITY	= wxT("PlaceCity.mp3");
    const wxString SOUND_PLACE_ROAD	= wxT("PlaceRoad.mp3");
    const wxString SOUND_PLACE_ROBBER = wxT("PlaceRobber.mp3");
    const wxString SOUND_PLACE_SETTLEMENT = wxT("PlaceSettlement.mp3");
    const wxString SOUND_PLACE_SETTLEMENT_ON_PORT = wxT("PlaceSettlementOnPort.mp3");
    const wxString SOUND_PLAY_MONOPOLY_CARD = wxT("PlayMonopolyCard.mp3");
    const wxString SOUND_PLAY_ROAD_BUILDING_CARD = wxT("PlayRoadBuildingCard.mp3");
    const wxString SOUND_PLAY_SOLDIER_CARD = wxT("PlaySoldierCard.mp3");
    const wxString SOUND_PLAY_VICTORY_CARD = wxT("PlayVPCard.mp3");
    const wxString SOUND_PLAY_YOP_CARD = wxT("PlayYearOfPlentyCard.mp3");
    const wxString SOUND_ROLL_DICE = wxT("RollDice.mp3");
    const wxString SOUND_STEAL_CARD	= wxT("StealACard.mp3");
    const wxString SOUND_TRADE_BANK	 = wxT("TradeToBank.mp3");
    const wxString SOUND_TRADE_OFFER = wxT("TradeOffer.mp3");
    const wxString SOUND_TRADE_REFUSE = wxT("TradeRefuse.mp3");
    const wxString SOUND_TRADE_REJECT = wxT("TradeReject.mp3");
    const wxString SOUND_TRADE_SELECT = wxT("TradeSelect.mp3");
    const wxString SOUND_WIN_GAME = wxT("WinGame.mp3");
 
    //-----------------------------------------------------------------------//
	// Section: Options
	//
    const wxString ALLOWED_CARDS = wxT("AllowedCards");
	const wxString DEVELOPMENT_CARDS = wxT("DevelopmentCards");
	
    // TODO: Implement Jungles in its own DLL.
	const wxString JUNGLES = wxT("Jungles");
    
    const wxString NO7S = wxT("No7s");
	const wxString POINTS = wxT("Points");
	const wxString STOCK_AMOUNTS = wxT("StockAmounts");
	
    //-----------------------------------------------------------------------//
	// Section: Rulesets
	//
    const wxString AROUND_THE_BOARD = wxT("AroundTheBoard");
	const wxString AROUND_THE_BOARD_PORTS = wxT("AroundTheBoardPorts");
	const wxString DEVCARDS = wxT("DevCards");
	const wxString GOLD = wxT("Gold");
    const wxString RANDOM_TILES = wxT("RandomTiles");
	const wxString STANDARD = wxT("Standard");
	
}

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
