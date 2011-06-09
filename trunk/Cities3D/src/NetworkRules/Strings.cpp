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
    const HashString shTileObject = "TileObject";

    // Data
    const HashString shBankResources = "BankResources";
    const HashString shComplete = "Complete";
	const HashString shCurrentPlayer = "CurrentPlayer";
	const HashString shDrop = "Drop";
	const HashString shHostID = "HostID";
    const HashString shHostIP = "HostIP";
    const HashString shHostPort = "HostPort";
    const HashString shHostQuit = "HostQuit";
    const HashString shMode = "Mode";
    const HashString shNetwork = "Network";
    const HashString shNetworkDropFlag = "NetworkDropFlag";
    const HashString shNetworkLaunch = "NetworkLaunch";
    const HashString shNetworkPlayerNotReady = "NetworkPlayerNotReady";
    const HashString shNetworkPlayerReady = "NetworkPlayerReady";
    const HashString shNetworkRestart = "NetworkRestart";
    const HashString shNetworkRestartState = "NetworkRestartState";
    const HashString shNew = "New";
    const HashString shQuit = "Quit";
    const HashString shReady = "Ready";
	const HashString shResources = "Resources";
    const HashString shRestartTime = "RestartTime";
    const HashString shSpectator = "Spectator";
    const HashString shSpectatorReplaceNo = "SpectatorReplaceNo";
    const HashString shSpectatorReplaceYes = "SpectatorReplaceYes";
    const HashString shState = "State";
	
    // Display
    const HashString shBackground = "Background";
    const HashString shBoldFont = "BoldFont";
    const HashString shChatMessageFont = "ChatMessageFont";
    const HashString shChatMessageText = "ChatMessageText";
    const HashString shChatMessageTimestamp = "ChatMessageTimestamp";
    const HashString shDefaultFont = "DefaultFont";
	const HashString shDefaultText = "DefaultText";
    const HashString shInfoFont = "InfoFont";
    const HashString shInfoText = "InfoText";
    const HashString shNetworkRestartAbsent = "NetworkRestartAbsent";
    const HashString shNetworkRestartListMainFont = "NetworkRestartListMainFont";
    const HashString shNetworkRestartListSubFont = "NetworkRestartListSubFont";
    const HashString shNetworkRestartPresent = "NetworkRestartPresent";
    const HashString shNetworkTextOff = "NetworkTextOff";
    const HashString shNetworkTextOn = "NetworkTextOn";
    const HashString shNetworkTextWait = "NetworkTextWait";
    const HashString shSystemMessageFont = "SystemMessageFont";
	const HashString shSystemMessageText = "SystemMessageText";
    const HashString shSystemMessageTimestamp = "SystemMessageTimestamp";
    
    // Events
	const HashString shEventAnimation = "eventAnimation";
    const HashString shEventChangeMainPane = "eventChangeMainPane";
    const HashString shEventChatMessage = "eventChatMessage";
    const HashString shEventCountdownTime = "eventCountdownTime";
    const HashString shEventGameConnect = "eventGameConnect";
    const HashString shEventLoadRulesets = "eventLoadRulesets";
    const HashString shEventMessageBox = "eventMessageBox";
    const HashString shEventMessageUI = "eventMessageUI";
    const HashString shEventNetworkAllSeatsFull = "eventNetworkAllSeatsFull";
    const HashString shEventNetworkConnectionAttempt = "eventNetworkConnectionAttempt";
    const HashString shEventNetworkConnectionEvent = "eventNetworkConnectionEvent";
    const HashString shEventNetworkRestart = "eventNetworkRestart";
    const HashString shEventNetworkRestartPlayers = "eventNetworkRestartPlayers";
    const HashString shEventNewPlayer = "eventNewPlayer";
    const HashString shEventPreGame = "eventPreGame";
    const HashString shEventPrepareNetworkGame = "eventPrepareNetworkGame";
    const HashString shEventRequestUserAttention = "eventRequestUserAttention";
    const HashString shEventResetGame = "eventResetGame";
    const HashString shEventResetReady = "eventResetReady";
    const HashString shEventServerConnection = "eventServerConnection";
    const HashString shEventShutdownUI = "eventShutdownUI";
	const HashString shEventSkin = "eventSkin";
    const HashString shEventSpectator = "eventSpectator";
    const HashString shEventStartGame = "eventStartGame";
    const HashString shEventSystemMessage = "eventSystemMessage";
    const HashString shEventTabChat = "eventTabChat";
    const HashString shEventTabEvents = "eventTabEvents";
    const HashString shEventTabSpectators = "eventTabSpectators";
	const HashString shEventUncheckMenu = "eventUncheckMenu";

    // Logic
    const HashString shLogicIsThisPlayer = "LogicIsThisPlayer";
    const HashString shNetworkLogicAllowSaves = "NetworkLogicAllowSaves";
    const HashString shNetworkLogicPlayerColor = "NetworkLogicPlayerColor";

    // Rules
    const HashString shNetworkRuleAddPlayer = "NetworkRuleAddPlayer";
    const HashString shNetworkRuleAddSpectator = "NetworkRuleAddSpectator";
    const HashString shNetworkRuleAllSeatsFull = "NetworkRuleAllSeatsFull";
    const HashString shNetworkRuleChatMessage = "NetworkRuleChatMessage";
    const HashString shNetworkRuleChangeColor = "NetworkRuleChangeColor";
    const HashString shNetworkRuleChangeColorMessage = "NetworkRuleChangeColorMessage";
    const HashString shNetworkRuleCheckCountdowns = "NetworkRuleCheckCountdowns";
    const HashString shNetworkRuleClearIP = "NetworkRuleClearIP";
    const HashString shNetworkRuleClientDesyncCheckBankResources = "NetworkRuleClientDesyncCheckBankResources";
    const HashString shNetworkRuleClientDesyncCheckPlayerResources = "NetworkRuleClientDesyncCheckPlayerResources";
    const HashString shNetworkRuleConnectionLost = "NetworkRuleConnectionLost";
    const HashString shNetworkRuleConnectionLostInGame = "NetworkRuleConnectionLostInGame";
    const HashString shNetworkRuleConnectionLostPreGame = "NetworkRuleConnectionLostPreGame";
    const HashString shNetworkRuleJoinGame = "NetworkRuleJoinGame";
    const HashString shNetworkRuleLaunchGame = "NetworkRuleLaunchGame";
    const HashString shNetworkRuleLoadMap = "NetworkRuleLoadMap";
    const HashString shNetworkRulePlayerNotReady = "NetworkRulePlayerNotReady";
    const HashString shNetworkRulePlayerReady = "NetworkRulePlayerReady";
    const HashString shNetworkRulePlayerReplace = "NetworkRulePlayerReplace";
    const HashString shNetworkRuleQuit = "NetworkRuleQuit";
    const HashString shNetworkRuleRemovePlayer = "NetworkRuleRemovePlayer";
    const HashString shNetworkRuleRemoveSpectator = "NetworkRuleRemoveSpectator";
    const HashString shNetworkRuleRequestAddPlayer = "NetworkRuleRequestAddPlayer";
    const HashString shNetworkRuleRequestChangeColor = "NetworkRuleRequestChangeColor";
    const HashString shNetworkRuleResetReady = "NetworkRuleResetReady";
    const HashString shNetworkRuleSpectatorState = "NetworkRuleSpectatorState";
    const HashString shNetworkRuleStartCountdown = "NetworkRuleStartCountdown";
    const HashString shNetworkRuleStartGame = "NetworkRuleStartGame";
    const HashString shNetworkRuleStoreIP = "NetworkRuleStoreIP";
    const HashString shNetworkRuleSystemMessage = "NetworkRuleSystemMessage";
    const HashString shNetworkRuleTakeSeat = "NetworkRuleTakeSeat";
    const HashString shNetworkRuleTickCountdown = "NetworkRuleTickCountdown";
    const HashString shRule0 = "Rule0";
    const HashString shRuleAddPlayer = "RuleAddPlayer";
    const HashString shRuleChangeColor = "RuleChangeColor";
    const HashString shRuleGenerateSystemMessage = "RuleGenerateSystemMessage";
    const HashString shRuleLoadMap = "RuleLoadMap";
    const HashString shRulePlaySound = "RulePlaySound";
    const HashString shRulePrepareUI = "RulePrepareUI";
    const HashString shRuleRandomizePlayers = "RuleRandomizePlayers";
    const HashString shRuleRestartGame = "RuleRestartGame";
    const HashString shRuleUIRemovePlayer = "RuleUIRemovePlayer";
    
    //-----------------------------------------------------------------------//
	// Section: wxStrings
	//
    const wxString swBaseGLCanvas = wxT("wxBaseGLCanvas");
    const wxString swConfigSectionNetwork = wxT("ConfigSectionNetwork");
    const wxString swConnectingToAGame = wxT("Connecting To A Game");
    const wxString swConnectToAGame = wxT("Connect To A Game");
    const wxString swCoreRules = wxT("CoreRules");
	const wxString swCoreGLCanvas = wxT("wxCoreGLCanvas");
    const wxString swDesyncDetectedBank = wxT("Desync detected [bank]: ");
    const wxString swEmpty = wxT("");
    const wxString swFrameworkGLCanvas = wxT("wxFrameworkGLCanvas");
    const wxString swHost = wxT("Host");
    const wxString swHostAddress = wxT("HostAddress");
    const wxString swHostPort = wxT("HostPort");
    const wxString swLastConnect = wxT("LastConnect");
    const wxString swLastPort = wxT("LastPort");
    const wxString swNetwork = wxT("Network");
    const wxString swNetworkGameRestart = wxT("Network Game Restart");
    const wxString swOriginalPlayer = wxT("OriginalPlayer");
    const wxString swPlay = wxT("Play");
    const wxString swPlayer = wxT("Player");
    const wxString swSave = wxT("Save");
    const wxString swSettlersGLCanvas = wxT("wxSettlersGLCanvas");
    const wxString swServerSettings = wxT("Server Settings");
    const wxString swZeroIP = wxT("0.0.0.0");

    // Formats
    const wxString swBracketLeft = wxT("[");
    const wxString swBracketRightSpace = wxT("] ");
    const wxString swColon = wxT(":");
    const wxString swCommaSpace = wxT(", ");
    const wxString swDesyncDetectedString = wxT("Desync detected [%s]: ");
    const wxString swFloat6 = wxT("%.6f");
    const wxString swIntFormat = wxT("%d");
    const wxString swIntOnServerIntOnClient = wxT("%s - [%d on server, %d on client]");
    const wxString swIPFormat = wxT("%s:%d (%s)");
    const wxString swMinutesSeconds = wxT(" (%M:%S)");
    const wxString swSlashStringSlashString = wxT("/%s/%s");
    const wxString swSpace = wxT(" ");
    const wxString swStringInt = wxT("%s (%d)");
    
    //-----------------------------------------------------------------------//
	// Section: Translations
	//
    const wxString stAdd = _("Add");
    const wxString stChat = _("Chat");
    const wxString stConnect = _("Connect");
    const wxString stConnected = _("Connected");
    const wxString stConnecting = _("Connecting...");
    const wxString stConnectingToAGame = _("Connecting To A Game");
    const wxString stConnection = _("Connection");
    const wxString stConnectTo = _("Connect To");
    const wxString stConnectToAGame = _("Connect To A Game");
    const wxString stDelete = _("Delete");
    const wxString stDisconnect = _("Disconnect");
    const wxString stDisconnectedFromServer = _("Disconnected from the server.");
    const wxString stDragAndDropNames = _("Drag and drop names, or double-click to add/remove.");
    const wxString stEnterAnIPAddress = _("Enter an IP address or host name in the Connect To window and the port number in the Port window.  The default port is 7176.");
    const wxString stEnterAValidHostname = _("Enter a valid host name or IP address in the Hostname window.  Enter the port number clients should connect to in the Port window.  The default port is 7176.");
    const wxString stEvents = _("Events");
    const wxString stGameSaved = _("Game saved.");
    const wxString stGameSetup = _("Game Setup");
    const wxString stHost = _("Host");
    const wxString stHostname = _("Hostname");
    const wxString stIllPlay = _("I'll play!");
    const wxString stIPAddress = _("IP address");
    const wxString stLANNetwork = _("LAN / Network");
    const wxString stLANNetworkSetup = _("LAN / Network Setup");
    const wxString stLostConnectionToTheServer = _("Lost connection to the server.");
    const wxString stNetwork = _("Network");
    const wxString stNetworkGameRestart = _("Network Game Restart");
    const wxString stNotConnected = _("Not connected");
    const wxString stNoThanks = _("No, thanks.");
    const wxString stNotReady = _("Not ready!");
    const wxString stOK = _("OK");
    const wxString stOnlineGameRestart = _("Online Game Start");
    const wxString stOptions = _("Options");
    const wxString stPlayer = _("Player");
    const wxString stPort = _("Port");
    const wxString stQuestion = _("Question");
    const wxString stReady = _("Ready!");
    const wxString stSelectMap = _("Select Map");
    const wxString stServerSettings = _("Server Settings");
    const wxString stSettings = _("Settings");
    const wxString stShowChatTab = _("Show Chat tab");
    const wxString stShowEventsTab = _("Show Events tab");
    const wxString stShowSpectatorsTab = _("Show Spectators tab");
    const wxString stSpectators = _("Spectators");
    const wxString stStartServer = _("Start Server");
    const wxString stStarted = _("Started");
    const wxString stStartGame = _("Start Game!");
    const wxString stStatus = _("Status");
    const wxString stStopped = _("Stopped");
    const wxString stStopServer = _("Stop Server");
    const wxString stThePortMustBeAValidNumber = _("The port must be a valid number between 0 and 65535.");
    const wxString stThereArePlayersConnectedToYou = _("There are players connected to you.  Are you sure you want to shut down the server?");
    const wxString stUnselected = _("Unselected");
    const wxString stUseTheConnectionSettings = _("Use the Connection settings to either host a game or connect to a game.  Select the player you wish to play with from the list.");
    const wxString stWaiting = _("Waiting...");
    const wxString stWaitingForPlayersToJoin = _("Waiting for players to join.");
    const wxString stWaitingForPlayersToRejoin = _("Waiting for players to rejoin.");
    const wxString stYouAreCurrentlyConnectedToAServer = _("You are currently connected to a server.  Are you sure you want to leave?");
    const wxString stYouMustEnterAnIPAddress = _("You must enter an IP address or host name.");
    const wxString stYouMustEnterAPort = _("You must enter a port number.");
    const wxString stYouMustEnterAValidHostname = _("You must enter a valid host name or IP address.");

    // Formats
    const wxString stAreYouSureYouWantToDelete = _("Are you sure you want to delete %s?");
    const wxString stAttempt = _("Attempt %d: %s");
    const wxString stAttemptingToConnectTo = _("Attempting to connect to %s:%d...");
    const wxString stAttemptOf = _("Attempt %d of %d");
    const wxString stAPlayerAttemptsToJoinWithVersion = _("A player attempts to join with version %s.  The server version is %s.");
    const wxString stBecomesASpectator = _("%s becomes a spectator.");
    const wxString stDropsFromTheGame = _("%s drops from the game.");
    const wxString stIsNoLongerRead = _("%s is no longer ready.");
    const wxString stIsNow = _("%s is now %s.");
    const wxString stIsReady = _("%s is ready.");
    const wxString stJoinsAs = _("%s joins as %s.");
    const wxString stLeavesTheServer = _("%s leaves the server.");
    const wxString stQuitsTheGame = _("%s quits the game.");
    const wxString stRejoinsAs = _("%s rejoins as %s.");
    const wxString stReplacesAs = _("%s replaces %s as %s.");
    const wxString stShutdownTheServer = _("%s shut down the server.");
    const wxString stTakesOverAs = _("%s takes over as %s.");
    const wxString stTheGameWillRestartIn = _("The game will restart in %d.");
    const wxString stTheGameWillStartIn = _("The game will start in %d.");
    const wxString stTheMapIsNow = _("The map is now %s.");
    
    //-----------------------------------------------------------------------//
	// Section: Sounds
	//
    const wxString SOUND_CHAT_MESSAGE = wxT("ChatMessageReceived.mp3");
    const wxString SOUND_YOUR_TURN = wxT("YourTurn.mp3");
    
    //-----------------------------------------------------------------------//
	// Section: Rulesets
	//
    const wxString MODULE = wxT("NetworkRules");
	const wxString NETWORK = wxT("Network");
}

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
