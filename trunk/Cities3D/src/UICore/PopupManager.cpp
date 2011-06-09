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
#include "PopupManager.h"
#include "TimerManager.h"
#include "IPopup.h"
#include "OwnerDrawFrame.h"
#include "SkinManager.h"
#include "Controller.h"
#include "ConfigDatabase.h"
#include "DataObject.h"
#include "IRuleEngine.h"
#include "GameEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	wxColour sOutline_PopupManager;

	wxInt32 sBorderWidth;
	wxInt32 sCurveWidth;

	class wxPopupWindow;

	const IPopup* sCurrentPopup = NULL;
	wxPopupWindow* sCurrentWindow = NULL;
	
	enum DestroyType
	{
		NotDestroyed		= 0,
		DestroyByTimer		,
		DestroyByPopup		,
	};

	DestroyType sDestroyType = NotDestroyed;

	size_t temp = 0;

	class wxPopupWindow : public wxOwnerDrawFrame
	{
	public:
		wxPopupWindow(const IPopup* popup, const wxPoint& pos)
			: wxOwnerDrawFrame(popup->GetThis(), swEmpty, wxDefaultPosition, 
			wxDefaultSize,
			wxFRAME_FLOAT_ON_PARENT | wxFRAME_NO_TASKBAR | wxFRAME_SHAPED)
			, mPopup(popup)
			, mInterval(0)
			, mTailDirection(static_cast<TailDirection>(temp++ % 10))
		{
			sCurrentWindow = this;

			wxASSERT(NULL != mPopup);

			sOutline_PopupManager = SKIN.Element(shPopupMenuOutline);

			sBorderWidth = SKIN.Size(shPopupMenuBorder);
			sCurveWidth = SKIN.Size(shPopupMenuCurve);

			SetBackgroundColour(SKIN.Element(shBackground));

			CreateLayout(pos);

			// Add the display timer.
			TIME.AddOneShotFunction(&wxPopupWindow::OnDestroy, this, mInterval);
		}

		~wxPopupWindow()
		{
			// If we were not destroyed by the timer or popup, we were destroyed
			// by the parent window being destroyed, so set the pointer correctly.
			if(NotDestroyed == sDestroyType)
			{
				sCurrentWindow = NULL;
			}

			sDestroyType = NotDestroyed;

			TIME.RemoveOneShotFunction(&wxPopupWindow::OnDestroy, this, 
				mInterval);

			Controller::get().Transmit(shEventDirty, 0);
		}

	private:
		void OnDestroy()
		{
			sDestroyType = DestroyByTimer;
			Destroy();
			sCurrentWindow = NULL;
		}

		virtual void Paint(wxMemoryDC &dc)
		{
			wxSize size = GetSize();

			wxColour background = GetBackgroundColour();

			dc.SetBrush(wxBrush(sOutline_PopupManager, wxSOLID));
			dc.SetPen(wxPen(sOutline_PopupManager, 1, wxSOLID));

			// Adjust the outline starting point based on the tail.
			wxInt32 x = 0, y = 0;
			wxInt32 w = size.x - 2;
			wxInt32 h = size.y - 2;

			switch(mTailDirection)
			{
			case TopLeft:
			case TopMiddle:
			case TopRight:
				y += TAIL_LENGTH;
				h -= TAIL_LENGTH;
				break;
			case BottomLeft:
			case BottomMiddle:
			case BottomRight:
				h -= TAIL_LENGTH;
				break;
			case LeftTop:
			case LeftBottom:
				x += TAIL_LENGTH;
				w -= TAIL_LENGTH;
				break;
			case RightTop:
			case RightBottom:
				w -= TAIL_LENGTH;
				break;
			default:
				wxASSERT(false);
				break;
			}

			// Main outline.
			dc.DrawRoundedRectangle(x, y, w, h, sCurveWidth);

			wxInt32 inner = sBorderWidth * 2;

			// Inner outline.
			dc.SetBrush(wxBrush(background, wxSOLID));
			dc.DrawRoundedRectangle(x + sBorderWidth, y + sBorderWidth, 
				w - inner, h - inner, sCurveWidth);

			// Draw the tail.
			wxPoint start, straight, diag;
			switch(mTailDirection)
			{
			case TopLeft:
				start = wxPoint(sCurveWidth + 1, 0);
				straight = wxPoint(sCurveWidth + 1, TAIL_LENGTH);
				diag = wxPoint(sCurveWidth + TAIL_WIDTH + 1, TAIL_LENGTH);
				break;
			case TopMiddle:
				start = wxPoint(w / 2, 0);
				straight = wxPoint((w / 2) - (TAIL_WIDTH / 2), TAIL_LENGTH);
				diag = wxPoint((w / 2) + (TAIL_WIDTH / 2), TAIL_LENGTH);
				break;
			case TopRight:
				start = wxPoint(w - sCurveWidth - 1, 0);
				straight = wxPoint(w - sCurveWidth - 1, TAIL_LENGTH);
				diag = wxPoint(w - sCurveWidth - TAIL_WIDTH - 1, TAIL_LENGTH);
				break;
			case BottomLeft:
				start = wxPoint(sCurveWidth + 1, h + TAIL_LENGTH - 1);
				straight = wxPoint(sCurveWidth + 1, h - 1);
				diag = wxPoint(sCurveWidth + TAIL_WIDTH + 1, h - 1);
				break;
			case BottomMiddle:
				start = wxPoint(w / 2, h + TAIL_LENGTH - 1);
				straight = wxPoint((w / 2) - (TAIL_WIDTH / 2), h - 1);
				diag = wxPoint((w / 2) + (TAIL_WIDTH / 2), h - 1);
				break;
			case BottomRight:
				start = wxPoint(w - sCurveWidth - 1, h + TAIL_LENGTH - 1);
				straight = wxPoint(w - sCurveWidth - 1, h - 1);
				diag = wxPoint(w - sCurveWidth - TAIL_WIDTH - 1, h - 1);
				break;
			case LeftTop:
				start = wxPoint(0, sCurveWidth + 1);
				straight = wxPoint(TAIL_LENGTH, sCurveWidth + 1);
				diag = wxPoint(TAIL_LENGTH, sCurveWidth + 1 + TAIL_WIDTH);
				break;
			case LeftBottom:
				start = wxPoint(0, h - sCurveWidth - 1);
				straight = wxPoint(TAIL_LENGTH, h - sCurveWidth - 1);
				diag = wxPoint(TAIL_LENGTH, h - sCurveWidth - 1 - TAIL_WIDTH);
				break;
			case RightTop:
				start = wxPoint(w + TAIL_LENGTH - 1, sCurveWidth + 1);
				straight = wxPoint(w - 1, sCurveWidth + 1);
				diag = wxPoint(w - 1, sCurveWidth + 1 + TAIL_WIDTH);
				break;
			case RightBottom:
				start = wxPoint(w + TAIL_LENGTH - 1, h - sCurveWidth - 1);
				straight = wxPoint(w - 1, h - sCurveWidth - 1);
				diag = wxPoint(w - 1, h - sCurveWidth - 1 - TAIL_WIDTH);
				break;
			default:
				wxASSERT(false);
				break;
			}

			dc.SetPen(wxPen(background, 1, wxSOLID));
			dc.DrawLine(straight.x, straight.y, diag.x, diag.y);
			
			dc.SetPen(wxPen(sOutline_PopupManager, 1, wxSOLID));

			dc.DrawLine(start.x, start.y, straight.x, straight.y);
			dc.DrawLine(start.x, start.y, diag.x, diag.y);
		}

		virtual void Size(wxSizeEvent &event)
		{
			wxOwnerDrawFrame::Size(event);

			wxSize size = GetSize();
			wxRect rect(GetRect());

			wxInt32 curveSize = (sCurveWidth * 2) - 1;

			// Adjust rect.
			switch(mTailDirection)
			{
			case TopLeft:
			case TopMiddle:
			case TopRight:
				rect.y += TAIL_LENGTH;
				rect.height -= TAIL_LENGTH;
				break;
			case BottomLeft:
			case BottomMiddle:
			case BottomRight:
				rect.height -= TAIL_LENGTH;
				break;
			case LeftTop:
			case LeftBottom:
				rect.x += TAIL_LENGTH;
				rect.width -= TAIL_LENGTH;
				break;
			case RightTop:
			case RightBottom:
				rect.width -= TAIL_LENGTH;
				break;
			default:
				wxASSERT(false);
				break;
			}

			// Use a bitmap to create the region of the rounded rectangle.
			wxMemoryDC mDc;
			wxBitmap roundRect(rect.x + rect.width, rect.y + rect.height, 1);
			mDc.SelectObject(roundRect);

			mDc.SetBackground(*wxWHITE_BRUSH);
			mDc.SetBrush(*wxBLACK_BRUSH);
			mDc.Clear();

			mDc.DrawRoundedRectangle(
				rect.x, rect.y, rect.width, rect.height, curveSize);
			
			mShapeRegion = wxRegion(roundRect, *wxWHITE);
			mShapeRegion.Offset(-GetRect().x, -GetRect().y);

			// Tail.
			wxPoint points[3];
			switch(mTailDirection)
			{
			case TopLeft:
				points[0].x = sCurveWidth + 2;
				points[0].y = 0;
				points[1].x = sCurveWidth + 2;
				points[1].y = TAIL_LENGTH + 1;
				points[2].x = sCurveWidth + TAIL_WIDTH + 3;
				points[2].y = TAIL_LENGTH + 1;
				break;
			case TopMiddle:
				points[0].x = (rect.width / 2);
				points[0].y = 0;
				points[1].x = (rect.width / 2) - 1 - (TAIL_WIDTH / 2);
				points[1].y = TAIL_LENGTH + 1;
				points[2].x = (rect.width / 2) + (TAIL_WIDTH / 2);
				points[2].y = TAIL_LENGTH + 1;
				break;
			case TopRight:
				points[0].x = rect.width - sCurveWidth - 1;
				points[0].y = 0;
				points[1].x = rect.width - sCurveWidth - TAIL_WIDTH - 2;
				points[1].y = TAIL_LENGTH + 1;
				points[2].x = rect.width - sCurveWidth - 1;
				points[2].y = TAIL_LENGTH + 1;
				break;
			case BottomLeft:
				points[0].x = sCurveWidth + 2;
				points[0].y = size.y - 1;
				points[1].x = sCurveWidth + 2;
				points[1].y = size.y - TAIL_LENGTH - 1;
				points[2].x = sCurveWidth + TAIL_WIDTH + 2;
				points[2].y = size.y - TAIL_LENGTH - 1;
				break;
			case BottomMiddle:
				points[0].x = (rect.width / 2);
				points[0].y = size.y - 1;
				points[1].x = (rect.width / 2) - (TAIL_WIDTH / 2);
				points[1].y = size.y - TAIL_LENGTH - 1;
				points[2].x = (rect.width / 2) + (TAIL_WIDTH / 2);
				points[2].y = size.y - TAIL_LENGTH - 1;
				break;
			case BottomRight:
				points[0].x = rect.width - sCurveWidth - 1;
				points[0].y = size.y - 1;
				points[1].x = rect.width - sCurveWidth - TAIL_WIDTH - 1;
				points[1].y = size.y - TAIL_LENGTH - 1;
				points[2].x = rect.width - sCurveWidth - 1;
				points[2].y = size.y - TAIL_LENGTH - 1;
				break;
			case LeftTop:
				points[0].x = 0; 
				points[0].y = sCurveWidth + 2;
				points[1].x = TAIL_LENGTH + 1; 
				points[1].y = sCurveWidth + 2;
				points[2].x = TAIL_LENGTH + 1;
				points[2].y = sCurveWidth + 2 + TAIL_WIDTH;
				break;
			case LeftBottom:
				points[0].x = 0; 
				points[0].y = rect.height - sCurveWidth - 1;
				points[1].x = TAIL_LENGTH + 1; 
				points[1].y = rect.height - sCurveWidth - 1;
				points[2].x = TAIL_LENGTH + 1;
				points[2].y = rect.height - sCurveWidth - 2 - TAIL_WIDTH;
				break;
			case RightTop:
				points[0].x = size.x; 
				points[0].y = sCurveWidth + 1;
				points[1].x = size.x - TAIL_LENGTH - 1; 
				points[1].y = sCurveWidth + 1;
				points[2].x = size.x - TAIL_LENGTH - 1;
				points[2].y = sCurveWidth + 1 + TAIL_WIDTH;
				break;
			case RightBottom:
				points[0].x = size.x; 
				points[0].y = rect.height - sCurveWidth - 1;
				points[1].x = size.x - TAIL_LENGTH - 1; 
				points[1].y = rect.height - sCurveWidth - 1;
				points[2].x = size.x - TAIL_LENGTH - 1;
				points[2].y = rect.height - sCurveWidth - 2 - TAIL_WIDTH;
				break;
			default:
				wxASSERT(false);
				break;
			}

			wxRegion hRgn2 = wxRegion(3, &points[0], wxODDEVEN_RULE);
			mShapeRegion.Union(hRgn2);
			mShapeRegion.Offset(-1, -1);

			// for wxGTK we set the shape in WindowCreate because
			// the window isn't yet created here
			#ifndef __WXGTK__
				SetShape(mShapeRegion);
			#endif
		}

		#ifdef __WXGTK__
		virtual void WindowCreate(wxWindowCreateEvent &event)
		{
			SetShape(mShapeRegion);
		}
		#endif

		void CreateLayout(const wxPoint& pos)
		{
			CalculateTailDirection(pos);

			// Create the layout.
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			wxSizer* layout = new wxBoxSizer(wxVERTICAL);
			
			wxSize size(0, 0);

			switch(mTailDirection)
			{
			case TopLeft:
			case TopMiddle:
			case TopRight:
				layout->AddSpacer(TAIL_LENGTH);
				size.y -= TAIL_LENGTH;
				break;
			case LeftTop:
			case LeftBottom:
				sizer->AddSpacer(TAIL_LENGTH);
				break;
			default:
				break;
			}

			wxString delay = CONFIG.read<wxString>(swConfigSectionUI, swPopupTimeout, sw3);
			delay.ToLong(&mInterval);
			mInterval *= 1000;

			mPopup->CreatePopup(this, layout);

			size += layout->GetMinSize();

			wxInt32 curveOffset = ((sCurveWidth + 1) / 2);
			curveOffset += sBorderWidth;

			switch(mTailDirection)
			{
			case BottomLeft:
			case BottomMiddle:
			case BottomRight:
				layout->AddSpacer(TAIL_LENGTH);
				break;
			default:
				break;
			}

			sizer->Add(layout, 1, wxEXPAND | wxLEFT | wxRIGHT, curveOffset);

			switch(mTailDirection)
			{
			case RightTop:
			case RightBottom:
				sizer->AddSpacer(TAIL_LENGTH);
				break;
			default:
				break;
			}

			SetSizer(sizer);

			size += wxSize(curveOffset * 2, 0);

			// Adjust the size for the tail.
			switch(mTailDirection)
			{
			case TopLeft:
			case TopMiddle:
			case TopRight:
			case BottomRight:
			case BottomMiddle:
			case BottomLeft:
				size += wxSize(0, TAIL_LENGTH);
				break;	
			case RightTop:
			case RightBottom:
			case LeftBottom:
			case LeftTop:
				size += wxSize(TAIL_LENGTH, 0);
				break;
			default:
				wxASSERT(false);
				break;
			}

			SetSize(size);

			AdjustPosition(pos);
		}

		void AdjustPosition(const wxPoint& pos)
		{
			wxPoint adjust;
			wxSize size = GetSize();

			switch(mTailDirection)
			{
			case TopLeft:
				adjust = wxPoint(-sCurveWidth, 0);
				break;
			case TopMiddle:
				adjust = wxPoint(-size.x / 2, 0);
				break;
			case TopRight:
				adjust = wxPoint(sCurveWidth - size.x, 0);
				break;
			case BottomLeft:
				adjust = wxPoint(-sCurveWidth, -size.y);
				break;
			case BottomMiddle:
				adjust = wxPoint(-size.x / 2, -size.y);
				break;
			case BottomRight:
				adjust = wxPoint(sCurveWidth - size.x, -size.y);
				break;
			case LeftTop:
				adjust = wxPoint(0, -sCurveWidth);
				break;
			case LeftBottom:
				adjust = wxPoint(0, sCurveWidth - size.y);
				break;
			case RightTop:
				adjust = wxPoint(-size.x, -sCurveWidth);
				break;
			case RightBottom:
				adjust = wxPoint(-size.x, sCurveWidth - size.y);
				break;
			default:
				wxASSERT(false);
				break;
			}

			Move(pos + adjust);
		};

		void CalculateTailDirection(const wxPoint& pos)
		{
			wxWindow* window = wxGetTopLevelParent(mPopup->GetThis());
			wxASSERT(NULL != window);
			
			wxSize size = window->GetSize();

			wxPoint client = window->ScreenToClient(pos);
			wxPoint center = wxPoint(size.x / 2, size.y / 2);

			float degrees = 360.0f / TailSize;

			// Woot, local class.
			class Vector2D
			{
			public:
				Vector2D() : mX(0.0f), mY(0.0f) {}

				Vector2D(wxInt32 x, wxInt32 y) : mX(x), mY(y) 
				{
					Normalize();
				}

				Vector2D(float x, float y) : mX(x), mY(y)
				{
				}

				// Rotate around the z axis.
				void Rotate(float theta)
				{
					static const float D2R = 0.017451587f;		//(PI/180)
					theta *= D2R;
					
					float s = sinf(theta);
					float c = cosf(theta);

					float x = mX;
					float y = mY;

					mX = (c * x) - (s * y);
					mY = (s * x) + (c * y);

					Normalize();
				}

				bool Compare(const Vector2D& lhs, const Vector2D& rhs)
				{
					// See if we're closer to both of them then they are to
					// each other.
					Vector2D dif(lhs.mX - rhs.mX, lhs.mY - rhs.mY);
					Vector2D test1(mX - lhs.mX, mY - lhs.mY);
					Vector2D test2(mX - rhs.mX, mY - rhs.mY);

					float len = dif.Length();
					float len1 = test1.Length();
					float len2 = test2.Length();

					bool ret = false;

					if( (len >= len1) &&
						(len >= len2))
					{
						ret = true;
					}

					return ret;
				}

				float mX;
				float mY;

			private:
				float Length()
				{
					return sqrtf(mX * mX + mY * mY);
				}

				void Normalize()
				{
					float len = Length();
					if(0.0 < len)
					{
						float oneOverLen = 1.0f / len;
						mX *= oneOverLen;
						mY *= oneOverLen;
					}
				}
			};

			// OK, here's how this works.  Use 2D vectors to determine which of
			// the 10 possible quadrants the point falls in.
			
			// Create the vector from the center to the popup point.
			Vector2D test(client.x - center.x, client.y - center.y);

			Vector2D start(-1, 0);
			Vector2D bottom, top;

			// Start off with the TopLeft quadrant and work our way around.
			for(wxInt32 i = TopLeft; i < TailSize; ++i)
			{
				// Create the test vectors.
				bottom = start;
				bottom.Rotate((i + 1) * degrees);

				top = start;
				top.Rotate((i + 2) * degrees);

				if(true == test.Compare(bottom, top))
				{
					mTailDirection = static_cast<TailDirection>(i);
					break;
				}
			}
		}

		enum TailDirection
		{
			// Top side.
			TopLeft				= 0,
			TopMiddle			,
			TopRight			,

			// Right side.
			RightTop			,
			RightBottom			,

			// Bottom side.
			BottomRight			,
			BottomMiddle		,
			BottomLeft			,
			
			// Left side.
			LeftBottom			,
			LeftTop				,

			TailSize			,
		};

		// Adjust this number to change the tail size.
		static const wxInt32 TAIL_LENGTH = 12;
		static const wxInt32 TAIL_WIDTH = 12;

		const IPopup* mPopup;
		long mInterval;
		TailDirection mTailDirection;
		wxRegion mShapeRegion;
	};

	const wxInt32 POLL_RESOLUTION = 200;
}

