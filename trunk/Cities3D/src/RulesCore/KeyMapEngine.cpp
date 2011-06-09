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
#ifdef __WXGTK__
#include <X11/Xlib.h>
#endif

//---------------------------- USER INCLUDES    -----------------------------//
#include "KeyMapEngine.h"
#include "Controller.h"
#include "ConfigDatabase.h"
#include "RuleSetDatabase.h"
#include "json_spirit.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef __WIN32__
extern WXDLLEXPORT WXWORD wxCharCodeWXToMSW(int id, bool *isVirtual);
#endif

#ifdef __WXGTK__
extern KeySym wxCharCodeWXToX(int id);
#endif

//---------------------------- STATICS          -----------------------------//
bool operator<(const wxKeyEvent& lhs, const wxKeyEvent& rhs)
{
	// Use a 64 bit number to do the comparison.
	wxUint64 l = (static_cast<wxUint64>(lhs.m_keyCode) << 32) | lhs.GetModifiers();
	wxUint64 r = (static_cast<wxUint64>(rhs.m_keyCode) << 32) | rhs.GetModifiers();

	return l < r;
}

namespace
{
    bool sIgnoreNextEvent = false;

	void GetText(wxInt32 code, wxString& text)
	{
		if(0 == code)
		{
			return;
		}

#ifdef __WIN32__
		// Back convert the wx key to a Win32 virtual key.
		WORD vk = wxCharCodeWXToMSW(code, NULL);
		
		unsigned int sc = MapVirtualKeyEx(vk, 0, GetKeyboardLayout(0));

		switch(vk) {
			// Keys which are "extended" (except for Return which is Numeric Enter as extended)
			case VK_INSERT:
			case VK_DELETE:
			case VK_HOME:
			case VK_END:
			case VK_NEXT:  // Page down
			case VK_PRIOR: // Page up
			case VK_LEFT:
			case VK_RIGHT:
			case VK_UP:
			case VK_DOWN:
				sc |= 0x100; // Add extended bit
		}	
		
		TCHAR buf[256];
		GetKeyNameText(sc << 16, buf, 256);
		text += buf;
#endif

#ifdef __WXGTK__
		KeySym keySym = wxCharCodeWXToX(code);
		char *symStr = XKeysymToString(keySym);
		if(NULL != symStr)
		{
			// FIXME: need translation, this currently is the raw key name
			wxString symStrWx = wxString(symStr, wxConvLibc);
			symStrWx.Replace(wxT("_"), wxT("-"));
			text += symStrWx;
		}
#endif
	}
}

//---------------------------- PUBLIC           -----------------------------//
void KeyMapEngine::Start()
{
    wxASSERT(false == mEventMap.empty());

    // First, recover overrides from the registry.
    LoadOverrides();

    // Now get a composite event map with the overrides applied.
    EventMap eventMap;
    GetEventMap(eventMap);

    EventMap::const_iterator it, itEnd = eventMap.end();
    for(it = eventMap.begin(); it != itEnd; ++it)
    {
        const KeyMap& keymap = it->second;

        // If it's not been erased, set it.
        if(0 != keymap.defaultEvent.m_keyCode)
        {
		    mKeyMap[keymap.defaultEvent] = keymap;
        }
    }
}

void KeyMapEngine::ProcessKeyEvent(wxKeyEvent& event) const
{
    // Transmit the keystroke to anything that requires special processing.
	Controller::get().Transmit(shEventKeyDown, event);

    event.Skip();

    // Sometimes we get weird combinations where the same event will be sent
    // twice in a row.  If this happens, ignore repeated instances of the
    // event.
    if( (-1 == event.m_keyCode) ||
        (true == sIgnoreNextEvent))
    {
        sIgnoreNextEvent = false;

        return;
    }

	wxLogDebug(wxT("%d pressed."), event.GetKeyCode());

	// See if the event is in our map.
	KeyMapMap::const_iterator it = mKeyMap.find(event);
	if(mKeyMap.end() != it)
	{
		// It's here, fire the event.
		const KeyMap& keymap = it->second;
		Controller::get().Transmit(keymap.event, 0);
	}

    event.m_keyCode = -1;
}

void KeyMapEngine::GetEventMap(EventMap& eventMap) const
{
    // Start off with the original event map.
    eventMap = mEventMap;

    // Apply the overrides.
    OverrideMap::const_iterator it, itEnd = mOverrideMap.end();
    for(it = mOverrideMap.begin(); it != itEnd; ++it)
    {
        const wxString& name = it->first;
        const wxKeyEvent& event = it->second;

        EventMap::iterator itE = eventMap.find(name);
        wxASSERT(eventMap.end() != itE);

        KeyMap& keymap = itE->second;
        keymap.defaultEvent = event;
    }
}

void KeyMapEngine::OverrideKeyEvent(const wxKeyEvent& event, 
    const wxString& name, wxString& collision)
{
    // First, check for an existing collision.
    KeyMapMap::const_iterator it = mKeyMap.find(event);
    if(mKeyMap.end() != it)
    {
        const KeyMap& keymap = it->second;
        collision = keymap.name;
    }

    // Clear out any old mapping.  It may not exist if this mapping
    // was cleared by a collision and is now being reassigned to.
    KeyMapMap::iterator itK, itKEnd = mKeyMap.end();
    for(itK = mKeyMap.begin(); itK != itKEnd; ++itK)
    {
        const KeyMap& keymap = itK->second;
        if(keymap.name == name)
        {
            mKeyMap.erase(itK);
            break;
        }
    }
    
    // Find the event so we can set it to its new place in the keymap.
    EventMap::const_iterator itE = mEventMap.find(name);
    wxASSERT(mEventMap.end() != itE);

    // Update the key and override maps.
    mKeyMap[event] = itE->second;
    mOverrideMap[name] = event;
    
    if(false == collision.IsEmpty())
    {
        wxKeyEvent zeroEvent;
        zeroEvent.m_keyCode = 0;
        mOverrideMap[collision] = zeroEvent;
    }

    // Save to registry.
    SaveOverrides();
}

