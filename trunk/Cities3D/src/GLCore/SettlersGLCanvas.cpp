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
#include <GL/glew.h>

//---------------------------- USER INCLUDES    -----------------------------//
#include "RenderTexture.h"
#include "SettlersGLCanvas.h"
#include "DataObject.h"
#include "ImageEngine.h"
#include "Map.h"
#include "Controller.h"
#include "DrawObject.h"
#include "AggregatorObject.h"
#include "SelectionObject.h"
#include "GLMaterial.h"
#include "IAnimation.h"
#include "ConfigDatabase.h"
#include "framebufferObject.h"
#include "renderbuffer.h"
#include "IDrawObjectEngine.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef __WIN32__
#ifdef max
#undef max
#endif
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	GLuint sRenderTex;

	const wxUint32 RENDER_TEXTURE_SIZE		= 256;
	const wxUint32 RENDER_TEXTURE_STRIDE	= RENDER_TEXTURE_SIZE * 3;

	class AnimationDrawObject : public IDrawObject
	{
	public:
		AnimationDrawObject()
		{
			Controller::get().AddReceiver(shEventAnimation,
				&AnimationDrawObject::OnAnimation, this);
		}

		~AnimationDrawObject()
		{
			Controller::get().RemoveReceiver(shEventAnimation,
				&AnimationDrawObject::OnAnimation, this);
		}

		virtual IGameObject *GetObject() const
		{
			return NULL;
		}

		virtual bool CanBuild(const GameObjectPtr &) const
		{
			return false;
		}

		virtual bool CanDelete() const
		{
			return false;
		}

		virtual bool CanSelect(const Vector &, const Vector &, float &, 
			wxInt32 &, bool)
		{
			return false;
		}

		virtual IDrawObject *Clone(const GameObjectPtr &) const
		{
			return NULL;
		}

		virtual wxInt32 GetPriority() const
		{
			return 1;
		}

		virtual void Render(const wxInt32) const
		{
			// Run animations.
			if(false == mAnimationSet.empty())
			{
				AnimationSet::iterator it = mAnimationSet.begin(), 
					itEnd = mAnimationSet.end();
				while(it != itEnd)
				{
					AnimationSet::iterator itThis = it++;

					const AnimationPtr &animation = (*itThis);
					
					// See if it's done before animating.
					if(true == animation->IsDone())
					{
						mAnimationSet.erase(itThis);
					}
					else
					{
						animation->Animate();
					}
				}
			}
		}

		virtual void Obtain()
		{
		}

		virtual void Release()
		{
		}

		virtual void PopupFunction(wxString&, wxString&, wxBitmap&, bool &, 
			ColorType&)
		{
		}

	private:
		void OnAnimation(const AnimationPtr &animation)
		{
			mAnimationSet.insert(animation);
		}

		typedef std::set<AnimationPtr> AnimationSet;
		mutable AnimationSet mAnimationSet;
	};
}

//---------------------------- PUBLIC           -----------------------------//
wxSettlersGLCanvas::wxSettlersGLCanvas(wxWindow *parent, const wxWindowID id, 
									   const wxPoint& pos, const wxSize& size, 
									   long style, 
									   const wxString& name, wxInt32 *attrib) :
wxFrameworkGLCanvas(parent, id, pos, size, style, name, attrib)
, mRenderToBitmapMode(Backbuffer)
{
	SetFocus();

	//hook up the event that listeners for new game objects to draw
	Controller::get().AddReceiver(shEventGameObjects, 
		&wxSettlersGLCanvas::OnGameObjects, this);
	Controller::get().AddReceiver(shEventSelectionObject, 
		&wxSettlersGLCanvas::OnSelectionObject, this);
	Controller::get().AddReceiver(shEventShutdownUI,
		&wxSettlersGLCanvas::OnShutdownUI, this);
	Controller::get().AddReceiver(shEventResetCamera, 
		&wxSettlersGLCanvas::OnResetCamera, this);
	Controller::get().AddReceiver(shEventImageUpdate, 
		&wxSettlersGLCanvas::OnImageUpdate, this);
	Controller::get().AddReceiver(shEventDirty, &wxSettlersGLCanvas::OnDirty, 
		this);
	Controller::get().AddReceiver(shEventRenderBitmaps, 
		&wxSettlersGLCanvas::OnRenderBitmaps, this);
	
	//obtain all needed resources
	DRAWOBJECT.ObtainAll();

	// Set up bitmap rendering.
	OnRenderBitmaps();

	// Add the animation object.
	DrawObjectPtr draw = DrawObjectPtr(new AnimationDrawObject);
	mPriorityMap.insert(std::make_pair(draw->GetPriority(), draw));
}

