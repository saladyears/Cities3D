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


#include "PlayerUI.h"
#include "TraitsUI.h"
#include "PlayerUIElement.h"
#include "PlayerGLElement.h"
#include "PlayerIcon.h"
#include "OwnerDrawWindow.h"
#include "Controller.h"
#include "DataObject.h"
#include "Game.h"
#include "SkinManager.h"
#include "ConfigDatabase.h"
#include "GLExport.h"
#include "IPluginEngine.h"
#include "IRuleEngine.h"
#include "ImageEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//---------------------------- STATICS          -----------------------------//
namespace
{
	struct no_delete_player
	{
		void operator()(void *){}
	};

	//the default TraitsUI for a player window
	class PlayerTraitsUI : public TraitsUI
	{
	public:
		PlayerTraitsUI(wxPlayerUI *ui)
			: mUI(ui)
		{			
			//defaults for a 100% sized wxPlayerUI
			mSizes[shHeight] = 93;
			mSizes[shTabWidth] = 80;
			mSizes[shTabHeight] = 16;
			mSizes[shImageWidth] = 16;
			mSizes[shImageHeight] = 16;
			mSizes[shImageSpacing] = 2;
			mSizes[shTextWidth] = 12;
			mSizes[shTextHeight] = 12;
			mSizes[shCardWidth] = 16;
			mSizes[shCardHeight] = 22;
			mSizes[shCardSpacing] = 3;
			mSizes[shIconSize] = 12;

			mConstants[shWidth] = 190;

			mFonts[shNameFont] = shPlayerUINameFont;
			mFonts[shTextFont] = shPlayerUITextFont;
		}

		wxInt32 GetWidth()
		{
			return mConstants[shWidth];
		}

		void SetWidth(const wxInt32 width)
		{
			mConstants[shWidth] = width;
		}

		virtual void Resize(const float percent)
		{
			TraitsUI::Resize(percent);

			//recalculate the width
			wxASSERT(NULL != mUI);
			mUI->CalculateWidth();
		}

	private:
		virtual void Reset()
		{
			*this = PlayerTraitsUI(mUI);
		}

		wxPlayerUI *mUI;
	};

	const wxInt32 sSpacing = 2;
}

//---------------------------- PUBLIC           -----------------------------//
wxPlayerUI::wxPlayerUI(const wxInt32 index, wxWindow *parent, 
	const GamePtr& game, wxSettlersGLCanvas* canvas)
