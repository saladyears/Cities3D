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
#include "SettlersFrame.h"
#include "ConfigDatabase.h"
#include "Controller.h"
#include "DataObject.h"
#include "PopupManager.h"
#include "GameEngine.h"
#include "ContextListCtrl.h"
#include "ILayoutCreator.h"
#include "IPluginEngine.h"
#include "IOptionEngine.h"
#include "INetworkEngine.h"
#include "IconWindow.h"
#include "IUIEngine.h"
#include "SkinManager.h"
#include "PaneMixin.h"
#include "LayoutManager.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	class ContextLayout : public ILayoutCreator
	{
	public:
		ContextLayout(const IUIEngine::MenuPriorityMap& items)
			: mItems(items)
		{}

		virtual wxBaseListCtrl *CreateLayout(wxWindow * const window) const
		{
			wxContextListCtrl *layout = new wxContextListCtrl(window);

			layout->Lock();

			// Add each of the menu items.
			IUIEngine::MenuPriorityMap::const_iterator it, itEnd = mItems.end();
			for(it = mItems.begin(); it != itEnd; ++it)
			{
				const IUIEngine::MenuItem& item = it->second;

				layout->AddMenuItem(item.text, item.index, item.image);
			}

			layout->Unlock();

			return layout;
		}

	private:
		IUIEngine::MenuPriorityMap mItems;
	};

	enum
	{
		ID_PANE_WINDOW		 = 1000,
	};

	wxIconWindow* spIconWindow = NULL;
	wxWindow* spPaneWindow = NULL;
	PaneMixin* spPaneMixin = NULL;

	const wxSize sMinIconPaneSize = wxSize(225, 600);

	void ClearPaneWindow(wxAuiManager& manager)
	{
		if(NULL != spPaneWindow)
		{
			// Hide the icon window.
			wxAuiPaneInfo& info = manager.GetPane(spPaneWindow);
			manager.ClosePane(info);

			spPaneMixin = NULL;
		}
	}

	void ClearIconWindow(wxAuiManager& manager)
	{
		wxAuiPaneInfo& info = manager.GetPane(spIconWindow);
		manager.ClosePane(info);
	}
	
	void CreateDefaultPaneWindow(wxWindow* frame, wxAuiManager& manager)
	{
		wxSize size = frame->GetClientSize();

		// Create a generic pane window to fill up the rest of the space.
		spPaneWindow = new wxWindow(frame, ID_PANE_WINDOW, 
			wxPoint(size.x, size.y));
		spPaneWindow->SetBackgroundColour(SKIN.Element(shBackground));
		manager.AddPane(spPaneWindow, wxAuiPaneInfo().CaptionVisible(false)
			.Dockable(false).Center().DestroyOnClose(true));
	}

	void CreateIconWindow(wxWindow* frame, wxAuiManager& manager)
	{
		ClearPaneWindow(manager);
		
		// Disable sizing while in the main UI.
		manager.GetArtProvider()->SetMetric(wxAUI_DOCKART_SASH_SIZE, 0);
		#ifdef __WXGTK__
		manager.GetArtProvider()->SetMetric(wxAUI_DOCKART_PANE_BORDER_SIZE, 0);
		#endif

		CreateDefaultPaneWindow(frame, manager);

		manager.Update();

		// Create the icon window and attach it.
		spIconWindow = new wxIconWindow(frame);
		manager.AddPane(spIconWindow, wxAuiPaneInfo().CaptionVisible(false)
			.Dockable(false).Left().MinSize(sMinIconPaneSize)
			.DestroyOnClose(true));
		
		manager.Update();
	}

	void CreatePaneWindow(PaneMixin* pane, const wxString& text, 
		wxAuiManager& manager)
	{
		ClearPaneWindow(manager);

		spPaneMixin = pane;
		spPaneWindow = pane->GetWindow();
		spPaneWindow->SetId(ID_PANE_WINDOW);
		manager.AddPane(spPaneWindow, wxAuiPaneInfo().CaptionVisible(true)
			.Dockable(false).Center().Caption(text).DestroyOnClose(true));
		manager.Update();
	}

	wxAuiNotebook* sSidePane = NULL;
	wxSize sMinSize = wxSize(165, 150);

	bool sInConstructor = false;
	bool sInExit = false;
}

