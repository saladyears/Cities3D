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

//---------------------------- USER INCLUDES    -----------------------------//

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class IDrawObject;
typedef boost::shared_ptr<IDrawObject> DrawObjectPtr;

class IGameObject;
typedef boost::shared_ptr<IGameObject> GameObjectPtr;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: BuildData
// 
// Project:
//     <RulesCore>
//
// Include:
//     BuildData.h
//
struct BuildData
{
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: BuildData
	//
	// The <ImageSet> bitmap PlayerData constructor.  Creates
	// a PlayerData that uses the given <ImageSet> image key 
	// and image name to create its screen bitmap.
	//
	// Parameters:
	//     row - The index of the <BuildUI> row the BuildData should be in on 
	//           screen.
	//     priority - The priority of the BuildData within its row.
	//     logic - The name of the <ILogic> that determines if the <BuildUI> 
	//             can be shown.
	//     rule - The name of the <IRule> to execute when the <BuildUI> is 
	//            clicked.
	//     imageKey - The <ImageSet> image group key of the bitmap of the 
	//                PlayerData.
	//     imageName - The image name within the group given by the image key.
	//     iconName  - The image name of any icon that can be overlaid.
	//     legend - The text describing what the item is.
	//     cost - The logic that determines the cost of the item.
	//	   keymap - The key event that triggers this item.
	//
	BuildData(wxInt32 row, wxInt32 priority, const HashString &logic, 
		const HashString &rule, const HashString &imageKey, 
		const HashString &imageName, const HashString& iconName,
		const wxString& legend, const HashString& cost, const HashString& keymap)
	: mRow(row), mPriority(priority), mLogic(logic), mRule(rule), 
		mImageKey(imageKey), mImageName(imageName), mIconName(iconName),
		mLegend(legend), mLogicCost(cost), mKeyEvent(keymap) {}

	//-----------------------------------------------------------------------//
	// Constructor: BuildData
	//
	// The OpenGL-rendered bitmap BuildData constructor.  Creates
	// a BuildData that uses the given <IGameObject> and <IDrawObject>
	// to create its screen bitmap.
	//
	// Parameters:
	//     row - The index of the <BuildUI> row the BuildData should be in on 
	//           screen.
	//     priority - The priority of the BuildData within its row.
	//     logic - The name of the <ILogic> that determines if the <BuildUI> 
	//             can be shown.
	//     rule - The name of the <IRule> to execute when the <BuildUI> is 
	//            clicked.
	//     gameObject - The <IGameObject> representing the type of model to 
	//                  draw.
	//     drawObject - The <IDrawObject> that renders the <IGameObject> in 
	//                  OpenGL.
	//     image - The name of the backup bitmap image.
	//     mask - The name of the backup bitmap image mask.
	//     iconName  - The image name of any icon that can be overlaid.
	//     legend - The text describing what the item is.
	//     cost - The logic that determines the cost of the item.
	//	   keymap - The key event that triggers this item.
	//
	BuildData(wxInt32 row, wxInt32 priority, const HashString &logic, 
		const HashString &rule, const GameObjectPtr &gameObject, 
		const DrawObjectPtr &drawObject, const HashString &image, 
		const HashString &mask, const HashString& iconName,
		const wxString& legend, const HashString& cost, const HashString& keymap)
	: mRow(row), mPriority(priority), mLogic(logic), mRule(rule), 
		mGameObject(gameObject), mDrawObject(drawObject), mImageKey(image),
		mImageName(mask), mIconName(iconName), mLegend(legend), 
		mLogicCost(cost), mKeyEvent(keymap) {}

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Int: mRow
	//
	// The row index on screen the BuildData should be in.
	//
	wxInt32 mRow;

	//-----------------------------------------------------------------------//
	// Int: mPriority
	//
	// The priority of the BuildData within its row.
	//
	wxInt32 mPriority;

	//-----------------------------------------------------------------------//
	// String: mLogic
	//
	// The name of the <ILogic> that determines the visibility of
	// the <BuildUI>.
	//
	HashString mLogic;

	//-----------------------------------------------------------------------//
	// String: mRule
	//
	// The name of the <IRule> that is executed when the <BuildUI>
	// is clicked.
	//
	HashString mRule;

	//-----------------------------------------------------------------------//
	// String: mImageKey
	//
	// The image key group.  If the value is not empty, the BuildData
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
	// String: mIconName
	//
	// The name of the icon image that can be overlaid the build image.
	//
	HashString mIconName;

	//-----------------------------------------------------------------------//
	// String: mLegend
	//
	// The text describing what this option is.
	//
	wxString mLegend;

	//-----------------------------------------------------------------------//
	// String: mCostLogic
	//
	// The logic determining the cost of the option.  May be empty.
	//
	HashString mLogicCost;

	//-----------------------------------------------------------------------//
	// String: mKeyEvent
	//
	// The key event that triggers this build ui.
	//
	HashString mKeyEvent;

	//-----------------------------------------------------------------------//
	// Object: mGameObject
	//
	// The <IGameObject> used to render the BuildData.
	// An <IDrawObject> must always have an <IGameObject> to
	// refer to when it renders itself in OpenGL.
	//
	GameObjectPtr mGameObject;

	//-----------------------------------------------------------------------//
	// Object: mDrawObject
	//
	// The <IDrawObject> used to render the BuildData in OpenGL.
	//
	DrawObjectPtr mDrawObject;
};

//---------------------------- PROTOTYPES       -----------------------------//
