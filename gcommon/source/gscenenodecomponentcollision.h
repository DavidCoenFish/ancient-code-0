//
//  GSceneNodeComponentCollision.h
//
//  Created by David Coen on 2011 02 18
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNodeComponentCollision_h_
#define _GSceneNodeComponentCollision_h_

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>
#include <string>
#include "GSceneNodeComponentBase.h"

class GAsset;
class GScene;
class GSceneVisitorRender;
class GSceneNode;
class GScenePostLoadCallback;
class GSceneNodeComponentCollisionLoad;
class GMaterial;
class GMaterialInstance;
class GMesh;
class GMeshInstance;
class GCamera;
class GSceneNodeComponentCollisionDataBase;
class GSceneNodeComponentCollisionResult;
class GVector3Float;

/**/
class GSceneNodeComponentCollision : public GSceneNodeComponentBase
{
	//typedef
private:
	typedef boost::shared_ptr<GSceneNodeComponentBase> TPointerSceneNodeComponentBase;
	typedef boost::shared_ptr<GSceneNodeComponentCollision> TPointerSceneNodeComponentCollision;
	typedef boost::weak_ptr<GSceneNodeComponentCollision> TWeakSceneNodeComponentCollision;

	typedef boost::shared_ptr<GAsset> TPointerBuffer;
	typedef boost::weak_ptr<GAsset> TWeakAsset;

	typedef boost::shared_ptr<GMaterialInstance> TPointerMaterialInstance;
	typedef boost::weak_ptr<GMaterialInstance> TWeakMaterialInstance;

	typedef std::vector<GScenePostLoadCallback> TArrayScenePostLoadCallback;

	typedef boost::shared_ptr<GMeshInstance> TPointerRenderMeshInstance;
	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;

	typedef boost::shared_ptr<GSceneNodeComponentCollisionDataBase> TPointerSceneNodeComponentCollisionDataBase;
	typedef std::vector<TPointerSceneNodeComponentCollisionDataBase> TArrayPointerSceneNodeComponentCollisionDataBase;

	typedef boost::shared_ptr<GSceneNodeComponentCollisionResult> TPointerSceneNodeComponentCollisionResult;
	typedef std::vector<TPointerSceneNodeComponentCollisionResult> TArrayPointerSceneNodeComponentCollisionResult;

	//static public methods
public:
	//different code path adds components to node/ scene. may want to create without 'activation'
	//generic interface for generalised loading. return base rather than type pointer due to generic interface
	static TPointerSceneNodeComponentBase Factory(
		GScene& inout_scene, //access to mesh/ material manager
		TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
		const void* const in_componentData
		);
	static const unsigned int GetComponentFlag();

	//constructor
public:
	GSceneNodeComponentCollision(
		GSceneNode& in_parentSceneNode,
		const int in_materialId,
		const std::string& in_data,
		TArrayPointerSceneNodeComponentCollisionDataBase& in_arrayData, //WARN: transfer ownership
		const bool in_active
		);
	virtual ~GSceneNodeComponentCollision();

	//public methods
public:
	void CollideRay(
		TArrayPointerSceneNodeComponentCollisionResult& inout_collisionResult,
		TWeakSceneNodeComponentCollision& in_weakOwner,
		const bool in_wantHitSurfaceNormalObjectSpace,
		const bool in_wantHitSurfaceNormalWorldSpace,
		const GVector3Float& in_worldSpaceRayOrigin,
		const GVector3Float& in_worldSpaceRayDirection
		);

	/*WARNING
		cone collision detection is correct, but it currently doenst calcualte where on the shape the
		collision occured (works by closest point on ray to the shape)
	*/
	void CollideCone(
		TArrayPointerSceneNodeComponentCollisionResult& inout_collisionResult,
		TWeakSceneNodeComponentCollision& in_weakOwner,
		const bool in_wantHitSurfaceNormalObjectSpace,
		const bool in_wantHitSurfaceNormalWorldSpace,
		const GVector3Float& in_worldSpaceRayOrigin,
		const GVector3Float& in_worldSpaceRayDirection,
		const float in_coneLength,
		const float in_radiusOne,
		const float in_radiusTwo
		);

	//public accessors
public:
	const bool GetActive()const{ return mActive; }
	void SetActive(const bool in_active){ mActive = in_active; return; }
	const std::string& GetData(){ return mData; }

	//private members;
private:
	GSceneNode& mParentSceneNode;
	const int mMaterialId;
	const std::string mData;
	TArrayPointerSceneNodeComponentCollisionDataBase mArrayData;
	bool mActive;

};

#endif 