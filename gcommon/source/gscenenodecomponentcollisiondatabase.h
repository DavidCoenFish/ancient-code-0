//
//  GSceneNodeComponentCollisionDataBase.h
//
//  Created by David Coen on 2011 02 21
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNodeComponentCollisionDataBase_h_
#define _GSceneNodeComponentCollisionDataBase_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>

class GSceneNodeComponentCollisionResult;
class GSceneNodeComponentCollision;
class GVector3Float;

class GSceneNodeComponentCollisionDataBase : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GSceneNodeComponentCollisionDataBase> TPointerSceneNodeComponentCollisionDataBase;
	typedef std::vector<TPointerSceneNodeComponentCollisionDataBase> TArrayPointerSceneNodeComponentCollisionDataBase;

	typedef boost::shared_ptr<GSceneNodeComponentCollisionResult> TPointerSceneNodeComponentCollisionResult;
	typedef std::vector<TPointerSceneNodeComponentCollisionResult> TArrayPointerSceneNodeComponentCollisionResult;

	typedef boost::shared_ptr<GSceneNodeComponentCollision> TPointerSceneNodeComponentCollision;
	typedef boost::weak_ptr<GSceneNodeComponentCollision> TWeakSceneNodeComponentCollision;

	//constructor
public:
	GSceneNodeComponentCollisionDataBase();
	virtual ~GSceneNodeComponentCollisionDataBase();

	//public methods
public:
	void CollideRay(
		TArrayPointerSceneNodeComponentCollisionResult& inout_collisionResult,
		TWeakSceneNodeComponentCollision& in_weakOwner,
		const bool in_wantHitSurfaceNormal,
		const GVector3Float& in_objectSpaceRayOrigin,
		const GVector3Float& in_objectSpaceRayDirection
		);

	void CollideCone(
		TArrayPointerSceneNodeComponentCollisionResult& inout_collisionResult,
		TWeakSceneNodeComponentCollision& in_weakOwner,
		const bool in_wantHitSurfaceNormal,
		const GVector3Float& in_objectSpaceRayOrigin,
		const GVector3Float& in_objectSpaceRayDirection,
		const float in_coneLength,
		const float in_radiusOne,
		const float in_radiusTwo
		);

	//private interface
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

};

#endif 