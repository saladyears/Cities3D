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
#include "ListSelection.h"
#include "BaseListCtrl.h"
#include "SkinManager.h"
#include "ImageEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
struct wxListSelection::Impl
{
	Impl(const size_t index, const size_t subItemsPerRow, const HashString &icon,
		const HashString &mainFont, const HashString &subFont)
		: mIndex(index)
		, mSubItemsPerRow(subItemsPerRow)
		, mIsHighlight(false)
		, mIsSelected(false)
		, mMainFontName(mainFont)
		, mSubFontName(subFont)
		, mIcon(icon)
	{}

	// The index of this selection.
	size_t mIndex;

	// The number of items to display per row in the main text.
	size_t mSubItemsPerRow;

	// Whether this selection is highlighted.
	bool mIsHighlight;

	// Whether this selection is selected.
	bool mIsSelected;

	// The fonts associated with this selection.
	HashString mMainFontName;
	HashString mSubFontName;

	// The icon to display with this selection.
	HashString mIcon;

	// The graphic that corresponds to the icon.
	wxBitmap mBitmap;

	// The starting x,y points of the text items.
	typedef std::vector<wxPoint> PositionArray;
	PositionArray mPoints;

	// The fields to display in the window.
	WX_DECLARE_STRING_HASH_MAP(wxString, StringHash);
	StringHash mFields;

	// Display fonts.
	wxFont mMainFont;
	wxFont mSubFont;

	// Display colors.
	wxColour mMainText;
	wxColour mBackground;
	wxColour mHighlight;
	wxColour mSelected;
	wxColour mHighlightAndSelected;
	wxColour mMainTextShadow;
	wxColour mMainTextDisabled;
	wxColour mSubText;
	wxColour mSubTextShadow;
	wxColour mTransparent;

	wxInt32 mImageBufferY;
};

namespace
{
	const wxInt32 sLeftIndent = 8;
	const wxInt32 sImageBufferX = 4;
	const wxInt32 sTopIndent = 3;
	const wxInt32 sBottomIndent = 3;
	const wxInt32 sSubItemSpace = 1;

	void ForwardMouseEvent_ListSelection(wxMouseEvent &event, wxWindow *window, 
		wxWindow *parent)
	{
		wxASSERT(NULL != window);
		wxASSERT(NULL != parent);

		// Set the point as parent coordinates.
		wxPoint pos = event.GetPosition();
		window->ClientToScreen(&pos.x, &pos.y);
		parent->ScreenToClient(&pos.x, &pos.y);
		event.m_x = pos.x;
		event.m_y = pos.y;

		parent->ProcessEvent(event);
	}
}

//---------------------------- PUBLIC           -----------------------------//
wxListSelection::wxListSelection(wxBaseListCtrl *parent, const size_t index, 
		const StringArray &array, const wxString &data, 
		const size_t subItemsPerRow, const HashString &mainFont,
		const HashString &subFont, const HashString &icon)
: wxOwnerDrawWindow(parent, -1)
, mpImpl(new Impl(index, subItemsPerRow, icon, mainFont, subFont))
, mParent(parent)
{
	wxASSERT(NULL != mParent);

	Init(array, data);
}

wxListSelection::wxListSelection(wxBaseListCtrl *parent, const size_t index, 
		const StringArray &array, const wxString &data, 
		const size_t subItemsPerRow, const HashString &mainFont, 
		const HashString &subFont, const wxImage &image)
: wxOwnerDrawWindow(parent, -1)
, mpImpl(new Impl(index, subItemsPerRow, shEmpty, mainFont, subFont))
, mParent(parent)
{
	wxASSERT(NULL != mParent);

	// Set the image directly.
	mpImpl->mBitmap = wxBitmap(image);

	Init(array, data);
}

void wxListSelection::SetIndex(const size_t index)
{
	mpImpl->mIndex = index;
}

void wxListSelection::SetSelected(const bool selected)
{
	mpImpl->mIsSelected = selected;
	Refresh();
}

void wxListSelection::SetItemText(const wxString &field, const wxString &text)
{
	// Make sure the field exists.
	Impl::StringHash::iterator it = mpImpl->mFields.find(field);

	if(mpImpl->mFields.end() != it)
	{
		it->second = text;

		// Calculate height even for empty strings.
		if(true == text.IsEmpty())
		{
			it->second = swAy;
		}

		SetMinimumSize();

		if(true == text.IsEmpty())
		{
			it->second = swEmpty;
		}

		wxSizer *sizer = mParent->GetSizer();
		if(NULL != sizer)
		{
			sizer->Layout();
		}

		Refresh();
	}
}

const wxString &wxListSelection::GetItemText(const wxString &field) const
{
	return mpImpl->mFields[field];
}

void wxListSelection::SetItemColor(const wxColour color)
{
	mpImpl->mMainText = color;
	Refresh();
}

wxColour wxListSelection::GetItemColor() const
{
	return mpImpl->mMainText;
}

