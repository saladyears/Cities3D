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
#include <math.h>

//---------------------------- USER INCLUDES    -----------------------------//
#include "FrameworkGLCanvas.h"
#include "ConfigDatabase.h"
#include "GameEngine.h"
#include "Controller.h"
#include "Map.h"
#include "Vector.h"
#include "define/defineGL.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	Vector sCamera;
	Vector sLookAt;
	Vector sViewCameraPos;
	Vector sViewCameraLookAt;
	Vector sRotation;

	wxInt32 sLastSelection;

	void UpdateViewCamera()
	{
		//optimized euler angle rotation taken from Q.36 at 
		//http://www.gamedev.net/reference/articles/article1691.asp

		float A = cosf(sRotation.x);
		float B = sinf(sRotation.x);
		float C = cosf(sRotation.y);
		float D = sinf(sRotation.y);

		float AC = A * C;
		float AD = A * D;
		float BC = B * C;
		float BD = B * D;
		
		//multiply the camera through it
		sViewCameraPos.x = (sCamera.x * C) + (sCamera.y * BD) + 
			(sCamera.z * -AD);
		sViewCameraPos.y = (sCamera.y * A) + (sCamera.z * B);
		sViewCameraPos.z = (sCamera.x * D )+ (sCamera.y * -BC) + 
			(sCamera.z * AC);

		sViewCameraLookAt.x = (sLookAt.x * C) + (sLookAt.y * BD) + 
			(sLookAt.z * -AD);
		sViewCameraLookAt.y = (sLookAt.y * A) + (sLookAt.z * B);
		sViewCameraLookAt.z = (sLookAt.x * D )+ (sLookAt.y * -BC) + 
			(sLookAt.z * AC);
	}

	const float sKeyRotateDelta = 4.0f;
	const float sKeyPanDelta = 0.5f;
	const float sKeyZoomDelta = 10.0f;
}

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//
wxFrameworkGLCanvas::wxFrameworkGLCanvas(wxWindow *parent, const wxWindowID id, 
										 const wxPoint& pos,
										 const wxSize& size, long style, 
										 const wxString& name, wxInt32 *attrib)
: wxBaseGLCanvas(parent, id, pos, size, style, name, attrib)
, mSelection(-1)
, mHitTest(false)
, mFinalHit(false)
{
	ResetCamera();

	Controller::get().AddReceiver(shEventMapZoomIn, 
		&wxFrameworkGLCanvas::OnMapZoomIn, this);
	Controller::get().AddReceiver(shEventMapZoomOut, 
		&wxFrameworkGLCanvas::OnMapZoomOut, this);
	Controller::get().AddReceiver(shEventMapPanLeft,
		&wxFrameworkGLCanvas::OnMapPanLeft, this);
	Controller::get().AddReceiver(shEventMapPanRight,
		&wxFrameworkGLCanvas::OnMapPanRight, this);
	Controller::get().AddReceiver(shEventMapPanUp,
		&wxFrameworkGLCanvas::OnMapPanUp, this);
	Controller::get().AddReceiver(shEventMapPanDown,
		&wxFrameworkGLCanvas::OnMapPanDown, this);
	Controller::get().AddReceiver(shEventMapRotateLeft,
		&wxFrameworkGLCanvas::OnMapRotateLeft, this);
	Controller::get().AddReceiver(shEventMapRotateRight,
		&wxFrameworkGLCanvas::OnMapRotateRight, this);
	Controller::get().AddReceiver(shEventMapRotateUp,
		&wxFrameworkGLCanvas::OnMapRotateUp, this);
	Controller::get().AddReceiver(shEventMapRotateDown,
		&wxFrameworkGLCanvas::OnMapRotateDown, this);
}

