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
#include "HTML.h"
#include "ColorInfoData.h"
#include "SkinManager.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
wxString HTML::ConvertColorToHex(const wxColour color)
{
	// Convert the color to an HTML representation.
	wxUint8 r = color.Red();
	wxUint8 g = color.Green();
	wxUint8 b = color.Blue();

	return wxString::Format(swHTMLColorFormat.c_str(), r, g, b);
}

wxString &HTML::Font(wxString &text, const HashString &size, 
					 const HashString &color, const HashString &font)
{
	const wxFont &thisFont = SKIN.Font(font);

	wxString str = swHTMLFontFace;
	str += thisFont.GetFaceName();
	str += swHTMLSize;
	str += wxString::Format(swIntFormat.c_str(), SKIN.Size(size));
	str += wxString::Format(swHTMLColor.c_str(), 
		ConvertColorToHex(SKIN.Element(color)).c_str());
	str += swHTMLClose;
	str += text;
	str += swHTMLFontClose;

	text = str;

	return text;
}

wxString &HTML::Color(wxString &text, const ColorType type)
{
	if(CR_SIZE != type)
	{
		wxColour color = ColorInfoData::UIColor(type);

		wxString str = swHTMLFontColor;
		str += ConvertColorToHex(color);
		str += swHTMLClose;
		str += text;
		str += swHTMLFontClose;

		text = str;
	}

	return text;
}

wxString &HTML::Bold(wxString &text)
{
	wxString str = swHTMLBold;
	str += text;
	str += swHTMLBoldClose;

	text = str;

	return text;
}

wxString &HTML::Italics(wxString &text)
{
	wxString str = swHTMLItalic;
	str += text;
	str += swHTMLItalicClose;

	text = str;

	return text;
}

wxString &HTML::Underline(wxString &text)
{
	wxString str = swHTMLUnderline;
	str += text;
	str += swHTMLUnderlineClose;

	text = str;

	return text;
}

wxString &HTML::Break(wxString &text)
{
	text += swHTMLBreak;
	return text;
}

wxString &HTML::Row(wxString &text)
{
	wxString str = swHTMLRow;
	str += text;
	str += swHTMLRowClose;

	text = str;

	return text;
}

wxString &HTML::Col(wxString &text)
{
	wxString str = swHTMLCol;
	str += text;
	str += swHTMLColClose;

	text = str;

	return text;
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
