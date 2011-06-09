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
#include "DatabaseExport.h"
#include "Singleton.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: LanguageDatabase
// 
// A Singleton class that can be used to discover all languages available to
// the application for use with a wxLocale.
//
// Derived From:
//     <Singleton>
//
// Project:
//     <DatabaseCore>
//
// Include:
//     LanguageDatabase.h
//
class DATABASE_EXPORT_SPEC LanguageDatabase 
	: public Singleton<LanguageDatabase>
{
	//---------------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	typedef std::vector<wxLanguageInfo> LanguageInfoArray;

	//-----------------------------------------------------------------------//
	// Function: GetLanguageInfo
	//
	// Returns an array of the languages available to the application.
	//
	// Returns:
	//     A std::vector of wxLanguageInfo.
	//
	const LanguageInfoArray &GetLanguageInfo() const
	{
		return mLanguageInfoArray;
	}

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: LanguageDatabase
	// 
	// The LanguageDatabase constructor. 
	//
	LanguageDatabase();

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Array: mLanguageInfoArray
	//
	// The array of languages available.
	//
	LanguageInfoArray mLanguageInfoArray;

	friend class Singleton<LanguageDatabase>;
};
//---------------------------- PROTOTYPES       -----------------------------//

