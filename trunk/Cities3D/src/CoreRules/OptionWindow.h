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

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxOptionWindow
// 
// Still volatile.
//
// Derived From:
//     wxWindow (see <http://www.wxwindows.org/manuals/2.4.2/wx411.htm>)
//
// Project:
//     <Cities3D>
//
// Include:
//     OptionWindow.h
//
class wxOptionWindow : public wxWindow
{
public:
	wxOptionWindow(wxWindow* parent, const wxString &key);

	virtual bool Show(bool show = true);

	const wxString &GetKey() const {return mKey;}

private:
	wxString mKey;
};

//---------------------------- PROTOTYPES       -----------------------------//
