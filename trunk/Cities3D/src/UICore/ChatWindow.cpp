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
#include "ChatWindow.h"
#include "ChatInputTextCtrl.h"
#include "Controller.h"
#include "HtmlWindowEx.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
wxChatWindow::wxChatWindow(wxWindow* parent, wxWindowID id, const bool entry, 
						   const wxPoint& pos, const wxSize &size)
: wxWindow(parent, id, pos, size)
, mpHtmlWindow(NULL)
, mpInputText(NULL)
{
	wxASSERT(NULL != parent);

	SetBackgroundColour(parent->GetBackgroundColour());

	mpHtmlWindow = new wxHtmlWindowEx(this, -1);
	mpHtmlWindow->SetBorders(1);
	if(true == entry)
	{
		mpInputText = new wxChatInputTextCtrl(this, 0);
	}

	// Get things going in the HTML window.
	mpHtmlWindow->AppendToPage(swHTMLBody);

	Controller::get().AddReceiver(shEventChatMessage, 
		&wxChatWindow::OnChatMessage, this);
}

wxChatWindow::~wxChatWindow()
{
	Controller::get().RemoveReceiver(shEventChatMessage, 
		&wxChatWindow::OnChatMessage, this);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxChatWindow, wxWindow)
    EVT_SIZE(wxChatWindow::OnSize)
END_EVENT_TABLE()

void wxChatWindow::OnChatMessage(const DataObject &object)
{
	const wxString &str = object.read<wxString>();

	mpHtmlWindow->AddText(str);	
}

void wxChatWindow::OnSize(wxSizeEvent& event)
{
	event.Skip();

	wxSize size = event.GetSize();

	wxInt32 w = size.GetWidth();
	wxInt32 h = size.GetHeight();
	wxInt32 th = 0;
	
	if(NULL != mpInputText)
	{
		th = mpInputText->GetSize().GetHeight();
		mpInputText->SetSize(0, h - th, w, th);
	}

	mpHtmlWindow->SetSize(0, 0, w, h - th - 1);
}
