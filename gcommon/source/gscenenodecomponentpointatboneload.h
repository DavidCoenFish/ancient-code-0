//
//  GSceneNodeComponentPointAtBoneLoad.h
//
//  Created by David Coen on 2011 01 11
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNodeComponentPointAtBoneLoad_h_
#define _GSceneNodeComponentPointAtBoneLoad_h_

#include <boost/noncopyable.hpp>
#include "GVector3Float.h"

class GSceneNodeLoad;

/*
param for manual creation of component, or memory image for load from buffer
*/
class GSceneNodeComponentPointAtBoneLoad : public boost::noncopyable
{
	//constructor
public:
	GSceneNodeComponentPointAtBoneLoad(
		const char* const in_boneName,
		const float in_maxAngleDegree,
		const GVector3Float& in_localAt,
		const GVector3Float& in_worldTarget
		);
	~GSceneNodeComponentPointAtBoneLoad();

	//public accessors
public:
	const char* const GetBoneName()const{ return m_boneName; }
	const float GetMaxAngleDegree()const{ return m_maxAngleDegree; }
	const GVector3Float& GetLocalAt()const{ return m_localAt; }
	const GVector3Float& GetWorldTarget()const{ return m_worldTarget; }

	//private members;
private:
	const char* const m_boneName;
	const float m_maxAngleDegree;
	const GVector3Float m_localAt;
	const GVector3Float m_worldTarget;

};

#endif 