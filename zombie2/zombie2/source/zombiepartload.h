//
//  ZombiePartLoad.h
//  Zombie
//
//  Created by David Coen on 2011 02 28
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _ZombiePartLoad_h_
#define _ZombiePartLoad_h_

#include <boost/noncopyable.hpp>

struct ZombiePartDataLoad;

struct ZombiePartLoad //: public boost::noncopyable
{
	//constructor
public:
	//ZombiePartLoad(
	//	const char* const in_partName,
	//	const int in_arrayDataCount,
	//	const ZombiePartDataLoad* const in_arrayData,
	//	const int in_arrayParentCount,
	//	const int* const in_arrayParent
	//	);
	//~ZombiePartLoad();

	//public accessors
public:
	const char* const GetPartName()const{ return mPartName; }

	const int GetArrayDataCount()const{ return mArrayDataCount; }
	const ZombiePartDataLoad* const GetArrayData()const{ return mArrayData; }

	const int GetArrayParentCount()const{ return mArrayParentCount; }
	const int* const GetArrayParent()const{ return mArrayParent; }

	//const bool GetSkinned()const{ return mSkinned; }

	//private members
public:
	const char* const mPartName; // soft bind to data in visual/ collision
	const int mArrayDataCount;
	const ZombiePartDataLoad* const mArrayData;
	const int mArrayParentCount;
	const int* const mArrayParent; //if all the non GCOMMON_INVALID_INDEX parents are dead, then this part should be dead too (detach)
	//const bool mSkinned;

};

#endif //_ZombiePartLoad_h_
