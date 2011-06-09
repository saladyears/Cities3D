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
#include "OptionAdvanced.h"
#include "Controller.h"
#include "ConfigDatabase.h"
#include "SkinManager.h"
#include "BaseStaticBox.h"
#include "BaseCheckBox.h"
#include "BaseRadioButton.h"
#include "FittedStaticText.h"
#include "DataObject.h"
#include "GLExport.h"
#include "Controller.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	enum
	{
		ID_RENDER_CHECK			 = 0,
		ID_FRAMEBUFFER_CHECK	,
		ID_PIXELBUFFER_CHECK	,
		ID_16_RADIO				,
		ID_24_RADIO				,
		ID_32_RADIO				,
		ID_DEBUG_CHECK			,
	};

	wxBaseCheckBox *spRenderCheck;
	wxBaseCheckBox *spFramebufferCheck;
	wxBaseCheckBox *spPixelbufferCheck;
	wxBaseRadioButton *sp16Radio;
	wxBaseRadioButton *sp24Radio;
	wxBaseRadioButton *sp32Radio;
	wxBaseCheckBox *spDebugCheck;
}

//---------------------------- PUBLIC           -----------------------------//
wxOptionAdvanced::wxOptionAdvanced(wxWindow* parent)
: wxOptionWindow(parent, swAdvanced)
{
	wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);

	wxBaseStaticBox *box = new wxBaseStaticBox(this, -1, stBitmaps, 
		wxDefaultPosition, wxSize(200, 25));
	wxStaticBoxSizer *boxSizer = new wxStaticBoxSizer(box, wxHORIZONTAL);

	bool render = CONFIG.read<wxInt32>(swConfigSectionOpenGL, 
		swRenderBitmaps, TRUE) || false;
	bool framebuffer = CONFIG.read<wxInt32>(swConfigSectionOpenGL, swFrameBuffer, 
		FALSE) || false;
	bool pixelbuffer = CONFIG.read<wxInt32>(swConfigSectionOpenGL, swPixelBuffer, 
		TRUE) || false;
	wxBoxSizer *innerVert2 = new wxBoxSizer(wxVERTICAL);
	spRenderCheck = new wxBaseCheckBox(this, ID_RENDER_CHECK, 
		stRenderBitmapsWithOpenGL);
	spRenderCheck->SetValue(render);
	spFramebufferCheck = new wxBaseCheckBox(this, ID_FRAMEBUFFER_CHECK,
		stUseFrameBuffers);
	spFramebufferCheck->SetValue(framebuffer);
	spFramebufferCheck->Enable(render);
	spPixelbufferCheck = new wxBaseCheckBox(this, ID_PIXELBUFFER_CHECK,
		stUsePixelBuffers);
	spPixelbufferCheck->SetValue(pixelbuffer);
	spPixelbufferCheck->Enable(render);
	innerVert2->Add(spRenderCheck, 1, wxEXPAND);
	innerVert2->AddSpacer(6);
	innerVert2->Add(spFramebufferCheck, 1, wxEXPAND | wxLEFT, 16);
	innerVert2->AddSpacer(6);
	innerVert2->Add(spPixelbufferCheck, 1, wxEXPAND | wxLEFT, 16);
	boxSizer->Add(innerVert2, 1, wxTOP | wxBOTTOM | wxLEFT | wxRIGHT, 7);

	wxBaseStaticBox *box2 = new wxBaseStaticBox(this, -1, 
		stDepthBufferSize, wxDefaultPosition, wxSize(200, 25));
	wxStaticBoxSizer *boxSizer2 = new wxStaticBoxSizer(box2, wxHORIZONTAL);
	sp16Radio = new wxBaseRadioButton(this, ID_16_RADIO, sw16, 
		shDefaultText, shDefaultFont, wxDefaultPosition, wxDefaultSize, 
		wxRB_GROUP);
	sp24Radio = new wxBaseRadioButton(this, ID_24_RADIO, sw24);
	sp32Radio = new wxBaseRadioButton(this, ID_32_RADIO, sw32);
	boxSizer2->Add(sp16Radio, 1, wxEXPAND);
	boxSizer2->Add(sp24Radio, 1, wxEXPAND);
	boxSizer2->Add(sp32Radio, 1, wxEXPAND);

	// Get the default window color depth and use that as the default.
	wxInt32 defaultSize = CONFIG.read<wxInt32>(swConfigSectionOpenGL, swDepthBuffer,
		::wxDisplayDepth());
	switch(defaultSize)
	{
	case 32:
		sp32Radio->SetValue(true);
		break;
	case 24:
		sp24Radio->SetValue(true);
		break;
	case 16:
	default:
		sp16Radio->SetValue(true);
		break;
	}

	spDebugCheck = new wxBaseCheckBox(this, ID_DEBUG_CHECK, 
		stDisplayOpenGLRender);
	bool debug = CONFIG.read<wxInt32>(swConfigSectionOpenGL, swDebugInfo, 
		TRUE) || false;
	spDebugCheck->SetValue(debug);
	
	pSizer->Add(boxSizer, 0, wxEXPAND);
	pSizer->Add(boxSizer2, 0, wxEXPAND | wxTOP, 7);
	pSizer->Add(spDebugCheck, 0, wxEXPAND | wxTOP, 14);
	
	SetSizer(pSizer);
	pSizer->SetSizeHints(this);
	pSizer->Layout();
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxOptionAdvanced, wxOptionWindow)
	EVT_CHECKBOX(ID_RENDER_CHECK, wxOptionAdvanced::OnRender)
	EVT_CHECKBOX(ID_FRAMEBUFFER_CHECK, wxOptionAdvanced::OnFramebuffer)
	EVT_CHECKBOX(ID_PIXELBUFFER_CHECK, wxOptionAdvanced::OnPixelbuffer)
	EVT_RADIOBUTTON(ID_16_RADIO, wxOptionAdvanced::On16Radio)
	EVT_RADIOBUTTON(ID_24_RADIO, wxOptionAdvanced::On24Radio)
	EVT_RADIOBUTTON(ID_32_RADIO, wxOptionAdvanced::On32Radio)
	EVT_CHECKBOX(ID_DEBUG_CHECK, wxOptionAdvanced::OnDebug)