wxSettlersGLCanvas::~wxSettlersGLCanvas()
{
	Controller::get().RemoveReceiver(shEventGameObjects, 
		&wxSettlersGLCanvas::OnGameObjects, this);
	Controller::get().RemoveReceiver(shEventSelectionObject, 
		&wxSettlersGLCanvas::OnSelectionObject, this);
	Controller::get().RemoveReceiver(shEventShutdownUI,
		&wxSettlersGLCanvas::OnShutdownUI, this);
	Controller::get().RemoveReceiver(shEventResetCamera, 
		&wxSettlersGLCanvas::OnResetCamera, this);
	Controller::get().RemoveReceiver(shEventImageUpdate, 
		&wxSettlersGLCanvas::OnImageUpdate, this);
	Controller::get().RemoveReceiver(shEventDirty, 
		&wxSettlersGLCanvas::OnDirty, this);
	Controller::get().RemoveReceiver(shEventRenderBitmaps, 
		&wxSettlersGLCanvas::OnRenderBitmaps, this);
	
	//release all resources used
	DRAWOBJECT.ReleaseAll();
}

wxBitmap wxSettlersGLCanvas::ConvertGLtoWX(const DrawObjectPtr &object, 
										   const size_t width, 
										   const size_t height, 
										   const Vector &eye)
{
	wxASSERT(object);

	wxImage image;

	if( (FALSE != GetContext()) &&
		(0 < width) &&
		(0 < height))
	{
		SetCurrent();

		//try to use a 4x size image for creating the initial image, which will 
		//be scaled down to required size and then cropped if needed
		size_t dim = std::max(width, height);
		size_t size = (dim << 2);

		// If we're using pixel buffers, we have to stay within the render
		// buffer boundary size.
		if( (Pixelbuffer == mRenderToBitmapMode) &&
			(RENDER_TEXTURE_SIZE < size))
		{
			size >>= 1;

			// This should never happen, but be safe.
			if(RENDER_TEXTURE_SIZE < size)
			{
				size = dim;
				wxASSERT(RENDER_TEXTURE_SIZE >= size);
			}
		}

		// Get everything ready depending on the mode we're in.
		BeginGLtoWX(size);

		// Render it.
		image = RenderGLtoWX(object, eye, size);

		// Clean up.
		EndGLtoWX();

		// And finally, conver the image into a usable form.
		image = ProcessGLtoWX(image, width, height);
	}
	
	return wxBitmap(image);
}

//---------------------------- PROTECTED        -----------------------------//
void wxSettlersGLCanvas::GenerateDebugInfo(StringArray &str)
{
	wxString render;

	switch(mRenderToBitmapMode)
	{
	case Framebuffer:
		render = swFramebuffer;
		break;
	case Pixelbuffer:
		render = swPixelBuffer;
		break;
	case Backbuffer:
		render = swBackBuffer;
		break;
	case Nonebuffer:
		render = swOff;
		break;
	default:
		wxASSERT(false);
		break;
	}

	str.push_back(wxString::Format(swGLBitmaps.c_str(), render.c_str()));
}

DrawObjectPtr wxSettlersGLCanvas::PopupSelect(wxInt32 x, wxInt32 y) const
{
	DrawObjectPtr object;
	SelectPrivate(x, y, object, true);

	return object;
}

