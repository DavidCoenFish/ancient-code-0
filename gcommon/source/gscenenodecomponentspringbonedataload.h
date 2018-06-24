//
//  GSceneNodeComponentSpringBoneDataLoad.h
//
//  Created by David Coen on 2011 01 11
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNodeComponentSpringBoneDataLoad_h_
#define _GSceneNodeComponentSpringBoneDataLoad_h_

#include <boost/noncopyable.hpp>
#include "GVector3Float.h"

class GSceneNodeLoad;

/*
param for manual creation of component, or memory image for load from buffer
*/
class GSceneNodeComponentSpringBoneDataLoad : public boost::noncopyable
{
	//typedef
public:
	struct TFlag
	{
		enum TEnum
		{
			TNone		= 0x00,
			TDebug		= 0x01,
			TTargetDown	= 0x02
		};

	};
	//constructor
public:
	GSceneNodeComponentSpringBoneDataLoad(
		const char* const in_boneName,
		const GVector3Float& in_centerMass,
		const float in_springConstant,
		const float in_springDampen,
		const float in_springMax,
		const int in_flag
		);
	~GSceneNodeComponentSpringBoneDataLoad();

	//public accessors
public:
	const char* const GetBoneName()const{ return mBoneName; }
	const GVector3Float& GetCenterMass()const{ return mCenterMass; }
	const float GetSpringConstant()const{ return mSpringConstant; }
	const float GetSpringDampen()const{ return mSpringDampen; }
	const float GetSpringMax()const{ return mSpringMax; } //max distance current point can go from target
	const bool GetTargetDown()const;
	const bool GetDebug()const;

	//private members;
private:
	const char* const mBoneName;
	const GVector3Float mCenterMass;
	const float mSpringConstant;
	const float mSpringDampen;
	const float mSpringMax; //max distance current point can go from target
	const int mFlag;

};

#endif 