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
#include "Animation.h"
#include "TimerManager.h"
#include "Controller.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	class AnimationScheduler
	{
	public:
		void AddAnimation(const Animation* animation)
		{
			mAnimationSet.insert(animation);
			
			// If this is the first one, start up the event trigger.
			if(1 == mAnimationSet.size())
			{
				wxLogDebug(wxT("AnimationScheduler: Starting animations."));

				// Animate at 30 FPS.
				TIME.AddPeriodicFunction(&AnimationScheduler::OnTimer, this, 
					33);
			}
		}

		void RemoveAnimation(const Animation* animation)
		{
			AnimationSet::iterator it = mAnimationSet.find(animation);
			wxASSERT(mAnimationSet.end() != it);

			if(mAnimationSet.end() != it)
			{
				mAnimationSet.erase(it);
			}

			// If we're back to zero, stop ticking.
			if(true == mAnimationSet.empty())
			{
				wxLogDebug(wxT("AnimationScheduler: Stopping animations."));

				TIME.RemovePeriodicFunction(&AnimationScheduler::OnTimer, this, 
					33);
			}
		}

	private:
		void OnTimer()
		{
			Controller::get().Transmit(shEventDirty, 0);
		}

		typedef std::set<const Animation*> AnimationSet;
		AnimationSet mAnimationSet;
	};

	AnimationScheduler sAnimationScheduler;
}

//---------------------------- PUBLIC           -----------------------------//
void Animation::Animate()
{
	// See if we are in the animation.
	long time = mStopWatch.Time();
	
	if(mStart <= time)
	{
		float percent = static_cast<float>(time - mStart) / 
			static_cast<float>(mDuration);
		// Clamp.
		percent = std::min(1.0f, percent);
		AnimatePrivate(percent);
	}
}

const bool Animation::IsDone() const
{
	return (mStart + mDuration < mStopWatch.Time());
}

//---------------------------- PROTECTED        -----------------------------//
Animation::Animation(const long duration, const long start)
: mDuration(duration)
, mStart(start)
{
	// Start up the stopwatch.
	mStopWatch.Start();

	// Add ourselves to the refresh pool.
	sAnimationScheduler.AddAnimation(this);
}

Animation::~Animation()
{
	sAnimationScheduler.RemoveAnimation(this);
}

//---------------------------- PRIVATE          -----------------------------//
