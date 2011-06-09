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
#include "ImageStrip.h"
#include "ImageEngine.h"
#include "SkinManager.h"
#include "Image.h"
#include "Controller.h"
#include "Utility.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	void ForwardMouseEvent_ImageStrip(wxMouseEvent &event, wxWindow *window, 
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

const HashString wxImageStrip::EmptyResource  = shEmptyResource;

//---------------------------- PUBLIC           -----------------------------//
wxImageStrip::wxImageStrip(wxWindow* parent, wxWindowID id,
	const size_t maxImages, const size_t minTextHeight, const HashString& key,
	const HashString& sizeImage, float ratio)
: wxOwnerDrawWindow(parent, id)
, mKey(key)
, mCurImages(0)
, mMaxImages(maxImages)
, mPixelSpacing(1)
, mCurrentImageSize(wxSize(0, 0))
, mMinTextHeight(minTextHeight)
, mSizeRatio(1.0f)
, mFlags(0)
{
	SetBackgroundColour(SKIN.Element(shBackground));

	mTextFont = SKIN.Font(shInfoFont);

	wxClientDC dc(this);
	Utility::calculateFontSize(dc, mTextFont, mMinTextHeight);

	// Calculate the minimum image size.
	wxImage image = IMAGE.GetImage(mKey, sizeImage);

	mMinImageSize = wxSize(
		static_cast<wxInt32>(image.GetWidth() * ratio), 
		static_cast<wxInt32>(image.GetHeight() * ratio));
	mCurrentImageSize = wxSize(image.GetWidth(), image.GetHeight());

	mImageEntryArray.resize(mMaxImages);

	Controller::get().AddReceiver(shEventImageUpdate, 
		&wxImageStrip::OnImageUpdate, this);
}

wxImageStrip::~wxImageStrip()
{
	Controller::get().RemoveReceiver(shEventImageUpdate, 
		&wxImageStrip::OnImageUpdate, this);
}

void wxImageStrip::Clear()
{
	mImageEntryArray.clear();
	mImageEntryArray.resize(mMaxImages);

	mCurImages = 0;

	Refresh();
}

void wxImageStrip::Add(const HashString &resource, const wxInt32 count)
{
	wxImage imageBitmap = IMAGE.GetImage(mKey, resource);
	imageBitmap = Image::Resize(imageBitmap, mCurrentImageSize);
	
	wxASSERT(mCurImages < mMaxImages);

	size_t i = 0;

	// Find out where the image should go.
	for(; i < mMaxImages; ++i)
	{
		if(EmptyResource == mImageEntryArray[i].resource)
		{
			break;			
		}
	}
	wxASSERT(i != mMaxImages);

	++mCurImages;

	mImageEntryArray[i].resource = resource;
	mImageEntryArray[i].count = count;
	mImageEntryArray[i].bitmap = wxBitmap(imageBitmap);

	Refresh();
}

void wxImageStrip::Remove(const HashString &resource, bool adjustMax)
{
	wxASSERT(false == resource.empty());

	// Use erase here so that ordering is preserved.
	ImageEntryArray::iterator it, itEnd = mImageEntryArray.end();
	for(it = mImageEntryArray.begin(); it != itEnd; ++it)
	{
		if(resource == (it->resource))
		{
			mImageEntryArray.erase(it);
			if(true == adjustMax)
			{
				--mMaxImages;
			}
			else
			{
				mImageEntryArray.push_back(ImageEntry());
			}

			--mCurImages;

			break;
		}
	}

	Refresh();
}

void wxImageStrip::SetResourceCount(const HashString &resource, 
	const wxInt32 count)
{
	for(size_t i = 0; i < mMaxImages; ++i)
	{
		if(mImageEntryArray[i].resource == resource)
		{
			mImageEntryArray[i].count = count;
			break;
		}
	}
	
	Refresh();
}

wxInt32 wxImageStrip::GetResourceCount(const HashString &resource) const
{
	wxInt32 count = -1;

	for(size_t i = 0; i < mMaxImages; ++i)
	{
		if(mImageEntryArray[i].resource == resource)
		{
			count = mImageEntryArray[i].count;
			break;
		}
	}

	return count;
}

wxInt32 wxImageStrip::GetResourceCount() const
{
	wxInt32 count = 0;

	for(size_t i = 0; i < mMaxImages; ++i)
	{
		if(EmptyResource != mImageEntryArray[i].resource)
		{
			count += mImageEntryArray[i].count;
		}
	}

	return count;
}