void wxListSelection::OnSkin()
{
	mpImpl->mMainFont = SKIN.Font(mpImpl->mMainFontName);
	mpImpl->mSubFont = SKIN.Font(mpImpl->mSubFontName);

	mpImpl->mBackground = SKIN.Element(shBackground);
	mpImpl->mHighlight = SKIN.Element(shListHighlight);
	mpImpl->mSelected = SKIN.Element(shListSelection);
	mpImpl->mHighlightAndSelected = SKIN.Element(shListHighlightAndSelection);
	mpImpl->mMainText = SKIN.Element(shListMainText);
	mpImpl->mMainTextShadow = SKIN.Element(shListMainTextShadow);
	mpImpl->mMainTextDisabled = SKIN.Element(shListMainTextDisabled);
	mpImpl->mSubText = SKIN.Element(shListSubText);
	mpImpl->mSubTextShadow = SKIN.Element(shListSubTextShadow);
	mpImpl->mTransparent = SKIN.Element(shTransparent);

	if(false == mpImpl->mIcon.empty())
	{
		wxImage thisImage = IMAGE.GetImage(shIcons, mpImpl->mIcon);

		mpImpl->mBitmap = wxBitmap(thisImage);
	}

	if(true == mpImpl->mBitmap.Ok())
	{
		mpImpl->mBitmap.SetMask(new wxMask(mpImpl->mBitmap, 
			SKIN.Element(shIconTransparent)));
	}

	SetMinimumSize();
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxListSelection, wxOwnerDrawWindow)
	EVT_ENTER_WINDOW(wxListSelection::OnEnterWindow)
	EVT_LEAVE_WINDOW(wxListSelection::OnLeaveWindow)
	EVT_MOTION(wxListSelection::OnMouseMove)
	EVT_LEFT_DOWN(wxListSelection::OnLeftDown)
	EVT_LEFT_UP(wxListSelection::OnLeftUp)
	EVT_LEFT_DCLICK(wxListSelection::OnLeftDoubleClick)
	EVT_MOUSEWHEEL(wxListSelection::OnMouseWheel)
    EVT_KEY_DOWN(wxListSelection::OnKeyDown)
END_EVENT_TABLE()

void wxListSelection::Init(const StringArray &array, const wxString &data)
{
	// Initialize the data fields.
	for(size_t i = 0; i < array.size(); ++i)
	{
		mpImpl->mFields[array[i]] = swEmpty;
	}

	// Set the primary data.
	mpImpl->mFields[array[0]] = data;

	// If the primary information is empty, add some temp text for calculating
	// the height correctly.
	if(true == data.IsEmpty())
	{
		mpImpl->mFields[array[0]] = swAy;
	}

	// Grab skin values.
	OnSkin();

	if(true == data.IsEmpty())
	{
		mpImpl->mFields[array[0]] = swEmpty;
	}

	SetCursor(wxCursor(wxCURSOR_HAND));
}

void wxListSelection::SetMinimumSize()
{
	mpImpl->mPoints.clear();

	// Determine how large of an area we require to draw this selection.
	const StringArray &displayOrder = mParent->GetDisplayOrder();
	wxASSERT(false == displayOrder.empty());

	// Main item size.
	wxClientDC dc(this);
	dc.SetFont(mpImpl->mMainFont);
	
	wxInt32 w, h;
	wxInt32 width = 0, height = 0;
	wxInt32 x = sLeftIndent, y = sTopIndent;
	mpImpl->mImageBufferY = 4;

	// Things are different if we have an image to work with.
	if(true == mpImpl->mBitmap.Ok())
	{
		x += mpImpl->mBitmap.GetWidth() + sImageBufferX;
		width = x;
	}

	mpImpl->mPoints.push_back(wxPoint(x, y));

	// The main item gets its own row.
	dc.GetTextExtent(mpImpl->mFields[displayOrder[0]], &w, &h);

	// For some reason, wx doesn't always get the full width right, so
	// buffer it a bit.
	w = static_cast<wxInt32>(w * 1.1f);

	// Add top and bottom indent pixels.
	width += w + sLeftIndent;
	height = h + sTopIndent + sBottomIndent;

	// Now go through each sub item and calculate any added height or width
	// required based on how many sub items are allowed per row.
	size_t subItem = 0;
	dc.SetFont(mpImpl->mSubFont);
	
	for(size_t i = 1; i < displayOrder.size(); ++i)
	{
		// If the subItem is zero, it means we're on a new row and need to
		// calculate height and width values for the row.
		if(0 == subItem)
		{
			x = sLeftIndent;
			y += h + sSubItemSpace;

			if(true == mpImpl->mBitmap.Ok())
			{
				x += mpImpl->mBitmap.GetWidth() + sImageBufferX;
			}

			mpImpl->mPoints.push_back(wxPoint(x, y));

			bool empty = mpImpl->mFields[displayOrder[i]].IsEmpty();

			if(true == empty)
			{
				mpImpl->mFields[displayOrder[i]] = swAy;
			}

			dc.GetTextExtent(mpImpl->mFields[displayOrder[i]], &w, &h);

			if(true == empty)
			{
				mpImpl->mFields[displayOrder[i]] = swEmpty;
			}

			// For some reason, wx doesn't always get the full width right, so
			// buffer it a bit.
			if(1 == mpImpl->mSubItemsPerRow)
			{
				w = static_cast<wxInt32>(w * 1.1f);
			}
			else
			{
				w = static_cast<wxInt32>(w * 1.5f);
			}

			// Add two pixels of padding between each row.
			height += (h + sSubItemSpace);

			width = std::max(width, w + sLeftIndent);
		}
		else
		{
			x += w;

			mpImpl->mPoints.push_back(wxPoint(x, y));

			dc.GetTextExtent(mpImpl->mFields[displayOrder[i]], &w, &h);

			width = std::max(width, x + w);
		}

		++subItem;

		// See if the next item belongs on a new row.
		if(mpImpl->mSubItemsPerRow <= subItem)
		{
			subItem = 0;
		}
	}

	// If the image height with buffers is greater than the text height, we
	// need to adjust all of the heights calculated to center them on the 
	// image.
	if(true == mpImpl->mBitmap.Ok())
	{
		wxInt32 imageHeight = mpImpl->mBitmap.GetHeight() + 
			(mpImpl->mImageBufferY * 2);

		if(imageHeight > height)
		{
			wxInt32 dif = (imageHeight - height) / 2;

			std::transform(mpImpl->mPoints.begin(), mpImpl->mPoints.end(), 
				mpImpl->mPoints.begin(), std::bind2nd(std::plus<wxPoint>(), 
				wxPoint(0, dif)));

			height = imageHeight;
		}
		// Otherwise, we need to center the image location.
		else if(height > imageHeight)
		{
			mpImpl->mImageBufferY = (height - mpImpl->mBitmap.GetHeight()) / 2;
		}
	}

	SetMinSize(wxSize(width, height));
}

