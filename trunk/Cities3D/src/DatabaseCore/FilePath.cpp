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
#include "FilePath.h"
#include "ConfigDatabase.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define STDPATHS wxStandardPaths::Get()

//---------------------------- STATICS          -----------------------------//
namespace
{
	typedef std::map<DataFileType, wxString> DataFileMap;
	DataFileMap sDataFileMap;
	DataFileMap sUserDataFileMap;
	wxArrayString sDataDirs;
	wxArrayString sPluginDirs;

	inline void EnumDataDirs()
	{
		if (0 == sDataDirs.GetCount())
		{
			wxString str;
			str = STDPATHS.GetUserDataDir();
			if(sDataDirs.Index(str) == wxNOT_FOUND) 
			{
				sDataDirs.Add(str);
			}

			str = wxFileName(STDPATHS.GetExecutablePath()).GetPath(
				wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);
			if(sDataDirs.Index(str) == wxNOT_FOUND)
			{
				sDataDirs.Add(str);
			}

			str = STDPATHS.GetDataDir();
			if(sDataDirs.Index(str) == wxNOT_FOUND) 
			{
				sDataDirs.Add(str);
			}
			wxASSERT(sDataDirs.GetCount() > 0);
		}
	}

	inline void EnumPluginDirs()
	{
		if (0 == sPluginDirs.GetCount())
		{
			wxString str;
			str = STDPATHS.GetUserDataDir();
			if(sPluginDirs.Index(str) == wxNOT_FOUND) 
			{
				sPluginDirs.Add(str);
			}

			str = wxFileName(STDPATHS.GetExecutablePath()).GetPath(
				wxPATH_GET_VOLUME);
			if(sPluginDirs.Index(str) == wxNOT_FOUND) 
			{
				sPluginDirs.Add(str);
			}

			str = STDPATHS.GetPluginsDir();
			if(sPluginDirs.Index(str) == wxNOT_FOUND) 
			{
				sPluginDirs.Add(str);
			}
			wxASSERT(sPluginDirs.GetCount() > 0);
		}
	}

	wxString FindInSearchPath(const wxString &middle, const wxString& conf,
			const wxString& filename)
	{
		wxPathList list;

		EnumDataDirs();

		wxArrayString top;
		
		//see if there's a user-selected subdirectory to search first
		if(false == conf.IsEmpty())
		{
			wxString ff = CONFIG.read<wxString>(swConfigSectionGeneral, conf, swEmpty);

			if(false == ff.IsEmpty())
			{
				top.Add(ff);
			}

			if(wxNOT_FOUND == top.Index(swStandard)) 
			{
				top.Add(swStandard);
			}
		}
		else
		{
			top.Add(swEmpty);
		}

		wxFileName fn;
		wxLogDebug(wxT("Search path for %s file %s:"), middle.c_str(), 
			filename.c_str());

		for(size_t i=0;i<sDataDirs.GetCount();++i)
		{
			for(size_t j=0;j<top.GetCount();++j)
			{
				fn.AssignDir(sDataDirs[i]);
				fn.AppendDir(middle);

				if(false == top[j].IsEmpty())
				{
					fn.AppendDir(top[j]);
				}

				wxString str = fn.GetFullPath();
				list.Add(str);
				wxLogDebug(wxT("\t\t%s"), str.c_str());
			}
		}

		wxString ret = list.FindAbsoluteValidPath(filename);
		if (ret.IsEmpty()) 
		{
			wxLogDebug(wxT("\tfile not found!"));
		} 
		else 
		{
			wxLogDebug(wxT("\tfile found: %s"), ret.c_str());
		}

		return ret;
	}
}

//---------------------------- PUBLIC           -----------------------------//
wxArrayString FilePath::EnumDataSubDirs(const wxString &sub)
{
	wxArrayString list;

	EnumDataDirs();

	wxFileName fn;
	for(size_t i=0;i<sDataDirs.GetCount();++i)
	{
		fn.AssignDir(sDataDirs[i]);
		fn.AppendDir(sub);

		if(fn.DirExists())
		{
			wxDir dir(fn.GetFullPath());
			wxString file;

			bool next = dir.GetFirst(&file, wxEmptyString, wxDIR_DIRS);
			while(true == next)
			{
				wxASSERT(!file.IsEmpty());
				if(list.Index(file) == wxNOT_FOUND)
				{
					wxLogDebug(wxT("found %s in %s"), file.c_str(),
							dir.GetName().c_str());
					list.Add(file);
				}
				next = dir.GetNext(&file);
			}

		}
	}
	return list;
}

