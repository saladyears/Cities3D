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
#include <boost/function.hpp>

//---------------------------- USER INCLUDES    -----------------------------//

//---------------------------- DEFINES          -----------------------------//
enum PlayerDataType
{
	PlayerResource			= 0,
	PlayerResourceBack		,
	PlayerCardBack			,
	PlayerStock				,
	PlayerPoints			,
	PlayerSpecial			,
};

//---------------------------- TYPEDEFS         -----------------------------//
typedef boost::function2<wxInt32, GamePtr, wxInt32> PlayerUpdateFunc;
typedef boost::function2<wxColour, GamePtr, wxInt32> PlayerColorFunc;

class IDrawObject;
typedef boost::shared_ptr<IDrawObject> DrawObjectPtr;

class IGameObject;
typedef boost::shared_ptr<IGameObject> GameObjectPtr;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: PlayerData
// 
// Describes a piece of information that needs to be displayed
// in the <PlayerUI>.  A <RuleSet> may add a number of different
// pieces of information to the <Game> that the players will want
// to have visual represntation of.  The struct PlayerData provides
// a way for <RuleSets> to detail to the <PlayerUI> the information
// needed to maintain a UI element inside a <PlayerUI>.  
//
// There are two types of PlayerData.  The first uses a bitmap retrieved 
// from an <ImageSet> as its icon or graphic.  The second uses an image
// rendered by OpenGL as its icon or graphic (usually stock items
// since they are actual game objects).  PlayerData must provide
// an update function that the <PlayerUI> can call when it needs to
// recalculate any counter associated with the PlayerData.  It also can
// optionally provide a color function that returns the text color the 
// counter should be displayed in.  Both the update and the color function
// are boost::function objects.  All PlayerData is created with a certain
// category type that tells the <PlayerUI> where that PlayerData belongs
// in the screen layout.  The types are given by the PlayerDataType enum.
// And finally, a piece of PlayerData may also provide a threshold value.  
// If the current counter returned by the update function is less than 
// or equal to the threshold value, the UI element is hidden.  A threshold 
// of -1 indicates that the UI element will always remain visible, even if 
// its count goes to zero.
//
// *NOTE*: PlayerData should be created by either the 
// <IMPLEMENT_PLAYER_IMAGE_DATA> or the <IMPLEMENT_PLAYER_OPENGL_DATA> macro 
// in <RuleSetDatabase>.
//
// Project:
//     <RulesCore>
//
// Include:
//     PlayerData.h
//
struct PlayerData
{
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: PlayerData
	//
	// The <ImageSet> bitmap PlayerData constructor.  Creates
	// a PlayerData that uses the given <ImageSet> image key 
	// and image name to create its screen bitmap.
	//
	// Parameters:
	//     type - The <PlayerUI> layout category.
	//     update - The counter update function.  A boost::function object.
	//     color - The counter color function.  A boost::function object.
	//     threshold - The visibility threshold.  See <PlayerData> remarks.
	//     imageKey - The <ImageSet> image group key of the bitmap of the 
	//                PlayerData.
	//     imageName - The image name within the group given by the image key.
	//     popuptext - The popup text for the image.
	//
	PlayerData(const PlayerDataType type, const PlayerUpdateFunc &update, 
		const PlayerColorFunc &color, const wxInt32 threshold, 
		const HashString &imageKey, const HashString &imageName,
		const wxString& popuptext)
	: mPlayerDataType(type), mUpdateFunc(update), mColorFunc(color), 
	mThreshold(threshold), mImageKey(imageKey), mImageName(imageName),
	mPopupText(popuptext) {}

