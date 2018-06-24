//
//  GSceneNodeComponentCollisionDataBox.h
//
//  Created by David Coen on 2011 02 21
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNodeComponentCollisionDataBox_h_
#define _GSceneNodeComponentCollisionDataBox_h_

#include "GSceneNodeComponentCollisionDataBase.h"
#include "GVector3Float.h"

/**/
class GSceneNodeComponentCollisionDataBox : public GSceneNodeComponentCollisionDataBase
{
	//typedef
private:
	typedef boost::shared_ptr<GSceneNodeComponentCollisionResult> TPointerSceneNodeComponentCollisionResult;
	typedef std::vector<TPointerSceneNodeComponentCollisionResult> TArrayPointerSceneNodeComponentCollisionResult;

	typedef boost::shared_ptr<GSceneNodeComponentCollision> TPointerSceneNodeComponentCollision;
	typedef boost::weak_ptr<GSceneNodeComponentCollision> TWeakSceneNodeComponentCollision;

	//constructor
public:
	GSceneNodeComponentCollisionDataBox(
		const GVector3Float& in_low,
		const GVector3Float& in_high
		);
	~GSceneNodeComponentCollisionDataBox();

	//implement GSceneNodeComponentCollisionDataBase
private:
	virtual void OnCollideRay(
		TArrayPointerSceneNodeComponentCollisionResult& inout_collisionResult,
		TWeakSceneNodeComponentCollision& in_weakOwner,
		const bool in_wantHitSurfaceNormal,
		const GVector3Float& in_objectSpaceRayOrigin,
		const GVector3Float& in_objectSpaceRayDirection
		);

	virtual void OnCollideCone(
		TArrayPointerSceneNodeComponentCollisionResult& inout_collisionResult,
		TWeakSceneNodeComponentCollision& in_weakOwner,
		const bool in_wantHitSurfaceNormal,
		const GVector3Float& in_objectSpaceRayOrigin,
		const GVector3Float& in_objectSpaceRayDirection,
		const float in_coneLength,
		const float in_radiusOne,
		const float in_radiusTwo
		);

	//private members
private:
	const GVector3Float mLow;
	const GVector3Float mHigh;

};

#endif 