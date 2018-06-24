//
//  GSceneNodeComponentAnimation.h
//
//  Created by David Coen on 10/10/05.
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNodeComponentAnimation_h_
#define _GSceneNodeComponentAnimation_h_

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>
#include "GSceneNodeComponentBase.h"

class GScenePostLoadCallback;
class GScene;
class GSceneNode;
class GBuffer;
class GSceneNodeComponentAnimationAction;
class GAnimationManagerInstance;
class GMeshInstance;
class GVector3Float;
class GSkeletonInstance;

/*
own a array of 'actions' to deal with updating things, so a action to update a bone in a meshExtend with index of streams in use
or an action to change a material colour, or a node position
*/
class GSceneNodeComponentAnimation : public GSceneNodeComponentBase
{
	//typedef
private:
	typedef boost::shared_ptr<GSceneNodeComponentBase> TPointerSceneNodeComponentBase;
	typedef boost::shared_ptr<GSceneNodeComponentAnimation> TPointerSceneNodeComponentAnimation;

	typedef boost::shared_ptr<GBuffer> TPointerFile;
	typedef boost::weak_ptr<GBuffer> TWeakFile;

	typedef boost::shared_ptr<GSceneNodeComponentAnimationAction> TPointerSceneNodeComponentAnimationAction;
	typedef std::vector<TPointerSceneNodeComponentAnimationAction> TArrayPointerSceneNodeComponentAnimationAction;

	typedef std::vector<GScenePostLoadCallback> TArrayScenePostLoadCallback;

	typedef boost::shared_ptr<GAnimationManagerInstance> TPointerAnimationManagerInstance;
	typedef boost::shared_ptr<GMeshInstance> TPointerMeshInstance;
	typedef boost::weak_ptr<GMeshInstance> TWeakMeshInstance;

	typedef boost::shared_ptr<GSkeletonInstance> TPointerSkeletonInstance;
	typedef std::vector<TPointerSkeletonInstance> TArrayPointerSkeletonInstance;
	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;

	typedef void (*TBoneUpdateCallback)(
		GVector3Float& inout_at,
		GVector3Float& inout_up,
		GVector3Float& inout_pos,
		float& inout_scale,
		GSceneNode& in_parentSceneNode,
		const float in_timeDelta,
		void* in_callbackData,
		GSkeletonInstance& inout_skeletonInstance
		);

	//static public methods
public:
	//different code path adds components to node/ scene. may want to create without 'activation'
	//generic interface for generalised loading
	static TPointerSceneNodeComponentBase Factory(
		GScene& inout_scene, //access to mesh/ material manager
		TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
		const void* const in_componentData
		);

	static void PostLoadCallback(
		GScene& inout_scene, //access to mesh/ material manager
		TPointerSceneNode& inout_owner,
		TPointerSceneNodeComponentBase& inout_component,
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback
		);

	static const unsigned int GetComponentFlag();

	//constructor
public:
	GSceneNodeComponentAnimation(
		TPointerAnimationManagerInstance in_animationManagerInstance
		);
	virtual ~GSceneNodeComponentAnimation();

	//public methods
public:
	//todo: a universal time int to help keep scene animations in sync?
	void Tick(TPointerSceneNode& inout_sceneNode, const float in_timeDelta);

	TPointerAnimationManagerInstance& GetAnimationManagerInstance(){ return mAnimationManagerInstance; }

	const int GetSkeletonCount()const;
	//we own the skeleton, but silly to return weak pointer? QUERY: policy
	TPointerSkeletonInstance& GetSkeleton(const int in_skeletonIndex);

	const bool AddBoneCallback(
		const int in_boneIndex,
		TPointerSkeletonInstance& inout_skeletonInstance,
		const TBoneUpdateCallback in_boneUpdateCallback,
		void* const in_callbackData
		);

	//private methods
private:
	void AttachToNode(GScene& inout_scene, TPointerSceneNode& inout_owner);

	//private members;
private:
	TPointerAnimationManagerInstance mAnimationManagerInstance;

	TArrayPointerSceneNodeComponentAnimationAction mArrayAction;

	TArrayPointerSkeletonInstance mArraySkeletonInstance;

};

#endif 