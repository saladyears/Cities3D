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

#ifdef _WIN32
	// 'identifier' : class 'type' needs to have dll-interface to be used by 
	// clients of class 'type2'; apparently, STL on MSVC doesn't have 
	// __declspec(dllexport) declared before it.. but since it's all 
	// templates, it doesn't matter... stupid MSVC
	#pragma warning(disable:4251)

	// "unreachable code", stupid MSVC complaint about their own STL 
	// implementation
	#pragma warning(disable:4702)

	// unreferenced formal parameter, nanny warning that we don't need.
	#pragma warning(disable:4100)

    // non dll-interface class 'std::_Container_base_aux' used as base for 
    // dll-interface class
    #pragma warning(disable:4275)
#endif

//---------------------------- SYSTEM INCLUDES  -----------------------------//
// Useful STL headers
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <stack>
#include <stdexcept>
#include <vector>

// Useful C headers
#include <math.h>
#include <time.h>

//---------------------------- USER INCLUDES    -----------------------------//
// GNU Autotools config header
#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

// Common wxWidgets header
#include "wxCommon.h"

// Common boost header
#include "boostCommon.h"

//---------------------------- DEFINES          -----------------------------//
// In vs2005, use customized new operator
#if defined(__WIN32__) && defined(_DEBUG)
#	include <crtdbg.h>
#	define DEBUG_NEW new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#else
#	define DEBUG_NEW new
#endif

// Use appropriate class/function attribute for vs2005 and gcc
#ifdef __WIN32__
#   define DLL_EXPORT __declspec(dllexport)
#   define DLL_IMPORT __declspec(dllimport)
#   define DLL_GLOBAL
#   define DLL_LOCAL
#else
#   define DLL_EXPORT __attribute__((visibility("default")))
#   define DLL_IMPORT __attribute__((visibility("default")))
#   define DLL_GLOBAL __attribute__((visibility("default")))
#   define DLL_LOCAL  __attribute__((visibility("hidden")))
#endif

// In unix, allow wxLogErrors to include a backtrace
#if defined(__UNIX__) && defined(_DEBUG)
#	define wxLogError(...) wxFAIL_MSG(wxString::Format(__VA_ARGS__))
#endif

// Define TRUE AND FALSE if not already done
#ifdef TRUE
#	undef TRUE
#	define TRUE ((wxInt32) 1)
#endif
#ifdef FALSE
#	undef FALSE
#	define FALSE ((wxInt32) 0)
#endif

// Use portable/unix equivalents for vs2005-specific functions/macros
#ifndef __WIN32__
	#define _countof(array) (sizeof(array) / sizeof(array[0]))
	#define _strdup(string) (strdup(string))
	#define _vsprintf_p(buf, len, fmt, argv) (vsnprintf(buf, len, fmt, argv))
#endif

//---------------------------- TYPEDEFS         -----------------------------//
typedef std::vector<wxInt32> IntArray;
typedef std::vector<IntArray> IntArray2;
typedef std::vector<IntArray2> IntArray3;
typedef std::vector<wxUint32> UintArray;
typedef std::vector<wxString> StringArray;

typedef boost::scoped_array<wxInt32> ScopedIntArray;

typedef wxInt32 wxBOOL;

class Game; typedef boost::shared_ptr<Game> GamePtr;

//---------------------------- CLASSES          -----------------------------//

//---------------------------- PROTOTYPES       -----------------------------//

