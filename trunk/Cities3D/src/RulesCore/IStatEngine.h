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
#include "RulesExport.h"
#include "ISerialize.h"
#include "json_spirit_value.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//
class DataObject;

//---------------------------- CLASSES          -----------------------------//

//---------------------------------------------------------------------------//
// Class: IStatEngine
//
// TODO: Fill this in.
//
// Project:
//     <RulesCore>
//
// Include:
//     IStatEngine.h
//
class RULES_EXPORT_SPEC IStatEngine : public ISerialize
{
public:
	virtual ~IStatEngine()=0;

	virtual void ProcessRule(const HashString& rule, const DataObject& object)=0;

    virtual void UnprocessRule(const HashString& rule)=0;

	virtual void GetJson(json_spirit::wObject& obj, bool forceEnglish = false)=0;
};

//---------------------------- PROTOTYPES       -----------------------------//
inline IStatEngine::~IStatEngine() {}
