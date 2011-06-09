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
#include "TradeLogic.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
void TradeLogic::SetParameters(wxInteractiveImageStrip* left, 
	wxInteractiveImageStrip* right, wxInteractiveImageStrip* resourcePool, 
	wxBaseButton* button)
{
	wxASSERT(NULL != resourcePool);
	mResourcePool = resourcePool;

	wxASSERT(NULL != left);
	mLeftSide = left;

	wxASSERT(NULL != right);
	mRightSide = right;

	wxASSERT(NULL != button);
	mButton = button;

	// Now that we have all the strips, get things set.
	Initialize();
}

//---------------------------- PROTECTED        -----------------------------//
TradeLogic::TradeLogic(boost::weak_ptr<Game> game)
: mGame(game)
{
	
}

//---------------------------- PRIVATE          -----------------------------//
