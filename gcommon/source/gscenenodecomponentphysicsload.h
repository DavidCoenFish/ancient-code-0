//
//  GSceneNodeComponentPhysicsLoad.h
//
//  Created by David Coen on 2011 02 18
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNodeComponentPhysicsLoad_h_
#define _GSceneNodeComponentPhysicsLoad_h_

#include <boost/noncopyable.hpp>
#include "GMatrix16Float.h"

class GSceneNodeLoad;

/*
param for manual creation of component, or memory image for load from buffer
*/
class GSceneNodeComponentPhysicsLoad : public boost::noncopyable
{
	//typedef
public:
	//enum via header included by pipeline packScene.cpp for
	struct TFlag
	{
		enum TEnum 
		{
			TNone			= 0x0000,
			TAllowMove		= 0x0001,
			TAllowRotate	= 0x0002,
			TFreeze			= 0x0004,
			TBounceYZero	= 0x0008
		};
	};

	//constructor
public:
	GSceneNodeComponentPhysicsLoad(
		const float in_massInvert,
		const GVector3Float* const in_initialVelocity,
		const int in_flag
		);
	~GSceneNodeComponentPhysicsLoad();

	//public accessors
public:
	const float GetMassInvert()const{ return mMassInvert; }
	const GVector3Float* const GetInitialVelocity()const{return mInitialVelocity; }
	const int GetFlag()const{ return mFlag; }

	//private members;
private:
	const float mMassInvert;
	const GVector3Float* const mInitialVelocity;
	const int mFlag;
};

#endif 