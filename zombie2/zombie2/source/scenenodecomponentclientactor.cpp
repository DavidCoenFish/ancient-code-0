//
//  SceneNodeComponentClientActor.cpp
//  Zombie
//
//  Created by David Coen on 2011 02 16
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "SceneNodeComponentClientActor.h"

#include "ZombieMission.h"

#include <GCommon.h>
#include <GMath.h>
#include <GScenePostLoadCallback.h>
#include <GScene.h>
#include <GSceneNode.h>
#include <GSceneNodeComponentVisual.h>
#include <GSceneNodeComponentCollision.h>
#include <GAnimationManagerInstance.h>
#include <GSceneNodeComponentAnimation.h>
#include <GComponentLerpVector3Float.h>
#include <GAnimationPlayParam.h>
#include <GSceneNodeComponentFade.h>
#include <GAssert.h>
#include <GSceneVisitorCollision.h>

typedef boost::shared_ptr<SceneNodeComponentClientObjectBase> TPointerSceneNodeComponentClientObjectBase;
typedef boost::shared_ptr<GAnimationManagerInstance> TPointerAnimationManagerInstance;
typedef boost::shared_ptr<GSceneNodeComponentCollision> TPointerSceneNodeComponentCollision;
typedef boost::weak_ptr<GSceneNodeComponentCollision> TWeakSceneNodeComponentCollision;
typedef boost::shared_ptr<GSceneNodeComponentCollisionResult> TPointerSceneNodeComponentCollisionResult;
typedef std::vector<TPointerSceneNodeComponentCollisionResult> TArrayPointerSceneNodeComponentCollisionResult;

//static public methods
/*static*/ SceneNodeComponentClientActor::TPointerSceneNodeComponentBase SceneNodeComponentClientActor::Factory(
	GScene& inout_scene,
	TPointerSceneNode& inout_owner,
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
	void* const in_data //address of ApplicationTaskGame
	)
{
	TPointerSceneNodeComponentBase pointer;
	if (!in_data)
	{
		return pointer;
	}

	ZombieMission& zombieMission = *((ZombieMission*)in_data);

	//const std::string& factoryData = inout_scene.GetFactoryData();

	TPointerSceneNodeComponentClientActor pointerActor;
	pointerActor.reset(new SceneNodeComponentClientActor(
		zombieMission,
		inout_owner
		));

	pointer = pointerActor;

	inout_arrayPostLoadCallback.push_back(GScenePostLoadCallback(
		inout_owner,
		pointer,
		SceneNodeComponentClientActor::PostLoadCallback
		));

	TPointerSceneNodeComponentClientObjectBase pointerObjectBase = pointerActor;
	zombieMission.AddClientActor(*pointerObjectBase);

	return pointer;
}

//static private methods
/*static*/ void SceneNodeComponentClientActor::PostLoadCallback(
	GScene& inout_scene, //access to mesh/ material manager
	TPointerSceneNode& inout_owner,
	TPointerSceneNodeComponentBase& inout_component,
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback
	)
{
	TPointerSceneNodeComponentClientActor pointer = boost::dynamic_pointer_cast<SceneNodeComponentClientActor>(inout_component);
	if (!pointer)
	{
		return;
	}

	//mAnimation
	GSceneNode& sceneNode = *inout_owner;
	const int componentCount = sceneNode.GetComponentCount();
	for (int index = 0; index < componentCount; ++index)
	{
		TPointerSceneNodeComponentAnimation pointerAnimation = sceneNode.GetComponentPointer<GSceneNodeComponentAnimation>(index);
		if (pointerAnimation)
		{
			pointer->mAnimation = pointerAnimation;
			break;
		}
	}

	return;
}

//constructor
SceneNodeComponentClientActor::SceneNodeComponentClientActor(
	ZombieMission& in_parent,
	TPointerSceneNode& in_pointerParent
	)
	: SceneNodeComponentClientObjectBase(in_pointerParent->GetName())
	, mWeakParent(in_pointerParent)
	, mParentMission(in_parent)
	, mTransformDirty(false)
	, mHealth(1.0F)
	, mDead(false)
	, mDeathCountDown(0.0F)
{
	const GMatrix16Float& worldTransform = in_pointerParent->GetWorldTransform();
	mLerpPosition.reset(new GComponentLerpVector3Float(worldTransform.GetPosition()));
	mLerpAt.reset(new GComponentLerpVector3Float(worldTransform.GetAt()));
	mLerpUp.reset(new GComponentLerpVector3Float(worldTransform.GetUp()));

	return;
}

/*virtual*/ SceneNodeComponentClientActor::~SceneNodeComponentClientActor()
{
	return;
}

