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
#include <map>
#include <set>
#include <wx/image.h>

//---------------------------- USER INCLUDES    -----------------------------//
#include "define/defineGL.h"
#include "FrameworkGLCanvas.h"

//---------------------------- DEFINES          -----------------------------//
#define DRAW_SPECIAL			0		//turn this on to use special drawing

//---------------------------- TYPEDEFS         -----------------------------//
class IDrawObject;
typedef boost::shared_ptr<IDrawObject> DrawObjectPtr;

class AggregatorObject;
typedef boost::shared_ptr<AggregatorObject> AggregatorObjectPtr;

class SelectionObject;
typedef boost::shared_ptr<SelectionObject> SelectionObjectPtr;

struct Vector;

class FramebufferObject;
class Renderbuffer;
class RenderTexture;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: wxSettlersGLCanvas
// 
// Renders all game objects in OpenGL.  The class wxSettlersGLCanvas is
// the top-level OpenGL canvas in the game.  It maintains a map
// of <IDrawObject> objects, sorted by draw priority.  Each <IDrawObject> is
// is responsible for drawing a single <IGameObject>.  Each time the scene
// is rendered, each <IDrawObject> is asked if it can be deleted, which
// happens when the <IGameObject> it refers to is destroyed.  If the 
// <IDrawObject> can be deleted, it is removed from the map and destroyed.  If
// it is valid, the <IDrawObject> is rendered to the screen.
//
// This class also handles game object selection in OpenGL.  When a <Rule> 
// signals to the View that a user needs to make a selection of a game object, 
// it sends a <SelectionObject> via the <Controller> with the selection 
// information.  wxSettlersGLCanvas listens for that <Event> and then maintains
// the given <SelectionObject> until a valid selection is made.  It then 
// executes the <Rule> associated with the <SelectionObject> via the 
// <RuleEngine> and destroys the no longer needed <SelectionObject>.  In this
// way, the wxSettlersGLCanvas is allowed to know absolutely nothing about any
// particular game object, but simply passes the information along to the 
// appropriate <Rule> for it to decide what to do.
//
// Also provided with wxSettlersGLCanvas is a utility function to convert any 
// <IDrawObject> into a wxBitmap of that object.  This allows for any game 
// object to be represented in a bitmap image on the fly without having to 
// create the bitmap externally
// and load it into the application.  The object is rendered in the back buffer
// of the OpenGL canvas, read into the internal memory of a wxImage, scaled, 
// and then converted into the outgoing wxBitmap.
//
// Derived From:
//     <wxFrameworkGLCanvas>
//
// Project:
//     <GLCore>
//
// Include:
//     SettlersGLCanvas.h
//
class GL_EXPORT_SPEC wxSettlersGLCanvas : public wxFrameworkGLCanvas
{
	//-----------------------------------------------------------------------//
	// Section: Public
	//
public:
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: wxSettlersGLCanvas
	// 
	// The wxSettlersGLCanvas constructor.  Adds OnGameObjects as 
	// a receiver of the eventGameObjects <Event>, and OnSelectionObject 
	// as a receiver of the eventSelectionObject <Event>.  Then calls 
	// the <DrawObjectEngine> to obtain all OpenGL resources needed.
	// 
	// Parameters:
	//     parent - The parent window (must not be NULL).
	//     id - The id for message handling.
	//     pos - The starting position.
	//     size - The starting size.
	//     style - The window style, using wxWindow parameters.
	//     name - The name.
	//     attrib - Starting OpenGL implementation attributes.
	//
	wxSettlersGLCanvas(wxWindow *parent, const wxWindowID id = -1, 
		const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = 0, 
		const wxString& name = swSettlersGLCanvas, 
		wxInt32 *attrib = NULL);

	//-----------------------------------------------------------------------//
	// Group: Destructor
	//

	//-----------------------------------------------------------------------//
	// Destructor: ~wxSettlersGLCanvas
	// 
	// The wxSettlersGLCanvas destructor.  The destructor removes
	// all receivers from the <Controller>, and then calls the
	// <DrawObjectEngine> to release all obtained OpenGL resources.
	// 
	virtual ~wxSettlersGLCanvas();

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: ConvertGLtoWX
	//
	// Converts an <IDrawObject> into a wxBitmap of the given size.
	// ConvertGLtoWX takes a given <IDrawObject>, renders it in OpenGL
	// in the back buffer of the current context, and then converts 
	// the rendered image into a wxImage object with the
	// given width and height dimensions. The trick is that the 
	// OpenGL rendering area is created much larger than the desired
	// dimensions.  The pixels are then read from OpenGL into a wxImage
	// of the larger size, which is then scaled down to meet the required
	// size.  This removes any jagged aliasing lines that might have
	// existed in the rendered OpenGL image.  Finally, the wxImage is 
	// converted into the outgoing wxBitmap.
	//
	// Parameters:
	//     object - The <IDrawObject> to render.
	//     width - The width of the desired wxBitmap.
	//     height - The height of the desired wxBitmap.
	//     eye - The eye position for the camera when rendering the 
	//           <IDrawObject>. ConvertGLtoWX assumes that the object will be
	//           centered on (0, 0, 0).
	//
	// Returns:
	//     A wxBitmap containing the converted image.
	//
	wxBitmap ConvertGLtoWX(const DrawObjectPtr &object, const size_t width, 
		const size_t height, const Vector &eye);

