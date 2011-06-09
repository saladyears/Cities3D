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
#include "UIExport.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class wxBaseListCtrl;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: ILayoutCreator
// 
// An interface for creating <wxBaseListCtrls> for <wxBaseContextMenus>.  
// ILayoutCreator is a workaround for the inability to call virtual functions 
// from within a constructor.  All classes that are derived from 
// <wxBaseContextMenu> must have a <wxBaseListCtrl> that handles their UI.  
// Ideally, this <wxBaseListCtrl> should be created in the constructor, and not 
// in some other abritrary initialization function.  However, it is not
// possible to have a virtual function in the derived context menu classes be 
// called in the constructor of <wxBaseContextMenu>.  The ILayoutCreater 
// interface solves this problem.  Each <wxBaseContextMenu>-derived class must 
// create its own LayoutCreator, derived from ILayoutCreator, and pass it down 
// to the <wxBaseContextMenu> constructor. The constructor of 
// <wxBaseContextMenu> takes an ILayoutCreator pointer as a parameter, and 
// calls its CreateLayout function.
//
// Project:
//     <UICore>
//
// Include:
//     ILayoutCreator.h
//
class UI_EXPORT_SPEC ILayoutCreator
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: ILayoutCreator
	//
	// The ILayoutCreator constructor.
	//
	ILayoutCreator() {}

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~ILayoutCreator
	//
	// The ILayoutCreator destructor.  Base classes must have virtual 
	// destructors.
	//
	virtual ~ILayoutCreator()=0;

	//-----------------------------------------------------------------------//
	// Group: Pure Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: CreateLayout
	//
	// Creates the <wxBaseListCtrl>.
	//
	// Returns:
	//     A pointer to the <wxBaseListCtrl>.
	//
	virtual wxBaseListCtrl *CreateLayout(wxWindow * const window) const=0;
};

//---------------------------- PROTOTYPES       -----------------------------//
inline ILayoutCreator::~ILayoutCreator() {}

typedef boost::shared_ptr<ILayoutCreator> ILayoutCreatorPtr;
