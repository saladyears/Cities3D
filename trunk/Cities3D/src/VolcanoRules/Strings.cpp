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

    // All strings used in this ruleset.

	//-----------------------------------------------------------------------//
	// Section: HashStrings
	//
    const HashString shChipObject = "ChipObject";
    const HashString shDummySelectionObject = "DummySelectionObject";
	const HashString shEmpty = "";
    const HashString shIntFormat = "%d";
	const HashString shTileObject = "TileObject";
    
	// Data
	const HashString sh1 = "1";
    const HashString shCities = "Cities";
    const HashString shCurrentPlayer = "CurrentPlayer";
    const HashString shDesert = "Desert";
    const HashString shDice = "Dice";
    const HashString shDie1 = "Die1";
    const HashString shDie2 = "Die2";
    const HashString shExtra = "Extra";
    const HashString shGold = "Gold";
    const HashString shInitial = "Initial";
    const HashString shMetropolis = "Metropolis";
    const HashString shMoveMetropolis = "MoveMetropolis";
    const HashString shNextVolcanoRule = "NextVolcanoRule";
    const HashString shOriginalPlayer = "OriginalPlayer";
    const HashString shRobber = "Robber";
    const HashString shRoll = "Roll";
	const HashString shSecondary = "Secondary";
    const HashString shSettlements = "Settlements";
    const HashString shSides = "Sides";
    const HashString shState = "State";
    const HashString shStockSettlements = "StockSettlements";
    const HashString shTileType = "TileType";
    const HashString shVolcano = "Volcano";
	const HashString shVolcanoDie = "VolcanoDie";
    const HashString shVolcanoMetropolis = "VolcanoMetropolis";
    
	// Display
    const HashString shBackground = "Background";
    const HashString shDefaultFont = "DefaultFont";
    const HashString shDefaultText = "DefaultText";

	// Events
    const HashString shEventAddSidePane = "eventAddSidePane";
    const HashString shEventBuildUI = "eventBuildUI";
    const HashString shEventControlsUI = "eventControlsUI";
    const HashString shEventGameObjects = "eventGameObjects";
    const HashString shEventMessageUI = "eventMessageUI";
    const HashString shEventPlayerReorder = "eventPlayerReorder";
    const HashString shEventPlayerUI = "eventPlayerUI";
    const HashString shEventSelectionObject = "eventSelectionObject";
    const HashString shEventTabVolcano = "eventTabVolcano";
    const HashString shEventVolcanoResult = "eventVolcanoResult";
    const HashString shEventVolcanoRoll = "eventVolcanoRoll";

	// Logic
    const HashString shLogicIsThisPlayer = "LogicIsThisPlayer";

	// Rules
    const HashString shRuleAdjustPoints = "RuleAdjustPoints";
    const HashString shRuleBeginBlockingAction = "RuleBeginBlockingAction";
    const HashString shRuleCityReducedVolcano = "RuleCityReducedVolcano";
    const HashString shRuleCornerDestroyedVolcano = "RuleCornerDestroyedVolcano";
    const HashString shRuleEndBlockingAction = "RuleEndBlockingAction";
    const HashString shRuleMarkerPostDieRoll = "RuleMarkerPostDiceRoll";
    const HashString shRulePlaceCityVolcanoCities = "RulePlaceCityVolcanoCities";
    const HashString shRulePlaySound = "RulePlaySound";
    const HashString shRuleReduceCityCommon = "RuleReduceCityCommon";
    const HashString shRuleReduceCityVolcanoCities = "RuleReduceCityVolcanoCities";
    const HashString shRuleRemoveCity = "RuleRemoveCity";
    const HashString shRuleRemoveSettlement = "RuleRemoveSettlement";
    const HashString shRuleResetObjectFlags = "RuleResetObjectFlags";
    const HashString shRuleResetTileFlags = "RuleResetTileFlags";
    const HashString shRuleRestartVolcanoMetropolis = "RuleRestartVolcanoMetropolis";
    const HashString shRuleSelectVolcanoMetropolis = "RuleSelectVolcanoMetropolis";
    const HashString shRuleUpdateNetworkUI = "RuleUpdateNetworkUI";
    const HashString shRuleVolcano = "RuleVolcano";
    
	//-----------------------------------------------------------------------//
	// Section: wxStrings
	//
	const wxString swCities = wxT("Cities");
    const wxString swCitiesRules = wxT("CitiesRules");
    const wxString swComma = wxT(", ");
	const wxString swEmpty = wxT("");
    const wxString swGold = wxT("Gold");
    const wxString swHiddenTiles = wxT("HiddenTiles");
    const wxString swSeafarersRules = wxT("SeafarersRules");
    const wxString swSingleBonus = wxT("SingleBonus");
    const wxString swStandardRules = wxT("StandardRules");
	const wxString swVolcano = wxT("Volcano");
    const wxString swVolcanoRules = wxT("VolcanoRules");

	// Formats
    const wxString swStringStringExclamation = wxT("%s %s!");
    
	//-----------------------------------------------------------------------//
	// Section: Translations
	//
    const wxString stDesertsAreVolcanoes = _("Deserts are volcanoes");
    const wxString stHas1CityReduced = _("has 1 city reduced");
    const wxString stLoses1City = _("loses 1 city");
    const wxString stLoses1Settlement = _("loses 1 settlement");
    const wxString stShowVolcanoTab = _("Show Volcano tab");
    const wxString stVolcano = _("Volcano");
    const wxString stVolcanoes = _("Volcanoes");
    const wxString stVolcanoLongDescription = _("Replaces all deserts with volcanoes.  A volcano is rich in resources and provides gold to players who build on it.  However, when rolled, it erupts, and destroys anything in the path of its lava, which flows to one of its corners!");
    const wxString stVolcanoTile = _("Volcano tile");

	// Formats
    const wxString stEscapesUnscathed = _("%s escapes unscathed!");
    const wxString stHasXCitiesReduced = _("has %d cities reduced");
    const wxString stMoveMetropolis = _("%s, move your metropolis.");
    const wxString stLosesXCities = _("loses %d cities");
    const wxString stLosesXSettlements = _("loses %d settlements");
    const wxString stLosesBonusPoint = _("%s loses 1 bonus point to the volcano.");
    const wxString stWaitingMoveMetropolis = _("Waiting for %s to move a metropolis.");
    
	//-----------------------------------------------------------------------//
	// Section: Sounds
	//
    const wxString SOUND_VOLCANIC_DESTRUCTION = wxT("VolcanicDestruction.mp3");
    const wxString SOUND_VOLCANIC_ERUPTION = wxT("VolcanicEruption.mp3");

    //-----------------------------------------------------------------------//
	// Section: Rulesets
	//
	const wxString VOLCANO(wxT("Volcano"));
	const wxString VOLCANO_CITIES(wxT("VolcanoCities"));
	const wxString VOLCANO_DESERT(wxT("VolcanoDesert"));
	const wxString VOLCANO_SINGLE_BONUS(wxT("VolcanoSingleBonus"));
	const wxString VOLCANO_HIDDEN_TILES(wxT("VolcanoHiddenTiles"));
}

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
