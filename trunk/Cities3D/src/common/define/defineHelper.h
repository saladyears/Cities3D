/*
 *  Cities3D - Copyright (C) 2001-2004 Jason Fugate (saladyears@gmail.com)
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
#include <wx/datstrm.h>

//---------------------------- USER INCLUDES    -----------------------------//

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
namespace
{
	template <typename T>
	void saveArray(wxDataOutputStream &stream, const std::vector<T> *pArray)
	{
		stream << (wxUint32) pArray->size();

		for(wxUint32 i = 0; i < pArray->size(); i++)
		{
			stream << (*pArray)[i];
		}
	}

	template <typename T>
	void saveArrays(wxDataOutputStream &stream, 
		const std::vector<std::vector<T> > *pArray)
	{
		wxUint32 i, size;
		size = pArray->size();

		stream << size;

		for(i = 0; i < size; i++)
		{
			saveArray(stream, &((*pArray)[i]));
		}
	}

	template <typename T>
	void save3Arrays(wxDataOutputStream &stream, 
		const std::vector<std::vector<std::vector<T> > > *pArray)
	{
		wxUint32 i, size;
		size = pArray->size();

		stream << size;

		for(i = 0; i < size; i++)
		{
			saveArrays(stream, &((*pArray)[i]));
		}
	}

	template <typename T>
	void loadArray(wxDataInputStream &stream, std::vector<T> *pArray)
	{
		wxUint32 i, size;

		stream >> size;
		pArray->resize(size);

		for(i = 0; i < size; i++)
		{
			stream >> (*pArray)[i];
		}
	}

	template <typename T>
	void loadArrays(wxDataInputStream &stream, 
		std::vector<std::vector<T> > *pArray)
	{
		wxUint32 i, size;

		stream >> size;
		pArray->resize(size);

		for(i = 0; i < size; i++)
		{
			loadArray(stream, &((*pArray)[i]));
		}
	}

	template <typename T>
	void load3Arrays(wxDataInputStream &stream, 
		std::vector<std::vector<std::vector<T> > > *pArray)
	{
		wxUint32 i, size;

		stream >> size;
		pArray->resize(size);

		for(i = 0; i < size; i++)
		{
			loadArrays(stream, &((*pArray)[i]));
		}
	}

	void saveDateTime(wxDataOutputStream &stream, const wxDateTime &time)
	{
		wxLongLong val = time.GetValue();
		stream << (wxUint32) val.GetHi();
		stream << (wxUint32) val.GetLo();
	}

	void loadDateTime(wxDataInputStream &stream, wxDateTime &time)
	{
		wxUint32 lo, hi;
		stream >> hi;
		stream >> lo;
		time = wxLongLong(hi, lo);
	}

	void saveTimeSpan(wxDataOutputStream &stream, const wxTimeSpan &time)
	{
		wxLongLong val = time.GetValue();
		stream << (wxUint32) val.GetHi();
		stream << (wxUint32) val.GetLo();
	}

	void loadTimeSpan(wxDataInputStream &stream, wxTimeSpan &time)
	{
		wxUint32 lo, hi;
		stream >> hi;
		stream >> lo;
		time = wxLongLong(hi, lo);
	}

	//date time specialization
	template <>
	void saveArray(wxDataOutputStream &stream, 
		const std::vector<wxDateTime> *pArray)
	{
		stream << (wxUint32) pArray->size();

		for(wxUint32 i = 0; i < pArray->size(); i++)
		{
			saveDateTime(stream, (*pArray)[i]);
		}
	}

	template <>
	void loadArray(wxDataInputStream &stream, std::vector<wxDateTime> *pArray)
	{
		wxUint32 i, size;

		stream >> size;
		pArray->resize(size);

		for(i = 0; i < size; i++)
		{
			loadDateTime(stream, (*pArray)[i]);
		}
	}
}

//---------------------------- CLASSES          -----------------------------//

//---------------------------- PROTOTYPES       -----------------------------//

