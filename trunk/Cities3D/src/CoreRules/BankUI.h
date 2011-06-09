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
#include <map>
#include <set>

//---------------------------- USER INCLUDES    -----------------------------//
#include "BaseUI.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class IDrawObject;
typedef boost::shared_ptr<IDrawObject> DrawObjectPtr;

class DataObject;
class wxSettlersGLCanvas;
class wxBankTextPanel;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxBankUI
// 
// Displays stacks of remaining bank cards. wxBankUI sits in the
// lower-left of the screen and shows the user how much of everything is
// left in the bank. On screen, the stacks of bank cards are displayed 
// as a series of wxStaticBitmap images, though the bank stack 
// images themselves are created by rendering them in OpenGL and 
// then converting them to a wxBitmap.  The bank stacks are updated 
// by transmitting a <DataObject> to the eventPlayerUI <Event>, since 
// anything that affects player data may also affect bank data, making it
// so there is no need for a separate bank update <Event>.  
// The transmitted <DataObject> in the eventPlayerUI <Event> must have
// the current <Game> object as its first parameter.
//
// When wxBankUI is initialized, it queries the <IPluginEngine>
// and retrieves all <BankData> objects supplied by the currently loaded
// <RuleSets>.  It then takes those pieces of <BankData> and creates a set
// of <IDrawObject>-derived objects, each of which is responsible for drawing
// a stack of cards using OpenGL.  wxBankUI then maintains a link between each
// wxStaticBitmap and the <IDrawObject> that creates the bitmap for it.  
// Whenever the eventPlayerUI <Event> is triggered, each <IDrawObject> is sent 
// in to the parent <wxSettlersGLCanvas> and converted from OpenGL to a 
// wxBitmap. This bitmap is then set as the wxStaticBitmaps image, allowing 
// the user to always see exactly how many cards are in one stack compared to 
// the others.
//
// The key idea with wxBankUI is that it knows nothing about any particular 
// cards, making it 100% plugin data.  It just takes the <BankData> supplied 
// by the loaded <RuleSets> and turns it into OpenGL-rendered stacks of cards.
//
// Derived From:
//     <wxBaseUI>
//
// Project:
//     <Cities3D>
//
// Include:
//     wxBankUI.h
//
class wxBankUI : public wxBaseUI<wxPanel>
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxBankUI
	//
	// The wxBankUI constructor.  
	//
	// Parameters:
	//     parent - The parent window.  Must not be NULL.
	//     game - The current <Game>.
	//     canvas - The game's OpenGL canvas.
	//
	wxBankUI(wxWindow *parent, const GamePtr& game, wxSettlersGLCanvas* canvas);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxBankUI
	//
	// The wxBankUI destructor.
	//
	virtual ~wxBankUI();

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Game Event Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnRenderBitmaps
	//
	// Updates the display to show either text or images depending on what
	// the user has selected.
	//
	void OnRenderBitmaps(wxInt32 skipLayout);

	//-----------------------------------------------------------------------//
	// Group: Variables
	//
	
	typedef std::vector<wxStaticBitmap*> StaticBitmapArray;
	typedef std::vector<wxBankTextPanel*> BankTextPanelArray;

	//-----------------------------------------------------------------------//
	// Object: mStaticBitmapArray
	//
	// Keeps track of the bitmaps we use in the display.
	//
	StaticBitmapArray mStaticBitmapArray;

	//-----------------------------------------------------------------------//
	// Object: mBankTextPanelArray
	//
	// Keeps track of the texts we use in the display.
	//
	BankTextPanelArray mBankTextPanelArray;
};

//---------------------------- PROTOTYPES       -----------------------------//

