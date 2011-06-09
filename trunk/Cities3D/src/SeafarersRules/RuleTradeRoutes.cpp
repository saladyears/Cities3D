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
#include "Rule.h"
#include "RuleSetDatabase.h"
#include "Utility.h"
#include "SideObject.h"
#include "CornerObject.h"
#include "Controller.h"
#include "IRuleEngine.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//

//---------------------------- RULES            -----------------------------//

//---------------------------------------------------------------------------//
// Class: RuleTradeRoutes
// 
// Determines trade route values between players.
//
// Derived From:
//     <Rule>
//
// Project:
//     <SeafarersRules>
//
// RuleSet:
//     <TradeRoutes>
//
// Mixin To:
//     <RulePlaceShipCommon>
//     <RulePlaceRoad>
//
class RuleTradeRoutes : public Rule
{
public:
	virtual void Execute(const DataObject &object)
	{
		wxInt32 tile1, side1;
		boost::tie(tile1, side1) = Utility::decodeSel(object.read<wxInt32>());

		// Here is the algorithm for this crazy ruleset:
		// 
		// 1.  For each settlement or city on the first island:
		// 2.  Follow each road or ship (of any color) and see if any other 
		//     settlement or city can be reached on the other island.
		// 3.  If a settlement or city can be reached, a trade route point is
		//     assigned.  Only one point may be assigned per pair of cities or
		//     settlement.

		// Go through each player, find the ones starting on the first home
		// island, and add their settlements/cities to the list to be checked.
		CornerObjectArray objects;
		BuildHashes(objects);

		// Find routes.
		std::for_each(objects.begin(), objects.end(), 
			boost::bind(&RuleTradeRoutes::CheckRoutes, this, _1));

		wxLogDebug(wxT("Found %u trade routes."), mTradeRoutes.size());

		// Assign points based on the routes.
		AssignPoints();

		// Award those points, if any.
		AwardPoints();

		// Set the extra points.
		SetExtraPoints();

		// Don't hold on to any memory.
		mEndPoints.clear();
		mCornerPoints.clear();
		mSegmentHash.clear();
		mTradeRoutes.clear();
		mCurrentRoute = TradeRoute();
		mPointsAssigned.clear();
	}

private:
	typedef std::vector<CornerObjectPtr> CornerObjectArray;
	typedef std::vector<SideObjectPtr> SideObjectArray;
	typedef std::map<wxInt32, CornerObjectPtr> CornerObjectMap;
	typedef std::map<CornerObjectPtr, wxInt32> CornerPointMap;

	struct TradeRoute
	{
		CornerObjectPtr start;
		CornerObjectPtr end;

		SideObjectArray route;
	};

	typedef std::vector<TradeRoute> TradeRouteArray;
	typedef std::map<CornerObjectPtr, TradeRouteArray> EndRouteMap;
	typedef std::map<CornerObjectPtr, EndRouteMap> TradeRouteMap;

	struct Segment
	{
		SideObjectPtr object;

		bool counted;
	};

	WX_DECLARE_HASH_MAP(wxInt32, Segment, wxIntegerHash, wxIntegerEqual,
		SegmentHash);

