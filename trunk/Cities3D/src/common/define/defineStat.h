/*
 *  Settlers3D - Copyright (C) 2001-2003 Jason Fugate (saladyears@gmail.com)
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

//---------------------------- TYPEDEFS         -----------------------------//
enum BankTradeType
{
	TRADE_BANK_2FOR1				= 0,
	TRADE_BANK_3FOR1				,
	TRADE_BANK_4FOR1				,
	TRADE_BANK_SIZE					,
	TRADE_BANK_DWORD				= 0x7fffffff
};

enum ResourceStatType
{
	RES_STAT_BOARD					= 0,
	RES_STAT_STOLEN					,
	RES_STAT_DEV					,
	RES_STAT_MONOPOLY				,
	RES_STAT_LOST_7					,
	RES_STAT_LOST_STOLEN				,
	RES_STAT_LOST_BOARD				,
	RES_STAT_TRADE_BANK				,
	RES_STAT_BUILD					,
	RES_STAT_TRADE					,
	RES_STAT_GOLD					,
	RES_STAT_DISCOVER				,
	RES_STAT_TRADE_JUNGLE				,
	RES_STAT_DWORD					= 0x7fffffff
};

enum StatType
{
	STAT_HEAD_ROLL					= 0,
	STAT_ROLL_FREQ					,
	STAT_ROLL_PLAYER				,
	STAT_HEAD_TIME					,
	STAT_TIME_TURN					,
	STAT_HEAD_GOODS_RECV				,
	STAT_GOODS_RECV_TOTAL				,
	STAT_GOODS_RECV_BOARD				,
	STAT_GOODS_RECV_STEAL				,
	STAT_GOODS_RECV_DEVCARD				,
	STAT_GOODS_RECV_GOLD				,
	STAT_GOODS_RECV_DISCOVER			,
	STAT_HEAD_GOODS_LOST				,
	STAT_GOODS_LOST_TOTAL				,
	STAT_GOODS_LOST_BOARD				,
	STAT_GOODS_LOST_STEAL				,
	STAT_GOODS_LOST_7				,
	STAT_HEAD_TRADES				,
	STAT_TRADE_NUMBER				,
	STAT_TRADE_NUM_OTHERS				,
	STAT_TRADE_RATIO				,
	STAT_TRADE_TO_BANK				,
	STAT_TRADE_FROM_BANK				,
	STAT_TRADE_TO_OTHERS				,
	STAT_TRADE_FROM_OTHERS				,
	STAT_SIZE					,
	STAT_DWORD					= 0x7fffffff
};

//---------------------------- DEFINES          -----------------------------//
namespace
{
inline bool IS_BANK(const ResourceStatType nStat)
{
	return ((nStat == RES_STAT_BOARD) || 
			(nStat == RES_STAT_DEV) || 
			(nStat == RES_STAT_LOST_7) || 
			(nStat == RES_STAT_TRADE_BANK) || 
			(nStat == RES_STAT_BUILD) || 
			(nStat == RES_STAT_GOLD) || 
			(nStat == RES_STAT_DISCOVER) || 
			(nStat == RES_STAT_TRADE_JUNGLE));
}
}

//---------------------------- CLASSES          -----------------------------//

//---------------------------- PROTOTYPES       -----------------------------//