//---------------------------- PUBLIC           -----------------------------//
wxSettlersFrame::wxSettlersFrame(wxWindow *parent, const wxPoint &pos, 
								 const wxSize &size)
: wxBaseFrame(parent, swCitiesOnline, pos, size)
, mClickId(-1)
, mWasMaximized(false)
, mRestartEngine(false)
{
	SetBackgroundColour(SKIN.Element(shBackground));

	sInConstructor = true;

	mAuiManager.SetManagedWindow(this);
	
	GUI.SetMainFrame(this);
	PopupManager::get().SetMainFrame(this);

	// In Cities3D, the main window must always be at least 800x600.
	SetMinSize(wxSize(875, 600));

	CreateIconWindow(this, mAuiManager);
	
	wxInt32 width, height;

	width = CONFIG.read(swConfigSectionUI, swMainFrameWidth, 875);
	height = CONFIG.read(swConfigSectionUI, swMainFrameHeight, 600);

	SetSize(-1, -1, width, height);

	Controller::get().AddReceiver(shEventStartGame, 
		&wxSettlersFrame::OnStartGame, this);
	Controller::get().AddReceiver(shEventInitializeUI,
		&wxSettlersFrame::OnInitializeUI, this);
	Controller::get().AddReceiver(shEventQuit,
		&wxSettlersFrame::OnQuit, this);
	Controller::get().AddReceiver(shEventExit,
		&wxSettlersFrame::OnExit, this);
	Controller::get().AddReceiver(shEventImmediateExit,
		&wxSettlersFrame::OnImmediateExit, this);
	Controller::get().AddReceiver(shEventRequestUserAttention,
		&wxSettlersFrame::OnRequestUserAttention, this);
	Controller::get().AddReceiver(shEventChangeMainPane,
		&wxSettlersFrame::OnChangeMainPane, this);
	Controller::get().AddReceiver(shEventNewSidePane,
		&wxSettlersFrame::OnNewSidePane, this);
	Controller::get().AddReceiver(shEventAddSidePane,
		&wxSettlersFrame::OnAddSidePane, this);
	Controller::get().AddReceiver(shEventUncheckMenu,
		&wxSettlersFrame::OnUncheckMenu, this);
	Controller::get().AddReceiver(shEventSaveLayout,
		&wxSettlersFrame::OnSaveLayout, this);
	Controller::get().AddReceiver(shEventRestoreLayout,
		&wxSettlersFrame::OnRestoreLayout, this);
	
	sInConstructor = false;

	// Always load the core rulesets.  This has to be done after the frame
	// exists.
	PLUGIN.LoadRuleset(swCoreRules, swCore);
	PLUGIN.LoadRuleset(swCitiesOnlineRules, swCitiesOnlineNoSpace);
}

