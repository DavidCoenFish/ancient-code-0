//
//  GSceneVisitorCollision.h
//
//  Created by David Coen on 2011 02 21
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneVisitorCollision_h_
#define _GSceneVisitorCollision_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>
#include <string>
#include "GVector3Float.h"

class GScene;
class GSceneNode;
class GSceneNodeComponentCollision;
class GSceneNodeComponentCollisionResult;

/**/
class GSceneVisitorCollisionCone : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;

	typedef boost::shared_ptr<GSceneNodeComponentCollisionResult> TPointerSceneNodeComponentCollisionResult;
	typedef std::vector<TPointerSceneNodeComponentCollisionResult> TArrayPointerSceneNodeComponentCollisionResult;

	//public static methods
public:
	static void Run(
		GScene& inout_scene,
		TArrayPointerSceneNodeComponentCollisionResult& inout_collisionResult,
		const bool in_wantHitSurfaceNormalObjectSpace,
		const bool in_wantHitSurfaceNormalWorldSpace,
		const GVector3Float& in_worldSpaceRayOrigin,
		const GVector3Float& in_worldSpaceRayDirection,
		const float in_coneLength,
		const float in_radiusOne,
		const float in_radiusTwo
		);

	static void Run(
		TPointerSceneNode& inout_sceneNode,
		TArrayPointerSceneNodeComponentCollisionResult& inout_collisionResult,
		const bool in_wantHitSurfaceNormalObjectSpace,
		const bool in_wantHitSurfaceNormalWorldSpace,
		const GVector3Float& in_worldSpaceRayOrigin,
		const GVector3Float& in_worldSpaceRayDirection,
		const float in_coneLength,
		const float in_radiusOne,
		const float in_radiusTwo
		);

	//constructor
private:
	GSceneVisitorCollisionCone(
		TArrayPointerSceneNodeComponentCollisionResult& inout_collisionResult,
		const bool in_wantHitSurfaceNormalObjectSpace,
		const bool in_wantHitSurfaceNormalWorldSpace,
		const GVector3Float& in_worldSpaceRayOrigin,
		const GVector3Float& in_worldSpaceRayDirection,
		const float in_coneLength,
		const float in_radiusOne,
		const float in_radiusTwo
		);
	~GSceneVisitorCollisionCone();

	//public methods
public:
	//contract with GSceneNode query
	void QuerySubmitNode(
		TPointerSceneNode& inout_node
		);
	void QueryFinish();

	//private members;
private:
	TArrayPointerSceneNodeComponentCollisionResult& mCollisionResult;
	const bool mWantHitSurfaceNormalObjectSpace;
	const bool mWantHitSurfaceNormalWorldSpace;
	const GVector3Float mWorldSpaceRayOrigin;
	const GVector3Float mWorldSpaceRayDirection;
	const float mConeLength;
	const float mRadiusOne;
	const float mRadiusTwo;

};

class GSceneVisitorCollisionCollect : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;

	typedef boost::shared_ptr<GSceneNodeComponentCollision> TPointerSceneNodeComponentCollision;
	typedef boost::weak_ptr<GSceneNodeComponentCollision> TWeakSceneNodeComponentCollision;
	typedef std::vector<TWeakSceneNodeComponentCollision> TArrayWeakSceneNodeComponentCollision;

	//public static methods
public:
	static void Run(
		TPointerSceneNode& inout_sceneNode,
		const std::string& in_data,
		TArrayWeakSceneNodeComponentCollision& out_arrayCollision
		);

	//constructor
private:
	GSceneVisitorCollisionCollect(
		const std::string& in_data,
		TArrayWeakSceneNodeComponentCollision& out_arrayCollision
		);
	~GSceneVisitorCollisionCollect();

	//public methods
public:
	//contract with GSceneNode query
	void QuerySubmitNode(
		TPointerSceneNode& inout_node
		);
	void QueryFinish();

	//private members;
private:
	const std::string mData;
	TArrayWeakSceneNodeComponentCollision& mArrayCollision;

};

#endif 