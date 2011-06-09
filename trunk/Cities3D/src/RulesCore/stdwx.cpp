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
// Topic: RulesCore
//
// RulesCore is the HOSS of Cities3D.  It is the shiznit, teh hotness, the 
// THANG, the Module of Mighty Might (TM) that makes everything go, go, go!  
// RulesCore contains the game engine, the rules, the logic, the selection, the 
// game access,and everything else that *does* something in the game.  It's the
// 350-HP V-10 engine underneath the fiberglass Cities3D shell.  Without it,
// nothing would happen.  Cities3D would sit like a bloated whale carcass on 
// empty digital shores.
//
// The focal point of the whole operation is the <GameEngine>.  The 
// <GameEngine> does what it's named.  It runs the game.  It holds the current 
// <Game> data being used, as well as the current <Map> that is being played 
// on.  It is also a repository of sub-engines that handle various other 
// responsibilities.  The <DrawObjectEngine> manages the <IDrawObjects> that 
// draw things on the board.  The <ImageEngine> stores all the <ImageSets> used
// for UI bitmaps and OpenGL textures.  The <PluginEngine> handles loading all 
// of the data from plugin modules.  The <RuleEngine> keeps track of all 
// <IRules>, <ILogic>, rule and logic mixins, and branching logic, so the game 
// actually goes!  And finally, the <SoundEngine> encapsulates the underlying 
// sound playing mechanism and plays sounds when requested.  Every object in 
// the game has access to the <GameEngine> because it's a <Singleton>-derived 
// class.  Every object also has access to the sub-engines because <GameEngine> 
// provides an interface function for each of them, as well as handy macros 
// that let developers avoid having to write out GameEngine::get().blahblah() 
// every time they want to access a sub-engine.
//
// Now you may be wondering just how the hell all the plugin module stuff 
// works. Well, hot damn, you found the answer!  The <PluginEngine>, when 
// started up by the <GameEngine>, looks through the /plugins subdirectory and 
// finds all plugin modules that provide <RuleSets>.  It then keeps track
// of these internally.  Whenever a new game is started, the <Map> data in 
// question has a list of any <RuleSets> that it requires.  Users can also 
// select extra <RuleSet> variants that they want to play with.  When the 
// <PluginEngine> is so instructed, it loads the requested <RuleSets>, out of 
// the plugin module they are found in, into the <RuleEngine>, <ImageEngine>, 
// and <DrawObjectEngine>.  Each <RuleSet> provides a whole mess o' data that 
// gets dropped into the separate buckets that keep track of it all.  Once all 
// the <RuleSet> data is in the places it needs to be, the game starts off, and
// then <RuleEngine> steals the spotlight.
//
// Everything that actually happens in the flow of the game happens through the
// <RuleEngine>.  The <RuleEngine> has internal hash maps of <IRules> and 
// <ILogic> which are the real rules and logic that make up game play.  Nothing
// happens in a game that does not happen through an <IRule> and any associated
// <ILogic>.  When the user clicks to roll the dice, an <IRule> is called in 
// the <RuleEngine>.  When the user selects an object in OpenGL, an <IRule> is 
// called in the <RuleEngine>.  When the user sends a chat message to everyone 
// else, an <IRule> is called in the <RuleEngine>.  You get the picture.  Every
// <IRule> is stored in its parent <RuleSet> in its plugin module with a string
// as its rule name.  That string is how an <IRule> is called.  By using 
// strings instead of actual objects, complete and total decoupling is 
// achieved between the View and the Model.  The View knows absolutely nothing 
// about <IRules>.  It just knows that the <GameEngine> has a <RuleEngine> that
// does cool and awesome stuff, if it just calls the little Execute function 
// with the right names and stuff.
//
// There are a whole lot of other classes in RulesCore.  This Overview barely
// scratches the surface of all that's going on.  Click on any of the links to 
// the left to drill down and get a finer picture of what's going on.
//
// Classes:
//     <Access>,
//     <BankData>,
//     <BaseMixin>,
//     <BuildData>,
//     <CornerSelectionObject>,
//     <DataObject>,
//     <DrawObjectEngine>,
//     <GameEngine>,
//     <GameObjectEngine>,
//     <HexSelectionObject>,
//     <IDrawObject>,
//     <IDrawObjectEngine>,
//     <IImageEngine>,
//     <ImageEngine>,
//     <ImageSet>,
//     <IPluginEngine>,
//     <IRule>,
//     <IRuleEngine>,
//     <ISoundEngine>,
//     <IUndoable>,
//     <Logic>,
//     <LogicBooleanMixin>,
//     <LogicBranchDefault>,
//     <PlayerData>,
//     <PluginEngine>,
//     <Rule>,
//     <RuleBase>,
//     <RuleExtensionMixin>,
//     <RuleEngine>,
//     <RuleHashMixin>,
//     <RuleSet>,
//     <SelectionObject>,
//     <SideSelectionObject>,
//     <SoundEngine>,
//
// Depends On:
//     <DatabaseCore>,
//     <ModelCore>
