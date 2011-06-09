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
#pragma once

#include "style.h"	//READ THIS BEFORE MAKING ANY CHANGES TO THIS FILE!!!
//---------------------------- SYSTEM INCLUDES  -----------------------------//

//---------------------------- USER INCLUDES    -----------------------------//

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class wxSettlersGLCanvas;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxPlayerUIPanel
// 
// Panel class that holds all PlayerUIs.
//
// Derived From:
//     wxPanel
//
// Project:
//     <CoreRules>
//
// Include:
//     PlayerUIPanel.h
//
class wxPlayerUIPanel : public wxPanel
{
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxPlayerUIPanel
	//
	// The wxPlayerUIPanel constructor.
	//
	// Parameters:
	//     parent - The parent window.
	//     canvas - The game's OpenGL canvas.
	//     game - The current <Game>.
	//
public:
	wxPlayerUIPanel(wxWindow* parent, wxSettlersGLCanvas* canvas, 
		const GamePtr& game);
};

//---------------------------- PROTOTYPES       -----------------------------//

