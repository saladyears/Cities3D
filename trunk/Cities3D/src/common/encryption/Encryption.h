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
#include "cryptlib.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//
namespace Encryption
{
	void Encrypt(const std::string& input, const byte* key, size_t keyLength, 
		std::string& output);
	void Encrypt(const wxString& input, const byte* key, size_t keyLength, 
		wxString& output);
	
	void Decrypt(const std::string& input, const byte* key, size_t keyLength,
		std::string& output);
	void Decrypt(const wxString& input, const byte* key, size_t keyLength,
		wxString& output);
	
	void HashMd5(const std::string& i_key, byte* o_hash);
	void HashSha1(const std::string& i_key, byte* o_hash);
}

//---------------------------- PROTOTYPES       -----------------------------//

