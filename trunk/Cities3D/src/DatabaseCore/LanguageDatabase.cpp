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
#include "stdwx.h"
#include "style.h"	//READ THIS BEFORE MAKING ANY CHANGES TO THIS FILE!!!

//---------------------------- SYSTEM INCLUDES  -----------------------------//

//---------------------------- USER INCLUDES    -----------------------------//
#include "LanguageDatabase.h"
#include "FilePath.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
LanguageDatabase::LanguageDatabase()
{
	// Search all of the subdirectories in the /locale directory to determine
	// which languages are available.  Note that just the directory existing
	// makes it a selectable language, even if no .mo files exist within that
	// directory.

	wxArrayString localeList = FilePath::EnumDataSubDirs(swLocale);

	for(size_t i = 0; i < localeList.GetCount(); ++i)
	{
		// See if language information exists for this directory name.
		const wxLanguageInfo *info = wxLocale::FindLanguageInfo(localeList[i]);

		// If it exists, add it to the list of available languages.
		if(NULL != info)
		{
			mLanguageInfoArray.push_back(*info);
		}
	}
}