wxSettlersFrame::~wxSettlersFrame()
{
	mAuiManager.UnInit();

	//record final location
	wxRect rect = GetRect();

	if( (false == IsMaximized()) &&
		(false == IsFullScreen()))
	{
		CONFIG.write(swConfigSectionUI, swMainFrameWidth, rect.width);
		CONFIG.write(swConfigSectionUI, swMainFrameHeight, rect.height);
	}

	Controller::get().RemoveReceiver(shEventStartGame, 
		&wxSettlersFrame::OnStartGame, this);
	Controller::get().RemoveReceiver(shEventInitializeUI,
		&wxSettlersFrame::OnInitializeUI, this);
	Controller::get().RemoveReceiver(shEventQuit,
		&wxSettlersFrame::OnQuit, this);
	Controller::get().RemoveReceiver(shEventExit,
		&wxSettlersFrame::OnExit, this);
	Controller::get().RemoveReceiver(shEventImmediateExit,
		&wxSettlersFrame::OnImmediateExit, this);
	Controller::get().RemoveReceiver(shEventRequestUserAttention,
		&wxSettlersFrame::OnRequestUserAttention, this);
	Controller::get().RemoveReceiver(shEventChangeMainPane,
		&wxSettlersFrame::OnChangeMainPane, this);
	Controller::get().RemoveReceiver(shEventNewSidePane,
		&wxSettlersFrame::OnNewSidePane, this);
	Controller::get().RemoveReceiver(shEventAddSidePane,
		&wxSettlersFrame::OnAddSidePane, this);
	Controller::get().RemoveReceiver(shEventUncheckMenu,
		&wxSettlersFrame::OnUncheckMenu, this);
	Controller::get().RemoveReceiver(shEventSaveLayout,
		&wxSettlersFrame::OnSaveLayout, this);
	Controller::get().RemoveReceiver(shEventRestoreLayout,
		&wxSettlersFrame::OnRestoreLayout, this);
}

//---------------------------- PROTECTED        -----------------------------//
void wxSettlersFrame::Size(wxSizeEvent &event)
{
	wxBOOL max = IsMaximized();

	// Since the maximize gets set outside of the constructor when the window
	// is created (in app.cpp), ignore the size events generated during
	// construction to determine if the maximize flag should be set.
	if(false == sInConstructor)
	{
		CONFIG.write(swConfigSectionUI, swMainFrameMaximized, max);
	}

	wxBaseFrame::Size(event);
}

//---------------------------- PRIVATE          -----------------------------//
BEGIN_EVENT_TABLE(wxSettlersFrame, wxBaseFrame)
	EVT_ACTIVATE(wxSettlersFrame::OnActivate)
	EVT_COMMAND_RANGE(0, INT_MAX, wxEVT_COMMAND_BUTTON_CLICKED, 
		wxSettlersFrame::OnButton)
	EVT_MENU_RANGE(0, INT_MAX, wxSettlersFrame::OnMenu)
	EVT_IDLE(wxSettlersFrame::OnIdle)
	EVT_MAXIMIZE(wxSettlersFrame::OnMaximize)
	EVT_CLOSE(wxSettlersFrame::OnClose)
	EVT_AUI_PANE_CLOSE(wxSettlersFrame::OnPaneClose)
END_EVENT_TABLE()

void wxSettlersFrame::RestartEngine()
{
	mRestartEngine = true;

	// Destroy the game menu if it exists.
	wxMenuBar* menu = GetMenuBar();
	if(NULL != menu)
	{
		SetMenuBar(NULL);
		menu->Destroy();
	}

	wxWakeUpIdle();
}

void wxSettlersFrame::OnStartGame(bool newGame)
{
	// When starting a game, we also must load any options the game requires.
	if(true == newGame)
	{
		OPTION.LoadOptions();
	}

	// Hide the windows.
	ClearIconWindow(mAuiManager);
	ClearPaneWindow(mAuiManager);

	// Don't need to call mAuiManager.Update() here because it will be handled
	// in the game window creation.
	
	Controller::get().Transmit(shEventCreateGameWindow, &mAuiManager);
}

