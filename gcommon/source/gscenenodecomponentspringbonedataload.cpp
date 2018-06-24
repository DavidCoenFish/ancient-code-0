//
//  GSceneNodeComponentSpringBoneDataLoad.cpp
//
//  Created by David Coen on 2011 01 11
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneNodeComponentSpringBoneDataLoad.h"

//constructor
GSceneNodeComponentSpringBoneDataLoad::GSceneNodeComponentSpringBoneDataLoad(
	const char* const in_boneName,
	const GVector3Float& in_centerMass,
	const float in_springConstant,
	const float in_springDampen,
	const float in_springMax,
	const int in_flag
	)
	: mBoneName(in_boneName)
	, mCenterMass(in_centerMass)
	, mSpringConstant(in_springConstant)
	, mSpringDampen(in_springDampen)
	, mSpringMax(in_springMax)
	, mFlag(in_flag)
{
	return;
}

GSceneNodeComponentSpringBoneDataLoad::~GSceneNodeComponentSpringBoneDataLoad()
{
	return;
}

const bool GSceneNodeComponentSpringBoneDataLoad::GetTargetDown()const
{
	return (0 != (mFlag & TFlag::TTargetDown));
}

const bool GSceneNodeComponentSpringBoneDataLoad::GetDebug()const
{
	return (0 != (mFlag & TFlag::TDebug));
}