wxFrameworkGLCanvas::~wxFrameworkGLCanvas()
{
	Controller::get().RemoveReceiver(shEventMapZoomIn, 
		&wxFrameworkGLCanvas::OnMapZoomIn, this);
	Controller::get().RemoveReceiver(shEventMapZoomOut, 
		&wxFrameworkGLCanvas::OnMapZoomOut, this);
	Controller::get().RemoveReceiver(shEventMapPanLeft,
		&wxFrameworkGLCanvas::OnMapPanLeft, this);
	Controller::get().RemoveReceiver(shEventMapPanRight,
		&wxFrameworkGLCanvas::OnMapPanRight, this);
	Controller::get().RemoveReceiver(shEventMapPanUp,
		&wxFrameworkGLCanvas::OnMapPanUp, this);
	Controller::get().RemoveReceiver(shEventMapPanDown,
		&wxFrameworkGLCanvas::OnMapPanDown, this);
	Controller::get().RemoveReceiver(shEventMapRotateLeft,
		&wxFrameworkGLCanvas::OnMapRotateLeft, this);
	Controller::get().RemoveReceiver(shEventMapRotateRight,
		&wxFrameworkGLCanvas::OnMapRotateRight, this);
	Controller::get().RemoveReceiver(shEventMapRotateUp,
		&wxFrameworkGLCanvas::OnMapRotateUp, this);
	Controller::get().RemoveReceiver(shEventMapRotateDown,
		&wxFrameworkGLCanvas::OnMapRotateDown, this);
}

void wxFrameworkGLCanvas::SetHitTest(const bool hit)
{
	//set the value
	mHitTest = hit;

	//reset selection
	mSelection = -1;
}

const Vector &wxFrameworkGLCanvas::GetCameraPos() const
{
	return sViewCameraPos;
}

const Vector &wxFrameworkGLCanvas::GetCameraLookAt() const
{
	return sViewCameraLookAt;
}

void wxFrameworkGLCanvas::SaveCamera()
{
	wxString str;

	//camera settings
	CONFIG.write(swConfigSectionOpenGL, swConfigGLCameraPosX, sCamera.x);
	CONFIG.write(swConfigSectionOpenGL, swConfigGLCameraPosY, sCamera.y);
	CONFIG.write(swConfigSectionOpenGL, swConfigGLCameraPosZ, sCamera.z);

	//look at settings
	CONFIG.write(swConfigSectionOpenGL, swConfigGLLookPosX, sLookAt.x);
	CONFIG.write(swConfigSectionOpenGL, swConfigGLLookPosY, sLookAt.y);
	CONFIG.write(swConfigSectionOpenGL, swConfigGLLookPosZ, sLookAt.z);

	//rotation
	CONFIG.write(swConfigSectionOpenGL, swConfigGLRotationX, sRotation.x);
	CONFIG.write(swConfigSectionOpenGL, swConfigGLRotationY, sRotation.y);
	CONFIG.write(swConfigSectionOpenGL, swConfigGLRotationZ, sRotation.z);
}

