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
#ifdef _WIN32
#include <psapi.h>

//---------------------------- USER INCLUDES    -----------------------------//
#include "Process.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	template<typename T> class CProcessData
	{
	public:    
		/*
			If you pass in a dwProcessId of 0, the current process Id is used.
			For the other arguments, see MSDN documentation for OpenProcess and
			VirtualAllocEx.
		*/
		CProcessData(DWORD dwProcessId = 0, 
			DWORD dwDesiredAccess = PROCESS_ALL_ACCESS,
			DWORD flAllocationType = MEM_COMMIT, DWORD flProtect = 
			PAGE_READWRITE)
		{
			m_hProcess = OpenProcess(dwDesiredAccess, FALSE, 
				dwProcessId ? dwProcessId : GetCurrentProcessId());
			if(m_hProcess)
			{
				m_lpData = VirtualAllocEx(m_hProcess, NULL, sizeof T, 
					flAllocationType, flProtect);
			}
		}

		~CProcessData()
		{
			if(m_hProcess)
			{            
				if(m_lpData)
				{
					VirtualFreeEx(m_hProcess, m_lpData, NULL, MEM_RELEASE);
				}
				CloseHandle(m_hProcess);
			}
		}

		//WriteData is used to copy data to memory in the foreign process

		BOOL WriteData(const T& data)
		{
			return (m_hProcess && m_lpData) ? WriteProcessMemory(
				m_hProcess, m_lpData, 
				(LPCVOID)&data, sizeof T, NULL) : FALSE;
		}

		//ReadData reads back data from memory in the foreign process

		BOOL ReadData(T* data)
		{
			return (m_hProcess && m_lpData) ? ReadProcessMemory(
				m_hProcess, m_lpData, 
				(LPVOID)data, sizeof T, NULL) : FALSE;
		}

		//Templated ReadData that's used to read a specific data type from

		//a memory address located in the foreign process

		template<typename TSUBTYPE> BOOL ReadData(
			TSUBTYPE* data, LPCVOID lpData)
		{
			return m_hProcess ? ReadProcessMemory(m_hProcess, lpData, 
				(LPVOID)data, sizeof TSUBTYPE, NULL) : FALSE;
		}

		//Gets the address of the allocated memory in the foreign process

		const T* GetData()
		{
			return (m_hProcess && m_lpData) ? (T*)m_lpData : NULL;
		}
	private:
		T m_Data;
		HANDLE m_hProcess;
		LPVOID m_lpData;
	};

	HWND FindTrayToolbarWindow()
	{
		HWND hWnd = ::FindWindow(swShell_TrayWnd.c_str(), NULL);
		if(NULL != hWnd)
		{
			hWnd = ::FindWindowEx(hWnd, NULL, swTrayNotifyWnd.c_str(), NULL);
			if(NULL != hWnd)
			{
				hWnd = ::FindWindowEx(hWnd, NULL, swSysPager.c_str(), NULL);
				if(NULL != hWnd)
				{                
					hWnd = ::FindWindowEx(hWnd, NULL, swToolbarWindow32.c_str(), NULL);
				}
			}
		}
		return hWnd;
	}

	struct TRAYDATA
	{
		HWND hwnd;                
		UINT uID;                
		UINT uCallbackMessage;    
		DWORD Reserved[2];        
		HICON hIcon;                
	};
}

//---------------------------- PUBLIC           -----------------------------//
void Process::TerminateOtherProcess(const wxString& exeName)
{
	// Get our executable name and look for it.
	wxFileName fileName(exeName);
	wxString exeFullName = fileName.GetFullName();

	// Cheat.  Since we know the other process is in the tray window, only
	// look there.
	HWND hwnd = FindTrayToolbarWindow();

	DWORD pid;
	GetWindowThreadProcessId(hwnd, &pid);
	CProcessData<TBBUTTON> data(pid);

	TBBUTTON tb = {0};
	TRAYDATA tray = {0};

	int count = ::SendMessage(hwnd, TB_BUTTONCOUNT, 0, 0);

	for(int i = 0; i < count; ++i)
	{
		::SendMessage(hwnd, TB_GETBUTTON, i, (LPARAM)data.GetData());

		data.ReadData(&tb);            
		data.ReadData<TRAYDATA>(&tray,(LPCVOID)tb.dwData);

		DWORD pid = 0;
		GetWindowThreadProcessId(tray.hwnd, &pid);

		HANDLE ph = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);

		if(ph)
		{
			TCHAR name[1024];
			GetProcessImageFileName(ph, name, 1023);
			CloseHandle(ph);

			// If we find it, shut it down.
			if(wxNOT_FOUND != wxString(name).Find(exeFullName))
			{
				PostMessage(tray.hwnd, WM_QUIT, 0, 0);
			}
		}
	}
}
#endif

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
