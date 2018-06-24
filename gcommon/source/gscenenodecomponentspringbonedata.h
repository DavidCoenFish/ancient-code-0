//
//  GSceneNodeComponentSpringBoneData.h
//
//  Created by David Coen on 2010/10/11.
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNodeComponentSpringBoneData_h_
#define _GSceneNodeComponentSpringBoneData_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include "GVector3Float.h"
#include "GPhysicsComponentPosition.h"
#include "GPhysicsComponentSpring.h"
#include "GPhysicsComponentLimit.h"

class GSceneNodeComponentSpringBoneDataLoad;
class GMatrix16Float;
class GScene;
class GSceneNode;
class GMeshInstance;
class GSkeletonInstance;

/**/
class GSceneNodeComponentSpringBoneData : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GSceneNodeComponentSpringBoneData> TPointerSceneNodeComponentSpringBoneData;
	typedef boost::shared_ptr<GMeshInstance> TPointerRenderMeshInstance;

	//static public methods
public:
	static TPointerSceneNodeComponentSpringBoneData Factory(
		const GSceneNodeComponentSpringBoneDataLoad& in_data
#ifdef DSC_DEBUG
		, GScene* const in_scene
#endif
		);

	static void BoneUpdateCallback(
		GVector3Float& inout_at,
		GVector3Float& inout_up,
		GVector3Float& inout_pos,
		float& inout_scale,
		GSceneNode& in_parentSceneNode,
		const float in_timeDelta,
		void* in_callbackData,
		GSkeletonInstance& inout_skeletonInstance
		);

	static void SetWorldJiggle(
		const GVector3Float& in_worldJiggle
		);

	//constructor
public:
	GSceneNodeComponentSpringBoneData(
		const std::string& in_boneName = "",
		const GVector3Float& in_centerMass = GVector3Float::sZero,
		const float in_constant = 0.0F,
		const float in_dampen = 0.0F,
		const float in_max = 0.0F,
		const bool in_targetDown = false
#ifdef DSC_DEBUG
		, const bool in_debug = false
		, GScene* const in_scene = NULL
#endif
		);
	~GSceneNodeComponentSpringBoneData();

	//public accessors
public:
	const std::string& GetName()const{ return mBoneName; }
	void SetBoneIndex(const int in_boneIndex){ mBoneIndex = in_boneIndex; return; }

	//private members;
private:
	const std::string mBoneName;
	int mBoneIndex;
	const GVector3Float mCenterMass;
	//const float mTargetLength;

	GVector3Float mOldTargetWorld;

	GPhysicsComponentPosition mComponentPosition;
	const GPhysicsComponentSpring mComponentSpring;
	const GPhysicsComponentLimit mComponentLimit;
	const float mLength;
	const bool mTargetDown;

#ifdef DSC_DEBUG
	const bool mDebug;
	GScene* mScene;
#endif

};

#endif 