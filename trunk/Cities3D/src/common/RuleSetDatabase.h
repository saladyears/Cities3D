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
#include "LocalSingleton.h"
#include "RuleSet.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifndef __WIN32__
	#pragma GCC visibility push(hidden)
	class RuleSetDatabase;
	template class LocalSingleton<RuleSetDatabase>;
	#pragma GCC visibility pop
#endif

//---------------------------- CLASSES          -----------------------------//
class DLL_LOCAL RuleSetDatabase : public LocalSingleton<RuleSetDatabase>
{
public:
	const RuleSetMap &map() const {return mRuleSetMap;}

	void RegisterName(const wxString& ruleset, const wxString& name);
	void RegisterRule(const wxString &ruleset, const RulePtr &rule, 
		const HashString &name);
	void RegisterLogic(const wxString &ruleset, const LogicPtr &logic, 
		const HashString &name);
	void RegisterMixin(const wxString &ruleset, const HashString &rule, 
		const HashString &key, const HashString &mixin);
	void RegisterBranch(const wxString &ruleset, const HashString &rule, 
		const wxInt32 priority, const LogicPtr &logic);
	void RegisterGameObject(const wxString &ruleset, const HashString &name, 
		const GameObjectPtr &object);
	void RegisterDrawObject(const wxString &ruleset, 
		const DrawObjectPtr &object);
	void RegisterImageSet(const wxString &ruleset, 
		const ImageSetPtr &imageset);
	void RegisterBankData(const wxString &ruleset, 
		const BankDataPtr &bankdata);
	void RegisterPlayerData(const wxString &ruleset, 
		const PlayerDataPtr &playerdata);
	void RegisterBuildData(const wxString &ruleset, 
		const BuildDataPtr &builddata);
	void RegisterUIPlugin(const wxString& ruleset, const UIPluginPtr& plugin);
	void RegisterExclusions(const wxString& ruleset, const StringPairArray& e);
	void RegisterKeyMap(const wxString& ruleset, const IKeyMapEngine::KeyMap& keymap);
	void RegisterStat(const wxString& ruleset, const StatPtr& stat);

private:
	RuleSetMap mRuleSetMap;

	friend class LocalSingleton<RuleSetDatabase>;
};

//---------------------------- DEFINES          -----------------------------//

#define IMPLEMENT_NAME(Ruleset, Name) \
namespace \
{ \
	struct sAutoRegister##Ruleset \
	{ \
		sAutoRegister##Ruleset(const wxString& name) \
		{ \
			RuleSetDatabase::get().RegisterName(Ruleset, name); \
		} \
	}; \
	sAutoRegister##Ruleset object##Ruleset(Name); \
}

//this macro makes auto-registering rules possible; note that we do not have to 
//worry about memory leaks in doing this because the RuleEngine stores rules as
//boost::shared_ptr objects, which are automatically deleted when they go out 
//of scope
#define IMPLEMENT_RULE(T, Ruleset) \
namespace \
{ \
	struct sAutoRegister##T \
	{ \
		sAutoRegister##T(Rule *rule) {RuleSetDatabase::get().RegisterRule( \
			Ruleset, RulePtr(rule), #T);} \
	}; \
	sAutoRegister##T object##T(new T); \
}

//this macro auto-registers a rule that is not in itself a new rule, but is a
//replacement (override) of an existing rule, which means that when it is 
//copied into the main game rules hash, it will replace the existing rule of 
//the same name; this allows rules that are derived classes of existing rules 
//to replace the base rules in the game, so that the game UI only needs to know
//the name of the base rule, not the derived rule
#define IMPLEMENT_RULE_REPLACE(T, Replaces, Ruleset) \
namespace \
{ \
	struct sAutoRegister##T \
	{ \
	sAutoRegister##T(Rule *rule) {RuleSetDatabase::get().RegisterRule(Ruleset,\
		RulePtr(rule), #Replaces);} \
	}; \
	sAutoRegister##T object##T(new T); \
}

