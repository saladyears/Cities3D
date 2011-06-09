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
// Topic: Cities3D
//
// Cities3D is a cross-platform implementation of the board game Settlers of 
// Catan with all expansions, including Seafarers, and Cities & Knights.  It 
// displays the game world in 3 dimensions, with rotation, panning, and 
// zooming, in bright beautiful textures contributed to the project freely by 
// talented individuals from around the world.  The user interface is entirely
// custom drawn, with crisp, clean lines and dozens of high-quality bitmaps to 
// facilitate an intuitive, pleasing experience for users.  Cities3D has an 
// extensible plugin architecture that allows new rules and variants to be 
// added into the game without changing an ounce of code in existing rules or 
// variants.  In fact, the core rules themselves for Settlers, and the 
// expansions, are all plugins.
//
// Cities3D is built on the firm foundation of OpenGL 
// (<http://www.opengl.org>), wxWidgets (<http://www.wxWidgets.org>) and boost 
// (<http://www.boost.org>), all real-world solutions that have been used in 
// production code.  The architecture was built with a lot of thought, and a 
// lot of Design Patterns, in an attempt to make a truly exceptional object-
// oriented system.  The sound library is fmod (<http://www.fmod.org>), and the
// unit testing is done with cppunit
// (<http://cppunit.sourceforge.net/cgi-bin/moin.cgi>).  The documentation you
// are reading was created using NaturalDocs (<http://www.naturaldocs.org>).
//
// The Cities3D module is the capstone module that brings everything together.
// As a result, it is a disparate collection of classes.  Some for the 
// application framework, some for the custom controls, some for the many 
// dialog windows, some for drag and drop functionality, and some for the 
// totally custom-drawn UI pieces. When it comes down to it, the Cities3D 
// module is nothing more than the pretty front end for all of the work 
// happening in other modules.  Cities3D is the View for the other modules 
// Model.  Through an implementation of the Observer pattern, Cities3D keeps 
// watch on all of the data and updates itself when notified that the data has
// changed.  It also provides mechanisms for the user to signal for certain 
// actions to take place, which uses the Command pattern to execute the user's 
// request.  Each of the modules that Cities3D is built upon has been cleanly 
// and logically separated from the others to maintain good stratification and
// encapsulation.  These other modules are:
// DatabaseCore - Provides mundane, but important game management tasks.
// GLCore - Provides a set of OpenGL windows and helper classes.
// ModelCore - Provides all data classes, with helper classes and functions.
// RulesCore - Provides the game play execution engine.
// UICore - Provides the building blocks for a consistent UI look and feel.
//
// It all starts with the <wxCities3DApp> class, the main application class 
// built on the wxWidgets application infrastructure.  The <wxCities3DApp> 
// class creates the <wxMainFrame> top-level frame window, which holds the 
// <SettlersView> main game window.  Inside the main game window are all of the
// custom-drawn UI elements responsible for displaying the game information to 
// the user.  These elements include <BankUI>, for showing the bank data, 
// <BuildUI>, for showing available build options, <ControlsUI>, for showing 
// what actions the user can take, <MessageUI>, for displaying text information
// to the user, and <PlayerUI>, for showing the user what they have.
//
// There are many other classes in Cities3D, particularly the dialog windows, 
// and the controls located within them (which includes all of the drag and 
// drop functionality). See the menus on the left for further information about
// any of them.
//
// Classes:
//     <BankUI>,
//     <BuildUI>,
//     <ControlsUI>,
//     <IPlayerDropTarget>,
//     <MessageUI>,
//     <PlayerUI>,
//     <SettlersView>,
//     <wxAddPlayerDialog>,
//     <wxCities3DApp>,
//     <wxGameListCtrl>,
//     <wxGameSetupDialog>,
//     <wxOpenGameDialog>,
//     <wxMainFrame>,
//     <wxMapListCtrl>,
//     <wxMapTextCtrl>,
//     <wxPlayerDataObject>,
//     <wxPlayerDropListCtrl>,
//     <wxPlayerDropTarget>,
//     <wxPlayerListCtrl>,
//     <wxPlayerTreeCtrl>,
//     <wxSelectMapDialog>,
//     <wxSettingsDialog>,
//     <wxSplashDialog>,
//     <wxOptionDisplaySize>,
//     <wxOptionWindow>
//
// Depends On:
//     <DatabaseCore>,
//     <GLCore>,
//     <ModelCore>,
//     <RulesCore>,
//     <UICore>
