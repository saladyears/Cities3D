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
#include <wx/hashmap.h>
#include <wx/config.h>

//---------------------------- USER INCLUDES    -----------------------------//
#include "Singleton.h"
#include "DatabaseExport.h"

//---------------------------- DEFINES          -----------------------------//
#define CONFIG ConfigDatabase::get()

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: ConfigDatabase
//
// A Singleton class that handles configuration options for the
// application.  All configuration options are stored in an OS-specific
// format, handled transparently by wxWidgets.  ConfigDatabase is a layer
// on top of wxWidgets that stores retrieved options in a local
// cache so that the OS is only pinged for previously unqueried
// values or when a configuration option changes.
//
// Derived From:
//     <Singleton>
//
// Project:
//     <DatabaseCore>
//
// Include:
//     ConfigDatabase.h
//
class DATABASE_EXPORT_SPEC ConfigDatabase : public Singleton<ConfigDatabase>
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: read
	//
	// Reads a configuration option from the cache or OS.  The cache is
	// first checked for the option.  If it does not exist, the OS is checked
	// for the option.  If it does not exist there, the default value is
	// returned. Only integer, float, and wxString values can be returned as
	// configuration options.
	//
	// Parameters:
	//     section - The configuration section to search.
	//     option - The configuration option to look for.
	//     defaultValue - A default value for the option if it does not exist
	//                    in the OS repository.
	//
	// Returns:
	//     The value of the configuration option.
	//
	template <typename T> T read(const wxString &section,
		const wxString &option, T defaultValue)
	{
		//use the same trick as in Data.h; if they try and use any version of this
		//function that's not explicitly specialized, they get a compile error
		return;
	}

	//-----------------------------------------------------------------------//
	// Function: write
	//
	// Writes a configuration option to the cache and OS.  The write happens
	// immediately. Only integer, float, and wxString values can be written as
	// configuration options.
	//
	// Parameters:
	//     section - The configuration section to write to.
	//     option - The configuration option to write.
	//     value - The value of the configuration option.
	//
	template <typename T> void write(const wxString &section,
		const wxString &option, T value)
	{
		//see above
		wxString YouAreWritingAnUnspecializedType;
		wxCOMPILE_TIME_ASSERT(false, YouAreWritingAnUnspecializedType);
	}

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: ConfigDatabase
	//
	// The ConfigDatabase constructor. Even though this constructor does
	// nothing, it still needs to exist and be instantiated in
	// ConfigDatabase.cpp or else there is nothing to link to in the .DLL for
	// the ConfigDatabase class and external link errors will result in other
	// modules.
	//
	ConfigDatabase();

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: readPrivate
	//
	// Reads a configuration key from the OS.
	//
	// Parameters:
	//     key - The key path to search in the OS repository.
	//     defaultValue - The default value if the key does not exist.
	// Returns:
	//     The value of the the configuration option.
	//
	template <typename T> bool readPrivate(const wxString &key, T &val,
		T defaultValue = T()) const
	{
		return wxConfigBase::Get()->Read(key, &val, defaultValue);
	}

	//-----------------------------------------------------------------------//
	// Function: writePrivate
	//
	// Writes a configuration key to the OS.
	//
	// Parameters:
	//     key - The key path to write to in the OS repository.
	//     value - The value of the configuration option.
	//
	template <typename T> bool writePrivate(const wxString &key, T value)
	{
		return wxConfigBase::Get()->Write(key, value);
	}

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	wxConfig *config;

	WX_DECLARE_STRING_HASH_MAP(wxString, StringHash);

	//-----------------------------------------------------------------------//
	// Hash: mCache
	//
	// The hash of configuration options.
	//
	StringHash mCache;

	friend class Singleton<ConfigDatabase>;
};

//---------------------------- PROTOTYPES       -----------------------------//

//explicit specializations to return specific data types

template <> inline wxInt32 ConfigDatabase::read(const wxString &section,
						const wxString &option,
						wxInt32 defaultValue)
{
	//immediate lookup in the cache to see if we've already got it
	wxString key = wxString::Format(swSlashStringSlashString.c_str(), 
        section.c_str(), option.c_str());

	//if it's not there, return the actual config lookup
	StringHash::iterator it(mCache.find(key));
	if(mCache.end() == it)
	{
		wxInt32 value;
		readPrivate(key, value, defaultValue);
		mCache[key] = wxString::Format(swIntFormat.c_str(), value);
		return value;
	}

	return atoi(it->second.mb_str());
}

template <> inline float ConfigDatabase::read(const wxString &section,
						const wxString &option,
						float defaultValue)
{
	//wxConfigBase likes doubles, not floats
	double value;

	//immediate lookup in the cache to see if we've already got it
	wxString key = wxString::Format(swSlashStringSlashString.c_str(), 
        section.c_str(), option.c_str());

	//if it's not there, return the actual config lookup
	StringHash::iterator it(mCache.find(key));
	if(mCache.end() == it)
	{
		readPrivate(key, value, static_cast<double>(defaultValue));
		mCache[key] = wxString::Format(swFloat6.c_str(), value);
		return (float) value;
	}

	((it->second).ToDouble(&value));
	return value;
}

template <> inline wxString ConfigDatabase::read(const wxString &section,
						 const wxString &option,
						 wxString defaultValue)
{
	//immediate lookup in the cache to see if we've already got it
	wxString key = wxString::Format(swSlashStringSlashString.c_str(), 
        section.c_str(), option.c_str());

	//if it's not there, return the actual config lookup
	StringHash::iterator it(mCache.find(key));
	if(mCache.end() == it)
	{
		wxString value;
		readPrivate(key, value, defaultValue);
		mCache[key] = value;
		return value;
	}

	return it->second;
}

//explicit specializations to write specific data types

template <> inline void ConfigDatabase::write(const wxString &section,
						const wxString &option,
						wxInt32 value)
{
	wxString key = wxString::Format(swSlashStringSlashString.c_str(), 
        section.c_str(), option.c_str());

	//write to the config
	writePrivate(key, value);

	//write to the cache
	mCache[key] = wxString::Format(swIntFormat.c_str(), value);
}

template <> inline void ConfigDatabase::write(const wxString &section,
						const wxString &option,
						float value)
{
	wxString key = wxString::Format(swSlashStringSlashString.c_str(), 
        section.c_str(), option.c_str());

	//write to the config (wxBaseConfig doesn't write floats, only doubles)
	writePrivate(key, static_cast<double>(value));

	//write to the cache
	mCache[key] = wxString::Format(swFloat6.c_str(), value);
}

template <> inline void ConfigDatabase::write(const wxString &section,
						const wxString &option,
						wxString value)
{
	wxString key = wxString::Format(swSlashStringSlashString.c_str(), 
        section.c_str(), option.c_str());

	//write to the config
	writePrivate(key, value);

	//write to the cache
	mCache[key] = value;
}
