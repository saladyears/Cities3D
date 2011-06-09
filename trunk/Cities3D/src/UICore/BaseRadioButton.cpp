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
#include "BaseRadioButton.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
wxBaseRadioButton::wxBaseRadioButton(wxWindow* parent, wxWindowID id, 
									 const wxString& label, 
									 const HashString &color, 
									 const HashString &font, const wxPoint& pos, 
									 const wxSize& size, long style)
: wxRadioButton(parent, id, label, pos, size, style)
, BaseSkinMixin<wxBaseRadioButton>(color, font, shInputBackground)
{
#ifdef __WIN32__
	if ( wxApp::GetComCtl32Version() >= 600 )
    {
		wxLogNull nolog;

		typedef boost::scoped_ptr<wxDynamicLibrary> DynamicLibraryPtr;

		DynamicLibraryPtr lib(new wxDynamicLibrary);
		if(true == lib->Load(wxT("uxtheme.dll")))
		{
			typedef HRESULT (__stdcall *PFNWXUSETWINDOWTHEME)(WXHWND, 
				const wchar_t*, const wchar_t *) ;

			PFNWXUSETWINDOWTHEME func = (PFNWXUSETWINDOWTHEME) 
				lib->GetSymbol(_T("SetWindowTheme"));
			if(func)
			{
				func((HWND) GetHWND(), LPCWSTR(" "), LPCWSTR(" "));
			}
		}
	}
#endif

	OnSkin();
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//


