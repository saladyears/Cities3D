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
#include "RNG.h"

//---------------------------- TYPEDEFS         -----------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------- STATICS          -----------------------------//
namespace
{
	const wxUint32 POOL_SIZE = 512;

	// Use a Mersenne Twister for our psuedo-random number generator.
	typedef boost::mt19937 base_generator_type;

	// When we create the pool, use a uniform distribution.
	typedef boost::uniform_int<wxUint32> pooled_distribution_type;

	// When we get unpooled values, use the small_int distribution.
	typedef boost::uniform_smallint<wxUint32> unpooled_distribution_type;
	
	// Combine the two into generators.
	typedef boost::variate_generator<base_generator_type &, 
		pooled_distribution_type> pooled_generator_type;

	typedef boost::variate_generator<base_generator_type &, 
		unpooled_distribution_type>	unpooled_generator_type;

	// The generator for unpooled values.
	base_generator_type sUnpooledGenerator;
}

//---------------------------- PUBLIC           -----------------------------//
void RNG::repool()
{
	wxLogDebug(wxT("RNG repool"));

	// See the RNG with the current time.  It's lame, but what other better way
	// is there?
	base_generator_type generator(time(NULL));

	// Since we are just creating a pool of values, we want them to be the raw
	// values reported by Mersenne.
	pooled_distribution_type distribute(0, 
		(std::numeric_limits<wxUint32>::max)());
	pooled_generator_type pool(generator, distribute);

	// Fill the pool.
	mPool.reset(new wxUint32[POOL_SIZE]);

	for(size_t i = 0; i < POOL_SIZE; ++i)
	{
		mPool[i] = pool();
	}

	// Start at the beginning again.
	mIndex = 0;
}

const wxUint32 RNG::unpooled(const wxUint32 n) const
{
	wxUint32 val = 0;

	if(1 < n)
	{
		unpooled_distribution_type distribute(0, n - 1);
		unpooled_generator_type gen(sUnpooledGenerator, distribute);

		val = gen();
	}

	wxLogDebug(wxT("RNG unpooled returns %u"), val);

	return val;
}

const wxUint32 RNG::pooled(const wxUint32 n) const
{
	wxUint32 val = 0;

	// This code is modified from boost::uniform_smallint.hpp.  The assumption
	// here is that the game is always going to ask for integers in a small
	// range (i.e. 0..6, or 0..25, etc.).  The nicest way to get these numbers 
	// is to mod the value returned by the random number generator.  However, 
	// there are problems with the low bits in a Linear Congruent Generator, 
	// which is what we are using, so boost uses this algorithm to determine a
	// factor that ensures we get a good distribution.
	if(1 < n)
	{
		wxUint32 bits = next();
		wxUint32 factor = 2;
		wxUint32 base = ((std::numeric_limits<wxUint32>::max)() / 2) + 1;

		wxUint32 mult = n * 32;

		for(; (base / mult) >= n; factor *= 2)
		{
			base /= 2;
		}

		val = (bits / factor) % n;
	}

	wxLogDebug(wxT("RNG pooled returns %u"), val);

	return val;
}

void RNG::load(wxDataInputStream &stream)
{
	stream >> mIndex;

	wxUint32 size;
	stream >> size;

	// They should be the same, but it won't crash us if they're not, just
	// make sure someone gets the message in Debug mode.
	wxASSERT(POOL_SIZE == size);

	mPool.reset(new wxUint32[size]);
	stream.Read32(mPool.get(), size);
}

void RNG::save(wxDataOutputStream &stream) const
{
	stream << mIndex;
	stream << POOL_SIZE;
	stream.Write32(mPool.get(), POOL_SIZE);
}

//---------------------------- PROTECTED        -----------------------------//

//---------------------------- PRIVATE          -----------------------------//
RNG::RNG()
: mIndex(0)
{
	repool();

	// Seed the unpooled generator only once.
	sUnpooledGenerator.seed(time(NULL));
}

const wxUint32 RNG::next() const
{
	// If we're over the end, wrap back around.  This should not happen often,
	// if ever.
	if(POOL_SIZE <= mIndex)
	{
		wxLogDebug(wxT("RNG count exceeded %d.  Wrapping back to zero."), 
			POOL_SIZE);
		mIndex = 0;
	}

	return mPool[mIndex++];
}
