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
#include "RuleEngine.h"
#include "IRule.h"
#include "ILogic.h"
#include "IUndoable.h"
#include "BaseMixin.h"
#include "Controller.h"
#include "GameEngine.h"
#include "RNG.h"
#include "ServerConnectionThread.h"
#include "NetworkPlayers.h"
#include "INetworkEngine.h"
#include "IStatEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
RuleEngine::RuleEngine()
: mRuleDepth(0)
, mUndoDepth(0)
, mWinner(-1)
, mCanUndo(false)
, mInUndo(false)
, mLock(wxMUTEX_RECURSIVE)
{
	Controller::get().AddReceiver(shEventServerConnection,
		&RuleEngine::OnServerConnection, this);
}

RuleEngine::~RuleEngine()
{
	Controller::get().RemoveReceiver(shEventServerConnection,
		&RuleEngine::OnServerConnection, this);
}

void RuleEngine::Stop()
{
	ClearUndoStack();

	mWorkingUndoList.clear();
	mRuleHash.clear();
	mLogicHash.clear();
	mMixinHash.clear();
	mBranchHash.clear();
}

void RuleEngine::Execute(const HashString &rule, const DataObject &object) const
{
	// If this is a top level rule, see if there's a network to take it.
	if(0 == mRuleDepth)
	{
		if(true == NET.IsConnected())
		{
			NET.SendRule(rule, object);
			return;
		}
	}

	AsynchExecute(rule, object);
}

void RuleEngine::AsynchExecute(const HashString &rule, 
	const DataObject &object) const
{
	wxMutexLocker lock(mLock);

	//design by contract
	wxASSERT(0 != mRuleHash.size());

	// Let the stats engine look at the rule.
	STAT.ProcessRule(rule, object);

	//find the rule first
	RuleHash::const_iterator it;
	it = mRuleHash.find(rule);

	//if we don't have this rule, log it to the error stream
	if(mRuleHash.end() == it)
	{
		wxLogError(wxT("Unable to find rule: %s."), rule.cwx_str());
		return;
	}

	//see if this rule even has the possibility of being undone
	IUndoable *pUndoable = dynamic_cast<IUndoable *>(it->second.get());

	//if this is a top-level rule (meaning, that the current rule depth is
	//zero), and the top-level rule is undoable, then add a new undo list to
	//the undo stack, since all rules that happen chained to this one need
	//to be kept track of
	if(0 == mRuleDepth)
	{
		wxLogDebug(wxT("RuleEngine executing rule %s."), 
			rule.cwx_str());

		mCanUndo = false;
		mWorkingUndoList.clear();

		// If this rule has the potention of being undoable, we need to
		// keep track of it in the undo stack.
		if(NULL != pUndoable)
		{
			mCanUndo = true;
		}
	}

	// The rule depth flag keeps track of how deep the rule chain currently
	// is.
	++mRuleDepth;

	//if this rule has the possibility for rule extensions, load them up
	AddMixins(rule, dynamic_cast<BaseMixin *>(it->second.get()));

	//otherwise, execute the rule with the data object
	try
	{
		it->second->Execute(object);
	}
	catch(boost::bad_any_cast &)
	{
		//log the error
		wxLogError(wxT("Bad call to DataObject::read<> in %s."), 
			rule.cwx_str());
	}

	//see if this rule branches into another rule in the rule chain
	Branch(rule, object);

	// See if this rule changes the undo situation.
	if( (true == mCanUndo) &&
		(NULL != pUndoable))
	{
		if(true == pUndoable->CanUndo())
		{
			// Add to the working undo list.
			mWorkingUndoList.push_back(std::make_pair(rule, object));
			wxLogDebug(wxT("RuleEngine adding %s to the undo list."), 
				rule.cwx_str());
		}
		else
		{
			wxLogDebug(wxT("RuleEngine clearing undo list because of %s."), 
				rule.cwx_str());

			// Not only clear the working list, but blow away the current
			// undo stack as well.
			ClearUndoStack();
			mWorkingUndoList.clear();
			mCanUndo = false;
		}
	}

	--mRuleDepth;

    // Let the stats engine handle the rule going out of scope.
	STAT.UnprocessRule(rule);

	//if we're back at rule depth 0, it means that our top-level rule
	//has finished executing; check to see if the can undo flag is set to
	//false, in which case the undo stack needs to be destroyed, because
	//we can't go back
	if(0 == mRuleDepth)
	{
		//If we have a winner, things change.
		if(-1 != mWinner)
		{
			// Clear everything.
			ClearUndoStack();
			mWorkingUndoList.clear();
			Controller::get().Transmit(shEventUpdateUndo, 0);

			// Run the winning rule.
			DataObject object(mWinner);
			mWinner = -1;
			AsynchExecute(shRuleWin, object);
            AsynchExecute(shRulePostWin, object);

            // Fire off a final stats update event.
            Controller::get().Transmit(shEventStats, -1);
		}
		else
		{
			if(false == mWorkingUndoList.empty())
			{
				mUndoStack.push(mWorkingUndoList);
				mWorkingUndoList.clear();
			}

			// Update the undo button.
			Controller::get().Transmit(shEventUpdateUndo, 0);
		}
	}
}

