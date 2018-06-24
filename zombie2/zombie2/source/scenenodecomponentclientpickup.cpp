//
//  SceneNodeComponentClientPickup.cpp
//  Zombie
//
//  Created by David Coen on 2011 03 25
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "SceneNodeComponentClientPickup.h"


#include "ZombieMission.h"
#include "ZombieMissionAchievement.h"
#include "ZombieStateType.h"
#include "ZombieGameVar.h"

#include <GCamera.h>
#include <GSceneNode.h>
#include <GSceneNodeComponentBase.h>
#include <GSceneNodeComponentCollision.h>
#include <GSceneNodeComponentParticle.h>
#include <GSceneNodeComponentVisual.h>
#include <GScenePostLoadCallback.h>
#include <GVector2Float.h>
#include <GMatrix16Float.h>
#include <GAssert.h>
#include <GMath.h>

struct TPickupType
{
	enum TEnum
	{
		TGarbage = 0,
		TSlugFountain,
		TAmmo,
		THeal,

		TCount
	};
};

typedef boost::shared_ptr<SceneNodeComponentClientObjectBase> TPointerSceneNodeComponentClientObjectBase;
typedef boost::shared_ptr<GSceneNodeComponentCollisionResult> TPointerSceneNodeComponentCollisionResult;
typedef std::vector<TPointerSceneNodeComponentCollisionResult> TArrayPointerSceneNodeComponentCollisionResult;

static void LocalSpawnSlug(
	ZombieMission& inout_parentMission, 
	const GVector3Float& in_position
	)
{
	const GVector3Float velocity(GMath::RandomUnitPlusMinius() * 2.0F, 5.0F + (GMath::RandomUnit() * 2.0F), GMath::RandomUnitPlusMinius() * 2.0F);
	const GVector3Float direction = Normalise(GVector3Float(velocity.m_x, 0.0F, velocity.m_y));

	inout_parentMission.SpawnSlug(
		direction,
		GVector3Float::sUnitY,
		in_position,
		velocity
		);

	return;
}

//static public methods
/*static*/ SceneNodeComponentClientPickup::TPointerSceneNodeComponentBase SceneNodeComponentClientPickup::FactoryGarbage(
	GScene& inout_scene,
	TPointerSceneNode& inout_owner,
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
	void* const in_data //address of ApplicationTaskGame
	)
{
	TPointerSceneNodeComponentBase pointer = Factory(
		TPickupType::TGarbage,
		inout_scene,
		inout_owner,
		inout_arrayPostLoadCallback,
		in_data
		);
	return pointer;
}

/*static*/ SceneNodeComponentClientPickup::TPointerSceneNodeComponentBase SceneNodeComponentClientPickup::FactorySlugFountain(
	GScene& inout_scene,
	TPointerSceneNode& inout_owner,
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
	void* const in_data //address of ApplicationTaskGame
	)
{
	TPointerSceneNodeComponentBase pointer = Factory(
		TPickupType::TSlugFountain,
		inout_scene,
		inout_owner,
		inout_arrayPostLoadCallback,
		in_data
		);
	return pointer;
}

/*static*/ SceneNodeComponentClientPickup::TPointerSceneNodeComponentBase SceneNodeComponentClientPickup::FactoryAmmo(
	GScene& inout_scene,
	TPointerSceneNode& inout_owner,
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
	void* const in_data //address of ApplicationTaskGame
	)
{
	TPointerSceneNodeComponentBase pointer = Factory(
		TPickupType::TAmmo,
		inout_scene,
		inout_owner,
		inout_arrayPostLoadCallback,
		in_data
		);
	return pointer;
}

/*static*/ SceneNodeComponentClientPickup::TPointerSceneNodeComponentBase SceneNodeComponentClientPickup::FactoryHeal(
	GScene& inout_scene,
	TPointerSceneNode& inout_owner,
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
	void* const in_data //address of ApplicationTaskGame
	)
{
	TPointerSceneNodeComponentBase pointer = Factory(
		TPickupType::THeal,
		inout_scene,
		inout_owner,
		inout_arrayPostLoadCallback,
		in_data
		);
	return pointer;
}

//static private methods
/*static*/ SceneNodeComponentClientPickup::TPointerSceneNodeComponentBase SceneNodeComponentClientPickup::Factory(
	const int in_type,
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

	pointer.reset(new SceneNodeComponentClientPickup(
		in_type,
		zombieMission,
		inout_owner
		));

	inout_arrayPostLoadCallback.push_back(GScenePostLoadCallback(
		inout_owner,
		pointer,
		SceneNodeComponentClientPickup::PostLoadCallback
		));

	return pointer;
}

/*static*/ void SceneNodeComponentClientPickup::PostLoadCallback(
	GScene& inout_scene, //access to mesh/ material manager
	TPointerSceneNode& inout_owner,
	TPointerSceneNodeComponentBase& inout_component,
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback
	)
{
	TPointerSceneNodeComponentClientPickup pointer = boost::dynamic_pointer_cast<SceneNodeComponentClientPickup>(inout_component);
	if (!pointer || !inout_owner)
	{
		return;
	}

	GSceneNode& sceneNode = *inout_owner;
	//find the collision, animation
	const int componentCount = sceneNode.GetComponentCount();
	for (int index = 0; index < componentCount; ++index)
	{
		TPointerSceneNodeComponentCollision pointerCollision = sceneNode.GetComponentPointer<GSceneNodeComponentCollision>(index);
		if (pointerCollision)
		{
			pointer->mCollision = pointerCollision;
			continue;
		}

		TPointerSceneNodeComponentVisual pointerVisual = sceneNode.GetComponentPointer<GSceneNodeComponentVisual>(index);
		if (pointerVisual && (pointerVisual->GetData() != "particleSystemVisual"))
		{
			pointer->mVisual = pointerVisual;
			continue;
		}

		TPointerSceneNodeComponentParticle pointerParticle = sceneNode.GetComponentPointer<GSceneNodeComponentParticle>(index);
		if (pointerParticle)
		{
			pointer->mParticle = pointerParticle;
			continue;
		}
	}

	return;
}