//---------------------------- PUBLIC           -----------------------------//
void PopupManager::AddPopup(const IPopup* popup, wxInt32 priority)
{
	PopupPriorityMap::iterator it = mPopupMap.find(priority);
	
	// First one of its priority.
	if(mPopupMap.end() == it)
	{
		PopupMap temp;
		temp[popup] = 0;

		mPopupMap.insert(std::make_pair(priority, temp));
	}
	else
	{
		PopupMap& map = it->second;
		wxASSERT(map.end() == map.find(popup));

		map[popup] = 0;
	}
	
	// If this is the first thing we added, start polling.
	if(1 == mPopupMap.size())
	{
		TIME.AddPeriodicFunction(&PopupManager::Poll, this, POLL_RESOLUTION);
	}
}

void PopupManager::RemovePopup(const IPopup* popup)
{
	bool found = false;

	PopupPriorityMap::iterator it, itEnd = mPopupMap.end();
	for(it = mPopupMap.begin(); it != itEnd; ++it)
	{
		PopupMap& map = it->second;
		PopupMap::iterator itMap, itMapEnd = map.end();
		for(itMap = map.begin(); itMap != itMapEnd; ++itMap)
		{
			const IPopup* thisPopup = itMap->first;
			if(popup == thisPopup)
			{
				found = true;
				map.erase(itMap);
				break;
			}
		}

		if(true == map.empty())
		{
			mPopupMap.erase(it);
		}

		if(true == found)
		{
			break;
		}
	}

	// If that was the last one, stop polling.
	if(true == mPopupMap.empty())
	{
		TIME.RemovePeriodicFunction(&PopupManager::Poll, this, 
			POLL_RESOLUTION);
	}
}

