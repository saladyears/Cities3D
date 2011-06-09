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
	const HashString shEmptyResource = "EmptyResource";
	const HashString shIntFormat = "%d";

	// Data
	const HashString sh1 = "1";
	const HashString shAnyOffer = "AnyOffer";
	const HashString shBarbarians = "Barbarians";
	const HashString shCurrentPlayer = "CurrentPlayer";
	const HashString shDice = "Dice";
	const HashString shDiceCK = "DiceCK";
	const HashString shDiceRed = "DiceRed";
	const HashString shFont = "Font";
	const HashString shHeight = "Height";
	const HashString shHotseat = "Hotseat";
	const HashString shIcons = "Icons";
	const HashString shMapText = "MapText";
	const HashString shMode = "Mode";
	const HashString shNetwork = "Network";
	const HashString shOutline = "Outline";
	const HashString shPlayerCards = "PlayerCards";
	const HashString shPolitics = "Politics";
	const HashString shRed1 = "Red1";
	const HashString shRedIntFormat = "Red%d";
	const HashString shResources = "Resources";
	const HashString shScience = "Science";
	const HashString shTileObject = "TileObject";
	const HashString shTimber = "Timber";
	const HashString shTrade = "Trade";
	const HashString shUI = "UI";
	const HashString shWidth = "Width";

	// Display
	const HashString shBackground = "Background";
	const HashString shBoldFont = "BoldFont";
	const HashString shButtonDisabled = "ButtonDisabled";
	const HashString shButtonEnabled = "ButtonEnabled";
	const HashString shButtonFont = "ButtonFont";
	const HashString shButtonText = "ButtonText";
	const HashString shButtonTextShadow = "ButtonTextShadow";
	const HashString shCardHeaderFont = "CardHeaderFont";
	const HashString shChatEntryFont = "ChatEntryFont";
	const HashString shChatEntryText = "ChatEntryText";
	const HashString shClickable = "Clickable";
	const HashString swContextMenuFont = "ContextMenuFont";
	const HashString shDefaultFont = "DefaultFont";
	const HashString shDefaultText = "DefaultText";
	const HashString shFontColor = "FontColor";
	const HashString shFrameCurve = "FrameCurve";
	const HashString shGameUIClickable = "GameUIClickable";
	const HashString shGameUIFont = "GameUIFont";
	const HashString shGameUIOutline = "GameUIOutline";
	const HashString shGameUIText = "GameUIText";
	const HashString shIconTransparent = "IconTransparent";
	const HashString shInfoFont = "InfoFont";
	const HashString shInputBackground = "InputBackground";
	const HashString shListHighlight = "ListHighlight";
	const HashString shListHighlightAndSelection = "ListHighlightAndSelection";
	const HashString shListMainText = "ListMainText";
	const HashString shListMainTextDisabled = "ListMainTextDisabled";
	const HashString shListMainTextShadow = "ListMainTextShadow";
	const HashString shListSelection = "ListSelection";
	const HashString shListSubText = "ListSubText";
	const HashString shListSubTextShadow = "ListSubTextShadow";
	const HashString shMapListMainFont = "MapListMainFont";
	const HashString shMapListSubFont = "MapListSubFont";
	const HashString shOptionExcluded = "OptionExcluded";
	const HashString shOptionSelected = "OptionSelected";
	const HashString shOptionUnselected = "OptionUnselected";
	const HashString shPlayerListMainFont = "PlayerListMainFont";
	const HashString shPlayerListSubFont = "PlayerListSubFont";
	const HashString shPopupFont = "PopupFont";
	const HashString shPopupMenuBorder = "PopupMenuBorder";
	const HashString shPopupMenuCurve = "PopupMenuCurve";
	const HashString shPopupMenuOutline = "PopupMenuOutline";
	const HashString shSpectatorListMainFont = "SpectatorListMainFont";
	const HashString shSpectatorListSubFont = "SpectatorListSubFont";
	const HashString shSystemMessageFont = "SystemMessageFont";
	const HashString shSystemMessageText = "SystemMessageText";
    const HashString shTradeBoxFont = "TradeBoxFont";
	const HashString shTradeCardOutline = "TradeCardOutline";
	const HashString shTransparent = "Transparent";
	
	// Events
	const HashString shEventCards = "eventCards";
	const HashString shEventChatMessage = "eventChatMessage";
	const HashString shEventContextMenuClick = "eventContextMenuClick";
	const HashString shEventDiceRoll = "eventDiceRoll";
	const HashString shEventDirty = "eventDirty";
	const HashString shEventImageUpdate = "eventImageUpdate";
	const HashString shEventKeyDown = "eventKeyDown";
	const HashString shEventMessageBox = "eventMessageBox";
	const HashString shEventPlayerReorder = "eventPlayerReorder";
	const HashString shEventPlayerResources = "eventPlayerResources";
	const HashString shEventPlayerUI = "eventPlayerUI";
	const HashString shEventPopupToggle = "eventPopupToggle";
	const HashString shEventPreGame = "eventPreGame";
	const HashString shEventSpectator = "eventSpectator";
	const HashString shEventTabRolls = "eventTabRolls";
	const HashString shEventThirdDie = "eventThirdDie";
	const HashString shEventToggleFullscreen = "eventToggleFullscreen";
	const HashString shEventTogglePopups = "eventTogglePopups";
	const HashString shEventUncheckMenu = "eventUncheckMenu";
	const HashString shEventUpdateOption = "eventUpdateOption";
	const HashString shEventShutdownUI = "eventShutdownUI";
	const HashString shEventSkin = "eventSkin";
	const HashString shEventSystemMessage = "eventSystemMessage";

	// Logic
	const HashString shLogicCardText = "LogicCardText";
	const HashString shLogicIsThisPlayer = "LogicIsThisPlayer";
	const HashString shLogicResourceOrder = "LogicResourceOrder";
	const HashString shLogicUIAllowColorChange = "LogicUIAllowColorChange";
	
	// Rules
	const HashString shNetworkRuleAddPlayer = "NetworkRuleAddPlayer";
	const HashString shNetworkRuleChatMessage = "NetworkRuleChatMessage";
	const HashString shNetworkRuleRemovePlayer = "NetworkRuleRemovePlayer";
	const HashString shNetworkRuleRequestAddPlayer = "NetworkRuleRequestAddPlayer";
	const HashString shRuleClearOptions = "RuleClearOptions";
	const HashString shRuleGenerateSystemMessage = "RuleGenerateSystemMessage";
	const HashString shRuleSelectOption = "RuleSelectOption";
	const HashString shRulePlayCard = "RulePlayCard";
	const HashString shRuleUIAddPlayer = "RuleUIAddPlayer";
	const HashString shRuleUIRemovePlayer = "RuleUIRemovePlayer";
	const HashString shRuleUnselectOption = "RuleUnselectOption";
	const HashString shRuleUpdateOption = "RuleUpdateOption";
	
	//-----------------------------------------------------------------------//
	// Section: wxStrings
	//
	const wxString sw3 = wxT("3");
    const wxString sw10Spaces = wxT("          ");
	const wxString swAddAPlayer = wxT("Add A Player");
	const wxString swAy = wxT("Ay");
	const wxString swCard = wxT("Card");
	const wxString swColon = wxT(":");
    const wxString swConfigSectionUI = wxT("ConfigSectionUI");
	const wxString swDash = wxT("- ");
	const wxString swEmpty = wxT("");
	const wxString swForwardSlash = wxT("/");
	const wxString swGameOptions = wxT("Game Options");
	const wxString swHeight = wxT("height");
	const wxString swHTMLBody = wxT("<HTML><BODY bgcolor=black>");
	const wxString swHTMLBold = wxT("<b>");
	const wxString swHTMLBoldClose = wxT("</b>");
	const wxString swHTMLBreak = wxT("<br>");
	const wxString swHTMLClose = wxT(">");
	const wxString swHTMLCol = wxT("<td>");
	const wxString swHTMLColClose = wxT("</td>");
	const wxString swHTMLFontClose = wxT("</font>");
	const wxString swHTMLFontColor = wxT("<font color=");
	const wxString swHTMLFontFace = wxT("<font face=");
	const wxString swHTMLItalic = wxT("<i>");
	const wxString swHTMLItalicClose = wxT("</i>");
	const wxString swHTMLRow = wxT("<tr>");
	const wxString swHTMLRowClose = wxT("</tr>");
	const wxString swHTMLSize = wxT(" size=");
	const wxString swHTMLUnderline = wxT("<u>");
	const wxString swHTMLUnderlineClose = wxT("</u>");
	const wxString swMap = wxT("Map");
	const wxString swMessageBox = wxT("MessageBox");
	const wxString swMenu = wxT("Menu");
	const wxString swOptions = wxT("options/");
	const wxString swPlayer = wxT("Player");
	const wxString swPlayers = wxT("Players");
	const wxString swPoints = wxT("Points");
	const wxString swPopups = wxT("Popups");
	const wxString swPopupTimeout = wxT("PopupTimeout");
	const wxString swRoot = wxT("root");
	const wxString swSelectAMap = wxT("Select A Map");
	const wxString swSelectResources = wxT("Select Resources");
	const wxString swSelectResourcesSlash = wxT("Select Resources/");
	const wxString swSpectator = wxT("Spectator");
	const wxString swWidth = wxT("width");
	const wxString swX = wxT("x");
	const wxString swXMLFiles = wxT("XML files (*.xml)|*.xml");
	const wxString swY = wxT("y");
	const wxString swYourCards = wxT("Your Cards");

	// Formats
    const wxString swFloat6 = wxT("%.6f");
	const wxString swHTMLColor = wxT(" color=%s");
	const wxString swHTMLColorFormat = wxT("#%X%X%X");
	const wxString swIntFormat = wxT("%d");
	const wxString swSlashStringSlashString = wxT("/%s/%s");

	//-----------------------------------------------------------------------//
	// Section: Translations
	//
	
	// Formats
	
}

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
