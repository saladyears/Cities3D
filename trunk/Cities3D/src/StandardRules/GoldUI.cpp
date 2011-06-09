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
#include "UIPlugin.h"
#include "RuleSetDatabase.h"
#include "Controller.h"
#include "GameEngine.h"
#include "DataObject.h"
#include "GoldDialog.h"
#include "IRuleEngine.h"
#include "SettlersGLCanvas.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
class GoldUI : public UIPlugin
{
public:
	GoldUI()
	{
		Controller::get().AddReceiver(shEventGold, &GoldUI::OnGold, this);
	}

	~GoldUI()
	{
		Controller::get().RemoveReceiver(shEventGold, &GoldUI::OnGold, 
			this);
	
	}

private:
	void OnGold(const DataObject& object)
	{
		wxInt32 player = object.read<wxInt32>();

		DataObject input(player), output;
		RULE.Decide(shLogicIsThisPlayer, input, output);

		if(true == output.read<bool>())
		{
			const GamePtr& game = object.read<GamePtr>(1);
			wxGoldDialog* dialog = new wxGoldDialog(GetGLCanvas(), player, 
				game);
			dialog->Show();
		}
	}
};

IMPLEMENT_UI_PLUGIN(GOLD, GoldUI)

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
