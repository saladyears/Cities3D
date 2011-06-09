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
#include <list>
#include <wx/hashmap.h>

//---------------------------- USER INCLUDES    -----------------------------//
#include "OwnerDrawWindow.h"
#include "BaseUI.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class PlayerElement;
typedef boost::shared_ptr<PlayerElement> PlayerElementPtr;

class DataObject;
class wxSettlersGLCanvas;

class PlayerIcon;
typedef boost::shared_ptr<PlayerIcon> PlayerIconPtr;

class TraitsUI;
typedef boost::shared_ptr<TraitsUI> TraitsUIPtr;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxPlayerUI
// 
// Displays game information for one player.  The wxPlayerUI windows
// start at the top left of the screen and continue down the left
// side.  wxPlayerUIs are the most complicated of all the UI pieces.
// They display the player's current resource cards, current development
// or progress cards, stock items, points from various means, special
// flags, and modify themselves in different modes of the game.  For instance,
// the current player in hotseat mode can see their cards, but when
// the current player changes, the wxPlayerUIs must update themselves so
// that only the new current player's cards are visible.  A wxPlayerUI
// is updated by transmitting a <DataObject> to the eventPlayerUI <Event>
// via the <Controller>.  The transmitted <DataObject> must have the 
// current <Game> as its first parameter, and a wxInt32 as its 
// second parameter.  The wxInt32 flag specifys which player's data needs to be
// updated, or -1 to update all, or -2 to update which player's turn it
// is.
//
// When a wxPlayerUI is initialized, it queries the <IPluginEngine>
// and retrieves all <PlayerData> objects supplied by the currently loaded
// <RuleSets>.  It then takes these pieces of <PlayerData> and builds a layout
// of icons representing cards, stock, and point information from them.  Some
// <PlayerData> objects may require OpenGL rendered bitmaps, namely the stock
// items.  In those cases, the associated <IDrawObjects> are sent to the parent
// <wxSettlersGLCanvas> and converted from OpenGL to a wxBitmap.  The wxPlayerUI
// keeps track of all of these bits of information and handles laying them out
// in a more or less generalized fashion so that different <RuleSets> can add
// new items to the player display windows without difficulty.
//
// The key idea with wxPlayerUI is that it knows nothing about any particular 
// piece of knowledge in the game, making it 100% plugin data.  It just takes 
// the <PlayerData> supplied by the loaded <RuleSets>, groups it by 
// categories, and lays it out in the window.
//
// Derived From:
//     <wxBaseUI>
//
// Project:
//     <Cities3D>
//
// Include:
//     wxPlayerUI.h
//
class wxPlayerUI : public wxBaseUI<wxOwnerDrawWindow>
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxPlayerUI
	//
	// The wxPlayerUI constructor.  Makes the wxPlayerUI visible
	// and adds OnUpdate as a receiver of the eventPlayerUI
	// <Event>.  Adds OnPlayerReorder as a receiver of the
	// eventPlayerReorder <Event>.
	//
	// Parameters:
	//     index - The player index in this UI.
	//     parent - The parent window.  Must not be NULL.
	//     game - The current <Game>.
	//     canvas - The game's OpenGL canvas.
	//
	wxPlayerUI(const wxInt32 index, wxWindow *parent, const GamePtr& game,
		wxSettlersGLCanvas* canvas);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxPlayerUI
	//
	// The wxPlayerUI destructor.  Removes OnUpdate as a receiver
	// of the eventPlayerUI <Event>.
	//
	virtual ~wxPlayerUI();

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: CalculateWidth
	//
	// Recalculates the proper width of the wxPlayerUI.  Takes
	// into account many factors which affect the
	// width.  When finished, sets the Width value
	// in the <TraitsUI> object.
	//
	// Parameters:
	//     overrideShow - Overrides the normal show logic to calculate the
	//                    width as if the whole wxPlayerUI was shown.
	//
	void CalculateWidth(bool overrideShow = false);

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Paint
	//
	// Paints the wxPlayerUI.  Fills in the background color
	// and draws the rounded outline.  Erases overlapping
	// parts when it draws the player tab, and adds separater 
	// lines between sections.
	//
	// Parameters:
	//     dc - The memory dc to draw into.
	//
	virtual void Paint(wxMemoryDC &dc);
	
	//-----------------------------------------------------------------------//
	// Function: Size
	//
	// Handles size events.
	//
	// Parameters:
	//     event - The size event.
	//
	virtual void Size(wxSizeEvent& event);

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: ResizeUI
	//
	// Resizes all the elements in the wxPlayerUI.
	//
	void ResizeUI();

	//-----------------------------------------------------------------------//
	// Group: Game Event Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnUpdate
	//
	// Updates the player UI elements by calling each
	// of their own Update functions.  Each player UI element
	// is responsible for showing/hiding itself and updating
	// its color and counter properly.  Shows and hides
	// various components if required by a change in turn.
	//
	// Parameters:
	//     object - The <DataObject> containing the update information.
	//              Must have a <Game> object as its first parameter, and
	//              a wxInt32 as the second parameter.
	//
	void OnUpdate(const DataObject &object);

	//-----------------------------------------------------------------------//
	// Function: OnPlayerReorder
	//
	// Updates the player UI color and name if and when
	// the players are reordered during the game.
	//
	// Parameters:
	//     game - The current <Game>.
	//
	void OnPlayerReorder(const GamePtr &game);

	//-----------------------------------------------------------------------//
	// Function: OnNewPlayer
	//
	// Updates the name of the player at this seat.
	//
	// Parameters:
	//     object - The <DataObject> containing the <Game> and the index of the
	//              seat to update.
	//
	void OnNewPlayer(const DataObject &object);

	//-----------------------------------------------------------------------//
	// Function: OnResetBitmaps
	//
	// Resets the bitmaps used by the wxPlayerUI.
	//
	void OnResetBitmaps(wxInt32);

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Int: mIndex
	//
	// The index of the player this wxPlayerUI represnets.
	//
	wxInt32 mIndex;

	//-----------------------------------------------------------------------//
	// Int: mCurrentIndex
	//
	// The index of current player.  If it is the same as
	// mIndex, the wxPlayerUI is outlined with the player's color
	// to indicate that it is their turn.
	//
	wxInt32 mCurrentIndex;

	//-----------------------------------------------------------------------//
	// Object: mTextColor
	//
	// The color used to draw this player's text and outline.
	//
	wxColour mTextColor;

	//-----------------------------------------------------------------------//
	// Object: mTraitsUI
	//
	// The <TraitsUI> object for this wxPlayerUI.
	//
	TraitsUIPtr mTraitsUI;

	//-----------------------------------------------------------------------//
	// Pointer: mpNameText
	//
	// The text window for the player's name.
	//
	wxStaticText* mpNameText;

	typedef std::list<wxPoint> PointList;

	//-----------------------------------------------------------------------//
	// Array: mLineList
	//
	// The points in the UI where vertical lines are drawn.
	//
	PointList mLineList;

	//the hash map storing all of the UI elements
	typedef std::list<PlayerElementPtr> PlayerElementList;
	WX_DECLARE_HASH_MAP(wxInt32, PlayerElementList, wxIntegerHash, 
		wxIntegerEqual, ElementHash);

	//-----------------------------------------------------------------------//
	// Hash: mElements
	//
	// The individual elements in the UI.  Stored in an
	// integer hash so that they can be grouped together
	// by category (resources, cards, stock, points, special)
	//
	ElementHash mElements;

	WX_DECLARE_STRING_HASH_MAP(wxBitmap, BitmapHash);

	//-----------------------------------------------------------------------//
	// Array: mIcons
	//
	// The icons that this player should display.
	//
	typedef std::list<PlayerIconPtr> PlayerIconList;
	PlayerIconList mIcons;
};

//---------------------------- PROTOTYPES       -----------------------------//
