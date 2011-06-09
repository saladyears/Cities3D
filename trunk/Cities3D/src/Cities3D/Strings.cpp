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

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
    // Force all strings to be statically initialized first.
#ifdef __WIN32__
    #pragma warning(disable:4073)
    #pragma init_seg(lib)
    #pragma warning(default:4073)
#endif

    // All strings used in this project.

    //-----------------------------------------------------------------------//
	// Section: HashStrings
	//
    const HashString shEmpty = "";
	const HashString shTileObject = "TileObject";

    // Data

    // Display
	const HashString shBackground = "Background";
	const HashString shDefaultFont = "DefaultFont";
	const HashString shDefaultText = "DefaultText";

    // Events
    const HashString shEventAddCatalog = "eventAddCatalog";
    const HashString shEventAddSidePane = "eventAddSidePane";
    const HashString shEventChangeMainPane = "eventChangeMainPane";
    const HashString shEventClose = "eventClose";
    const HashString shEventCloseGameWindow = "eventCloseGameWindow";
    const HashString shEventCreateGameWindow = "eventCreateGameWindow";
    const HashString shEventDirty = "eventDirty";
    const HashString shEventExit = "eventExit";
    const HashString shEventFindPlugin = "eventFindPlugin";
    const HashString shEventImmediateExit = "eventImmediateExit";
    const HashString shEventInitializeUI = "eventInitializeUI";
    const HashString shEventNewSidePane = "eventNewSidePane";
    const HashString shEventPlayerUI = "eventPlayerUI";
    const HashString shEventQuit = "eventQuit";
    const HashString shEventRequestUserAttention = "eventRequestUserAttention";
    const HashString shEventResetLocale = "eventResetLocale";
    const HashString shEventRestoreLayout = "eventRestoreLayout";
    const HashString shEventSaveLayout = "eventSaveLayout";
    const HashString shEventStartGame = "eventStartGame";
    const HashString shEventStartup = "eventStartup";
	const HashString shEventSkin = "eventSkin";
    const HashString shEventUncheckMenu = "eventUncheckMenu";
	
    // Logic

    // Rules

    //-----------------------------------------------------------------------//
	// Section: wxStrings
	//
    const wxString swACities3D = wxT("a_cities3d");
    const wxString swActions = wxT("Actions");
    const wxString swCities3D = wxT("Cities3D");
    const wxString swCitiesOnline = wxT("Cities Online");
	const wxString swCitiesOnlineNoSpace = wxT("CitiesOnline");
    const wxString swCitiesOnlineRules = wxT("CitiesOnlineRules");
    const wxString swConfigSectionGeneral = wxT("ConfigSectionGeneral");
    const wxString swConfigSectionUI = wxT("ConfigSectionUI");
    const wxString swCore = wxT("Core");
    const wxString swCoreCities = wxT("CoreCities");
    const wxString swCoreCitiesLayout = wxT("layout2|name=SettlersView;caption=Game Window;state=1984;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=800;besth=600;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Cities;caption=Cities and Knights;state=2044;dir=2;layer=1;row=0;pos=1;prop=164771;bestw=165;besth=150;minw=165;minh=150;maxw=-1;maxh=-1;floatx=629;floaty=181;floatw=400;floath=250|name=Status;caption=Status;state=2044;dir=2;layer=1;row=0;pos=0;prop=35229;bestw=173;besth=48;minw=173;minh=48;maxw=-1;maxh=-1;floatx=169;floaty=612;floatw=400;floath=250|name=Controls;caption=Controls;state=2044;dir=1;layer=0;row=1;pos=0;prop=100000;bestw=-1;besth=-1;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=390;floaty=602;floatw=400;floath=250|name=BuildOptions;caption=Build Options;state=2045;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=418;besth=106;minw=418;minh=106;maxw=-1;maxh=-1;floatx=495;floaty=0;floatw=426;floath=132|name=Players;caption=Players;state=2044;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=227;besth=283;minw=227;minh=283;maxw=-1;maxh=-1;floatx=157;floaty=176;floatw=400;floath=309|name=Bank;caption=Bank;state=2044;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=304;besth=20;minw=304;minh=20;maxw=-1;maxh=-1;floatx=765;floaty=341;floatw=312;floath=46|dock_size(5,0,0)=22|dock_size(2,1,0)=175|dock_size(4,1,0)=229|dock_size(1,0,1)=41|dock_size(3,0,1)=39|");
    const wxString swCoreCitiesOnlineCities = wxT("CoreCitiesOnlineCities");
    const wxString swCoreCitiesOnlineCitiesLayout = wxT("layout2|name=SettlersView;caption=Game Window;state=1984;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=800;besth=600;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=CitiesOnline;caption=Cities Online;state=2044;dir=2;layer=1;row=0;pos=1;prop=51838;bestw=0;besth=0;minw=0;minh=0;maxw=-1;maxh=-1;floatx=1372;floaty=447;floatw=400;floath=250|name=Cities;caption=Cities and Knights;state=2044;dir=2;layer=1;row=0;pos=1;prop=164771;bestw=165;besth=150;minw=165;minh=150;maxw=-1;maxh=-1;floatx=629;floaty=181;floatw=400;floath=250|name=Status;caption=Status;state=2044;dir=2;layer=1;row=0;pos=0;prop=35229;bestw=173;besth=48;minw=173;minh=48;maxw=-1;maxh=-1;floatx=169;floaty=612;floatw=400;floath=250|name=Controls;caption=Controls;state=2044;dir=1;layer=0;row=1;pos=0;prop=100000;bestw=-1;besth=-1;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=390;floaty=602;floatw=400;floath=250|name=BuildOptions;caption=Build Options;state=2045;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=418;besth=106;minw=418;minh=106;maxw=-1;maxh=-1;floatx=495;floaty=0;floatw=426;floath=132|name=Players;caption=Players;state=2044;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=227;besth=283;minw=227;minh=283;maxw=-1;maxh=-1;floatx=157;floaty=176;floatw=400;floath=309|name=Bank;caption=Bank;state=2044;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=304;besth=20;minw=304;minh=20;maxw=-1;maxh=-1;floatx=765;floaty=341;floatw=312;floath=46|dock_size(5,0,0)=22|dock_size(2,1,0)=175|dock_size(4,1,0)=229|dock_size(1,0,1)=41|dock_size(3,0,1)=39|");
    const wxString swCoreCitiesOnlineCitiesVolcano = wxT("CoreCitiesOnlineCitiesVolcano");
    const wxString swCoreCitiesOnlineCitiesVolcanoLayout = wxT("layout2|name=SettlersView;caption=Game Window;state=1984;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=800;besth=600;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=CitiesOnline;caption=Cities Online;state=2044;dir=2;layer=1;row=0;pos=1;prop=51838;bestw=0;besth=0;minw=0;minh=0;maxw=-1;maxh=-1;floatx=1372;floaty=447;floatw=400;floath=250|name=Cities;caption=Cities and Knights;state=2044;dir=2;layer=1;row=0;pos=1;prop=164771;bestw=165;besth=150;minw=165;minh=150;maxw=-1;maxh=-1;floatx=629;floaty=181;floatw=400;floath=250|name=Status;caption=Status;state=2044;dir=2;layer=1;row=0;pos=0;prop=35229;bestw=173;besth=48;minw=173;minh=48;maxw=-1;maxh=-1;floatx=169;floaty=612;floatw=400;floath=250|name=Controls;caption=Controls;state=2044;dir=1;layer=0;row=1;pos=0;prop=100000;bestw=-1;besth=-1;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=390;floaty=602;floatw=400;floath=250|name=BuildOptions;caption=Build Options;state=2045;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=418;besth=106;minw=418;minh=106;maxw=-1;maxh=-1;floatx=495;floaty=0;floatw=426;floath=132|name=Players;caption=Players;state=2044;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=227;besth=283;minw=227;minh=283;maxw=-1;maxh=-1;floatx=157;floaty=176;floatw=400;floath=309|name=Bank;caption=Bank;state=2044;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=304;besth=20;minw=304;minh=20;maxw=-1;maxh=-1;floatx=765;floaty=341;floatw=312;floath=46|dock_size(5,0,0)=22|dock_size(2,1,0)=175|dock_size(4,1,0)=229|dock_size(1,0,1)=41|dock_size(3,0,1)=39|");
    const wxString swCoreCitiesOnlineDevCards = wxT("CoreCitiesOnlineDevCards");
    const wxString swCoreCitiesOnlineDevCardsLayout = wxT("layout2|name=SettlersView;caption=Game Window;state=1984;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=800;besth=600;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=CitiesOnline;caption=Cities Online;state=2044;dir=2;layer=1;row=0;pos=1;prop=51838;bestw=0;besth=0;minw=0;minh=0;maxw=-1;maxh=-1;floatx=1372;floaty=447;floatw=400;floath=250|name=Status;caption=Status;state=2044;dir=2;layer=1;row=0;pos=0;prop=29724;bestw=173;besth=48;minw=173;minh=48;maxw=-1;maxh=-1;floatx=574;floaty=359;floatw=400;floath=250|name=Controls;caption=Controls;state=2044;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=-1;besth=-1;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=764;floaty=135;floatw=400;floath=250|name=BuildOptions;caption=Build Options;state=2044;dir=4;layer=1;row=0;pos=0;prop=27522;bestw=210;besth=54;minw=210;minh=54;maxw=-1;maxh=-1;floatx=6;floaty=19;floatw=218;floath=80|name=Players;caption=Players;state=2044;dir=4;layer=1;row=0;pos=1;prop=172478;bestw=187;besth=377;minw=187;minh=377;maxw=-1;maxh=-1;floatx=-11;floaty=316;floatw=400;floath=403|name=Bank;caption=Bank;state=2044;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=228;besth=20;minw=228;minh=20;maxw=-1;maxh=-1;floatx=749;floaty=384;floatw=236;floath=46|name=DiceRolls;caption=Dice Rolls;state=2044;dir=2;layer=1;row=0;pos=1;prop=170276;bestw=165;besth=150;minw=165;minh=150;maxw=-1;maxh=-1;floatx=790;floaty=209;floatw=400;floath=250|dock_size(5,0,0)=22|dock_size(1,0,0)=42|dock_size(4,1,0)=212|dock_size(2,1,0)=175|dock_size(3,0,1)=39|");
    const wxString swCoreCitiesOnlineDevCardsVolcano = wxT("CoreCitiesOnlineDevCardsVolcano");
    const wxString swCoreCitiesOnlineDevCardsVolcanoLayout = wxT("layout2|name=SettlersView;caption=Game Window;state=1984;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=800;besth=600;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=CitiesOnline;caption=Cities Online;state=2044;dir=2;layer=1;row=0;pos=1;prop=51838;bestw=0;besth=0;minw=0;minh=0;maxw=-1;maxh=-1;floatx=1372;floaty=447;floatw=400;floath=250|name=Status;caption=Status;state=2044;dir=2;layer=1;row=0;pos=0;prop=29724;bestw=173;besth=48;minw=173;minh=48;maxw=-1;maxh=-1;floatx=574;floaty=359;floatw=400;floath=250|name=Controls;caption=Controls;state=2044;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=-1;besth=-1;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=764;floaty=135;floatw=400;floath=250|name=BuildOptions;caption=Build Options;state=2044;dir=4;layer=1;row=0;pos=0;prop=27522;bestw=210;besth=54;minw=210;minh=54;maxw=-1;maxh=-1;floatx=6;floaty=19;floatw=218;floath=80|name=Players;caption=Players;state=2044;dir=4;layer=1;row=0;pos=1;prop=172478;bestw=187;besth=377;minw=187;minh=377;maxw=-1;maxh=-1;floatx=-11;floaty=316;floatw=400;floath=403|name=Bank;caption=Bank;state=2044;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=228;besth=20;minw=228;minh=20;maxw=-1;maxh=-1;floatx=376;floaty=87;floatw=236;floath=46|name=DiceRolls;caption=Dice Rolls;state=2044;dir=2;layer=1;row=0;pos=1;prop=170276;bestw=165;besth=150;minw=165;minh=150;maxw=-1;maxh=-1;floatx=790;floaty=209;floatw=400;floath=250|dock_size(5,0,0)=22|dock_size(1,0,0)=42|dock_size(4,1,0)=212|dock_size(2,1,0)=175|dock_size(3,0,1)=39|");
    const wxString swCoreCitiesOnlineNetworkCities = wxT("CoreCitiesOnlineNetworkCities");
    const wxString swCoreCitiesOnlineNetworkCitiesLayout = wxT("layout2|name=SettlersView;caption=Game Window;state=1984;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=800;besth=600;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=CitiesOnline;caption=Cities Online;state=2044;dir=2;layer=1;row=0;pos=1;prop=51838;bestw=0;besth=0;minw=0;minh=0;maxw=-1;maxh=-1;floatx=1372;floaty=447;floatw=400;floath=250|name=Cities;caption=Cities and Knights;state=2044;dir=2;layer=1;row=0;pos=1;prop=127941;bestw=165;besth=150;minw=165;minh=150;maxw=-1;maxh=-1;floatx=629;floaty=181;floatw=400;floath=250|name=Status;caption=Status;state=2044;dir=2;layer=1;row=0;pos=0;prop=55147;bestw=173;besth=48;minw=173;minh=48;maxw=-1;maxh=-1;floatx=169;floaty=612;floatw=400;floath=250|name=Controls;caption=Controls;state=2044;dir=1;layer=0;row=1;pos=0;prop=100000;bestw=-1;besth=-1;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=390;floaty=602;floatw=400;floath=250|name=BuildOptions;caption=Build Options;state=2045;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=418;besth=106;minw=418;minh=106;maxw=-1;maxh=-1;floatx=495;floaty=0;floatw=426;floath=132|name=Players;caption=Players;state=2044;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=227;besth=283;minw=227;minh=283;maxw=-1;maxh=-1;floatx=157;floaty=176;floatw=400;floath=309|name=Bank;caption=Bank;state=2044;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=304;besth=20;minw=304;minh=20;maxw=-1;maxh=-1;floatx=765;floaty=341;floatw=312;floath=46|name=Network;caption=Network;state=2044;dir=2;layer=1;row=0;pos=2;prop=116912;bestw=165;besth=150;minw=165;minh=150;maxw=-1;maxh=-1;floatx=617;floaty=225;floatw=400;floath=250|dock_size(5,0,0)=22|dock_size(2,1,0)=175|dock_size(4,1,0)=229|dock_size(1,0,1)=41|dock_size(3,0,1)=39|");
    const wxString swCoreCitiesOnlineNetworkCitiesVolcano = wxT("CoreCitiesOnlineNetworkCitiesVolcano");
    const wxString swCoreCitiesOnlineNetworkCitiesVolcanoLayout = wxT("layout2|name=SettlersView;caption=Game Window;state=1984;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=800;besth=600;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=CitiesOnline;caption=Cities Online;state=2044;dir=2;layer=1;row=0;pos=1;prop=51838;bestw=0;besth=0;minw=0;minh=0;maxw=-1;maxh=-1;floatx=1372;floaty=447;floatw=400;floath=250|name=Cities;caption=Cities and Knights;state=2044;dir=2;layer=1;row=0;pos=1;prop=127941;bestw=165;besth=150;minw=165;minh=150;maxw=-1;maxh=-1;floatx=629;floaty=181;floatw=400;floath=250|name=Status;caption=Status;state=2044;dir=2;layer=1;row=0;pos=0;prop=55147;bestw=173;besth=48;minw=173;minh=48;maxw=-1;maxh=-1;floatx=169;floaty=612;floatw=400;floath=250|name=Controls;caption=Controls;state=2044;dir=1;layer=0;row=1;pos=0;prop=100000;bestw=-1;besth=-1;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=390;floaty=602;floatw=400;floath=250|name=BuildOptions;caption=Build Options;state=2045;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=418;besth=106;minw=418;minh=106;maxw=-1;maxh=-1;floatx=495;floaty=0;floatw=426;floath=132|name=Players;caption=Players;state=2044;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=227;besth=283;minw=227;minh=283;maxw=-1;maxh=-1;floatx=157;floaty=176;floatw=400;floath=309|name=Bank;caption=Bank;state=2044;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=304;besth=20;minw=304;minh=20;maxw=-1;maxh=-1;floatx=765;floaty=341;floatw=312;floath=46|name=Network;caption=Network;state=2044;dir=2;layer=1;row=0;pos=2;prop=116912;bestw=165;besth=150;minw=165;minh=150;maxw=-1;maxh=-1;floatx=617;floaty=225;floatw=400;floath=250|dock_size(5,0,0)=22|dock_size(2,1,0)=175|dock_size(4,1,0)=229|dock_size(1,0,1)=41|dock_size(3,0,1)=39|");
    const wxString swCoreCitiesOnlineNetworkDevCards = wxT("CoreCitiesOnlineNetworkDevCards");
    const wxString swCoreCitiesOnlineNetworkDevCardsLayout = wxT("layout2|name=SettlersView;caption=Game Window;state=1984;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=800;besth=600;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=CitiesOnline;caption=Cities Online;state=2044;dir=2;layer=1;row=0;pos=1;prop=51838;bestw=0;besth=0;minw=0;minh=0;maxw=-1;maxh=-1;floatx=1372;floaty=447;floatw=400;floath=250|name=Status;caption=Status;state=2044;dir=2;layer=1;row=0;pos=0;prop=29724;bestw=173;besth=48;minw=173;minh=48;maxw=-1;maxh=-1;floatx=574;floaty=359;floatw=400;floath=250|name=Controls;caption=Controls;state=2044;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=-1;besth=-1;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=764;floaty=135;floatw=400;floath=250|name=BuildOptions;caption=Build Options;state=2044;dir=4;layer=1;row=0;pos=0;prop=27522;bestw=210;besth=54;minw=210;minh=54;maxw=-1;maxh=-1;floatx=6;floaty=19;floatw=218;floath=80|name=Players;caption=Players;state=2044;dir=4;layer=1;row=0;pos=1;prop=172478;bestw=187;besth=377;minw=187;minh=377;maxw=-1;maxh=-1;floatx=-11;floaty=316;floatw=400;floath=403|name=Bank;caption=Bank;state=2044;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=228;besth=20;minw=228;minh=20;maxw=-1;maxh=-1;floatx=376;floaty=87;floatw=236;floath=46|name=Network;caption=Network;state=2044;dir=2;layer=1;row=0;pos=2;prop=118077;bestw=165;besth=150;minw=165;minh=150;maxw=-1;maxh=-1;floatx=615;floaty=190;floatw=400;floath=250|name=DiceRolls;caption=Dice Rolls;state=2044;dir=2;layer=1;row=0;pos=1;prop=81923;bestw=165;besth=150;minw=165;minh=150;maxw=-1;maxh=-1;floatx=620;floaty=424;floatw=400;floath=250|dock_size(5,0,0)=22|dock_size(1,0,0)=42|dock_size(4,1,0)=212|dock_size(2,1,0)=175|dock_size(3,0,1)=39|");
    const wxString swCoreCitiesOnlineNetworkDevCardsVolcano = wxT("CoreCitiesOnlineNetworkDevCardsVolcano");
    const wxString swCoreCitiesOnlineNetworkDevCardsVolcanoLayout = wxT("layout2|name=SettlersView;caption=Game Window;state=1984;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=800;besth=600;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=CitiesOnline;caption=Cities Online;state=2044;dir=2;layer=1;row=0;pos=1;prop=51838;bestw=0;besth=0;minw=0;minh=0;maxw=-1;maxh=-1;floatx=1372;floaty=447;floatw=400;floath=250|name=Status;caption=Status;state=2044;dir=2;layer=1;row=0;pos=0;prop=29724;bestw=173;besth=48;minw=173;minh=48;maxw=-1;maxh=-1;floatx=574;floaty=359;floatw=400;floath=250|name=Controls;caption=Controls;state=2044;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=-1;besth=-1;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=764;floaty=135;floatw=400;floath=250|name=BuildOptions;caption=Build Options;state=2044;dir=4;layer=1;row=0;pos=0;prop=27522;bestw=210;besth=54;minw=210;minh=54;maxw=-1;maxh=-1;floatx=6;floaty=19;floatw=218;floath=80|name=Players;caption=Players;state=2044;dir=4;layer=1;row=0;pos=1;prop=172478;bestw=187;besth=377;minw=187;minh=377;maxw=-1;maxh=-1;floatx=-11;floaty=316;floatw=400;floath=403|name=Bank;caption=Bank;state=2044;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=228;besth=20;minw=228;minh=20;maxw=-1;maxh=-1;floatx=376;floaty=87;floatw=236;floath=46|name=Network;caption=Network;state=2044;dir=2;layer=1;row=0;pos=2;prop=118077;bestw=165;besth=150;minw=165;minh=150;maxw=-1;maxh=-1;floatx=615;floaty=190;floatw=400;floath=250|name=DiceRolls;caption=Dice Rolls;state=2044;dir=2;layer=1;row=0;pos=1;prop=81923;bestw=165;besth=150;minw=165;minh=150;maxw=-1;maxh=-1;floatx=620;floaty=424;floatw=400;floath=250|dock_size(5,0,0)=22|dock_size(1,0,0)=42|dock_size(4,1,0)=212|dock_size(2,1,0)=175|dock_size(3,0,1)=39|");
    const wxString swCoreCitiesVolcano = wxT("CoreCitiesVolcano");
    const wxString swCoreCitiesVolcanoLayout = wxT("layout2|name=SettlersView;caption=Game Window;state=1984;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=800;besth=600;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Cities;caption=Cities and Knights;state=2044;dir=2;layer=1;row=0;pos=1;prop=164771;bestw=165;besth=150;minw=165;minh=150;maxw=-1;maxh=-1;floatx=629;floaty=181;floatw=400;floath=250|name=Status;caption=Status;state=2044;dir=2;layer=1;row=0;pos=0;prop=35229;bestw=173;besth=48;minw=173;minh=48;maxw=-1;maxh=-1;floatx=169;floaty=612;floatw=400;floath=250|name=Controls;caption=Controls;state=2044;dir=1;layer=0;row=1;pos=0;prop=100000;bestw=-1;besth=-1;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=390;floaty=602;floatw=400;floath=250|name=BuildOptions;caption=Build Options;state=2045;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=418;besth=106;minw=418;minh=106;maxw=-1;maxh=-1;floatx=495;floaty=0;floatw=426;floath=132|name=Players;caption=Players;state=2044;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=227;besth=283;minw=227;minh=283;maxw=-1;maxh=-1;floatx=157;floaty=176;floatw=400;floath=309|name=Bank;caption=Bank;state=2044;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=304;besth=20;minw=304;minh=20;maxw=-1;maxh=-1;floatx=765;floaty=341;floatw=312;floath=46|dock_size(5,0,0)=22|dock_size(2,1,0)=175|dock_size(4,1,0)=229|dock_size(1,0,1)=41|dock_size(3,0,1)=39|");
    const wxString swCoreDevCards = wxT("CoreDevCards");
    const wxString swCoreDevCardsLayout = wxT("layout2|name=SettlersView;caption=Game Window;state=1984;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=800;besth=600;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Status;caption=Status;state=2044;dir=2;layer=1;row=0;pos=0;prop=29724;bestw=173;besth=48;minw=173;minh=48;maxw=-1;maxh=-1;floatx=574;floaty=359;floatw=400;floath=250|name=Controls;caption=Controls;state=2044;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=-1;besth=-1;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=764;floaty=135;floatw=400;floath=250|name=BuildOptions;caption=Build Options;state=2044;dir=4;layer=1;row=0;pos=0;prop=27522;bestw=210;besth=54;minw=210;minh=54;maxw=-1;maxh=-1;floatx=6;floaty=19;floatw=218;floath=80|name=Players;caption=Players;state=2044;dir=4;layer=1;row=0;pos=1;prop=172478;bestw=187;besth=377;minw=187;minh=377;maxw=-1;maxh=-1;floatx=-11;floaty=316;floatw=400;floath=403|name=Bank;caption=Bank;state=2044;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=228;besth=20;minw=228;minh=20;maxw=-1;maxh=-1;floatx=749;floaty=384;floatw=236;floath=46|name=DiceRolls;caption=Dice Rolls;state=2044;dir=2;layer=1;row=0;pos=1;prop=170276;bestw=165;besth=150;minw=165;minh=150;maxw=-1;maxh=-1;floatx=790;floaty=209;floatw=400;floath=250|dock_size(5,0,0)=22|dock_size(1,0,0)=42|dock_size(4,1,0)=212|dock_size(2,1,0)=175|dock_size(3,0,1)=39|");
    const wxString swCoreDevCardsVolcano = wxT("CoreDevCardsVolcano");
    const wxString swCoreDevCardsVolcanoLayout = wxT("layout2|name=SettlersView;caption=Game Window;state=1984;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=800;besth=600;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Status;caption=Status;state=2044;dir=2;layer=1;row=0;pos=0;prop=29724;bestw=173;besth=48;minw=173;minh=48;maxw=-1;maxh=-1;floatx=574;floaty=359;floatw=400;floath=250|name=Controls;caption=Controls;state=2044;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=-1;besth=-1;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=764;floaty=135;floatw=400;floath=250|name=BuildOptions;caption=Build Options;state=2044;dir=4;layer=1;row=0;pos=0;prop=27522;bestw=210;besth=54;minw=210;minh=54;maxw=-1;maxh=-1;floatx=6;floaty=19;floatw=218;floath=80|name=Players;caption=Players;state=2044;dir=4;layer=1;row=0;pos=1;prop=172478;bestw=187;besth=377;minw=187;minh=377;maxw=-1;maxh=-1;floatx=-11;floaty=316;floatw=400;floath=403|name=Bank;caption=Bank;state=2044;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=228;besth=20;minw=228;minh=20;maxw=-1;maxh=-1;floatx=376;floaty=87;floatw=236;floath=46|name=DiceRolls;caption=Dice Rolls;state=2044;dir=2;layer=1;row=0;pos=1;prop=170276;bestw=165;besth=150;minw=165;minh=150;maxw=-1;maxh=-1;floatx=790;floaty=209;floatw=400;floath=250|dock_size(5,0,0)=22|dock_size(1,0,0)=42|dock_size(4,1,0)=212|dock_size(2,1,0)=175|dock_size(3,0,1)=39|");
    const wxString swCoreRules = wxT("CoreRules");
    const wxString swDefault = wxT("Default");
    const wxString swEditor = wxT("Editor");
	const wxString swEmpty = wxT("");
    const wxString swEnglishUS = wxT("English (U.S.)");
    const wxString swExit = wxT("Exit");
    const wxString swForwardSlash = wxT("/");
    const wxString swGtkrc = wxT("gtkrc");
    const wxString swLanguage = wxT("Language");
    const wxString swLayout = wxT("Layout");
    const wxString swLayouts = wxT("Layouts");
    const wxString swMainFrameHeight = wxT("Main Frame/height");
    const wxString swMainFrameMaximized = wxT("Main Frame/Maximized");
    const wxString swMainFrameWidth = wxT("Main Frame/width");
    const wxString swName = wxT("Name");
    const wxString swCoreNetworkCities = wxT("CoreNetworkCities");
    const wxString swCoreNetworkCitiesLayout = wxT("layout2|name=SettlersView;caption=Game Window;state=1984;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=800;besth=600;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Cities;caption=Cities and Knights;state=2044;dir=2;layer=1;row=0;pos=1;prop=127941;bestw=165;besth=150;minw=165;minh=150;maxw=-1;maxh=-1;floatx=629;floaty=181;floatw=400;floath=250|name=Status;caption=Status;state=2044;dir=2;layer=1;row=0;pos=0;prop=55147;bestw=173;besth=48;minw=173;minh=48;maxw=-1;maxh=-1;floatx=169;floaty=612;floatw=400;floath=250|name=Controls;caption=Controls;state=2044;dir=1;layer=0;row=1;pos=0;prop=100000;bestw=-1;besth=-1;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=390;floaty=602;floatw=400;floath=250|name=BuildOptions;caption=Build Options;state=2045;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=418;besth=106;minw=418;minh=106;maxw=-1;maxh=-1;floatx=495;floaty=0;floatw=426;floath=132|name=Players;caption=Players;state=2044;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=227;besth=283;minw=227;minh=283;maxw=-1;maxh=-1;floatx=157;floaty=176;floatw=400;floath=309|name=Bank;caption=Bank;state=2044;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=304;besth=20;minw=304;minh=20;maxw=-1;maxh=-1;floatx=765;floaty=341;floatw=312;floath=46|name=Network;caption=Network;state=2044;dir=2;layer=1;row=0;pos=2;prop=116912;bestw=165;besth=150;minw=165;minh=150;maxw=-1;maxh=-1;floatx=617;floaty=225;floatw=400;floath=250|dock_size(5,0,0)=22|dock_size(2,1,0)=175|dock_size(4,1,0)=229|dock_size(1,0,1)=41|dock_size(3,0,1)=39|");
    const wxString swCoreNetworkCitiesVolcano = wxT("CoreNetworkCitiesVolcano");
    const wxString swCoreNetworkCitiesVolcanoLayout = wxT("layout2|name=SettlersView;caption=Game Window;state=1984;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=800;besth=600;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Cities;caption=Cities and Knights;state=2044;dir=2;layer=1;row=0;pos=1;prop=127941;bestw=165;besth=150;minw=165;minh=150;maxw=-1;maxh=-1;floatx=629;floaty=181;floatw=400;floath=250|name=Status;caption=Status;state=2044;dir=2;layer=1;row=0;pos=0;prop=55147;bestw=173;besth=48;minw=173;minh=48;maxw=-1;maxh=-1;floatx=169;floaty=612;floatw=400;floath=250|name=Controls;caption=Controls;state=2044;dir=1;layer=0;row=1;pos=0;prop=100000;bestw=-1;besth=-1;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=390;floaty=602;floatw=400;floath=250|name=BuildOptions;caption=Build Options;state=2045;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=418;besth=106;minw=418;minh=106;maxw=-1;maxh=-1;floatx=495;floaty=0;floatw=426;floath=132|name=Players;caption=Players;state=2044;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=227;besth=283;minw=227;minh=283;maxw=-1;maxh=-1;floatx=157;floaty=176;floatw=400;floath=309|name=Bank;caption=Bank;state=2044;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=304;besth=20;minw=304;minh=20;maxw=-1;maxh=-1;floatx=765;floaty=341;floatw=312;floath=46|name=Network;caption=Network;state=2044;dir=2;layer=1;row=0;pos=2;prop=116912;bestw=165;besth=150;minw=165;minh=150;maxw=-1;maxh=-1;floatx=617;floaty=225;floatw=400;floath=250|dock_size(5,0,0)=22|dock_size(2,1,0)=175|dock_size(4,1,0)=229|dock_size(1,0,1)=41|dock_size(3,0,1)=39|");
    const wxString swCoreNetworkDevCards = wxT("CoreNetworkDevCards");
    const wxString swCoreNetworkDevCardsLayout = wxT("layout2|name=SettlersView;caption=Game Window;state=1984;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=800;besth=600;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Status;caption=Status;state=2044;dir=2;layer=1;row=0;pos=0;prop=29724;bestw=173;besth=48;minw=173;minh=48;maxw=-1;maxh=-1;floatx=574;floaty=359;floatw=400;floath=250|name=Controls;caption=Controls;state=2044;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=-1;besth=-1;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=764;floaty=135;floatw=400;floath=250|name=BuildOptions;caption=Build Options;state=2044;dir=4;layer=1;row=0;pos=0;prop=27522;bestw=210;besth=54;minw=210;minh=54;maxw=-1;maxh=-1;floatx=6;floaty=19;floatw=218;floath=80|name=Players;caption=Players;state=2044;dir=4;layer=1;row=0;pos=1;prop=172478;bestw=187;besth=377;minw=187;minh=377;maxw=-1;maxh=-1;floatx=-11;floaty=316;floatw=400;floath=403|name=Bank;caption=Bank;state=2044;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=228;besth=20;minw=228;minh=20;maxw=-1;maxh=-1;floatx=376;floaty=87;floatw=236;floath=46|name=Network;caption=Network;state=2044;dir=2;layer=1;row=0;pos=2;prop=118077;bestw=165;besth=150;minw=165;minh=150;maxw=-1;maxh=-1;floatx=615;floaty=190;floatw=400;floath=250|name=DiceRolls;caption=Dice Rolls;state=2044;dir=2;layer=1;row=0;pos=1;prop=81923;bestw=165;besth=150;minw=165;minh=150;maxw=-1;maxh=-1;floatx=620;floaty=424;floatw=400;floath=250|dock_size(5,0,0)=22|dock_size(1,0,0)=42|dock_size(4,1,0)=212|dock_size(2,1,0)=175|dock_size(3,0,1)=39|");
    const wxString swCoreNetworkDevCardsVolcano = wxT("CoreNetworkDevCardsVolcano");
    const wxString swCoreNetworkDevCardsVolcanoLayout = wxT("layout2|name=SettlersView;caption=Game Window;state=1984;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=800;besth=600;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Status;caption=Status;state=2044;dir=2;layer=1;row=0;pos=0;prop=29724;bestw=173;besth=48;minw=173;minh=48;maxw=-1;maxh=-1;floatx=574;floaty=359;floatw=400;floath=250|name=Controls;caption=Controls;state=2044;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=-1;besth=-1;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=764;floaty=135;floatw=400;floath=250|name=BuildOptions;caption=Build Options;state=2044;dir=4;layer=1;row=0;pos=0;prop=27522;bestw=210;besth=54;minw=210;minh=54;maxw=-1;maxh=-1;floatx=6;floaty=19;floatw=218;floath=80|name=Players;caption=Players;state=2044;dir=4;layer=1;row=0;pos=1;prop=172478;bestw=187;besth=377;minw=187;minh=377;maxw=-1;maxh=-1;floatx=-11;floaty=316;floatw=400;floath=403|name=Bank;caption=Bank;state=2044;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=228;besth=20;minw=228;minh=20;maxw=-1;maxh=-1;floatx=376;floaty=87;floatw=236;floath=46|name=Network;caption=Network;state=2044;dir=2;layer=1;row=0;pos=2;prop=118077;bestw=165;besth=150;minw=165;minh=150;maxw=-1;maxh=-1;floatx=615;floaty=190;floatw=400;floath=250|name=DiceRolls;caption=Dice Rolls;state=2044;dir=2;layer=1;row=0;pos=1;prop=81923;bestw=165;besth=150;minw=165;minh=150;maxw=-1;maxh=-1;floatx=620;floaty=424;floatw=400;floath=250|dock_size(5,0,0)=22|dock_size(1,0,0)=42|dock_size(4,1,0)=212|dock_size(2,1,0)=175|dock_size(3,0,1)=39|");
    const wxString swPlay = wxT("Play");
    const wxString swSelectALanguage = wxT("Select a Language");
    const wxString swSettings = wxT("Settings");
    const wxString swSpace = wxT(" ");
    const wxString swUISpashTex = wxT("uisplash.tex");
    const wxString swXml = wxT(".xml");

    // Formats
    const wxString swFloat6 = wxT("%.6f");
    const wxString swIntFormat = wxT("%d");
    const wxString swSlashStringSlashString = wxT("/%s/%s");
	
    //-----------------------------------------------------------------------//
	// Section: Translations
	//
    
    // Formats
}

//---------------------------- PUBLIC           -----------------------------//

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
