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
#include "StatEngine.h"
#include "Controller.h"
#include "Game.h"
#include "GameEngine.h"
#include "DataObject.h"
#include "IPluginEngine.h"
#include "IRuleEngine.h"
#include "Stat.h"
#include "json_spirit_writer.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
StatEngine::StatEngine()
: mRuleDepth(0)
, mStatUpdate(false)
{
    Controller::get().AddReceiver(shEventInitializeUI,
		&StatEngine::OnInitializeUI, this);
}

StatEngine::~StatEngine()
{
    Controller::get().RemoveReceiver(shEventInitializeUI,
		&StatEngine::OnInitializeUI, this);
}

void StatEngine::ProcessRule(const HashString& rule, const DataObject& object)
{
	++mRuleDepth;

	PushModifier(rule);
	UpdateStats(rule, object);
}

void StatEngine::UnprocessRule(const HashString& rule)
{
	--mRuleDepth;

	PopModifier(rule);

	if( (0 == mRuleDepth) &&
		(true == mStatUpdate))
	{
		mStatUpdate = false;
		Controller::get().Transmit(shEventStats, GetGame()->read<wxInt32>(shID));
	}
}

void StatEngine::GetJson(json_spirit::wObject& obj, bool forceEnglish)
{
	using namespace json_spirit;

	const HashString& state = GetGame()->read<HashString>(shState);
	bool gameOver = (shComplete == state);

	// Use the display map to output the stats in the correct display order.
	StatDisplayMap::const_iterator it, itEnd = mStatDisplayMap.end();
	for(it = mStatDisplayMap.begin(); it != itEnd; ++it)
	{
		// The stat name.
		const HashString& stat = it->second.first;
		const ModifierDisplayMap& modifierMap = it->second.second;

		// Check if this stat is allowed to be displayed right now.
		wxASSERT(mStatNameMap.end() != mStatNameMap.find(stat));

		const StatPtr& thisStat = mStatNameMap[stat];

		if( (false == gameOver) &&
			(true == thisStat->mHiddenUntilDone))
		{
			continue;
		}

		obj.push_back(wPair(swStat, 
			forceEnglish ? thisStat->mEnglishDisplay : thisStat->mDisplay));

		switch(thisStat->mType)
		{
		case Stat::DiceRoll:
			WriteDiceRollStat(stat, obj, modifierMap, forceEnglish);
			break;
		case Stat::Player:
			WritePlayerStat(stat, obj, modifierMap, forceEnglish);
			break;
		default:
			wxASSERT(false);
			break;
		}
	}
}

void StatEngine::save(wxDataOutputStream& stream) const
{
	// Write out game stats.
	stream << static_cast<wxUint32>(mDiceRollStatMap.size());

	{
		DiceRollStatMap::const_iterator it, itEnd = mDiceRollStatMap.end();
		for(it = mDiceRollStatMap.begin(); it != itEnd; ++it)
		{
			const HashString& stat = it->first;
			const DiceRollStatModifierHash& hash = it->second;
			
			// Write the name of this stat.
			stream << stat;

			// Write the hash values.
			stream << static_cast<wxUint32>(hash.size());
			DiceRollStatModifierHash::const_iterator itH, itHEnd = hash.end();
			for(itH = hash.begin(); itH != itHEnd; ++itH)
			{
				const HashString& modifier = itH->first;
				wxInt32 val = itH->second;

				stream << modifier;
				stream << val;
			}
		}
	}

	// Write out player stats.
	stream << static_cast<wxUint32>(mPlayerStatMap.size());
	
	{
		PlayerStatMap::const_iterator it, itEnd = mPlayerStatMap.end();
		for(it = mPlayerStatMap.begin(); it != itEnd; ++it)
		{
			const HashString& stat = it->first;
			const PlayerStatModifierHash& hash = it->second;

			// Write the name of this stat.
			stream << stat;

			// Write the hash values.
			stream << static_cast<wxUint32>(hash.size());
			PlayerStatModifierHash::const_iterator itH, itHEnd = hash.end();
			for(itH = hash.begin(); itH != itHEnd; ++itH)
			{
				const HashString& modifier = itH->first;
				const PlayerStatArray& array = itH->second;

				wxUint32 size = array.size();
				
				stream << modifier;
				stream << size;

				for(wxUint32 i = 0; i < size; ++i)
				{
					stream << array[i];
				}
			}
		}
	}
}

