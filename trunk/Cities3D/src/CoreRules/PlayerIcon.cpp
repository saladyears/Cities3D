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
#include "PlayerIcon.h"
#include "TextPopup.h"
#include "ImageEngine.h"
#include "SkinManager.h"
#include "Controller.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	struct no_delete_player_icon
	{
		void operator()(void *){}
	};

	class wxPopupBitmapIcon
		: public wxStaticBitmap
		, public TextPopup
	{
	public:
	#ifdef __WIN32__
		//disables the 'this' : used in base member initializer list warning
		#pragma warning(disable:4355)
	#endif
		wxPopupBitmapIcon(const wxString& text, wxWindow* parent, wxWindowID id, 
			const wxBitmap& label)
		: wxStaticBitmap(parent, id, label, wxDefaultPosition, wxSize(0, 0))
		, TextPopup(this, text)
		{}
	};
}

//---------------------------- PUBLIC           -----------------------------//
PlayerIcon::PlayerIcon(wxWindow *parent, const PlayerUpdateFunc &update,
	const HashString& key, const wxString& text)
: mUpdateFunc(update)
, mKey(key)
{
	wxASSERT(NULL != parent);
	wxASSERT(NULL != mUpdateFunc);

	mBitmap = StaticBitmapPtr(
		new wxPopupBitmapIcon(text, parent, 0, wxBitmap(1, 1)), no_delete_player_icon());

	OnSkin();

	Controller::get().AddReceiver(shEventSkin, &PlayerIcon::OnSkin, 
		this);
}

PlayerIcon::~PlayerIcon()
{
	Controller::get().RemoveReceiver(shEventSkin, &PlayerIcon::OnSkin, 
		this);
}

void PlayerIcon::Update(const GamePtr &game, const wxInt32 index)
{
	// See if we need to be visible.
	wxBOOL show = mUpdateFunc(game, index);

	mBitmap->Show(show || false);
}

void PlayerIcon::Resize(const wxPoint &origin, const wxSize &bitmapSize)
{
	wxSize oldSize = mBitmap->GetSize();

	//first off, resize the bitmap
	mBitmap->SetSize(origin.x, origin.y, bitmapSize.GetWidth(), 
		bitmapSize.GetHeight());

	// Finally, update the bitmap, if needed.
	if(oldSize != bitmapSize)
	{
		wxImage image = IMAGE.GetImage(shPlayerIcon, mKey, bitmapSize);
		wxBitmap bitmap(image);
		bitmap.SetMask(new wxMask(bitmap, SKIN.Element(shIconTransparent)));

		mBitmap->SetBitmap(bitmap);
	}
}

void PlayerIcon::ResetBitmap()
{
	const wxSize &size = mBitmap->GetSize();
	if(wxSize(0, 0) != size)
	{
		wxImage image = IMAGE.GetImage(shPlayerIcon, mKey, size);
		wxBitmap bitmap(image);
		bitmap.SetMask(new wxMask(bitmap, SKIN.Element(shIconTransparent)));

		mBitmap->SetBitmap(bitmap);
	}
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void PlayerIcon::OnSkin(wxInt32)
{
	mBitmap->SetBackgroundColour(SKIN.Element(shBackground));
}
