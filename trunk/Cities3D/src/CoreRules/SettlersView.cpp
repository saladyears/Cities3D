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
#include "SettlersView.h"
#include "Controller.h"
#include "Game.h"
#include "DataObject.h"
#include "GameEngine.h"
#include "ConfigDatabase.h"
#include "MessageUI.h"
#include "ControlsUI.h"
#include "PlayerUI.h"
#include "BankUI.h"
#include "BuildUI.h"
#include "BuildData.h"
#include "BaseStaticText.h"
#include "SkinManager.h"
#include "DrawObject.h"
#include "PopupManager.h"
#include "IPluginEngine.h"
#include "IUIEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	
	DrawObjectPtr sPopupObject;

	// The delay before showing a popup.
	const wxInt32 DELAY_TICKS = 3;
}

//---------------------------- PUBLIC           -----------------------------//
wxSettlersView::wxSettlersView(wxWindow *parent, wxAuiManager* manager,
	const wxWindowID id, const wxPoint& pos, const wxSize& size, long style, 
	const wxString& name, wxInt32 *attrib) 
: wxSettlersGLCanvas(parent, id, pos, size, style, name, attrib)
, mAuiManager(manager)
{
	wxASSERT(NULL != manager);

	PopupManager::get().AddPopup(this, 1);

	GUI.SetGLCanvas(this);

	Show(true);

	// Add ourselves to the manager as the center window.
	mAuiManager->AddPane(this, wxAuiPaneInfo().Name(swSettlersView)
		.Caption(stGameWindow).Center().CloseButton(false)
		.MaximizeButton(false).Dockable(false));

	// Enable pane window sizing while in game.
	mAuiManager->GetArtProvider()->SetMetric(wxAUI_DOCKART_SASH_SIZE, 1);

	SetFocus();
}

wxSettlersView::~wxSettlersView()
{
	sPopupObject = DrawObjectPtr();
	PopupManager::get().RemovePopup(this);

	mAuiManager->DetachPane(this);
}

bool wxSettlersView::TestPopup(const wxPoint& pos, wxInt32& delay) const
{
	wxPoint point = ScreenToClient(pos);

	DrawObjectPtr object = PopupSelect(point.x, point.y);

	bool ret = object;

	delay = DELAY_TICKS;

	if(sPopupObject != object)
	{
		sPopupObject = object;
		ret = false;
		delay = -1;
	}

	return ret;
}

void wxSettlersView::CreatePopup(wxWindow* window, wxSizer* sizer) const
{
	wxASSERT(sPopupObject);

	wxString name, owner;
	wxBitmap image;
	bool createBitmap = false;
	ColorType color = CR_SIZE;
	
	sPopupObject->PopupFunction(name, owner, image, createBitmap, color);

	wxBoxSizer* horz = new wxBoxSizer(wxHORIZONTAL);

	// See if we need to create a bitmap for the item.
	if( (true == createBitmap) &&
		(TRUE == CONFIG.read<wxInt32>(swConfigSectionOpenGL, swRenderBitmaps, TRUE)))
	{
		// Create the bitmap for the object.
		static const Vector vector(10.0f, 10.0f, 10.0f);
		image = const_cast<wxSettlersView*>(this)->ConvertGLtoWX(sPopupObject, 
			32, 32, vector);
	}

	// Add a bitmap if we need to.
	if(true == image.Ok())
	{
		wxBoxSizer* vert = new wxBoxSizer(wxVERTICAL);
		wxSize size(image.GetWidth(), image.GetHeight());

		wxStaticBitmap* bitmap = new wxStaticBitmap(window, -1, image,
			wxDefaultPosition, size);

		vert->AddStretchSpacer();
		vert->Add(bitmap, 0, wxALL, 4);
		vert->AddStretchSpacer();

		horz->Add(vert, 0, wxEXPAND);
	}

	wxBoxSizer* vert = new wxBoxSizer(wxVERTICAL);

	wxClientDC dc(window);
	wxFont font = SKIN.Font(shPopupFont);

	dc.SetFont(font);

	wxInt32 x, y;
	dc.GetTextExtent(name, &x, &y);

	wxBaseStaticText* text = new wxBaseStaticText(window, -1, 
		name, shDefaultText, shPopupFont, wxSize(x + 2, y + 2));

	vert->Add(text, 0, wxEXPAND | wxALL, 4);

	if(false == owner.IsEmpty())
	{
		// Get color.
		wxColour textColor = wxColour(255, 255, 255);
		if(CR_SIZE != color)
		{
			textColor = ColorInfoData::UIColor(color);
		}

		dc.GetTextExtent(owner, &x, &y);

		text = new wxBaseStaticText(window, -1, owner, shDefaultText, 
			shPopupFont, wxSize(x + 2, y + 2));
		text->SetForegroundColour(textColor);

		vert->Add(text, 0, wxEXPAND | wxBOTTOM | wxRIGHT | wxLEFT, 4);
	}

	horz->Add(vert, 1, wxEXPAND);

	sizer->Add(horz, 1, wxEXPAND | wxBOTTOM, 2);
}

wxWindow* wxSettlersView::GetThis() const
{
	return const_cast<wxSettlersView*>(this);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void wxSettlersView::GenerateDebugInfo(StringArray &str)
{
	wxSettlersGLCanvas::GenerateDebugInfo(str);

	// Draw it.
	wxFont font(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, 
		wxFONTSTYLE_NORMAL, false, swTahoma);

	wxSize size = GetSize();
	wxInt32 x = 10;
	wxInt32 y = size.y - 70;

	wxWindowDC dc(this);
	dc.SetFont(font);
	dc.SetBackgroundMode(wxTRANSPARENT);

	for(size_t i = 0; i < str.size(); ++i)
	{
		dc.SetTextForeground(wxColour(0, 0, 0));
		dc.DrawText(str[i], x + 1, y + 1);
		dc.SetTextForeground(wxColour(96, 96, 96));
		dc.DrawText(str[i], x, y);

		y += 15;
	}
}
