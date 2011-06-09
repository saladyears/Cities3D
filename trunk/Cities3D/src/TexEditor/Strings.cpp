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
	const HashString shHotseat = "Hotseat";
    
	// Display
	const HashString shBackground = "Background";
	const HashString shDefaultFont = "DefaultFont";
	const HashString shDefaultText = "DefaultText";
	const HashString shInfoFont = "InfoFont";
    const HashString shInfoText = "InfoText";
    const HashString shTexListMainFont = "TexListMainFont";
    const HashString shTexListSubFont = "TexListSubFont";
    const HashString shUserChangeableText = "UserChangeableText";

	// Events
	const HashString shEventKeyDown = "eventKeyDown";
    const HashString shEventChangeMainPane = "eventChangeMainPane";
	const HashString shEventSkin = "eventSkin";
	const HashString shEventUncheckMenu = "eventUncheckMenu";
    
	// Logic

	// Rules
	
	//-----------------------------------------------------------------------//
	// Section: wxStrings
	//
    const wxString swEditor = wxT("Editor");
	const wxString swEmpty = wxT("");
    const wxString swFile = wxT("File");
    const wxString swForwardSlash = wxT("/");
    const wxString swImg = wxT("img");
    const wxString swTex = wxT("tex");
    const wxString swTexEditor = wxT("TexEditor");
    const wxString swTexFiles = wxT("TEX files (*.tex)|*.tex");

	// Formats
    const wxString swByteFormat = wxT("%s: %dx%d, %s: %d bytes");
    const wxString swStringStringColon = wxT("%s %s:");

	//-----------------------------------------------------------------------//
	// Section: Translations
	//
    const wxString stDimensions = _("Dimensions");
    const wxString stExport = _("Export");
    const wxString stExtract = _("Extract");
    const wxString stImageEditor = _("Image Editor");
    const wxString stImport = _("Import");
    const wxString stSelectADirectoryToExport = _("Select a directory to export");
    const wxString stSelectADirectoryToExtract = _("Select a directory to extract the files in");
    const wxString stSelectATexFileToImport = _("Select a .tex file to import.");
    const wxString stSize = _("Size");
    
	// Formats

    //-----------------------------------------------------------------------//
	// Section: Rulesets
	//
    const wxString MODULE = wxT("TexEditor");
	const wxString TEXEDITOR = wxT("TexEditor");
}

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
