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
	const HashString shEmpty = "";
    
	// Data
	const HashString sh2 = "2";
    const HashString sh3 = "3";
    const HashString sh4 = "4";
    const HashString sh5 = "5";
    const HashString sh6 = "6";
    const HashString sh7 = "7";
    const HashString sh8 = "8";
    const HashString sh9 = "9";
    const HashString sh10 = "10";
    const HashString sh11 = "11";
    const HashString sh12 = "12";
    const HashString shAllowedCards = "AllowedCards";
    const HashString shBorder = "Border";
    const HashString shChipless = "Chipless";
    const HashString shChiplessChipArray = "ChiplessChipArray";
    const HashString shChiplessChips = "ChiplessChips";
    const HashString shChiplessResources = "ChiplessResources";
    const HashString shChiplessTiles = "ChiplessTiles";
    const HashString shChipObject = "ChipObject";
    const HashString shClay = "Clay";
    const HashString shDesert = "Desert";
	const HashString shGold = "Gold";
    const HashString shHidden = "Hidden";
    const HashString shHiddenChipArray = "HiddenChipArray";
    const HashString shHiddenChips = "HiddenChips";
    const HashString shHiddenResources = "HiddenResources";
    const HashString shHiddenTiles = "HiddenTiles";
    const HashString shHotseat = "Hotseat";
    const HashString shID = "ID";
    const HashString shIndex = "Index";
    const HashString shInitial = "Initial";
    const HashString shInitialDepth = "InitialDepth";
    const HashString shInitialRotation = "InitialRotation";
    const HashString shIsland = "Island";
    const HashString shLandTiles = "LandTiles";
    const HashString shLastEdited = "LastEdited";
    const HashString shMapID = "MapID";
    const HashString shMinPlayers = "MinPlayers";
    const HashString shMaxPlayers = "MaxPlayers";
    const HashString shMode = "Mode";
    const HashString shNew = "New";
    const HashString shOcean = "Ocean";
    const HashString shPoints = "Points";
    const HashString shPort = "Port";
    const HashString shPort1 = "Port1";
    const HashString shPort2 = "Port2";
    const HashString shPort3 = "Port3";
    const HashString shPortClay = "PortClay";
    const HashString shPortObject = "PortObject";
    const HashString shPortOre = "PortOre";
    const HashString shPorts = "Ports";
    const HashString shPortSheep = "PortSheep";
    const HashString shPortSide = "PortSide";
    const HashString shPortTimber = "PortTimber";
    const HashString shPortWheat = "PortWheat";
    const HashString shOfficial = "Official";
    const HashString shOre = "Ore";
    const HashString shRandom = "Random";
    const HashString shRandomChipArray = "RandomChipArray";
    const HashString shRandomChips = "RandomChips";
    const HashString shRandomTiles = "RandomTiles";
    const HashString shRandomPorts = "RandomPorts";
    const HashString shRandomResources = "RandomResources";
    const HashString shRoll = "Roll";
    const HashString shRollPlaces = "RollPlaces";
    const HashString shSeaTiles = "SeaTiles";
    const HashString shSecondary = "Secondary";
    const HashString shSheep = "Sheep";
    const HashString shState = "State";
    const HashString shStockCities = "StockCities";
    const HashString shStockRoads = "StockRoads";
    const HashString shStockSettlements = "StockSettlements";
    const HashString shStockShips = "StockShips";
    const HashString shSides = "Sides";
    const HashString shTileType = "TileType";
	const HashString shTimber = "Timber";
    const HashString shTimeStart = "TimeStart";
    const HashString shTileObject = "TileObject";
    const HashString shVersion = "Version";
    const HashString shVictoryPoints = "VictoryPoints";
    const HashString shWheat = "Wheat";
    const HashString shWinner = "Winner";

	// Display

	// Events
	const HashString shEventLoadRulesets = "eventLoadRulesets";
    const HashString shEventLoadOptions = "eventLoadOptions";
    const HashString shEventStartGame = "eventStartGame";
    
	// Logic

	// Rules
	
	//-----------------------------------------------------------------------//
	// Section: wxStrings
	//
    const wxString swANewWorldIII = wxT("A New World (III)");
    const wxString swANewWorldIIIInternal = wxT("ANewWorldIII");
    const wxString swANewWorldIV = wxT("A New World (IV)");
    const wxString swANewWorldIVInternal = wxT("ANewWorldIV");
    const wxString swANewWorldV = wxT("A New World (V)");
    const wxString swANewWorldVInternal = wxT("ANewWorldV");
    const wxString swANewWorldVI = wxT("A New World (VI)");
    const wxString swANewWorldVIInternal = wxT("ANewWorldVI");
    const wxString swAroundTheBoard = wxT("AroundTheBoard");
    const wxString swAroundTheBoardPorts = wxT("AroundTheBoardPorts");
    const wxString swChiplessTiles = wxT("ChiplessTiles");
	const wxString swEmpty = wxT("");
    const wxString swExtent = wxT("AyjL|p");
    const wxString swFiveIslandsV = wxT("Five Islands (V)");
    const wxString swFiveIslandsInternal = wxT("FiveIslandsV");
    const wxString swFourIslandsIII = wxT("Four Islands (III)");
    const wxString swFourIslandsIIIInternal = wxT("FourIslandsIII");
    const wxString swFourIslandsIV = wxT("Four Islands (IV)");
    const wxString swFourIslandsIVInternal = wxT("FourIslandsIV");
    const wxString swGame = wxT("game");
    const wxString swGold = wxT("Gold");
    const wxString swHiddenTiles = wxT("HiddenTiles");
    const wxString swHTMLListIntString = wxT("<LI>%d %s");
    const wxString swHTMLListIntIntString = wxT("<LI>%d-%d %s");
    const wxString swInitialShips = wxT("InitialShips");
    const wxString swIntoTheDesertIII = wxT("Into The Desert (III)");
    const wxString swIntoTheDesertIIIInternal = wxT("IntoTheDesertIII");
    const wxString swIntoTheDesertIV = wxT("Into The Desert (IV)");
    const wxString swIntoTheDesertIVInternal = wxT("IntoTheDesertIV");
    const wxString swIntoTheDesertV = wxT("Into The Desert (V)");
    const wxString swIntoTheDesertVInternal = wxT("IntoTheDesertV");
    const wxString swIntoTheDesertVI = wxT("Into The Desert (VI)");
    const wxString swIntoTheDesertVIInternal = wxT("IntoTheDesertVI");
    const wxString swJasonFugate = wxT("Jason Fugate");
    const wxString swKeys = wxT("keys");
    const wxString swNewShores = wxT("New Shores");
    const wxString swNewShoresInternal = wxT("NewShores");
    const wxString swOceansIII = wxT("Oceans (III)");
    const wxString swOceansIIIInternal = wxT("OceansIII");
    const wxString swOceansIV = wxT("Oceans (IV)");
    const wxString swOceansIVInternal = wxT("OceansIV");
    const wxString swOceansV = wxT("Oceans (V)");
    const wxString swOceansVInternal = wxT("OceansV");
    const wxString swOceansVI = wxT("Oceans (VI)");
    const wxString swOceansVIInternal = wxT("OceansVI");
    const wxString swPortPlacement = wxT("PortPlacement");
    const wxString swRandomTiles = wxT("RandomTiles");
    const wxString swRepeatingBonus = wxT("RepeatingBonus");
    const wxString swSeafarers = wxT("Seafarers");
    const wxString swSeafarersRules = wxT("SeafarersRules");
    const wxString swSingleBonus = wxT("SingleBonus");
    const wxString swSixIslands = wxT("Six Islands (VI)");
    const wxString swSixIslandsInternal = wxT("SixIslandsVI");
    const wxString swStandard = wxT("Standard");
    const wxString swStandardRules = wxT("StandardRules");
    const wxString swStandardSettlersIV = wxT("Standard Settlers (III/IV)");
    const wxString swStandardSettlersIVInternal = wxT("StandardSettlersIIIIV");
    const wxString swStandardSettlersVI = wxT("Standard Settlers (V/VI)");
    const wxString swStandardSettlersVIInternal = wxT("StandardSettlersVVI");
    const wxString swTheBigIslandIV = wxT("The Big Island (IV)");
    const wxString swTheBigIslandIVInternal = wxT("TheBigIslandIV");
    const wxString swTheBigIslandVVI = wxT("The Big Island (V/VI)");
    const wxString swTheBigIslandVVIInternal = wxT("TheBigIslandVVI");
    const wxString swTheGreatCrossingIV = wxT("The Great Crossing (IV)");
    const wxString swTheGreatCrossingIVInternal = wxT("TheGreatCrossingIV");
    const wxString swTheGreatCrossingVI = wxT("The Great Crossing (VI)");
    const wxString swTheGreatCrossingVIInternal = wxT("TheGreatCrossingVI");
    const wxString swTradeRoutes = wxT("TradeRoutes");

	// Formats

	//-----------------------------------------------------------------------//
	// Section: Translations
	//
    
	// Formats
}

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
