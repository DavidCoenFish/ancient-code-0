//
//  SceneNodeComponentClientPickup.h
//  Zombie
//
//  Created by David Coen on 2011 03 25
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _SceneNodeComponentClientPickup_h_
#define _SceneNodeComponentClientPickup_h_

#include "SceneNodeComponentClientObjectBase.h"

class GCamera;
class GSceneNode;
class GSceneNodeComponentBase;
class GScenePostLoadCallback;
class GSceneNodeComponentCollision;
class GSceneNodeComponentVisual;
class GSceneNodeComponentParticle;
class GVector2Float;
class ZombieMission;

class SceneNodeComponentClientPickup : public SceneNodeComponentClientObjectBase
{
	//typedef
private:
	typedef boost::shared_ptr<GSceneNodeComponentBase> TPointerSceneNodeComponentBase;
	typedef boost::shared_ptr<SceneNodeComponentClientPickup> TPointerSceneNodeComponentClientPickup;
	typedef std::vector<GScenePostLoadCallback> TArrayScenePostLoadCallback;

	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;
	typedef boost::weak_ptr<GSceneNode> TWeakSceneNode;

	typedef boost::shared_ptr<GSceneNodeComponentCollisionResult> TPointerSceneNodeComponentCollisionResult;
	typedef std::pair<SceneNodeComponentClientObjectBase*, TPointerSceneNodeComponentCollisionResult> TPairClientObjectCollisionResult;
	typedef std::vector<TPairClientObjectCollisionResult> TArrayPairClientObjectCollisionResult;

	typedef boost::shared_ptr<GSceneNodeComponentCollision> TPointerSceneNodeComponentCollision;
	typedef boost::weak_ptr<GSceneNodeComponentCollision> TWeakSceneNodeComponentCollision;

	typedef boost::shared_ptr<GSceneNodeComponentParticle> TPointerSceneNodeComponentParticle;
	typedef boost::weak_ptr<GSceneNodeComponentParticle> TWeakSceneNodeComponentParticle;

	typedef boost::shared_ptr<GSceneNodeComponentVisual> TPointerSceneNodeComponentVisual;
	typedef boost::weak_ptr<GSceneNodeComponentVisual> TWeakSceneNodeComponentVisual;

	//static public methods
public:
	static TPointerSceneNodeComponentBase FactoryGarbage(
		GScene& inout_scene,
		TPointerSceneNode& inout_owner,
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
		void* const in_data //address of ApplicationTaskGame
		);
	static TPointerSceneNodeComponentBase FactorySlugFountain(
		GScene& inout_scene,
		TPointerSceneNode& inout_owner,
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
		void* const in_data //address of ApplicationTaskGame
		);
	static TPointerSceneNodeComponentBase FactoryAmmo(
		GScene& inout_scene,
		TPointerSceneNode& inout_owner,
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
		void* const in_data //address of ApplicationTaskGame
		);
	static TPointerSceneNodeComponentBase FactoryHeal(
		GScene& inout_scene,
		TPointerSceneNode& inout_owner,
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
		void* const in_data //address of ApplicationTaskGame
		);

	//static private methods
private:
	static TPointerSceneNodeComponentBase Factory(
		const int in_type,
		GScene& inout_scene,
		TPointerSceneNode& inout_owner,
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
		void* const in_data //address of ApplicationTaskGame
		);
	static void PostLoadCallback(
		GScene& inout_scene, //access to mesh/ material manager
		TPointerSceneNode& inout_owner,
		TPointerSceneNodeComponentBase& inout_component,
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback
		);

	//constructor
public:
	SceneNodeComponentClientPickup(
		const int in_type,
		ZombieMission& inout_zombieMission,
		TPointerSceneNode& inout_owner
		);
	virtual ~SceneNodeComponentClientPickup();

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

	virtual void OnDestroy();

	//private members
private:
	const int mType;
	ZombieMission& mParentMission; //to destroy node on death
	TWeakSceneNode mParentSceneNode;
	TWeakSceneNodeComponentVisual mVisual; 
	TWeakSceneNodeComponentParticle mParticle;
	TWeakSceneNodeComponentCollision mCollision;
	float mHealth;
	float mDeathCountDown;
	bool mDead;


};

#endif //_SceneNodeComponentClientPickup_h_
