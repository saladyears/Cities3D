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
#include <GL/glew.h>

//---------------------------- USER INCLUDES    -----------------------------//
#include "CoreGLCanvas.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	void InitGL(wxCoreGLCanvas *canvas)
	{
		wxASSERT(NULL != canvas);

		if(NULL != canvas->GetContext())
		{
			//set shading model to smooth
			glShadeModel(GL_SMOOTH);

			//adjust the perspective hwxInt32
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
			glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

			//set the clear color
			glClearColor(0., 0., 0., 0.);

			//set up depth testing
			glClearDepth(1.0f);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);

			//set up lighting
			GLfloat amb[] = {1.0f, 1.0f, 1.0f, 1.0f};
			GLfloat dif[] = {1.0f, 1.0f, 1.0f, 1.0f};
			GLfloat pos[] = {0.0f, 1.0f, 4.0f, 1.0f};

			glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
			glLightfv(GL_LIGHT0, GL_POSITION, pos);
			glEnable(GL_LIGHT0);

			//turn on lighting
			glEnable(GL_LIGHTING);
		}
	}
}

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//
wxCoreGLCanvas::wxCoreGLCanvas(wxWindow *parent, wxWindowID id, 
							   const wxPoint& pos, const wxSize& size, 
							   long style, const wxString& name, 
							   wxInt32 *attrib)
: wxGLCanvas(parent, id, pos, size, style, name, attrib)
, mpTopLevelWindow(dynamic_cast<wxTopLevelWindow *>(parent))
, mDirty(true)
#ifdef __WXGTK__
, mPainted(false)
#endif
{
	if (GetContext())
    {
        SetCurrent();

		// Initialize glew extensions.
		GLenum err = glewInit();
		if(GLEW_OK != err)
		{
			wxLogError(
				wxT("No OpenGL extensions are available on this hardware."));
		}

		InitGL(this);
	}
}

wxCoreGLCanvas::~wxCoreGLCanvas()
{
}

wxGLContext* wxCoreGLCanvas::GetContext()
{
	#ifdef __WXGTK__
	if(false == IsShown())
	{
		return NULL;
	}
	#endif

	return wxGLCanvas::GetContext();
}

void wxCoreGLCanvas::ResizeGL(void)
{
    // set GL viewport (not called by wxGLCanvas::OnSize on all platforms...)
    wxInt32 w, h;
    GetClientSize(&w, &h);

    if (GetContext())
    {
        SetCurrent();
        glViewport(0, 0, (GLint) w, (GLint) h);

		//select the projection matrix and reset it
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//calculate the aspect ratio
		GLfloat dAspect;
		dAspect = (GLfloat) w / (GLfloat) h;
		gluPerspective(45.0f, dAspect, 0.1, 250.);

		//reset the modelview matrix
		glMatrixMode(GL_MODELVIEW);
    }
}

void wxCoreGLCanvas::SetDirty(bool dirty)
{
	mDirty = dirty;
	
	if(true == dirty)
	{
		wxPaintEvent event;
		AddPendingEvent(event);
	}
}

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxCoreGLCanvas, wxGLCanvas)
    EVT_SIZE(wxCoreGLCanvas::OnSize)
	EVT_PAINT(wxCoreGLCanvas::OnPaint)
    EVT_ERASE_BACKGROUND(wxCoreGLCanvas::OnEraseBackground)
END_EVENT_TABLE()

void wxCoreGLCanvas::OnPaint(wxPaintEvent &)
{
	wxPaintDC dc(this);

	// Only paint if we're in the background.
	if( (NULL != mpTopLevelWindow) &&
		(false == mpTopLevelWindow->IsActive()))
	{
		mDirty = true;
	}
	
	if(true == GetParent()->IsShown())
	{
		Paint();
	}

#	ifdef __WXGTK__
	mPainted = true;
#	endif
}

void wxCoreGLCanvas::OnSize(wxSizeEvent& event)
{
#	ifdef __WXGTK__
	if(false == mPainted)
	{
		return;
	}
#	endif

    //this is also necessary to update the context on some platforms
    event.Skip();

	ResizeGL();

	SetDirty();
}

void wxCoreGLCanvas::OnEraseBackground(wxEraseEvent&)
{
    //do nothing, which avoids flashing in windows
}
