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
class wxInteractiveImageStrip;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: IResourceLogic
// 
// Abstract base class that links <wxInteractiveImageStrips> with logic.
//
// Project:
//     <Cities3D>
//
// Include:
//     IResourceLogic.h
//
class UI_EXPORT_SPEC IResourceLogic
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Pure Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Initialize
	//
	// Resets all <wxInteractiveImageStrips>.
	//
	virtual void Initialize()=0;

	//-----------------------------------------------------------------------//
	// Function: AllowDrop
	//
	// Returns whether the target <wxInteractiveImageStrip> may have the given
	// resource dropped on it at the given coordinates.
	//
	// Parameters:
	//     target - The <wxInteractiveImageStrip> drop target.
	//     x - The x coord of the drop.
	//     y - The y coord of the drop.
	//     res - The resource to drop.
	//
	// Returns:
	//     A bool that is true if the drop is allowed and false if not.
	//
	virtual bool AllowDrop(wxInteractiveImageStrip* target, wxCoord x, 
		wxCoord y, const HashString& res)=0;

	//-----------------------------------------------------------------------//
	// Function: ExecuteDrop
	//
	// Executes a drop of the given resource onto the target 
	// <wxInteractiveImageStrip> at the given coordinates.
	//
	// Parameters:
	//     target - The <wxInteractiveImageStrip> drop target.
	//     x - The x coord of the drop.
	//     y - The y coord of the drop.
	//     res - The resource to drop.
	//
	virtual void ExecuteDrop(wxInteractiveImageStrip* target, wxCoord x, 
		wxCoord y, const HashString& res)=0;

	//-----------------------------------------------------------------------//
	// Function: HandleLeftClick
	//
	// Handles a left-double-click of the given resource from the origin 
	// <wxInteractiveImageStrip>.
	//
	// Parameters:
	//     origin - The origin <wxInteractiveImageStrip>.
	//     res - The resource to handle.
	//
	virtual void HandleLeftClick(wxInteractiveImageStrip* origin,
		const HashString& res)=0;

	//-----------------------------------------------------------------------//
	// Function: HandleRightClick
	//
	// Handles a right-double-click of the given resource from the origin 
	// <wxInteractiveImageStrip>.
	//
	// Parameters:
	//     origin - The origin <wxInteractiveImageStrip>.
	//     res - The resource to handle.
	//
	virtual void HandleRightClick(wxInteractiveImageStrip* origin, 
		const HashString& res)=0;

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~IResourceLogic
	//
	// The IResourceLogic destructor.
	//
	virtual ~IResourceLogic() {}
};

//---------------------------- PROTOTYPES       -----------------------------//

