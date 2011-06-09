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
#include "BankUI.h"
#include "TraitsUI.h"
#include "Controller.h"
#include "DataObject.h"
#include "Game.h"
#include "BankData.h"
#include "ImageEngine.h"
#include "Image.h"
#include "GLTextures.h"
#include "IDrawObject.h"
#include "SettlersGLCanvas.h"
#include "SkinManager.h"
#include "ConfigDatabase.h"
#include "BaseStaticText.h"
#include "TextPopup.h"
#include "IPluginEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	bool sForceRefresh = false;

	//class for drawing card stacks
	class DrawCardObject : public IDrawObject
	{
	public:
		DrawCardObject(const HashString &imageKey, const HashString &imageName, 
			const HashString &location, const HashString &type, 
			const wxInt32 startCards)
			: mImageKey(imageKey), mImageName(imageName), mLocation(location), 
			mType(type), mStartCards(startCards), mCards(0),
			mEye(21.0f, 9.0f, -24.0f)
		{
			Obtain();
		}

		~DrawCardObject() 
		{
			Release();
		}

		void SetCards(wxInt32 cards) 
		{
			mCards = cards;
		}
		
		wxInt32 GetCards() const 
		{
			return mCards;
		}

		const Vector& GetEye() const
		{
			return mEye;
		}

		const HashString &GetLocation() const 
		{
			return mLocation;
		}

		const HashString &GetType() const 
		{
			return mType;
		}

		virtual IGameObject *GetObject() const 
		{
			return NULL;
		}
		
		virtual bool CanDelete() const 
		{
			return false;
		}
		
		virtual wxInt32 GetPriority() const 
		{
			return 0;
		}

		virtual bool CanBuild(const GameObjectPtr &) const 
		{
			return false;
		}
		
		virtual bool CanSelect(const Vector &, const Vector &, float &, 
			wxInt32 &, bool) 
		{
			return false;
		}

		virtual IDrawObject *Clone(const GameObjectPtr &) const 
		{
			return NULL;
		}

		//functions for managing resources
		virtual void Obtain()
		{
			wxImage image = Image::Borderize(
				IMAGE.GetImage(mImageKey, mImageName));
			wxASSERT(image.Ok());

			//create the texture id
			mCard[0] = GLTextures::BuildTexture(image);
		}

		virtual void Release()
		{
			//set no texture
			glBindTexture(GL_TEXTURE_2D, 0);

			//clear out the old textures
			glDeleteTextures(1, &mCard[0]);
		}

		virtual void Render(const wxInt32) const
		{
			glEnable(GL_TEXTURE_2D);

			glEnable(GL_LINE_SMOOTH);
			glEnable(GL_POLYGON_SMOOTH);
			glHint(GL_POLYGON_SMOOTH, GL_NICEST);
			glBindTexture(GL_TEXTURE_2D, 0);

			glPushMatrix();

			//start off halfway below center
			glTranslated(0.0, (-0.03 * (mStartCards >> 1)), 0.0);
			for(wxInt32 i = 0; i < mCards; ++i)
			{
				if(i == mCards - 1)
				{
					glBindTexture(GL_TEXTURE_2D, mCard[0]);
				}

				glBegin(GL_QUADS);
					glNormal3f(0.0, 1.0, 0.0);
					glTexCoord2d(0.00, 0.00); glVertex3d(-1.45, 0.0, -2.1);
					glTexCoord2d(1.00, 0.00); glVertex3d(1.45, 0.0, -2.1);
					glTexCoord2d(1.00, 1.00); glVertex3d(1.45, 0.0, 2.1);
					glTexCoord2d(0.00, 1.00); glVertex3d(-1.45, 0.0, 2.1);	
				glEnd();
				glTranslated(0.0, 0.03, 0.0);
			}

			glPopMatrix();
		}

			
		virtual void PopupFunction(wxString&, wxString&, wxBitmap&, bool &,
			ColorType&)
		{
		}

		const wxInt32 StartCards() const 
		{
			return mStartCards;
		}

	private:
		//the card texture
		GLuint mCard[1];
		const Vector mEye;

		wxInt32 mStartCards;
		wxInt32 mCards;
		HashString mImageKey;
		HashString mImageName;
		HashString mLocation;
		HashString mType;
	};

	typedef boost::shared_ptr<DrawCardObject> DrawCardObjectPtr;

	const wxSize sMinImageSize = wxSize(38, 20);

	wxString sCategories[] = 
	{
		stAll,
		stMost,
		stMany,
		stHalf,
		stSome,
		stFew,
		stNone
	};
	wxString sLongestString;
}