void RuleEngine::Decide(const HashString &logic, const DataObject &input, 
						DataObject &output) const
{
	wxMutexLocker lock(mLock);

	//design by contract
	wxASSERT(0 != input.numItems());
	wxASSERT(0 == output.numItems());
	wxASSERT(0 != mLogicHash.size());

	//find the logic first
	LogicHash::const_iterator it;
	it = mLogicHash.find(logic);

	//if we don't have this logic, log it to the error stream
	if(mLogicHash.end() == it)
	{
		wxLogError(wxT("Unable to find logic: %s."), logic.cwx_str());
		return;
	}

	//if this logic has the possibility for logic extensions, load them up
	AddMixins(logic, dynamic_cast<BaseMixin *>(it->second.get()));

	//execute the logic
	try
	{
		it->second->Decide(input, output);
	}
	catch(boost::bad_any_cast &)
	{
		//log the error
		wxLogError(wxT("Bad call to DataObject::read<> in %s."), 
			logic.cwx_str());
	}

	//design by contract
	wxASSERT(0 != input.numItems());
}

bool RuleEngine::CanUndo() const
{
	// There has to be something to undo.
	return (!mUndoStack.empty());
}

bool RuleEngine::PotentialUndo() const
{
	// We have to be in something that can be undone.
	return (!mWorkingUndoList.empty());
}

void RuleEngine::Undo()
{
	while(false == mUndoStack.empty())
	{
		mInUndo = true;

		++mUndoDepth;

		const UndoList undoList = mUndoStack.top();

		// Remove this undo list from the stack.
		mUndoStack.pop();

		// Go through the undo list, calling each rule/data combination
		// in reverse order that they were executed.
		UndoList::const_iterator it, itEnd = undoList.end();
		for(it = undoList.begin(); it != itEnd; ++it)
		{
			const HashString &rule = it->first;

			//find the rule we are undoing first
			RuleHash::iterator itRule;
			itRule = mRuleHash.find(rule);
			wxASSERT(mRuleHash.end() != itRule);

			wxLogDebug(wxT("RuleEngine undoing rule %s."), 
				rule.cwx_str());

			// Allow the stat engine to deal with the rule.
			STAT.ProcessRule(rule, it->second);

			//unexecute the rule
			itRule->second->Unexecute(it->second);

			STAT.UnprocessRule(rule);
		}

		--mUndoDepth;

		// Allow reentrance.
		if( (0 == mUndoDepth) &&
			(0 == mUndoStack.size()))
		{
			//play the sound
			Execute(shRulePlaySound, DataObject(SOUND_UNDO_ACTION));

			//use the restart logic to figure out what to do
			Execute(shRuleRestartGame, DataObject());

			mInUndo = false;
		}
	}
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void RuleEngine::Branch(const HashString &rule, const DataObject &object) const
{
	//see if this rule exists in the rule chain
	BranchPriorityHash::iterator it(mBranchHash.find(rule));
	if(mBranchHash.end() == it)
	{
		return;
	}

	//sanity
	wxASSERT(false == it->second.empty());

	size_t size = it->second.size();
	wxInt32 priority = 0;

	if(1 < size)
	{
		//test to see if this branch has already been used at this
		//priority level
		DataObject input(rule), output;
		Decide(shLogicMarkBranch, input, output);
		priority = output.read<wxInt32>();

		wxLogDebug(wxT("Branch: Rule - %s, Priority  - %d"), 
			rule.cwx_str(), priority);
	}

	//we've got a branch, now execute it
	BranchPriorityMap::iterator it2, itEnd2(it->second.end());
	for(it2 = it->second.begin(); it2 != itEnd2; ++it2)
	{
		//see if we are allowed to test this branch logic
		if( (0 != priority) &&
			(it2->first >= priority))
		{
			continue;
		}

		DataObject output;
		it2->second->Decide(object, output);

		//if we've got a branch, follow it
		HashString rule = output.read<HashString>();
		if(false == rule.empty())
		{
			//mark that we've used the branch
			if(1 < size)
			{
				Execute(shRuleMarkBranch,
					DataObject(it->first, it2->first));
			}

			wxLogDebug(wxT("Branch: Executing %s."), rule.cwx_str());
			Execute(rule, object);

			return;
		}
	}

	//should never get here
	wxASSERT(false);
}

void RuleEngine::AddMixins(const HashString &name, BaseMixin *pMixin) const
{
	if(NULL != pMixin)
	{
		//find the extension list in the hash
		MixinHash::iterator it2(mMixinHash.find(name));

		//if there are mixins for this rule, copy them to the rule's mixin list
		if( (mMixinHash.end() != it2) &&
			(true == pMixin->mStringHash.empty()))
		{
			StringHash::iterator it3, it3End = it2->second.end();
			for(it3 = it2->second.begin(); it3 != it3End; ++it3)
			{
				pMixin->mStringHash.insert(*it3);
			}
		}
	}
}

void RuleEngine::ClearUndoStack() const
{
	while(false == mUndoStack.empty())
	{
		mUndoStack.pop();
	}
}

void RuleEngine::OnServerConnection(const DataObject &object)
{
	wxMutexLocker lock(mLock);

	// Stuff the RNG into a memory stream for easier passing.
	wxCountingOutputStream count;
	wxDataOutputStream counter(count);

	RAND.save(counter);

	size_t size = count.GetSize();
	boost::scoped_array<wxUint8> data(new wxUint8[size]);

	wxMemoryOutputStream mem(data.get(), size);
	wxDataOutputStream output(mem);

	RAND.save(output);

	// Fire up the connection thread.
	wxServerConnectionThread *thread = new wxServerConnectionThread(
		object.read<wxEvtHandler *>(), object.read<wxCities3DSocket *>(1),
		*(object.read<GamePtr>(2)),
		GAMEVERSION, NET.Players().GetSpectators(), data,
		size);

	if(wxTHREAD_NO_ERROR != thread->Create())
	{
		wxLogError(wxT("Unable to create a connection thread."));
	}
	else
	{
		thread->Run();
	}
}
