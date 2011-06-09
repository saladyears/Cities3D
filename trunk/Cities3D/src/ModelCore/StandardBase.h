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

#include "style.h"	//READ THIS BEFORE MAKING ANY CHANGES TO THIS FILE!!!

//---------------------------- SYSTEM INCLUDES  -----------------------------//

//---------------------------- USER INCLUDES    -----------------------------//
#include "OfficialMap.h"
#include "define/defineGL.h"

//---------------------------- DEFINES          -----------------------------//

//---------------------------- TYPEDEFS         -----------------------------//

//---------------------------- CLASSES          -----------------------------//
class StandardBase : public OfficialMap
{
public:
	StandardBase()
	{
		mRuleSets.push_back(std::make_pair(swStandardRules, 
			swRandomTiles));
		mRuleSets.push_back(std::make_pair(swStandardRules, 
			swAroundTheBoard));
		mRuleSets.push_back(std::make_pair(swStandardRules, 
			swAroundTheBoardPorts));
	}

protected:
	void StandardCoords(const wxInt32 index)
	{
		Vector coords;

		coords.y = 0.;

		switch(index)
		{
		case 0: coords.x = 0.00;					coords.z = 0.00;	break;
		case 1: coords.x = -((X / 2) + X);			coords.z = -Z;		break;
		case 2: coords.x = -((X / 2) + X);			coords.z = Z;		break;
		case 3: coords.x = 0.00;					coords.z = Z * 2;	break;
		case 4: coords.x = ((X / 2) + X);			coords.z = Z;		break;
		case 5: coords.x = ((X / 2) + X);			coords.z = -Z;		break;
		case 6: coords.x = 0.00;					coords.z = -Z * 2;	break;
		case 7: coords.x = -((X / 2) + X);			coords.z = -Z * 3;	break;
		case 8: coords.x = -((X / 2) + X) * 2;		coords.z = -Z * 2;	break;
		case 9: coords.x = -((X / 2) + X) * 2;		coords.z = 0.00;	break;
		case 10: coords.x = -((X / 2) + X) * 2;		coords.z = Z * 2;	break;
		case 11: coords.x = -((X / 2) + X);			coords.z = Z * 3;	break;
		case 12: coords.x = 0.00;					coords.z = Z * 4;	break;
		case 13: coords.x = ((X / 2) + X);			coords.z = Z * 3;	break;
		case 14: coords.x = ((X / 2) + X) * 2;		coords.z = Z * 2;	break;
		case 15: coords.x = ((X / 2) + X) * 2;		coords.z = 0.00;	break;
		case 16: coords.x = ((X / 2) + X) * 2;		coords.z = -Z * 2;	break;
		case 17: coords.x = ((X / 2) + X);			coords.z = -Z * 3;	break;
		case 18: coords.x = 0.00;					coords.z = -Z * 4;	break;
		case 19: coords.x = -((X / 2) + X);			coords.z = -Z * 5;	break;
		case 20: coords.x = -((X / 2) + X) * 2;		coords.z = -Z * 4;	break;
		case 21: coords.x = -((X / 2) + X) * 3;		coords.z = -Z * 3;	break;
		case 22: coords.x = -((X / 2) + X) * 3;		coords.z = -Z;		break;
		case 23: coords.x = -((X / 2) + X) * 3;		coords.z = Z;		break;
		case 24: coords.x = -((X / 2) + X) * 3;		coords.z = Z * 3;	break;
		case 25: coords.x = -((X / 2) + X) * 2;		coords.z = Z * 4;	break;
		case 26: coords.x = -((X / 2) + X);			coords.z = Z * 5;	break;
		case 27: coords.x = 0.00;					coords.z = Z * 6;	break;
		case 28: coords.x = ((X / 2) + X);			coords.z = Z * 5;	break;
		case 29: coords.x = ((X / 2) + X) * 2;		coords.z = Z * 4;	break;
		case 30: coords.x = ((X / 2) + X) * 3;		coords.z = Z * 3;	break;
		case 31: coords.x = ((X / 2) + X) * 3;		coords.z = Z;		break;
		case 32: coords.x = ((X / 2) + X) * 3;		coords.z = -Z;		break;
		case 33: coords.x = ((X / 2) + X) * 3;		coords.z = -Z * 3;	break;
		case 34: coords.x = ((X / 2) + X) * 2;		coords.z = -Z * 4;	break;
		case 35: coords.x = ((X / 2) + X);			coords.z = -Z * 5;	break;
		case 36: coords.x = 0.00;					coords.z = -Z * 6;	break;

		//5-6 player
		case 37: coords.x = -((X / 2) + X);			coords.z = -Z * 7;	break;
		case 38: coords.x = -((X / 2) + X) * 2;		coords.z = -Z * 6;	break;
		case 39: coords.x = -((X / 2) + X) * 3;		coords.z = -Z * 5;	break;
		case 40: coords.x = -((X / 2) + X) * 4;		coords.z = -Z * 4;	break;
		case 41: coords.x = -((X / 2) + X) * 4;		coords.z = -Z * 2;	break;
		case 42: coords.x = -((X / 2) + X) * 4;		coords.z = 0.00;	break;
		case 43: coords.x = -((X / 2) + X) * 4;		coords.z = Z * 2;	break;
		case 44: coords.x = -((X / 2) + X) * 4;		coords.z = Z * 4;	break;
		case 45: coords.x = -((X / 2) + X) * 3;		coords.z = Z * 5;	break;
		case 46: coords.x = -((X / 2) + X) * 2;		coords.z = Z * 6;	break;
		case 47: coords.x = -((X / 2) + X);			coords.z = Z * 7;	break;
		case 48: coords.x = 0.00;					coords.z = Z * 8;	break;
		case 49: coords.x = ((X / 2) + X);			coords.z = Z * 7;	break;
		case 50: coords.x = ((X / 2) + X) * 2;		coords.z = Z * 6;	break;
		case 51: coords.x = ((X / 2) + X) * 3;		coords.z = Z * 5;	break;
		}

		mTiles[index].coords(coords);

		IntArray &sides = tile<IntArray>(shSides, index);

		//set sides
		switch(index)
		{
		case 0: sides[0] = 1; sides[1] = 2; sides[2] = 3; sides[3] = 4;
				sides[4] = 5; sides[5] = 6; break;
		
		case 1: sides[0] = 8; sides[1] = 9; sides[2] = 2; sides[3] = 0;
				sides[4] = 6; sides[5] = 7; break;
		
		case 2: sides[0] = 9; sides[1] = 10; sides[2] = 11; sides[3] = 3;
				sides[4] = 0; sides[5] = 1; break;

		case 3: sides[0] = 2; sides[1] = 11; sides[2] = 12; sides[3] = 13;
				sides[4] = 4; sides[5] = 0; break;

		case 4: sides[0] = 0; sides[1] = 3; sides[2] = 13; sides[3] = 14;
				sides[4] = 15; sides[5] = 5; break;

		case 5: sides[0] = 6; sides[1] = 0; sides[2] = 4; sides[3] = 15;
				sides[4] = 16; sides[5] = 17; break;

		case 6: sides[0] = 7; sides[1] = 1; sides[2] = 0; sides[3] = 5;
				sides[4] = 17; sides[5] = 18; break;

		case 7: sides[0] = 20; sides[1] = 8; sides[2] = 1; sides[3] = 6;
				sides[4] = 18; sides[5] = 19; break;

		case 8: sides[0] = 21; sides[1] = 22; sides[2] = 9; sides[3] = 1;
				sides[4] = 7; sides[5] = 20; break;

		case 9: sides[0] = 22; sides[1] = 23; sides[2] = 10; sides[3] = 2;
				sides[4] = 1; sides[5] = 8; break;

		case 10: sides[0] = 23; sides[1] = 24; sides[2] = 25; sides[3] = 11;
				sides[4] = 2; sides[5] = 9; break;

		case 11: sides[0] = 10; sides[1] = 25; sides[2] = 26; sides[3] = 12;
				sides[4] = 3; sides[5] = 2; break;

		case 12: sides[0] = 11; sides[1] = 26; sides[2] = 27; sides[3] = 28;
				sides[4] = 13; sides[5] = 3; break;

		case 13: sides[0] = 3; sides[1] = 12; sides[2] = 28; sides[3] = 29;
				sides[4] = 14; sides[5] = 4; break;

		case 14: sides[0] = 4; sides[1] = 13; sides[2] = 29; sides[3] = 30;
				sides[4] = 31; sides[5] = 15; break;

		case 15: sides[0] = 5; sides[1] = 4; sides[2] = 14; sides[3] = 31;
				sides[4] = 32; sides[5] = 16; break;

		case 16: sides[0] = 17; sides[1] = 5; sides[2] = 15; sides[3] = 32;
				sides[4] = 33; sides[5] = 34; break;

		case 17: sides[0] = 18; sides[1] = 6; sides[2] = 5; sides[3] = 16;
				sides[4] = 34; sides[5] = 35; break;

		case 18: sides[0] = 19; sides[1] = 7; sides[2] = 6; sides[3] = 17;
				sides[4] = 35; sides[5] = 36; break;

		case 19: sides[0] = 38; sides[1] = 20; sides[2] = 7; sides[3] = 18;
				sides[4] = 36; sides[5] = 37; break;

		case 20: sides[0] = 39; sides[1] = 21; sides[2] = 8; sides[3] = 7;
				sides[4] = 19; sides[5] = 38; break;

		case 21: sides[0] = 40; sides[1] = 41; sides[2] = 22; sides[3] = 8;
				sides[4] = 20; sides[5] = 39; break;

		case 22: sides[0] = 41; sides[1] = 42; sides[2] = 23; sides[3] = 9;
				sides[4] = 8; sides[5] = 21; break;

		case 23: sides[0] = 42; sides[1] = 43; sides[2] = 24; sides[3] = 10;
				sides[4] = 9; sides[5] = 22; break;

		case 24: sides[0] = 43; sides[1] = 44; sides[2] = 45; sides[3] = 25;
				sides[4] = 10; sides[5] = 23; break;

		case 25: sides[0] = 24; sides[1] = 45; sides[2] = 46; sides[3] = 26;
				sides[4] = 11; sides[5] = 10; break;

		case 26: sides[0] = 25; sides[1] = 46; sides[2] = 47; sides[3] = 27;
				sides[4] = 12; sides[5] = 11; break;

		case 27: sides[0] = 26; sides[1] = 47; sides[2] = 48; sides[3] = 49;
				sides[4] = 28; sides[5] = 12; break;

		case 28: sides[0] = 12; sides[1] = 27; sides[2] = 49; sides[3] = 50;
				sides[4] = 29; sides[5] = 13; break;

		case 29: sides[0] = 13; sides[1] = 28; sides[2] = 50; sides[3] = 51;
				sides[4] = 30; sides[5] = 14; break;

		case 30: sides[0] = 14; sides[1] = 29; sides[2] = 51; sides[3] = -1;
				sides[4] = -1; sides[5] = 31; break;

		case 31: sides[0] = 15; sides[1] = 14; sides[2] = 30; sides[3] = -1;
				sides[4] = -1; sides[5] = 32; break;

		case 32: sides[0] = 16; sides[1] = 15; sides[2] = 31; sides[3] = -1;
				sides[4] = -1; sides[5] = 33; break;

		case 33: sides[0] = 34; sides[1] = 16; sides[2] = 32; sides[3] = -1;
				sides[4] = -1; sides[5] = -1; break;

		case 34: sides[0] = 35; sides[1] = 17; sides[2] = 16; sides[3] = 33;
				sides[4] = -1; sides[5] = -1; break;

		case 35: sides[0] = 36; sides[1] = 18; sides[2] = 17; sides[3] = 34;
				sides[4] = -1; sides[5] = -1; break;

		case 36: sides[0] = 37; sides[1] = 19; sides[2] = 18; sides[3] = 35;
				sides[4] = -1; sides[5] = -1; break;


		//5-6 pl
		case 37: sides[0] = -1; sides[1] = 38; sides[2] = 19; sides[3] = 36;
				sides[4] = -1; sides[5] = -1; break;

		case 38: sides[0] = -1; sides[1] = 39; sides[2] = 20; sides[3] = 19;
				sides[4] = 37; sides[5] = -1; break;

		case 39: sides[0] = -1; sides[1] = 40; sides[2] = 21; sides[3] = 20;
				sides[4] = 38; sides[5] = -1; break;

		case 40: sides[0] = -1; sides[1] = -1; sides[2] = 41; sides[3] = 21;
				sides[4] = 39; sides[5] = -1; break;

		case 41: sides[0] = -1; sides[1] = -1; sides[2] = 42; sides[3] = 22;
				sides[4] = 21; sides[5] = 40; break;

		case 42: sides[0] = -1; sides[1] = -1; sides[2] = 43; sides[3] = 23;
				sides[4] = 22; sides[5] = 41; break;

		case 43: sides[0] = -1; sides[1] = -1; sides[2] = 44; sides[3] = 24;
				sides[4] = 23; sides[5] = 42; break;

		case 44: sides[0] = -1; sides[1] = -1; sides[2] = -1; sides[3] = 45;
				sides[4] = 24; sides[5] = 43; break;

		case 45: sides[0] = 44; sides[1] = -1; sides[2] = -1; sides[3] = 46;
				sides[4] = 25; sides[5] = 24; break;

		case 46: sides[0] = 45; sides[1] = -1; sides[2] = -1; sides[3] = 47;
				sides[4] = 26; sides[5] = 25; break;

		case 47: sides[0] = 46; sides[1] = -1; sides[2] = -1; sides[3] = 48;
				sides[4] = 27; sides[5] = 26; break;

		case 48: sides[0] = 47; sides[1] = -1; sides[2] = -1; sides[3] = -1;
				sides[4] = 49; sides[5] = 27; break;

		case 49: sides[0] = 27; sides[1] = 48; sides[2] = -1; sides[3] = -1;
				sides[4] = 50; sides[5] = 28; break;

		case 50: sides[0] = 28; sides[1] = 49; sides[2] = -1; sides[3] = -1;
				sides[4] = 51; sides[5] = 29; break;

		case 51: sides[0] = 29; sides[1] = 50; sides[2] = -1; sides[3] = -1;
				sides[4] = -1; sides[5] = 30; break;

		default:
			break;
		}
	}

};

//---------------------------- PROTOTYPES       -----------------------------//

