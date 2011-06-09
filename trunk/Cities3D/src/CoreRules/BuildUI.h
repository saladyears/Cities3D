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
#include "BuildPopup.h"
#include "OwnerDrawWindow.h"
#include "BaseUI.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class DataObject;

class BuildBitmap;
typedef boost::shared_ptr<BuildBitmap> BuildBitmapPtr;

struct BuildData;
typedef boost::shared_ptr<BuildData> BuildDataPtr;

class wxBuildBitmap;
class wxPopupBitmap;
class wxSettlersGLCanvas;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxBuildUI
// 
// Displays a possible action for the player.  wxBuildUIs sit
// at the top of the screen just below the <ControlsUI> and
// <MessageUI>.  Each wxBuildUI represents one type of object the
// player can build or buy (such as stock items or dev cards), 
// or an action the user can trigger (such as moving a ship or knight).
// A bitmap is displayed in the wxBuildUI representing the action the
// user can take with that wxBuildUI.
//
// Each wxBuildUI is created with a piece of <BuildData>, located in the
// loaded <RuleSets>, that tells the wxBuildUI how to draw the object
// it handles, what <ILogic> updates it, and what <IRule> to call when
// the user clicks on the wxBuildUI to build, buy, or perform the action.
// wxBuildUI can be updated by transmitting a <DataObject> to the eventBuildUI
// <Event> via the <Controller>.  The transmitted <DataObject> must
// have a bool as its first parameter, and the current <Game> object as
// its second parameter.  If the bool value is false, the Build hides itself; 
// if the bool value is true, the wxBuildUI calls its internal <ILogic> to 
// see if can be shown.  If it can be shown, the color of the displayed
// bitmap is updated based on the current player in the <Game> object.  If
// the player clicks the visible wxBuildUI, the wxBuildUI calls the <RuleEngine>
// with its internal <IRule> to execute the player's action.
//
// Derived From:
//     <wxOwnerDrawWindow>
//
// Project:
//     <Cities3D>
//
// Include:
//     wxBuildUI.h
//
class wxBuildUI 
	: public wxBaseUI<wxOwnerDrawWindow>
	, public BuildPopup
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxBuildUI
	//
	// The wxBuildUI constructor.  Stores the <IRule> and <ILogic>
	// names to use.  Creates the bitmap handler for display. 
	// Adds OnUpdate as a receiver of the eventBuildUI <Event>.  
	//
	// Parameters:
	//     parent - The parent window.  Must not be NULL.
	//     build - The <BuildData> used to construct the wxBuildUI.
	//	   canvas - The game's OpenGL canvas.  
	//
	wxBuildUI(wxWindow* parent, const BuildDataPtr &build, 
		wxSettlersGLCanvas* canvas);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxBuildUI
	//
	// The wxBuildUI destructor.  Removes OnUpdate as a receiver
	// of the eventBuildUI <Event>.
	//
	~wxBuildUI();

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

	//-----------------------------------------------------------------------//
	// Function: CreateDisabledBitmap
	//
	// Creates the disabled bitmap from the enabled version.
	//
	void CreateDisabledBitmap();

	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Paint
	//
	// Paints the wxBuildUI.  Fills in the background color
	// and draws the rounded outline.
	//
	// Parameters:
	//     dc - The memory dc to draw into.
	//
	virtual void Paint(wxMemoryDC &dc);

	//-----------------------------------------------------------------------//
	// Function: Size
	//
	// Resizes the bitmaps for the window on a size event.
	//
	// Parameters:
	//     event - The resize event.
	//
	virtual void Size(wxSizeEvent & event);

	//-----------------------------------------------------------------------//
	// Group: Game Event Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnUpdate
	//
	// Updates the wxBuildUI.  Triggered by the eventBuildUI
	// <Event>.  The given <DataObject> must have a bool as its
	// first parameter.  If the bool is true, the wxBuildUI calls
	// its internal <ILogic> to see if it can be shown on screen.
	// If so, updates the color of its bitmap and redisplays it,
	// then shows itself.
	//
	// Parameters:
	//     object - The <DataObject> containing the update information.
	//              Must have a bool as its first parameter and the
	//              current <Game> object as its second.
	//
	void OnUpdate(const DataObject &object);

	//-----------------------------------------------------------------------//
	// Function: OnShutdownUI
	//
	// Hides the UI when a global shutdown occurs.
	//
	void OnShutdownUI(wxInt32);

	//-----------------------------------------------------------------------//
	// Function: OnResetBitmaps
	//
	// Resets the bitmaps used by the wxBuildUI.
	//
	void OnResetBitmaps(wxInt32);

	//-----------------------------------------------------------------------//
	// Function: OnKeyEvent
	//
	// Handles a key map event.
	//
	void OnKeyEvent(wxInt32);

	//-----------------------------------------------------------------------//
	// Group: Event Message Handling
	//

	//-----------------------------------------------------------------------//
	// Function: OnButton
	//
	// Handles clicking the displayedbitmap.  Calls the <RuleEngine>
	// with the internal <IRule>.
	//
	// Parameters:
	//     event - The button click event.
	//
	void OnButton(wxCommandEvent &event);

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Int: mLastIndex
	//
	// The index of the most recently current player when the wxBuildUI
	// was last updated.  Used as an optimization to avoid recreating
	// the displayed bitmap every time OnUpdate is called.
	//
	wxInt32 mLastIndex;

	//-----------------------------------------------------------------------//
	// Int: mButtonID
	//
	// The internal wxWidgets ID of the displayed bitmap.  Used
	// as a workaround to wxWidgets not allowing the same ID to
	// be used for all bitmaps in Release mode.
	//
	wxInt32 mButtonID;

	//-----------------------------------------------------------------------//
	// String: mRule
	//
	// The name of the <IRule> to be called when the wxBuildUI is
	// clicked.
	//
	HashString mRule;

	//-----------------------------------------------------------------------//
	// String: mLogic
	//
	// The name of the <ILogic> that determines if the wxBuildUI
	// should be visible.
	//
	HashString mLogic;

	//-----------------------------------------------------------------------//
	// String: mLegend
	//
	// The legend text for the wxBuildUI.
	//
	wxString mLegend;

	//-----------------------------------------------------------------------//
	// String: mLogicCost
	//
	// The logic that determines the cost of the wxBuildUI.
	//
	HashString mLogicCost;

	//-----------------------------------------------------------------------//
	// String: mKeyEvent
	//
	// The key map event that triggers this wxBuildUI.
	//
	HashString mKeyEvent;

	//-----------------------------------------------------------------------//
	// Bool: mClickable
	//
	// A flag that is true if the wxBuildUI can be clicked and false if not.
	//
	bool mClickable;

	wxBitmap mEnabledBitmap;
	wxBitmap mDisabledBitmap;

	//-----------------------------------------------------------------------//
	// Object: mBitmap
	//
	// The displayed bitmap of the object the wxBuildUI represents.
	//
	boost::shared_ptr<wxBuildBitmap> mBitmap;

	//-----------------------------------------------------------------------//
	// Object: mBuildBitmap
	//
	// The bitmap helper that creates the displayed bitmap based
	// on the original <BuildData> the wxBuildUI was created with.
	//
	BuildBitmapPtr mBuildBitmap;
};

//---------------------------- PROTOTYPES       -----------------------------//