wxArrayString FilePath::EnumPlugins()
{
	wxArrayString list;

	EnumPluginDirs();

	wxFileName fn;
	for(size_t i=0;i<sPluginDirs.GetCount();++i)
	{
		fn.AssignDir(sPluginDirs[i]);

#if defined(_DEBUG) && defined(__WIN32__)
		fn.AppendDir(swPluginsD);
#else
		fn.AppendDir(swPlugins);
#endif

		wxString search = wxString::Format(swSearch.c_str(), 
			wxDynamicLibrary::GetDllExt());

		if(fn.DirExists())
		{
			wxDir dir(fn.GetFullPath());
			//sanity check
			wxASSERT(dir.IsOpened());
			wxString file;

			//traverse the directory, looking for dynamically loadable library files
			bool next = dir.GetFirst(&file, search, wxDIR_FILES);
			while(true == next)
			{
				wxASSERT(!file.IsEmpty());
				if(list.Index(file) == wxNOT_FOUND)
				{
					wxLogDebug(wxT("found %s in %s"), file.c_str(), 
						dir.GetName().c_str());
					wxFileName filepath(dir.GetName(), file);
					wxString path(filepath.GetFullPath());
					list.Add(path);
				}
				else
				{
					wxLogDebug(wxT("ignored %s in %s"), file.c_str(), 
						dir.GetName().c_str());
				}
				next = dir.GetNext(&file);
			}

		}
	}
	return list;
}

wxArrayString FilePath::EnumLocaleDirs(const wxString &lang)
{
	EnumDataDirs();

	wxArrayString list;
	for(size_t i = 0; i < sDataDirs.GetCount(); ++i)
	{
		wxFileName fn;
		fn.AssignDir(sDataDirs[i]);
		fn.AppendDir(swLocale);
		fn.AppendDir(lang);

		if(fn.DirExists())
			list.Add(fn.GetFullPath());
	}
	return list;
}

wxString FilePath::Tex(const wxString &filename)
{
	return FindInSearchPath(swTex, swTexFolder, filename);
}

wxString FilePath::Skin(const wxString &filename)
{
	return FindInSearchPath(swSkins, swSkinFolder, filename);
}

wxString FilePath::Sound(const wxString &filename)
{
	return FindInSearchPath(swSounds, swSoundFolder, filename);
}

wxString FilePath::Layout(const wxString& filename)
{
	return FindInSearchPath(swLayouts, swEmpty, filename);
}

wxString FilePath::UserDataDir(const wxString &dirname)
{
	// Create "data" directory if it does not exist.
	wxFileName fn(STDPATHS.GetUserDataDir(), swEmpty);

	fn.AppendDir(swData);

	if (!dirname.empty())
		fn.AppendDir(dirname);

	if (!fn.DirExists())
	{
		fn.Mkdir(0777, wxPATH_MKDIR_FULL);
	}

	return fn.GetFullPath();
}

wxString FilePath::Data(const DataFileType type)
{
	wxASSERT(DataFileSize > (wxUint32) type);

	//set up the paths if needed
	if(0 == sDataFileMap.size())
	{
		//set the map values
		sDataFileMap[DataFileMaps] = FILE_MAP;
		sDataFileMap[DataFileMapKeys] = FILE_MAP_KEYS;
	}
	if(0 == sUserDataFileMap.size())
	{
		//set the map values
		sUserDataFileMap[DataFilePlayers] = FILE_PLAYERS;
		sUserDataFileMap[DataFileGames] = FILE_GAMES;
		sUserDataFileMap[DataFileGameKeys] = FILE_GAME_KEYS;
	}

	DataFileMap::const_iterator it;

	it = sDataFileMap.find(type);
	if (sDataFileMap.end() != it)
	{
	    wxFileName fn(STDPATHS.GetDataDir(), swEmpty);
	    fn.AppendDir(swData);
	    wxString file = fn.GetFullPath() + it->second;
	    return file;
	}
	
	it = sUserDataFileMap.find(type);
	if (sUserDataFileMap.end() != it)
	{
	    wxString file = UserDataDir() + it->second;
	    return file;
	}

	// We shouldn't reach here.
	wxASSERT(false);
	return swEmpty;
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//


