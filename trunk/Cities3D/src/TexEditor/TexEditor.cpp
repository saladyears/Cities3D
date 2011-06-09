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
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "RuleSetDatabase.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- PUBLIC           -----------------------------//
#ifdef __WIN32__
BOOL APIENTRY DllMain(HANDLE, DWORD, LPVOID)
{
    return TRUE;
}
#endif

//---------------------------------------------------------------------------//
// Topic: TexEditor
// 

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __WIN32__
__declspec(dllexport)
#endif
const RuleSetMap& ProvideRuleSets()
{
	return RuleSetDatabase::get().map();
}

#ifdef __WIN32__
__declspec(dllexport)
#endif
void ProvideModuleInfo(wxString& module, wxInt32& priority)
{
	module = swTexEditor;
	priority = 5;
}

#ifdef __cplusplus
}
#endif

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//

