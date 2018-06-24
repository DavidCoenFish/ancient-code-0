//
//  SceneNodeComponentClientSlug.h
//  Zombie
//
//  Created by David Coen on 2011 02 22
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _SceneNodeComponentClientSlug_h_
#define _SceneNodeComponentClientSlug_h_

#include "SceneNodeComponentClientObjectBase.h"
#include <boost/shared_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <map>
#include <GVector2Float.h>
#include <GVector3Float.h>

class ZombieMission;
class GSceneNodeComponentAnimation;
class GSceneNodeComponentCollision;
class GSceneNodeComponentCollisionResult;
class GSceneNodeComponentParticle;
class GComponentLerpVector3Float;

class SceneNodeComponentClientSlug : public SceneNodeComponentClientObjectBase
{
	//typedef
private:
	typedef boost::shared_ptr<GSceneNodeComponentBase> TPointerSceneNodeComponentBase;
	typedef boost::shared_ptr<SceneNodeComponentClientSlug> TPointerSceneNodeComponentClientSlug;
	typedef std::vector<GScenePostLoadCallback> TArrayScenePostLoadCallback;

	typedef boost::shared_ptr<GSceneNodeComponentCollisionResult> TPointerSceneNodeComponentCollisionResult;
	typedef std::pair<SceneNodeComponentClientObjectBase*, TPointerSceneNodeComponentCollisionResult> TPairClientObjectCollisionResult;
	typedef std::vector<TPairClientObjectCollisionResult> TArrayPairClientObjectCollisionResult;

	typedef boost::shared_ptr<GSceneNodeComponentCollision> TPointerSceneNodeComponentCollision;
	typedef boost::weak_ptr<GSceneNodeComponentCollision> TWeakSceneNodeComponentCollision;

	typedef boost::shared_ptr<GSceneNodeComponentAnimation> TPointerSceneNodeComponentAnimation;
	typedef boost::weak_ptr<GSceneNodeComponentAnimation> TWeakSceneNodeComponentAnimation;

	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;
	typedef boost::weak_ptr<GSceneNode> TWeakSceneNode;

	typedef boost::shared_ptr<GSceneNodeComponentParticle> TPointerSceneNodeComponentParticle;
	typedef boost::weak_ptr<GSceneNodeComponentParticle> TWeakSceneNodeComponentParticle;

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
	SceneNodeComponentClientSlug(
		ZombieMission& in_parent,
		TPointerSceneNode& in_parentSceneNode,
		const float in_healthFactor
		);
	virtual ~SceneNodeComponentClientSlug();

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

	virtual const bool OnGetAiActive()const;
	virtual void OnDestroy();

	//public methods
public:
	void SetFlightMode(const GVector3Float& in_velocity);

	//private methods
private:
	void PlayAnimation(const int in_animation, const bool in_force = false);

	//private members
private:
	ZombieMission& mParentMission; //to destroy node on death
	TWeakSceneNode mSceneNode; //destroy needs scene node

	float mHealth;

	TWeakSceneNodeComponentCollision mCollision;
	TWeakSceneNodeComponentAnimation mAnimation;
	TWeakSceneNodeComponentParticle mParticle;

	TPointerComponentLerpVector3Float mLerpPosition;
	TPointerComponentLerpVector3Float mLerpAt;
	TPointerComponentLerpVector3Float mLerpUp;

	float mStateTimer;
	int mState;
	int mApproachPlayerCount;

	int mCurrentAnimation;
	float mAnimationCountdown;

	float mJumpHeight;
	GVector2Float mScreenPosAttach;

	GVector3Float mVelocity; //for state TFlight

	bool mTransformDirty;

};

#endif //_SceneNodeComponentClientSlug_h_