//constructor
SceneNodeComponentClientPickup::SceneNodeComponentClientPickup(
	const int in_type,
	ZombieMission& inout_zombieMission,
	TPointerSceneNode& inout_owner	
	)
: SceneNodeComponentClientObjectBase("pickup")
, mType(in_type)
, mParentMission(inout_zombieMission)
, mParentSceneNode(inout_owner)
, mHealth(1.0F)
, mDeathCountDown(0.0F)
, mDead(false)
{
	mParentMission.AddClientObject(*this);
	return;
}

/*virtual*/ SceneNodeComponentClientPickup::~SceneNodeComponentClientPickup()
{
	return;
}

//implement GSceneNodeComponentClient
/*virtual*/ void SceneNodeComponentClientPickup::OnTick(TPointerSceneNode& inout_sceneNode, const float in_timeDelta)
{
	if (mDead)
	{
		mDeathCountDown -= in_timeDelta;
		if (mDeathCountDown <= 0.0F)
		{
			mParentMission.RequestNodeDestroy(inout_sceneNode);
			return;
		}
	}
	return;
}

//implement SceneNodeComponentClientObjectBase
/*virtual*/ void SceneNodeComponentClientPickup::OnCollectDamageReceivers(
	TArrayPairClientObjectCollisionResult& out_arrayClientObjectCollisionResult,
	const GVector3Float& in_worldSpaceConeOrigin,
	const GVector3Float& in_worldSpaceConeDirection,
	const float in_coneLength,
	const float in_coneRadiusPoint,
	const float in_coneRadiusBase
	)
{
	TPointerSceneNodeComponentCollision pointerCollision = mCollision.lock();
	if (!pointerCollision)
	{
		return;
	}

	GSceneNodeComponentCollision& componentCollision = *pointerCollision;
	if (!componentCollision.GetActive())
	{
		return;
	}
	TWeakSceneNodeComponentCollision weakCollision = pointerCollision;
	TArrayPointerSceneNodeComponentCollisionResult arrayCollisionResult;

	componentCollision.CollideCone(
		arrayCollisionResult,
		weakCollision,
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
			*(iterator)
			));
	}

	return;
}

/*virtual*/ void SceneNodeComponentClientPickup::OnApplyDamage(
	float& inout_damage, 
	const std::string& in_data
	)
{
	const float damage = GMath::Minimum(GMath::Maximum(0.0F, mHealth), -inout_damage);
	inout_damage += damage;
	mHealth -= damage;

	return;
}


/*virtual*/ void SceneNodeComponentClientPickup::OnAfterDamagePass(GScene& inout_scene, TPointerSceneNode& inout_sceenRoot)
{
	if (mHealth <= 0.0F)
	{
		mListHook.unlink();
		mDead = true;
		mDeathCountDown = 5.0F;

		//hide visual
		TPointerSceneNodeComponentVisual pointerVisual = mVisual.lock();
		if (pointerVisual)
		{
			pointerVisual->SetVisible(false);
		}

		GVector3Float position;
		TPointerSceneNode pointerSceneNode = mParentSceneNode.lock();
		if (pointerSceneNode)
		{
			position = pointerSceneNode->GetWorldTransform().GetPosition();
		}
		GMatrix16Float transform(GMatrix16Float::sIdentity);
		transform.SetPosition(position);

		//trigger particle
		TPointerSceneNodeComponentParticle pointerParticle = mParticle.lock();
		if (pointerParticle)
		{
			//pointerParticle->SetVisible(true);
			pointerParticle->RequestEmittor(3, 0.1F);
		}

		ZombieGameVar& gameVar = *mParentMission.GetGameVar();

		//trigger 
		switch (mType)
		{
		default:
			break;
		case TPickupType::TAmmo:
			{
				const int missionId = gameVar.GetMissionId();
				if (!gameVar.GetMissionHasBeenUsed(missionId))
				{
					gameVar.SetMissionHasBeenUsed(missionId);
					mParentMission.GetMissionAchievement()->AddBonus(ZombieStateType::TBonus::TAddAmmo);
					mParentMission.ClientFactory(
						transform,
						"FactoryAmmo",
						"",
						"pickupAmmo"
						);
				}
			}
			break;
		case TPickupType::TSlugFountain:
			LocalSpawnSlug(mParentMission, position);
			LocalSpawnSlug(mParentMission, position);
			LocalSpawnSlug(mParentMission, position);
			LocalSpawnSlug(mParentMission, position);
			LocalSpawnSlug(mParentMission, position);
			break;
		case TPickupType::THeal:
			mParentMission.GetMissionAchievement()->AddBonus(ZombieStateType::TBonus::THeal);
			mParentMission.ClientFactory(
				transform,
				"FactoryHealth",
				"",
				"pickupHealth"
				);

			break;
		}

	}
	return;
}

/*virtual*/ void SceneNodeComponentClientPickup::OnDestroy()
{
	TPointerSceneNode pointerSceneNode = mParentSceneNode.lock();
	mParentMission.RequestNodeDestroy(pointerSceneNode);
	return;
}