END_EVENT_TABLE()

void wxOptionAdvanced::OnRender(wxCommandEvent &)
{
	bool check = spRenderCheck->GetValue();

	CONFIG.write(swConfigSectionOpenGL, swRenderBitmaps, check ? TRUE : FALSE);

	spFramebufferCheck->Enable(check);
	spPixelbufferCheck->Enable(check);

	Controller::get().Transmit(shEventRenderBitmaps, 0);
}

void wxOptionAdvanced::OnFramebuffer(wxCommandEvent &)
{
	bool check = spFramebufferCheck->GetValue();

	CONFIG.write(swConfigSectionOpenGL, swFrameBuffer, check ? TRUE : FALSE);

	Controller::get().Transmit(shEventRenderBitmaps, 0);
}

void wxOptionAdvanced::OnPixelbuffer(wxCommandEvent &)
{
	bool check = spPixelbufferCheck->GetValue();

	CONFIG.write(swConfigSectionOpenGL, swPixelBuffer, check ? TRUE : FALSE);

	Controller::get().Transmit(shEventRenderBitmaps, 0);
}

void wxOptionAdvanced::On16Radio(wxCommandEvent &)
{
	CONFIG.write(swConfigSectionOpenGL, swDepthBuffer, 16);
}

void wxOptionAdvanced::On24Radio(wxCommandEvent &)
{
	CONFIG.write(swConfigSectionOpenGL, swDepthBuffer, 24);
}

void wxOptionAdvanced::On32Radio(wxCommandEvent &)
{
	CONFIG.write(swConfigSectionOpenGL, swDepthBuffer, 32);
}

void wxOptionAdvanced::OnDebug(wxCommandEvent &)
{
	bool check = spDebugCheck->GetValue();

	CONFIG.write(swConfigSectionOpenGL, swDebugInfo, check ? TRUE : FALSE);

	Controller::get().Transmit(shEventDirty, 0);
}
