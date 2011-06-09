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
#include "BaseImageButton.h"
#include "SkinManager.h"
#include "Controller.h"
#include "ImageEngine.h"
#include "Utility.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	const float fTextPercent = .70f;
	const float fTextMaxWidth = .95f;
	const float fTextMaxHeight = 1.0f;
	const wxInt32 sMultiLineBuffer = -1;

	wxColour sButtonText;
	wxColour sButtonTextShadow;
	wxColour sTransparent;

	wxPoint calculateTextPosition(const wxInt32 w, const wxInt32 h, const
		wxRect &rect)
	{
		wxPoint pos;

		// Finally, set the proper location for text output.
		pos.x = ((rect.GetWidth() - w) >> 1) + 1;
		pos.y = (rect.GetHeight() - h) >> 1;

		return pos;
	}
}

//---------------------------- PUBLIC           -----------------------------//
wxBaseImageButton::wxBaseImageButton(wxWindow* parent, wxWindowID id, 
	const wxString& label, const wxSize size, const HashString &key,
	const HashString &enabled, const HashString &disabled)
: wxBaseBitmapButton(parent, id, wxDefaultPosition, size)
, mKey(key)
, mEnabled(enabled)
, mDisabled(disabled)
, mLabel(label)
{
	// Set up the colors
	OnSkin();
}

void wxBaseImageButton::SetLabel(const wxString &label)
{
	mLabel = label;
	mDrawFont = mFont;

	mLabelArray.clear();

	// Determine if this label will actually fit into the width of the
	// button.
	wxClientDC dc(this);
	dc.SetFont(mFont);

	wxInt32 w, h;
	dc.GetTextExtent(mLabel, &w, &h);

	wxRect rect = GetRect();
	wxInt32 width = rect.GetWidth();
	wxInt32 height = rect.GetHeight();

	bool multiline = false;

	// If the text extent width is greater than the allowed width percentage,
	// we need to scale down the font.
	if( (rect.GetWidth() * fTextMaxWidth) < w)
	{
		// First, see if there are any spaces in the label text.  If there are,
		// we can try to multiline the text.
		wxInt32 spaces = mLabel.Freq(' ');
		if(0 < spaces)
		{
			// Determine what point size font will allow multiline for this
			// button.
			wxInt32 pointSize = mFont.GetPointSize();

			// Only try one size lower.  The point of multiline is to make it
			// as close to th normal font as possible
			wxInt32 minSize = pointSize - 1;

			bool success = false;

			while(minSize <= pointSize)
			{
				mDrawFont.SetPointSize(pointSize);
				dc.SetFont(mDrawFont);

				dc.GetTextExtent(mLabel, &w, &h);

				// See if it fits.
				if( (height * fTextMaxHeight) >= 
					(h * 2) + sMultiLineBuffer)
				{
					success = true;
					break;
				}
				
				--pointSize;
			}

			// If it is possible to get multiline text onto this button, see
			// if we can actually do it with the spaces we have.
			if(true == success)
			{
				wxString line1, line2;
				wxInt32 w1 = 0, w2 = 0, h1 = 0, h2 = 0;
				bool fits = false;

				wxInt32 space = mLabel.Find(' ');
				wxASSERT(-1 != space);

				// Keep trying every space until there are none left.
				while(-1 != space)
				{
					line1 = mLabel.Left(space);
					line2 = mLabel.Right(mLabel.Length() - space - 1);

					// If both of these lines fit at the given font, we have
					// a winner.
					dc.GetTextExtent(line1, &w1, &h1);
					if((width * fTextMaxWidth) >= w1)
					{
						dc.GetTextExtent(line2, &w2, &h2);
						if((width * fTextMaxWidth) >= w2)
						{
							fits = true;
							break;
						}
					}

					space = line2.Find(' ');

					if(-1 != space)
					{
						// Since we are splitting off the original label, we
						// have to add in the amount that line2 was split from.
						space += line1.Len() + 1;
					}
				}

				// Were we successful?
				if(true == fits)
				{
					// Set the lines.
					wxPoint pos;
					pos.x = ((width - w1) >> 1) + 1;
					pos.y = (height - ((h1 * 2) + sMultiLineBuffer)) >> 1;
					mLabelArray.push_back(std::make_pair(line1, pos));

					pos.x = ((width - w2) >> 1) + 1;
					pos.y = pos.y + h2 + sMultiLineBuffer;
					mLabelArray.push_back(std::make_pair(line2, pos));

					multiline = true;
				}
			}
		}

		// If there are no spaces, or multilining fails, just start dropping
		// the font size until it fits.
		if(false == multiline)
		{
			wxInt32 pointSize = mFont.GetPointSize() - 1;

			while(2 <= pointSize)
			{
				mDrawFont.SetPointSize(pointSize);
				dc.SetFont(mDrawFont);

				// See if it now fits.
				dc.GetTextExtent(mLabel, &w, &h);

				if((width * fTextMaxWidth) >= w)
				{
					break;
				}

				--pointSize;
			}
		}
	}

	// Set final position.
	if(false == multiline)
	{
		wxPoint pos = calculateTextPosition(w, h, rect);
		mLabelArray.push_back(std::make_pair(mLabel, pos));
	}

	Refresh();
}