//implement GSceneNodeComponentClient
/*virtual*/ void SceneNodeComponentClientActor::OnTick(TPointerSceneNode& inout_sceneNode, const float in_timeDelta)
{
	if (mDead)
	{
		mDeathCountDown -= in_timeDelta;
		if (mDeathCountDown < 0.0F)
		{
			TPointerSceneNode pointerSceneNode = mWeakParent.lock();
			mParentMission.RequestNodeDestroy(pointerSceneNode);
			//mDead = false;
			//mDeathCountDown = 0.0F;
		}
	}

	if ((mLerpPosition->GetActive()) ||
		(mLerpAt->GetActive()) ||
		(mLerpUp->GetActive()))
	{
		mTransformDirty = true;
		mLerpPosition->Tick(in_timeDelta);
		mLerpAt->Tick(in_timeDelta);
		mLerpUp->Tick(in_timeDelta);
	}
	if (mTransformDirty)
	{
		mTransformDirty = false;
		GVector3Float at = mLerpAt->GetValue();
		GVector3Float up = mLerpUp->GetValue();
		if ((mLerpAt->GetActive()) ||
			(mLerpUp->GetActive()))
		{
			Orthogonalise(at, up);
		}

		const GMatrix16Float transform = GMatrix16FloatConstructAtUp(
			at, 
			up,
			GVector3Float::sUnitZ, 
			GVector3Float::sUnitY, 
			mLerpPosition->GetValue()
			);
		inout_sceneNode->SetParentRelativeTransform(transform);
	}

	return;
}
//implement SceneNodeComponentClientObjectBase
/*virtual*/ void SceneNodeComponentClientActor::OnCollectDamageReceivers(
	TArrayPairClientObjectCollisionResult& out_arrayClientObjectCollisionResult,
	const GVector3Float& in_worldSpaceConeOrigin,
	const GVector3Float& in_worldSpaceConeDirection,
	const float in_coneLength,
	const float in_coneRadiusPoint,
	const float in_coneRadiusBase
	)
{
	if (mDead)
	{
		return;
	}
	TArrayPointerSceneNodeComponentCollisionResult arrayCollisionResult;
	TPointerSceneNode pointerSceneNode = mWeakParent.lock();

	GSceneVisitorCollisionCone::Run(
		pointerSceneNode,
		arrayCollisionResult,
		false,
		false,
		in_worldSpaceConeOrigin,
		in_worldSpaceConeDirection,
		in_coneLength,
		in_coneRadiusPoint,
		in_coneRadiusBase
		);

	for (TArrayPointerSceneNodeComponentCollisionResult::iterator iterator = arrayCollisionResult.begin(); iterator != arrayCollisionResult.end(); ++iterator)
	{
		out_arrayClientObjectCollisionResult.push_back(TPairClientObjectCollisionResult(
			this,
			(*iterator)
			));
	}

	return;
}

/*virtual*/ void SceneNodeComponentClientActor::OnApplyDamage(
	float& inout_damage, 
	const std::string& in_data
	)
{
	const float damage = GMath::Minimum(GMath::Maximum(0.0F, mHealth), -inout_damage);
	inout_damage += damage;
	mHealth -= damage;

	return;
}

/*virtual*/ void SceneNodeComponentClientActor::OnAfterDamagePass(GScene& inout_scene, TPointerSceneNode& inout_sceenRoot)
{
	if ((!mDead) && (mHealth <= 0.0F))
	{
		mDeathCountDown = 2.0F;
		GAnimationPlayParam defaultParam;
		Animate(
			"death", 
			defaultParam
			);

		mDead = true;
		mLerpPosition->SetValue(mLerpPosition->GetValue());
		mLerpAt->SetValue(mLerpAt->GetValue());
		mLerpUp->SetValue(mLerpUp->GetValue());
		mTransformDirty = true;

		mListHook.unlink();
	}

	return;
}

/*virtual*/ void SceneNodeComponentClientActor::OnAnimate(
	const std::string& in_animationName, 
	const GAnimationPlayParam& in_playParam
	)
{
	if (mDead)
	{
		return;
	}

	TPointerSceneNodeComponentAnimation pointerAnimation = mAnimation.lock();
	if (!pointerAnimation)
	{
		return;
	}

	TPointerAnimationManagerInstance& pointerAnimationManagerInstance = pointerAnimation->GetAnimationManagerInstance();
	if (!pointerAnimationManagerInstance)
	{
		return;
	}

	GAnimationManagerInstance& animationManagerInstance = *pointerAnimationManagerInstance;

	animationManagerInstance.StopAnimation(in_playParam.GetFadeInDuration(), in_playParam.GetFadeRate());
	animationManagerInstance.PlayAnimation(
		in_animationName,
		in_playParam
		);

	return;
}

/*virtual*/ void SceneNodeComponentClientActor::OnMove(
	const GVector3Float& in_position,
	const float in_transitionTime,
	const int in_transitionType
	)
{
	if (mDead)
	{
		return;
	}

	mLerpPosition->SetValue(in_position, in_transitionTime, GComponentLerp::TType::TEnum(in_transitionType));
	mTransformDirty = true;
	return;
}

/*virtual*/ void SceneNodeComponentClientActor::OnRotate(
	const GVector3Float& in_at,
	const GVector3Float& in_up,
	const float in_transitionTime,
	const int in_transitionType
	)
{
	if (mDead)
	{
		return;
	}

	mLerpAt->SetValue(in_at, in_transitionTime, GComponentLerp::TType::TEnum(in_transitionType));
	mLerpUp->SetValue(in_up, in_transitionTime, GComponentLerp::TType::TEnum(in_transitionType));
	mTransformDirty = true;
	return;
}

/*virtual*/ void SceneNodeComponentClientActor::OnDestroy()
{
	TPointerSceneNode sceneNode = mWeakParent.lock();
	if (sceneNode)
	{
		mParentMission.RequestNodeDestroy(sceneNode);
	}

	return;
}