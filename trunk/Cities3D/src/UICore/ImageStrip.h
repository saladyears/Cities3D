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
#pragma once

#include "style.h"	//READ THIS BEFORE MAKING ANY CHANGES TO THIS FILE!!!

//---------------------------- SYSTEM INCLUDES  -----------------------------//
#include <map>
#include <boost/function.hpp>

//---------------------------- USER INCLUDES    -----------------------------//
#include "UIExport.h"
#include "OwnerDrawWindow.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxImageStrip
//
// Displays a list of images in a horizontal strip, with counter text.
//
// Derived From:
//     <wxOwnerDrawWindow>
//
// Project:
//     <Cities3D>
//
// Include:
//     ImageStrip.h
//
class UI_EXPORT_SPEC wxImageStrip : public wxOwnerDrawWindow
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	typedef boost::function1<wxColour, const HashString&> CounterColorFunc;
	typedef std::map<HashString, wxInt32> ResourceMap;

	static const HashString EmptyResource;

	enum Flags
	{
		FlagAlignCenter			= 1,
		FlagReverseOrder		= 2,
		FlagOneOutline			= 4,
		FlagDisabled			= 8,
		FlagHideText			= 16,
		FlagHideZeroes			= 32,
		FlagForwardMouseEvents	= 64,
		FlagAlternateDraw		= 128,
	};

	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxImageStrip
	//
	// The wxImageStrip constructor.
	//
	// Parameters:
	//     parent - The parent window (must not be NULL).
	//     id - The id for message handling.
	//     maxImages - The maximum number of images in the image strip.
	//     minTextHeight - The minimum text height.
	//     key - The key in the <ImageEngine> where the images are.
	//     sizeImage - An image that gives the default size for images in the
	//                 strip.
	//     ratio - The ratio of the default size to use.
	//
	wxImageStrip(wxWindow* parent, wxWindowID id, const size_t maxImages,
		const size_t minTextHeight, const HashString& key = shResources, 
		const HashString& sizeImage = shTimber, float ratio = 0.5f);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxImageStrip
	//
	// The wxImageStrip destructor.
	//
	virtual ~wxImageStrip();

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Clear
	//
	// Clears the internal image and count array.
	//
	void Clear();

	//-----------------------------------------------------------------------//
	// Function: Add
	//
	// Adds a resource image to the array with the given count.
	//
	// Parameters:
	//     resource - The resource to add.
	//     count - The count for the image.
	//
	void Add(const HashString &resource, const wxInt32 count);

	//-----------------------------------------------------------------------//
	// Function: Remove
	//
	// Removes a resource image from the array.
	//
	// Parameters:
	//     resource - The resource to remove.
	//     adjustMax - A flag that is true if the max images count should be
	//                 decreased and false if not.
	//
	void Remove(const HashString &resource, bool adjustMax = false);

	//-----------------------------------------------------------------------//
	// Function: SetResourceCount
	//
	// Sets the count for the given resource image.
	//
	// Parameters:
	//     resource - The resource to set the count for.
	//     count - The count to set.
	//
	void SetResourceCount(const HashString& resource, const wxInt32 count);

	//-----------------------------------------------------------------------//
	// Function: GetResourceCount
	//
	// Gets the count for the given resource image.
	//
	// Parameters:
	//     resource - The resource to get the count of.
	//
	// Returns:
	//     The count of the given resource.
	//
	wxInt32 GetResourceCount(const HashString &resource) const;

	//-----------------------------------------------------------------------//
	// Function: GetResourceCount
	//
	// Returns the sum of all resource counts in the strip.
	//
	// Returns:
	//     The sum of all resource counts in the strip.
	//
	wxInt32 GetResourceCount() const;

	//-----------------------------------------------------------------------//
	// Function: SetKey
	//
	// Sets the image lookup key for the strip.
	//
	// Parameters:
	//     key - The key.
	//
	void SetKey(const HashString& key, bool clear = true)
	{
		mKey = key;

		if(true == clear)
		{
			Clear();
		}
	}

	//-----------------------------------------------------------------------//
	// Function: SetMaxImages
	//
	// Sets the maximum number of images in the images trip to the given
	// amount.
	//
	// Parameters:
	//     size - The maximum number of images in this image strip.
	//
	void SetMaxImages(size_t size)
	{
		mMaxImages = size;
		Clear();
	}

	//-----------------------------------------------------------------------//
	// Function: GetMaxImages
	//
	// Returns the maximum number of images the strip can hold.
	//
	// Returns:
	//     The maximum number of images the strip can hold.
	//
	size_t GetMaxImages() const
	{
		return mMaxImages;
	}

	//-----------------------------------------------------------------------//
	// Function: GetImage
	//
	// Returns the image at the given point in the image strip, if any exists
	// there.
	//
	// Parameters:
	//     pos - The position to test.
	//
	// Returns:
	//     The name of the resource at the point, if it exists, or an empty
	//     string if none exists.
	//
	HashString GetImage(const wxPoint& pos) const;

	//-----------------------------------------------------------------------//
	// Function: SetCounterColorFunc
	//
	// Sets an optional callback function that determines text color for each
	// count in the list.
	//
	// Parameters:
	//     func - The callback function.
	//
	void SetCounterColorFunc(CounterColorFunc func)
	{
		mCounterColorFunc = func;
		Refresh();
	}

	//-----------------------------------------------------------------------//
	// Function: IsFull
	//
	// Returns whether the image strip is full of images or not.
	//
	// Returns:
	//     A bool that is true if no more images can be added to the strip
	//     and false if not.
	//
	bool IsFull() const
	{
		return mCurImages == mMaxImages;
	}

	//-----------------------------------------------------------------------//
	// Function: FillResourceMap
	//
	// Fills the given resource map with what's in the strip.
	//
	// Parameters:
	//     map - The map to fill.
	//
	void FillResourceMap(ResourceMap& map) const;

	//-----------------------------------------------------------------------//
	// Function: SetResourceMap
	//
	// Sets the strip to the given map.
	//
	// Parameters:
	//     map - The map to set.
	//
	void SetResourceMap(const ResourceMap& map);

	//-----------------------------------------------------------------------//
	// Function: SetFlags
	//
	// Sets the flags for the strip.
	//
	// Parameters:
	//     flags - The flags.
	//
	void SetFlags(size_t flags)
	{
		mFlags |= flags;
		Refresh();
	}

	//-----------------------------------------------------------------------//
	// Function: GetFlags
	//
	// Returns the strip flags.
	//
	// Returns:
	//    The strip flags bitfield.
	//
	size_t GetFlags() const
	{
		return mFlags;
	}

	//-----------------------------------------------------------------------//
	// Function: SetPixelSpacing
	//
	// Sets the number of pixels between each image.
	//
	// Parameters:
	//     pixels - The number of pixels between each image.
	//
	void SetPixelSpacing(size_t pixels)
	{
		mPixelSpacing = pixels;
	}

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Size
	//
	// Handles window resizes.
	//
	// Parameters:
	//     event - The resize event.
	//
	virtual void Size(wxSizeEvent &event);

	//-----------------------------------------------------------------------//
	// Function: CalcluateMinSize
	//
	// Calculates the minimum size required by the image strip, and allows
	// derived classes to tack on their own calculations.
	//
	// Parameters:
	//     size - [out] The min size being calculated.
	//
	virtual void CalcluateMinSize(wxSize &size);

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: GetSizeRatio
	//
	// Returns the current size ratio.
	//
	// Returns:
	//     The current size ratio.
	//
	const float GetSizeRatio() const {return mSizeRatio;}

	//-----------------------------------------------------------------------//
	// Function: DrawImages
	//
	// Draws the images to the given dc at the given coordinates.
	//
	// Parameters:
	//     dc - The device context to draw to.
	//     x - The starting x coordinate.
	//     y - The starting y coordinate.
	//
	void DrawImages(wxDC &dc, size_t x, size_t y) const;

	//-----------------------------------------------------------------------//
	// Group: Event Message Handling
	//

	//-----------------------------------------------------------------------//
	// Function: OnMouseMove
	//
	// Forwards mouse move events to the parent.
	//
	// Parameters:
	//     event - The mouse event.
	//
	virtual void OnMouseMove(wxMouseEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnLeftDown
	//
	// Handles the left mouse down event.
	// 
	// Parameters:
	//     event - The mouse event.
	//
	virtual void OnLeftDown(wxMouseEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnLeftUp
	//
	// Handles the left mouse up event.
	// 
	// Parameters:
	//     event - The mouse event.
	//
	virtual void OnLeftUp(wxMouseEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnRightUp
	//
	// Handles the right mouse up event.
	// 
	// Parameters:
	//     event - The mouse event.
	//
	virtual void OnRightUp(wxMouseEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnLeftDoubleClick
	//
	// Handles the left mouse double-click event.
	// 
	// Parameters:
	//     event - The mouse event.
	//
	virtual void OnLeftDoubleClick(wxMouseEvent &event);

	//-----------------------------------------------------------------------//
	// Function: OnRightDoubleClick
	//
	// Handles the right mouse double-click event.
	// 
	// Parameters:
	//     event - The mouse event.
	//
	virtual void OnRightDoubleClick(wxMouseEvent &event);

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Macro: DECLARE_EVENT_TABLE
	//
	// The wxWidgets event table declaration.
	//
	DECLARE_EVENT_TABLE()

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Paint
	//
	// Paints the window.
	//
	// Parameters:
	//     dc - The device context.
	//
	virtual void Paint(wxMemoryDC &dc);

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: ResizeBitmaps
	//
	// Resizes all of the bitmaps in the display.
	//
	void ResizeBitmaps();

	//-----------------------------------------------------------------------//
	// Group: Game Event Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnImageUpdate
	//
	// Handles a change in the game images used.
	//
	void OnImageUpdate(bool);

	struct ImageEntry
	{
		ImageEntry() : resource(EmptyResource), count(-1) {}

		HashString resource;
		wxBitmap bitmap;
		wxInt32 count;
	};

	typedef std::vector<ImageEntry> ImageEntryArray;
	
	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Object: mKey
	//
	// The key in the <ImageEngine> where to look for the images.
	//
	HashString mKey;

	//-----------------------------------------------------------------------//
	// Object: mImageEntryArray
	//
	// The array of images to be displayed, along with their name and counts.
	//
	ImageEntryArray mImageEntryArray;

	//-----------------------------------------------------------------------//
	// Object: mTextFont
	//
	// The text display font.
	//
	wxFont mTextFont;

	//-----------------------------------------------------------------------//
	// Int: mCurImages
	//
	// The current number of images in the image strip.
	//
	size_t mCurImages;

	//-----------------------------------------------------------------------//
	// Int: mMaxImages
	//
	// The maximum number of images the image strip will contain (and 
	// therefore, the minimum size it must be.)
	//
	size_t mMaxImages;

	//-----------------------------------------------------------------------//
	// Int: mPixelSpacing
	//
	// The number of pixels between each image.
	//
	size_t mPixelSpacing;

	//-----------------------------------------------------------------------//
	// Object: mMinImageSize
	//
	// The minimum size the bitmaps can be.
	//
	wxSize mMinImageSize;

	//-----------------------------------------------------------------------//
	// Object: mMinImageSize
	//
	// The current size of the bitmaps.
	//
	wxSize mCurrentImageSize;

	//-----------------------------------------------------------------------//
	// Int: mMinTextHeight
	//
	// The minimum height the text can be.
	//
	size_t mMinTextHeight;

	//-----------------------------------------------------------------------//
	// Float: mSizeRatio
	//
	// The current size ratio over the min size.
	//
	float mSizeRatio;

	//-----------------------------------------------------------------------//
	// Object: mCounterColorFunc
	//
	// Callback function to set counter color.
	//
	CounterColorFunc mCounterColorFunc;

	//-----------------------------------------------------------------------//
	// Int: mFlags
	//
	// The strip flags.
	//
	size_t mFlags;
};

//---------------------------- PROTOTYPES       -----------------------------//