	//-----------------------------------------------------------------------//
	// Constructor: PlayerData
	//
	// The OpenGL-rendered bitmap PlayerData constructor.  Creates
	// a PlayerData that uses the given <IGameObject> and <IDrawObject>
	// to create its screen bitmap.
	//
	// Parameters:
	//     type - The <PlayerUI> layout category.
	//     update - The counter update function.  A boost::function object.
	//     color - The counter color function.  A boost::function object.
	//     threshold - The visibility threshold.  See <PlayerData> remarks.
	//     gameObject - The <IGameObject> representing the type of model to 
	//                  draw.
	//     drawObject - The <IDrawObject> that renders the <IGameObject> in 
	//                  OpenGL.
	//     image - The name of the backup bitmap image.
	//     mask - The name of the backup bitmap image mask.
	//     popuptext - The popup text for the image.
	//
	PlayerData(const PlayerDataType type, const PlayerUpdateFunc &update, 
		const PlayerColorFunc &color, const wxInt32 threshold, 
		const GameObjectPtr &gameObject, const DrawObjectPtr &drawObject,
		const HashString &image, const HashString &mask, 
		const wxString& popuptext)
	: mPlayerDataType(type), mUpdateFunc(update), mColorFunc(color), 
	mThreshold(threshold), mGameObject(gameObject), mDrawObject(drawObject),
	mImageKey(image), mImageName(mask), mPopupText(popuptext)  {}

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Object: mPlayerDataType
	//
	// The layout "bucket" the PlayerData falls into.  The
	// <PlayerUI> groups all PlayerData by buckets and then lays
	// them out on screen according to its internal logic.
	//
	PlayerDataType mPlayerDataType;

	//-----------------------------------------------------------------------//
	// Object: mUpdateFunc
	//
	// The counter update function object.  A boost::function object
	// that must be in the form *wxInt32 func(GamePtr, wxInt32)*.  Should
	// return the current count for the piece of PlayerData.
	//
	PlayerUpdateFunc mUpdateFunc;

	//-----------------------------------------------------------------------//
	// Object: mColorFunc
	//
	// The counter color function object.  Optional.  A boost::function
	// object that must be in the form *wxColour func(GamePtr, wxInt32)*.
	// If existant, should return the text color for the current
	// cout of the piece of PlayerData.
	//
	PlayerColorFunc mColorFunc;

	//-----------------------------------------------------------------------//
	// Int: mThreshold
	//
	// The visibility threshold.  PlayerData may have a threshold 
	// beneath which it should not be visible.  For example, if a 
	// player has 0 timber, the timber card in their <PlayerUI> should 
	// be hidden.  By setting mThreshold to 0, this effect is
	// achieved.  A value of -1 means the data is *always* shown.  A
	// positive value (or zero) means the data is shown only when its
	// update function returns a number greater than the threshold.
	//
	wxInt32 mThreshold;

	//-----------------------------------------------------------------------//
	// String: mImageKey
	//
	// The image key group.  If the value is not empty, the PlayerData
	// is assumed to be using the <IImageEngine> and an <ImageSet> to
	// produce its screen bitmap.  If empty, the OpenGL fields are used.
	//
	HashString mImageKey;

	//-----------------------------------------------------------------------//
	// String: mImageName
	//
	// The image name.  Used in conjunction with the image key to 
	// retrieve the screen bitmap image from an <ImageSet> within
	// the <IImageEngine>.
	//
	HashString mImageName;

	//-----------------------------------------------------------------------//
	// String: mPopupText
	//
	// The popup text for the item.
	//
	wxString mPopupText;

	//-----------------------------------------------------------------------//
	// Object: mGameObject
	//
	// The <IGameObject> used to render the PlayerData.
	// An <IDrawObject> must always have an <IGameObject> to
	// refer to when it renders itself in OpenGL.
	//
	GameObjectPtr mGameObject;

	//-----------------------------------------------------------------------//
	// Object: mDrawObject
	//
	// The <IDrawObject> used to render the PlayerData in OpenGL.
	//
	DrawObjectPtr mDrawObject;
};

typedef boost::shared_ptr<PlayerData> PlayerDataPtr;

//---------------------------- PROTOTYPES       -----------------------------//