class wxBankBitmap
	: public wxStaticBitmap
	, public TextPopup
{
public:
#ifdef __WIN32__
	//disables the 'this' : used in base member initializer list warning
	#pragma warning(disable:4355)
#endif
	wxBankBitmap(const wxString& text, wxWindow* parent, wxWindowID id, 
		const wxBitmap& label, const DrawCardObjectPtr& object, 
		wxSettlersGLCanvas* canvas,	const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize)
	: wxStaticBitmap(parent, id, label, pos, size)
	, TextPopup(this, text)
	, mDrawObject(object)
	, mCanvas(canvas)
	{
		wxASSERT(object);

		Controller::get().AddReceiver(shEventPlayerUI, 
			&wxBankBitmap::OnUpdate, this);
		Controller::get().AddReceiver(shEventImageUpdate, 
			&wxBankBitmap::OnImageUpdate, this);
		Controller::get().AddReceiver(shEventResetBitmaps,
			&wxBankBitmap::OnResetBitmaps, this);
	}

	virtual ~wxBankBitmap()
	{
		Controller::get().RemoveReceiver(shEventPlayerUI, 
			&wxBankBitmap::OnUpdate, this);
		Controller::get().RemoveReceiver(shEventImageUpdate, 
			&wxBankBitmap::OnImageUpdate, this);
		Controller::get().RemoveReceiver(shEventResetBitmaps,
			&wxBankBitmap::OnResetBitmaps, this);
	}

	DECLARE_EVENT_TABLE()

private:
	void OnSize(wxSizeEvent& event)
	{
		// When we get resized, create a new bitmap at the new size.
		ResetBitmap(event.GetSize());
	}

	void OnUpdate(const DataObject &object)
	{
		wxASSERT(1 <= object.numItems());

		const GamePtr &game = object.read<GamePtr>();
		wxASSERT(game);

		wxInt32 cards = 
			game->read(mDrawObject->GetLocation(), mDrawObject->GetType());

		// If the number of cards changes, refresh the bitmap.
		if(cards != mDrawObject->GetCards())
		{
			mDrawObject->SetCards(cards);
			ResetBitmap(GetSize());
		}
	}

	void OnImageUpdate(bool)
	{
		IMAGE.IgnoreCache();

		// Clear the bitmap used by the draw object.
		mDrawObject->Release();
		mDrawObject->Obtain();
		
		ResetBitmap(GetSize());
	}

	void OnResetBitmaps(wxInt32)
	{
		if(TRUE == CONFIG.read<wxInt32>(swConfigSectionOpenGL, swRenderBitmaps, 
			TRUE))
		{
			ResetBitmap(GetSize());
		}
	}

	void ResetBitmap(const wxSize& size)
	{
		// Images must be larger than 1x1.
		if( (true == IsShown()) && 
			(1 < size.x) && 
			(1 < size.y))
		{
			SetBitmap(mCanvas->ConvertGLtoWX(mDrawObject, size.x, size.y, 
				mDrawObject->GetEye()));

			Refresh();
		}
	}

	DrawCardObjectPtr mDrawObject;
	wxSettlersGLCanvas* mCanvas;
};

BEGIN_EVENT_TABLE(wxBankBitmap, wxStaticBitmap)
	EVT_SIZE(wxBankBitmap::OnSize)
END_EVENT_TABLE()

class wxBankTextPanel : public wxPanel
{
public:
	wxBankTextPanel(wxWindow* parent, const wxSize& size, const HashString& key,
		const HashString& name, const wxString& text, 
		const HashString &location, const HashString &type, wxInt32 startCards)
		: wxPanel(parent, wxID_ANY, wxDefaultPosition, size)
		, mText(NULL)
		, mPercent(NULL)
		, mLocation(location)
		, mType(type)
		, mStartCards(startCards)
		, mCards(startCards)
	{
		SetBackgroundColour(SKIN.Element(shBackground));

		// Use the bank image to calculate the average color of the image and
		// set the text colour to it.
		wxImage image = IMAGE.GetImage(key, name);
		wxColour color = AverageColor(image);

		wxString translation = wxGetTranslation(text);
		if(translation.Length() > sLongestString.Length())
		{
			sLongestString = translation;
		}
		
		// Create text objects.
		mText = new wxBaseStaticText(this, wxID_ANY, translation, 
			shDefaultText, shDefaultFont, wxDefaultSize, wxALIGN_CENTRE | 
			wxST_NO_AUTORESIZE);
		mText->SetForegroundColour(color);
		mPercent = new wxBaseStaticText(this, wxID_ANY, stAll, 
			shDefaultText, shDefaultFont, wxDefaultSize, wxALIGN_CENTRE |
			wxST_NO_AUTORESIZE);

		mText->Show();
		mPercent->Show();

		Controller::get().AddReceiver(shEventPlayerUI, 
			&wxBankTextPanel::OnUpdate, this);
	}