	//-----------------------------------------------------------------------//
	// Section: Protected
	//
protected:
	//-----------------------------------------------------------------------//
	// Function: GenerateDebugInfo
	//
	// Adds debug information to the given string.
	// 
	// Parameters:
	//     str - A string of debug info.
	//
	virtual void GenerateDebugInfo(StringArray &str);

	//-----------------------------------------------------------------------//
	// Function: PopupSelect
	//
	// Tests each draw object to see if the mouse is over it for a popup
	// window.
	//
	DrawObjectPtr PopupSelect(wxInt32 x, wxInt32 y) const;

	//-----------------------------------------------------------------------//
	// Section: Private
	//
private:
	//-----------------------------------------------------------------------//
	// Group: Virtual Functions
	//

	//-----------------------------------------------------------------------//
	// Function: DrawScene
	//
	// Renders the scene.  First applies the world rotation
	// to the current matrix and then traverses the entire priority map
	// of <IDrawObjects>, rendering each one, unless it should
	// be deleted, in which case it is removed from the map.
	//
	virtual void DrawScene();

	//-----------------------------------------------------------------------//
	// Function: Select
	//
	// Does object selection checking based on the given
	// window coordinates.  Determines the origin point in 
	// world coordinates and the ray from that point into the
	// world.  Calls CanSelect on each <IDrawObject>, selecting
	// the object nearest the origin. along the ray.
	//
	// Parameters:
	//     x - The x coordinate of the current mouse position.
	//     y - The y coordinate of the current mouse position.
	//
	// Returns:
	//     The OpenGL id of nearest object, or -1 if none.
	//
	virtual wxInt32 Select(const wxInt32 x, const wxInt32 y) const;

	//-----------------------------------------------------------------------//
	// Function: SelectObject
	//
	// Handles game object selection.  If a valid object was selected,
	// the current <SelectionObject> is reset, and the <Rule> that
	// it referred to is called with the ID of the selected object as
	// its <DataObject> parameter.
	//
	// Parameters:
	//     id - The OpenGL id of the selected object.
	//
	virtual void SelectObject(wxInt32 id);

	//-----------------------------------------------------------------------//
	// Group: Functions
	//

	//-----------------------------------------------------------------------//
	// Function: SelectPrivate
	//
	// Performs the actual test for selection.
	//
	// Parameters:
	//     x - The x coordinate of the current mouse position.
	//     y - The y coordinate of the current mouse position.
	//     object - [out] - The <IDrawObject> that was selected.
	//     popup - A bool that is true if the selection is being tested for a
	//             popup and false if not.
	//
	// Returns:
	//     The OpenGL id of nearest object, or -1 if none.
	//
	wxInt32 SelectPrivate(wxInt32 x, wxInt32  y, DrawObjectPtr& object,
		bool popup = false) const;

	//-----------------------------------------------------------------------//
	// Function: BeginGLtoWX
	//
	// Prepares the app for a render to a bitmap based on the current mode.
	//
	// Parameters:
	//     size - The width and height of the bitmap to render.
	//
	void BeginGLtoWX(const size_t size) const;

	//-----------------------------------------------------------------------//
	// Function: RenderGLtoWX
	//
	// Renders the given object into the render buffer.
	//
	// Parameters:
	//     object - The <IDrawObject> to render.
	//     eye - The eye position for the camera when rendering the 
	//           <IDrawObject>.
	//     size - The width and height of the bitmap to render.
	//
	// Returns:
	//     The rendered bitmap in wxImage form.
	//
	wxImage RenderGLtoWX(const DrawObjectPtr &object, const Vector &eye, 
		const size_t size) const;

	//-----------------------------------------------------------------------//
	// Function: EndGLtoWX
	//
	// Cleans up the render to bitmap based on the current mode.
	//
	void EndGLtoWX();

	//-----------------------------------------------------------------------//
	// Function: ProcessGLtoWX
	//
	// Processes the rendered image to put it in a form that is correct in
	// wxWidgets and crops it if needed based on the given dimensions.
	//
	// Parameters:
	//     image - The image to process.
	//     width - The desired width of the image.
	//     height - The desired height of the image.
	//
	// Returns:
	//     The processed image.
	//
	wxImage ProcessGLtoWX(wxImage image, const size_t width, 
		const size_t height) const;

	//-----------------------------------------------------------------------//
	// Group: Game Event Functions
	//