HashString wxImageStrip::GetImage(const wxPoint& pos) const
{
	HashString resource;
	wxInt32 index = -1;

	wxInt32 testX = pos.x;
	wxInt32 testY = pos.y;

	// Adjust for centering.
	if(FlagAlignCenter & mFlags)
	{
		wxInt32 width = GetSize().x;
		testX -= ((width - (mMaxImages * 
			(mCurrentImageSize.x + mPixelSpacing - 1)))) / 2;
	
		wxInt32 height = GetSize().y;
		testY -= (height - (mCurrentImageSize.y + mMinTextHeight)) / 2;
	}

	// Determine which image index the point is above (if any).
	if( (0 <= testY) &&
		(mCurrentImageSize.y >= testY))
	{
		wxInt32 x = 0;

		for(size_t i = 0; (i < mMaxImages) && (x <= testX); ++i)
		{
			if( (x <= pos.x) &&
				(x + mCurrentImageSize.x >= testX))
			{
				index = i;
				break;
			}

			x += mCurrentImageSize.x + mPixelSpacing;
		}
	}

	if(-1 != index)
	{
		if(FlagReverseOrder & mFlags)
		{
			index = mMaxImages - index - 1;
		}

		resource = mImageEntryArray[index].resource;
	}

	return resource;
}

void wxImageStrip::FillResourceMap(ResourceMap& map) const
{
	map.clear();

	for(size_t i = 0; i < mMaxImages; ++i)
	{
		const HashString& resource = mImageEntryArray[i].resource;
		if(EmptyResource != resource)
		{
			map[resource] = mImageEntryArray[i].count;
		}
	}
}

void wxImageStrip::SetResourceMap(const ResourceMap& map)
{
	Clear();

	ResourceMap::const_iterator it, itEnd = map.end();
	for(it = map.begin(); it != itEnd; ++it)
	{
		const HashString& res = it->first;
		wxInt32 amount = it->second;

		Add(res, amount);
	}

	Refresh();
}

//---------------------------- PROTECTED        -----------------------------//
void wxImageStrip::Size(wxSizeEvent &event)
{
	// Determine how much the window has grown in comparison to the minimum
	// size.
	wxSize currentSize = event.GetSize();

	// If we get a (-1, -1), force size to the normal card size.
	bool forceSize = (currentSize == wxSize(-1, -1));
	wxSize oldMinSize = mMinImageSize;
	
	if(true == forceSize)
	{
		mMinImageSize = mCurrentImageSize;
	}

	// Set the minimum size.  This should ideally only be done once, but since
	// we need to allow derived classes to add their own size requirements,
	// this can't be done in the constructor, so we do it in a place that is
	// guaranteed to be called at least once.
	wxSize minSize;
	CalcluateMinSize(minSize);
	SetMinSize(minSize);

	if(wxSize(0, 0) == currentSize)
	{
		currentSize = minSize;
	}

	if(true == forceSize)
	{
		mMinImageSize = oldMinSize;
		currentSize = mCurrentImageSize;
	}

	// Since we need to preserve aspect ratio, resize the bitmaps only by
	// the minimum x or y delta.
	float xDelta = static_cast<float>(currentSize.x) / 
		static_cast<float>(minSize.x);
	float yDelta = static_cast<float>(currentSize.y) / 
		static_cast<float>(minSize.y);
	mSizeRatio = std::min(xDelta, yDelta);

	wxSize oldSize = mCurrentImageSize;
	mCurrentImageSize.x = static_cast<wxInt32>(mMinImageSize.x * mSizeRatio);
	mCurrentImageSize.y = static_cast<wxInt32>(mMinImageSize.y * mSizeRatio);

	// Resize all of the bitmaps if needed.
	if(oldSize != mCurrentImageSize)
	{
		ResizeBitmaps();

		// Recalculate font size.
		size_t textHeight = static_cast<size_t>(mMinTextHeight * mSizeRatio);
		wxClientDC dc(this);
		Utility::calculateFontSize(dc, mTextFont, textHeight);
	}

	Refresh();
}

void wxImageStrip::CalcluateMinSize(wxSize &size)
{
	// Calculate the width taken up by the maximum number of bitmaps with
	// one less times the amount of padding between them.
	size_t w = (mMaxImages * (mMinImageSize.x + mPixelSpacing - 1));
	
	// The minimum height is the height of one bitmap plus the minimum image
	// text height.
	size_t h = mMinImageSize.y;

	if(0 == (FlagAlternateDraw & mFlags))
	{
		h += mMinTextHeight;
	}

	w += 2;
	h += 2;

	size = wxSize(w, h);
}

