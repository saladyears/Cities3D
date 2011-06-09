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
#include "BaseListCtrl.h"
#include "ListSelection.h"
#include "Controller.h"
#include "SkinManager.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	class Sorter
	{
	public:
		Sorter(const wxString &field) : mField(field) {}

		typedef std::pair<wxInt32, wxListSelection *> SelectionPair;

		bool operator()(const SelectionPair &sel1, const SelectionPair &sel2)
		{
			return (sel1.second->GetItemText(mField) < 
				sel2.second->GetItemText(mField));
		}

	private:
		const wxString &mField;
	};
}

//---------------------------- PUBLIC           -----------------------------//
wxBaseListCtrl::wxBaseListCtrl(wxWindow* parent, wxWindowID id, 
	const wxString array[], const size_t num_items, const HashString &mainFont,
	const HashString &subFont, const wxPoint& pos, const wxSize& size, 
	const wxInt32 subItemsPerRow, const wxInt32 style)
: wxScrolledWindow(parent, id, pos, size, style)
, mSelectedItem(-1)
, mLocked(false)
, mSubItemsPerRow(subItemsPerRow)
, mMainFont(mainFont)
, mSubFont(subFont)
{
	wxASSERT(0 < num_items);

	// Set the display order properly.
	for(size_t i = 0; i < num_items; ++i)
	{
		mDisplayOrder.push_back(array[i]);
	}

	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	sizer->SetSizeHints(this);
	SetSizer(sizer);

	SetMinSize(size);
	SetBackgroundColour(SKIN.Element(shBackground));

	Controller::get().AddReceiver(shEventSkin, &wxBaseListCtrl::OnSkin, this);
}

wxBaseListCtrl::~wxBaseListCtrl()
{
	Controller::get().RemoveReceiver(shEventSkin, &wxBaseListCtrl::OnSkin, 
		this);
}

void wxBaseListCtrl::Lock()
{
	Freeze();

	mLocked = true;
}

void wxBaseListCtrl::Unlock()
{
	mLocked = false;
	CalculateVirtualSize();
	Thaw();
}

const size_t wxBaseListCtrl::AppendItem(const wxString &str, 
										const HashString &icon)
{
	size_t index = mSelections.size();
	wxListSelection *selection = new wxListSelection(this, index, 
		mDisplayOrder, str, mSubItemsPerRow, mMainFont, mSubFont, icon);
	
	AppendItem(index, selection);

	return index;
}

const size_t wxBaseListCtrl::AppendItem(const wxString &str, 
										const wxImage &image)
{
	size_t index = mSelections.size();
	wxListSelection *selection = new wxListSelection(this, index, 
		mDisplayOrder, str, mSubItemsPerRow, mMainFont, mSubFont, image);
	
	AppendItem(index, selection);

	return index;
}

void wxBaseListCtrl::DeleteItem(const size_t index)
{
	size_t size = mSelections.size();

	if(index < size)
	{
		mSelections[index].second->Destroy();
		mSelections.erase(mSelections.begin() + index);

		// Clear the selection if necessary.
		if(static_cast<wxInt32>(index) == mSelectedItem)
		{
			SetSelectedItem(-1);
		}

		ReorderSelections();
		CalculateVirtualSize();

		wxSizer *sizer = GetSizer();
		wxASSERT(NULL != sizer);
		sizer->Layout();

		Refresh();
	}
}

void wxBaseListCtrl::DeleteAllItems()
{
	// Destroy all children and clear the array.
	DestroyChildren();
	mSelections.clear();
	mSelectedItem = -1;
}

void wxBaseListCtrl::SetItemText(const size_t index, const wxString &field, 
	const wxString &text)
{
	size_t size = mSelections.size();

	if(index < size)
	{
		mSelections[index].second->SetItemText(field, text);
	}
}

const wxString wxBaseListCtrl::GetItemText(const size_t index, 
	 const wxString &field) const
{
	wxString str;
	size_t size = mSelections.size();

	if(index < size)
	{
		str = mSelections[index].second->GetItemText(field);
	}

	return str;
}

