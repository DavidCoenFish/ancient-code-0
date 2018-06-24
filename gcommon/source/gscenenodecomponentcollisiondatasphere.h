//
//  GSceneNodeComponentCollisionDataSphere.h
//
//  Created by David Coen on 2011 02 21
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNodeComponentCollisionDataSphere_h_
#define _GSceneNodeComponentCollisionDataSphere_h_

#include "GSceneNodeComponentCollisionDataBase.h"
#include "GVector3Float.h"
/**/
class GSceneNodeComponentCollisionDataSphere : public GSceneNodeComponentCollisionDataBase
{
	//typedef
private:
	typedef boost::shared_ptr<GSceneNodeComponentCollisionResult> TPointerSceneNodeComponentCollisionResult;
	typedef std::vector<TPointerSceneNodeComponentCollisionResult> TArrayPointerSceneNodeComponentCollisionResult;

	typedef boost::shared_ptr<GSceneNodeComponentCollision> TPointerSceneNodeComponentCollision;
	typedef boost::weak_ptr<GSceneNodeComponentCollision> TWeakSceneNodeComponentCollision;

	//constructor
public:
	GSceneNodeComponentCollisionDataSphere(
		const float in_radius,
		const GVector3Float& in_origin
		);
	~GSceneNodeComponentCollisionDataSphere();

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

	//private members;
private:
	const float mRadius;
	const GVector3Float mOrigin;

};

#endif 