void wxImageStrip::DrawImages(wxDC &dc, size_t x, size_t y) const
{
	dc.SetFont(mTextFont);

	// Adjust for alignment, if needed.
	if(FlagAlignCenter & mFlags)
	{
		wxInt32 width = GetSize().x;
		x += ((width - (mMaxImages * 
			(mCurrentImageSize.x + mPixelSpacing - 1)))) / 2;

		wxInt32 height = GetSize().y;
		y += (height - (mCurrentImageSize.y + mMinTextHeight)) / 2;
	}

	bool reverse = (FlagReverseOrder & mFlags) || false;

	wxInt32 drawX = x;
	wxInt32 width = mCurrentImageSize.x + mPixelSpacing;

	if(true == reverse)
	{
		drawX = GetSize().x - x - mCurrentImageSize.x;
		width = -width;
	}

	bool once = false;

	for(size_t i = 0; i < mMaxImages; ++i)
	{
		const HashString& resource = mImageEntryArray[i].resource;
		if(EmptyResource != resource)
		{
			wxInt32 count = mImageEntryArray[i].count;

			if( (0 != count) ||
				(0 == (FlagHideZeroes & mFlags)))
			{		
				// Bitmap
				const wxBitmap &bitmap = mImageEntryArray[i].bitmap;
				dc.DrawBitmap(bitmap, drawX, y);
				
				// If this is the alternate draw method, draw more bitmaps.
				if( (1  < count) &&
					(FlagAlternateDraw & mFlags))
				{
					for(wxInt32 i = 0; i < (count - 1); ++i)
					{
						drawX += (mCurrentImageSize.x / 4);

						dc.DrawBitmap(bitmap, drawX, y);
					}
				}

				// Text
				if( (0 < count) &&
					(0 == (FlagHideText & mFlags)) &&
					(0 == (FlagAlternateDraw & mFlags)))
				{
					wxColour color = SKIN.Element(shDefaultText);
					if(mCounterColorFunc)
					{
						color = mCounterColorFunc(resource);
					}

					dc.SetTextForeground(color);
				
					wxString str = wxString::Format(swIntFormat, count);
					wxCoord w, h;
					dc.GetTextExtent(str, &w, &h);
					dc.DrawText(str, drawX + ((bitmap.GetWidth() - w) / 2), 
						y + bitmap.GetHeight());
				}
			}
		}
		// Otherwise, if we have the draw one outline flag on, draw an outline
		// around the first open resource area.
		else if((FlagOneOutline & mFlags) &&
				(0 == (FlagDisabled & mFlags)) &&
				(false == once))
		{
			wxColour background = SKIN.Element(shBackground);
			wxColour outline = SKIN.Element(shTradeCardOutline);
			dc.SetBrush(wxBrush(background));
			dc.SetPen(wxPen(outline));

			dc.DrawRectangle(drawX, y, mCurrentImageSize.x, 
				mCurrentImageSize.y);
			once = true;
		}

		drawX += width;
	}
}

void wxImageStrip::OnMouseMove(wxMouseEvent &event)
{
	if(FlagForwardMouseEvents & mFlags)
	{
		ForwardMouseEvent_ImageStrip(event, this, GetParent());
	}
}

void wxImageStrip::OnLeftDown(wxMouseEvent &event)
{
	if(FlagForwardMouseEvents & mFlags)
	{
		ForwardMouseEvent_ImageStrip(event, this, GetParent());
	}

	event.Skip();
}

void wxImageStrip::OnLeftUp(wxMouseEvent &event)
{
	if(FlagForwardMouseEvents & mFlags)
	{
		ForwardMouseEvent_ImageStrip(event, this, GetParent());
	}
}

void wxImageStrip::OnRightUp(wxMouseEvent &event)
{
	if(FlagForwardMouseEvents & mFlags)
	{
		ForwardMouseEvent_ImageStrip(event, this, GetParent());
	}
}

void wxImageStrip::OnLeftDoubleClick(wxMouseEvent &event)
{
	if(FlagForwardMouseEvents & mFlags)
	{
		ForwardMouseEvent_ImageStrip(event, this, GetParent());
	}
}

void wxImageStrip::OnRightDoubleClick(wxMouseEvent &event)
{
	if(FlagForwardMouseEvents & mFlags)
	{
		ForwardMouseEvent_ImageStrip(event, this, GetParent());
	}
}

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxImageStrip, wxOwnerDrawWindow)
	EVT_MOTION(wxImageStrip::OnMouseMove)
	EVT_LEFT_DOWN(wxImageStrip::OnLeftDown)
	EVT_LEFT_UP(wxImageStrip::OnLeftUp)
	EVT_RIGHT_UP(wxImageStrip::OnRightUp)
	EVT_LEFT_DCLICK(wxImageStrip::OnLeftDoubleClick)
	EVT_RIGHT_DCLICK(wxImageStrip::OnRightDoubleClick)
END_EVENT_TABLE()

void wxImageStrip::Paint(wxMemoryDC &dc)
{
	wxColour background = GetBackgroundColour();
	dc.SetBrush(wxBrush(background));
	dc.SetPen(wxPen(background));

	const wxSize &size = GetSize();
	dc.DrawRectangle(0, 0, size.x, size.y);

	DrawImages(dc, 0, 0);
}

void wxImageStrip::ResizeBitmaps()
{
	for(size_t i = 0; i < mMaxImages; ++i)
	{
		const HashString &resource = mImageEntryArray[i].resource;
		if(EmptyResource != resource)
		{
			// Now retrieve the new bitmap and resize it.
			wxImage imageBitmap = IMAGE.GetImage(mKey, resource);
			imageBitmap = Image::Resize(imageBitmap, mCurrentImageSize);
			wxBitmap bitmap = wxBitmap(imageBitmap);

			mImageEntryArray[i].bitmap = bitmap;
		}
	}
}

void wxImageStrip::OnImageUpdate(bool)
{
	// Reget all of the bitmaps.
	ResizeBitmaps();

	Refresh();
}