void wxSettlersFrame::OnInitializeUI(const GamePtr& game)
{
	IUIEngine::MenuItemMap menuItems;
	GUI.GetGameMenuItems(menuItems);

	// Create the menu system while we're at it.
	wxMenuBar* bar = new wxMenuBar;
	
	IUIEngine::MenuItemMap::const_iterator it, itEnd = menuItems.end();
	for(it = menuItems.begin(); it != itEnd; ++it)
	{
		const wxString& group = it->first;
		const IUIEngine::MenuPriorityMap& map = it->second;

		wxMenu* menu = new wxMenu;

		IUIEngine::MenuPriorityMap::const_iterator itMap, itMapEnd = map.end();
		for(itMap = map.begin(); itMap != itMapEnd; ++itMap)
		{
			const IUIEngine::MenuItem& item = itMap->second;

			menu->Append(item.index, item.text, wxEmptyString, item.menutype);
		}

		bar->Append(menu, group);
	}

	SetMenuBar(bar);

	sSidePane = NULL;

	GUI.CreateUI(game, mAuiManager);

	// Set the perspective, if any.
	wxString layout;
	LayoutManager::LoadLayout(layout);

	if(false == layout.IsEmpty())
	{
		mAuiManager.LoadPerspective(layout, false);
	}

	mAuiManager.Update();

	//update player data immediately
	Controller::get().Transmit(shEventPlayerUI, DataObject(game, -1));
	Controller::get().Transmit(shEventPlayerUI, DataObject(game, -2));
}

void wxSettlersFrame::OnQuit(wxWindow* window)
{
	if(true == GUI.CanQuit(window))
	{
		// Must destroy the GL canvas to free all GL memory first.
		Controller::get().Transmit(shEventCloseGameWindow, 0);

		// Destroy all UI.
		wxAuiPaneInfoArray info = mAuiManager.GetAllPanes();
		for(size_t i = 0; i < info.GetCount(); ++i)
		{
			wxWindow* window = info[i].window;

#ifdef __WXGTK__
			// this prevents wxgtk from triggering an destructive
			// resize event when detaching the window
			window->SetAutoLayout(false);
#endif

			mAuiManager.DetachPane(window);
			window->Destroy();
		}

		// Restart the game engine.
		RestartEngine();

		CreateIconWindow(this, mAuiManager);
	}
}

void wxSettlersFrame::OnExit(wxWindow* window)
{
	sInExit = true;

	if( (NULL == window) || 
        (true == GUI.CanQuit(window)))
	{
		// Close things down first to allow time to clean up.
		Controller::get().Transmit(shEventCloseGameWindow, 0);

		Destroy();
	}

	sInExit = false;
}

void wxSettlersFrame::OnImmediateExit(wxInt32)
{
	exit(-1);
}

void wxSettlersFrame::OnRequestUserAttention(wxInt32)
{
	RequestUserAttention();
}

void wxSettlersFrame::OnChangeMainPane(const DataObject& object)
{
	PaneMixin* pane = object.read<PaneMixin*>();
	const wxString& text = object.read<wxString>(1);

	CreatePaneWindow(pane, text, mAuiManager);
}

void wxSettlersFrame::OnNewSidePane(const DataObject& object)
{
	wxASSERT(NULL == sSidePane);

	wxSize size = GetClientSize();

	sSidePane = new wxAuiNotebook(this, wxID_ANY, wxPoint(size.x, size.y), 
		sMinSize, wxAUI_NB_TOP | wxAUI_NB_TAB_SPLIT |wxAUI_NB_TAB_MOVE | 
		wxAUI_NB_SCROLL_BUTTONS | wxAUI_NB_TAB_EXTERNAL_MOVE | wxNO_BORDER);

	const wxString& caption = object.read<wxString>();
	const wxString& name = object.read<wxString>(1);

	mAuiManager.AddPane(sSidePane, wxAuiPaneInfo().MinSize(sMinSize)
		.Caption(caption).CloseButton(false).MaximizeButton(false)
		.Dockable(true).Right().Name(name));
}

void wxSettlersFrame::OnAddSidePane(const DataObject& object)
{
	wxASSERT(NULL != sSidePane);

	wxWindow* window = object.read<wxWindow*>();
	const wxString& name = object.read<wxString>(1);

	window->Reparent(sSidePane);

	sSidePane->AddPage(window, name);
}