	//-----------------------------------------------------------------------//
	// Function: OnGameObjects
	//
	// Creates <IDrawObjects> for new <IGameObjects> and stores
	// them in the priority draw map.  Calls the <DrawObjectEngine>
	// for each <IGameObject> in the given <AggregatorObject>.  If a 
	// valid <IDrawObject> is returned from the <DrawObjectEngine>, the
	// <IDrawObject> is added into the priority draw map according to its
	// priority and then drawn in the next render pass.  Triggered by the
	// eventGameObjects <Event>.
	//
	void OnGameObjects(const AggregatorObjectPtr &object);

	//-----------------------------------------------------------------------//
	// Function: OnSelectionObject
	//
	// Stores the given <SelectionObject> and then prepares the canvas
	// for selection.  Calls OnGameObjects with the given <SelectionObject>
	// so that any needed visual representation in the scene is created.
	// Turns on selection handling by calling SetHitTest in the base class
	// <wxFrameworkGLCanvas>.  Triggered by the eventSelectionObject <Event>.
	//
	// Parameters:
	//     object - The new <SelectionObject>.
	//
	void OnSelectionObject(const SelectionObjectPtr &object);

	//-----------------------------------------------------------------------//
	// Function: OnShutdownUI
	//
	// Shuts down any selection object currently shown when a global shutdown
	// occurs.
	//
	void OnShutdownUI(wxInt32);

	//-----------------------------------------------------------------------//
	// Function: OnResetCamera
	//
	// Resets the eye point to the loaded map's default.
	//
	// Parameters:
	//     dummy - Needed to satisfy <Event>'s one parameter requirement.
	//             *Unused*.
	//
	void OnResetCamera(bool dummy);

	//-----------------------------------------------------------------------//
	// Function: OnImageUpdate
	//
	// Updates the resources used by all <IDrawObjects> in the
	// <IDrawObjectEngine>.  Calls <ReleaseAll> followed by <ObtainAll>
	//
	// Parameters:
	//     dummy - A dummy parameter required by <Events>.  *Unused*.
	//
	void OnImageUpdate(bool dummy);

	//-----------------------------------------------------------------------//
	// Function: OnDirty
	//
	// Marks the scene as dirty so it will be redrawn.
	//
	void OnDirty(wxInt32);

	//-----------------------------------------------------------------------//
	// Function: OnRenderBitmaps
	//
	// Handles a change in the users' desired method of bitmap rendering.
	//
	void OnRenderBitmaps(wxInt32 = 0);

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	typedef std::multimap<wxInt32, DrawObjectPtr> PriorityMap;

	//-----------------------------------------------------------------------//
	// Object: mPriorityMap
	//
	// A std::multimap of the <IDrawObjects> to render.  A
	// std::multimap is ideal in this situation because it allows multiple
	// objects with the same key.  Since it does not matter which order objects
	// of the same priority are drawn in, just adding new objects to the 
	// multimap with their priorities keeps them sorted in proper priority
	// order.
	//
	PriorityMap mPriorityMap;

	//-----------------------------------------------------------------------//
	// Object: mSelectionObject
	//
	// The current <SelectionObject>, if any.  It is maintained here
	// because if it was not, it would go out of scope, be destroyed, and then
	// any <IDrawObject> that referred to it would be destoryed in the next
	// render pass, making it worthless.  Therefore, it persists in
	// the wxSettlersGLCanvas until something is selected, at which point
	// it is destroyed.
	//
	SelectionObjectPtr mSelectionObject;

	enum RenderToBitmapMode
	{
		Nonebuffer		= 0,
		Backbuffer		,
		Pixelbuffer		,
		Framebuffer		,
	};

	//-----------------------------------------------------------------------//
	// Int: mRenderToBitmapMode
	//
	// The render to bitmap mode used by <ConvertGLtoWX>.
	//
	RenderToBitmapMode mRenderToBitmapMode;

	//-----------------------------------------------------------------------//
	// Object: mpFBO
	//
	// The <FramebufferObject> used by <ConvertGLtoWX> if the 
	// GL_EXT_framebuffer_object extension is available.
	//
	boost::scoped_ptr<FramebufferObject> mpFBO;

	//-----------------------------------------------------------------------//
	// Object: mpFBO
	//
	// The <Renderbuffer> used by <ConvertGLtoWX> if the 
	// GL_EXT_framebuffer_object extension is available.
	//
	boost::scoped_ptr<Renderbuffer> mpRB;

	//-----------------------------------------------------------------------//
	// Object: mpRenderTexture
	//
	// The <RenderTexutre> used by <ConvertGLtoWX> if pixel buffers are
	// available.
	//
	mutable boost::scoped_ptr<RenderTexture> mpRenderTexture;

	//-----------------------------------------------------------------------//
	// Object: mRenderImage
	//
	// The image buffer that reads from the pixel buffer texture.
	//
	wxImage mRenderImage;
};
//---------------------------- PROTOTYPES       -----------------------------//
