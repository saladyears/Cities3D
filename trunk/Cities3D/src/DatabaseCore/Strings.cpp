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

	// Data
	const HashString shBuild = "Build";
	const HashString shNoMask = "no_mask";
	
	// Display
	const HashString shBackground = "Background";
    const HashString shDefaultFont = "DefaultFont";
	const HashString shIconTransparent = "IconTransparent";
    
	// Events

	// Logic

	// Rules
	
	//-----------------------------------------------------------------------//
	// Section: wxStrings
	//
	const wxString sw8 = wxT("8");
    const wxString swB = wxT("b");
	const wxString swBlack = wxT("Black");
	const wxString swBlue = wxT("Blue");
    const wxString swBold = wxT("bold");
	const wxString swBrown = wxT("Brown");
    const wxString swCities3DXML = wxT("Cities3D.xml");
    const wxString swColor = wxT("color");
    const wxString swColors = wxT("colors");
    const wxString swConfigSectionGeneral = wxT("ConfigSectionGeneral");
    const wxString swData = wxT("data");
    const wxString swElements = wxT("elements");
	const wxString swEmpty = wxT("");
    const wxString swFalse = wxT("false");
    const wxString swFace = wxT("face");
    const wxString swFonts = wxT("fonts");
    const wxString swG = wxT("g");
	const wxString swGreen = wxT("Green");
    const wxString swItalics = wxT("italics");
    const wxString swLayouts = wxT("layouts");
    const wxString swLocale = wxT("locale");
    const wxString swName = wxT("name");
	const wxString swOrange = wxT("Orange");
    const wxString swPlugins = wxT("plugins");
    const wxString swPluginsD = wxT("pluginsD");
    const wxString swPoint = wxT("point");
	const wxString swPurple = wxT("Purple");
    const wxString swR = wxT("r");
	const wxString swRed = wxT("Red");
    const wxString swSizes = wxT("sizes");
    const wxString swSkinFolder = wxT("SkinFolder");
    const wxString swSkins = wxT("skins");
    const wxString swSoundFolder = wxT("SoundFolder");
    const wxString swSounds = wxT("sounds");
    const wxString swStandard = wxT("Standard");
    const wxString swSize = wxT("size");
    const wxString swTahoma = wxT("Tahoma");
    const wxString swTex = wxT("tex");
    const wxString swTexFolder = wxT("TexFolder");
    const wxString swTrue = wxT("true");
    const wxString swUnderline = wxT("underline");
	const wxString swWhite = wxT("White");
    
	// Formats
	const wxString swFloat6 = wxT("%.6f");
    const wxString swIntFormat = wxT("%d");
    const wxString swSlashStringSlashString = wxT("/%s/%s");
    const wxString swSearch = wxT("*%s");
    
    //-----------------------------------------------------------------------//
	// Section: Translations
	//

	// Formats
}

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
