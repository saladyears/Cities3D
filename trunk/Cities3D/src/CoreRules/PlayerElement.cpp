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
#include "PlayerElement.h"
#include "Controller.h"
#include "SkinManager.h"
#include "TextPopup.h"
#include "BaseListCtrl.h"
#include "DataObject.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	class wxPopupBitmapElement
		: public wxStaticBitmap
		, public TextPopup
	{
	public:
	#ifdef __WIN32__
		//disables the 'this' : used in base member initializer list warning
		#pragma warning(disable:4355)
	#endif
		wxPopupBitmapElement(const wxString& text, wxWindow* parent, wxWindowID id, 
			const wxBitmap& label)
		: wxStaticBitmap(parent, id, label, wxDefaultPosition, wxSize(0, 0))
		, TextPopup(this, text)
		{}
	};

	struct no_delete_player_element
	{
		void operator()(void *){}
	};

	wxString sFields_PlayerElement[] = {swCards};

	class CardPopup : public TextPopup
	{
	public:
		CardPopup(wxWindow* window, const HashString& card, wxInt32 index) 
			: TextPopup(window, stThatWouldBeCheating) 
			, mCard(card)
			, mIndex(index)
		{}

		// Card popups are always visisble.
		PopupType Type() const
		{
			return PopupAlwaysVisible;
		}

		virtual void CreatePopup(wxWindow* window, wxSizer* sizer) const
		{
			DataObject input(mCard, mIndex), output;
			RULE.Decide(shLogicCardList, input, output);
			
			bool valid = output.read<bool>();

			if(true == valid)
			{
				wxASSERT(3 <= output.numItems());

				const StringArray& playable = output.read<StringArray>(1);
				const StringArray& purchased = output.read<StringArray>(2);

				wxBaseListCtrl* list = new wxBaseListCtrl(window, -1, 
					sFields_PlayerElement, 1, shPopupFont, shPopupFont, 
					wxDefaultPosition, wxDefaultSize, 1, 0);

				// Add the cards to the list.
				StringArray::const_iterator it, itEnd = playable.end();
				for(it = playable.begin(); it != itEnd; ++it)
				{
					list->AppendItem(*it);
				}

				itEnd = purchased.end();
				for(it = purchased.begin(); it != itEnd; ++it)
				{
					size_t index = list->AppendItem(*it);
					list->SetItemColor(index, wxColour(128, 128, 128));
				}

				wxSize size = list->GetListBestSize();
				sizer->SetMinSize(size);
				sizer->Add(list, 0, wxEXPAND | wxALL, 4);
				sizer->AddSpacer(2);
			}
			else
			{
				// Treat it just like a text popup if they can't see the cards.
				TextPopup::CreatePopup(window, sizer);
			}
		}

	private:
		HashString mCard;
		wxInt32 mIndex;
	};

	class wxCardBitmap
		: public wxStaticBitmap
		, public CardPopup
	{
	public:
		wxCardBitmap(const HashString& card, wxInt32 index, wxWindow* parent, 
			wxWindowID id, const wxBitmap& label)
		: wxStaticBitmap(parent, id, label, wxDefaultPosition, wxSize(0, 0))
		, CardPopup(this, card, index)
		{}
	};
}

//---------------------------- PUBLIC           -----------------------------//
void PlayerElement::Show(const bool show)
{
	mShowExternal = show;
	ShowPrivate();
}

void PlayerElement::SetFont(const wxFont &font)
{
	mText->SetFont(font);
}

void PlayerElement::SetRight(const bool right)
{
	mRight = right;
}

void PlayerElement::Update(const GamePtr &game, const wxInt32 index)
{
	//call the update function to get and set the text
	wxInt32 val = mUpdateFunc(game, index);
	mText->SetLabel(wxString::Format(swIntFormat.c_str(), val));

	//check color
	if(NULL != mColorFunc)
	{
		mText->SetForegroundColour(mColorFunc(game, index));
	}

	//set threshold
	mShowInternal = (mThreshold < val);
	ShowPrivate();
}

void PlayerElement::Resize(const wxPoint &origin, const wxSize &bitmapSize, 
	const wxSize textSize)
{
	wxSize oldSize = mBitmap->GetSize();

	//first off, resize the bitmap
	mBitmap->SetSize(origin.x, origin.y, bitmapSize.GetWidth(), 
		bitmapSize.GetHeight());

	wxPoint point;

	//now calcluate the new location for the text
	if(true == mRight)
	{
		//point y should be centered on the bitmap
		point.y = origin.y + ((bitmapSize.GetHeight() - 
			textSize.GetHeight()) >> 1);

		//point x should be just to the right of the bitmap
		point.x = origin.x + bitmapSize.GetWidth();
	}
	else
	{
		//point y should be directly below the bitmap
		point.y = origin.y + bitmapSize.GetHeight();

		//point x should be centered on the bitmap
		point.x = origin.x + ((bitmapSize.GetWidth() - 
			textSize.GetWidth()) >> 1) + 1;
	}

	//set the new position for the text
	mText->SetSize(point.x, point.y, textSize.GetWidth(), 
		textSize.GetHeight());

	// Finally, update the bitmap, if needed.
	if(oldSize != bitmapSize)
	{
		mBitmap->SetBitmap(GetBitmap(bitmapSize));
	}
}

void PlayerElement::ResetBitmap()
{
	const wxSize &size = mBitmap->GetSize();
	if(wxSize(0, 0) != size)
	{
		mBitmap->SetBitmap(GetBitmap(mBitmap->GetSize()));
	}
}

//---------------------------- PROTECTED        -----------------------------//
PlayerElement::PlayerElement(wxWindow *parent, const PlayerUpdateFunc &update, 
	const PlayerColorFunc &color, bool right, const wxInt32 threshold,
	const wxString& text, bool card, wxInt32 index)
: mUpdateFunc(update)
, mColorFunc(color)
, mRight(right)
, mThreshold(threshold)
, mShowInternal(true)
, mShowExternal(true)
{
	wxASSERT(NULL != parent);
	wxASSERT(NULL != mUpdateFunc);

	if(true == card)
	{
		mBitmap = StaticBitmapPtr(
			new wxCardBitmap(HashString(text), index, parent, 0, 
			wxBitmap(1, 1)), no_delete_player_element());
	}
	else
	{
		mBitmap = StaticBitmapPtr(
			new wxPopupBitmapElement(text, parent, 0, wxBitmap(1, 1)), no_delete_player_element());
	}

	mText = StaticTextPtr(
		new wxStaticText(parent, 0, sw0, wxDefaultPosition, 
		wxDefaultSize, wxALIGN_CENTRE | wxST_NO_AUTORESIZE), no_delete_player_element());
	
	mBitmap->Show(false);
	mText->Show(false);

	OnSkin();

	Controller::get().AddReceiver(shEventSkin, &PlayerElement::OnSkin, 
		this);
}

PlayerElement::~PlayerElement()
{
	Controller::get().RemoveReceiver(shEventSkin, &PlayerElement::OnSkin, 
		this);
}

//---------------------------- PRIVATE          -----------------------------//
void PlayerElement::OnSkin(wxInt32)
{
	mText->SetBackgroundColour(SKIN.Element(shBackground));

	// If we have a color function, it has to be updated by its own self.
	if(NULL == mColorFunc)
	{
		mText->SetForegroundColour(SKIN.Element(shPlayerUICounterText));
	}
}

void PlayerElement::ShowPrivate()
{
	bool show = mShowExternal && mShowInternal;
	mBitmap->Show(show);
	mText->Show(show);
}
