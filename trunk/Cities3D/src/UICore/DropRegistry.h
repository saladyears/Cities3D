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
#include <set>

//---------------------------- USER INCLUDES    -----------------------------//

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//
template <typename T> class DropTarget;
template <typename T> class DropSource;

//---------------------------------------------------------------------------//
// Class: DropRegistry
// 
// A repository for <DropTargets> to make themselves available for drag and
// drop operations.
//
// Project:
//     <Cities3D>
//
// Include:
//     DropRegistry.h
//
template <typename T>
class DropRegistry
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Static Functions
	//

	//-----------------------------------------------------------------------//
	// Function: RegisterDropTarget
	//
	// Registers the <DropTarget> for use by DropSources.
	//
	// Parameters:
	//     target - The target to register.
	//
	static void RegisterDropTarget(DropTarget<T>* target)
	{
		msDropTargets.insert(target);
	}

	//-----------------------------------------------------------------------//
	// Function: UnregisterDropTarget
	//
	// Unregisters the <DropTarget>
	//
	// Parameters:
	//     target - The target to unregister.
	//
	static void UnregisterDropTarget(DropTarget<T>* target)
	{
		typename DropTargetSet::iterator it = msDropTargets.find(target);
		if(msDropTargets.end() != it)
		{
			msDropTargets.erase(it);
		}
	}

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Array: mDropTargets 
	//
	// The set of possible <DropTargets>.
	//
	typedef typename std::set<DropTarget<T> *> DropTargetSet;
	static DropTargetSet msDropTargets;

	// This friend declaration avoids some nasty circular header includes
	// that would be unavoidable without it.
	friend class DropSource<T>;
};

//---------------------------- PROTOTYPES       -----------------------------//
template <typename T> typename
DropRegistry<T>::DropTargetSet DropRegistry<T>::msDropTargets;