void wxBaseImageButton::SetBitmaps(const HashString &enabled, 
	const HashString &disabled)
{
	mEnabled = enabled;
	mDisabled = disabled;

	OnSkin();
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void wxBaseImageButton::GetBitmaps()
{
	wxSize size = GetSize() - wxSize(1, 1);
	wxImage image = IMAGE.GetImage(mKey, mEnabled, size);

	wxBitmap enabled = wxBitmap(image);

	// Cheap, because of reference counting.
	wxBitmap disabled = enabled;

	if(false == mDisabled.empty())
	{
		image = IMAGE.GetImage(mKey, mDisabled, size);
		disabled = wxBitmap(image);
	}

	wxBaseBitmapButton::SetBitmaps(enabled, disabled);
}

void wxBaseImageButton::Paint(wxMemoryDC &dc)
{
	// Draw original button.
	wxBaseBitmapButton::Paint(dc);

	wxInt32 origin = 0;
	if(ModeDown == GetMode())
	{
		origin = 1;
	}

	//see if we even need to draw text
	if(false == mLabelArray.empty())
	{
		//draw text
		dc.SetFont(mDrawFont);
		dc.SetBackgroundMode(wxTRANSPARENT);

		// Draw each piece of the label
		for(size_t i = 0; i < mLabelArray.size(); ++i)
		{
			wxInt32 w = mLabelArray[i].second.x;
			wxInt32 h = mLabelArray[i].second.y;

			if(sTransparent != sButtonTextShadow)
			{
				dc.SetTextForeground(sButtonTextShadow);
				dc.DrawText(mLabelArray[i].first, w + origin, h + origin);
			}
			dc.SetTextForeground(sButtonText);
			dc.DrawText(mLabelArray[i].first, --w + origin, --h + origin);
		}

		dc.SetBackgroundMode(wxSOLID);
	}
}

void wxBaseImageButton::OnSkin(wxInt32)
{
	// Text
	sButtonText = SKIN.Element(shButtonText);
	sButtonTextShadow = SKIN.Element(shButtonTextShadow);
	sTransparent = SKIN.Element(shTransparent);
	mFont = SKIN.Font(shButtonFont);
	mDrawFont = SKIN.Font(shButtonFont);

	// Bitmaps
	GetBitmaps();

	wxSizeEvent size(GetSize());
	Size(size);

	SetLabel(mLabel);
}

void wxBaseImageButton::Size(wxSizeEvent &event)
{
	GetBitmaps();

	//reset the font to the proper size
	wxClientDC dc(this);
	Utility::calculateFontSize(dc, mFont, static_cast<wxInt32>(
		(static_cast<float>(event.GetSize().GetHeight()) * fTextPercent)));

	// Update the label text.
	SetLabel(mLabel);
}
