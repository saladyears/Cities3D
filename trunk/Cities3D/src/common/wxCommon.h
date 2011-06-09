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
#ifdef _WIN32
	// 'localtime': This function or variable may be unsafe. Consider using 
	// localtime_s instead. To disable deprecation, use 
	// _CRT_SECURE_NO_WARNINGS. See online help for details.
	#pragma warning(disable:4996)
#endif

// Standard precompiled wxWidgets header
#include <wx/wxprec.h>

// Other wxWidgets headers
#include <wx/aui/aui.h>
#include <wx/config.h>
#include <wx/datetime.h>
#include <wx/datstrm.h>
#include <wx/dir.h>
#include <wx/dynlib.h>
#include <wx/evtloop.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/hashmap.h>
#include <wx/hashset.h>
#include <wx/image.h>
#include <wx/listctrl.h>
#include <wx/mstream.h>
#include <wx/object.h>
#include <wx/sckaddr.h>
#include <wx/sckstrm.h>
#include <wx/slider.h>
#include <wx/socket.h>
#include <wx/stdpaths.h>
#include <wx/stream.h>
#include <wx/timer.h>
#include <wx/treectrl.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>
#include <wx/xml/xml.h>

#ifdef _WIN32
	#pragma warning(default:4996)
#endif


//---------------------------- USER INCLUDES    -----------------------------//

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------- PROTOTYPES       -----------------------------//