void wxFrameworkGLCanvas::ResetCamera()
{
	//reset eyepoint
	sCamera.x = 0.;
	sCamera.y = MAP.read<float>(shInitialDepth);
	sCamera.z = 1.0;	

	//reset rotation
	sRotation.x = 0.0;
	sRotation.y = MAP.read<float>(shInitialRotation) * D2R;
	sRotation.z = 0.0;

	//reset look point
	sLookAt.x = 0.0;
	sLookAt.y = 0.0;
	sLookAt.z = 0.0;

	//camera pos
	sCamera.x = CONFIG.read<float>(swConfigSectionOpenGL, swConfigGLCameraPosX, 
		sCamera.x);
	sCamera.y = CONFIG.read<float>(swConfigSectionOpenGL, swConfigGLCameraPosY, 
		sCamera.y);
	sCamera.z = CONFIG.read<float>(swConfigSectionOpenGL, swConfigGLCameraPosZ, 
		sCamera.z);

	//look at
	sLookAt.x = CONFIG.read<float>(swConfigSectionOpenGL, swConfigGLLookPosX, 
		sLookAt.x);
	sLookAt.y = CONFIG.read<float>(swConfigSectionOpenGL, swConfigGLLookPosY, 
		sLookAt.y);
	sLookAt.z = CONFIG.read<float>(swConfigSectionOpenGL, swConfigGLLookPosZ, 
		sLookAt.z);

	//rotation
	sRotation.x = CONFIG.read<float>(swConfigSectionOpenGL, swConfigGLRotationX, 
		sRotation.x);
	sRotation.y = CONFIG.read<float>(swConfigSectionOpenGL, swConfigGLRotationY, 
		sRotation.y);
	sRotation.z = CONFIG.read<float>(swConfigSectionOpenGL, swConfigGLRotationZ, 
		sRotation.z);
	
	UpdateViewCamera();
}

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxFrameworkGLCanvas, wxBaseGLCanvas)
	EVT_MOTION(wxFrameworkGLCanvas::OnMouseMove)
	EVT_MOUSEWHEEL(wxFrameworkGLCanvas::OnMouseWheel)
	EVT_LEFT_DOWN(wxFrameworkGLCanvas::OnLButtonDown)
	EVT_MIDDLE_DOWN(wxFrameworkGLCanvas::OnMButtonDown)
	EVT_MIDDLE_UP(wxFrameworkGLCanvas::OnMButtonUp)
	EVT_RIGHT_DOWN(wxFrameworkGLCanvas::OnRButtonDown)
	EVT_RIGHT_UP(wxFrameworkGLCanvas::OnRButtonUp)
	EVT_RIGHT_DCLICK(wxFrameworkGLCanvas::OnRButtonDblClk)
END_EVENT_TABLE()

void wxFrameworkGLCanvas::rotate(const float x, const float y)
{
	float magnitude;
	float theta;
	float divisor = 1.0f / 120.f;
	
	magnitude = sqrtf((x * x) + (y * y));
	theta = atan2f(y, x);

	sRotation.x += (magnitude * sinf(theta) * divisor);
	sRotation.y += (magnitude * cosf(theta) * divisor);

	//clamp (-86 degrees)
	if(sRotation.x < -1.500836482f) 
	{
		sRotation.x = -1.500836482f;
	}
	else if(sRotation.x > 0) 
	{
		sRotation.x = 0.;
	}

	UpdateViewCamera();

	SetDirty();
}

void wxFrameworkGLCanvas::pan(const float x, const float y)
{
	//alter the look point and eye points
	sLookAt.x += x;
	sLookAt.z -= y;
	sCamera.x += x;
	sCamera.z -= y;

	UpdateViewCamera();

	SetDirty();
}

void wxFrameworkGLCanvas::zoom(const float x, const float y)
{
	float dA, dB;

	dA = sCamera.y + y;
	dB = sCamera.z + x;

	//clamp
	if( (dA > 10.0) && 
		(dA < 150.0))
	{
		sCamera.y = dA;
		sCamera.z = dB;
	}

	UpdateViewCamera();

	SetDirty();
}

//in mouse move events, we determine if we are rotating the board (right 
//button down), panning the board (middle button down), zooming (control 
// right), or else just looking for hit tests
void wxFrameworkGLCanvas::OnMouseMove(wxMouseEvent &event)
{
	wxPoint point;
	point = event.GetPosition();

	float dX, dY;

	dX = static_cast<float>(point.x - mPoint.x);
	dY = static_cast<float>(point.y - mPoint.y);

	//do things if the right mouse button is down
	if(true == event.m_rightDown)
	{
		bool shift = event.m_shiftDown;
		bool ctrl = event.m_controlDown;

		//panning is with the shift key down
		if( (true == shift) &&
			(false == ctrl))
		{
			pan((-dX / 7.0), (dY / 7.0));
		}
		//zooming is with the control key down
		else if( (true == ctrl) &&
				 (false == shift))
		{
			zoom((dX / 800.0), (dY / 10.0));
		}
		else
		{
			rotate(dX, dY);
		}
	}
	//panning is with the middle button down
	else if(true == event.m_middleDown)
	{
		pan((-dX / 7.0), (dY / 7.0));
	}
	else if(true == mHitTest)
	{
		//try selection
		mSelection = Select(point.x, point.y);

		//set mouse pointer
		if(-1 == mSelection)
		{
			SetCursor(wxCursor(wxCURSOR_ARROW));
		}
		else
		{
			SetCursor(wxCursor(wxCURSOR_HAND));
		}

		if(mSelection != sLastSelection)
		{
			SetDirty();
			sLastSelection = mSelection;
		}
	}

	//save as previous point
	mPoint = point;
}

