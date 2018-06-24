//
//  GSceneNodeComponentPointAtBoneLoad.cpp
//
//  Created by David Coen on 2011 01 11
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneNodeComponentPointAtBoneLoad.h"

//constructor
GSceneNodeComponentPointAtBoneLoad::GSceneNodeComponentPointAtBoneLoad(
	const char* const in_boneName,
	const float in_maxAngleDegree,
	const GVector3Float& in_localAt,
	const GVector3Float& in_worldTarget
	)
	: m_boneName(in_boneName)
	, m_maxAngleDegree(in_maxAngleDegree)
	, m_localAt(in_localAt)
	, m_worldTarget(in_worldTarget)
{
	return;
}

GSceneNodeComponentPointAtBoneLoad::~GSceneNodeComponentPointAtBoneLoad()
{
	return;
}

