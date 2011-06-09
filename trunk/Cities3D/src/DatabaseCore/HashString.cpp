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
#include "HashString.h"
#include "fnv.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace 
{
    wxUint32 CalculateHash(const char* str, size_t length)
    {
        return fnv_32_buf(str, length, 0);
    }
}

wxDataOutputStream& operator<<(wxDataOutputStream& out, const HashString& str)
{
	// Write the number of bytes.
	wxUint32 size = str.size();
	const char* chars = str.c_str();

	out << size;
	for(wxUint32 i = 0; i < size; ++i)
	{
		char c = chars[i];
		out.Write8(c);
	}
	
	return out;
}

wxDataInputStream& operator>>(wxDataInputStream& in, HashString& str)
{
	wxUint32 size;
	in >> size;

	char c;

	str.resize(size);
	for(wxUint32 i = 0; i < size; ++i)
	{
		c = static_cast<char>(in.Read8());
		str[i] = c;
	}

    str.mHash = CalculateHash(str.c_str(), str.length());

	return in;
}

HashString HashString::Format(const char* format, ...)
{
	va_list argptr;
	va_start(argptr, format);

	HashString s;
	wxInt32 size = 1024;

	for ( ;; )
	{
		wxCharBuffer buffer(size + 1);
		char *buf = buffer.data();

		// Check for out of memory.
		if(!buf)
		{
			break;
		}

		// wxVsnprintf() may modify the original arg pointer, so pass it only a copy
		va_list argptrcopy;
		wxVaCopy(argptrcopy, argptr);
		int len = _vsprintf_p(buf, size, format, argptrcopy);
		va_end(argptrcopy);

		// some implementations of vsnprintf() don't NUL terminate
		// the string if there is not enough space for it so
		// always do it manually
		buf[size] = 0;

		// vsnprintf() may return either -1 (traditional Unix behaviour) or the
		// total number of characters which would have been written if the
		// buffer were large enough (newer standards such as Unix98)
		if ( len < 0 )
		{
			// still not enough, as we don't know how much we need, double the
			// current size of the buffer
			size *= 2;
		}
		else if ( len >= size )
		{
			// some vsnprintf() implementations NUL-terminate the buffer and
			// some don't in len == size case, to be safe always add 1
			size = len + 1;
		}
		else // ok, there was enough space
		{
            s = HashString(buf, strlen(buf));
			break;
		}
	}

	return s;
}

//---------------------------- PUBLIC           -----------------------------//
HashString::HashString()
: std::string()
{
    mHash = CalculateHash(c_str(), length());
}

HashString::HashString(const char* str)
: std::string(str)
{
    mHash = CalculateHash(c_str(), length());
}

HashString::HashString(const char* str, size_t count)
: std::string(str, count)
{
    mHash = CalculateHash(c_str(), length());
}
	
HashString::HashString(const wxString& str)
: std::string(str.mb_str(), str.Length())
{
    mHash = CalculateHash(c_str(), length());
}

wxString HashString::wx_str() const
{
	return wxString(c_str(), wxConvUTF8);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
