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
#include "GameEngine.h"
#include "IUIEngine.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxBaseUI
// 
// A base class for all BuildUI.  Templated because some BuildUI need to
// derive from wxPanel, some from wxWindow.
//
// Derived From:
//     wxPanel
//     wxWindow
//
// Project:
//     <UICore>
//
// Include:
//     wxBaseUI.h
//
template <typename Base = wxWindow>
class wxBaseUI : public Base
{
	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxBaseUI
	//
	// The wxBaseUI constructor.  This works for both wxWindow and wxPanel.
	//
	// Parameters:
	//     parent - The parent window.
	//     size - The initial size.
	//
	wxBaseUI(wxWindow* parent, const wxSize& size)
		: Base(parent, wxID_ANY, wxDefaultPosition, size, 0)
	{
		GUI.RegisterUI(this);
	}

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxBaseUI
	//
	// The wxBaseUI destructor.
	//
	virtual ~wxBaseUI()
	{
		const std::type_info &info(typeid(this));
		HashString str = info.name();

		wxLogDebug(wxT("Unregistering %s."), str.cwx_str());
		GUI.UnregisterUI(this);
	}
};

//---------------------------- PROTOTYPES       -----------------------------//

