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
#include "BaseBitmapButton.h"
#include "SkinManager.h"
#include "Controller.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	wxColour sBackground_BaseBitmapButton;
}

//---------------------------- PUBLIC           -----------------------------//
wxBaseBitmapButton::wxBaseBitmapButton(wxWindow* parent, wxWindowID id, 
	const wxPoint& pos, const wxSize& size)
: wxOwnerDrawWindow(parent, id, pos, wxSize(size) + wxSize(1, 1))
, mMode(ModeNone)
{
	// Set up the colors
	OnSkin();

	SetCursor(wxCursor(wxCURSOR_HAND));

	Controller::get().AddReceiver(shEventSkin, &wxBaseBitmapButton::OnSkin, 
		this);
}

wxBaseBitmapButton::~wxBaseBitmapButton()
{
	Controller::get().RemoveReceiver(shEventSkin, &wxBaseBitmapButton::OnSkin, 
		this);
}

bool wxBaseBitmapButton::Enable(bool enable)
{
	bool ret = wxOwnerDrawWindow::Enable(enable);
	SetBitmap();
	Refresh();
	return ret;
}

void wxBaseBitmapButton::SetBitmaps(const wxBitmap& enabled, 
	const wxBitmap& disabled)
{
	mEnabledBitmap = enabled;
	mDisabledBitmap = disabled;

	SetBitmap();
	Refresh();
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxBaseBitmapButton, wxOwnerDrawWindow)
	EVT_ENTER_WINDOW(wxBaseBitmapButton::OnEnterWindow)
	EVT_LEFT_DOWN(wxBaseBitmapButton::OnLeftDown)
	EVT_LEFT_UP(wxBaseBitmapButton::OnLeftUp)
	EVT_LEAVE_WINDOW(wxBaseBitmapButton::OnLeaveWindow)
	EVT_MOTION(wxBaseBitmapButton::OnMouseMove)
END_EVENT_TABLE()

void wxBaseBitmapButton::SetBitmap()
{
	mDrawBitmap = IsEnabled() ? mEnabledBitmap : mDisabledBitmap;
}

void wxBaseBitmapButton::Paint(wxMemoryDC &dc)
{
	wxInt32 origin = 0;
	if(ModeDown == mMode)
	{
		origin = 1;
	}

	const wxSize &size = GetSize();
	dc.SetBrush(wxBrush(sBackground_BaseBitmapButton));
	dc.DrawRectangle(0, 0, size.x, size.y);

	dc.DrawBitmap(mDrawBitmap, origin, origin, true);
}

void wxBaseBitmapButton::OnSkin(wxInt32)
{
	// Text
	sBackground_BaseBitmapButton = SKIN.Element(shBackground);
}

void wxBaseBitmapButton::OnEnterWindow(wxMouseEvent &)
{
	if(ModeDownOutside == mMode)
	{
		mMode = ModeDown;
	}
	Refresh();
}

void wxBaseBitmapButton::OnLeftDown(wxMouseEvent &)
{
	mMode = ModeDown;
	CaptureMouse();
	Refresh();
}

void wxBaseBitmapButton::OnLeftUp(wxMouseEvent &)
{
	ReleaseMouse();

	Mode currentMode = mMode;
	mMode = ModeNone;
	Refresh();

	//send the click message to the parent
	if(ModeDown == currentMode)
	{
		wxCommandEvent command(wxEVT_COMMAND_BUTTON_CLICKED, GetId());
		ProcessEvent(command);
	}
}

void wxBaseBitmapButton::OnLeaveWindow(wxMouseEvent &)
{
	if(ModeDown == mMode)
	{
		mMode = ModeDownOutside;
	}
	Refresh();
}

void wxBaseBitmapButton::OnMouseMove(wxMouseEvent &event)
{
	// We only care about mouse movement if we've captured the mouse.
	if(true == HasCapture())
	{
		const wxSize &thisSize = GetSize();
		const wxPoint &pos = event.GetPosition();
		
		// Since leave window events apparently no longer get called during
		// mouse capture, we have to check to see if the mouse is over the
		// button during capture and set the mode accordingly.
		Mode thisMode = ModeDown;

		if( (0 > pos.x) || (0 > pos.y) ||
			(thisSize.x < pos.y) || (thisSize.y < pos.y))
		{
			thisMode = ModeDownOutside;
		}

		if(thisMode != mMode)
		{
			mMode = thisMode;
			Refresh();
		}
	}
}
