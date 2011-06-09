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

//---------------------------- USER INCLUDES    -----------------------------//
#include "BasePopup.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: BuildPopup
// 
// Provides a popup displaying build costs.
//
// Derived From:
//     <BasePopup>
//
// Project:
//     <CoreRules>
//
// Include:
//     BuildPopup.h
//
class BuildPopup : public BasePopup
{
public:
	BuildPopup(wxWindow* thisWindow);

	virtual void CreatePopup(wxWindow* window, wxSizer* sizer) const;

	void SetText(const wxString& text)
	{
		mText = text;
	}

	void SetLogicCost(const HashString& cost)
	{
		mLogicCost = cost;
	}

	void SetKeyEvent(const HashString& event)
	{
		mKeyEvent = event;
	}

private:
	typedef std::map<HashString, wxInt32> ResourceMap;
	typedef std::map<wxInt32, HashString> OrderMap;
	
	OrderMap mOrderMap;

	wxString mText;
	HashString mLogicCost;
	HashString mKeyEvent;
};

//---------------------------- PROTOTYPES       -----------------------------//
