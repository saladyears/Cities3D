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

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Struct: BankData
// 
// Describes a stack of cards to be drawn in the <BankUI>.  If
// a <RuleSet> adds additional resource cards or another type of cards
// to the game, it is important that the user have a visual representation
// of how many of those cards are left in the bank.  The struct
// BankData provides a way for <RuleSets> to detail to the <BankUI>
// not only what image to use to draw the card stack, but also where in
// the <Game> data the current number of cards in that stack are
// located.  
//
// *NOTE*: BankData should be created via the <IMPLEMENT_BANK_DATA> macro 
// in <RuleSetDatabase>.
//
// Project:
//     <RulesCore>
//
// Include:
//     BankData.h
//
struct BankData
{
	//-----------------------------------------------------------------------//
	// Group: Constructors
	//

	//-----------------------------------------------------------------------//
	// Constructor: BankData
	//
	// The BankData constructor.  Stores the given locations
	// of data and image information.
	//
	// Parameters:
	//     orig - The location or the original values of the bank data.
	//     loc - The location of the bank data in the <Game> IntHash.
	//     type - The type of bank data within the location.
	//     key - The <ImageSet> key of the image to draw the card stack.
	//     name - The image name of the image to draw the card stack.
	//     text - The popup text for the image.
	//
	BankData(const HashString &orig, const HashString &loc, 
		const HashString &type, const HashString &key, const HashString &name, 
		const wxString& text)
		: mOrigLocation(orig), mDataLocation(loc), mDataType(type), 
		mImageKey(key), mImageName(name), mPopupText(text) 
	{}

	//-----------------------------------------------------------------------//
	// Group: Variables
	//

	//-----------------------------------------------------------------------//
	// Object: mOrigLocation
	//
	// The location in the <Game> IntHash map where the original card amount
	// data is stored.
	// 
	HashString mOrigLocation;

	//-----------------------------------------------------------------------//
	// Object: mDataLocation
	//
	// The location in the <Game> IntHash map where the card amount
	// data is stored.
	// 
	HashString mDataLocation;

	//-----------------------------------------------------------------------//
	// Object: mDataType
	//
	// The type of data within the data location.  It must be an existing
	// hash entry in the IntHash specified by <mDataLocation>, or an error
	// message will be thrown to the screen by the <Data> class.
	//
	HashString mDataType;

	//-----------------------------------------------------------------------//
	// Object: mImageKey
	//
	// The <ImageSet> key for the group of images that the card
	// image resides in.
	//
	HashString mImageKey;

	//-----------------------------------------------------------------------//
	// Object: mImageName
	//
	// The name of the image within the <ImageSet> group key.
	//
	HashString mImageName;

	//-----------------------------------------------------------------------//
	// Object: mPopupText
	//
	// The popup text for the bank data.
	//
	wxString mPopupText;
};

typedef boost::shared_ptr<BankData> BankDataPtr;

//---------------------------- PROTOTYPES       -----------------------------//
