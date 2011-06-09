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
    const HashString shCornerSelectionObject = "CornerSelectionObject";
	const HashString shDummySelectionObject = "DummySelectionObject";
	const HashString shEmpty = "";
	const HashString shEvent = "event";
    const HashString shPortObject = "PortObject";
    const HashString shSideSelectionObject = "SideSelectionObject";
    const HashString shTileObject = "TileObject";

	// Data
	const HashString shAnyMatch = "AnyMatch";
	const HashString shAnyOffer = "AnyOffer";
	const HashString shAutoTradeLogic = "AutoTradeLogic";
	const HashString shAutoTradeRule = "AutoTradeRule";
	const HashString shBankResources = "BankResources";
	const HashString shComplete = "Complete";
	const HashString shCurrentPlayer = "CurrentPlayer";
	const HashString shID = "ID";
	const HashString shMapID = "MapID";
	const HashString shMode = "Mode";
	const HashString shOriginalBankResources = "OriginalBankResources";
    const HashString shPoints = "Points";
	const HashString shPort = "Port";
	const HashString shPort1 = "Port1";
	const HashString shPort2 = "Port2";
	const HashString shPorts = "Ports";
	const HashString shPortSide = "PortSide";
	const HashString shRandomPorts = "RandomPorts";
	const HashString shResources = "Resources";
	const HashString shSides = "Sides";
	const HashString shState = "State";
	const HashString shTileType = "TileType";
	const HashString shTimber = "Timber";
	const HashString shTimeStart = "TimeStart";

	// Display
	const HashString shBackground = "Background";
	const HashString shBoldFont = "BoldFont";
	const HashString shBuildUIIcons = "BuildUIIcons";
	const HashString shCard = "Card";
	const HashString shCardHeight = "CardHeight";
	const HashString shCardSpacing = "CardSpacing";
	const HashString shCardWidth = "CardWidth";
    const HashString shDefaultFont = "DefaultFont";
	const HashString shDefaultText = "DefaultText";
	const HashString shDisabledCardText = "DisabledCardText";
	const HashString shExit = "Exit";
	const HashString shFrameCurve = "FrameCurve";
	const HashString shGameListMainFont = "GameListMainFont";
	const HashString shGameListSubFont = "GameListSubFont";
	const HashString shGameUIClickable = "GameUIClickable";
	const HashString shGameUIFont = "GameUIFont";
	const HashString shGameUIOutline = "GameUIOutline";
	const HashString shGameUIText = "GameUIText";
	const HashString shHeight = "Height";
	const HashString shHotseat = "Hotseat";
	const HashString shIconSize = "IconSize";
	const HashString shIconTransparent = "IconTransparent";
	const HashString shImageHeight = "ImageHeight";
	const HashString shImageSpacing = "ImageSpacing";
	const HashString shImageWidth = "ImageWidth";
	const HashString shInfoFont = "InfoFont";
	const HashString shInfoText = "InfoText";
	const HashString shMapText = "MapText";
	const HashString shNameFont = "NameFont";
	const HashString shOpenGame = "OpenGame";
	const HashString shOutline = "Outline";
	const HashString shPlayerIcon = "PlayerIcon";
	const HashString shPlayerSelectionMainFont = "PlayerSelectionMainFont";
	const HashString shPlayerSelectionSubFont = "PlayerSelectionSubFont";
	const HashString shPlayerUICounterText = "PlayerUICounterText";
	const HashString shPlayerUINameFont = "PlayerUINameFont";
	const HashString shPlayerUINegative = "PlayerUINegative";
	const HashString shPlayerUIPositive = "PlayerUIPositive";
	const HashString shPlayerUITextFont = "PlayerUITextFont";
	const HashString shPopupFont = "PopupFont";
	const HashString shSettings = "Settings";
	const HashString shSpectatorListMainFont = "SpectatorListMainFont";
	const HashString shSpectatorListSubFont = "SpectatorListSubFont";
	const HashString shSystemMessageFont = "SystemMessageFont";
	const HashString shSystemMessageText = "SystemMessageText";
	const HashString shTabHeight = "TabHeight";
	const HashString shTabWidth = "TabWidth";
	const HashString shTextFont = "TextFont";
	const HashString shTextHeight = "TextHeight";
	const HashString shTextWidth = "TextWidth";
	const HashString shTrade = "Trade";
	const HashString shTradeBoxFont = "TradeBoxFont";
	const HashString shTradeClick = "TradeClick";
	const HashString shTradeMatch = "TradeMatch";
	const HashString shUserChangeableText = "UserChangeableText";
	const HashString shWidth = "Width";

	// Events
	const HashString shEventAcceptOffer = "eventAcceptOffer";
	const HashString shEventBankTrade = "eventBankTrade";
	const HashString shEventBuildUI = "eventBuildUI";
	const HashString shEventChangeMainPane = "eventChangeMainPane";
	const HashString shEventClearOffer = "eventClearOffer";
	const HashString shEventClickOffer = "eventClickOffer";
	const HashString shEventCloseGameWindow = "eventCloseGameWindow";
	const HashString shEventControlsBank = "eventControlsBank";
	const HashString shEventControlsEndTurn = "eventControlsEndTurn";
	const HashString shEventControlsPlayCard = "eventControlsPlayCard";
	const HashString shEventControlsRollDice = "eventControlsRollDice";
	const HashString shEventControlsTrade = "eventControlsTrade";
	const HashString shEventControlsUI = "eventControlsUI";
	const HashString shEventControlsUndo = "eventControlsUndo";
	const HashString shEventCreateGameWindow = "eventCreateGameWindow";
	const HashString shEventCurrentOffer = "eventCurrentOffer";
	const HashString shEventDirty = "eventDirty";
	const HashString shEventExit = "eventExit";
	const HashString shEventGameObjects = "eventGameObjects";
	const HashString shEventImageUpdate = "eventImageUpdate";
	const HashString shEventInitializeUI = "eventInitializeUI";
	const HashString shEventLoadMap = "eventLoadMap";
	const HashString shEventLoadRulesets = "eventLoadRulesets";
	const HashString shEventMatchOffer = "eventMatchOffer";
	const HashString shEventMessageBox = "eventMessageBox";
	const HashString shEventMessageUI = "eventMessageUI";
	const HashString shEventNewPlayer = "eventNewPlayer";
	const HashString shEventOthersTrade = "eventOthersTrade";
	const HashString shEventPlayerOffer = "eventPlayerOffer";
	const HashString shEventPlayerReorder = "eventPlayerReorder";
	const HashString shEventPlayerUI = "eventPlayerUI";
	const HashString shEventPopupToggle = "eventPopupToggle";
	const HashString shEventPreGame = "eventPreGame";
	const HashString shEventPreRollCard = "eventPreRollCard";
	const HashString shEventQuit = "eventQuit";
	const HashString shEventRenderBitmaps = "eventRenderBitmaps";
	const HashString shEventRejectOffer = "eventRejectOffer";
	const HashString shEventResetBitmaps = "eventResetBitmaps";
	const HashString shEventResetReady = "eventResetReady";
    const HashString shEventRestoreLayout = "eventRestoreLayout";
	const HashString shEventSaveLayout = "eventSaveLayout";
	const HashString shEventShowResources = "eventShowResources";
	const HashString shEventShutdownModelessUI = "eventShutdownModelessUI";
	const HashString shEventShutdownUI = "eventShutdownUI";
	const HashString shEventSkin = "eventSkin";
	const HashString shEventSpectator = "eventSpectator";
	const HashString shEventStartGame = "eventStartGame";
	const HashString shEventStats = "eventStats";
	const HashString shEventTrade = "eventTrade";
	const HashString shEventUncheckMenu = "eventUncheckMenu";
	const HashString shEventUpdateUndo = "eventUpdateUndo";
	const HashString shEventWontTrade = "eventWontTrade";

	// Logic
	const HashString shLogicCanAcceptTrade = "LogicCanAcceptTrade";
	const HashString shLogicCanAutoTrade = "LogicCanAutoTrade";
	const HashString shLogicCanEndTurn = "LogicCanEndTurn";
	const HashString shLogicCanShowCard = "LogicCanShowCard";
	const HashString shLogicCanTradeBank = "LogicCanTradeBank";
	const HashString shLogicCanTradeOthers = "LogicCanTradeOthers";
	const HashString shLogicCardList = "LogicCardList";
	const HashString shLogicIsResourceBankTradeable = "LogicIsResourceBankTradeable";
	const HashString shLogicIsThisPlayer = "LogicIsThisPlayer";
	const HashString shLogicResourceOrder = "LogicResourceOrder";
	const HashString shLogicRollText = "LogicRollText";
	const HashString shLogicShowPlayerUI = "LogicShowPlayerUI";
	const HashString shLogicTileIsOcean = "LogicTileIsOcean";
	const HashString shLogicTileIsPort = "LogicTileIsPort";
	const HashString shLogicTileIsResource = "LogicTileIsResource";

	// Rules
	const HashString shRule0 = "Rule0";
	const HashString shRuleAcceptOffer = "RuleAcceptOffer";
	const HashString shRuleAddPlayer = "RuleAddPlayer";
	const HashString shRuleBreakUndo = "RuleBreakUndo";
	const HashString shRuleChangeColor = "RuleChangeColor";
	const HashString shRuleClearOffer = "RuleClearOffer";
	const HashString shRuleClickOffer = "RuleClickOffer";
	const HashString shRuleCloseTradeWindow = "RuleCloseTradeWindow";
	const HashString shRuleExecuteBankTrade = "RuleExecuteBankTrade";
	const HashString shRuleExecuteTrade = "RuleExecuteTrade";
	const HashString shRuleFindPorts = "RuleFindPorts";
	const HashString shRuleGenerateBoard = "RuleGenerateBoard";
	const HashString shRuleGeneratePorts = "RuleGeneratePorts";
	const HashString shRuleInitializeData = "RuleInitializeData";
	const HashString shRuleInitializeOptionOverrides = "RuleInitializeOptionOverrides";
	const HashString shRuleInitializeOptions = "RuleInitializeOptions";
	const HashString shRuleLoadMap = "RuleLoadMap";
	const HashString shRuleMarkerEndTurn = "RuleMarkerEndTurn";
	const HashString shRuleMarkerStartInitial = "RuleMarkerStartInitial";
	const HashString shRuleOfferTrade = "RuleOfferTrade";
    const HashString shRulePerformGameAction = "RulePerformGameAction";
	const HashString shRulePlaySound = "RulePlaySound";
	const HashString shRulePreGenerateBoard = "RulePreGenerateBoard";
	const HashString shRulePrepareUI = "RulePrepareUI";
	const HashString shRulePostGenerateBoard = "RulePostGenerateBoard";
	const HashString shRuleRandomizePlayers = "RuleRandomizePlayers";
	const HashString shRuleRejectOffer = "RuleRejectOffer";
	const HashString shRuleResetAutoTrade = "RuleResetAutoTrade";
	const HashString shRuleResetObjectFlags = "RuleResetObjectFlags";
	const HashString shRuleResetTileFlags = "RuleResetTileFlags";
	const HashString shRuleRollDice = "RuleRollDice";
	const HashString shRuleSelectOption = "RuleSelectOption";
	const HashString shRuleShowCards = "RuleShowCards";
	const HashString shRuleShowResources = "RuleShowResources";
	const HashString shRuleSilentUpdateOption = "RuleSilentUpdateOption";
	const HashString shRuleTradeBank = "RuleTradeBank";
	const HashString shRuleTradeOthers = "RuleTradeOthers";
	const HashString shRuleUIAddPlayer = "RuleUIAddPlayer";
	const HashString shRuleUIChangeColor = "RuleUIChangeColor";
	const HashString shRuleUIRemovePlayer = "RuleUIRemovePlayer";
	const HashString shRuleUndo = "RuleUndo";
	const HashString shRuleWontTrade = "RuleWontTrade";

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
	const wxString sw16 = wxT("16");
	const wxString sw24 = wxT("24");
	const wxString sw32 = wxT("32");
	const wxString swActions = wxT("Actions");
	const wxString swAddALayout = wxT("Add A Layout");
	const wxString swAdvanced = wxT("Advanced");
	const wxString swAnimation = wxT("Animation");
	const wxString swAnimationBoard = wxT("Animation/Board");
	const wxString swBank = wxT("Bank");
    const wxString swBaseGLCanvas = wxT("wxBaseGLCanvas");
	const wxString swBuildOptions = wxT("BuildOptions");
	const wxString swCards = wxT("Cards");
	const wxString swCitiesOnline = wxT("Cities Online");
	const wxString swColon = wxT(":");
	const wxString swCommaSpace = wxT(", ");
    const wxString swConfigSectionGeneral = wxT("ConfigSectionGeneral");
    const wxString swConfigSectionOpenGL = wxT("ConfigSectionOpenGL");
    const wxString swConfigSectionUI = wxT("ConfigSectionUI");
	const wxString swControls = wxT("Controls");
    const wxString swCoreGLCanvas = wxT("wxCoreGLCanvas");
	const wxString swDate = wxT("Date");
	const wxString swDebugInfo = wxT("DebugInfo");
	const wxString swDepthBuffer = wxT("DepthBuffer");
	const wxString swEmpty = wxT("");
	const wxString swEnglishUS = wxT("English (U.S.)");
	const wxString swExit = wxT("Exit");
	const wxString swFileAmp = wxT("&File");
	const wxString swFrameBuffer = wxT("Framebuffer");
    const wxString swFrameworkGLCanvas = wxT("wxFrameworkGLCanvas");
	const wxString swHeight = wxT("height");
	const wxString swHTMLBody = wxT("<HTML><BODY bgcolor=black>");
	const wxString swHotseat = wxT("Hotseat");
	const wxString swImages = wxT("Images");
	const wxString swKeyMaps = wxT("Keymaps");
	const wxString swLanguage = wxT("Language");
	const wxString swLastOptionOpen = wxT("LastOptionOpen");
	const wxString swMap = wxT("Map");
	const wxString swMipmap = wxT("Mipmap");
	const wxString swModule = wxT("Module");
	const wxString swOpen = wxT("Open");
	const wxString swOption = wxT("Option");
	const wxString swPanes = wxT("&Panes");
	const wxString swPixelBuffer = wxT("Pixelbuffer");
	const wxString swPlay = wxT("Play");
	const wxString swPlayer = wxT("Player");
	const wxString swPlayers = wxT("Players");
	const wxString swPlaySounds = wxT("PlaySounds");
	const wxString swPopup = wxT("Popup");
	const wxString swPopups = wxT("Popups");
	const wxString swPopupTimeout = wxT("PopupTimeout");
	const wxString swQuit = wxT("Quit");
	const wxString swRenderBitmaps = wxT("RenderBitmaps");
	const wxString swRestoreLayout = wxT("RestoreLayout");
	const wxString swRoot = wxT("root");
	const wxString swSave = wxT("Save");
    const wxString swSaveLayout = wxT("SaveLayout");
	const wxString swSettings = wxT("Settings");
	const wxString swSettlersGLCanvas = wxT("wxSettlersGLCanvas");
	const wxString swSettlersView = wxT("SettlersView");
	const wxString swSkin = wxT("Skin");
	const wxString swSkinFolder = wxT("SkinFolder");
	const wxString swSkins = wxT("skins");
	const wxString swSoundFolder = wxT("SoundFolder");
	const wxString swSounds = wxT("sounds");
	const wxString swSoundsCaps = wxT("Sounds");
    const wxString swSpace = wxT(" ");
	const wxString swSpectator = wxT("Spectator");
	const wxString swStandard = wxT("Standard");
	const wxString swStats = wxT("Stats");
	const wxString swStatus = wxT("Status");
	const wxString swStockyard = wxT("Stockyard");
	const wxString swStockyardSlash = wxT("Stockyard/");
	const wxString swTahoma = wxT("Tahoma");
	const wxString swTex = wxT("tex");
	const wxString swTexFoler = wxT("TexFolder");
	const wxString swTradeWithTheBank = wxT("Trade With The Bank");
	const wxString swTradeWithTheBankSlash = wxT("Trade With The Bank/");
	const wxString swTradeWithOthers = wxT("Trade With Others");
	const wxString swTradeWithOthersSlash = wxT("Trade With Others/");
	const wxString swTradeWithOthersX = wxT("Trade With Others/x");
	const wxString swTradeWithOthersY = wxT("Trade With Others/y");
	const wxString swWidth = wxT("width");
	const wxString swWxSettlersView = wxT("wxSettlersView");

	// Formats
    const wxString swFloat6 = wxT("%.6f");
    const wxString swIntFormat = wxT("%d");
    const wxString swSlashStringSlashString = wxT("/%s/%s");
	const wxString swStringFormat = wxT("%s");
	const wxString swTimeFormat = wxT("%#c");

	//-----------------------------------------------------------------------//
	// Section: Translations
	//
	const wxString stAccept = _("Accept");
	const wxString stAction = _("Action");
	const wxString stAdd = _("Add");
	const wxString stAdvanced = _("Advanced");
	const wxString stAll = _("All");
	const wxString stAnimation = _("Animation");
	const wxString stAssign = _("Assign");
	const wxString stBitmaps = _("Bitmaps");
	const wxString stBank = _("Bank");
	const wxString stBoardAnimationsShow = _("Board animations show you when another player changes the board.");
	const wxString stBuildOptions = _("Build Options");
	const wxString stCards = _("Cards");
	const wxString stClear = _("Clear");
	const wxString stClickOnAnOfferToTrade = _("2.  Click an offer to trade with that player.");
	const wxString stClickOtherOffers = _("Click other offers to match them.");
	const wxString stControls = _("Controls");
	const wxString stCore = _("Core");
	const wxString stCurrentGameSettings = _("Current game settings");
	const wxString stDefault = _("Default");
	const wxString stDelete = _("Delete");
	const wxString stDepthBufferSize = _("Depth Buffer Size");
	const wxString stDisplay = _("Display");
	const wxString stDisplayOpenGLRender = _("Display OpenGL render information");
	const wxString stDragAndDropCards = _("Drag and drop cards to create a trade.");
	const wxString stDragAndDropCardsOffer = _("1.  Drag and drop cards to create an offer.");
	const wxString stDragAndDropNames = _("Drag and drop names, or double-click to add/remove.");
	const wxString stEndTheTurn = _("End the turn");
	const wxString stEndTurn = _("End Turn");
	const wxString stExit = _("Exit");
	const wxString stExitAmp = _("E&xit");
	const wxString stFew = _("Few");
	const wxString stFinalizeYourTrade = _("3.  Finalize your trade!");
	const wxString stFor = _("for");
	const wxString stForMostImageSets = _("For most image sets, softening the tiles makes the game board look better.");
	const wxString stGameImages = _("Game Images");
	const wxString stGameSounds = _("Game Sounds");
	const wxString stGameWindow = _("Game Window");
	const wxString stHalf = _("Half");
	const wxString stHidePopups = _("Hide popups");
	const wxString stHotseat = _("Hotseat");
	const wxString stHotseatSetup = _("Hotseat Setup");
	const wxString stImages = _("Images");
	const wxString stIsCurrentlyAssignedTo = _("%s is currently assigned to %s.");
	const wxString stKeyBindings = _("Key Bindings");
	const wxString stKeyboard = _("Keyboard");
	const wxString stKeystroke = _("Keystroke");
	const wxString stLanguage = _("Language");
	const wxString stLayoutAmp = _("&Layout");
	const wxString stLoadASavedGame = _("Load A Saved Game");
	const wxString stLoadSaved = _("Load Saved");
	const wxString stMany = _("Many");
	const wxString stMost = _("Most");
	const wxString stName = _("Name");
	const wxString stNew = _("New");
	const wxString stNewKeystroke = _("New keystroke");
	const wxString stNone = _("None");
	const wxString stNoThanks = _("No, thanks.");
	const wxString stNoTrades = _("No Trades");
	const wxString stOffer = _("Offer");
	const wxString stOK = _("OK");
	const wxString stOptions = _("Options");
	const wxString stPanMapDown = _("Pan map down");
	const wxString stPanMapLeft = _("Pan map left");
	const wxString stPanMapRight = _("Pan map right");
	const wxString stPanMapUp = _("Pan map up");
	const wxString stPlayACard = _("Play a card");
	const wxString stPlayBoardAnimations = _("Play board animations");
	const wxString stPlayers = _("Players");
	const wxString stPlayNext =_("Play Next");
	const wxString stPlaySounds = _("Play Sounds");
	const wxString stPopups = _("Popups");
	const wxString stQuestion = _("Question");
	const wxString stQuitAmp = _("&Quit");
	const wxString stRecommendedForPlayers = _("Recommended for new players or to distinguish between different colors.");
	const wxString stReject = _("Reject");
	const wxString stRenderBitmapsWithOpenGL = _("Render bitmaps with OpenGL");
	const wxString stRespondWithStockyard = _("Respond with the Stockyard buttons, or make a counteroffer.");
    const wxString stRestoreOriginalLayout = _("&Restore original layout");
	const wxString stRollDice = _("Roll Dice");
	const wxString stRollTheDice = _("Roll the dice");
	const wxString stRotateMapDown = _("Rotate map down");
	const wxString stRotateMapLeft = _("Rotate map left");
	const wxString stRotateMapRight = _("Rotate map right");
	const wxString stRotateMapUp = _("Rotate map up");
	const wxString stSaveAmp = _("&Save");
    const wxString stSecondsBeforePopups = _("Seconds before popups disappear");
	const wxString stSelectALanguage = _("Select a Language");
    const wxString stSelectMap = _("Select Map");
	const wxString stSelectTheLanguage = _("Select the language you would like to use from the list.  You must restart Cities Online for your choice to take effect.");
	const wxString stSelectTheSetOfImages = _("Select the set of images you would like to use from the list.  The display will be updated immediately.");
	const wxString stSelectTheSetOfSounds = _("Select the set of sounds you would like to use from the list.  Your choice will take effect immediately.");
	const wxString stSelectTheSkin = _("Select the skin you would like to use from the list.  Your choice will take effect immediately.");
	const wxString stSetThisLayoutAsDefault = _("&Set this layout as default");
	const wxString stSettings = _("Settings");
	const wxString stSettingsAmp = _("&Settings");
	const wxString stShowPopups = _("Show popups");
	const wxString stSkin = _("Skin");
	const wxString stSkins = _("Skins");
	const wxString stSoftenTiles = _("Soften tiles");
	const wxString stSome = _("Some");
	const wxString stSounds = _("Sounds");
	const wxString stStats = _("Stats");
	const wxString stStatsAmp = _("&Stats");	
	const wxString stStatus = _("Status");
	const wxString stStockyard = _("Stockyard");
	const wxString stThatWouldBeCheating = _("That would be cheating!");
	const wxString stToggleFullscreen = _("Toggle fullscreen");
	const wxString stTogglePopups = _("Toggle popups");
	const wxString stTrade = _("Trade");
	const wxString stTradeExclamation = _("Trade!");
	const wxString stTradeWithBank = _("Trade with bank");
	const wxString stTradeWithOthers = _("Trade with others");
	const wxString stTradeWithOthersCaps = _("Trade With Others");
	const wxString stTradeWithTheBank = _("Trade With The Bank");
	const wxString stTurningSoundsOff = _("Turning sounds off will make the game completely silent.");
	const wxString stUnableToRestartTheGame = _("Unable to restart the game.");
	const wxString stUndo = _("Undo");
	const wxString stUndoAction = _("Undo action");
	const wxString stUseFrameBuffers = _("Use frame buffers (if available)");
	const wxString stUsePixelBuffers = _("Use pixel buffers (if available)");
	const wxString stYou = _("You");
	const wxString stYouAccept = _("You accept");
	const wxString stYouGet = _("You get");
	const wxString stYouGive = _("You give");
	const wxString stYouOffer = _("You offer");
	const wxString stYouPass = _("You pass");
	const wxString stYouWant = _("You want");
	const wxString stYouWontTrade = _("You won't trade");
	const wxString stZoomIntoMap = _("Zoom into map");
	const wxString stZoomOutOfMap = _("Zoom out of map");

	// Formats
	const wxString stAreYouSureYouWantToDelete = _("Are you sure you want to delete %s?");
	const wxString stIfAcceptsYourOffer = _("If %s accepts your offer, finalize it.");
	const wxString stStringAccepts = _("%s accepts");
	const wxString stStringGives = _("%s gives");
	const wxString stStringOffers = _("%s offers");
	const wxString stStringPasses = _("%s passes");
	const wxString stStringWants = _("%s wants");
	const wxString stStringWontTrade = _("%s won't trade");

	//-----------------------------------------------------------------------//
	// Section: Sounds
	//
	const wxString SOUND_TRADE_ACCEPT = wxT("TradeAccept.mp3");
	const wxString SOUND_TRADE_COUNTER = wxT("TradeCounter.mp3");

	//-----------------------------------------------------------------------//
	// Section: Rulesets
	//
	const wxString CORE = wxT("Core");
	const wxString EXIT = wxT("Exit");
	const wxString HOTSEAT = wxT("Hotseat");
	const wxString LAYOUT = wxT("Layout");
	const wxString MODULE = wxT("CoreRules");
	const wxString OPEN = wxT("Open");
	const wxString SAVE = wxT("Save");
	const wxString SETTINGS = wxT("Settings");
	const wxString STATS = wxT("Stats");
	const wxString QUIT = wxT("Quit");
}

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
