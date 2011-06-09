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
#include "ServerGameThread.h"
#include "Cities3DSocket.h"
#include "RuleEvent.h"
#include "NetworkUtility.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#define SEND_DESYNC_CHECKS
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	size_t sCycle = 0;
}

//---------------------------- PUBLIC           -----------------------------//
wxServerGameThread::wxServerGameThread(wxEvtHandler * const handler, 
									   const Player &hostPlayer)
: mHandler(handler)
, mNullPlayer(NullPlayer)
, mHostPlayer(hostPlayer)
, mLock(wxMUTEX_RECURSIVE)
{
	wxASSERT(NULL != handler);

	sCycle = 0;
}

void wxServerGameThread::AddSocket(wxCities3DSocket * const socket)
{
	wxMutexLocker lock(mLock);

	wxASSERT(NULL != socket);

	wxLogDebug(wxT("Server game thread adding new socket."));

	if(mSocketMap.end() != mSocketMap.find(socket))
	{
		wxLogDebug(wxT("Server game thread tried to add the same socket twice."));
	}

	// Insert the socket into the map.
	mSocketMap[socket] = SocketStream();
}

bool wxServerGameThread::HasSockets() const
{
	wxMutexLocker lock(mLock);

	return (0 < mSocketMap.size());
}

void wxServerGameThread::SendRule(const boost::shared_array<wxUint8> &data, 
	const size_t size)
{
	wxMutexLocker lock(mLock);

	// Read the rule out of memory first.
	HashString rule;
	DataObject object;
		
	try
	{
		NetworkUtility::ReadRuleFromMemory(rule, object, data, size);
	}
	catch(const std::exception &e)
	{
		wxLogDebug(HashString(e.what()).wx_str());
	}

	wxLogDebug(wxT("%d - Server game thread received rule %s."), sCycle, 
		rule.cwx_str());

	// See if this rule should be executed.
	DataObject input(rule), output;
	RULE.Decide(shNetworkLogicAllowRule, input, output);

	if(true == output.read<bool>())
	{
		// Execute the rule on the server.
		wxRuleEvent event(rule, object, true);
		mHandler->AddPendingEvent(event);

		// Create our own copy of the data so we don't share it across threads.
		boost::shared_array<wxUint8> thisData(new wxUint8[size]);
		memcpy(thisData.get(), data.get(), size);

		// Add the outgoing data to everyone's buffer.
		WriteSocketBuffer buffer;
		buffer.data = thisData;
		buffer.size = size;
		buffer.sent = 0;

		SocketMap::iterator it, itEnd = mSocketMap.end();
		for(it = mSocketMap.begin(); it != itEnd; ++it)
		{
			it->second.first.push_back(buffer);
		}

#ifdef SEND_DESYNC_CHECKS
		if(shRuleMarkerEndTurn == rule)
		{
			// If we're checking for desyncs, fire off that rule.
			wxRuleEvent event(shNetworkRuleServerDesyncCheck, DataObject(), true);
			mHandler->AddPendingEvent(event);
		}
#endif	// SEND_DESYNC_CHECKS

	}
	else
	{
		wxLogDebug(wxT("%d - Server game thread rejected rule %s."), sCycle, 
			rule.cwx_str());
	}
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void *wxServerGameThread::Entry()
{
	// Run until told not to.
	while(false == TestDestroy())
	{
		++sCycle;

		// This block is necessary to make the critical section exit before
		// we sleep.
		{
			wxMutexLocker lock(mLock);

			// Handle joins if no disconnects have occurred.
			if( (false == HandleDisconnects()) &&
				(false == TestDestroy()))
			{
				// Handle I/O if no joins have occurred.
				if( (false == HandleJoins()) &&
					(false == TestDestroy()))
				{
					HandleIO();
				}
			}
		}

		// Do this every 20 milliseconds.
		Sleep(20);
	}

	return NULL;
}

bool wxServerGameThread::HandleDisconnects()
{
	bool disconnect = false;

	// Look for disconnects.
	SocketMap::iterator it, itEnd = mSocketMap.end();
	for(it = mSocketMap.begin(); it != itEnd; ++it)
	{
		wxCities3DSocket * const socket = it->first;

		// If the socket has disconnected since our last loop, get rid of 
		// it and alert the UI.
		if(false == socket->IsConnected())
		{
			wxLogDebug(wxT("%d - Server game thread found a disconnection."), 
				sCycle);
			mSocketMap.erase(it);

			// If the socket was already complete, and joined, we need to make 
			// note of the person leaving.
			if( (true == socket->IsComplete()) &&
				(true == socket->HasJoined()))
			{
				const Player &player = socket->GetPlayer();

				if(mNullPlayer != player)
				{
					wxRuleEvent event(shNetworkRuleConnectionLost, 
						DataObject(player));
					mHandler->AddPendingEvent(event);

					// Play the sound.
					wxRuleEvent event2(shRulePlaySound, DataObject(SOUND_PLAYER_LEAVE));
					mHandler->AddPendingEvent(event2);
				}

				wxLogDebug(wxT("%d - %s has disconnected."), sCycle,
					player.Name().c_str());
			}

			disconnect = true;

			socket->Destroy();

			// Only handle one disconnect per slice.
			break;
		}
	}

	return disconnect;
}

bool wxServerGameThread::HandleJoins()
{
	bool join = false;

	SocketMap::iterator it, itEnd = mSocketMap.end();
	for(it = mSocketMap.begin(); it != itEnd; ++it)
	{
		wxCities3DSocket * const socket = it->first;

		// The player has joined if they are marked as complete and not marked
		// as joined.  Let everyone know about it.
		if( (true == socket->IsComplete()) &&
			(false == socket->HasJoined())
			)
		{
			wxLogDebug(wxT("%d - Server game thread found a join."), sCycle);

			const Player &player = socket->GetPlayer();
			
			// First, see if this player is already connected, either in a 
			// socket, or as the host.  If they are, dump them.
			bool found = false;
			SocketMap::iterator it2, itEnd2 = mSocketMap.end();
			for(it2 = mSocketMap.begin(); it2 != itEnd2; ++it2)
			{
				if( (it != it2) &&
					(player == it2->first->GetPlayer()))
				{
					found = true;
					break;
				}
			}

			// Check against the host.
			if( (false == found) &&
				(player == mHostPlayer))
			{
				found = true;
			}

			// They've been found.  Alert everyone and disconnect them.
			if(true == found)
			{
                static const wxString stTriesToJoinASecondTime = _("%s tries to join a second time and is refused.");
    
				wxString str = wxString::Format(stTriesToJoinASecondTime.c_str(),
					player.Name().c_str());
				
				wxRuleEvent event(shRuleGenerateSystemMessage, DataObject(str));
				mHandler->AddPendingEvent(event);

				// Let them know they failed.
				wxUint8 fail = 0;
				socket->WriteMsg(&fail, 1);

				// This allows it to get picked up in the next disconnect pass.
				socket->Close();

				wxLogDebug(wxT("%d - %s was rejected."), sCycle,
					player.Name().c_str());
			}
			// Otherwise, they are a valid player and may join.
			else
			{
				socket->Join();
				
				// Let them know they succeeded.
				wxUint8 success = 1;
				socket->WriteMsg(&success, 1);
				
				// Add them to the spectator list and update.
				wxRuleEvent event(shNetworkRuleAddSpectator, DataObject(player));
				mHandler->AddPendingEvent(event);

				// Play the sound.
				wxRuleEvent event2(shRulePlaySound, DataObject(SOUND_PLAYER_JOIN));
				mHandler->AddPendingEvent(event2);
			
                static const wxString stJoinsAsASpectator = _("%s joins as a spectator.");
				wxString str = wxString::Format(stJoinsAsASpectator.c_str(), 
					player.Name().c_str());
				wxRuleEvent event3(shRuleGenerateSystemMessage, DataObject(str));
				mHandler->AddPendingEvent(event3);

				wxLogDebug(wxT("%d - %s has joined."), sCycle,
					player.Name().c_str());
			}

			join = true;

			// Only handle one per slice.
			break;
		}
	}

	return join;
}

void wxServerGameThread::HandleIO()
{
	size_t lastCount = 0;

	SocketMap::iterator it, itEnd = mSocketMap.end();
	for(it = mSocketMap.begin(); it != itEnd; ++it)
	{
		wxCities3DSocket * const socket = it->first;
		
		// The socket must have completed negotiations before any data can be
		// read from or written to it.
		if(true == socket->IsComplete())
		{
			// Ensure that we will not block on i/o.
			socket->SetFlags(wxSOCKET_NOWAIT);

			SocketStream &stream = it->second;
			WriteArray &write = stream.first;
			ReadSocketBuffer &read = stream.second;

			// Write data to the socket first.
			if(false == write.empty())
			{
				WriteSocketBuffer &buffer = write[0];

				switch(buffer.mode)
				{
				case WriteSocketBuffer::SendingType:
					{
						// Attempt to write out the type.
						wxUint8 *data = reinterpret_cast<wxUint8 *>(
							&buffer.type);
						socket->Write(data, 4 - buffer.sent);
						lastCount = socket->LastCount();
						buffer.sent += lastCount;

						// See if we're done sending the type.
						if(4 <= buffer.sent)
						{
							buffer.mode = WriteSocketBuffer::SendingSize;
							buffer.sent = 0;
						}
					}

					// A neat trick to continue sending the data if we sent the
					// size succesfully.
					if(buffer.mode == WriteSocketBuffer::SendingType)
					{
						break;
					}

				case WriteSocketBuffer::SendingSize:
					{
						// Attempt to write out the size.
						wxUint8 *data = reinterpret_cast<wxUint8 *>(
							&buffer.size);
						socket->Write(data, 4 - buffer.sent);
						lastCount = socket->LastCount();
						buffer.sent += lastCount;

						// See if we're done sending the size.
						if(4 <= buffer.sent)
						{
							buffer.mode = WriteSocketBuffer::SendingData;
							buffer.sent = 0;
						}
					}

					// A neat trick to continue sending the data if we sent the
					// size succesfully.
					if(buffer.mode == WriteSocketBuffer::SendingSize)
					{
						break;
					}
				case WriteSocketBuffer::SendingData:
					{
						// Write as much as we can, starting from where we left 
						// off.
						socket->Write(&buffer.data[buffer.sent], buffer.size - 
							buffer.sent);
						lastCount = socket->LastCount();
						buffer.sent += lastCount;

						// See if we're done sending this message.
						if(buffer.size <= buffer.sent)
						{
							wxLogDebug(
								wxT("%d - Server game thread wrote %d bytes to %s."), 
								sCycle, buffer.sent, 
								socket->GetPlayer().Name().c_str());
							write.pop_front();
						}
					}
					break;
				default:
					wxASSERT(false);
				}
			}

			// Read from the socket second.
			switch(read.mode)
			{
			case ReadSocketBuffer::WaitingForData:
				{
					// Attempt to read the size of the incoming rule.
					socket->Read(&read.size, 4);
					lastCount = socket->LastCount();
					if(4 != lastCount)
					{
						// Not enough data to read yet.
						socket->Unread(&read.size, lastCount);
						continue;
					}
					// We got the length, get ready to read in the rule.
					
					//TODO: Deal with endianness.
					read.mode = ReadSocketBuffer::WaitingForRule;
					read.data.reset(new wxUint8[read.size]);
				}
				if(read.mode == ReadSocketBuffer::WaitingForData)
				{
					break;
				}
			case ReadSocketBuffer::WaitingForRule:
				{
					socket->Read(read.data.get(), read.size);
					lastCount = socket->LastCount();

					//Only do something if we get all the data.
					if(read.size != lastCount)
					{
						socket->Unread(read.data.get(), lastCount);
						continue;
					}

					wxLogDebug(
						wxT("%d - Server game thread read %d bytes from %s."), 
						sCycle, read.size, 
						socket->GetPlayer().Name().c_str());

					//TODO: Deal with endianness.
					
					// Send the rule to everyone but this socket.
					SendRule(read.data, read.size);

					// Reset.
					read.mode = ReadSocketBuffer::WaitingForData;
				}
				break;			
			default:
				wxASSERT(false);
				break;
			}
		}
	}
}

void wxServerGameThread::OnExit()
{
	// Shut down all of the listening sockets.
	SocketMap::iterator it, itEnd = mSocketMap.end();
	for(it = mSocketMap.begin(); it != itEnd; ++it)
	{
		it->first->Destroy();
	}
}
