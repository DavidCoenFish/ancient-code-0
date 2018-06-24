//
//  GSceneNodeComponentAnimationAction.h
//
//  Created by David Coen on 10/10/05.
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNodeComponentAnimationAction_h_
#define _GSceneNodeComponentAnimationAction_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>

class GSceneNode;
class GSceneNodeComponentVisual;
class GAnimationInstanceMixer;
class GVector3Float;
class GMatrix16Float;
class GAnimationManagerInstance;
class GMeshInstance;
class GSkeletonInstance;

/**/
class GSceneNodeComponentAnimationAction : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GAnimationManagerInstance> TPointerAnimationManagerInstance;

	typedef boost::shared_ptr<GSceneNodeComponentAnimationAction> TPointerSceneNodeComponentAnimationAction;
	typedef std::vector<TPointerSceneNodeComponentAnimationAction> TArrayPointerSceneNodeComponentAnimationAction;

	//typedef boost::shared_ptr<GMeshInstance> TPointerRenderMeshInstance;
	//typedef boost::weak_ptr<GMeshInstance> TWeakRenderMeshInstance;

	typedef boost::shared_ptr<GSkeletonInstance> TPointerSkeletonInstance;
	typedef boost::weak_ptr<GSkeletonInstance> TWeakSkeletonInstance;

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

	//public static methods
public:
	static void Factory(
		TArrayPointerSceneNodeComponentAnimationAction& out_arrayAction,
		TPointerAnimationManagerInstance& in_animationManager,
		TPointerSkeletonInstance& out_arraySkeletonInstance
		);

	//constructor
public:
	GSceneNodeComponentAnimationAction(
		const int in_type,
		const int in_streamIndexPosition,
		const int in_streamIndexRotation,
		const int in_streamIndexScale,
		TPointerSkeletonInstance& inout_skeletonInstance,
		const int in_boneIndex
		);
	~GSceneNodeComponentAnimationAction();

	//public methods
public:
	void Apply(
		GSceneNode& in_sceneNode,
		TPointerAnimationManagerInstance& in_animationManager,
		const float in_timeDelta
		)const;

	//public Accessors
public:
	const bool TargetIsBone(const int in_boneIndex, TPointerSkeletonInstance& in_skeletonInstance)const;
	void AddBoneUpdateCallback(
		const TBoneUpdateCallback in_boneUpdateCallback,
		void* const in_boneCallbackData
		);

	//private methods;
private:
	void ApplyVisualBoneAnimation(
		GSceneNode& in_sceneNode,
		TPointerAnimationManagerInstance& in_animationManager,
		const float in_timeDelta //we don't tick the animation manager, but manual bone update could want to know what the time delta is
		)const;

	void ApplyMorphTargetAnimation(TPointerAnimationManagerInstance& in_animationManager)const;

	//private members;
private:
	const int mType;

	int mStreamIndexPosition;
	int mStreamIndexRotation;
	int mStreamIndexScale;

	TWeakSkeletonInstance mWeakSkeletonInstance;
	int mBoneIndex;

	TBoneUpdateCallback mBoneUpdateCallback;
	void* mBoneCallbackData;

};

#endif 