//---------------------------- PRIVATE          -----------------------------//
void wxSettlersGLCanvas::DrawScene()
{
	const Vector &camera = GetCameraPos();
	const Vector &lookat = GetCameraLookAt();

	gluLookAt(camera.x, camera.y, camera.z, 
		lookat.x, lookat.y, lookat.z, 
		0.0, 1.0, 0.0);

	// Make sure that textures are on for drawing
	glEnable(GL_TEXTURE_2D);

	PriorityMap::iterator it = mPriorityMap.begin(), itEnd = mPriorityMap.end();
	while(it != itEnd)
	{
		PriorityMap::iterator itNow(it++);

		if(true == itNow->second->CanDelete())
		{
			mPriorityMap.erase(itNow);
		}
		else
		{
			itNow->second->Render(GetSelection());
		}
	}

	//turn off textures when done
	glDisable(GL_TEXTURE_2D);
}

wxInt32 wxSettlersGLCanvas::Select(const wxInt32 x, const wxInt32 y) const
{
	DrawObjectPtr object;
	wxInt32 id = SelectPrivate(x, y, object);

	return id;
}

void wxSettlersGLCanvas::SelectObject(wxInt32 selection)
{
	//check for a good selection
	if(selection < 0) 
	{
		return;
	}

	// It is possible, with network rules, for an object to have its Select
	// state turned on, but not be selectable for the player at this computer,
	// so it has to be a final hit, AND a valid SelectionObject in order to
	// be final.
	if( (true == GotFinalHit()) &&
		(mSelectionObject))
	{
		// Clear out any current render state.
		Controller::get().Transmit(shEventShowResources, DataObject(false));

		//reset the selection object as it is no longer needed
		HashString rule = mSelectionObject->rule();
		mSelectionObject.reset();

		DataObject data(selection);

		//no more hit tests needed
		SetHitTest(false);

		//run the rule in question
		RULE.Execute(rule, data);

		// Any time a selection object is handled, save the game.
		RULE.Execute(shRuleAutoSave, DataObject());
	}
}

wxInt32 wxSettlersGLCanvas::SelectPrivate(wxInt32 x, wxInt32  y, 
	DrawObjectPtr& object, bool popup) const
{
	// Run through each object, testing for selection.

	// What's this? a legitimate use of const_cast??
	wxASSERT(const_cast<wxSettlersGLCanvas *>(this)->GetContext());
	const_cast<wxSettlersGLCanvas *>(this)->SetCurrent();

	glLoadIdentity();

	const Vector &camera = GetCameraPos();
	const Vector &lookat = GetCameraLookAt();

	gluLookAt(camera.x, camera.y, camera.z, 
		lookat.x, lookat.y, lookat.z, 
		0.0, 1.0, 0.0);

	GLint viewport[4];
	GLdouble mvmatrix[16], projmatrix[16];
	
	//retrieve projection values
	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);

	GLdouble winX = (GLdouble) x;
	GLdouble winY = (GLdouble) (viewport[3] - y);
	GLdouble nearX, nearY, nearZ;
	
	//first, determine world origin of click
	gluUnProject(winX, winY, 0.0, mvmatrix, projmatrix, viewport, &nearX, 
		&nearY, &nearZ);
	
	GLdouble farX, farY, farZ;
	gluUnProject(winX, winY, 1.0, mvmatrix, projmatrix, viewport, &farX, &farY,
		&farZ);
	
	Vector ray(farX - nearX, farY - nearY, farZ - nearZ);
	
	ray.Normalize();

	wxInt32 id = -1, objectid;
	float nearest = 0.0f, distance;

	//look for the closest intersecting object
	PriorityMap::const_iterator it = mPriorityMap.begin(), itEnd = 
		mPriorityMap.end();
	for(; it != itEnd; ++it)
	{
		const DrawObjectPtr &thisObject = it->second;

		if(true == thisObject->CanSelect(camera, ray, distance, objectid, 
			popup))
		{
			distance = fabs(distance);

			if(true == popup)
			{
				wxLogDebug(wxT("Found id %d, distance %.4f"), objectid, 
					distance);
			}

			if( (-1 == id) ||
				(distance < nearest))
			{
				id = objectid;
				object = thisObject;
				nearest = distance;
			}
		}
	}
	
	return id;
}