	~wxBankTextPanel()
	{
		Controller::get().RemoveReceiver(shEventPlayerUI, 
			&wxBankTextPanel::OnUpdate, this);
	}

	DECLARE_EVENT_TABLE()

private:
	void OnSize(wxSizeEvent& event)
	{
		wxSize size = event.GetSize();

		wxInt32 halfY = size.y / 2;

		mText->SetSize(0, 0, size.x, halfY);
		mPercent->SetSize(0, halfY, size.x, halfY);

		// Calculate the best font size.
		wxFont font = SKIN.Font(shDefaultFont);
		wxClientDC dc(this);
		font = CalculateFontSize(sLongestString, dc, font, size.x, halfY);

		mText->SetFont(font);
		mPercent->SetFont(font);

		Refresh();
	}

	void OnUpdate(const DataObject& object)
	{
		wxASSERT(1 <= object.numItems());

		const GamePtr &game = object.read<GamePtr>();
		wxASSERT(game);

		wxInt32 cards = game->read(mLocation, mType);

		// Update the card count if it's changed.
		if(cards != mCards)
		{
			float percent = static_cast<float>(cards) / 
				static_cast<float>(mStartCards);

			wxString text;
			float colorPercent = 255.0f;
			for(size_t i = 0; i < _countof(sCategories); ++i)
			{
				float val = 1.0f - (0.2f * i);

				if(val <= percent)
				{
					text = sCategories[i];
					colorPercent *= (1.0f - (0.12 * i));
					break;
				}
			}
			
			mPercent->SetForegroundColour(wxColour(
				static_cast<wxUint8>(colorPercent),
				static_cast<wxUint8>(colorPercent),
				static_cast<wxUint8>(colorPercent)));
			mPercent->SetLabel(text);

			mCards = cards;
		}
	}

	wxColour AverageColor(const wxImage &image)
	{
		size_t r = 0, g = 0, b = 0;

		// Find the average color in an image
		wxInt32 w = image.GetWidth();
		wxInt32 h = image.GetHeight();
		size_t size = w * h;

		wxUint8 *byte = image.GetData();
		wxUint8 *end = byte + (size * 3);
		while(byte < end)
		{
			r += byte[0];
			g += byte[1];
			b += byte[2];

			byte += 3;
		}

		float mult = 1.0f / static_cast<float>(size);

		wxUint8 red = static_cast<wxUint8>(r * mult);
		wxUint8 green = static_cast<wxUint8>(g * mult);
		wxUint8 blue = static_cast<wxUint8>(b * mult);

		return wxColour(red, green, blue);
	}

	wxFont CalculateFontSize(const wxString &text, wxDC &dc, 
		const wxFont &font, wxInt32 width, wxInt32 height)
	{
		wxFont sizedFont = font;

		// Loop until we find a size that fits.
		for(wxInt32 size = 1; size <= 100; ++size)
		{
			//set the font's point size
			sizedFont.SetPointSize(size);
			dc.SetFont(sizedFont);

			//retrieve the text extent of a full string
			wxInt32 w, h;
			dc.GetTextExtent(text, &w, &h);

			//check size
			if( (width < w) || 
				(height < h))
			{
				sizedFont.SetPointSize(size - 1);
				break;
			}
		}

		return sizedFont;
	}

	wxBaseStaticText* mText;
	wxBaseStaticText* mPercent;

	HashString mLocation;
	HashString mType;
	wxInt32 mStartCards;
	wxInt32 mCards;
};

BEGIN_EVENT_TABLE(wxBankTextPanel, wxPanel)
	EVT_SIZE(wxBankTextPanel::OnSize)
END_EVENT_TABLE()

