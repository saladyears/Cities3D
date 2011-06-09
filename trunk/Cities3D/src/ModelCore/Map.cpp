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
#include "Map.h"
#include "Controller.h"
#include "define/defineGL.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	//piece integrity
	void integrity(const Map &map)
	{
		Map::TileArray::const_iterator it = map.tiles_begin(), 
			itEnd = map.tiles_end();

		//run through each piece looking at sides
		for(; it != itEnd; ++it)
		{
			const IntArray &sides = it->read<IntArray>(shSides);

			//look at each side
			for(size_t j = 0; j < 6; ++j)
			{
				//skip -1's
				if(-1 == sides[j])
				{
					continue;
				}

				Map::TileArray::const_iterator itSearch = map.tiles_begin() + 
					sides[j];
				const IntArray &other = itSearch->read<IntArray>(shSides);

				wxInt32 compare = std::distance(map.tiles_begin(), it);

				if(other[(j + 3) % 6] != compare)
				{
                    wxLogError(wxT("Piece %d did not have %d."), 
						sides[j], compare);
					break;
				}
			}
		}
	}
}

void Map::CreateMap(Map &map, wxInt32 width, wxInt32 rows, 
					const TileCallback &func)
{
	const float sHalfX = (X * 0.5f);
	const float sOneAndHalfX = (X * 1.5f);
	const float sTwoZ = (Z * 2.0f);

	wxInt32 size = (width * rows) + (rows >> 1);

	map.mTiles.resize(size);

	wxInt32 jump = (width << 1) + 1;

	//adjust starting point for center
	float widthX = ((sOneAndHalfX * width) + sHalfX) + ((rows >> 1) *
		sOneAndHalfX);
	float widthZ = (((rows + 1) >> 1) * sTwoZ) + ((rows >> 1) * Z) + 
		((width - 1 + !(rows & 1)) * Z);

	float x = -widthX * 0.5f;
	float z = (-widthZ * 0.5f) + (width * Z);

	//now set the id, index, and coordinates for each tile
	for(wxInt32 index = 0; index < size; ++index)
	{
		map.mTiles[index].id(index);
		map.mTiles[index].data<wxInt32>(shIndex) = index;

		//set borderness
		if( 
			//top row
			((0 <= index) && (width > index)) ||
			//bottom row
			(((size - width - !(rows & 1)) <= index) && (size > index)) ||
			//sides
			(0 == index % jump) || (0 == (index + 1) % jump) ||
			(width == index % jump) || (width == (index + 1) % jump)
			)
		{
			map.mTiles[index].data<wxInt32>(shBorder) = TRUE;
		}

		//set map coordinates
		map.mTiles[index].coords(Vector(x, 0.0f, z));

		//adjust map coords for the next time around
		if( (width != (index + 1) % jump) &&
			(0 != ((index + 1)  % jump)))
		{
			x += (X * 1.5f);
			z -= Z;
		}
		//new row, so adjust down and over
		else
		{
			x -= (X * (1.5 * (width - 1)));
			z += (Z * (width + 1));
		}

		//set map sides--they are all created with -1 as all sides, so we only 
		//need to set valid sides
		IntArray &sides = map.mTiles[index].data<IntArray>(shSides);

		//side 0 (upper-left)
		if( (0 <= (index - width - 1)) &&
			(width != index % jump))
		{
			sides[0] = index - width - 1;
		}

		//side 1 (lower-left)
		if( (0 <= (index - 1)) &&
			(width != index % jump) &&
			(0 != index % jump))
		{
			sides[1] = index - 1;
		}

		//side 2 (bottom)
		if( (size > (index + width)) &&
			(width != index % jump))
		{
			sides[2] = index + width;
		}

		//side 3 (lower-right)
		if( (size > (index + width + 1)) &&
			(0 != (index + 1) % jump))
		{
			sides[3] = index + width + 1;
		}

		//side 4 (upper-right)
		if( (size > (index + 1)) &&
			(width != (index + 1) % jump) &&
			(0 != (index + 1) % jump))
		{
			sides[4] = index + 1;
		}

		//side 5 (top)
		if( (0 <= (index - width)) &&
			(0 != (index + 1) % jump))
		{
			sides[5] = index - width;
		}

		//set tile specific data, if possible
		if(func)
		{
			func(index);
		}
	}

	//always have initial rotation on these kind of maps
	if(width < rows)
	{
		map.data<float>(shInitialRotation) = -30.0f;
	}
	else
	{
		map.data<float>(shInitialRotation) = 60.0f;
	}

	integrity(map);
}

//---------------------------- PUBLIC           -----------------------------//
Map::Map()
{
	//initialized data
	data<wxInt32>(shID) = (wxInt32) time(NULL);
	data<wxInt32>(shVersion) = 0;
	data<wxInt32>(shPoints) = 10;
	data<wxInt32>(shMinPlayers) = 1;
	data<wxInt32>(shMaxPlayers) = 6;
	data<wxInt32>(shAllowedCards) = 7;
	data<wxInt32>(shStockRoads) = 15;
	data<wxInt32>(shStockSettlements) = 5;
	data<wxInt32>(shStockCities) = 4;
	
	data<float>(shInitialDepth) = 80.0f;
	data<float>(shInitialRotation) = 0.0f;

	data<wxDateTime>(shLastEdited) = wxDateTime::UNow();
}

void Map::load(wxDataInputStream &stream)
{
	wxUint32 temp;

	stream >> mName;
	stream >> mInternalName;
	stream >> mCreator;

	//tiles
	stream >> temp;
	mTiles.clear();
	mTiles.resize(temp);
	for(wxUint32 i = 0; i < temp; i++)
	{
		mTiles[i].load(stream);
	}

	//rulesets
	stream >> temp;
	mRuleSets.clear();
	wxString str1, str2;
	for(wxUint32 i = 0; i < temp; ++i)
	{
		stream >> str1;
		stream >> str2;
		mRuleSets.push_back(std::make_pair(str1, str2));
	}

	Data::load(stream);
}

void Map::save(wxDataOutputStream &stream) const
{
	stream << mName;
	stream << mInternalName;
	stream << mCreator;

	//tiles
	stream << (wxUint32) mTiles.size();
	for(wxUint32 i = 0; i < mTiles.size(); i++)
	{
		mTiles[i].save(stream);
	}

	//rulesets
	stream << (wxUint32) mRuleSets.size();
	StringPairArray::const_iterator it, itEnd(mRuleSets.end());
	for(it = mRuleSets.begin(); it != mRuleSets.end(); ++it)
	{
		stream << it->first;
		stream << it->second;
	}

	Data::save(stream);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//