void wxSettlersGLCanvas::BeginGLtoWX(const size_t size) const
{
	glEnable(GL_TEXTURE_2D);

	switch(mRenderToBitmapMode)
	{
	case Framebuffer:
		{
		// Prepare the destination texture.
		glGenTextures(1, &sRenderTex);
		glBindTexture(GL_TEXTURE_2D, sRenderTex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size, 0, GL_RGB, GL_FLOAT, 
			0);

		// Bind the frame buffer object.
		mpFBO->Bind();

		mpFBO->AttachTexture(GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, 
			sRenderTex);

		// Attach a depth render buffer.
		mpRB->Set(GL_DEPTH_COMPONENT24, size, size);
		mpFBO->AttachRenderBuffer(GL_DEPTH_ATTACHMENT_EXT, mpRB->GetId());

		mpFBO->IsValid();

		glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
		glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
		}
		break;
	case Pixelbuffer:
		{
		// Start recording.
		mpRenderTexture->BeginCapture();
		glEnable(GL_TEXTURE_2D);

		}
		break;
	case Backbuffer:
		// With nothing, just try and render to the back buffer.
		glDrawBuffer(GL_BACK);
		glReadBuffer(GL_BACK);

		break;
	case Nonebuffer:
	default:
		wxASSERT(false);
		break;
	}
}

wxImage wxSettlersGLCanvas::RenderGLtoWX(const DrawObjectPtr &object, 
									  const Vector &eye, 
									  const size_t size) const
{
	wxImage image(size, size);

	Vector coords(0.0f, 0.0f, 0.0f);
	Vector rotation(0.0f, 0.0f, 0.0f);

	IGameObject* gameObject = object->GetObject();
	if(NULL != gameObject)
	{
		coords = gameObject->coords();
		rotation = gameObject->rotation();

		gameObject->coords(gameObject->uicoords());
		gameObject->rotation(Vector(0.0, 0.0, 0.0));
	}

	//zoom in with the viewport
	wxInt32 size2 = size * 2;
	wxInt32 size5 = size * 5;
	glViewport(-size2, -size2, size5, size5);
	
	//select the projection matrix and reset it
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//calculate the aspect ratio
	gluPerspective(45.0f, 1.0, 0.01f, 250.);

	//reset the modelview matrix
	glMatrixMode(GL_MODELVIEW);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(eye.x, eye.y, eye.z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	GLMaterial white(wxColour(255, 255, 255));
	white.UseMaterial();

	//render the object
	object->Render(-1);
	
	glFinish();

	// Now create the bitmap.

	switch(mRenderToBitmapMode)
	{
	case Framebuffer:
	case Backbuffer:
		glReadPixels(0, 0, size, size, GL_RGB, GL_UNSIGNED_BYTE, 
			image.GetData());
		break;
	case Pixelbuffer:
		{
		mpRenderTexture->EndCapture();
		mpRenderTexture->Bind();
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, 
			mRenderImage.GetData());

		// Now crop the data from the render image into the target image.
		wxUint8 *src = mRenderImage.GetData();
		wxUint8 *dest = image.GetData();

		size_t rowSize = size * 3;

		wxASSERT(size <= RENDER_TEXTURE_SIZE);

		// Copy each row.
		for(size_t i = 0; i < size; ++i)
		{
			memcpy(dest, src, rowSize);
			src += RENDER_TEXTURE_STRIDE;
			dest += rowSize;
		}
		
		break;
		}
	case Nonebuffer:
	default:
		wxASSERT(false);
		break;
	}
	
	if(NULL != gameObject)
	{
		gameObject->coords(coords);
		gameObject->rotation(rotation);
	}

	return image;
}

void wxSettlersGLCanvas::EndGLtoWX()
{
	switch(mRenderToBitmapMode)
	{
	case Framebuffer:
		// Clean up.
		mpFBO->Unattach(GL_COLOR_ATTACHMENT0_EXT);
		mpFBO->Unattach(GL_RENDERBUFFER_EXT);
		FramebufferObject::Disable();

		glDeleteTextures(1, &sRenderTex);

		glReadBuffer(GL_BACK);
		glDrawBuffer(GL_BACK);

		break;
	case Pixelbuffer:
	case Backbuffer:
		// No need to do anything here.
		break;
	case Nonebuffer:
	default:
		wxASSERT(false);
		break;
	};

	// Reset the viewport.
	ResizeGL();
}

