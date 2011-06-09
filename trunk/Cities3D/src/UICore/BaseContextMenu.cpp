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
#include "BaseContextMenu.h"
#include "ILayoutCreator.h"
#include "BaseListCtrl.h"
#include "Controller.h"
#include "SkinManager.h"
#include "TimerManager.h"
#include "PopupManager.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	// The number of milliseconds off the frame before we shut down the menu.
	const wxInt32 DELAY_TIME = 500;
}

//---------------------------- PUBLIC           -----------------------------//
wxBaseContextMenu::wxBaseContextMenu(wxWindow *parent, 
	ILayoutCreatorPtr layout, const wxPoint &pos)
: wxFrame(parent, -1, swEmpty, pos, wxDefaultSize, wxFRAME_FLOAT_ON_PARENT | 
	wxFRAME_NO_TASKBAR)
, mIsOutside(false)
, mOutside(-1)
{
	wxASSERT(NULL != parent);
	wxASSERT(layout);

	SetBackgroundColour(SKIN.Element(shBackground));

	// Create the layout.
	wxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
	wxBaseListCtrl *iconLayout = layout->CreateLayout(this);
	
	wxSize size = iconLayout->GetListBestSize();
	iconLayout->SetSize(size);

	sizer->Add(iconLayout, 1, wxEXPAND);
	SetSizer(sizer);

	SetSize(size);

	Controller::get().AddReceiver(shEventContextMenuClick, 
		&wxBaseContextMenu::OnContextMenuClick, this);

	mWatch.Start();

	PopupManager::get().AddWindow(this, 2);

	TIME.AddPeriodicFunction(&wxBaseContextMenu::SampleMouse, this, 33);
}

wxBaseContextMenu::~wxBaseContextMenu()
{
	TIME.RemovePeriodicFunction(&wxBaseContextMenu::SampleMouse, this, 33);

	PopupManager::get().RemoveWindow(this);

	Controller::get().RemoveReceiver(shEventContextMenuClick, 
		&wxBaseContextMenu::OnContextMenuClick, this);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void wxBaseContextMenu::SampleMouse()
{
	long thisTime = 0;

	// Get the current mouse position.
	wxPoint point = ScreenToClient(wxGetMousePosition());
	wxSize size = GetSize();

	if( (0 > point.x) || (size.x < point.x) ||
		(0 > point.y) || (size.y < point.y))
	{
		if(true == mIsOutside)
		{
			thisTime = mWatch.Time();
		}
		else
		{
			mIsOutside = true;
			mOutside = mWatch.Time();
		}
	}
	else
	{
		mIsOutside = false;
	}

	if(DELAY_TIME < thisTime - mOutside)
	{
		TIME.RemovePeriodicFunction(&wxBaseContextMenu::SampleMouse, this, 33);
		wxLogDebug(wxT("Context menu destroyed by time."));
		Show(false);
		Destroy();
		Controller::get().Transmit(shEventDirty, 0);
	}
}

void wxBaseContextMenu::OnContextMenuClick(wxInt32)
{
	wxLogDebug(wxT("Context menu destroyed by click."));

	Destroy();
}
