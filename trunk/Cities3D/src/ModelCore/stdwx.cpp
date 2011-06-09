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

//---------------------------------------------------------------------------//
// Topic: ModelCore
//
// ModelCore provides all of the data classes and associated helper classes
// and functions required to represent a complete game.  The base class for
// all game data classes is the <Data> class, which provides string-based hash
// maps for easily extensible data fields.
//
// Built on top of <Data> are several logically separated classes.  The <Map>
// class is the data center for a single map, whether the standard Settlers 
// maps or any of the Seafarers maps.  There are 23 official maps in Cities3D,
// corresponding to the maps in the standard Settlers and Seafarers almanacs.
//
// There are two major logical divisions separating the data in a game.  
// First, there is data that is particular to the game as a whole, like the 
// board being played on, the number of points to play to, the number of cards 
// available, etc.  Then there is data that is particular to each player in the
// game, like individual points, built objects, current resource and other 
// cards, etc.  These logical divisions are implemented in the <Game> class, 
// which stores instances of the <PlayerGame> class for each player in the game.
// Finally, there is a simple <Player> class that allows player data to be 
// lodged in a <PlayerGame>, but also stored in the external <PlayerDatabase> 
// so that users can create, delete, and editplayers separately from using them
// in a game.
//
// All objects in the game that can be represented visually on the screen have 
// a common base interface class, <IGameObject>, which ensures that various 
// requirements are met, such as having draw flags, color, position and 
// rotation data.  A default implementation of <IGameObject> is provided in 
// <GameObject>, though not all game objects are required to use the default 
// implementation.  Three classes derived from <GameObject> are also provided 
// as base classes for all other objects. All objects in the game Settlers are 
// either built on the corners of three tiles, the sides of two tiles, or lay 
// on one hex directly.  The classes <CornerObject>, <SideObject>, and 
// <HexObject> provide a foundation for every <GameObject> in the game.  
// Finally, the <Vector> class provides a lightweight storage structure for 
// three dimensional position data.
//
// The most fundamental game object in the game is the tile.  All boards are 
// made up of many tiles, and the class <TileObject> holds all of the data for 
// one tile.  The board in a <Game> class is created from the user selected 
// <Map> class and results in a std::vector of <TileObjects> being created.  
// Tiles can also have separate chips and port objects laying on top of them, 
// which are handled by the <ChipObject> and <PortObject> classes respectively.
// Since both the <ChipObject> and <PortObject> classes have much in common, 
// they are derived from a common <BaseTiledObject> class.
//
// There are three separate Database classes which reference external data on 
// the disk, corresponding to saved <Game> data, stored <Player> data, and all 
// <Maps> that can be used in the game.  These classes are similarly named 
// <GameDatabase>, <PlayerDatabase>, and <MapDatabase>.  They are all derived 
// from the <Singleton> class, as there should always be one and only one means 
// of accessing the external databases.
//
// ModelCore also provides three seperate Utility namespaces that provide 
// miscellaneous functions for different aspects of the game.  These namespaces
// are <Utility>, <UtilityGL>, and <UtilityMap>.
//
// Finally, ModelCore also provides a communication mechanism between the data 
// classes and any higher level classes which may want to be notified of 
// changes to the data and update themselves according to the changes.  The 
// <Controller> class provides a completely generalized means of achieving 
// total object decoupling.  Any higher level class may add receivers to the 
// Controller by giving a string-based event name to listen to. The 
// <Controller> class keeps a map of <Event> objects, linking event names and 
// <Event> objects so that update events can be triggered by name, sending data
// points to all listening classes.  This way, the data classes know nothing at
// all about any higher level classes, and are simply repositories of data, 
// which they should be.  Whenever a new game object is created that must be 
// represented on the screen, the <AggregatorObject> class can be used to 
// aggregate all of the new game objects and send them via the <Controller> up 
// to the view classes which can listen for them and then create needed classes 
// to render the game objects on the screen.
//
// Classes:
//    <AggregatorObject>
//    <BaseTiledObject>
//    <ChipObject>
//    <Controller>
//    <CornerObject>
//    <Data>
//    <Event>
//    <IGameObject>
//    <IGameObjectEngine>
//    <Game>
//    <GameDatabase>
//    <GameObject>
//    <HexObject>
//    <Map>
//    <MapDatabase>
//    <Player>
//    <PlayerDatabase>
//    <PlayerGame>
//    <PortObject>
//    <SideObject>
//    <TileObject>
//    <Vector>
//
// Namespaces:
//    <Utility>
//    <UtilityGL>
//    <UtilityMap>
//
// Depends On:
//    <DatabaseCore>
