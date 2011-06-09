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

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
    // Force all strings to be statically initialized first.
#ifdef __WIN32__
    #pragma warning(disable:4073)
    #pragma init_seg(lib)
    #pragma warning(default:4073)
#endif

    // All strings used in this project.

    //-----------------------------------------------------------------------//
	// Section: HashStrings
	//
    const HashString shEmpty = "";

    // Data

    // Display
	const HashString shBackground = "Background";
	const HashString shDefaultFont = "DefaultFont";
	const HashString shDefaultText = "DefaultText";
	
    // Events
	const HashString shEventSkin = "eventSkin";

    // Logic

    // Rules

    //-----------------------------------------------------------------------//
	// Section: wxStrings
	//
    const wxString sw127001 = wxT("127.0.0.1");
    const wxString sw1921680 = wxT("192.168.0.");
    const wxString sw1921681 = wxT("192.168.1.");
    const wxString swCities3D = wxT("Cities3D");
    const wxString swCities3DDExe = wxT("Cities3DD.exe");
    const wxString swCities3DExe = wxT("Cities3D.exe");
    const wxString swCitiesOnline = wxT("Cities Online");
    const wxString swCitiesOnlineNoSpaces = wxT("CitiesOnline");
    const wxString swCitiesOnlineTCP = wxT("Cities Online TCP");
    const wxString swColon = wxT(":");
    const wxString swDash = wxT("-");
    const wxString swEnterYourPassword = wxT("Enter Your Password");
    const wxString swGtkrc = wxT("gtkrc");
    const wxString swNoAdapter = wxT("no_adapter");
    const wxString swPercent20 = wxT("%20");
    const wxString swPercent22 = wxT("%22");
    const wxString swProtocol = wxT("citiesonline://");
    const wxString swQuotation = wxT("\"");
    const wxString swShell_TrayWnd = wxT("Shell_TrayWnd");
    const wxString swSpace = wxT(" ");
    const wxString swSysPager = wxT("SysPager");
    const wxString swTCP = wxT("TCP");
    const wxString swToolbarWindow32 = wxT("ToolbarWindow32");
    const wxString swTrayNotifyWnd = wxT("TrayNotifyWnd");
	
    // Formats
    const wxString swC1 = wxT("?command=%s&player=%s&session=%s&port=%d");
    const wxString swConfigSectionGeneral = wxT("ConfigSectionGeneral");
    const wxString swEmpty = wxT("");
    const wxString swFloat6 = wxT("%.6f");
    const wxString swIntFormat = wxT("%d");
    const wxString swLanguage = wxT("Language");
    const wxString swSlashStringSlashString = wxT("/%s/%s");
    const wxString swSynchronizeNys = wxT("synchronize_%s.nys");
    const wxString swSynchronizeSyn = wxT("synchronize_%s.syn");
    const wxString swVersion = wxT("%d.%d.%d");
    
    //-----------------------------------------------------------------------//
	// Section: Translations
	//

    // Formats

}

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
