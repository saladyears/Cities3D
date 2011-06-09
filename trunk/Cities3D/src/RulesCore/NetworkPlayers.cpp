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
#include "NetworkPlayers.h"
#include "Player.h"
#include "Controller.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- PUBLIC           -----------------------------//
NetworkPlayers::NetworkPlayers()
: mLock(wxMUTEX_RECURSIVE)
{
}

void NetworkPlayers::Reset()
{
	wxMutexLocker lock(mLock);

	mSpectators.clear();
	mThisPlayer = NullPlayer;
	mHostPlayer = NullPlayer;
}

const SpectatorArray &NetworkPlayers::GetSpectators() const
{
	wxMutexLocker lock(mLock);

	return mSpectators;
}

void NetworkPlayers::SetSpectators(const SpectatorArray &spectators)
{
	wxMutexLocker lock(mLock);

	mSpectators = spectators;
	Controller::get().Transmit(shEventSpectator, 0);
}

void NetworkPlayers::SetThisPlayer(const Player &player)
{
	wxMutexLocker lock(mLock);

	mThisPlayer = player;
}

void NetworkPlayers::SetHostPlayer(const Player &player)
{
	wxMutexLocker lock(mLock);

	mHostPlayer = player;
}

const Player &NetworkPlayers::GetThisPlayer()
{
	wxMutexLocker lock(mLock);

	return mThisPlayer;
}

const Player &NetworkPlayers::GetHostPlayer()
{
	wxMutexLocker lock(mLock);

	return mHostPlayer;
}

bool NetworkPlayers::IsHost()
{
	wxMutexLocker lock(mLock);

	return	(mHostPlayer == mThisPlayer) &&
			(mHostPlayer != NullPlayer);
}

bool NetworkPlayers::IsSpectator(const Player& player)
{
	wxMutexLocker lock(mLock);
	Player thisPlayer = player;

	if(thisPlayer == NullPlayer)
	{
		thisPlayer = mThisPlayer;
	}

	bool spectator = false;
	SpectatorArray::const_iterator it, itEnd = mSpectators.end();
	for(it = mSpectators.begin(); it != itEnd; ++it)
	{
		if(thisPlayer == it->first)
		{
			spectator = true;
			break;
		}
	}
	
	return spectator;
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