void wxFrameworkGLCanvas::OnMouseWheel(wxMouseEvent &event)
{
	float dY;

	dY = -(event.m_wheelRotation / 120.);

	zoom((dY / 160.0), (dY * 5.0));
}

void wxFrameworkGLCanvas::OnLButtonDown(wxMouseEvent &event)
{
	SetFocus();

	// This only needs to be tested if hit tests are turned on.
	if(true == mHitTest)
	{
		wxPoint point = event.GetPosition();

		mFinalHit = true;

		mSelection = Select(point.x, point.y);
		SelectObject(mSelection);
		
		mFinalHit = false;

		SetCursor(wxCursor(wxCURSOR_ARROW));
	}
}

void wxFrameworkGLCanvas::OnMButtonDown(wxMouseEvent &event)
{
	//set the starting point
	mPoint = event.GetPosition();

	//start mouse capture
	CaptureMouse();
}

void wxFrameworkGLCanvas::OnMButtonUp(wxMouseEvent &)
{
	//release mouse capture
	if(GetCapture() == this)
	{
		ReleaseMouse();
	}
}

void wxFrameworkGLCanvas::OnRButtonDown(wxMouseEvent &event)
{
	//same code as middle down
	OnMButtonDown(event);
}

void wxFrameworkGLCanvas::OnRButtonUp(wxMouseEvent &event)
{
	//same code as middle up
	OnMButtonUp(event);
}

void wxFrameworkGLCanvas::OnRButtonDblClk(wxMouseEvent &)
{
	//reset eye position
	ResetCamera();
}

void wxFrameworkGLCanvas::OnMapZoomIn(wxInt32)
{
	zoom((-sKeyZoomDelta / 800.0), (-sKeyZoomDelta / 10.0));
}

void wxFrameworkGLCanvas::OnMapZoomOut(wxInt32)
{
	zoom((sKeyZoomDelta / 800.0), (sKeyZoomDelta / 10.0));
}

void wxFrameworkGLCanvas::OnMapPanLeft(wxInt32)
{
	pan(sKeyPanDelta, 0.0f);
}

void wxFrameworkGLCanvas::OnMapPanRight(wxInt32)
{
	pan(-sKeyPanDelta, 0.0f);
}

void wxFrameworkGLCanvas::OnMapPanUp(wxInt32)
{
	pan(0.0f, -sKeyPanDelta);
}

void wxFrameworkGLCanvas::OnMapPanDown(wxInt32)
{
	pan(0.0f, sKeyPanDelta);
}

void wxFrameworkGLCanvas::OnMapRotateLeft(wxInt32)
{
	rotate(-sKeyRotateDelta, 0.0f);
}

void wxFrameworkGLCanvas::OnMapRotateRight(wxInt32)
{
	rotate(sKeyRotateDelta, 0.0f);
}

void wxFrameworkGLCanvas::OnMapRotateUp(wxInt32)
{
	rotate(0.0f, -sKeyRotateDelta);
}

void wxFrameworkGLCanvas::OnMapRotateDown(wxInt32)
{
	rotate(0.0f, sKeyRotateDelta);
}