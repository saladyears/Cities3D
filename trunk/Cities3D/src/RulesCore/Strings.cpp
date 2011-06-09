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
	const HashString shComplete = "Complete";
    const HashString shCornerSelectionObject = "CornerSelectionObject";
	const HashString shCurrentPlayer = "CurrentPlayer";
    const HashString shDummySelectionObject = "DummySelectionObject";
	const HashString shHostID = "HostID";
    const HashString shHostIP = "HostIP";
    const HashString shHostPort = "HostPort";
    const HashString shHostQuit = "HostQuit";
    const HashString shID = "ID";
    const HashString shMapID = "MapID";
    const HashString shMode = "Mode";
    const HashString shNetwork = "Network";
    const HashString shSideSelectionObject = "SideSelectionObject";
    const HashString shState = "State";
    const HashString shTileObject = "TileObject";
    
	// Display

	// Events
	const HashString shEventAddCatalog = "eventAddCatalog";
    const HashString shEventClose = "eventClose";
    const HashString shEventCloseGameWindow = "eventCloseGameWindow";
    const HashString shEventFindPlugin = "eventFindPlugin";
    const HashString shEventKeyDown = "eventKeyDown";
    const HashString shEventGameConnect = "eventGameConnect";
    const HashString shEventInitializeUI = "eventInitializeUI";
    const HashString shEventLoadMap = "eventLoadMap";
    const HashString shEventLoadOptions = "eventLoadOptions";
    const HashString shEventLoadRulesets = "eventLoadRulesets";
    const HashString shEventNetworkConnectionAttempt = "eventNetworkConnectionAttempt";
    const HashString shEventNetworkConnectionEvent = "eventNetworkConnectionEvent";
    const HashString shEventPlayerUI = "eventPlayerUI";
    const HashString shEventPreGame = "eventPreGame";
    const HashString shEventResetCamera = "eventResetCamera";
    const HashString shEventResetGame = "eventResetGame";
    const HashString shEventServerConnection = "eventServerConnection";
	const HashString shEventSkin = "eventSkin";
    const HashString shEventSpectator = "eventSpectator";
    const HashString shEventStartGame = "eventStartGame";
    const HashString shEventStats = "eventStats";
    const HashString shEventUpdateUndo = "eventUpdateUndo";
    
	// Logic
	const HashString shLogicMarkBranch = "LogicMarkBranch";
    const HashString shNetworkLogicAllowRule = "NetworkLogicAllowRule";
    
	// Rules
    const HashString shNetworkRuleAllSeatsFull = "NetworkRuleAllSeatsFull";
    const HashString shNetworkRuleAddSpectator = "NetworkRuleAddSpectator";
    const HashString shNetworkRuleChatMessage = "NetworkRuleChatMessage";
    const HashString shNetworkRuleClientConnectionLost = "NetworkRuleClientConnectionLost";
    const HashString shNetworkRuleConnectionLost = "NetworkRuleConnectionLost";
    const HashString shNetworkRuleQuit = "NetworkRuleQuit";
    const HashString shNetworkRuleRestartGame = "NetworkRuleRestartGame";
    const HashString shNetworkRuleServerConnection = "NetworkRuleServerConnection";
    const HashString shNetworkRuleServerDesyncCheck = "NetworkRuleServerDesyncCheck";
    const HashString shNetworkRuleServerStart = "NetworkRuleServerStart";
    const HashString shNetworkRuleStartCountdown = "NetworkRuleStartCountdown";
    const HashString shNetworkRuleTakeSeat = "NetworkRuleTakeSeat";
    const HashString shNetworkRuleTickCountdown = "NetworkRuleTickCountdown";
    const HashString shNetworkRuleSystemMessage = "NetworkRuleSystemMessage";
    const HashString shNetworkRuleVersionMismatch = "NetworkRuleVersionMismatch";
    const HashString shRuleBuildRoad = "RuleBuildRoad";
    const HashString shRuleBuildShip = "RuleBuildShip";
    const HashString shRuleGenerateSystemMessage = "RuleGenerateSystemMessage";
    const HashString shRuleLoadMap = "RuleLoadMap";
    const HashString shRuleLoadOptionsFromFile = "RuleLoadOptionsFromFile";
    const HashString shRuleMarkBranch = "RuleMarkBranch";
    const HashString shRuleMarkerEndTurn = "RuleMarkerEndTurn";
    const HashString shRulePlaySound = "RulePlaySound";
    const HashString shRulePostWin = "RulePostWin";
    const HashString shRulePrepareUI = "RulePrepareUI";
    const HashString shRuleRestartGame = "RuleRestartGame";
    const HashString shRuleWin = "RuleWin";
    
	//-----------------------------------------------------------------------//
	// Section: wxStrings
	//
    const wxString swAmount = wxT("Amount");
    const wxString swBlock = wxT("block");
    const wxString swConfigSectionGeneral = wxT("ConfigSectionGeneral");
	const wxString swEmpty = wxT("");
    const wxString swExpected = wxT("Expected");
	const wxString swKeyOverrides = wxT("KeyOverrides");
    const wxString swModule = wxT("Module");
    const wxString swNetwork = wxT("Network");
    const wxString swNetworkRules = wxT("NetworkRules");
    const wxString swOption = wxT("Option");
    const wxString swOptionSet = wxT("OptionSet");
    const wxString swPercent = wxT("Percent");
    const wxString swPlayer = wxT("Player");
    const wxString swPlaySounds = wxT("PlaySounds");
	const wxString swPlus = wxT("+");
    const wxString swProvideModuleInfo = wxT("ProvideModuleInfo");
    const wxString swProvideOptions = wxT("ProvideOptions");
    const wxString swProvideRulesets = wxT("ProvideRuleSets");
    const wxString swRolls = wxT("Rolls");
    const wxString swStat = wxT("stat");
    const wxString swTotal = wxT("Total");
    
	// Formats
    const wxString swFloat6 = wxT("%.6f");
    const wxString swFloatPercentFormat = wxT("%.1f%%");
    const wxString swIntFormat = wxT("%d");
    const wxString swIntStringFormat = wxT("%d%s");
    const wxString swSlashStringSlashString = wxT("/%s/%s");
    const wxString swVersion = wxT("%d.%d.%d");
    
	//-----------------------------------------------------------------------//
	// Section: Translations
	//
	
	// Formats
    
	//-----------------------------------------------------------------------//
	// Section: Sounds
	//
    const wxString SOUND_PLAYER_JOIN = wxT("PlayerJoin.mp3");
    const wxString SOUND_PLAYER_LEAVE = wxT("PlayerLeave.mp3");
    const wxString SOUND_UNDO_ACTION = wxT("UndoAction.mp3");

}

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
