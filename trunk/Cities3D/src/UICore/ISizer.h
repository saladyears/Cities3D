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

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: ISizer
// 
// An interface for sizing dialog windows.  ISizer is a workaround
// for the inability to call virtual functions from within a 
// constructor.  All classes that are derived from <wxBaseDialog> must
// have a mechanism to size themselves correctly and layout their objects
// when created.  Ideally, this should happen in the constructor, and not in
// some other abritrary initialization function.  However, it is not
// possible to have a virtual function in the derived dialog classes be called 
// in the constructor of <wxBaseDialog>.  The ISizer interface solves this
// problem.  Each <wxBaseDialog>-derived class must create its own Sizer, 
// derived from ISizer, and pass it down to the <wxBaseDialog> constructor.
// The constructor of <wxBaseDialog> takes an ISizer pointer as
// a parameter, and calls its CreateSizers function.  Since each ISizer must
// be created with the pointer to the dialog it belongs to, CreateSizers
// can then create all the sizing for the dialog while still in the
// base class constructor.
//
// Project:
//     <UICore>
//
// Include:
//     ISizer.h
//
class UI_EXPORT_SPEC ISizer
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: ISizer
	//
	// The ISizer constructor.  The given wxWindow must
	// be the window to be sized by ISizer.
	//
	// Parameters:
	//     window - The wxWindow to be sized in CreateSizers.
	//
	ISizer(wxWindow *window) 
		: mpWindow(window) {wxASSERT(NULL != mpWindow);}

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~ISizer
	//
	// The ISizer destructor.  It is pure virtual because
	// ISizer must be derived from to be used.
	//
	virtual ~ISizer()=0;

	//-----------------------------------------------------------------------//
	// Group: Pure Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: CreateLayout
	//
	// Creates the window layout.  Derived classes must supply this function 
	// to provide the layout needed for their dialog window.
	//
	// Parameters:
	//     sizer - The top level wxBoxSizer.  All subsequent
	//              sizers should be added as children to sizer.
	//
	virtual void CreateLayout(wxBoxSizer *sizer)=0;

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Pointer: mpWindow
	//
	// The wxWindow to be sized.
	//
	wxWindow *mpWindow;
};

//---------------------------- PROTOTYPES       -----------------------------//
inline ISizer::~ISizer() {}

typedef boost::shared_ptr<ISizer> ISizerPtr;
