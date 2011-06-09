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
#include "CoreGLCanvas.h"
#include "BaseUI.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class KnightCount;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxBarbarianUI
// 
// A class that shows the barbarian tile and ship in OpenGL.
//
// Derived From:
//     <wxBaseUI>
//
// Project:
//     <GLCore>
//
// Include:
//     BarbarianGLCanvas.h
//
class wxBarbarianUI : public wxBaseUI<wxCoreGLCanvas>
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxBarbarianUI
	// 
	// The wxBarbarianUI constructor.
	// 
	// Parameters:
	//     parent - The parent window (must not be NULL).
	//     game - The current game object.
	//     size - The starting size.
	//
	wxBarbarianUI(wxWindow *parent, const GamePtr& game, 
		const wxSize& size = wxDefaultSize);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxBarbarianUI
	// 
	// The wxBarbarianUI destructor.
	//
	virtual ~wxBarbarianUI();

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Macro: DECLARE_EVENT_TABLE
	//
	// The wxWidgets event table declaration.
	//
	DECLARE_EVENT_TABLE()

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//
	void DrawScene();

	void Animate();

	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Paint
	//
	// Paints the window.
	// 
	virtual void Paint();

	//-----------------------------------------------------------------------//
	// Group: Game Event Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnKnightCounts
	//
	// Handles knight counts being updated.
	//
	// Parameters:
	//     game - The <Game> data.
	//
	void OnKnightCounts(const GamePtr& game);

	//-----------------------------------------------------------------------//
	// Function: OnPlayerReorder
	//
	// Handles the players being reordered.
	//
	// Parameters:
	//     game - The current <Game>.
	//
	void OnPlayerReorder(const GamePtr& game);

	void OnBarbarianAdvance(const GamePtr& game);

	void OnImageUpdate(bool);

	//-----------------------------------------------------------------------//
	// Function: OnTabBarbarians
	//
	// Handles a raise tab event.
	//
	void OnTabBarbarians(wxInt32);

	//-----------------------------------------------------------------------//
	// Group: Event Message Handling
	//

	//-----------------------------------------------------------------------//
	// Function: OnSize
	//
	// Handles a window resize.
	//
	void OnSize(wxSizeEvent &event);

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Array: mKnightCounts
	//
	// The array of <KnightCount> objects.
	//
	typedef std::vector<KnightCount*> KnightCountArray;
	KnightCountArray mKnightCounts;

	size_t mAnimateDepth;
	size_t mAnimatePos;
	wxLongLong mAnimationStart;

};

//---------------------------- PROTOTYPES       -----------------------------//