	void BuildHashes(CornerObjectArray& objects)
	{
		typedef std::pair<wxInt32, wxInt32> TileCorner;

		for(wxInt32 i = 0; i < numPlayers(); ++i)
		{
			const CornerObjectArray& settlements = 
				playerGame(i).getCornerObjects(shSettlements);
			const CornerObjectArray& cities =
				playerGame(i).getCornerObjects(shCities);

			// All objects are end points, since trade routes can't continue
			// past another city/settlement on the same island.
			{
				CornerObjectArray::const_iterator it, itEnd = settlements.end();
				for(it = settlements.begin(); it != itEnd; ++it)
				{
					const CornerObjectPtr& object = (*it);
				
					TileCorner tile1 = object->tile1();
					TileCorner tile2 = object->tile2();
					TileCorner tile3 = object->tile3();

					wxInt32 id1 = Utility::encodeSel(tile1.first, 
						tile1.second);
					wxInt32 id2 = Utility::encodeSel(tile2.first, 
						tile2.second);
					wxInt32 id3 = Utility::encodeSel(tile3.first, 
						tile3.second);
					
					mEndPoints[id1] = object;
					mEndPoints[id2] = object;
					mEndPoints[id3] = object;

					// In adition, all corner objects need to keep track of
					// how many points they get.
					mCornerPoints[object] = 0;

					if( (1 == tile<wxInt32>(shIsland, tile1.first)) ||
						(1 == tile<wxInt32>(shIsland, tile2.first)) ||
						(1 == tile<wxInt32>(shIsland, tile3.first)))

					{
						objects.push_back(object);
					}
				}

				itEnd = cities.end();
				for(it = cities.begin(); it != itEnd; ++it)
				{
					const CornerObjectPtr& object = (*it);
				
					TileCorner tile1 = object->tile1();
					TileCorner tile2 = object->tile2();
					TileCorner tile3 = object->tile3();

					wxInt32 id1 = Utility::encodeSel(tile1.first, 
						tile1.second);
					wxInt32 id2 = Utility::encodeSel(tile2.first, 
						tile2.second);
					wxInt32 id3 = Utility::encodeSel(tile3.first, 
						tile3.second);
					
					mEndPoints[id1] = object;
					mEndPoints[id2] = object;
					mEndPoints[id3] = object;

					// In adition, all corner objects need to keep track of
					// how many points they get.
					mCornerPoints[object] = 0;

					if( (1 == tile<wxInt32>(shIsland, tile1.first)) ||
						(1 == tile<wxInt32>(shIsland, tile2.first)) ||
						(1 == tile<wxInt32>(shIsland, tile3.first)))

					{
						objects.push_back(object);
					}
				}
			}

			// In addition, add all roads/ships of the player's to the hash
			// of segments.
			PlayerGame::SideObjectArray sides;
			playerGame(i).getAllSideObjects(sides);

			PlayerGame::SideObjectArray::const_iterator it, 
				itEnd = sides.end();
			for(it = sides.begin(); it != itEnd; ++it)
			{
				// Add each to the hash.
				SideObject::TileSide side = (*it)->tile1();
				wxInt32 id = Utility::encodeSel(side.first, side.second);
				mSegmentHash[id].object = (*it);
				mSegmentHash[id].counted = false;

				side = (*it)->tile2();
				id = Utility::encodeSel(side.first, side.second);
				mSegmentHash[id].object = (*it);
				mSegmentHash[id].counted = false;
			}
		}
	}

	void CheckRoutes(const CornerObjectPtr& object)
	{
		wxASSERT(true == mCurrentRoute.route.empty());

		// Start from this point.
		mCurrentRoute.start = object;

		// There are three possible road/ship options for this object.  Iterate
		// on each of them.

		wxInt32 tile1 = object->tile1().first;
		wxInt32 corner1 = object->tile1().second;

		wxInt32 id1 = Utility::encodeSel(tile1, corner1);
		wxInt32 id2 = Utility::encodeSel(tile1, (corner1 + 5) % 6);
		wxInt32 tile2 = tile<IntArray>(shSides, tile1)[corner1];
		wxInt32 id3 = Utility::encodeSel(tile2, (corner1 + 4) % 6);

		Traverse(id1, id2, id3);
		Traverse(id2, id1, id3);
		Traverse(id3, id1, id2);
	}
	
