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
#include "Singleton.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
typedef std::vector<std::string> StdStringArray;

class wxCommandThread;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: Launcher
//
// Handles command-lines and launches Cities3D or a host check thread as 
// required.
// 
// Derived From:
//     <Singleton>
//
// Project:
//     <CitiesOnline>
//
// Include:
//     Launcher.h
//
class Launcher : public Singleton<Launcher>
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	enum DecryptionSuccess
	{
		Success				= 0,
		InvalidPassword		,
		InvalidVersion		,
		Failure				,
	};

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: Launch
	//
	// Handles the password validation and application launching.
	//
	// Parameters:
	//     url - The url to parse.
	//
	bool Launch(const wxString& url);

	//-----------------------------------------------------------------------//
	// Function: Command
	//
	// Encrypts the given strings with the current user's password and sends 
	// them as commands to the server.
	//
	// Parameters:
	//     i_str - The plaintext strings.
	//
	void Command(const StdStringArray& i_str);

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: Launcher
	//
	// The default Launcher constructor.
	//
	Launcher();

	//-----------------------------------------------------------------------//
	// Destructor: ~Launcher
	//
	// The Launcher destructor.
	//
	~Launcher();

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: DecryptUrl
	//
	// Decrypts the given Url for further validation.
	//
	// Parameters:
	//     url - The url to decrypt.
	//     plainText - The unencrypted plaintext.
	//
	// Returns:
	//     A bool that is true if the decryption was successful and false if
	//     not.
	//
	DecryptionSuccess DecryptUrl(const wxString& url, wxString& plainText);

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Pointer: mpCommandThread
	//
	// The command thread that sends commands to the site.
	//
	wxCommandThread* mpCommandThread;

	friend class Singleton<Launcher>;
};

//---------------------------- PROTOTYPES       -----------------------------//

