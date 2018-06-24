//
//  SceneNodeComponentClientZombieBoss.h
//  Zombie
//
//  Created by David Coen on 2011 03 22
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _SceneNodeComponentClientZombieBoss_h_
#define _SceneNodeComponentClientZombieBoss_h_

#include "SceneNodeComponentClientObjectBase.h"
#include <boost/shared_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <map>

class ZombieMission;
class ZombiePart;
class GSceneNodeComponentAnimation;
class GSceneNodeComponentCollision;
class GSceneNodeComponentCollisionResult;
class GSceneNodeComponentVisual;
class GComponentLerpVector3Float;
class GSceneNodeComponentParticle;
class GSkeletonInstance;
class ZombieComponent;

class SceneNodeComponentClientZombieBoss : public SceneNodeComponentClientObjectBase
{
	//typedef
private:
	typedef boost::shared_ptr<GSceneNodeComponentBase> TPointerSceneNodeComponentBase;
	typedef boost::shared_ptr<SceneNodeComponentClientZombieBoss> TPointerSceneNodeComponentClientZombieBoss;
	typedef std::vector<GScenePostLoadCallback> TArrayScenePostLoadCallback;

	typedef boost::shared_ptr<GSceneNodeComponentCollisionResult> TPointerSceneNodeComponentCollisionResult;
	typedef std::pair<SceneNodeComponentClientObjectBase*, TPointerSceneNodeComponentCollisionResult> TPairClientObjectCollisionResult;
	typedef std::vector<TPairClientObjectCollisionResult> TArrayPairClientObjectCollisionResult;

	typedef boost::shared_ptr<GSceneNodeComponentCollision> TPointerSceneNodeComponentCollision;
	typedef boost::weak_ptr<GSceneNodeComponentCollision> TWeakSceneNodeComponentCollision;
	typedef std::vector<TWeakSceneNodeComponentCollision> TArrayWeakSceneNodeComponentCollision;

	typedef boost::shared_ptr<GSceneNodeComponentAnimation> TPointerSceneNodeComponentAnimation;
	typedef boost::weak_ptr<GSceneNodeComponentAnimation> TWeakSceneNodeComponentAnimation;

	typedef boost::shared_ptr<GSceneNodeComponentVisual> TPointerSceneNodeComponentVisual;
	typedef boost::weak_ptr<GSceneNodeComponentVisual> TWeakSceneNodeComponentVisual;
	typedef std::vector<TWeakSceneNodeComponentVisual> TArrayWeakSceneNodeComponentVisual;

	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;
	typedef boost::weak_ptr<GSceneNode> TWeakSceneNode;
	typedef boost::shared_ptr<GComponentLerpVector3Float> TPointerComponentLerpVector3Float;

	typedef boost::shared_ptr<GSkeletonInstance> TPointerSkeletonInstance;
	typedef boost::weak_ptr<GSkeletonInstance> TWeakSkeletonInstance;

public:
	struct TZombieBossVar
	{
		enum TEnum
		{
			TMaggotCount = 12
		};
	};

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
	SceneNodeComponentClientZombieBoss(
		ZombieMission& in_parent,
		TPointerSceneNode& in_parentSceneNode,
		const std::string& in_factoryData,
		const float in_wingHealth,
		const float in_maggotHealth,
		const int in_maggotCountHigh,
		const int in_maggotCountLow
		);
	virtual ~SceneNodeComponentClientZombieBoss();

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
	void SpawnSlugJaw();
	void UpdateMaggotVisual();
	void DropMaggot(GScene& inout_scene, TPointerSceneNode& inout_sceenRoot);

	//private members
private:
	//GScene& mScene
	ZombieMission& mParentMission; //to destroy node on death
	TWeakSceneNode mParentSceneNode; //to get world pos
	const std::string mFactoryData;
	float mHealthFlight;
	float mHealthFlightMax;
	float mHealthMaggot;
	float mHealthPerMaggot;
	int mMaggotCountHigh;
	int mMaggotCountLow;
	int mMaggotCount;
	float mInvunerableTime;

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

	TWeakSkeletonInstance mWeakSkeletonInstance; //reference to the skeleton instance that has the bone we track
	int mBoneJawIndex;
	int mBoneSpineIndex;

	TWeakSceneNodeComponentVisual mArrayMaggotVisual[TZombieBossVar::TMaggotCount];

};

#endif //_SceneNodeComponentClientZombieBoss_h_