	void Traverse(wxInt32 id, wxInt32 avoidID1, wxInt32 avoidID2)
	{
		SegmentHash::iterator it = mSegmentHash.find(id);

		// If there's no side object here, or it has already been visited,
		// do nothing.
		if( (mSegmentHash.end() == it) ||
			(true == it->second.counted))
		{
			return;
		}

		// Mark this segment as being traversed so we don't hit it again.
		Segment &segment = it->second;
		segment.counted = true;

		// In addition, mark the other side as being visited.
		wxInt32 thisTile, thisSide;
		boost::tie(thisTile, thisSide) = Utility::decodeSel(id);

		wxInt32 otherTile = tile<IntArray>(shSides, thisTile)[thisSide];
		wxInt32 otherID = Utility::encodeSel(otherTile, (thisSide + 3) % 6);
		wxASSERT(mSegmentHash.end() != mSegmentHash.find(otherID));
		mSegmentHash[otherID].counted = true;

		// Push the addition of this segment onto the trade route stack.
		mCurrentRoute.route.push_back(segment.object);

		// Branch direction 1.
		wxInt32 branch1ID = Utility::encodeSel(thisTile, (thisSide + 1) % 6);
		wxInt32 branch2ID = Utility::encodeSel(otherTile, (thisSide + 2) % 6);

		wxInt32 cornerID = branch1ID;

		if( (avoidID1 != branch1ID) &&
			(avoidID1 != branch2ID) &&
			(avoidID2 != branch1ID) &&
			(avoidID2 != branch2ID)
			)
		{
			// First, see if we hit an end point this direction.  If we don't,
			// continue looking.
			CornerObjectMap::const_iterator it = mEndPoints.find(cornerID);
			if(mEndPoints.end() != it)
			{
				// We found an end point, see if it closes off a route.
				wxInt32 tileID = it->second->tile1().first;
				if(2 == tile<wxInt32>(shIsland, tileID))
				{
					// Found one, this is a route!
					mCurrentRoute.end = it->second;
					mTradeRoutes[mCurrentRoute.start][mCurrentRoute.end].push_back(
						mCurrentRoute);
				}
			}
			else
			{
				Traverse(branch1ID, id, otherID);
				Traverse(branch2ID, id, otherID);
			}
		}

		// Branch direction 2.
		wxInt32 curSide = (thisSide + 5) % 6;
		branch1ID = Utility::encodeSel(thisTile, curSide);
		otherTile = tile<IntArray>(shSides, thisTile)[curSide];
		branch2ID = Utility::encodeSel(otherTile, (thisSide + 1) % 6);

		cornerID = id;

		if( (avoidID1 != branch1ID) &&
			(avoidID1 != branch2ID) &&
			(avoidID2 != branch1ID) &&
			(avoidID2 != branch2ID)
			)
		{
			// First, see if we hit an end point this direction.  If we don't,
			// continue looking.
			CornerObjectMap::const_iterator it = mEndPoints.find(cornerID);
			if(mEndPoints.end() != it)
			{
				// We found an end point, see if it closes off a route.
				wxInt32 tileID = it->second->tile1().first;
				if(2 == tile<wxInt32>(shIsland, tileID))
				{
					// Found one, this is a route!
					mCurrentRoute.end = it->second;
					mTradeRoutes[mCurrentRoute.start][mCurrentRoute.end].push_back(
						mCurrentRoute);
				}
			}
			else
			{
				Traverse(branch1ID, id, otherID);
				Traverse(branch2ID, id, otherID);
			}
		}

		// Remove this object from the current route stack.
		mCurrentRoute.route.pop_back();

		// We're off the segment so it can be counted in another chain.
		segment.counted = false;
		mSegmentHash[otherID].counted = false;
	}

	void AssignPoints()
	{
		// The trade point is assigned by the following criteria:
		//
		// 1.  Only the shortest trade route between two points is considered.
		// 2.  Only players who own the start or end point of the route are
		//     considered.
		// 3a. In case of a single shortest trade route, the player with the 
		//     most pieces in the route receives the point.
		// 3b. In case of multiple trade routes of the same length, the player 
		//     with the most pieces within these trade routes receives the 
		//     point. 
		// 4.  In the case where the piece counts are equal, the player with the most
		//     recently played piece in the route(s) receives the point.

		wxInt32 players = numPlayers();

		mPointsAssigned.resize(players, 0);

		// Iterate through all trade routes found, determining who gets each
		// point.
		TradeRouteMap::const_iterator it, itEnd = mTradeRoutes.end();
		for(it = mTradeRoutes.begin(); it != itEnd; ++it)
		{
			CornerObjectPtr start = it->first;

			// Each start point may have multiple end points for different
			// trade routes, so iterate through those, too.
			EndRouteMap::const_iterator it2, it2End = it->second.end();
			for(it2 = it->second.begin(); it2 != it2End; ++it2)
			{
				CornerObjectPtr end = it2->first;
				TradeRouteArray routes = it2->second;

				// There may actually be multiple routes between these two, so
				// first, remove everything but the shortest route(s).
				size_t minSize = routes[0].route.size();
				size_t size = routes.size();
				for(size_t i = 1; i < size; ++i)
				{
					size_t thisSize = routes[i].route.size();
					if(minSize > thisSize)
					{
						minSize = thisSize;
					}
				}

				// Remove all routes longer than the minimum.
				TradeRouteArray shortest;
				for(size_t i = 0; i < size; ++i)
				{
					if(routes[i].route.size() == minSize)
					{
						shortest.push_back(routes[i]);
					}
				}

				// Now that we are working only with the shortest route(s), 
				// calculate points.

				size = shortest.size();
				wxASSERT(0 < size);

				size_t startCount = 0;
				size_t endCount = 0;
				wxInt32 recent = 0;
				wxInt32 recentOwner = -1;

				// The easy case.
				for(size_t i = 0; i < size; ++i)
				{
					GetTradeCounts(shortest[i], startCount, endCount, recent, 
						recentOwner);
				}

				wxInt32 startOwner = start->owner();
				wxInt32 endOwner = end->owner();

				// Now that we have counts, assign the winner.
				wxInt32 winner = -1;
				if(startCount > endCount)
				{
					winner = startOwner;
				}
				else if(startCount < endCount)
				{
					winner = endOwner;
				}
				else
				{
					wxASSERT(-1 != recentOwner);
					winner = recentOwner;

					// See if we need to switch start and end points, if
					// the player builds their own complete tree, but their 
					// home island is number 2.
					if( (startCount == endCount) &&
						(startOwner == endOwner) &&
						(2 == playerGameData<wxInt32>(shHomeIsland, 
						startOwner)))
					{
						CornerObjectPtr temp = end;
						end = start;
						start = temp;
					}
				}

				// Assign the point.
				wxASSERT(-1 != winner);

				mPointsAssigned[winner]++;

				// Add to the corner object's points.
				if(winner == startOwner)
				{
					mCornerPoints[start]++;
				}
				else
				{
					mCornerPoints[end]++;
				}
			}
		}
	}