: wxBaseUI<wxOwnerDrawWindow>(parent, wxSize(0, 0))
, mIndex(index)
, mCurrentIndex(-1)
, mpNameText(NULL)
{
	//create the sizer
	mTraitsUI = TraitsUIPtr(new PlayerTraitsUI(this));

	//set the draw and text colors
	ColorType color;
	color = game->playerGame(mIndex).color();

	mTextColor = ColorInfoData::UIColor(color);

	//create the player's name box
	mpNameText = new wxStaticText(this, wxID_ANY, game->player(mIndex).Name(), 
		wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	mpNameText->Hide();

	mpNameText->SetBackgroundColour(mTraitsUI->color(shBackground));
	mpNameText->SetForegroundColour(mTextColor);

	//now comes the fun part--read in all of the player data from the rules
	//database
	typedef std::vector<PlayerDataPtr> PlayerDataArray;
	PlayerDataArray array;
	PLUGIN.LoadPlayerData(array);

	PlayerDataArray::const_iterator it, itEnd(array.end());
	for(it = array.begin(); it != itEnd; ++it)
	{
		//only resources have the counter below
		bool right = true;
		
		if( (PlayerResource == (*it)->mPlayerDataType) ||
			(PlayerResourceBack == (*it)->mPlayerDataType))
		{
			right = false;
		}

		const PlayerData &data = *((*it).get());

		//create the right kind of element
		if(data.mPlayerDataType != PlayerSpecial)
		{
			if(data.mGameObject)
			{
				mElements[data.mPlayerDataType].push_back(PlayerElementPtr(
					new PlayerGLElement(this, data.mUpdateFunc, 
					data.mColorFunc, color, data.mThreshold,
					data.mGameObject, data.mDrawObject,
					data.mImageKey, data.mImageName, data.mPopupText, 
					canvas)));
			}
			else
			{
				const HashString& image = data.mImageName;
				bool card = (HashString::npos != image.find(shCard));

				wxString popup = card ? image.wx_str() : data.mPopupText;

				mElements[data.mPlayerDataType].push_back(PlayerElementPtr(
					new PlayerUIElement(this, data.mUpdateFunc, 
					data.mColorFunc, right, data.mThreshold, 
					data.mImageKey, image, popup, card, mIndex)));
			}
		}
		else
		{
			mIcons.push_back(PlayerIconPtr(new PlayerIcon(this, data.mUpdateFunc,
				data.mImageName, data.mPopupText)));
		}
	}

	//if there's only one card type, set its text below, not to the right
	if(1 == mElements[PlayerCardBack].size())
	{
		(*mElements[PlayerCardBack].begin())->SetRight(false);
	}

	CalculateWidth(true);

	// Set the min size.
	wxInt32 height = mTraitsUI->size(shHeight);
	wxInt32 width =
		boost::shared_static_cast<PlayerTraitsUI>(mTraitsUI)->GetWidth();
	
	wxSize minSize = wxSize(width, height);
	SetMinSize(minSize);
	SetSize(minSize);

	ResizeUI();

	Show();
	mpNameText->Show();

	Controller::get().AddReceiver(shEventPlayerUI, 
		&wxPlayerUI::OnUpdate, this);
	Controller::get().AddReceiver(shEventPlayerReorder, 
		&wxPlayerUI::OnPlayerReorder, this);
	Controller::get().AddReceiver(shEventNewPlayer, 
		&wxPlayerUI::OnNewPlayer, this);
	Controller::get().AddReceiver(shEventResetBitmaps,
		&wxPlayerUI::OnResetBitmaps, this);
}

wxPlayerUI::~wxPlayerUI()
{
	Controller::get().RemoveReceiver(shEventPlayerUI, 
		&wxPlayerUI::OnUpdate, this);
	Controller::get().RemoveReceiver(shEventPlayerReorder, 
		&wxPlayerUI::OnPlayerReorder, this);
	Controller::get().RemoveReceiver(shEventNewPlayer, 
		&wxPlayerUI::OnNewPlayer, this);
	Controller::get().RemoveReceiver(shEventResetBitmaps,
		&wxPlayerUI::OnResetBitmaps, this);
}

void wxPlayerUI::CalculateWidth(bool overrideShow)
{
	DataObject input(mIndex), output;
	RULE.Decide(shLogicShowPlayerUI, input, output);

	bool show = output.read<bool>() || overrideShow;

	//recalculate width
	wxInt32 width;
	wxInt32 cardWidth = mTraitsUI->size(shCardWidth);
	wxInt32 textWidth = mTraitsUI->size(shTextWidth);
	wxInt32 imageWidth = mTraitsUI->size(shImageWidth);
	wxInt32 cardSpacing = mTraitsUI->size(shCardSpacing);
	wxInt32 numCards = mElements[PlayerCardBack].size();

	//if this is the current player, the full width must be calculated
	if(true == show)
	{
		if(1 < numCards)
		{
			width = (cardWidth * 5) + (cardSpacing * 6);
		}
		else
		{
			width = (cardWidth * 6) + (cardSpacing * 8);
		}
	}
	//otherwise, start the width off at the tab width, since that's the 
	//easy divider
	else
	{
		width = mTraitsUI->size(shTabWidth);
	}

	//then, add card width, if there are more than one card types
	if(1 < numCards)
	{
		width += (cardSpacing + cardWidth + textWidth);
	}

	//TODO: adjust for user decided stock visibility

	//add stock, if it's being shown
	if(true == show)
	{
		//allow 4 stock items per column
		wxInt32 stockCols = ((mElements[PlayerStock].size() - 1) >> 2) + 1;
		width += ((cardSpacing * (stockCols + 1)) + 
			((imageWidth + textWidth) * stockCols));
	}

	//add the points column, which is always shown
	width += (cardSpacing + imageWidth + textWidth);

	PlayerTraitsUI *pTraits = static_cast<PlayerTraitsUI *>(mTraitsUI.get());
	pTraits->SetWidth(width);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void wxPlayerUI::Paint(wxMemoryDC &dc)
{
	wxASSERT(mTraitsUI);

	wxColour drawColor;

	if(mCurrentIndex == mIndex)
	{
		drawColor = mTextColor;
	}
	else
	{
		drawColor = mTraitsUI->color(shOutline);
	}

	//background
	wxColour background(mTraitsUI->color(shBackground));
	dc.SetBrush(wxBrush(background, wxSOLID));
	dc.SetPen(wxPen(background, 1, wxSOLID));
	wxSize size = GetSize();
	dc.DrawRectangle(0, 0, size.x, size.y);

	dc.SetPen(wxPen(drawColor, 1, wxSOLID));

	//tab
	wxInt32 height = mTraitsUI->size(shHeight);
	wxInt32 tabHeight = mTraitsUI->size(shTabHeight);
	wxInt32 tabWidth = mTraitsUI->size(shTabWidth);
	wxInt32 tabY = height - tabHeight;

	//draw rects
	wxInt32 width =
		boost::shared_static_cast<PlayerTraitsUI>(mTraitsUI)->GetWidth();
	dc.DrawRectangle(0, 0, width, tabY);

	//now black out the part we don't want
	dc.SetPen(wxPen(background, 1, wxSOLID));
	dc.DrawRectangle(0, tabY - 1, tabWidth - 1, 1);

	dc.SetPen(wxPen(drawColor, 1, wxSOLID));
	dc.DrawLine(0, tabY - 1, 0, tabY + 1);

	//draw vertical lines
	PointList::const_iterator it;
	for(it = mLineList.begin(); it != mLineList.end(); ++it)
	{
		dc.DrawLine((*it).x, 0, (*it).x, tabY);
	}

	dc.SetPen(wxPen(drawColor, 1, wxSOLID));

	wxInt32 halfY = tabHeight / 2;

	dc.DrawRectangle(0, tabY, tabWidth, tabHeight);
	dc.DrawLine(0, tabY, 0, tabY + halfY);
	dc.DrawLine(tabWidth - 1, tabY, tabWidth - 1, tabY + halfY);

	dc.SetPen(wxPen(background, 1, wxSOLID));
	dc.DrawRectangle(1, tabY, tabWidth - 2, halfY);
	
	dc.SetBrush(wxNullBrush);
	dc.SetPen(wxNullPen);
}

void wxPlayerUI::Size(wxSizeEvent& event)
{
	wxSize size = event.GetSize();
	wxSize minSize = GetMinSize();

	float ratioX = static_cast<float>(size.x) / static_cast<float>(minSize.x);
	float ratioY = static_cast<float>(size.y) / static_cast<float>(minSize.y);
	float ratio = std::min(ratioX, ratioY);

	mTraitsUI->Resize(ratio);

	ResizeUI();
}

void wxPlayerUI::ResizeUI()
{
	wxASSERT(mTraitsUI);

	DataObject input(mIndex), output;
	RULE.Decide(shLogicShowPlayerUI, input, output);

	bool show = output.read<bool>();
	
	wxInt32 tabWidth = mTraitsUI->size(shTabWidth);
	wxInt32 tabHeight = mTraitsUI->size(shTabHeight);
	wxInt32 height = mTraitsUI->size(shHeight) - tabHeight;

	//get some more needed values
	wxInt32 cardWidth = mTraitsUI->size(shCardWidth);
	wxInt32 cardHeight = mTraitsUI->size(shCardHeight);
	wxInt32 textWidth = mTraitsUI->size(shTextWidth);
	wxInt32 textHeight = mTraitsUI->size(shTextHeight);
	wxInt32 imageSpacing = mTraitsUI->size(shImageSpacing);
	wxInt32 cardSpacing = mTraitsUI->size(shCardSpacing);

	//calculate the font size needed
	wxClientDC dc(this);
	const wxFont textFont = mTraitsUI->calculateFontSize(dc, shTextFont, 
		shTextHeight);

	wxPoint point;
	point.x = cardSpacing;
	point.y = height - textHeight - cardHeight - imageSpacing;

	//resource card backs
	PlayerElementList::iterator it, itEnd(mElements[PlayerResourceBack].end());
	for(it = mElements[PlayerResourceBack].begin(); it != itEnd; ++it)
	{
		(*it)->Resize(point, wxSize(cardWidth, cardHeight), 
			wxSize(textWidth, textHeight));
		(*it)->SetFont(textFont);
		point.x += (cardWidth + cardSpacing);
	}

	//reset back to the left for the normal resources
	point.x = cardSpacing;

	wxInt32 count = 0;

	//now we actually resize the various parts of the UI
	itEnd = mElements[PlayerResource].end();
	for(it = mElements[PlayerResource].begin(); it != itEnd; ++it)
	{
		if( (0 != count) && 
			(0 == (count % 5)))
			{
				//if there's more than 5 resources, reset back up to the left
				//and above
				point.x = cardSpacing;
				point.y -= (cardHeight + textHeight);
			}

		(*it)->Resize(point, wxSize(cardWidth, cardHeight), 
			wxSize(textWidth, textHeight));
		(*it)->SetFont(textFont);
		point.x += (cardWidth + cardSpacing);

		++count;
	}

	//set the left-most x and y properly
	point.x = cardSpacing + (5 * (cardWidth + cardSpacing));
	point.y = height - textHeight - cardHeight - imageSpacing;

	//reset the line list and set the lines appropriately
	mLineList.clear();
	
	//if there's only one card type (Standard Settlers with dev cards), do some
	//different placements to keep it near the resource cards or resource card
	//back
	if(1 == mElements[PlayerCardBack].size())
	{
		//cards		
		it = mElements[PlayerCardBack].begin();

		//if we are the current player, just draw it a little bit more to the 
		//right
		if(true == show)
		{
			point.x += cardSpacing;
		}
		//if we are not the current player, keep it much closer to the
		//resource card back
		else
		{
			point.x = (cardSpacing * 3) + cardWidth;
		}

		(*it)->Resize(point, wxSize(cardWidth, cardHeight), 
			wxSize(textWidth, textHeight));
		(*it)->SetFont(textFont);

		//now set the x correctly for the next line
		if(true == show)
		{
			point.x += (cardWidth + cardSpacing);
		}
		else
		{
			point.x = tabWidth - 1;
		}
	}
	//if there's more than one card type, stack them on top of each other, 
	//otherwise keep them lined up with the resource cards
	else
	{
		//cards
		if(false == show)
		{
			point.x = tabWidth - 1;
		}

		mLineList.push_back(point);

		point.x += cardSpacing;
		point.y = imageSpacing;
		
		itEnd = mElements[PlayerCardBack].end();
		for(it = mElements[PlayerCardBack].begin(); it != itEnd; ++it)
		{
			(*it)->Resize(point, wxSize(cardWidth, cardHeight), 
				wxSize(textWidth, textHeight));
			(*it)->SetFont(textFont);

			//TODO: space them evenly instead of absolutely
			point.y += (cardHeight + imageSpacing);
		}

		point.x += (cardWidth + textWidth);
	}

	//line
	mLineList.push_back(point);

	wxInt32 imageWidth = mTraitsUI->size(shImageWidth);
	wxInt32 imageHeight = mTraitsUI->size(shImageHeight);

	//TODO: allow the user to show/hide stock any time they want for
	//any player

	//stock
	if(true == show)
	{
		point.x += cardSpacing;
		point.y = imageSpacing;
		
		wxInt32 count = 0;

		itEnd = mElements[PlayerStock].end();
		for(it = mElements[PlayerStock].begin(); it != itEnd; ++it)
		{
			if( (0 != count) && 
				(0 == (count % 4)))
			{
				//if there's more than 4 stocks, reset back up to the top
				point.x += (imageWidth + textWidth + cardSpacing);
				point.y = imageSpacing;
			}

			(*it)->Resize(point, wxSize(imageWidth, imageHeight), 
				wxSize(textWidth, textHeight));
			(*it)->SetFont(textFont);

			point.y += (imageHeight + imageSpacing);

			++count;
		}

		//line
		point.x += (imageWidth + textWidth);
		mLineList.push_back(point);
	}

	//points
	point.x += cardSpacing;
	point.y = imageSpacing;

	itEnd = mElements[PlayerPoints].end();
	for(it = mElements[PlayerPoints].begin(); it != itEnd; ++it)
	{
		(*it)->Resize(point, wxSize(imageWidth, imageHeight), 
			wxSize(textWidth, textHeight));
		(*it)->SetFont(textFont);

		point.y += (imageHeight + imageSpacing);
	}

	// Set player icons if they exist.
	if(false == mIcons.empty())
	{
		wxASSERT(false == mLineList.empty());

		wxInt32 iconSize = mTraitsUI->size(shIconSize);

		wxPoint point = *mLineList.begin();
		point.x -= (iconSize + 2);
		point.y = 2;

		// Use rows of 3.
		wxInt32 count = 0;

		PlayerIconList::iterator it, itEnd = mIcons.end();
		for(it = mIcons.begin(); it != itEnd; ++it)
		{
			PlayerIconPtr icon = (*it);
			icon->Resize(point, wxSize(iconSize, iconSize));

			point.x -= iconSize;

			++count;
			if(3 == count)
			{
				point = *mLineList.begin();
				point.x -= (iconSize + 2);
				point.y = (iconSize + 4);
				count = 0;
			}
		}
	}

	//adjust the name tab and font size
	const wxFont nameFont = mTraitsUI->calculateFontSize(dc, shNameFont, 
		shTabHeight);

	mpNameText->SetSize(5, height, tabWidth - 8, tabHeight - 2);
	mpNameText->SetFont(nameFont);

	Refresh();
}

void wxPlayerUI::OnUpdate(const DataObject &object)
{
	wxASSERT(2 == object.numItems());
	
	//update all our data if this is the right index
	const GamePtr &game = object.read<GamePtr>();
	wxInt32 index = object.read<wxInt32>(1);

	if( (mIndex == index) ||
		(-1 == index))
	{
		Freeze();

		ElementHash::iterator it, itEnd(mElements.end());
		for(it = mElements.begin(); it != itEnd; ++it)
		{
			//update each member
			std::for_each(it->second.begin(), it->second.end(), 
				boost::bind(&PlayerElement::Update, _1, game, mIndex));
		}

		std::for_each(mIcons.begin(), mIcons.end(), 
				boost::bind(&PlayerIcon::Update, _1, game, mIndex));

		Thaw();
	}
	else if(-2 >= index)
	{
		Freeze();

		wxInt32 current = game->read<wxInt32>(shCurrentPlayer);

		if( (mCurrentIndex != current) ||
			(-3 == index))
		{
			mCurrentIndex = current;

			// Find out whether we should be shown or hidden.
			DataObject input(mIndex), output;
			RULE.Decide(shLogicShowPlayerUI, input, output);

			bool show = output.read<bool>();
				
			//show/hide the appropriate resources
			std::for_each(mElements[PlayerResource].begin(),
				mElements[PlayerResource].end(),
				boost::bind(&PlayerElement::Show, _1, show));
			std::for_each(mElements[PlayerStock].begin(), 
				mElements[PlayerStock].end(),
				boost::bind(&PlayerElement::Show, _1, show));
			std::for_each(mElements[PlayerResourceBack].begin(),
				mElements[PlayerResourceBack].end(),
				boost::bind(&PlayerElement::Show, _1, !show));

			//recalculate our width
			CalculateWidth();

			ResizeUI();
		}

		Thaw();
	}
}

void wxPlayerUI::OnPlayerReorder(const GamePtr &game)
{
	ColorType color;
	color = game->playerGame(mIndex).color();

	mTextColor = ColorInfoData::UIColor(color);

	mpNameText->SetLabel(game->player(mIndex).Name());
	mpNameText->SetForegroundColour(mTextColor);

	//set the proper player color for all bitmaps
	ElementHash::iterator it, itEnd = mElements.end();
	for(it = mElements.begin(); it != itEnd; ++it)
	{
		PlayerElementList::const_iterator it2, it2End = it->second.end();
		for(it2 = it->second.begin(); it2 != it2End; ++it2)
		{
			(*it2)->SetPlayerColor(color);
		}
	}

	//reset the current player
	OnUpdate(DataObject(game, -2));
}

void wxPlayerUI::OnNewPlayer(const DataObject &object)
{
	wxASSERT(2 == object.numItems());
	
	// Update the player name if this is the right index.
	const GamePtr &game = object.read<GamePtr>();
	wxInt32 index = object.read<wxInt32>(1);

	if(mIndex == index)
	{
		mpNameText->SetLabel(game->player(mIndex).Name());
		
		mCurrentIndex = -1;
		OnUpdate(DataObject(game, -2));
	}
}

void wxPlayerUI::OnResetBitmaps(wxInt32)
{
	std::for_each(mElements[PlayerStock].begin(), 
		mElements[PlayerStock].end(),
		boost::bind(&PlayerElement::ResetBitmap, _1));

	std::for_each(mIcons.begin(), mIcons.end(),
		boost::bind(&PlayerIcon::ResetBitmap, _1));
}
