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
#include "TexDropListCtrl.h"
#include "TexEditorPane.h"
#include "Controller.h"

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- STATICS          -----------------------------//
namespace
{
	class wxTexDropTarget : public wxFileDropTarget
	{
	public:
		wxTexDropTarget(wxTexDropListCtrl* list);

		//overrides
		virtual wxDragResult OnDragOver(wxCoord x, wxCoord y, 
			wxDragResult def);
		virtual bool OnDrop(long x, long y, const void *data, size_t size);
		virtual bool OnDropFiles(wxCoord x, wxCoord y, 
			const wxArrayString& filenames);

	private:
		wxTexDropListCtrl* mpTexDropListCtrl;
	};

	wxTexDropTarget::wxTexDropTarget(wxTexDropListCtrl* list)
		: mpTexDropListCtrl(list)
	{
	}
	
	wxDragResult wxTexDropTarget::OnDragOver(wxCoord x, wxCoord y, 
		wxDragResult def)
	{
		wxInt32 index = mpTexDropListCtrl->HitTest(wxPoint(x, y));
		
		mpTexDropListCtrl->SetSelectedItem(index);

		return def;
	}

	bool wxTexDropTarget::OnDrop(long x, long y, const void *, size_t)
	{
		wxInt32 index = mpTexDropListCtrl->HitTest(wxPoint(x, y));

		return (-1 != index);
	}

	bool wxTexDropTarget::OnDropFiles(wxCoord x, wxCoord y, 
		const wxArrayString& filenames)
	{
		wxInt32 index = mpTexDropListCtrl->HitTest(wxPoint(x, y));

		//sanity
		if( (-1 == index) || (0 == filenames.Count()))
		{
			return false;
		}

		//set set the first one
		wxTexEditorPane *frame = dynamic_cast<wxTexEditorPane *>(
			mpTexDropListCtrl->GetParent());
		wxASSERT(NULL != frame);
		frame->SetPath(index, filenames[0]);

		return true;
	}

	wxString sFields[] = {swFile};
}

//---------------------------- PUBLIC           -----------------------------//
wxTexDropListCtrl::wxTexDropListCtrl(wxWindow* parent, wxWindowID id, 
									 const wxPoint& pos, const wxSize& size)
: wxBaseListCtrl(parent, id, sFields, _countof(sFields), shTexListMainFont, 
				 shTexListSubFont, pos, size)
{
	SetDropTarget(new wxTexDropTarget(this));

	Controller::get().AddReceiver(shEventKeyDown, 
		&wxTexDropListCtrl::OnKeyDown, this);
}

wxTexDropListCtrl::~wxTexDropListCtrl()
{
	Controller::get().RemoveReceiver(shEventKeyDown, 
		&wxTexDropListCtrl::OnKeyDown, this);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void wxTexDropListCtrl::OnKeyDown(const wxKeyEvent &event)
{
	//check for a deletion
	if(WXK_DELETE == event.GetKeyCode())
	{
		wxTexEditorPane *frame = static_cast<wxTexEditorPane *>(GetParent());
		wxASSERT(NULL != frame);

		wxInt32 index = GetSelectedItem();

		if(-1 != index)
		{
			frame->DeleteItem(index);

			// We need to stop promulgating this event becase we just deleted
			// the window it came from.
			const_cast<wxKeyEvent&>(event).Skip(false);
		}
	}
}