wxImage wxSettlersGLCanvas::ProcessGLtoWX(wxImage image, const size_t width, 
		const size_t height) const
{
	// Because GL reads pixels in starting from the lower-left, we need to flip
	// the image around.
	wxImage final(image.Mirror(false));

	// Rescale the image to the right dimensions.
	size_t dim = std::max(width, height);
	final.Rescale(dim, dim);

	//and finally, crop the image if needed
	if(width != height)
	{
		wxImage temp = wxImage(width, height);
		wxUint8 *source = final.GetData();
		wxUint8 *dest = temp.GetData();

		wxUint8 *byte = NULL;
		wxUint8 *end = NULL;

		//determine if we need to crop horizontally or vertically
		if(width > height)
		{
			wxInt32 start = (width - height) >> 1;

			//crop horizontally
			byte = source + (start * width * 3);
			end = byte + (height * width * 3);
			for(; byte < end; byte += 3, dest += 3)
			{
				dest[0] = byte[0];
				dest[1] = byte[1];
				dest[2] = byte[2];
			}
		}
		else
		{
			//TODO: vertical cropping
		}

		final = temp;
	}
	
	return final;
}

void wxSettlersGLCanvas::OnGameObjects(const AggregatorObjectPtr &object)
{
	wxASSERT(object);

	//iterate through all of the game objects, creating draw objects
	//for them and adding them to our draw list
	AggregatorObject::GameObjectList::const_iterator it;

	for(it = object->objects_begin(); it != object->objects_end(); ++it)
	{
		//avoid looking for empty objects
		if(*it)
		{
			//create a new draw object for this game object
			DrawObjectPtr draw;
			draw = DRAWOBJECT.CreateDrawObject((*it));

			if(draw)
			{
				mPriorityMap.insert(std::make_pair(draw->GetPriority(), draw));
			}
		}
	}

	SetDirty();
}

void wxSettlersGLCanvas::OnSelectionObject(const SelectionObjectPtr &object)
{
	DataObject input(-1), output;
	RULE.Decide(shLogicIsThisPlayer, input, output);

	// Only add the selection object if it is the player at this computer.
	if(true == output.read<bool>())
	{
		// Clear out any current render state.
		Controller::get().Transmit(shEventShowResources, DataObject(false));

		//save it off for use when something is selected
		mSelectionObject = object;

		if(object)
		{
			//add a draw object to show these selection options
			AggregatorObjectPtr aggregate(new AggregatorObject);
			aggregate->add(object);

			//turn on hit tests since we're now selecting something
			SetHitTest(true);

			OnGameObjects(aggregate);
		}
	}
}

void wxSettlersGLCanvas::OnShutdownUI(wxInt32)
{
	if(mSelectionObject)
	{
		mSelectionObject.reset();
		SetHitTest(false);
		SetDirty();
	}
}

void wxSettlersGLCanvas::OnResetCamera(bool)
{
	ResetCamera();
	SetDirty();
}

void wxSettlersGLCanvas::OnImageUpdate(bool)
{
	if(FALSE == GetContext()) 
	{
		return;
	}

	SetCurrent();

	IMAGE.IgnoreCache();
	DRAWOBJECT.ReleaseAll();
	DRAWOBJECT.ObtainAll();
}

void wxSettlersGLCanvas::OnDirty(wxInt32)
{
	SetDirty();
}

