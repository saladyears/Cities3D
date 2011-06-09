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

//---------------------------- USER INCLUDES    -----------------------------//
#include "DatabaseExport.h"

//---------------------------- DEFINES          -----------------------------//
class HashString;
#define WX_DECLARE_HASH_STRING_HASH_MAP(Value, Name) \
    WX_DECLARE_HASH_MAP(HashString, Value, HashString::Hash, HashString::Equality, Name)

#define WX_DECLARE_HASH_STRING_HASH_SET(Name) \
    WX_DECLARE_HASH_SET(HashString, HashString::Hash, HashString::Equality, Name)

//---------------------------- TYPEDEFS         -----------------------------//
class wxDataInputStream;
class wxDataOutputStream;

typedef std::vector<HashString> HashStringArray;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: HashString
// 
// A non-Unicode string for use in hashes.
//
// Derived From:
//     std::string
//
// Project:
//     <DatabaseCore>
//
// Include:
//     HashString.h
//
class DATABASE_EXPORT_SPEC HashString : public std::string
{
public:
    // Constructors.
	HashString();
	HashString(const char* str);
	explicit HashString(const char* str, size_t count);
	explicit HashString(const wxString& str);

    // Public methods.
    inline wxUint32 hash() const 
    {
        return mHash;
    }

    wxString wx_str() const;
    
	static HashString Format(const char* format, ...);

    // Operators.
    inline bool operator<(const HashString& rhs) const
	{
		int compare = strncmp(c_str(), rhs.c_str(), length());
		return (0 > compare);
	}

    inline bool operator==(const HashString& rhs) const
    {
        // First compare hashes.  If they differ, it's a quick out.
        bool ret = true;

        if(mHash != rhs.mHash)
        {
            ret = false;
        }
        else
        {
            ret = (0 == strncmp(c_str(), rhs.c_str(), length()));
        }

        return ret;
    }

    inline void operator +=(const HashString& rhs)
    {
        std::string temp = *this + rhs;
        
        // Force an update of the hash.
        *this = HashString(temp.c_str());
    }

    // Nested Types.
    struct Hash
    {
        inline unsigned long operator()(const HashString& str) const
        {
            return str.hash();
        }
    };

    struct Equality
    {
        inline bool operator()(const HashString& lhs, const HashString& rhs) const
        {
            return lhs == rhs;
        }
    };

private:
    wxUint32 mHash;

    friend DATABASE_EXPORT_SPEC wxDataInputStream& operator>>(wxDataInputStream& in, HashString& str);
};

// For use in wxLog functions.
#define cwx_str() wx_str().c_str()
	
//---------------------------- PROTOTYPES       -----------------------------//

DATABASE_EXPORT_SPEC wxDataOutputStream& operator<<(wxDataOutputStream& out, 
	const HashString& str);

DATABASE_EXPORT_SPEC wxDataInputStream& operator>>(wxDataInputStream& in, 
	HashString& str);