//---------------------------- PUBLIC           -----------------------------//
wxBankUI::wxBankUI(wxWindow *parent, const GamePtr& game, 
	wxSettlersGLCanvas* canvas)
: wxBaseUI<wxPanel>(parent, wxDefaultSize)
{
	wxASSERT(NULL != canvas);

	SetBackgroundColour(SKIN.Element(shBackground));

	// Determine the longest category string, for whichever translation we
	// happen to be using at the moment.
	sLongestString.Empty();

	for(size_t i = 0; i < _countof(sCategories); ++i)
	{
		const wxString& category = sCategories[i];

		if(category.Length() > sLongestString.Length())
		{
			sLongestString = category;
		}
	}

	// Grab all of the bank data from the rulesets.
	typedef std::vector<BankDataPtr> BankDataArray;

	BankDataArray bankdata;
	PLUGIN.LoadBankData(bankdata);

	// Create the needed wxBankBitmaps.
	BankDataArray::const_iterator it, itEnd(bankdata.end());
	for(it = bankdata.begin(); it != itEnd; ++it)
	{
		const BankData &data = *((*it).get());
		wxInt32 start = game->read(data.mOrigLocation, data.mDataType);
		wxASSERT(0 <= start);

		// GL display.
		DrawCardObjectPtr object(new DrawCardObject(data.mImageKey, 
			data.mImageName, data.mDataLocation, data.mDataType, start));

		wxBankBitmap* bitmap =  new wxBankBitmap(data.mPopupText, this, 
			wxID_ANY, wxBitmap(1, 1), object, canvas, wxDefaultPosition,
			sMinImageSize);
		bitmap->SetMinSize(sMinImageSize);
		bitmap->Hide();
		mStaticBitmapArray.push_back(bitmap);

		// Text display.
		wxBankTextPanel* panel = new wxBankTextPanel(this, sMinImageSize,
			data.mImageKey, data.mImageName, data.mDataType.wx_str(), 
			data.mDataLocation, data.mDataType, start);
		panel->SetMinSize(sMinImageSize);
		panel->Hide();
		mBankTextPanelArray.push_back(panel);
	}

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->SetSizeHints(this);
	SetSizer(sizer);

	OnRenderBitmaps(TRUE);

	//create the sizer
	size_t size = bankdata.size();
	
	wxSize minSize = wxSize(size * sMinImageSize.x, sMinImageSize.y);

	SetSize(minSize);
	SetMinSize(minSize);

	Show(true);	

	Controller::get().AddReceiver(shEventRenderBitmaps, 
		&wxBankUI::OnRenderBitmaps, this);
}

wxBankUI::~wxBankUI()
{
	Controller::get().RemoveReceiver(shEventRenderBitmaps, 
		&wxBankUI::OnRenderBitmaps, this);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
void wxBankUI::OnRenderBitmaps(wxBOOL skipLayout)
{
	// First, detach anything from the existing sizer, if it exists.
	wxSizer* sizer = GetSizer();

	if(NULL != sizer)
	{
		wxSizerItemList& items = sizer->GetChildren();

		size_t count = items.GetCount();
		for(size_t i = 0; i < count; ++i)
		{
			wxWindow* window = items[0]->GetWindow();

			if(NULL == window)
			{
				sizer->Detach(0);
				continue;
			}

			window->Hide();
			sizer->Detach(window);
		}
	}

	wxBOOL render = CONFIG.read<wxInt32>(swConfigSectionOpenGL, swRenderBitmaps, 
		TRUE);

	// If GL is on, show bitmaps.  Otherwise, show text.
	if(TRUE == render)
	{
		for(size_t i = 0; i < mStaticBitmapArray.size(); ++i)
		{
			wxWindow* window = mStaticBitmapArray[i];
			window->Show();
			sizer->Add(window, 1, wxEXPAND | wxSHAPED);
		}
	}
	else
	{
		for(size_t i = 0; i < mBankTextPanelArray.size(); ++i)
		{
			wxWindow* window = mBankTextPanelArray[i];
			window->Show();
			sizer->Add(window, 1, wxEXPAND | wxSHAPED);
		}
	}

    sizer->AddStretchSpacer();

	if(FALSE == skipLayout)
	{
		// I wish I knew a better way to force a resize.
		wxSize size = GetSize();
		Freeze();
		SetSize(size - wxSize(1, 1));
		SetSize(size);
		Thaw();
	}
}