void wxListSelection::Paint(wxMemoryDC &dc)
{
	const StringArray &displayOrder = mParent->GetDisplayOrder();
	wxASSERT(false == displayOrder.empty());
	wxASSERT(displayOrder.size() == mpImpl->mPoints.size());

	wxSize size = GetSize();
	wxColour background = mpImpl->mBackground;

	// Set background color.
	if(mpImpl->mIsHighlight && mpImpl->mIsSelected)
	{
		background = mpImpl->mHighlightAndSelected;
	}
	else if(mpImpl->mIsHighlight)
	{
		background = mpImpl->mHighlight;
	}
	else if(mpImpl->mIsSelected)
	{
		background = mpImpl->mSelected;
	}

	dc.SetPen(wxPen(background));
	dc.SetBrush(wxBrush(background));
	dc.DrawRectangle(0, 0, size.x, size.y);

	// Bitmap (if any)
	if(true == mpImpl->mBitmap.Ok())
	{
		dc.DrawBitmap(mpImpl->mBitmap, sImageBufferX, mpImpl->mImageBufferY, true);
	}

	// Draw text.
	dc.SetFont(mpImpl->mMainFont);
	wxColour shadow = mpImpl->mMainTextShadow;
	wxColour main = IsEnabled() ? mpImpl->mMainText : 
		mpImpl->mMainTextDisabled;
	for(size_t i = 0; i < displayOrder.size(); ++i)
	{
		if(1 == i)
		{
			dc.SetFont(mpImpl->mSubFont);
			shadow = mpImpl->mSubTextShadow;
			main = mpImpl->mSubText;
		}

		const wxString &str = mpImpl->mFields[displayOrder[i]];

		if(false == str.IsEmpty())
		{
			const wxPoint &point = mpImpl->mPoints[i];

			if(shadow != mpImpl->mTransparent)
			{
				dc.SetTextForeground(shadow);
				dc.DrawText(str, point.x + 1, point.y + 1);
			}
			dc.SetTextForeground(main);
			dc.DrawText(str, point.x, point.y);
		}
	}
}

void wxListSelection::OnEnterWindow(wxMouseEvent &)
{
	mpImpl->mIsHighlight = true;
	Refresh();
}

void wxListSelection::OnLeaveWindow(wxMouseEvent &)
{
	mpImpl->mIsHighlight = false;
	Refresh();
}

void wxListSelection::OnMouseMove(wxMouseEvent &event)
{
	ForwardMouseEvent_ListSelection(event, this, mParent);
}

void wxListSelection::OnLeftDown(wxMouseEvent &event)
{
	mParent->SetFocus();

	ForwardMouseEvent_ListSelection(event, this, mParent);
}

void wxListSelection::OnLeftUp(wxMouseEvent &event)
{
	ForwardMouseEvent_ListSelection(event, this, mParent);
}

void wxListSelection::OnLeftDoubleClick(wxMouseEvent &event)
{
	ForwardMouseEvent_ListSelection(event, this, mParent);
}

void wxListSelection::OnMouseWheel(wxMouseEvent &event)
{
	ForwardMouseEvent_ListSelection(event, this, mParent);
}

void wxListSelection::OnKeyDown(wxKeyEvent &event)
{
    KEYMAP.ProcessKeyEvent(event);

    event.Skip(false);
}
