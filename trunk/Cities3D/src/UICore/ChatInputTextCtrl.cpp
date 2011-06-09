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
#include "ChatInputTextCtrl.h"
#include "GameEngine.h"
#include "DataObject.h"
#include "Player.h"
#include "Controller.h"
#include "NetworkPlayers.h"
#include "INetworkEngine.h"
#include "IRuleEngine.h"
#include "IKeyMapEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
wxChatInputTextCtrl::wxChatInputTextCtrl(wxWindow* parent, wxWindowID id, 
	const wxPoint& pos, const wxSize& size)
: wxBaseTextCtrl(parent, id, pos, size, wxTE_PROCESS_ENTER, shChatEntryText,
				 shChatEntryFont)
{
	SetMaxLength(256);

	Controller::get().AddReceiver(shEventKeyDown, 
		&wxChatInputTextCtrl::OnKeyDown, this);
}

wxChatInputTextCtrl::~wxChatInputTextCtrl()
{
	Controller::get().RemoveReceiver(shEventKeyDown, 
		&wxChatInputTextCtrl::OnKeyDown, this);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxChatInputTextCtrl, wxBaseTextCtrl)
    EVT_KEY_DOWN(wxChatInputTextCtrl::KeyDown)
END_EVENT_TABLE()

void wxChatInputTextCtrl::KeyDown(wxKeyEvent& event)
{
    KEYMAP.IgnoreNextEvent();

    event.Skip();
}

void wxChatInputTextCtrl::OnKeyDown(const wxKeyEvent &event)
{
	wxUint32 c = event.GetKeyCode();

	// The chat window receives listens to all keyboard input.  If we don't 
	// have the focus and we get a Return event, set the focus, otherwise,
	// send a chat message.
	if( (WXK_RETURN == c) ||
		(WXK_NUMPAD_ENTER == c))
	{
		if(this != wxWindow::FindFocus())
		{
			SetFocus();
		}
		else
		{
			// We got a chat message, send it out (if it is non-empty).
			wxString str = GetValue();
			Clear();

			if(false == str.IsEmpty())
			{
				RULE.Execute(shNetworkRuleChatMessage, DataObject(
					NET.Players().GetThisPlayer(), str, wxDateTime::Now()));
			}
		}
	}
}
