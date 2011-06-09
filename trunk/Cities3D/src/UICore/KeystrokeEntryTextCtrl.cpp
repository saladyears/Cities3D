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
#include "KeystrokeEntryTextCtrl.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
wxKeystrokeEntryTextCtrl::wxKeystrokeEntryTextCtrl(wxWindow* parent, wxWindowID id, 
    IKeystrokeHandler* keystrokeHandler, const wxPoint& pos, const wxSize& size, 
    long style, const HashString &color, const HashString &font)
: wxBaseTextCtrl(parent, id, pos, size, style, color, font)
, mKeystrokeHandler(keystrokeHandler)
{
    wxASSERT(NULL != mKeystrokeHandler);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
// Intentionally bypassing base class.
BEGIN_EVENT_TABLE(wxKeystrokeEntryTextCtrl, wxTextCtrl)
    EVT_KEY_DOWN(wxKeystrokeEntryTextCtrl::OnKeyDown)
    EVT_KEY_UP(wxKeystrokeEntryTextCtrl::OnKeyUp)
END_EVENT_TABLE()

void wxKeystrokeEntryTextCtrl::OnKeyDown(wxKeyEvent& event)
{
    // Notify the parent.
    mKeystrokeHandler->HandleKeyEvent(event, true);
}

void wxKeystrokeEntryTextCtrl::OnKeyUp(wxKeyEvent& event)
{
    // Notify the parent.
    mKeystrokeHandler->HandleKeyEvent(event, false);
}
