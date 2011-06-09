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
#ifdef __WIN32__
#include <GL/wglew.h>
#endif

#ifndef __WIN32__
#include <sys/time.h>
#endif

//---------------------------- USER INCLUDES    -----------------------------//
#include "BaseGLCanvas.h"
#include "ConfigDatabase.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
#ifdef __WIN32__
	float sMillisecondCounts;
#endif
}

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//
wxBaseGLCanvas::wxBaseGLCanvas(wxWindow *parent, wxWindowID id, 
	const wxPoint& pos, const wxSize& size, long style, const wxString& name, 
	wxInt32 *attrib)
: wxCoreGLCanvas(parent, id, pos, size, style, name, attrib)
, mpVendor(glGetString(GL_VENDOR))
{
#ifdef __WIN32__
	LARGE_INTEGER counts;	
	QueryPerformanceFrequency(&counts);
	sMillisecondCounts = static_cast<float>(counts.QuadPart) / 1000.0f;

	if(WGLEW_EXT_swap_control)
	{
		wglSwapIntervalEXT(0);
	}
#endif
}

//---------------------------- PRIVATE          -----------------------------//
void wxBaseGLCanvas::Paint()
{
	if(true == IsDirty())
	{
#ifdef __WIN32__
		static LARGE_INTEGER start;
		static LARGE_INTEGER middle1;
		static LARGE_INTEGER end;
#else
		static timeval start;
		static timeval middle1;
		static timeval end;
#endif
		
#ifdef __WIN32__
		QueryPerformanceCounter(&start);
#else
		gettimeofday(&start, NULL);
#endif

		if (GetContext()) 
		{
			SetCurrent();
		    
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glLoadIdentity();

			DrawScene();
			glFinish();
		}

#ifdef __WIN32__
		QueryPerformanceCounter(&middle1);
#else
		gettimeofday(&middle1, NULL);
#endif

		if (GetContext()) 
		{
			SwapBuffers();
			glFinish();
		}

#ifdef __WIN32__
		QueryPerformanceCounter(&end);
#else
		gettimeofday(&end, NULL);
#endif

#ifdef __WIN32__
		float first = static_cast<float>(middle1.QuadPart - start.QuadPart) / sMillisecondCounts;
		float second = static_cast<float>(end.QuadPart - middle1.QuadPart) / sMillisecondCounts;
#else
		float first = (static_cast<float>(middle1.tv_sec - start.tv_sec) * 1000.0f) +
				(static_cast<float>(middle1.tv_usec - start.tv_usec) / 1000.0f);
		float second = (static_cast<float>(end.tv_sec - middle1.tv_sec) * 1000.0f) +
				(static_cast<float>(end.tv_usec - middle1.tv_usec) / 1000.0f);
#endif

		// Output debug info if necessary.
		if(TRUE == CONFIG.read<wxInt32>(swConfigSectionOpenGL, swDebugInfo, FALSE))
		{
			// Generate a default string that any derived class can use or append to.
			StringArray str;
			str.push_back(wxString::Format(swDriverString.c_str(), 
				HashString(reinterpret_cast<const char*>(mpVendor)).cwx_str()));
			str.push_back(wxString::Format(swRenderString.c_str(), 
				first, second));
			str.push_back(wxString::Format(swFPS.c_str(), 
				1000.0f / (first + second)));
			GenerateDebugInfo(str);
		}

		SetDirty(false);
	}
}