void PopupManager::AddWindow(wxWindow* window, wxInt32 priority)
{
	mWindowsToAvoid.insert(std::make_pair(priority, window));
}

void PopupManager::RemoveWindow(wxWindow* window)
{
	WindowMap::iterator it, itEnd = mWindowsToAvoid.end();

	for(it = mWindowsToAvoid.begin(); it != itEnd; ++it)
	{
		if(window == it->second)
		{
			mWindowsToAvoid.erase(it);
			break;
		}
	}
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
PopupManager::PopupManager()
: mMainFrame(NULL)
{
	Controller::get().AddReceiver(shEventTogglePopups, 
		&PopupManager::OnTogglePopups, this);
}

PopupManager::~PopupManager()
{
	Controller::get().RemoveReceiver(shEventTogglePopups, 
		&PopupManager::OnTogglePopups, this);

	TIME.RemovePeriodicFunction(&PopupManager::Poll, this, 
		POLL_RESOLUTION);
}

void PopupManager::Poll()
{
	wxASSERT(false == mPopupMap.empty());
	wxASSERT(NULL != mMainFrame);

	wxPoint pos = wxGetMousePosition();

	// Do not test if the main frame is not visible.
	if(false == mMainFrame->IsShown())
	{
		return;
	}

	wxInt32 avoidPriority = -1;

	// First, see if any dialog windows are up that should be avoided.
	WindowMap::const_iterator itWin, itWinEnd = mWindowsToAvoid.end();
	for(itWin = mWindowsToAvoid.begin(); itWin != itWinEnd; ++itWin)
	{
		wxInt32 priority = itWin->first;
		wxWindow* window = itWin->second;

		wxPoint test = window->ScreenToClient(pos);
		wxRect rect = window->GetSize();

		if(true == rect.Contains(test))
		{
			// Since the map is sorted from lowest to highest, we always get 
			// the window with the highest priority.
			avoidPriority = priority;
		}
	}

	bool display = CONFIG.read<wxInt32>(swConfigSectionUI, swPopups, TRUE) 
		|| false;

	// Set the always on flag.
	if( (false == display) &&
		(true == ::wxGetKeyState(WXK_SHIFT)))
	{
		display = true;
	}

	// Test each window to see if it should show a popup.  Start from highest 
	// priority and work our way down.
	bool done = false;
	PopupPriorityMap::reverse_iterator it = mPopupMap.rbegin(), 
		itEnd = mPopupMap.rend();
	for(; it != itEnd; ++it)
	{
		wxInt32 priority = it->first;

		// See if we're done.
		if( (priority <= avoidPriority) ||
			(true == done))
		{
			break;
		}

		PopupMap& map = it->second;
		PopupMap::iterator itMap, itMapEnd = map.end();

		for(itMap = map.begin(); itMap != itMapEnd; ++itMap)
		{
			const IPopup* popup = itMap->first;
			wxInt32 &ticks = itMap->second;
			wxInt32 delay = 0;

			IPopup::PopupType type = popup->Type();

			if( ((true == display) ||
				(IPopup::PopupAlwaysVisible == type)) &&
				(true == popup->TestPopup(pos, delay)))
			{
				// See if we moved off this popup before coming back onto it.
				if( (popup == sCurrentPopup) &&
					(NULL == sCurrentWindow) &&
					(0 == ticks))
				{
					sCurrentPopup = NULL;
				}

				// Tick the number of polls we've been on this object.
				++ticks;

				// If we get over the right amount of delay time, show the popup.
				if( (delay <= ticks) &&
					(popup != sCurrentPopup))
				{
					// Destroy any current window.
					if(NULL != sCurrentWindow)
					{
						wxLogDebug(
							wxT("PopupManager: Destroying current window."));
						sDestroyType = DestroyByPopup;
						sCurrentWindow->Destroy();
						sCurrentWindow = NULL;
					}

					wxLogDebug(
						wxT("PopupManager: Creating popup window."));

					wxWindow* focus = wxWindow::FindFocus();
					wxPopupWindow* window = new wxPopupWindow(popup, pos);
					window->Show();
					sCurrentPopup = popup;

					if(NULL != focus)
					{
						focus->SetFocus();
					}
				}

				// Assume that the first found is the most visible.
				done = true;
				break;
			}
			else
			{
				if(-1 == delay)
				{
					sCurrentPopup = NULL;
				}

				ticks = 0;
			}
		}
	}
}

void PopupManager::OnTogglePopups(wxInt32)
{
    wxBOOL popup = CONFIG.read<wxInt32>(swConfigSectionUI, swPopups, TRUE);
    CONFIG.write(swConfigSectionUI, swPopups, popup ? FALSE : TRUE);

    // Update UI if needed.
    Controller::get().Transmit(shEventPopupToggle, popup ? true : false);

    const wxString stPopupsAreNowOff = _("Popups are now off.");
	const wxString stPopupsAreNowOn = _("Popups are now on.");
	
    wxString str = popup ? stPopupsAreNowOff : stPopupsAreNowOn;
    RULE.AsynchExecute(shRuleGenerateSystemMessage, DataObject(str, wxDateTime::Now()));

    // Kill any existing popup.
	if( (TRUE == popup) &&
		(NULL != sCurrentWindow))
	{
		wxLogDebug(wxT("PopupManager: Destroying current window."));
		sDestroyType = DestroyByPopup;
		sCurrentWindow->Destroy();
		sCurrentWindow = NULL;
	}
}
