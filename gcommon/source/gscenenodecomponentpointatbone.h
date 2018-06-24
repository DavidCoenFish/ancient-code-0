//
//  GSceneNodeComponentPointAtBone.h
//
//  Created by David Coen on 2010/10/11.
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNodeComponentPointAtBone_h_
#define _GSceneNodeComponentPointAtBone_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include "GSceneNodeComponentBase.h"
#include "GVector3Float.h"

class GSceneNodeComponentPointAtBoneLoad;
class GMatrix16Float;
class GScene;
class GSceneNode;
class GScenePostLoadCallback;
class GSkeletonInstance;

/**/
class GSceneNodeComponentPointAtBone : public GSceneNodeComponentBase
{
	//typedef
private:
	typedef boost::shared_ptr<GSceneNodeComponentBase> TPointerSceneNodeComponentBase;
	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;
	typedef std::vector<GScenePostLoadCallback> TArrayScenePostLoadCallback;

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

	static void PostLoadCallbackA(
		GScene& inout_scene, //access to mesh/ material manager
		TPointerSceneNode& inout_owner,
		TPointerSceneNodeComponentBase& inout_component,
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback
		);

	static void PostLoadCallbackB(
		GScene& inout_scene, //access to mesh/ material manager
		TPointerSceneNode& inout_owner,
		TPointerSceneNodeComponentBase& inout_component,
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback
		);

	static const unsigned int GetComponentFlag();

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

	//constructor
public:
	GSceneNodeComponentPointAtBone(
		const std::string& in_boneName = "",
		const float in_maxAngleDegree = 45.0F,
		const GVector3Float& in_localAt = GVector3Float::sUnitX,
		const GVector3Float& in_worldTarget = GVector3Float::sZero
#ifdef DSC_DEBUG
		, GScene* const in_scene = NULL
#endif
		);
	~GSceneNodeComponentPointAtBone();

	//public accessors
public:
	const std::string& GetName()const{ return m_boneName; }
	void SetBoneIndex(const int in_boneIndex){ m_boneIndex = in_boneIndex; return; }
	void SetWorldTarget(const GVector3Float& in_worldTarget){ m_worldTarget = in_worldTarget; return; }

	const GR32 GetMaxAngleRadian()const{ return m_maxAngleRadian; }
	const GR32 GetCurrentAngleRadian()const{ return m_currentAngleRadian; }

	//private members;
private:
	const std::string m_boneName;
	const float m_maxAngleRadian;
	const GVector3Float m_localAt;
	int m_boneIndex;
	GVector3Float m_worldTarget;
	GR32 m_currentAngleRadian;

	//GVector3Float m_previousLocalAt;

#ifdef DSC_DEBUG
	GScene* mScene;
#endif

};

#endif 