void StatEngine::load(wxDataInputStream& stream)
{
	// Read in game stats.
	wxUint32 size;
	stream >> size;

	for(wxUint32 i = 0; i < size; ++i)
	{
		HashString name;
		stream >> name;

		DiceRollStatModifierHash hash;
		wxUint32 modifiers;
		stream >> modifiers;

		for(wxUint32 j = 0; j < modifiers; ++j)
		{
			HashString modifier;
			wxInt32 val;

			stream >> modifier;
			stream >> val;

			hash[modifier] = val;
		}

		mDiceRollStatMap[name] = hash;
	}

	// Read in player stats.
	stream >> size;

	for(wxUint32 i = 0; i < size; ++i)
	{
		HashString name;
		stream >> name;

		PlayerStatModifierHash hash;
		wxUint32 modifiers;
		stream >> modifiers;

		for(wxUint32 j = 0; j < modifiers; ++j)
		{
			HashString modifier;
			wxUint32 players;

			stream >> modifier;
			stream >> players;

			PlayerStatArray array(players, 0);

			for(wxUint32 k = 0; k < players; ++k)
			{
				stream >> array[k];
			}
			
			hash[modifier] = array;
		}

		mPlayerStatMap[name] = hash;
	}
}

void StatEngine::Stop()
{
    mStatRuleMap.clear();
    mStatModifierMap.clear();
	mStatNameMap.clear();
    mModifierMap.clear();
	mStatDisplayMap.clear();
    mDiceRollStatMap.clear();
    mPlayerStatMap.clear();
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void StatEngine::PushModifier(const HashString& rule)
{
	StatRuleMap::iterator it = mStatModifierMap.find(rule);

	// If there are modifiers for this rule, put them on the stack for the rule
	// they modify.
	if(mStatModifierMap.end() != it)
	{
		const StatArray& modifiers = it->second;
		
		StatArray::const_iterator itS, itSEnd = modifiers.end();
		for(itS = modifiers.begin(); itS != itSEnd; ++itS)
		{
			const StatPtr& stat = *itS;

			mModifierMap[stat->mModifies].push_back(stat->mName);
		}
	}
}

void StatEngine::PopModifier(const HashString& rule)
{
	StatRuleMap::iterator it = mStatModifierMap.find(rule);

	// If there are modifiers for this rule, pop them off the stack for the rule
	// they modify.
	if(mStatModifierMap.end() != it)
	{
		const StatArray& modifiers = it->second;
		
		StatArray::const_iterator itS, itSEnd = modifiers.end();
		for(itS = modifiers.begin(); itS != itSEnd; ++itS)
		{
			const StatPtr& stat = *itS;

			mModifierMap[stat->mModifies].pop_back();
		}
	}
}

void StatEngine::UpdateStats(const HashString& rule, const DataObject& object)
{
	StatRuleMap::iterator it = mStatRuleMap.find(rule);

    if(mStatRuleMap.end() != it)
    {
		mStatUpdate = true;

		const StatArray& stats = it->second;

		StatArray::const_iterator itS, itSEnd = stats.end();
		for(itS = stats.begin(); itS != itSEnd; ++itS)
		{
			// We have a stat for this rule, process it.
			const StatPtr& stat = *itS;

			// Get the current modifier.
			const HashStringList& modifierList = mModifierMap[stat->mName];
			HashString modifier;

			if(false == modifierList.empty())
			{
				modifier = modifierList.back();
			}
			
			// The stat functions are allowed to change the modifier category.
			switch(stat->mType)
			{
			case Stat::DiceRoll:
				{
					wxASSERT(stat->mDiceRollFunc);

					stat->mDiceRollFunc(GetGame(), modifier);

					if(false == modifier.empty())
					{
						mDiceRollStatMap[stat->mName][modifier]++;
					}
				}
				break;

			case Stat::Player:
				{
					wxASSERT(stat->mPlayerFunc);

					wxInt32 player = -1;
					wxInt32 adjust = 0;
					stat->mPlayerFunc(object, player, adjust, modifier);

					if(-2 == player)
					{
						player = GetGame()->read<wxInt32>(shCurrentPlayer);
					}

					if(false == modifier.empty())
					{
						wxASSERT(-1 != player);

						// Sanity check.
						if(0 == mPlayerStatMap[stat->mName][modifier].size())
						{
							wxLogError(wxT("Programmer Error: No stat entry for %s."),
								modifier.cwx_str());
							return;
						}

						mPlayerStatMap[stat->mName][modifier][player] += adjust;
					}
				}
				break;

			default:
				wxASSERT(false);
				break;
			}
		}
    }
}

void StatEngine::OnInitializeUI(const GamePtr& game)
{
    wxASSERT(game);

    typedef std::vector<StatPtr> StatArray;
    StatArray stats;

    PLUGIN.LoadStats(stats);

	StatArray mainStats;

    // Put the stat definitions into a lookup map.
    StatArray::const_iterator it, itEnd = stats.end();
    for(it = stats.begin(); it != itEnd; ++it)
    {
        const StatPtr& stat = *it;

		mStatNameMap[stat->mName] = stat;

		if(Stat::Modifier == stat->mType)
		{
			mModifierMap[stat->mModifies].push_back(stat->mName);

			wxLogDebug(wxT("Modifier %s (%u) modifies %s."), stat->mName.cwx_str(), stat->mName.hash(),
				stat->mModifies.cwx_str());
		}

		HashStringArray::const_iterator itR, itREnd = stat->mRules.end();
        for(itR = stat->mRules.begin(); itR != itREnd; ++itR)
        {
			const HashString& rule = *itR;
			if(Stat::Modifier == stat->mType)
			{
				mStatModifierMap[rule].push_back(stat);
			}
			else
			{
				mStatRuleMap[rule].push_back(stat);
				mainStats.push_back(stat);
			}
        }
    }

    wxInt32 numPlayers = game->numPlayers();

    // Now that we have all the stats loaded, create the layouts we'll need
    // to manage them.
	itEnd = mainStats.end();
    for(it = mainStats.begin(); it != itEnd; ++it)
    {
        const StatPtr& stat = *it;
		const HashString& name = stat->mName;

		ModifierDisplayMap modifierDisplayMap;

		// Add all of the modifier categories for the stat.
		ModifierMap::iterator it = mModifierMap.find(stat->mName);
		if(mModifierMap.end() == it)
		{
			continue;
		}

		wxLogDebug(wxT("Creating modifier display map for %s"),
			stat->mName.cwx_str());
		
		HashStringList& modifierList = it->second;
		HashStringList::const_iterator itL, itLEnd = modifierList.end();
		for(itL = modifierList.begin(); itL != itLEnd; ++itL)
		{
			const HashString& modifier = *itL;

			// It's possible that the modifiers might already exist if the stats
			// we're loaded from disk or the network, so, if the modifiers exist,
			// don't reset the stats.
			switch(stat->mType)
			{
			case Stat::DiceRoll:
				if(mDiceRollStatMap[name].end() == mDiceRollStatMap[name].find(modifier))
				{
					mDiceRollStatMap[name][modifier] = 0;
				}
				break;

			case Stat::Player:
				if(mPlayerStatMap[name].end() == mPlayerStatMap[name].find(modifier))
				{
					mPlayerStatMap[name][modifier].resize(numPlayers, 0);
				}
				break;

			default:
				wxASSERT(false);
				break;
			}

			// Look up the modifier so we can set its display order.
			StatNameMap::const_iterator itM = mStatNameMap.find(modifier);
			wxASSERT(mStatNameMap.end() != itM);

			// Check for clashes.
			const StatPtr& modifierStat = itM->second;
			wxInt32 priority = modifierStat->mPriority;

			const HashString& check = modifierDisplayMap[priority];
			if( (false == check.empty()) &&
				(check != modifierStat->mName))
			{
				wxLogError(wxT("Programmer Error: Stat modifier %s has the same priority as %s"),
					modifierStat->mName.cwx_str(), 
					modifierDisplayMap[priority].cwx_str());
			}

			modifierDisplayMap[priority] = modifierStat->mName;
		}

		// Add the stat to the layout map.
		wxInt32 priority = stat->mPriority;
		
		// Check for clashes.
		if(mStatDisplayMap.end() != mStatDisplayMap.find(priority))
		{
			wxLogError(wxT("Programmer Error: Stat %s has the same priority as %s"),
				stat->mName.cwx_str(), 
				mStatDisplayMap[priority].first.cwx_str());
		}

		mStatDisplayMap[priority] = StatPair(stat->mName, modifierDisplayMap);
    }

	// Clear the modifier map for use during play.
	mModifierMap.clear();
}

void StatEngine::WriteDiceRollStat(const HashString& stat, json_spirit::wObject& obj, 
	const ModifierDisplayMap& displayMap, bool forceEnglish)
{
	using namespace json_spirit;

	wxASSERT(mDiceRollStatMap.end() != mDiceRollStatMap.find(stat));
	DiceRollStatModifierHash& hash = mDiceRollStatMap[stat];

	wArray statBlock;

	// Keep track of values in an array of arrays.
	typedef std::vector<wArray> Array;
	Array arr;
	arr.resize(4);

    static const wxString stAmount = _("Amount");
    static const wxString stExpected = _("Expected");
    static const wxString stPercent = _("Percent");
    static const wxString stRolls = _("Rolls");
    
	// Set the row headers
	arr[0].push_back(forceEnglish ? swRolls : stRolls);
	arr[1].push_back(forceEnglish ? swAmount : stAmount);
	arr[2].push_back(forceEnglish ? swPercent : stPercent);
	arr[3].push_back(forceEnglish ? swExpected : stExpected);

	wxInt32 total = 0;
	IntArray counts;

	ModifierDisplayMap::const_iterator itM, itMEnd = displayMap.end();
	for(itM = displayMap.begin(); itM != itMEnd; ++itM)
	{
		const HashString& modifier = itM->second;

		// Find this entry in the actual stat hash.
		wxASSERT(hash.end() != hash.find(modifier));

		wxInt32 amount = hash[modifier];

		counts.push_back(amount);
		total += amount;

		wxASSERT(mStatNameMap.end() != mStatNameMap.find(modifier));

		const StatPtr& modifierStat = mStatNameMap[modifier];

		arr[0].push_back(forceEnglish ? modifierStat->mEnglishDisplay : 
			modifierStat->mDisplay);
		arr[1].push_back(wxString::Format(swIntFormat.c_str(), amount));
		arr[3].push_back(wxString::Format(swFloatPercentFormat.c_str(), 
            modifierStat->mExpected * 100.0f));
	}

	// Set percents.
	for(wxUint32 i = 0; i < counts.size(); ++i)
	{
		float percent = 0.0f;
		if(0 < total)
		{
			percent = static_cast<float>(counts[i]) / static_cast<float>(total);
		}

		arr[2].push_back(wxString::Format(swFloatPercentFormat.c_str(), 
            percent * 100.0f));
	}

	for(wxUint32 i = 0; i < 4; ++i)
	{
		statBlock.push_back(arr[i]);
	}

	// Add the block.
	obj.push_back(wPair(swBlock, statBlock));
}

void StatEngine::WritePlayerStat(const HashString& stat, json_spirit::wObject& obj, 
	const ModifierDisplayMap& displayMap, bool forceEnglish)
{
	using namespace json_spirit;

    static const wxString stTotal = _("Total");
    
	wxInt32 numPlayers = GetGame()->numPlayers();

	// Look up the stat block.
	wxASSERT(mPlayerStatMap.end() != mPlayerStatMap.find(stat));
	PlayerStatModifierHash& hash = mPlayerStatMap[stat];

	wArray statBlock;

	// Keep track of everything in an array of arrays.
	typedef std::vector<wArray> Array;
	Array arr;
	arr.resize(numPlayers + 1);

	IntArray totals(numPlayers, 0);

	// Iterate through display order version of the stats.
	ModifierDisplayMap::const_iterator itM, itMEnd = displayMap.end();
	for(itM = displayMap.begin(); itM != itMEnd; ++itM)
	{
		const HashString& modifier = itM->second;
		
		// Find this entry in the actual stat hash.
		wxASSERT(hash.end() != hash.find(modifier));

		const PlayerStatArray& stats = hash[modifier];

		wxASSERT(mStatNameMap.end() != mStatNameMap.find(modifier));

		const StatPtr& modifierStat = mStatNameMap[modifier];

		// Stick the modifiers into the first array.
		arr[0].push_back(forceEnglish ? modifierStat->mEnglishDisplay : 
			modifierStat->mDisplay);

		wxUint32 size = stats.size();
		for(wxUint32 i = 0; i < size; ++i)
		{
			arr[i + 1].push_back(wxString::Format(swIntFormat.c_str(), stats[i]));
			totals[i] += stats[i];
		}
	}

	// Create the final line arrays.
	Array lines;
	lines.resize(numPlayers + 1);

	// Player column.
	for(wxInt32 i = 0; i < numPlayers + 1; ++i)
	{
        static const wxString stPlayer = _("Player");
    
		wxString name = forceEnglish ? swPlayer : stPlayer;

		if(0 < i)
		{
			name = GetGame()->player(i - 1).Name();
		}
		
		lines[i].push_back(name);
	}

	// Final block.
	for(wxInt32 i = 0; i < numPlayers + 1; ++i)
	{
		wxString str = forceEnglish ? swTotal : stTotal;

		if(0 < i)
		{
			str = wxString::Format(swIntFormat.c_str(), totals[i-1]);
		}
		
		lines[i].push_back(str);

		// Copy the lines into the block.
		std::copy(arr[i].begin(), arr[i].end(), std::back_inserter(lines[i]));
		statBlock.push_back(lines[i]);
	}

	// Add the block.
	obj.push_back(wPair(swBlock, statBlock));
}