void KeyMapEngine::IgnoreNextEvent()
{
    sIgnoreNextEvent = true;
}

void KeyMapEngine::GetKeyText(const wxKeyEvent& event, wxString& text) const
{
	if(true == event.ControlDown())
	{
		GetText(WXK_CONTROL, text);
        text += swPlus;
	}
	if(true == event.AltDown())
	{
		GetText(WXK_ALT, text);
        text += swPlus;
	}
	if(true == event.ShiftDown())
	{
		GetText(WXK_SHIFT, text);
        text += swPlus;
	}

    // If the event is just meta keys, we're done.
    wxInt32 keyCode = event.GetKeyCode();

    switch(keyCode)
    {
    case WXK_CONTROL:
    case WXK_ALT:
    case WXK_SHIFT:
        break;
    default:
        GetText(keyCode, text);
        break;

    }
}

void KeyMapEngine::GetKeyText(const HashString& keyEvent, wxString& text) const
{
	// Look through the map for the this key event.
	KeyMapMap::const_iterator it, itEnd = mKeyMap.end();
	for(it = mKeyMap.begin(); it != itEnd; ++it)
	{
		const KeyMap& keyMap = it->second;
		if(keyEvent == keyMap.event)
		{
			GetKeyText(it->first, text);
			break;
		}
	}
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void KeyMapEngine::ProcessRuleSet(const RuleSetPtr& ruleset)
{
	// Get all of the keymaps for this rule set.
	const KeyMapArray& keymaps = ruleset->keymaps();

    KeyMapMap tempMap;

	KeyMapArray::const_iterator it, itEnd = keymaps.end();
	for(it = keymaps.begin(); it != itEnd; ++it)
	{
		const KeyMap& keymap = *it;

        // Check for collisions.
        KeyMapMap::const_iterator itK = tempMap.find(keymap.defaultEvent);
		if(tempMap.end() != itK)
		{
			const KeyMap& k = itK->second;
			wxLogError(wxString::Format(wxT("Keymap collision: %s is replacing %s."),
				keymap.name.c_str(), k.name.c_str()));
		}
        tempMap[keymap.defaultEvent] = keymap;

		// Keep track of all entries for the UI.
		EventMap::const_iterator itE = mEventMap.find(keymap.name);
		if(mEventMap.end() != itE)
		{
			wxLogError(wxString::Format(wxT("Programmer Error: Duplicate keymap entries for %s."),
				keymap.name));
		}

		mEventMap[keymap.name] = keymap;
	}
}

void KeyMapEngine::SaveOverrides()
{
    using namespace json_spirit;
    wObject obj;

    // Create a json string out of our overrides.
    OverrideMap::const_iterator it, itEnd = mOverrideMap.end();
    for(it = mOverrideMap.begin(); it != itEnd; ++it)
    {
        const wxString& name = it->first;
        const wxKeyEvent& event = it->second;

        // Check the existing event map and see if this override is
        // identical to the original.  If it is, we don't need to write it.
        EventMap::const_iterator itE = mEventMap.find(name);
        wxASSERT(mEventMap.end() != itE);

        const wxKeyEvent& original = itE->second.defaultEvent;

        if( (event.m_keyCode != original.m_keyCode) ||
            (event.GetModifiers() != original.GetModifiers()))
        {
            wArray arr;
            arr.push_back(static_cast<wxInt32>(event.m_keyCode));
            arr.push_back(event.GetModifiers());

            wPair override(name, wValue(arr));

            obj.push_back(override);
        }
    }

    wxString str = write(obj);

    // Write it to the configuration database.
    CONFIG.write<wxString>(swConfigSectionGeneral, swKeyOverrides, str);
}

void KeyMapEngine::LoadOverrides()
{
    using namespace json_spirit;

    wxString json = CONFIG.read<wxString>(swConfigSectionGeneral, swKeyOverrides, swEmpty);

	if(swEmpty == json)
	{
		return;
	}

    // Parse the string.
    wValue value;
    read(json, value);

    const wObject& top = value.get_obj();

    for(wObject::const_iterator it = top.begin(); it != top.end(); ++it)
    {
        const wPair& pair = *it;

        // Read the name out of the of the pair.
        const wxString& name = pair.name_;

        // Read the array.
        const wArray& arr = pair.value_.get_array();
        wxInt32 keyCode = arr[0].get_int();
        wxInt32 modifiers = arr[1].get_int();

        // Create the key event.
        wxKeyEvent event;
        event.m_keyCode = keyCode;
     
        if(modifiers & wxMOD_ALT) event.m_altDown = true;
		if(modifiers & wxMOD_CONTROL) event.m_controlDown = true;
		if(modifiers & wxMOD_SHIFT) event.m_shiftDown = true;
		if(modifiers & wxMOD_META) event.m_metaDown = true;

        mOverrideMap[name] = event;
    }
}