//this macro also adds an entry in the mixin hash for the ruleset, meaning that
//the rule being stored is an extension to an existing rule
#define IMPLEMENT_RULE_MIXIN(T, Extends, Ruleset) \
namespace \
{ \
	struct sAutoRegister##T##Extends \
	{ \
		sAutoRegister##T##Extends(Rule *rule) \
		{ \
			RuleSetDatabase::get().RegisterRule(Ruleset, RulePtr(rule), \
				#T); \
			RuleSetDatabase::get().RegisterMixin(Ruleset, #Extends, \
				#T, #T); \
		} \
	}; \
	sAutoRegister##T##Extends object##T##Extends(new T); \
}

#define IMPLEMENT_RULE_KEYED_MIXIN(T, Key, Extends, Ruleset) \
namespace \
{ \
	struct sAutoRegister##T##Key \
	{ \
		sAutoRegister##T##Key(Rule *rule) \
		{ \
			RuleSetDatabase::get().RegisterRule(Ruleset, RulePtr(rule), \
				#T); \
			RuleSetDatabase::get().RegisterMixin(Ruleset, #Extends, \
				#Key, #T); \
		} \
	}; \
	sAutoRegister##T##Key object##T##Key(new T); \
}

//this macro adds a core piece of logic to a certain ruleset
#define IMPLEMENT_LOGIC(T, Ruleset) \
namespace \
{ \
	struct sAutoRegister##T \
	{ \
		sAutoRegister##T(Logic *logic) \
		{ \
			RuleSetDatabase::get().RegisterLogic(Ruleset, LogicPtr(logic), \
				#T); \
		} \
	}; \
	sAutoRegister##T object##T(new T); \
}

#define IMPLEMENT_LOGIC_REPLACE(T, Ruleset, Replaces) \
namespace \
{ \
	struct sAutoRegister##T \
	{ \
		sAutoRegister##T(Logic *logic) \
		{ \
			RuleSetDatabase::get().RegisterLogic(Ruleset, LogicPtr(logic), \
				#Replaces); \
		} \
	}; \
	sAutoRegister##T object##T(new T); \
}

//this macro adds an entry into the mixin hash for the ruleset, meaning that
//this piece of logic is stored as an extension to an existing piece of logic
#define IMPLEMENT_LOGIC_MIXIN(T, Decides, Ruleset) \
namespace \
{ \
	struct sAutoRegister##T \
	{ \
		sAutoRegister##T(Logic *logic) \
		{ \
			RuleSetDatabase::get().RegisterLogic(Ruleset, LogicPtr(logic), \
				#T); \
			RuleSetDatabase::get().RegisterMixin(Ruleset, #Decides, \
				#T, #T); \
		} \
	}; \
	sAutoRegister##T object##T(new T); \
}

#define IMPLEMENT_LOGIC_KEYED_MIXIN(T, Key, Decides, Ruleset) \
namespace \
{ \
	struct sAutoRegister##T \
	{ \
		sAutoRegister##T(Logic *logic) \
		{ \
			RuleSetDatabase::get().RegisterLogic(Ruleset, LogicPtr(logic), \
				#T); \
			RuleSetDatabase::get().RegisterMixin(Ruleset, #Decides, \
				#Key, #T); \
		} \
	}; \
	sAutoRegister##T object##T(new T); \
}

//this macro creates a default rule branch in the rule chain, priority zero
//(the lowest), which just gives a deafult path to follow through the rule
//chain that can be replaced with a higher priority branch if needed by any
//variants
#define IMPLEMENT_DEFAULT_BRANCH(Ruleset, Rule, Branch) \
namespace \
{ \
	struct sAutoRegister##Rule##Ruleset \
	{ \
		sAutoRegister##Rule##Ruleset(Logic *logic) \
		{ \
			RuleSetDatabase::get().RegisterBranch(Ruleset, #Rule, 0, \
				LogicPtr(logic)); \
		} \
	}; \
	sAutoRegister##Rule##Ruleset object##Rule##Ruleset( \
		new LogicBranchDefault(#Branch)); \
}

//implements a default branch with a given priority
#define IMPLEMENT_PRIORITY_DEFAULT_BRANCH(Ruleset, Rule, Branch, Priority) \
namespace \
{ \
	struct sAutoRegister##Rule \
	{ \
		sAutoRegister##Rule(Logic *logic) \
		{ \
			RuleSetDatabase::get().RegisterBranch(Ruleset, #Rule, \
				Priority, LogicPtr(logic)); \
		} \
	}; \
	sAutoRegister##Rule object##Rule(new LogicBranchDefault(#Branch)); \
}

//implements a branch with a default priority
#define IMPLEMENT_BRANCH(Ruleset, Rule, BranchLogic) \
namespace \
{ \
	struct sAutoRegister##Rule##Ruleset \
	{ \
		sAutoRegister##Rule##Ruleset(Logic *logic) \
		{ \
			RuleSetDatabase::get().RegisterBranch(Ruleset, #Rule, 0, \
				LogicPtr(logic)); \
		} \
	}; \
	sAutoRegister##Rule##Ruleset object##Rule##Ruleset(new BranchLogic); \
}

//implements a branch with the given priority
#define IMPLEMENT_PRIORITY_BRANCH(Ruleset, Rule, BranchLogic, Priority) \
namespace \
{ \
	struct sAutoRegister##Rule##Priority \
	{ \
		sAutoRegister##Rule##Priority(Logic *logic) \
		{ \
			RuleSetDatabase::get().RegisterBranch(Ruleset, #Rule, \
				Priority, LogicPtr(logic)); \
		} \
	}; \
	sAutoRegister##Rule##Priority object##Rule##Priority(new BranchLogic); \
}

//game object macros
#define DECLARE_GAME_OBJECT(T) \
public: \
	virtual IGameObject *Clone() {return new T;} \
    virtual const HashString& name() {return sh##T;}

#define IMPLEMENT_GAME_OBJECT(T, Ruleset) \
namespace \
{ \
	struct sAutoRegisterGameObject##T \
	{ \
	sAutoRegisterGameObject##T(IGameObject *pObject) { \
		RuleSetDatabase::get().RegisterGameObject(Ruleset, #T, \
			GameObjectPtr(pObject));} \
	}; \
	sAutoRegisterGameObject##T objectGameObject##T(new T); \
}

//draw object macros
#define DECLARE_DRAW_OBJECT(T, U) \
public: \
	virtual bool CanBuild(const GameObjectPtr &object) const; \
	virtual DrawObject *Clone(const GameObjectPtr &pObject) const { \
		return new T(pObject); \
	}

#define IMPLEMENT_DRAW_OBJECT(T, U, Ruleset) \
bool T::CanBuild(const GameObjectPtr &object) const \
	{ \
		return (NULL != dynamic_cast<U *>(object.get())); \
	} \
namespace \
{ \
	struct sAutoRegisterDrawObject##T \
	{ \
		sAutoRegisterDrawObject##T(IDrawObject *pObject) \
		{ \
			RuleSetDatabase::get().RegisterDrawObject( \
				Ruleset, DrawObjectPtr(pObject));} \
	}; \
	sAutoRegisterDrawObject##T objectDrawObject##T(new T(GameObjectPtr())); \
}

//these macros make creating and registering image sets an easy process;
//just start off with IMPLEMENT_IMAGE_SET_START, add the image names in
//the order they are stored in the .tex file with IMPLEMENT_IMAGE_SET_NAME
//and then finish it up with IMPLEMENT_IMAGE_SET_END
#define IMPLEMENT_IMAGE_SET_START(Key, Type) \
namespace \
{ \
	struct sAutoRegister##Key##Type \
	{ \
		sAutoRegister##Key##Type(ImageSet *imageset) \
		{ \
			wxInt32 count = 0;

#define IMPLEMENT_IMAGE_SET_NAME(Name) \
			imageset->index(#Name, count++);

#define IMPLEMENT_IMAGE_SET_END(Ruleset, Key, Type, TexFile) \
			RuleSetDatabase::get().RegisterImageSet(Ruleset, \
				ImageSetPtr(imageset)); \
		} \
	}; \
	ImageSet *imageset##Key##Type = new ImageSet(#Key, wxT(#TexFile)); \
	sAutoRegister##Key##Type object##Key##Type(imageset##Key##Type); \
}

//macro for registering bank data
#define IMPLEMENT_BANK_DATA(Ruleset, OrigLocation, Location, DataType, \
	ImageKey, ImageName, PopupText) \
namespace \
{ \
	struct sAutoRegister##DataType \
	{ \
		sAutoRegister##DataType(BankData *bankdata) \
		{ \
			RuleSetDatabase::get().RegisterBankData(Ruleset, \
				BankDataPtr(bankdata)); \
		} \
	}; \
	sAutoRegister##DataType object##DataType(new BankData(#OrigLocation, \
		#Location, #DataType, #ImageKey, #ImageName, PopupText)); \
}

//macro for registering image set dependent player data
#define IMPLEMENT_PLAYER_IMAGE_DATA(Ruleset, type, update, color, threshold, \
	imageType, imageData, popuptext) \
namespace \
{ \
	struct sAutoRegister##imageData##Ruleset \
	{ \
		sAutoRegister##imageData##Ruleset(PlayerData *playerdata) \
		{ \
			RuleSetDatabase::get().RegisterPlayerData(Ruleset, \
				PlayerDataPtr(playerdata)); \
		} \
	}; \
	sAutoRegister##imageData##Ruleset object##imageData##Ruleset( \
		new PlayerData(type, update, color, threshold, #imageType, \
			#imageData, popuptext)); \
}

//macro for registering OpenGL dependent player data
#define IMPLEMENT_PLAYER_OPENGL_DATA(Ruleset, type, update, color, threshold, \
	gameObject, drawObject, image, mask, popuptext) \
namespace \
{ \
	struct sAutoRegister##type##gameObject \
	{ \
		sAutoRegister##type##gameObject(PlayerData *playerdata) \
		{ \
			RuleSetDatabase::get().RegisterPlayerData(Ruleset, \
				PlayerDataPtr(playerdata)); \
		} \
	}; \
	GameObjectPtr gobject##type##gameObject = GameObjectPtr( \
		new gameObject); \
	sAutoRegister##type##gameObject object##type##gameObject( \
		new PlayerData(type, update, color, threshold, \
			gobject##type##gameObject, DrawObjectPtr( \
				new drawObject(gobject##type##gameObject)), \
				#image, #mask, popuptext)); \
}

//macro for registering image set dependent build data
#define IMPLEMENT_BUILD_IMAGE_DATA(Ruleset, row, priority, logic, rule, \
	imageType, imageData, iconName, legend, cost, keymap) \
namespace \
{ \
	struct sAutoRegister##imageData \
	{ \
		sAutoRegister##imageData(BuildData *builddata) \
		{ \
			RuleSetDatabase::get().RegisterBuildData(Ruleset, \
				BuildDataPtr(builddata)); \
		} \
	}; \
	sAutoRegister##imageData object##imageData( \
		new BuildData(row, priority, #logic, #rule, #imageType,\
			#imageData, #iconName, legend, #cost, #keymap)); \
}

//macro for registering OpenGL dependent build data
#define IMPLEMENT_BUILD_OPENGL_DATA(Ruleset, row, priority, logic, rule, \
	gameObject, drawObject, image, mask, iconName, legend, cost, keymap) \
namespace \
{ \
	struct sAutoRegister##type##gameObject##rule \
	{ \
		sAutoRegister##type##gameObject##rule(BuildData *builddata) \
		{ \
			RuleSetDatabase::get().RegisterBuildData(Ruleset, \
				BuildDataPtr(builddata)); \
		} \
	}; \
	GameObjectPtr gobject##type##gameObject##rule = GameObjectPtr( \
		new gameObject); \
	sAutoRegister##type##gameObject##rule object##type##gameObject##rule( \
		new BuildData(row, priority, #logic, #rule, \
			gobject##type##gameObject##rule, DrawObjectPtr( \
			new drawObject(gobject##type##gameObject##rule)), \
				#image, #mask, #iconName, legend, #cost, #keymap)); \
}

#define IMPLEMENT_UI_PLUGIN(ruleset, plugin) \
namespace \
{ \
	struct sAutoRegister##plugin \
	{ \
		sAutoRegister##plugin(IUIPlugin* ui) \
		{ \
			RuleSetDatabase::get().RegisterUIPlugin(ruleset, \
				UIPluginPtr(ui)); \
		} \
	}; \
	sAutoRegister##plugin object##plugin(new plugin); \
}

#define IMPLEMENT_KEYMAP(Ruleset, Description, Name, KeyCode, Flags) \
namespace \
{ \
	struct sAutoRegister##Name \
	{ \
		sAutoRegister##Name() \
		{ \
			HashString event("event"); \
			event += #Name; \
			wxKeyEvent defaultEvent; \
			defaultEvent.m_keyCode = KeyCode; \
			if(Flags & wxMOD_ALT) defaultEvent.m_altDown = true; \
			if(Flags & wxMOD_CONTROL) defaultEvent.m_controlDown = true; \
			if(Flags & wxMOD_SHIFT) defaultEvent.m_shiftDown = true; \
			if(Flags & wxMOD_META) defaultEvent.m_metaDown = true; \
			IKeyMapEngine::KeyMap keymap(Description, wxT(#Name), event, defaultEvent); \
			RuleSetDatabase::get().RegisterKeyMap(Ruleset, keymap); \
		} \
	}; \
	sAutoRegister##Name object##Name; \
}

#define IMPLEMENT_STAT(Ruleset, Display, English, Name, Rule, Func, Priority, Hidden) \
namespace \
{ \
	struct sAutoRegister##Name \
	{ \
		sAutoRegister##Name(Stat *stat) \
		{ \
			RuleSetDatabase::get().RegisterStat(Ruleset, StatPtr(stat)); \
		} \
	}; \
	sAutoRegister##Name object##Name(new Stat(Display, wxT(English), #Name, #Rule, Func, Priority, Hidden)); \
}

#define IMPLEMENT_STAT_MODIFIER(Ruleset, Display, English, Name, Rule, Modifies, Priority) \
namespace \
{ \
	struct sAutoRegister##Name##Rule##Modifies \
	{ \
		sAutoRegister##Name##Rule##Modifies(Stat *stat) \
		{ \
			RuleSetDatabase::get().RegisterStat(Ruleset, StatPtr(stat)); \
		} \
	}; \
    sAutoRegister##Name##Rule##Modifies object##Name##Rule##Modifies( \
		new Stat(Display, wxT(English), HashString(#Name), HashString(#Rule), HashString(#Modifies), Priority)); \
}

#define IMPLEMENT_DICE_ROLL_STAT_MODIFIER(Ruleset, Display, English, Name, Rule, Modifies, Priority, Percent) \
namespace \
{ \
	struct sAutoRegister##Name##Rule##Modifies \
	{ \
		sAutoRegister##Name##Rule##Modifies(Stat *stat) \
		{ \
			RuleSetDatabase::get().RegisterStat(Ruleset, StatPtr(stat)); \
		} \
	}; \
    sAutoRegister##Name##Rule##Modifies object##Name##Rule##Modifies( \
		new Stat(Display, wxT(English), HashString(#Name), HashString(#Rule), HashString(#Modifies), Priority, Percent)); \
}

#define IMPLEMENT_MULTISTAT_MODIFIER_START(Name) \
namespace \
{ \
	struct sAutoRegister##Name \
	{ \
		sAutoRegister##Name(Stat *stat) \
		{ \
            HashStringArray rules;

#define IMPLEMENT_MULTISTAT_MODIFIER_RULE(Rule) \
            rules.push_back(#Rule);

#define IMPLEMENT_MULTISTAT_MODIFIER_END(Name, Display, English, Modifies, Ruleset, Priority) \
            stat->mRules = rules; \
    		RuleSetDatabase::get().RegisterStat(Ruleset, StatPtr(stat)); \
		} \
	}; \
    sAutoRegister##Name object##Name(new Stat(Display, wxT(English), #Name, #Modifies, Priority)); \
}

//---------------------------- PROTOTYPES       -----------------------------//
