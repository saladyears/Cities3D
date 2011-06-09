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
#include <sha.h>
#include <wx/uri.h>

// Yep, MD5 is weak.
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <md5.h>

//---------------------------- USER INCLUDES    -----------------------------//
#include "Launcher.h"
#include "encryption/Encryption.h"
#include "LoginDialog.h"
#include "CitiesOnlineApp.h"
#include "CommandThread.h"
#include "network/Network.h"
#include "BaseMessageBox.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	// The Cities Online Version.
	const wxInt32 CON_VERSION_MAJOR		= 0;
	const wxInt32 CON_VERSION_MINOR		= 0;
	const wxInt32 CON_VERSION_PATCH		= 5;

	const wxChar sSeparator = wxChar(':');

	const size_t sMinimumNameLength = 3;
	const size_t sMaximumNameLength = 16;

	wxString sUserName;
	byte sSHA[CryptoPP::SHA::DIGESTSIZE];

	typedef std::map<char, wxInt32> CharMap;
	CharMap sCharMap;

	typedef std::map<wxInt32, char> ReverseCharMap;
	ReverseCharMap sReverseCharMap;

	const char* sItoa64 = "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	
	bool GetSalt(const wxString& url, const wxString& userName, wxString& salt)
	{
		bool ret = false;

		wxInt32 index = url.Find(userName.c_str());

		// The salt is 12 characters long.
		if(wxNOT_FOUND != index)
		{
			salt = url.Mid(index + userName.Length() + 1, 12);
			
			ret = true;
		}

		return ret;
	}

	bool GetUserNameAndSalt(const wxString& url, wxString& userName, wxString& salt)
	{
		bool ret = false;

		wxInt32 index = url.Lower().Find(swProtocol.c_str());

		if(0 == index)
		{
			// Find the first : after the index.
			wxString right = url.Right(url.Length() - swProtocol.Length());

			index = right.Find(sSeparator);

			// Has to be within our name length parameters.
			if( (sMinimumNameLength <= index) &&
				(sMaximumNameLength >= index))
			{
				// We've got the name.
				userName = right.Left(index);
				
				// Get the salt as well.
				ret = GetSalt(url, userName, salt);
			}
		}

		return ret;
	}

	bool GetEncodedText(const wxString& url, const wxString& salt,
		wxString& encodedText)
	{
		bool ret = false;

		wxInt32 index = url.Find(salt.c_str());

		// This should never happen, but what the hell.
		if(wxNOT_FOUND != index)
		{
			encodedText = url.Right(url.Length() - salt.Length() - 
				index);

			ret = true;
		}

		return ret;
	}

	bool GetEncryptedText(const wxString& url, const wxString& salt,
		std::string& encryptedText)
	{
		bool ret = false;
		wxString encodedText;

		if(true == GetEncodedText(url, salt, encodedText))
		{
			encryptedText.clear();

			// The encrypted text must be a multiple of two as every entry
			// must be xx.
			size_t length = encodedText.Length();
			if(0 == (length % 2))
			{
				encryptedText.reserve(length >> 1);

				ret = true;

				// Convert to char* for processing.
				wxWritableCharBuffer buffer = encodedText.char_str();
				const char* str = static_cast<const char*>(buffer);
				char c = 0;
				
				for(size_t i = 0; i < length; i += 2)
				{
					const char* p = &str[i];
				
					// Check each sub-chunk for validity.
					if(	(sCharMap.end() == sCharMap.find(p[0])) ||
						(sCharMap.end() == sCharMap.find(p[1])))
					{
						ret = false;
						break;
					}

					// Convert.
					c = sCharMap[p[0]] << 4 | sCharMap[p[1]];

					encryptedText += c;
				}
			}
		}

		return ret;
	}

	bool GetPassword(wxString& password)
	{
		bool ret = false;

		// Replace any %20s in their name with spaces, for display.
		wxString userName;
		Network::FixNetworkString(sUserName, userName);
		
		// Get their password.
		wxLoginDialog dlg(userName);
		if(wxID_OK == dlg.ShowModal())
		{
			password = dlg.GetUserEntry();

			ret = true;
		}

		return ret;
	}

	wxString GetGameVersion()
	{
		return wxString::Format(swVersion.c_str(), CON_VERSION_MAJOR, 
			CON_VERSION_MINOR, CON_VERSION_PATCH);
	}

	Launcher::DecryptionSuccess ValidatePlaintext(wxString& plainText)
	{
		// The plaintext format is <version>:<command line> where the version
		// is exactly five characters with x.x.x format.  Therefore, we can
		// check to see if the player's password was correct first, and then
		// if the version was valid second.
		Launcher::DecryptionSuccess ret = Launcher::InvalidPassword;

		// Check for valid version number.
		wxString version = plainText.Left(6);

		wxWritableCharBuffer buffer = version.char_str();
		const char* ver = static_cast<const char*>(buffer);

		if( (sCharMap.end() != sCharMap.find(ver[0])) &&
			(sCharMap.end() != sCharMap.find(ver[2])) &&
			(sCharMap.end() != sCharMap.find(ver[4])) &&
			('.' == ver[1]) &&
			('.' == ver[3]) &&
			(':' == ver[5]))
		{
			// Now we compare version numbers.
			ret = Launcher::InvalidVersion;

			// If they are identical, we're good to go.
			wxString gameVersion = GetGameVersion();

			if(version.Left(5) == gameVersion)
			{
				ret = Launcher::Success;

				// Extract the command line portion of it.
				plainText = plainText.Right(plainText.Length() - 6);
			}
		}
		// If their password failed, clear the user name so they have to 
		// reenter.
		else
		{
			sUserName.Clear();
		}

		return ret;
	}

	void Encode64(byte* input, size_t count, std::string& out)
	{
		out.clear();

		size_t i = 0;
		do {
			size_t c = input[i++];
			out += sItoa64[c & 0x3f];
			if (i < count)
				c |= input[i] << 8;
			out += sItoa64[(c >> 6) & 0x3f];
			if (i++ >= count)
				break;
			if (i < count)
				c |= input[i] << 16;
			out += sItoa64[(c >> 12) & 0x3f];
			if (i++ >= count)
				break;
			out += sItoa64[(c >> 18) & 0x3f];
		} while (i < count);
	}
}

