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
#include "Rule.h"
#include "IUndoable.h"
#include "RuleSetDatabase.h"
#include "Controller.h"
#include "HTML.h"
#include "NetworkPlayers.h"
#include "INetworkEngine.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: NetworkRuleChatMessage
// 
// Formats a text message for display in an HTML chat window and sends it on.
//
// Derived From:
//     <Rule>
//
// Project:
//     <NetworkRules>
//
// RuleSet:
//     <Network>
//
// Transmits To:
//     eventChatMessage
//
class NetworkRuleChatMessage 
	: public Rule
	, public IUndoable
{
public:
	virtual void Execute(const DataObject &object)
	{
		const Player &player = object.read<Player>();
		const wxString &text = object.read<wxString>(1);
		const wxDateTime &time = object.read<wxDateTime>(2);

		// Create the timestamp text.
		wxString timestamp = swBracketLeft + time.FormatISOTime() + swBracketRightSpace;

		// Determine what color should be used, if any.
		DataObject output;
		RULE.Decide(shNetworkLogicPlayerColor, object, output);
		ColorType color = output.read<ColorType>();

		// If they aren't in the game yet, just make them a grey.
		bool inGame = (color != CR_SIZE);
		
		// Format the name part of things.
		wxString str = player.Name();
		str += wxT(": ");

		// Add bold if they are in the game.
		if(true == inGame)
		{
			HTML::Bold(str);
		}

		// Format the text message part of things.
		str += text;

		// Wrap it in the appropriate color.
		if(CR_SIZE != color)
		{
			HTML::Color(str, color);
		}

		// Wrap it in the appropriate font size and add the line break.
		HTML::Font(
				str, shChatMessageText, shChatMessageText, shChatMessageFont);
		HTML::Font(timestamp, shChatMessageTimestamp, shChatMessageTimestamp, 
			shChatMessageFont);
		str = timestamp + str;

		HTML::Break(str);			

		// Play the appropriate sound if this is coming from another player.
		if(player != NET.Players().GetThisPlayer())
		{
			RULE.Execute(shRulePlaySound, DataObject(SOUND_CHAT_MESSAGE));
		}

		// Finally, send the message up to the UI.
		Controller::get().Transmit(shEventChatMessage, 
			DataObject(str));
	}

	virtual bool CanUndo() const
	{
		// Since chat messages come at any time in network games, they
		// would normally turn off any rule that could be undone, so just
		// return whether undo is currently available.
		return RULE.PotentialUndo() || RULE.CanUndo();
	}

	virtual void Unexecute(const DataObject &)
	{
		// If it's possible that we can undo the system message, just let the
		// undo continue to the next rule.
		RULE.Undo();
	}
};

IMPLEMENT_RULE(NetworkRuleChatMessage, NETWORK)