void wxSettlersFrame::OnUncheckMenu(wxInt32 id)
{
	// If we still have a menu bar, find the item and uncheck it.
	wxMenuBar* bar = GetMenuBar();
	if(NULL != bar)
	{
		wxMenuItem* item = bar->FindItem(id);
		if(NULL != item)
		{
			item->Check(false);
		}
	}
}

void wxSettlersFrame::OnSaveLayout(wxInt32)
{
	// Get the layout string from the manager.
	wxString str = mAuiManager.SavePerspective();

    wxLogDebug(str);

	LayoutManager::SaveLayout(str);
}

void wxSettlersFrame::OnRestoreLayout(wxInt32)
{
    wxString layout;
	LayoutManager::RestoreLayout(layout);

    if(false == layout.IsEmpty())
	{
		mAuiManager.LoadPerspective(layout, false);
	}

	mAuiManager.Update();
}

void wxSettlersFrame::OnMaximize(wxMaximizeEvent&)
{
	mWasMaximized = IsMaximized();
}

void wxSettlersFrame::OnPaneClose(wxAuiManagerEvent& event)
{
	// If we're in a game, i.e. the pane mixin is NULL, ignore the event,
	// it's a pane from the game being closed.
	if( (NULL != spPaneMixin) &&
		(true == GUI.CanQuit(spPaneWindow)))
	{
		// Handle any pane-specific cleanup.
		spPaneMixin->ClosePane();

		// If a user-initiated pane window is closed, replace it with the default
		// empty.
		CreateDefaultPaneWindow(this, mAuiManager);

		RestartEngine();
	}
}

void wxSettlersFrame::OnActivate(wxActivateEvent &)
{
	Controller::get().Transmit(shEventDirty, 0);
}

void wxSettlersFrame::OnButton(wxCommandEvent &event)
{
	size_t id = event.GetId();

	// If we have a pane already up, we need to restart the game engine.  This
	// fixes situations where rules may have been loaded by the pane that
	// interfere with other rulesets it they stay resident (i.e. Network).
	if(NULL != spPaneMixin)
	{
		mClickId = static_cast<wxInt32>(id);
		ClearPaneWindow(mAuiManager);
		RestartEngine();
	}
	else
	{
		GUI.HandleMainMenuClick(id);
	}
}

void wxSettlersFrame::OnMenu(wxCommandEvent &event)
{
	size_t id = event.GetId();

	// Find the actual menu entry in the menu bar.
	wxMenuBar* bar = GetMenuBar();
	wxASSERT(NULL != bar);

	wxMenuItem* item = bar->FindItem(id);

	GUI.HandleGameMenuClick(id, mAuiManager, item);
}

void wxSettlersFrame::OnIdle(wxIdleEvent& )
{
	// Have to wait until all UI have unregistered themselves.
	if( (true == mRestartEngine) &&
		(false == GUI.HasRegisteredUI()))
	{
		mRestartEngine = false;
		GameEngine::get().StartEngine();
		PLUGIN.LoadRuleset(swCoreRules, swCore);
		PLUGIN.LoadRuleset(swCitiesOnlineRules, swCitiesOnlineNoSpace);

		// If we have a menu click run it.
		if(-1 != mClickId)
		{
			GUI.HandleMainMenuClick(mClickId);
			mClickId = -1;
		}
	}
}

void wxSettlersFrame::OnClose(wxCloseEvent& event)
{
	bool destroy = true;
	
	// If we're allowed to veto the event, we need to run our quit handler.
	if( (true == event.CanVeto()) &&
		(false == GUI.CanQuit(this)))
	{
		// They had a change of heart.
		destroy = false;
		event.Veto();
	}

	if(true == destroy)
	{
		// We only need to do this cleanup if this is happening from the main
		// window, not the context menu.
		if(false == sInExit)
		{
			Controller::get().Transmit(shEventClose, 0);
		}

		// Shut down.
		Destroy();
	}
}