void wxSettlersGLCanvas::OnRenderBitmaps(wxInt32)
{
	bool reset = false;
	
	wxInt32 render = CONFIG.read<wxInt32>(swConfigSectionOpenGL, swRenderBitmaps, 
		TRUE);
	wxInt32 framebuffer = CONFIG.read<wxInt32>(swConfigSectionOpenGL, 
		swFramebuffer, FALSE);
	wxInt32 pixelbuffer = CONFIG.read<wxInt32>(swConfigSectionOpenGL, 
		swPixelBuffer, TRUE);

#	ifdef __WXGTK__
	// FIXME: disable pixelbuffer on wxGTK until we have a
	// fix for the crash in RenderTexture::initialize
	// see trac ticket #493
#	warning DISABLED PIXELBUFFER due to a bug in rendertexture
	pixelbuffer = FALSE;
#	endif

	// See if they've turned off bitmap rendering.
	if(FALSE == render)
	{
		if(Nonebuffer != mRenderToBitmapMode)
		{
			wxLogDebug(
				wxT("wxSettlersGLCanvas: Setting render mode to Nonebuffer."));
			mRenderToBitmapMode = Nonebuffer;
			reset = true;

			mpFBO.reset();
			mpRB.reset();
			mpRenderTexture.reset();
			mRenderImage = wxImage();
		}
	}
	// It's possible for these options to be set by the user, but still fail to
	// work.
	else
	{
		bool done = false;

		// Using a frame buffer is our first choice.
		if( (TRUE == framebuffer) &&
			(Framebuffer != mRenderToBitmapMode) &&
			(GLEW_EXT_framebuffer_object))
		{
			wxLogDebug(
				wxT("wxSettlersGLCanvas: Setting render mode to Framebuffer."));
			mRenderToBitmapMode = Framebuffer;
			reset = true;

			mpFBO.reset(new FramebufferObject);
			mpRB.reset(new Renderbuffer);
			mpRenderTexture.reset();
			mRenderImage = wxImage();

			done = true;
		}

		// Pixel buffers are our second choice.
		if( (false == done) &&
			(TRUE == pixelbuffer) &&
#ifdef __WIN32__
			(Pixelbuffer != mRenderToBitmapMode) &&
			(WGLEW_ARB_pbuffer) && 
			(WGLEW_ARB_pixel_format) && 
			(WGLEW_ARB_render_texture) && 
			(GLEW_ARB_depth_texture))
#elif __MACH__
			// FIXME
			(Pixelbuffer != mRenderToBitmapMode))
#else
			(Pixelbuffer != mRenderToBitmapMode) &&
			(GLXEW_SGIX_pbuffer) && 
			(GLXEW_SGIX_fbconfig) && 
			(GLEW_ARB_depth_texture))
#endif
		{
			wxLogDebug(
				wxT("wxSettlersGLCanvas: Setting render mode to Pixelbuffer."));
			mRenderToBitmapMode = Pixelbuffer;
			reset = true;

			mpRenderTexture.reset(new RenderTexture);
            mpRenderTexture->Reset(shRgbTex2D.c_str());
			mpFBO.reset();
			mpRB.reset();

			// Create a texture that will hold the scaled image.
			if(false == mpRenderTexture->Initialize
				(static_cast<wxInt32>(RENDER_TEXTURE_SIZE), 
				 static_cast<wxInt32>(RENDER_TEXTURE_SIZE)))
			{
				wxLogDebug(wxT("Failed to initialize render texture."));
			}

			mpRenderTexture->BeginCapture();
			
			// Set up defaults.
			glShadeModel(GL_SMOOTH);

			//adjust the perspective hwxInt32
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
			glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

			//set the clear color
			glClearColor(0., 0., 0., 0.);

			//set up depth testing
			glClearDepth(1.0f);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);

			//set up lighting
			GLfloat amb[] = {1.0f, 1.0f, 1.0f, 1.0f};
			GLfloat dif[] = {1.0f, 1.0f, 1.0f, 1.0f};
			GLfloat pos[] = {0.0f, 1.0f, 4.0f, 1.0f};
			glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
			glLightfv(GL_LIGHT0, GL_POSITION, pos);

			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
			mpRenderTexture->EndCapture();

			mRenderImage = wxImage(RENDER_TEXTURE_SIZE, RENDER_TEXTURE_SIZE);

			done = true;
		}
		
		// The back buffer is our last choice.
		if( (false == done) &&
			(Backbuffer != mRenderToBitmapMode))
		{
			wxLogDebug(
				wxT("wxSettlersGLCanvas: Setting render mode to Backbuffer."));
			mRenderToBitmapMode = Backbuffer;
			reset = true;

			mpFBO.reset();
			mpRB.reset();
			mpRenderTexture.reset();
			mRenderImage = wxImage();
		}
	}

	if(true == reset)
	{
		Controller::get().Transmit(shEventResetBitmaps, 0);
	}
}
