//
//  SceneNodeComponentClientActor.h
//  Zombie
//
//  Created by David Coen on 2011 02 16
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _SceneNodeComponentClientActor_h_
#define _SceneNodeComponentClientActor_h_

#include "SceneNodeComponentClientObjectBase.h"
#include <boost/shared_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <map>

class ZombieMission;
class ZombiePart;
class GSceneNodeComponentAnimation;
class GSceneNodeComponentCollision;
class GSceneNodeComponentCollisionResult;
class GComponentLerpVector3Float;

class SceneNodeComponentClientActor : public SceneNodeComponentClientObjectBase
{
	//typedef
private:
	typedef boost::shared_ptr<GSceneNodeComponentBase> TPointerSceneNodeComponentBase;
	typedef boost::shared_ptr<SceneNodeComponentClientActor> TPointerSceneNodeComponentClientActor;
	typedef std::vector<GScenePostLoadCallback> TArrayScenePostLoadCallback;

	typedef boost::shared_ptr<GSceneNodeComponentCollisionResult> TPointerSceneNodeComponentCollisionResult;
	typedef std::pair<SceneNodeComponentClientObjectBase*, TPointerSceneNodeComponentCollisionResult> TPairClientObjectCollisionResult;
	typedef std::vector<TPairClientObjectCollisionResult> TArrayPairClientObjectCollisionResult;

	typedef boost::shared_ptr<GSceneNodeComponentAnimation> TPointerSceneNodeComponentAnimation;
	typedef boost::weak_ptr<GSceneNodeComponentAnimation> TWeakSceneNodeComponentAnimation;

	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;
	typedef boost::weak_ptr<GSceneNode> TWeakSceneNode;
	typedef boost::shared_ptr<GComponentLerpVector3Float> TPointerComponentLerpVector3Float;

	//static public methods
public:
	static TPointerSceneNodeComponentBase Factory(
		GScene& inout_scene,
		TPointerSceneNode& inout_owner,
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
		void* const in_data //address of ApplicationTaskGame
		);

	//static private methods
private:
	static void PostLoadCallback(
		GScene& inout_scene, //access to mesh/ material manager
		TPointerSceneNode& inout_owner,
		TPointerSceneNodeComponentBase& inout_component,
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback
		);

	//constructor
public:
	SceneNodeComponentClientActor(
		ZombieMission& in_parent,
		TPointerSceneNode& in_pointerParent
		);
	virtual ~SceneNodeComponentClientActor();

	//implement GSceneNodeComponentClient
private:
	virtual void OnTick(TPointerSceneNode& inout_sceneNode, const float in_timeDelta);

	//implement SceneNodeComponentClientObjectBase
private:
	virtual void OnCollectDamageReceivers(
		TArrayPairClientObjectCollisionResult& out_arrayClientObjectCollisionResult,
		const GVector3Float& in_worldSpaceConeOrigin,
		const GVector3Float& in_worldSpaceConeDirection,
		const float in_coneLength,
		const float in_coneRadiusPoint,
		const float in_coneRadiusBase
		);

	virtual void OnApplyDamage(
		float& inout_damage, 
		const std::string& in_data
		);

	virtual void OnAfterDamagePass(GScene& inout_scene, TPointerSceneNode& inout_sceenRoot);

	virtual void OnAnimate(
		const std::string& in_animationName, 
		const GAnimationPlayParam& in_playParam
		);

	virtual void OnMove(
		const GVector3Float& in_position,
		const float in_transitionTime,
		const int in_transitionType
		);

	virtual void OnRotate(
		const GVector3Float& in_at,
		const GVector3Float& in_up,
		const float in_transitionTime,
		const int in_transitionType
		);
	virtual void OnDestroy();

	//private methods
private:

	//private members
private:
	ZombieMission& mParentMission; //to destroy node on death
	TWeakSceneNode mWeakParent; //scene visitor collision wants parent node smart pointer
	TWeakSceneNodeComponentAnimation mAnimation;

	TPointerComponentLerpVector3Float mLerpPosition;
	TPointerComponentLerpVector3Float mLerpAt;
	TPointerComponentLerpVector3Float mLerpUp;

	float mHealth;
	bool mTransformDirty;

	bool mDead;
	float mDeathCountDown;

};

#endif //_SceneNodeComponentClientActor_h_