const wxInt32 wxBaseListCtrl::FindItem(const wxString &field, 
	const wxString &text) const
{
	wxInt32 index = -1;

	for(size_t i = 0; i < mSelections.size(); ++i)
	{
		if(text == GetItemText(i, field))
		{
			index = i;
			break;
		}
	}

	return index;
}

const wxInt32 wxBaseListCtrl::HitTest(const wxPoint &point) const
{
	wxInt32 index = -1;
	wxInt32 y = 0;

	wxPoint thisPoint = point;
	
	// Adjust for virtualness.
	wxInt32 xOff, yOff, xUnit, yUnit;
	GetViewStart(&xOff, &yOff);
	GetScrollPixelsPerUnit(&xUnit, &yUnit);
	thisPoint.x += (xOff * xUnit);
	thisPoint.y += (yOff * yUnit);

	for(size_t i = 0; i < mSelections.size(); ++i)
	{
		const wxSize &size = mSelections[i].second->GetSize();

		// See if we got it.
		if( (0 <= thisPoint.x) && (size.x >= thisPoint.x) &&
			(y <= thisPoint.y) && (y + size.y >= thisPoint.y))
		{
			index = i;
			break;
		}

		y += size.y;
	}

	return index;
}

void wxBaseListCtrl::Sort(const wxString &field)
{
	// First, detach everything from the sizer.
	wxSizer *sizer = GetSizer();
	wxASSERT(NULL != sizer);

	SelectionArray::iterator it, itEnd = mSelections.end();
	for(it = mSelections.begin(); it != itEnd; ++it)
	{
		sizer->Detach(it->second);
	}

	// Now sort.
	std::sort(mSelections.begin(), mSelections.end(), Sorter(field));

	// Finally, reattach everything to the sizer.
	for(it = mSelections.begin(); it != itEnd; ++it)
	{
		sizer->Add(it->second, 0, wxEXPAND);
	}

	ReorderSelections();

	sizer->Layout();

	Refresh();
}

void wxBaseListCtrl::SetSelectedItem(const wxInt32 index)
{
	wxInt32 size = mSelections.size();

	wxInt32 oldSelection = mSelectedItem;
	mSelectedItem = index;

	// First, clear any current selection.
	if( (-1 != oldSelection) &&
		(size > oldSelection))
	{
		mSelections[oldSelection].second->SetSelected(false);

		// Fire the selected event.
		wxListEvent event(wxEVT_COMMAND_LIST_ITEM_DESELECTED, GetId());
		ProcessEvent(event);
	}

	if( (-1 != mSelectedItem) &&
		(size > mSelectedItem))
	{
		mSelections[mSelectedItem].second->SetSelected(true);

		// Fire the selected event.
		wxListEvent event(wxEVT_COMMAND_LIST_ITEM_SELECTED, GetId());
		ProcessEvent(event);
	}
}

void wxBaseListCtrl::SetItemData(const size_t index, const wxInt32 data)
{
	size_t size = mSelections.size();

	if(index < size)
	{
		mSelections[index].first = data;
	}
}

const wxInt32 wxBaseListCtrl::GetItemData(const size_t index) const
{
	size_t size = mSelections.size();
	wxInt32 data = -1;

	if(index < size)
	{
		data = mSelections[index].first;
	}

	return data;
}

void wxBaseListCtrl::SetItemColor(const size_t index, const wxColour color)
{
	size_t size = mSelections.size();

	if(index < size)
	{
		mSelections[index].second->SetItemColor(color);
	}
}

wxColour wxBaseListCtrl::GetItemColor(const size_t index) const
{
	wxColour color = wxColour(0, 0, 0);

	size_t size = mSelections.size();

	if(index < size)
	{
		color = mSelections[index].second->GetItemColor();
	}

	return color;
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxBaseListCtrl, wxScrolledWindow)
	EVT_MOTION(wxBaseListCtrl::OnMouseMove)
	EVT_LEFT_DOWN(wxBaseListCtrl::OnLeftDown)
	EVT_LEFT_UP(wxBaseListCtrl::OnLeftUp)
	EVT_LEFT_DCLICK(wxBaseListCtrl::OnLeftDoubleClick)
	EVT_MOUSEWHEEL(wxBaseListCtrl::OnMouseWheel)
	EVT_SIZE(wxBaseListCtrl::OnSize)
END_EVENT_TABLE()

