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

    const HashString shConnectCmd = "?command=gameconnect&gameid=%d&connectid=%d&order=%d&color=%s";
    const HashString shDisconnectCmd = "?command=gamedisconnect&gameid=%d&disconnectid=%d";
    const HashString shQuitCmd = "?command=gamequit&gameid=%d&quitid=%d&passive=%d";
    const HashString shStartCmd = "?command=gamestart&gameid=%d";
    const HashString shUpdateCmd = "?command=gameupdate&gameid=%d&points=%d&resources=%d&cards=%d&turn=%d";
    const HashString shWinCmd = "?command=gamewin&gameid=%d&winnerid=%d&points=%d";

    // Data
    const HashString shCitiesOnline = "CitiesOnline";
	const HashString shCurrentPlayer = "CurrentPlayer";
	const HashString shExtraTimers = "ExtraTimers";
    const HashString shExtraTimerStep = "ExtraTimerStep";
    const HashString shID = "ID";
    const HashString shIgnoreExtraTimers = "IgnoreExtraTimers";
    const HashString shInfoText = "InfoText";
    const HashString shMinus = "-";
    const HashString shNetworkDropFlag = "NetworkDropFlag";
    const HashString shNetworkRestartState = "NetworkRestartState";
    const HashString shPlus = "+";
    const HashString shPassiveQuit = "PassiveQuit";
	const HashString shPassiveTime = "PassiveTime";
    const HashString shPassiveTurnTimer = "PassiveTurnTimer";
    const HashString shPoints = "Points";
    const HashString shQuit = "Quit";
    const HashString shState = "State";
    const HashString shTurn = "Turn";
	const HashString shTurnTime = "TurnTime";

    // Display
    const HashString shBackground = "Background";
	const HashString shDefaultFont = "DefaultFont";
	const HashString shDefaultText = "DefaultText";
	const HashString shPlayerUINegative = "PlayerUINegative";
    const HashString shPlayerUINeutral = "PlayerUINeutral";
    const HashString shPlayerUIPositive = "PlayerUIPositive";
    const HashString shPlayerUIWarning = "PlayerUIWarning";
    const HashString shTurnTimerFont = "TurnTimerFont";
    const HashString shTurnTimerTextFont = "TurnTimerTextFont";

    // Events
    const HashString shEventExit = "eventExit";
    const HashString shEventImmediateExit = "eventImmediateExit";
    const HashString shEventLoadRulesets = "eventLoadRulesets";
    const HashString shEventMessageBox = "eventMessageBox";
    const HashString shEventMessageUI = "eventMessageUI";
    const HashString shEventNetworkRestart = "eventNetworkRestart";
    const HashString shEventPlayerReorder = "eventPlayerReorder";
    const HashString shEventRequestUserAttention = "eventRequestUserAttention";
    const HashString shEventResetPassiveTime = "eventResetPassiveTime";
	const HashString shEventResetTurnTime = "eventResetTurnTime";
    const HashString shEventShutdownUI = "eventShutdownUI";
    const HashString shEventStartGame = "eventStartGame";
    const HashString shEventStartup = "eventStartup";
    const HashString shEventSynchronize = "eventSynchronize";
    const HashString shEventTransientCommand = "eventTransientCommand";
    
    // Logic
    const HashString shLogicCardCount = "LogicCardCount";
    const HashString shLogicIsCitiesOnline = "LogicIsCitiesOnline";
    const HashString shLogicIsOutOfTime = "LogicIsOutOfTime";
    const HashString shLogicIsThisPlayer = "LogicIsThisPlayer";
    const HashString shLogicResourceCount = "LogicResourceCount";

    // Rules
    const HashString shNetworkRuleQuit = "NetworkRuleQuit";
    const HashString shNetworkRuleTakeSeat = "NetworkRuleTakeSeat";
    const HashString shRule0 = "Rule0";
    const HashString shRuleDisableUI = "RuleDisableUI";
    const HashString shRuleMarkerEndTurn = "RuleMarkerEndTurn";
    const HashString shRulePassiveTimerDone = "RulePassiveTimerDone";
    const HashString shRulePlaySound = "RulePlaySound";
    const HashString shRulePrepareUI = "RulePrepareUI";
    const HashString shRuleRandomizePlayers = "RuleRandomizePlayers";
    const HashString shRuleRestartGame = "RuleRestartGame";
    const HashString shRuleResetPassiveTime = "RuleResetPassiveTime";
	const HashString shRuleResetTurnTime = "RuleResetTurnTime";
    const HashString shRuleStartConnectCommandGame = "RuleStartConnectCommandGame";
    const HashString shRuleStartHostCommandGame = "RuleStartHostCommandGame";
    const HashString shRuleStartSpectateCommandGame = "RuleStartSpectateCommandGame";
	const HashString shRuleTurnTimerDone = "RuleTurnTimerDone";
    const HashString shRuleUpdateNetworkUI = "RuleUpdateNetworkUI";

    //-----------------------------------------------------------------------//
	// Section: wxStrings
	//
    const wxString swCities3D = wxT("Cities3D");
    const wxString swCities3DDExe = wxT("Cities3DD.exe");
    const wxString swCities3DExe = wxT("Cities3D.exe");
    const wxString swCitiesOnline = wxT("Cities Online");
    const wxString swCitiesOnlineNoSpaces = wxT("CitiesOnline");
    const wxString swColon = wxT(":");
    const wxString swConnect = wxT("connect");
    const wxString swConnectGameID = wxT("connectgameid");
    const wxString swConnectPlayer = wxT("connectplayer");
    const wxString swConnectPort = wxT("connectport");
    const wxString swDash = wxT("-");
    const wxString swDashTime = wxT("-:--");
	const wxString swExtraTimers = wxT("extratimers");
    const wxString swHost = wxT("host");
    const wxString swHostGameID = wxT("hostgameid");
    const wxString swHostPlayer = wxT("hostplayer");
    const wxString swHostPort = wxT("hostport");
    const wxString swMap = wxT("map");
	const wxString swMinutesSeconds = wxT("%u:%02u");
    const wxString swNetwork = wxT("Network");
    const wxString swNetworkRules = wxT("NetworkRules");
    const wxString swNoAdapter = wxT("no_adapter");
    const wxString swOpponent1 = wxT("opponent1");
    const wxString swOpponent2 = wxT("opponent2");
    const wxString swOpponent3 = wxT("opponent3");
    const wxString swOpponent4 = wxT("opponent4");
    const wxString swOpponent5 = wxT("opponent5");
    const wxString swOptions = wxT("options");
	const wxString swPassiveTime = wxT("passivetime");
    const wxString swPercent20 = wxT("%20");
    const wxString swPercent22 = wxT("%22");
    const wxString swQuotation = wxT("\"");
    const wxString swSpace = wxT(" ");
    const wxString swSpace3 = wxT("   ");
    const wxString swSpectate = wxT("spectate");
    const wxString swSpectatePlayer = wxT("spectateplayer");
    const wxString swSpectatePort = wxT("spectateport");
	const wxString swTurnTime= wxT("turntime");
	const wxString swZeroTime = wxT("0:00");
    
    // Formats
    const wxString swFloat6 = wxT("%.6f");
    const wxString swIntFormat = wxT("%d");
    const wxString swSlashStringSlashString = wxT("/%s/%s");
    const wxString swStringFormat = wxT("%s");
    const wxString swSynchronizeNys = wxT("synchronize_%s.nys");
    const wxString swSynchronizeSyn = wxT("synchronize_%s.syn");
    const wxString swXIntFormat = wxT("x%d");
    
    //-----------------------------------------------------------------------//
	// Section: Translations
	//
    const wxString stActToAvoid = _("Act to avoid a passive quit!");
    const wxString stCitiesOnlineWasShutDown = _("Cities Online was shut down!  This game can no longer be played.  Restart the game from the site to continue.");
    const wxString stMultiplier = _("Multiplier");
    const wxString stQuestion = _("Question");
    const wxString stThereArePlayersConnectedToYou = _("There are players connected to you.  Are you sure you want to shut down the server?");
    const wxString stTurnTimersLeft = _("Turn timers left");
    const wxString stWaitingForPlayersToJoin = _("Waiting for players to join.");
    const wxString stYouAreCurrentlyConnectedToAServer = _("You are currently connected to a server.  Are you sure you want to leave?");
    const wxString stYouAreCurrentlyInAGame = _("You are currently in a Cities Online game!  Are you sure you want to quit?  It may go on your record!");
    const wxString stYouAreCurrentlyWatching = _("You are currently watching a Cities Online game, are you sure you want to quit?");
    
    // Formats
    const wxString stIsOutOfTime = _("%s is out of time.");
    const wxString stUsesOneTurnTimer = _("%s uses 1 turn timer (%d left).");
    const wxString stUsesXTurnTimers = _("%s uses %d turn timers (%d left).");

    //-----------------------------------------------------------------------//
	// Section: Rulesets
	//
    const wxString CITIESONLINE(wxT("CitiesOnline"));

    //-----------------------------------------------------------------------//
	// Section: Sounds
	//
    const wxString SOUND_LOW_ON_TIME = wxT("LowOnTime.mp3");
    const wxString SOUND_PASSIVE_QUIT = wxT("PassiveQuit.mp3");
    const wxString SOUND_OUT_OF_TIME = wxT("OutOfTime.mp3");
    const wxString SOUND_USE_ONE_TURN_TIMER = wxT("UseOneTurnTimer.mp3");
    const wxString SOUND_YOU_NEED_TO_DO_SOMETHING = wxT("YouNeedToDoSomething.mp3");
}

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
