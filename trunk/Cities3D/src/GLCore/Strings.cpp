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

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
    // Force all strings to be statically initialized first.
#ifdef __WIN32__
    #pragma warning(disable:4073)
    #pragma init_seg(lib)
    #pragma warning(default:4073)
#endif

    // All strings used in this project.

	//-----------------------------------------------------------------------//
	// Section: HashStrings
	//
	const HashString shA = "a";
    const HashString shAux = "aux";
    const HashString shB = "b";
    const HashString shBitFormat = "%d%s,%d%s,%d%s,%d%s";
    const HashString shCommaSpaceNewline = ", \n";
    const HashString shCtt = "ctt";
    const HashString shDepth = "depth";
    const HashString shDepthSpace = "depth ";
    const HashString shDepthTex = "depthTex";
    const HashString shDepthTexCUBE = "depthTexCUBE";
    const HashString shDepthTexRECT = "depthTexRECT";
    const HashString shDouble = "double";
    const HashString shDoubleBuffer = "doublebuffer";
	const HashString shEmpty = "";
    const HashString shEquals = "=";
    const HashString shF = "f";
    const HashString shG = "g";
    const HashString shMipmap = "mipmap";
    const HashString shMipmapSpace = "mipmap ";
    const HashString shR = "r";
    const HashString shRg = "rg";
    const HashString shRgb = "rgb";
    const HashString shRgba = "rgba";
    const HashString shRgbTex2D = "rgb tex2D";
    const HashString shRtt = "rtt";
    const HashString shSpace = " ";
    const HashString shSamples = "samples";
    const HashString shStencil = "stencil";
    const HashString shStencilSpace = "stencil ";
    const HashString shTex = "tex";
    const HashString shTex2DSpace = "tex2D ";
    const HashString shTexCUBE = "texCUBE";
    const HashString shTexRECT = "texRECT";
    const HashString shTexRECTSpace = "texRECT ";
    const HashString shTileObject = "TileObject";

	// Data
    const HashString shInitialDepth = "InitialDepth";
    const HashString shInitialRotation = "InitialRotation";

	// Display

	// Events
	const HashString shEventAnimation = "eventAnimation";
    const HashString shEventDirty = "eventDirty";
    const HashString shEventGameObjects = "eventGameObjects";
    const HashString shEventImageUpdate = "eventImageUpdate";
    const HashString shEventMapPanDown = "eventMapPanDown";
    const HashString shEventMapPanLeft = "eventMapPanLeft";
    const HashString shEventMapPanRight = "eventMapPanRight";
    const HashString shEventMapPanUp = "eventMapPanUp";
    const HashString shEventMapRotateDown = "eventMapRotateDown";
    const HashString shEventMapRotateLeft = "eventMapRotateLeft";
    const HashString shEventMapRotateRight = "eventMapRotateRight";
    const HashString shEventMapRotateUp = "eventMapRotateUp";
    const HashString shEventMapZoomIn = "eventMapZoomIn";
    const HashString shEventMapZoomOut = "eventMapZoomOut";
    const HashString shEventRenderBitmaps = "eventRenderBitmaps";
    const HashString shEventResetBitmaps = "eventResetBitmaps";
    const HashString shEventResetCamera = "eventResetCamera";
    const HashString shEventSelectionObject = "eventSelectionObject";
    const HashString shEventShowResources = "eventShowResources";
    const HashString shEventShutdownUI = "eventShutdownUI";
    
	// Logic
	const HashString shLogicIsThisPlayer = "LogicIsThisPlayer";
    
	// Rules
    const HashString shRuleAutoSave = "RuleAutoSave";
    
	//-----------------------------------------------------------------------//
	// Section: wxStrings
	//
    const wxString swBackBuffer = wxT("Back buffer");
    const wxString swBaseGLCanvas = wxT("wxBaseGLCanvas");
    const wxString swConfigGLCameraPosX = wxT("ConfigGLCameraPos/x");
    const wxString swConfigGLCameraPosY = wxT("ConfigGLCameraPos/y");
    const wxString swConfigGLCameraPosZ = wxT("ConfigGLCameraPos/z");
    const wxString swConfigGLLongestColor = wxT("ConfigGLLongestColor");
    const wxString swConfigGLLookPosX = wxT("ConfigGLLookPos/x");
    const wxString swConfigGLLookPosY = wxT("ConfigGLLookPos/y");
    const wxString swConfigGLLookPosZ = wxT("ConfigGLLookPos/z");
    const wxString swConfigGLRotationX = wxT("ConfigGLRotation/x");
    const wxString swConfigGLRotationY = wxT("ConfigGLRotation/y");
    const wxString swConfigGLRotationZ = wxT("ConfigGLRotation/z");
    const wxString swConfigGLSectionColor = wxT("ConfigGLSelectionColor");
    const wxString swConfigGLWhiteColor = wxT("ConfigGLWhiteColor");
    const wxString swConfigSectionOpenGL = wxT("ConfigSectionOpenGL");
    const wxString swCoreGLCanvas = wxT("wxCoreGLCanvas");
    const wxString swDebugInfo = wxT("DebugInfo");
    const wxString swFramebuffer = wxT("Framebuffer");
    const wxString swFrameworkGLCanvas = wxT("wxFrameworkGLCanvas");
    const wxString swOff = wxT("Off");
    const wxString swRenderBitmaps = wxT("RenderBitmaps");
    const wxString swPixelBuffer = wxT("Pixelbuffer");
    const wxString swSettlersGLCanvas = wxT("wxSettlersGLCanvas");

	// Formats
    const wxString swDriverString = wxT("Driver: %s");
    const wxString swFloat6 = wxT("%.6f");
    const wxString swFPS = wxT("FPS: %.2f");
    const wxString swGLBitmaps = wxT("GL Bitmaps: %s");
    const wxString swIntFormat = wxT("%d");
    const wxString swRenderString = wxT("Render: %.2f Swap: %.2f");
    const wxString swSlashStringSlashString = wxT("/%s/%s");

	//-----------------------------------------------------------------------//
	// Section: Translations
	//

	// Formats        	
}

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
