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

//---------------------------------------------------------------------------//
// Topic: UICore
//
// UICore provides a set of common UI controls that promote a constant look and
// feel across all aspects of the UI.
//
// The largest piece of UICore is the <wxBaseDialog> class, which provides a 
// foundation for all dialog windows in the game.  A helper class <ISizer> is 
// included to allow for derived dialog windows to provide their own layout 
// mechanism during construction of the window.  Ninety percent of the time (or
// more), all UI controls will be displayed in a dialog window of some sort, so
// <wxBaseDialog> is provided as the common foundation that will drive all of 
// the child controls.
//
// UICore provides several kinds of child controls.  There are two types of 
// buttons, <wxClearButton>, a custom-drawn bitmapped button, and 
// <wxFreeButton>, a borderless button useful for embedding button-clicking 
// into a window without the look of a button.  There is a list control, 
// <wxBaseListCtrl>, a tree control, <wxBaseTreeCtrl>, a check box, 
// <wxBaseCheckBox>, a static grouping box, <wxBaseStaticBox>, and two types of
// text controls.  The first is <wxBaseTextCtrl>, which allows user to enter 
// text into a field, and then <wxBaseStaticText>, which is a read-only text 
// box for displaying information to the user.
//
// UICore also provides base classes for any custom-drawn main screen UI 
// elements.  The <UI> interface allows the main screen window to dynamically 
// resize and reposition UI elements without having to know a thing about their
// internals.  The <BaseUI> class builds on the <UI> interface and provides the 
// support for custom drawing, as well as working with the <TraitsUI> class to 
// handle the dynamic resize functionality required by the <UI> interface.  
// With these classes in place, a main screen window can simply maintain a list
// of UI * objects and simply use the <UI> interface to redraw itself on all 
// size and UI resize events, making extension and maintenance a breeze.
//
// All UI elements in the game should be derived from one of these classes, to 
// maintain a consistent user interface throughout the game.
//
// Classes:
//     <BaseUI>
//     <ISizer>
//     <TraitsUI>
//     <wxBaseCheckBox>
//     <wxBaseDialog>
//     <wxBaseListCtrl>
//     <wxBaseStaticBox>
//     <wxBaseStaticText>
//     <wxBaseTextCtrl>
//     <wxBaseTreeCtrl>
//     <wxClearButton>
//     <wxFreeButton>
//     <UI>
//
// Depends On:
//     <DatabaseCore>
//     <RulesCore>
