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
#pragma once

/*
THE CITIES3D C++ CODING STYLE GUIDE

INTRODUCTION

In any successful Open Source project, there are bound to be several developers
working on various parts of the system, and in order for the project to achieve
both correctness and maintainability, (the two most important ingredients for 
success), it is critical that the code itself be well documented, clear, and 
consistent.  Therefore, the purpose of this document is to describe the coding 
standard for this project (Cities3D) and its related modules, which may include 
(at some point in the future) Tex3D, Maps3D, and Server3D.

Please carefully read through this document *BEFORE* making any changes to the
existing code.  You may not agree with all of the standards this document lays 
down.  However, this project is not about you.  =P  It is about putting out a 
great game that many people will enjoy, and most developers who work on it will
probably have to sacrifice a few of their favorite, or at least, usual 
practices, since everyone has different standards.  Please follow the 
guidelines for the benefit of all who come after.  I will say it one more time, 
writing maintainable code is absolutely critical for this project, and having a
common coding style makes maintainability drastically easier to achieve.

That is all.

PRINCIPLES OF DEVELOPMENT
-Keep everything DRY.  What does that mean?  Don't Repeat Yourself (DRY).  If 
 you find two functions that are almost the same, don't repeat the code.  Find 
 a way to combine the common parts.  Refactor if needed.  STL is fantastic for 
 code reuse, especially with its function objects.  Whatever the case, take the
 time to make it right, and never, ever cut and paste chunks of code from one 
 place to another.  That's just asking for maintanability nightmares!
-Sign your work.  If you want to alter your header and cpp templates to include
 your name as the author of what you add, feel free.  It means that YOU are 
 responsible for the bugs and passing along the knowledge of how that class 
 works.  This is what you want. As a developer with integrity, you want to 
 stand by your work, no matter how good or bad it is.  If it works great, 
 awesome, you have the credit right there.  If it doesn't, take responsibility 
 for it and work to make it better.  Ask others for help if needed.  The bottom
 line is that a software craftsman puts his or her reputation on the line with 
 the code they write.  Always keep that in mind.  If you do change your 
 header/cpp templates, please leave my name and email address intact and add 
 your name and email address on a separate line.
-Prefer compiler errors to run-time errors.  This means that if you can find a 
 way to get the compiler to tell you when something is wrong, use it.  A good 
 example of this is using the const keyword to clue you in when you might be 
 changing something that you shouldn't be changing.
-Use Design By Contract in your function development.  Basically, what this 
 boils down to is to use wxASSERT to test all of the preconditions that your 
 function requires in order to ensure that it is going to do the right thing.  
 Remember, the compiler removes all wxASSERT statements in the final release 
 build, so it costs you nothing, and it will help you catch ALL KINDS OF 
 ERRORS.  A good rule of thumb is, if there is something that you are 100% 
 certain can never, ever happen (or should never ever happen), put in a 
 wxASSERT to make sure that it never ever happens.  Then, when it DOES happen, 
 you'll immediately know and can figure out why the impossible just happened.

GENERAL
-When creating new files, the first thing to do is to apply the 
 HeaderTemplate.txt or the CppTemplate.txt template to the blank page, then 
 fill in the blanks.  Just do a simple copy and paste.  It is so easy, and it 
 gives every file the same look, which leads to greater consistence and BETTER 
 MAINTAINABILITY.
-In general, avoid the ternary (? :) operator and use if/else instead.  It is 
 more clear to the developer who comes after you.  However, there are times 
 when the data is trivial (i.e. making a name plural or singular in a string), 
 where the ternary operator is worthwhile.  Just make sure it's easy to 
 understand.  And absolutely no nested ternaries.  =P
-Use the const keyword everywhere, as much as you can!  Get in the habit of 
 writing function declarations with const to start off, then remove it if 
 absolutely necessary.  It is much better to have the compiler catch the 
 problem rather than the run-time!
-If you have any data that in any way resembles a list of things, USE AN ENUM 
 to represent it.  Check out any of the data classes to see what this can look 
 like (i.e. TileObject, Board, Game, PlayerGame, etc.)  Enums give you 
 type safety, another thing the compiler can catch before the run-time does.  
 This is good.
-Pass class/struct objects by reference or pointer instead of by value, but 
 pass simple types by value.  Examples:
	void foo(const wxString &str);
	void foo2(const wxInt32 val, const bool bPlayer);
	void foo3(const GamePtr *game);
 The reason for this is to maximize efficiency.  In the case of class objects,
 the compiler won't have to create a temporary copy of the object and use the 
 copy constructor. In the case of simple types, the compiler can often just 
 inline the value right into function, since it is a simple type (usually 32 
 bits or less).

COMMENTS
-Use C++ style comments (// ...) not C style comments (/ * ... * /)  This is a 
 great reason why right here.  I can't even close off the * / without closing 
 off the comment section for this document!
-Do not overcomment.  Example:
 //calculate road lengths for everyone
 game->calculateRoadLengths(board);

 The reason this is bad is that you just violated the DRY principal.  The code 
 itself tells you obviously what it is doing.  Too many comments render 
 comments useless.  Use comments to explain WHY something is being done the way
 it is, not WHAT it is doing (unless that is not obvious).  Also use comments 
 to explain the purpose of your class and functions, and do it in one place, 
 usually the header file.

STL
-Use STL as much you possibly can.  There is no need to reinvent the wheel, 
 and there is almost always a way to accomplish something using one of the STL 
 algorithms.  Check out the Function.h file for cool applications of STL 
 function objects.  Let your mind expand.

BOOST
-Use the boost library for all heap-allocated memory!  This means use the 
 scoped_ptr, shared_ptr, weak_ptr, and their comparative array versions (though
 for arrays, you'll probably be using STL, right?)  Using these auto pointers 
 will help prevent memory leaks.
-Boost is a huge resource and contains many fabulous advantages, like STL.  
 Check it out when faced with a problem.  It probably has something you can 
 use.  Parts of it are even on the table to get included in the next revision 
 of the C++ standard.

CLASS DECLARATIONS
-Classes should have names that describe what they are.  Each part of a 
 compound name should be capitalized, as well as the first letter of the class.
 Examples:
	class TileObject
	{
		...
	};

	class MapDatabase
	{
		...
	};
 At no time should any class start with a single letter, like MFC's CFoo style.
-Class declarations should be in this order: public, protected, private.  
 Functions should proceed variables in the declaration.  The constructor and 
 destructor should come first in whatever section they are a part of.  
 Example:
	class TileObject
	{
	public:
		TileObject();
		~TileObject();

		void DoSomethingNeat(const wxInt32 val);
		...
	protected:
		void OverrideThis(void);
		...
	private:
		void InternalFunction(void);

		wxInt32 mName;
		wxString mStyle;
	};

-Function name style isn't quite as important.  If you like thisStyle(), use 
 it, but if UsingThisStyle() is your favorite, go with it.  Just be consistent.
 And use one of those two styles.
-Use initializer lists in class constructors whenever possible.  This is far 
 more efficient then using the assignment operator in the constructor body.  It
 saves an extra construction and assignment.  
 Example:
	TileObject::TileObject() :
		mSide(1)
		, mValid(false)
		, mDouble(0.0)
	{
		[avoid initializing values here]
	}
-Absolutely no getX and setX methods!!  If you need to write the equivalent of 
 get/set functions for your class, make them the same name but give them 
 different parameters. This makes it more like an attribute, which it probably 
 is.
 Example:
	const wxInt32 index(void) const {return mIndex;}
	void index(const wxInt32 val) {mIndex = val;}

VARIABLES
-Member variables of a class should be proceeded by an 'm' then capitalized.  
Examples:
	TileObject mTile;
	GamePtr mGame;

-Function variables should be declared just before they are first used, NOT at 
 the top of the function.  That style is a holdover from C and is no longer 
 necessary.  It is much more efficient to declare just before use.  It saves 
 needless constructions if the function returns early.  There are parts of the 
 code that are still C-style (notably some of the helper algorithms).  Feel 
 free to refactor them if you have the chance.
-Prefer bool to wxBOOL.  Exception: Data that gets written to the disk should be 
 wxBOOL.
-All integer types must be written as their wxWindows equivalent.  That means 
 no 'int', 'short', 'char', etc.  Use wxInt32, wxInt16, wxInt8, or their 
 unsigned equivalents.
-Integer variables names should just be the name of the variable, with no 
 capital letters.  Example: wxInt32 tile, corner;
-Integer variables with longer names should have the second and following 
 chunks capitalized.  Example: wxInt32 pirateTile, robberTile = 0, 
 fooMonkeyFriends
-Bool variables should be the name of the variable prefixed with a b.
 Example: bool bCardsLost, bResources
-Other variables should have descriptive names with style similar to compound
 integer names. 
 Examples: 
	Event eventCrash;
	TileObject tileSpecial;

USE ANONYMOUS NAMESPACES
-Anonymous (or unnamed) namespaces should be used instead of declaring anything
 globally static.  The reason for this is that an unnamed namespace makes 
 everything in it static and you are not limited by what you can put in the 
 namespace (i.e. classes, structs, functions, function objects, etc.).  There 
 is no need to declare anything in the namespace as static, either.  
 Example:

 namespace
 {
	const wxInt32 foo;
	struct functionObject
	{
		void operator()(const wxInt32 val)=0;
	};
	void passShootScore(void)
	{
	}
 }

CONDITIONAL SYNTAX

-Put rvalues on the left of the compare.  This makes the compiler tell you when
 you do something wrong like if(NULL = foo).  Without the rvalue, you would 
 have written if(foo = NULL) and then we're all in the crapper.

//if/else statement single conditionals
if(true == foo)
{
	[do your work here]
}
else
{
	[do your work here]
}

//if/else if/else multiple conditionals
if( (true == foo) &&
	(false == foo2))
{
	[do your work here]
}
else if( (false == foo) &&
		 (true == foo2))
{
	[do your work here]
}
else
{
	[do your work here]
}

//for loops
for(i = 0; i < size; i++)
{
	[do your work here]
}
*/
