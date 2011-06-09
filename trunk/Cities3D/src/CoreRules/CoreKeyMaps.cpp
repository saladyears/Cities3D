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
#include "RuleSetDatabase.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

// Controls.
IMPLEMENT_KEYMAP(CORE, stUndoAction,		ControlsUndo,		'Z',		wxMOD_CONTROL);
IMPLEMENT_KEYMAP(CORE, stTradeWithOthers,	ControlsTrade,		'T',		wxMOD_NONE);
IMPLEMENT_KEYMAP(CORE, stTradeWithBank,		ControlsBank,		'B',		wxMOD_NONE);
IMPLEMENT_KEYMAP(CORE, stPlayACard,			ControlsPlayCard,	'C',		wxMOD_NONE);
IMPLEMENT_KEYMAP(CORE, stRollTheDice,	    ControlsRollDice,	'R',		wxMOD_NONE);
IMPLEMENT_KEYMAP(CORE, stEndTheTurn,	    ControlsEndTurn,	'E',		wxMOD_NONE);

// Map manipulation.
IMPLEMENT_KEYMAP(CORE, stZoomIntoMap,		MapZoomIn,		WXK_UP,		wxMOD_CONTROL);
IMPLEMENT_KEYMAP(CORE, stZoomOutOfMap,		MapZoomOut,		WXK_DOWN,	wxMOD_CONTROL);
IMPLEMENT_KEYMAP(CORE, stPanMapLeft,		MapPanLeft,		WXK_LEFT,	wxMOD_SHIFT);
IMPLEMENT_KEYMAP(CORE, stPanMapRight,		MapPanRight,	WXK_RIGHT,	wxMOD_SHIFT);
IMPLEMENT_KEYMAP(CORE, stPanMapUp,			MapPanUp,		WXK_UP,		wxMOD_SHIFT);
IMPLEMENT_KEYMAP(CORE, stPanMapDown,		MapPanDown,		WXK_DOWN,	wxMOD_SHIFT);
IMPLEMENT_KEYMAP(CORE, stRotateMapLeft,		MapRotateLeft,	WXK_LEFT,	wxMOD_NONE);
IMPLEMENT_KEYMAP(CORE, stRotateMapRight,	MapRotateRight,	WXK_RIGHT,	wxMOD_NONE);
IMPLEMENT_KEYMAP(CORE, stRotateMapUp,		MapRotateUp,	WXK_UP,		wxMOD_NONE);
IMPLEMENT_KEYMAP(CORE, stRotateMapDown,		MapRotateDown,	WXK_DOWN,	wxMOD_NONE);

// Popups.
IMPLEMENT_KEYMAP(CORE, stTogglePopups,      TogglePopups,   'P',    wxMOD_NONE);

// Fullscreen.
IMPLEMENT_KEYMAP(CORE, stToggleFullscreen,  ToggleFullscreen,   WXK_F11,    wxMOD_NONE);

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
