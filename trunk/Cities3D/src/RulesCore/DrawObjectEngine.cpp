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
#include "DrawObjectEngine.h"
#include "IDrawObject.h"
#include "IGameObject.h"
#include "Controller.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
void DrawObjectEngine::Stop()
{
	mObjectList.clear();
}

DrawObjectPtr DrawObjectEngine::CreateDrawObject(
	const GameObjectPtr &object) const
{
	//look through the object list for something that can handle this
	//game object
	DrawObjectPtrList::const_iterator it;

	for(it = mObjectList.begin(); it != mObjectList.end(); ++it)
	{
		if(true == (*it)->CanBuild(object))
		{
			wxLogDebug(wxT("Cloned draw object: [%s], for game object: %s"),
				HashString(typeid(**it).name()).cwx_str(), object->name().cwx_str());
			return DrawObjectPtr((*it)->Clone(object));
		}
	}

	//could not find it
	wxLogError(wxT("Could not find a draw object for %s."), object->name().cwx_str());
	return DrawObjectPtr();
}

void DrawObjectEngine::ObtainAll()
{
	std::for_each(mObjectList.begin(), mObjectList.end(), 
		boost::bind(&IDrawObject::Obtain, _1));
}

void DrawObjectEngine::ReleaseAll()
{
	std::for_each(mObjectList.begin(), mObjectList.end(), 
		boost::bind(&IDrawObject::Release, _1));
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//


