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
#include <modes.h>
#include <blowfish.h>
#include <filters.h>
#include <sha.h>

// Yep, MD5 is weak.
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <md5.h>

//---------------------------- USER INCLUDES    -----------------------------//
#include "Encryption.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ENCRYPTION Blowfish

//---------------------------- STATICS          -----------------------------//
namespace
{
	byte sIV[CryptoPP::ENCRYPTION::BLOCKSIZE] = 
		{23, 57, 249, 149, 143, 37, 103, 211};
}

//---------------------------- PUBLIC           -----------------------------//
void Encryption::Encrypt(const std::string& input, const byte* key, 
	size_t keyLength, std::string& output)
{
	// Pad with spaces until we hit the block size.
	std::string plainText = input;
	size_t padding = plainText.length() % CryptoPP::ENCRYPTION::BLOCKSIZE;

    if(0 < padding) {
        for(size_t i = padding; i < CryptoPP::ENCRYPTION::BLOCKSIZE; ++i)
        {
	        plainText += " ";
        }
    }

	size_t length = plainText.length();

	// Encrypt.
	CryptoPP::ENCRYPTION::Encryption encryption(key, keyLength);
	CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(
		encryption, sIV);

	boost::scoped_array<byte> bytes(new byte[length]);

	cbcEncryption.ProcessData(bytes.get(), 
		reinterpret_cast<const byte*>(plainText.c_str()), 
		length);

	// Now convert to outgoing.
	output = std::string(reinterpret_cast<const char*>(bytes.get()), length);
}

void Encryption::Encrypt(const wxString& input, const byte* key, 
	size_t keyLength, wxString& output)
{
	// Convert to char*.
	std::string in = (char*)input.char_str();
	std::string out;

	Encrypt(in, key, keyLength, out);

	// Now convert to outgoing.
	output = wxString(out.c_str(), wxConvLibc, out.length());
}

void Encryption::Decrypt(const std::string& input, const byte* key, 
	size_t keyLength, std::string& output)
{
	// Convert to char*.
	std::string decryptedText;

	//Decrypt.
	CryptoPP::ENCRYPTION::Decryption decryption(key, keyLength);
	CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(
		decryption, sIV);

	size_t length = input.length();

	boost::scoped_array<byte> bytes(new byte[length]);

	cbcDecryption.ProcessData(bytes.get(), 
		reinterpret_cast<const byte*>(input.c_str()), 
		input.length());

	output = std::string(reinterpret_cast<const char*>(bytes.get()), length);
}

void Encryption::Decrypt(const wxString& input, const byte* key, 
	size_t keyLength, wxString& output)
{
	std::string in = (char*)input.char_str();
	std::string out;

	Decrypt(in, key, keyLength, out);

	// Now convert to outgoing.
	output = wxString(out.c_str(), wxConvLibc, out.length());

	// Trim any padded spaces we added at the end.
	output.Trim();
}

void Encryption::HashMd5(const std::string& i_key, byte* o_hash)
{
	CryptoPP::Weak::MD5().CalculateDigest(o_hash,
		reinterpret_cast<const byte*>(i_key.c_str()),
		i_key.length());
}

void Encryption::HashSha1(const std::string& i_key, byte* o_hash)
{
	// Hash the password.
	CryptoPP::SHA().CalculateDigest(o_hash, 
		reinterpret_cast<const byte*>(i_key.c_str()), 
		i_key.length());
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
