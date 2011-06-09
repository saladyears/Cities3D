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
typedef boost::function4<void, const DataObject&, wxInt32&, wxInt32&, HashString&> StatPlayerFunc;
typedef boost::function2<void, const GamePtr&, HashString&> StatDiceRollFunc;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: Stat
// 
// Encapsulates one stat for the <IStatEngine> to keep track of.
//
// Project:
//     <RulesCore>
//
// Include:
//     Stat.h
//
struct Stat
{
	// Whether this Stat is a game stat or a player stat.
	enum Type
	{
		DiceRoll	= 0,
		Player,
        Modifier,
	};

	//-----------------------------------------------------------------------//
	// Group: Constructors
	//
	Stat(const wxString& display, const wxString& englishDisplay, 
		const HashString& name, const HashString& rule, 
		const StatPlayerFunc& func, wxInt32 priority, bool hiddenUntilDone)
		: mDisplay(display)
		, mEnglishDisplay(englishDisplay)
		, mName(name)
		, mPlayerFunc(func)
		, mType(Player)
		, mPriority(priority)
		, mExpected(0.0f)
		, mHiddenUntilDone(hiddenUntilDone)
	{
        mRules.push_back(rule);
    }

	Stat(const wxString& display, const wxString& englishDisplay, 
		const HashString& name, const HashString& rule,
		const StatDiceRollFunc& func, wxInt32 priority, bool hiddenUntilDone)
		: mDisplay(display)
		, mEnglishDisplay(englishDisplay)
		, mName(name)
		, mDiceRollFunc(func)
		, mType(DiceRoll)
		, mPriority(priority)
		, mExpected(0.0f)
		, mHiddenUntilDone(hiddenUntilDone)
	{
        mRules.push_back(rule);
    }

    Stat(const wxString& display, const wxString& englishDisplay, 
		const HashString& name, const HashString& rule, 
		const HashString& modifies, wxInt32 priority)
        : mDisplay(display)
		, mEnglishDisplay(englishDisplay)
		, mName(name)
		, mModifies(modifies)
		, mType(Modifier)
		, mPriority(priority)
		, mExpected(0.0f)
		, mHiddenUntilDone(false)
	{
        mRules.push_back(rule);
    }

	Stat(const wxString& display, const wxString& englishDisplay, 
		const HashString& name, const HashString& rule, 
		const HashString& modifies, wxInt32 priority, float expected)
        : mDisplay(display)
		, mEnglishDisplay(englishDisplay)
		, mName(name)
		, mModifies(modifies)
		, mType(Modifier)
		, mPriority(priority)
		, mExpected(expected)
		, mHiddenUntilDone(false)
	{
        mRules.push_back(rule);
    }

    Stat(const wxString& display, const wxString& englishDisplay, 
		const HashString& name, const HashString& modifies, wxInt32 priority)
        : mDisplay(display)
		, mEnglishDisplay(englishDisplay)
		, mName(name)
        , mModifies(modifies)
		, mType(Modifier)
		, mPriority(priority)
		, mExpected(0.0f)
		, mHiddenUntilDone(false)
	{
    }

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	
	// The display name for this stat.
	wxString mDisplay;

	// The display name in English for this stat.
	wxString mEnglishDisplay;

	// The internal name of this stat.
	HashString mName;

	// The rules this stat parses.
	HashStringArray mRules;

    // The rule, if any, this stat modifies.
    HashString mModifies;

	// The function, for a player stat, that parses the rule's DataObject.
	StatPlayerFunc mPlayerFunc;

	// The function, for a game stat, that parses the rule's DataObject.
	StatDiceRollFunc mDiceRollFunc;

	// The type of the stat (player or game).
	Type mType;

	// The display priority of this stat.
	wxInt32 mPriority;

	// The expected roll percentage of a dice roll.
	float mExpected;

	// A flag that determines if this stat is hidden until after the game is done.
	bool mHiddenUntilDone;
};

//---------------------------- PROTOTYPES       -----------------------------//
