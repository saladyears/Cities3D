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
// Topic: GLCore
//
// GLCore provides the underlying window classes and tools needed to create and
// use an OpenGL window with a camera, lighting, selection, mouse/keyboard 
// handling, and <IDrawObject> rendering.  
//
// There are two abstract classes and one concrete class available for creating
// new OpenGL windows.  The lightweight class <wxBaseGLCanvas> provides a very 
// simple foundation for OpenGL drawing.  It handles setting up lighting, 
// perspective, and handles window resizing, but has no camera, mouse, or 
// selection support.  The slightly sturdier (though still light) class
// <wxFrameworkGLCanvas> builds on <wxBaseGLCanvas> but also adds a camera,
// mouse handling for rotation, zooming, and panning, and object selection.
// Both classes contain pure virtual functions and must be derived from in 
// order to be used.
//
// However, the class <wxSettlersGLCanvas> is a standalone class derived from
// <wxFrameworkGLCanvas> that maintains and renders a priority map of 
// <IDrawObject> objects. The class fully implements the object selection from 
// <wxFrameworkGLCanvas> and utilizes the <RuleEngine> to notify the <Rule> 
// engine when a game object has been selected by the user.  It also provides 
// functionality for saving and loading the camera position, lookat and world 
// rotation via the <ConfigDatabase>, and provides a powerful utility function 
// for converting any OpenGL-rendered <IDrawObject> into a UI-friendly wxBitmap.
//
// In summary, derive from <wxBaseGLCanvas> if you need a static, 
// non-interactive OpenGL window.  Derive from <wxFrameworkGLCanvas> if you 
// need mouse handling or object selection testing.  And utilize or derive from
// <wxSettlersGLCanvas> if you need a top-level OpenGL window capable of 
// working with the game's <RuleEngine>.
//
// GLCore also provides the <GLTextures> namespace, which has two functions, 
// BuildTexture, and ReadGLTextures.  These functions provide a mechanism to 
// create OpenGL textures in a granularity of one at a time (with 
// BuildTexture), or by reading multiple images from a file and converting them
// all to textures (with ReadGLTextures).
//
// <GLMaterial> is another class included with GLCore that wraps all of the 
// OpenGL functions for manipulating properties of materials.  It can handle 
// setting ambient, diffuse, emissive, specular, and shininess properties for 
// any object in the OpenGL canvas.
//
// Finally, <DrawObject> provides a base class for all OpenGL-rendered objects 
// in the game.  It handles everything necessary for handling draw parameters 
// and preparing the canvas for drawing the object without actually drawing the
// object itself, which must be done by derived classes.
//
// Classes:
//     <DrawObject>,
//     <GLMaterial>,
//     <wxBaseGLCanvas>,
//     <wxFrameworkGLCanvas>,
//     <wxSettlersGLCanvas>
//
// Namespaces:
//     <GLTextures>
//
// Depends On:
//     <DatabaseCore>,
//     <ModelCore>,
//     <RulesCore>
