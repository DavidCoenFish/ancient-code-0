//
//  GSceneNodeComponentCollisionResult.h
//
//  Created by David Coen on 2011 02 21
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNodeComponentCollisionResult_h_
#define _GSceneNodeComponentCollisionResult_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "GVector3Float.h"

class GSceneNodeComponentCollision;
class GMatrix9Float;

/*
	result of a collision
*/
class GSceneNodeComponentCollisionResult : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GSceneNodeComponentCollision> TPointerSceneNodeComponentCollision;
	typedef boost::weak_ptr<GSceneNodeComponentCollision> TWeakSceneNodeComponentCollision;

	//constructor
public:
	GSceneNodeComponentCollisionResult(
		TWeakSceneNodeComponentCollision& in_weakOwner,
		const float in_hitRatio,
		const GVector3Float& in_hitSurfaceNormal,
		const bool in_hitSurfaceNormalalised
		);
	~GSceneNodeComponentCollisionResult();

	//public methods
public:
	void PutNormalIntoWorldSpace(const GMatrix9Float& in_worldRotation);

	//public accessors
public:
	const float GetHitRatio()const{ return mHitRatio; }
	const GVector3Float& GetHitSuraceNormal();

	TWeakSceneNodeComponentCollision& GetWeakComponentCollision(){ return mWeakComponentCollision; }

	//private members
private:
	const float mHitRatio;
	GVector3Float mHitSurfaceNormal; //lazy, normalise on demand
	bool mHitSurfaceNormalalised;

	TWeakSceneNodeComponentCollision mWeakComponentCollision;

};

#endif 