	void GetTradeCounts(const TradeRoute& route, size_t &startOwner,
		size_t& endOwner, wxInt32 &recent, wxInt32& recentOwner)
	{
		wxInt32 owner1 = route.start->owner();
		wxInt32 owner2 = route.end->owner();

		// If the same person owns both sides, it's easy.
		if(owner1 == owner2)
		{
			startOwner = endOwner = route.route.size();
			recentOwner = owner1;
		}
		else
		{
			// Go through the trade route, summing up totals for each player.
			SideObjectArray::const_iterator it, itEnd = route.route.end();
			for(it = route.route.begin(); it != itEnd; ++it)
			{
				SideObjectPtr object = (*it);

				wxInt32 owner = object->owner();
				wxInt32 turn = object->turn();
				bool thisOwner = false;

				if(owner1 == owner)
				{
					++startOwner;
					thisOwner = true;
				}
				else if(owner2 == owner)
				{
					++endOwner;
					thisOwner = true;
				}

				if( (true == thisOwner) &&
					(turn > recent))
				{
					recent = turn;
					recentOwner = owner;
				}
			}
		}
	}

	void AwardPoints()
	{
		// We only need to make changes if point values have changed.
		wxInt32 players = numPlayers();

		for(wxInt32 i = 0; i < players; ++i)
		{
			wxInt32 currentPoints = mPointsAssigned[i];
			wxInt32 &oldPoints = playerGameData<wxInt32>(shTradePoints, i);

			if(oldPoints != currentPoints)
			{
				wxInt32 dif = currentPoints - oldPoints;

				// Adjust point totals.
				RULE.Execute(shRuleAdjustPoints, DataObject(i, dif));

				oldPoints = currentPoints;

				Controller::get().Transmit(shEventPlayerUI, 
					DataObject(GetGame(), i));

				// Create the text.
				wxString str;

				if(0 > dif)
				{
					if(-1 == dif)
					{
						str = stLosesOneTradeRoutePoint;
					}
					else
					{
						str = wxString::Format(stLosesXTradeRoutePoints.c_str(), 
							swStringFormat.c_str(), -dif);
					}
				}
				else
				{
					if(1 == dif)
					{
						str = stGainsOneTradeRoutePoint;
					}
					else
					{
						str = wxString::Format(stGainsXTradeRoutePoints.c_str(), 
							swStringFormat.c_str(), dif);
					}
				}

				// Send the text.
				RULE.Execute(shRuleUpdateNetworkUI, DataObject(str, i));
			}
		}
	}

	void SetExtraPoints()
	{
		// Go through each object and compare the points it now have with
		// how many points it currently has.  If there are any changes,
		// deal with them.
		CornerPointMap::const_iterator it, itEnd = mCornerPoints.end();
		for(it = mCornerPoints.begin(); it != itEnd; ++it)
		{
			CornerObjectPtr object = it->first;
			wxInt32 points = it->second;
			wxInt32 links = object->links();

			if(points != links)
			{
				RULE.Execute(shRuleAdjustExtras, DataObject(object, 
					points - links));
			}
		}
	}

	CornerObjectMap mEndPoints;
	CornerPointMap mCornerPoints;

	SegmentHash mSegmentHash;

	TradeRouteMap mTradeRoutes;
	TradeRoute mCurrentRoute;

	IntArray mPointsAssigned;
};

IMPLEMENT_RULE_MIXIN(RuleTradeRoutes, RulePlaceShipCommon, TRADE_ROUTES)
IMPLEMENT_RULE_MIXIN(RuleTradeRoutes, RulePlaceRoad, TRADE_ROUTES)
IMPLEMENT_RULE_MIXIN(RuleTradeRoutes, RulePlaceSettlementTurn, TRADE_ROUTES)