void wxBaseListCtrl::AppendItem(const size_t index, 
								wxListSelection * const selection)
{
	mSelections.push_back(std::make_pair(index, selection));

	wxSizer *sizer = GetSizer();
	wxASSERT(NULL != sizer);

	sizer->Add(selection, 0, wxEXPAND);
	sizer->Layout();

	CalculateVirtualSize();
}

void wxBaseListCtrl::CalculateVirtualSize()
{
	if(false == mLocked)
	{
		const wxSize clientSize = GetClientSize();

		// Go through all of the children, seeing if our client size can handle
		// all of the children.
		wxInt32 width = 0, height = 0;

		SelectionArray::iterator it, itEnd = mSelections.end();
		for(it = mSelections.begin(); it != itEnd; ++it)
		{
			const wxSize size = it->second->GetMinSize();
			height += size.y;
			width = std::max(width, size.x);
		}

		// If the size of the children is bigger than our current client size, 
		// we need to set a virtual size so the scroll bars will kick in.
		if( (GetWindowStyle() & wxSTATIC_BORDER) &&
			((clientSize.x < width) ||
			(clientSize.y < height)))
		{
			wxSize virtualSize = wxSize(std::max(width, clientSize.x), 
				std::max(height, clientSize.y));
			SetVirtualSize(virtualSize);

			SetScrollRate(10, height / mSelections.size());
		}
		else
		{
			SetVirtualSize(clientSize);
			SetScrollRate(0, 0);
		}

		mBestSize = wxSize(width, height) + wxSize(2, 2);
	}
}

void wxBaseListCtrl::ReorderSelections()
{
	size_t size = mSelections.size();

	for(size_t i = 0; i < size; ++i)
	{
		mSelections[i].second->SetIndex(i);
	}
}

void wxBaseListCtrl::OnSkin(const wxInt32)
{
	SetBackgroundColour(SKIN.Element(shBackground));

	// Detach all of the children, reskin them, then reattach them.
	wxSizer *sizer = GetSizer();
	wxASSERT(NULL != sizer);

	SelectionArray::iterator it, itEnd = mSelections.end();
	for(it = mSelections.begin(); it != itEnd; ++it)
	{
		sizer->Detach(it->second);
		it->second->OnSkin();
		sizer->Add(it->second, 0, wxEXPAND);
	}
	
	wxSizer *parentSizer = GetParent()->GetSizer();
	wxASSERT(NULL != parentSizer);
	
	parentSizer->Layout();
}

void wxBaseListCtrl::OnSize(wxSizeEvent &event)
{
	CalculateVirtualSize();

	event.Skip();
}

void wxBaseListCtrl::OnMouseMove(wxMouseEvent &event)
{
	MouseMove(event);
}

void wxBaseListCtrl::OnLeftDown(wxMouseEvent &event)
{
	wxInt32 index = HitTest(event.GetPosition());

	if(mSelectedItem != index)
	{
		SetSelectedItem(index);
	}

	// Call other implemenations.
	LeftDown(event);
}

void wxBaseListCtrl::OnLeftUp(wxMouseEvent &event)
{
	LeftUp(event);
}

void wxBaseListCtrl::OnLeftDoubleClick(wxMouseEvent &event)
{
	wxInt32 index = HitTest(event.GetPosition());

	if(mSelectedItem != index)
	{
		SetSelectedItem(index);
	}

	// Call other implementations.
	LeftDoubleClick(event);

	wxListEvent listEvent(wxEVT_COMMAND_LIST_ITEM_ACTIVATED, GetId());
	ProcessEvent(listEvent);
}

void wxBaseListCtrl::OnMouseWheel(wxMouseEvent &event)
{
	wxInt32 range = GetScrollRange(wxVERTICAL);
	wxInt32 pos = GetScrollPos(wxVERTICAL);

	if(0 != range)
	{
		wxInt32 vX, vY;
		GetViewStart(&vX, &vY);

		if( (0 > event.m_wheelRotation) &&
			(range - 1 > pos))
		{
			Scroll(vX, ++vY);
		}
		else if( (0 < event.m_wheelRotation) &&
				 (0 < pos))
		{
			Scroll(vX, --vY);
		}
	}
}
