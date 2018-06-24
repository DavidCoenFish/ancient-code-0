//
//  SceneNodeComponentClientZombieFly.h
//  Zombie
//
//  Created by David Coen on 2011 03 22
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _SceneNodeComponentClientZombieFly_h_
#define _SceneNodeComponentClientZombieFly_h_

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
class GSceneNodeComponentParticle;
class ZombieComponent;

class SceneNodeComponentClientZombieFly : public SceneNodeComponentClientObjectBase
{
	//typedef
private:
	typedef boost::shared_ptr<GSceneNodeComponentBase> TPointerSceneNodeComponentBase;
	typedef boost::shared_ptr<SceneNodeComponentClientZombieFly> TPointerSceneNodeComponentClientZombieFly;
	typedef std::vector<GScenePostLoadCallback> TArrayScenePostLoadCallback;

	typedef boost::shared_ptr<GSceneNodeComponentCollisionResult> TPointerSceneNodeComponentCollisionResult;
	typedef std::pair<SceneNodeComponentClientObjectBase*, TPointerSceneNodeComponentCollisionResult> TPairClientObjectCollisionResult;
	typedef std::vector<TPairClientObjectCollisionResult> TArrayPairClientObjectCollisionResult;

	typedef boost::shared_ptr<GSceneNodeComponentCollision> TPointerSceneNodeComponentCollision;
	typedef boost::weak_ptr<GSceneNodeComponentCollision> TWeakSceneNodeComponentCollision;
	typedef std::vector<TWeakSceneNodeComponentCollision> TArrayWeakSceneNodeComponentCollision;

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
		void* const in_data //address of ZombieMission
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
	SceneNodeComponentClientZombieFly(
		ZombieMission& in_parent,
		TPointerSceneNode& in_parentSceneNode,
		const std::string& in_factoryData,
		const float in_healthFly,
		const float in_healthDrag
		);
	virtual ~SceneNodeComponentClientZombieFly();

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
	virtual void OnSetAiActive(const bool in_active);
	virtual const bool OnGetAiActive()const;
	virtual void OnDestroy();

	//private methods
private:
	void AiSelectAction(const float in_timeDelta);
	void PlayAnimation(const int in_animation, const bool in_force = false);

	//private members
private:
	ZombieMission& mParentMission; //to destroy node on death
	TWeakSceneNode mParentSceneNode; //to detach limbs, need to know scene node world pos, 
	const std::string mFactoryData;
	float mHealthFly;
	float mHealthDrag;

	bool mTransformDirty;
	bool mAiControl;
	float mStateCountDown;

	int mState;
	int mCurrentAnimation;

	TPointerComponentLerpVector3Float mLerpPosition;
	TPointerComponentLerpVector3Float mLerpAt;
	TPointerComponentLerpVector3Float mLerpUp;

	TArrayWeakSceneNodeComponentCollision mArrayCollision;
	TWeakSceneNodeComponentAnimation mAnimation;

};

#endif //_SceneNodeComponentClientZombieFly_h_
