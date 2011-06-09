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
#include "RuleSet.h"
//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
RuleSet::RuleSet(const wxString &name)
: mName(name)
{
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void RuleSet::RegisterRule(const RulePtr &rule, const HashString &name)
{
	wxASSERT(rule);
	wxASSERT(false == name.empty());

	mRuleHash[name] = rule;
}

void RuleSet::RegisterLogic(const LogicPtr &logic, const HashString &name)
{
	wxASSERT(logic);
	wxASSERT(false == name.empty());

	mLogicHash[name] = logic;
}

void RuleSet::RegisterMixin(const HashString &rule, const HashString &key, 
							const HashString &mixin)
{
	wxASSERT(false == rule.empty());
	wxASSERT(false == key.empty());
	wxASSERT(false == mixin.empty());

	mMixinHash[rule][key] = mixin;
}

void RuleSet::RegisterBranch(const HashString &rule, const wxInt32 priority, 
							 const LogicPtr &logic)
{
	wxASSERT(false == rule.empty());
	wxASSERT(logic);

	mBranchHash[rule].push_back(std::make_pair(priority, logic));
}

void RuleSet::RegisterGameObject(const HashString &name, 
								 const GameObjectPtr &object)
{
	wxASSERT(false == name.empty());
	wxASSERT(object);

	mGameObjectHash[name] = object;
}

void RuleSet::RegisterDrawObject(const DrawObjectPtr &object)
{
	wxASSERT(object);

	//by using a set, we ensure only one of each object
	mDrawObjectSet.insert(object);
}

void RuleSet::RegisterImageSet(const ImageSetPtr &imageset)
{
	wxASSERT(imageset);

	mImageSetList.push_back(imageset);
}

void RuleSet::RegisterBankData(const BankDataPtr &bankdata)
{
	wxASSERT(bankdata);

	mBankDataList.push_back(bankdata);
}

void RuleSet::RegisterPlayerData(const PlayerDataPtr &playerdata)
{
	wxASSERT(playerdata);

	mPlayerDataList.push_back(playerdata);
}

void RuleSet::RegisterBuildData(const BuildDataPtr &builddata)
{
	wxASSERT(builddata);

	mBuildDataList.push_back(builddata);
}

void RuleSet::SetUIPlugin(const UIPluginPtr& plugin)
{
	wxASSERT(plugin);
	wxASSERT(!mUIPlugin);

	mUIPlugin = plugin;
}

void RuleSet::SetOptionUI(const OptionUIPtr& option)
{
	wxASSERT(option);
	wxASSERT(!mOptionUI);

	mOptionUI = option;
}

