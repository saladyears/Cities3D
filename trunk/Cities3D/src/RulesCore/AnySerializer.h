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
#include <wx/datstrm.h>

//---------------------------- USER INCLUDES    -----------------------------//
#include "IAnySerializer.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: AnySerializer
// 
// Needs documentation.
//
// Derived From:
//     <IAnySerializer>
//
// Project:
//     <RulesCore>
//
// Include:
//     IAnySerializer.h
//
template <typename T>
class AnySerializer : public IAnySerializer
{
public:
	AnySerializer(const HashString& type)
		: mType(type)
		, mTypeID(typeid(T).name())
	{
	}

	virtual const HashString& Type() const
	{
		return mType;
	}

	virtual const HashString& TypeID() const
	{
		return mTypeID;
	}

	virtual void Serialize(const boost::any &data, wxDataOutputStream &output)
	{
		const T *value = boost::any_cast<T>(&data);

		if (NULL == value)
		{
			boost::throw_exception(boost::bad_any_cast());
		}

		PrivateSerialize(*value, output);
	}

	virtual void Unserialize(boost::any &data, wxDataInputStream &input)
	{
		PrivateUnserialize(data, input);
	}

private:
	AnySerializer();

	//functions for types that can be written/read with << and >>
	void EasySerialize(const T &value, wxDataOutputStream &output)
	{
		output << value;
	}

	void EasyUnserialize(boost::any &data, wxDataInputStream &input)
	{
		T value;
		input >> value;
		data = boost::any(value);
	}

	//default functions which will compile, but produce errors if actually used
	void PrivateSerialize(const T &, wxDataOutputStream &)
	{
		wxLogError(wxT("Programmer Error: Attempting to serialize a %s"), 
			Type().cwx_str());
	}

	void PrivateUnserialize(boost::any &, wxDataInputStream &)
	{
		wxLogError(wxT("Programmer Error: Attempting to unserialize a %s"), 
			Type().cwx_str());
	}

	HashString mType;
	HashString mTypeID;
};

//---------------------------- PROTOTYPES       -----------------------------//

//specializations for supported types

//ints
template <>
inline void AnySerializer<wxInt32>::PrivateSerialize(const wxInt32 &value, wxDataOutputStream &output)
{
	EasySerialize(value, output);
}

template <>
inline void AnySerializer<wxInt32>::PrivateUnserialize(boost::any &data, wxDataInputStream &input)
{
	EasyUnserialize(data, input);
}

//strings
template <>
inline void AnySerializer<HashString>::PrivateSerialize(const HashString &value, 
	wxDataOutputStream &output)
{
	EasySerialize(value, output);
}

template <>
inline void AnySerializer<HashString>::PrivateUnserialize(boost::any &data, 
	wxDataInputStream &input)
{
	EasyUnserialize(data, input);
}

template <>
inline void AnySerializer<wxString>::PrivateSerialize(const wxString &value, 
	wxDataOutputStream &output)
{
	EasySerialize(value, output);
}

template <>
inline void AnySerializer<wxString>::PrivateUnserialize(boost::any &data, 
	wxDataInputStream &input)
{
	EasyUnserialize(data, input);
}

//bools
template <>
inline void AnySerializer<bool>::PrivateSerialize(const bool &value, wxDataOutputStream &output)
{
	output << (value ? TRUE : FALSE);
}

template <>
inline void AnySerializer<bool>::PrivateUnserialize(boost::any &data, wxDataInputStream &input)
{
	wxBOOL value;
	input >> value;
	data = boost::any(value || false);
}