//---------------------------- PUBLIC           -----------------------------//
bool Launcher::Launch(const wxString& url)
{
	bool ret = true;

	// Ignore all commands if they are already in a game.
	if(true == wxTheCitiesOnlineApp->InGame())
	{
        static const wxString stWhileYouAreInAGame = _("While you are in a game, Cities Online ignores all commands from the site.");
		wxBaseMessageBox dialog(NULL, swCitiesOnline, stWhileYouAreInAGame);
		dialog.ShowModal();
	}
	else
	{
		wxString plainText;
		DecryptionSuccess success = InvalidPassword;

		while(InvalidPassword == success)
		{
			success = DecryptUrl(url, plainText);
		}

		// If we're good, launch!
		if(Success == success)
		{
			wxTheCitiesOnlineApp->ProcessString(plainText);
		}
		else if(InvalidVersion == success)
		{
            static const wxString stYourVersionIsOutOfDate = _("Your version of Cities Online is out-of-date.");
			wxBaseMessageBox dialog(NULL, swCitiesOnline, 
                stYourVersionIsOutOfDate);
			dialog.ShowModal();

			// If we fail the version check, we need to exit the program.
			ret = false;
		}
	}

	return ret;
}

void Launcher::Command(const StdStringArray& i_str)
{
	// Get the command URL from the application.
	wxString commandUrl = wxTheCitiesOnlineApp->CommandUrl();

	StringArray commands;

	StdStringArray::const_iterator it, itEnd = i_str.end();
	for(it = i_str.begin(); it != itEnd; ++it)
	{
		std::string output;

		try
		{
			Encryption::Encrypt(*it, sSHA, 
				CryptoPP::SHA::DIGESTSIZE, output);
		}
		catch(const CryptoPP::Exception& e)
		{
			wxMessageBox(wxString(e.GetWhat().c_str(), wxConvLibc));
		}

		// Now we need to hexadecimalize it.
        size_t size = output.length();
		std::string hex(size << 1, 0);
		for(size_t i = 0; i < size; ++i)
		{
			size_t index = i << 1;
			unsigned char c = output[i];

			hex[index] = toupper(sReverseCharMap[(c >> 4)]);
			hex[index + 1] = toupper(sReverseCharMap[(c & 0x0F)]);
		}

		// Now set the final command and add it to the queue.
		wxString command = commandUrl + wxString(hex.c_str(), wxConvLibc);
		commands.push_back(command);
	}
	
	mpCommandThread->SendCommands(commands);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
Launcher::Launcher()
{
	// Create lookup tables.
	sCharMap['0'] = 0;
	sCharMap['1'] = 1;
	sCharMap['2'] = 2;
	sCharMap['3'] = 3;
	sCharMap['4'] = 4;
	sCharMap['5'] = 5;
	sCharMap['6'] = 6;
	sCharMap['7'] = 7;
	sCharMap['8'] = 8;
	sCharMap['9'] = 9;
	sCharMap['A'] = 10;
	sCharMap['B'] = 11;
	sCharMap['C'] = 12;
	sCharMap['D'] = 13;
	sCharMap['E'] = 14;
	sCharMap['F'] = 15;

	sReverseCharMap[0] = '0';
	sReverseCharMap[1] = '1';
	sReverseCharMap[2] = '2';
	sReverseCharMap[3] = '3';
	sReverseCharMap[4] = '4';
	sReverseCharMap[5] = '5';
	sReverseCharMap[6] = '6';
	sReverseCharMap[7] = '7';
	sReverseCharMap[8] = '8';
	sReverseCharMap[9] = '9';
	sReverseCharMap[10] = 'a';
	sReverseCharMap[11] = 'b';
	sReverseCharMap[12] = 'c';
	sReverseCharMap[13] = 'd';
	sReverseCharMap[14] = 'e';
	sReverseCharMap[15] = 'f';

	// Kick off the command thread.
	mpCommandThread = new wxCommandThread;
	if(wxTHREAD_NO_ERROR == mpCommandThread->Create())
	{
		mpCommandThread->Run();
	}
	else
	{
		// Bad things have just happened.  The user will get no site commands.
		wxLogError(wxT("Unable to create the command thread."));
	}
}

Launcher::~Launcher()
{
	mpCommandThread->Delete();
}

Launcher::DecryptionSuccess Launcher::DecryptUrl(const wxString& url, 
	wxString& plainText)
{
	DecryptionSuccess ret = Failure;

	// Url format:
	//
	// CitiesOnline://username:<salt>encodedandencryptedtext
	//
	// 1.  Get username and salt from message.
	// 2.  If username does not exist or is different, enter password.
	// 3.  SHA1 hash password.
	// 4.  Unencode encrypted text.
	// 5.  Decrypt unencoded text using SHA1 hash.

	// Strip off any trailing / from the browser.
	wxString strippedUrl = url;
	size_t length = strippedUrl.Length();
	if( (0 < length) && 
		('/' == strippedUrl[length - 1]))
	{
		strippedUrl = strippedUrl.Left(length - 1);
	}

	// If anything fails, do nothing.  Don't give hackers any information.
	wxString userName;
	wxString salt;

	if(true == GetUserNameAndSalt(strippedUrl, userName, salt))
	{
		bool havePassword = true;

		// Now that we have a valid user name, test it against our stored name.
		// If it is different, they either haven't logged in yet or have logged
		// in on the site as a different user than the one they were playing
		// with.
		if(userName != sUserName)
		{
			sUserName = userName;
			wxString password;

			havePassword = GetPassword(password);
			if(true == havePassword)
			{
				// First encrypt it to md5 because that's what the site does.
				byte md5[CryptoPP::Weak::MD5::DIGESTSIZE];

				std::string s = (char*)salt.char_str();
				std::string presalt;
				presalt.assign(&s[4], 8);

				std::string p = (char*)password.char_str();
				std::string pwd = presalt + p;

				// This crazy encryption is what the site does so we have to match it.
				char c = s[3];

				// Find this character in the itoa64 string.
				const char* a = sItoa64;
				for(; a && (*a != c); ++a);

				size_t count_log2 = a - sItoa64;
				if (count_log2 >= 7 && count_log2 <= 30)
				{
					size_t count = 1 << count_log2;

					Encryption::HashMd5(pwd, md5);

					do{
						pwd.assign(reinterpret_cast<const char*>(&md5[0]), CryptoPP::Weak::MD5::DIGESTSIZE);
						pwd += p;
						Encryption::HashMd5(pwd, md5);
					} while(--count);
				}

				// Finally, encode the md5 hash the way the site does.
				std::string out;
				Encode64(&md5[0], CryptoPP::Weak::MD5::DIGESTSIZE, out);

				// Create the final string.
				out = s + out;

				Encryption::HashSha1(out, sSHA);
			}
		}

		// If we have their password, decrypt the url using the hash.
		if(true == havePassword)
		{
			std::string encryptedText;
			std::string decryptedText;

			if(true == GetEncryptedText(strippedUrl, salt, encryptedText))
			{
				ret = Success;

				try
				{
					Encryption::Decrypt(encryptedText, sSHA, 
						CryptoPP::SHA::DIGESTSIZE, decryptedText);
				}
				catch(const CryptoPP::Exception& e)
				{
					wxMessageBox(wxString(e.GetWhat().c_str(), wxConvLibc));

					ret = Failure;
				}

				// Convert back to Unicode after decryption.
				plainText = wxString(decryptedText.c_str(), wxConvLibc);

				// It must be at least six characters in length to have a 
				// valid version string.
				if(6 >= plainText.Length())
				{
					ret = Failure;
				}

				// If we have sucessfully decrypted to plaintext, validate it.
				if(Success == ret)
				{
					ret = ValidatePlaintext(plainText);
				}
			}
		}
	}

	return ret;
}
