//
//  SceneNodeComponentClientZombieBoss.cpp
//  Zombie
//
//  Created by David Coen on 2011 03 22
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "SceneNodeComponentClientZombieBoss.h"

#include "ZombieMission.h"
#include "ZombieGameVar.h"
#include "ZombieGuiDamage.h"
#include "ZombieMission.h"
#include "ZombieSound.h"

#include <GCommon.h>
#include <GMath.h>
#include <GScenePostLoadCallback.h>
#include <GScene.h>
#include <GSceneNode.h>
#include <GSceneNodeUtility.h>
#include <GSceneNodeComponentVisual.h>
#include <GSceneNodeComponentCollision.h>
#include <GAnimationManagerInstance.h>
#include <GSceneNodeComponentAnimation.h>
#include <GSceneNodeComponentPhysics.h>
#include <GSceneNodeComponentPhysicsLoad.h>
#include <GSceneNodeComponentParticle.h>
#include <GComponentLerpVector3Float.h>
#include <GAnimationPlayParam.h>
#include <GSceneNodeComponentFade.h>
#include <GAssert.h>
#include <GSceneVisitorCollision.h>
#include <GSceneVisitorAll.h>
#include <GSkeletonInstance.h>

//softbinding names
struct TZombieBossState
{
	enum TEnum
	{
		TFly = 0,
		TFallToGround,
		TWalk,
		TDead,

		TCount
	};
};

struct TZombieBossAnimation
{
	enum TEnum
	{
		TFly,
		TFlyAttackBite,
		TFlyAttackKick,
		TFlyIdle,
		TIdle,
		TRecoil,
		TWalk,
		TWalkAttackBite,
		TWalkAttackLeft,
		TWalkAttackRight,
		TWalkIdle,
		TWalkRecoil,
		TDeath,

		TCount
	};
};

static const float sZombieBossAnimationRate[TZombieBossAnimation::TCount] = {
	1.0F,	//TFly,
	0.75F,	//TFlyAttackBite,
	0.75F,	//TFlyAttackKick,
	1.0F,	//TFlyIdle,
	1.0F,	//TIdle,
	1.0F,	//TRecoil,
	1.0F,	//TWalk,
	0.75F,	//TWalkAttackBite,
	0.75F,	//TWalkAttackLeft,
	0.75F,	//TWalkAttackRight,
	1.0F,	//TWalkIdle,
	1.0F,	//TWalkRecoil
	1.0F,	//TDeath
};

static const char* const sZombieBossAnimationName[TZombieBossAnimation::TCount] = {
	"fly",
	"fly_attack_bite",
	"fly_attack_kick",
	"fly_idle",
	"idle",
	"recoil",
	"walk",
	"walk_attack_bite",
	"walk_attack_left",
	"walk_attack_right",
	"walk_idle",
	"walk_recoil",
	"death"
};


static const char* const sZombieBossMaggotNames[SceneNodeComponentClientZombieBoss::TZombieBossVar::TMaggotCount] = 
{
	"one",
	"two",
	"three",
	"four",
	"five",
	"six",
	"seven",
	"eight",
	"nine",
	"ten",
	"eleven",
	"twelve"
};
	
//typedef
typedef boost::shared_ptr<SceneNodeComponentClientObjectBase> TPointerSceneNodeComponentClientObjectBase;
typedef boost::shared_ptr<GSceneNodeComponentCollisionResult> TPointerSceneNodeComponentCollisionResult;
typedef std::vector<TPointerSceneNodeComponentCollisionResult> TArrayPointerSceneNodeComponentCollisionResult;
typedef boost::shared_ptr<GAnimationManagerInstance> TPointerAnimationManagerInstance;

typedef boost::shared_ptr<GSceneNodeComponentVisual> TPointerSceneNodeComponentVisual;
typedef boost::weak_ptr<GSceneNodeComponentVisual> TWeakSceneNodeComponentVisual;

typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;
typedef boost::weak_ptr<GSceneNode> TWeakSceneNode;

typedef boost::shared_ptr<GSceneNodeComponentBase> TPointerSceneNodeComponentBase;
typedef boost::shared_ptr<ZombieSound> TPointerZombieSound;

/**/
static const float sZombieBossWingHealthFactor = 4.0F;
static const float sZombieBossMaggotHealthFactor = 4.0F;
static const int sMaggotCount = 6;
static const float sZombieBossDeathDuration = 5.0F;
static const float sZombieBossAnimationDefaultFadeTime = 5.0F / 30.0F;
static const float sRecoilCountDown = 5.0F / 30.0F;
static const float sAttackCountDown = 15.0F / 30.0F; //(10 / .75) / 30
static const float sChanceAttack = 1.0F / 2.0F;

static const float sZombieBossInvunTime = 1.0F;

//////////////////////////////////////////////////////////////////
// Local static methods
//////////////////////////////////////////////////////////////////

//static public methods
/*static*/ SceneNodeComponentClientZombieBoss::TPointerSceneNodeComponentBase SceneNodeComponentClientZombieBoss::Factory(
	GScene& inout_scene,
	TPointerSceneNode& inout_owner,
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
	void* const in_data //address of ZombieMission
	)
{
	TPointerSceneNodeComponentBase pointer;
	if (!in_data)
	{
		return pointer;
	}

	ZombieMission& zombieMission = *((ZombieMission*)in_data);
	ZombieGameVar& zombieGameVar = *(zombieMission.GetGameVar());
	const float healthFactor = zombieGameVar.GetZombieHealthFactor();
	const std::string& factoryData = inout_scene.GetFactoryData();
	const int maggotCountHigh = zombieGameVar.GetMaggotCountHigh();
	const int maggotCountLow = zombieGameVar.GetMaggotCountLow();

	TPointerSceneNodeComponentClientZombieBoss pointerZombieBoss;
	pointerZombieBoss.reset(new SceneNodeComponentClientZombieBoss(
		zombieMission,
		inout_owner,
		factoryData,
		healthFactor * sZombieBossWingHealthFactor,
		healthFactor * sZombieBossMaggotHealthFactor,
		maggotCountHigh,
		maggotCountLow
		));

	pointer = pointerZombieBoss;

	inout_arrayPostLoadCallback.push_back(GScenePostLoadCallback(
		inout_owner,
		pointer,
		SceneNodeComponentClientZombieBoss::PostLoadCallback
		));

	TPointerSceneNodeComponentClientObjectBase pointerObjectBase = pointerZombieBoss;
	zombieMission.AddClientOpponent(*pointerObjectBase);

	return pointer;
}

//static private methods
/*static*/ void SceneNodeComponentClientZombieBoss::PostLoadCallback(
	GScene& inout_scene, //access to mesh/ material manager
	TPointerSceneNode& inout_owner,
	TPointerSceneNodeComponentBase& inout_component,
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback
	)
{
	TPointerSceneNodeComponentClientZombieBoss pointer = boost::dynamic_pointer_cast<SceneNodeComponentClientZombieBoss>(inout_component);
	if (!pointer || !inout_owner)
	{
		return;
	}
	SceneNodeComponentClientZombieBoss& zombieBoss = *pointer;

	GSceneNode& sceneNodeParent = *inout_owner;

	//find the collision
	GSceneVisitorAllCollect<GSceneNodeComponentCollision>::Run(inout_owner, zombieBoss.mArrayCollision);

	//find the animation
	const int componentCount = sceneNodeParent.GetComponentCount();
	for (int index = 0; index < componentCount; ++index)
	{
		TPointerSceneNodeComponentAnimation pointerAnimation = sceneNodeParent.GetComponentPointer<GSceneNodeComponentAnimation>(index);
		if (pointerAnimation)
		{
			zombieBoss.mAnimation = pointerAnimation;

			for (int subIndex = 0; subIndex < pointerAnimation->GetSkeletonCount(); ++subIndex)
			{
				TPointerSkeletonInstance skeletonInstance = pointerAnimation->GetSkeleton(subIndex);

				if (GCOMMON_INVALID_INDEX == zombieBoss.mBoneJawIndex)
				{
					zombieBoss.mBoneJawIndex = skeletonInstance->GetBoneIndex("Bip01 Head");
					zombieBoss.mWeakSkeletonInstance = skeletonInstance;
				}
				if (GCOMMON_INVALID_INDEX == zombieBoss.mBoneSpineIndex)
				{
					zombieBoss.mBoneSpineIndex = skeletonInstance->GetBoneIndex("Bip01 Spine1");
				}

				if ((GCOMMON_INVALID_INDEX != zombieBoss.mBoneJawIndex) &&
					(GCOMMON_INVALID_INDEX != zombieBoss.mBoneSpineIndex))
				{
					break;
				}
			}

			break;
		}
	}

	//find the maggots
	TArrayWeakSceneNodeComponentVisual arrayVisual;
	GSceneVisitorAllCollect<GSceneNodeComponentVisual>::Run(inout_owner, arrayVisual);
	for (TArrayWeakSceneNodeComponentVisual::iterator iterator = arrayVisual.begin(); iterator != arrayVisual.end(); ++iterator)
	{
		TPointerSceneNodeComponentVisual pointerVisual = (*iterator).lock();
		if (!pointerVisual)
		{
			continue;
		}
		GSceneNodeComponentVisual& visual = *pointerVisual;
		if (visual.GetData().empty())
		{
			continue;
		}
		for (int index = 0; index < TZombieBossVar::TMaggotCount; ++index)
		{
			if (visual.GetData() == sZombieBossMaggotNames[index])
			{
				zombieBoss.mArrayMaggotVisual[index] = pointerVisual;
			}
		}
	}

	zombieBoss.UpdateMaggotVisual();


	return;
}

//constructor
SceneNodeComponentClientZombieBoss::SceneNodeComponentClientZombieBoss(
	ZombieMission& in_parent,
	TPointerSceneNode& in_parentSceneNode,
	const std::string& in_factoryData,
	const float in_wingHealth,
	const float in_maggotHealth,
	const int in_maggotCountHigh,
	const int in_maggotCountLow
	)
	: SceneNodeComponentClientObjectBase(in_parentSceneNode->GetName())
	, mParentMission(in_parent)
	, mParentSceneNode(in_parentSceneNode)
	, mFactoryData(in_factoryData)
	, mHealthFlight(in_wingHealth)
	, mHealthFlightMax(in_wingHealth)
	, mHealthMaggot(in_maggotHealth)
	, mHealthPerMaggot(in_maggotHealth)
	, mMaggotCount(in_maggotCountHigh)
	, mMaggotCountHigh(in_maggotCountHigh)
	, mMaggotCountLow(in_maggotCountLow)
	, mInvunerableTime(0.0F)
	, mTransformDirty(true)
	, mAiControl(false)
	, mStateCountDown(0.0F)
	, mState(TZombieBossState::TFly)
	, mCurrentAnimation(TZombieBossAnimation::TIdle)
	, mLerpPosition()
	, mLerpAt()
	, mLerpUp()
	, mBoneJawIndex(GCOMMON_INVALID_INDEX)
	, mBoneSpineIndex(GCOMMON_INVALID_INDEX)
{
	const GMatrix16Float& worldTransform = in_parentSceneNode->GetWorldTransform();
	mLerpPosition.reset(new GComponentLerpVector3Float(worldTransform.GetPosition()));
	mLerpAt.reset(new GComponentLerpVector3Float(worldTransform.GetAt()));
	mLerpUp.reset(new GComponentLerpVector3Float(worldTransform.GetUp()));

	return;
}

/*virtual*/ SceneNodeComponentClientZombieBoss::~SceneNodeComponentClientZombieBoss()
{
	return;
}

//implement GSceneNodeComponentClient
/*virtual*/ void SceneNodeComponentClientZombieBoss::OnTick(TPointerSceneNode& inout_sceneNode, const float in_timeDelta)
{
	if (mAiControl)
	{
		mStateCountDown -= in_timeDelta;
	}

	if (mInvunerableTime)
	{
		mInvunerableTime -= in_timeDelta;
		if (mInvunerableTime < 0.0F)
		{
			mInvunerableTime = 0.0F;
		}
	}

	if (mStateCountDown < 0.0F)
	{
		switch (mState)
		{
		default:
			break;
		case TZombieBossState::TDead:
			mParentMission.RequestNodeDestroy(inout_sceneNode);
			return;
			break;
		case TZombieBossState::TFly:
			AiSelectAction(in_timeDelta);
			break;
		case TZombieBossState::TFallToGround:
			mState = TZombieBossState::TWalk;
			AiSelectAction(in_timeDelta);
			break;
		case TZombieBossState::TWalk:
			AiSelectAction(in_timeDelta);
			break;
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
/*virtual*/ void SceneNodeComponentClientZombieBoss::OnCollectDamageReceivers(
	TArrayPairClientObjectCollisionResult& out_arrayClientObjectCollisionResult,
	const GVector3Float& in_worldSpaceConeOrigin,
	const GVector3Float& in_worldSpaceConeDirection,
	const float in_coneLength,
	const float in_coneRadiusPoint,
	const float in_coneRadiusBase
	)
{
	if ((TZombieBossState::TDead == mState) ||
		(!mAiControl))
	{
		return;
	}

	TArrayPointerSceneNodeComponentCollisionResult arrayCollisionResult;

	for (TArrayWeakSceneNodeComponentCollision::iterator iterator = mArrayCollision.begin(); iterator != mArrayCollision.end(); ++iterator)
	{
		TPointerSceneNodeComponentCollision pointerCollision = (*iterator).lock();
		if (!pointerCollision)
		{
			continue;
		}

		GSceneNodeComponentCollision& componentCollision = *pointerCollision;
		if (!componentCollision.GetActive())
		{
			continue;
		}
		TWeakSceneNodeComponentCollision weakCollision = pointerCollision;

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
	}

	for (TArrayPointerSceneNodeComponentCollisionResult::iterator iterator = arrayCollisionResult.begin(); iterator != arrayCollisionResult.end(); ++iterator)
	{
		out_arrayClientObjectCollisionResult.push_back(TPairClientObjectCollisionResult(
			this,
			*(iterator)
			));
	}

	return;
}

/*virtual*/ void SceneNodeComponentClientZombieBoss::OnApplyDamage(
	float& inout_damage, 
	const std::string& in_data
	)
{
	if ((TZombieBossState::TDead == mState) ||
		(0.0F < mInvunerableTime) ||
		(!mAiControl))
	{
		return;
	}

	//not the boss though
	////if you shoot at cinematic zombie, ai is activated and it startes chasing the player
	//mAiControl = true;

	if (in_data == "chest")
	{
		const float damage = GMath::Minimum(GMath::Maximum(0.0F, mHealthMaggot), -inout_damage);
		inout_damage += damage;
		mHealthMaggot -= damage;
	}
	else if ((in_data == "wing") && (mState == TZombieBossState::TFly))
	{
		const float damage = GMath::Minimum(GMath::Maximum(0.0F, mHealthFlight), -inout_damage);
		inout_damage += damage;
		mHealthFlight -= damage;
	}
	else
	{
		inout_damage = 0.0F;
	}

	//not tracking damage to other parts of the body, but still recoil

	switch (mState)
	{
	default:
		break;
	case TZombieBossState::TFly:
		PlayAnimation(TZombieBossAnimation::TRecoil, true);
		mStateCountDown = sRecoilCountDown;
		break;
	case TZombieBossState::TWalk:
		PlayAnimation(TZombieBossAnimation::TWalkRecoil, true);
		mStateCountDown = sRecoilCountDown;
		break;
	};

	return;
}

/*virtual*/ void SceneNodeComponentClientZombieBoss::OnAfterDamagePass(GScene& inout_scene, TPointerSceneNode& inout_sceenRoot)
{
	if (mHealthMaggot <= 0.0F)
	{
		mMaggotCount -= 1;
		mHealthMaggot = mHealthPerMaggot;
		mParentMission.BossHealthBarIncrement();

		UpdateMaggotVisual();
		DropMaggot(inout_scene, inout_sceenRoot);
		DropMaggot(inout_scene, inout_sceenRoot);
		DropMaggot(inout_scene, inout_sceenRoot);

		//death (or reteat if still has maggots)
		if (mMaggotCount <= mMaggotCountLow)
		{
			mState = TZombieBossState::TDead;
			mStateCountDown = sZombieBossDeathDuration;
			if (mMaggotCountLow <= 0)
			{
				PlayAnimation(TZombieBossAnimation::TDeath, true);
				GVector3Float position = mLerpPosition->GetValue();
				const float oldHeight = position.m_y;
				position.m_y = (0.0F);
				mLerpPosition->SetValue(position, oldHeight / 10.0F);

				//recorde death
				ZombieGameVar& gameVar = *(mParentMission.GetGameVar());
				gameVar.SetPersistanceKillCount(gameVar.GetPersistanceKillCount() + 1);
			}
			else
			{
				PlayAnimation(TZombieBossAnimation::TWalk, true);
				GVector3Float position = mLerpPosition->GetValue();
				position.m_y = (0.0F);
				mLerpPosition->SetValue(position);

				//run away
				ZombieGameVar& gameVar = *mParentMission.GetGameVar();
				const GVector3Float playerPosition = gameVar.GetPlayerPosition();
				const GVector3Float playerAt = gameVar.GetPlayerDirection();

				const GVector3Float zombieTarget = playerPosition + (playerAt * 20.0F);

				mLerpPosition->SetValue(zombieTarget, mStateCountDown);
				mLerpAt->SetValue(playerAt, 0.25F, GComponentLerp::TType::TEaseInOut);
				mLerpUp->SetValue(GVector3Float::sUnitY, 0.25F, GComponentLerp::TType::TEaseInOut);
			}
			return;
		}

		//run away
		{
			mInvunerableTime = sZombieBossInvunTime;
			mHealthFlight = mHealthFlightMax;
			PlayAnimation(TZombieBossAnimation::TFly);
			mState = TZombieBossState::TFly;

			ZombieGameVar& gameVar = *mParentMission.GetGameVar();
			const GVector3Float playerPosition = gameVar.GetPlayerPosition();
			const GVector3Float playerAt = gameVar.GetPlayerDirection();
			const GVector3Float playerCross = CrossProduct(playerAt, GVector3Float::sUnitY);
			const GVector3Float zombiePosition = mLerpPosition->GetValue();

			GVector3Float zombieTarget = playerPosition + (playerAt * (8.0F + (8.0F * GMath::RandomUnit()))) + ((3.0F * GMath::RandomUnitPlusMinius()) * playerCross);
			zombieTarget.m_y = (1.5F + (3.0F * GMath::RandomUnit()));

			float distance = 0.0F;
			GVector3Float normalTozombieTarget = Normalise(zombieTarget - zombiePosition, distance);
			mStateCountDown = distance / 2.5F;
			const GVector3Float bossUp = Normalise(CrossProduct(normalTozombieTarget, CrossProduct(GVector3Float::sUnitY, normalTozombieTarget)));

			mLerpPosition->SetValue(zombieTarget, mStateCountDown, GComponentLerp::TType::TEaseInOut);
			mLerpAt->SetValue(normalTozombieTarget, 0.25F, GComponentLerp::TType::TEaseInOut);
			mLerpUp->SetValue(bossUp, 0.25F, GComponentLerp::TType::TEaseInOut);

			return;
		}
	}

	if (mHealthFlight <= 0.0F)
	{
		mState = TZombieBossState::TFallToGround;
		GVector3Float position = mLerpPosition->GetValue();
		mStateCountDown = position.m_y / 10.0F;
		position.m_y = (0.0F);
		mLerpPosition->SetValue(position, mStateCountDown);
		PlayAnimation(TZombieBossAnimation::TRecoil, true);

		GVector3Float at = mLerpAt->GetValue();
		at.m_y = (0.0F);
		at = Normalise(at);
		mLerpAt->SetValue(at, mStateCountDown);
		mLerpUp->SetValue(GVector3Float::sUnitY, mStateCountDown);
		return;
	}

	return;
}

/*virtual*/ void SceneNodeComponentClientZombieBoss::OnAnimate(
	const std::string& in_animationName, 
	const GAnimationPlayParam& in_playParam
	)
{
	if (mAiControl)
	{
		//yes, we ignore if chasing character, turn off ai on actor if needed for cinematic but could be a bad idea (since the actor could be killed)
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

	animationManagerInstance.StopAnimation(sZombieBossAnimationDefaultFadeTime);
	animationManagerInstance.PlayAnimation(
		in_animationName,
		in_playParam
		);

	//HACK. boss is being used as actor in finial anim sequence, we have no message/ easy way of setting the maggot count to zero
	// better to have the game var updated on maggot removal?
	if (in_animationName == sZombieBossAnimationName[TZombieBossAnimation::TDeath])
	{
		mMaggotCount = 0;
		UpdateMaggotVisual();
	}

	return;
}

/*virtual*/ void SceneNodeComponentClientZombieBoss::OnMove(
	const GVector3Float& in_position,
	const float in_transitionTime,
	const int in_transitionType
	)
{
	if (mAiControl)
	{
		//i'm ignoring you now
		return;
	}

	mLerpPosition->SetValue(in_position, in_transitionTime, GComponentLerp::TType::TEnum(in_transitionType));
	mTransformDirty = true;
	return;
}

/*virtual*/ void SceneNodeComponentClientZombieBoss::OnRotate(
	const GVector3Float& in_at,
	const GVector3Float& in_up,
	const float in_transitionTime,
	const int in_transitionType
	)
{
	if (mAiControl)
	{
		//i'm ignoring you now
		return;
	}

	mLerpAt->SetValue(in_at, in_transitionTime, GComponentLerp::TType::TEnum(in_transitionType));
	mLerpUp->SetValue(in_up, in_transitionTime, GComponentLerp::TType::TEnum(in_transitionType));
	mTransformDirty = true;
	return;
}

/*virtual*/ void SceneNodeComponentClientZombieBoss::OnSetAiActive(const bool in_active)
{
	mAiControl = in_active;
	return;
}

/*virtual*/ const bool SceneNodeComponentClientZombieBoss::OnGetAiActive()const
{
	return mAiControl;
}

/*virtual*/ void SceneNodeComponentClientZombieBoss::OnDestroy()
{
	TPointerSceneNode pointerSceneNode = mParentSceneNode.lock();
	mParentMission.RequestNodeDestroy(pointerSceneNode);
}

//private methods
void SceneNodeComponentClientZombieBoss::AiSelectAction(const float in_timeDelta)
{
	if (!mAiControl)
	{
		return;
	}

	//try to move towards player, if close enough to player, attack 
	ZombieGameVar& gameVar = *mParentMission.GetGameVar();
	const GVector3Float playerPosition = gameVar.GetPlayerPosition();
	const GVector3Float playerAt = gameVar.GetPlayerDirection();
	const GVector3Float playerCross = CrossProduct(playerAt, GVector3Float::sUnitY);
	const ZombieStateType::TGameDifficulty::TEnum difficulty = gameVar.GetGameDifficulty();

	GVector3Float zombiePosition = mLerpPosition->GetValue();

	const float distanceToPlayerSquared = LengthSquared(playerPosition - zombiePosition);

	const bool moveTowardsPlayer = (6.0F < distanceToPlayerSquared);

	bool walk = (mState == TZombieBossState::TWalk);

	if (walk && (0.0F != zombiePosition.m_y))
	{
		zombiePosition.m_y = (0.0F);
		mLerpPosition->SetValue(zombiePosition, 0.0F);
	}

	if (moveTowardsPlayer)
	{
		//hiss4 sound silly, use for fly zombie?

		if (walk)
		{
			GVector3Float playerTarget = playerPosition + (playerAt * 1.5F) + ((0.33F * GMath::RandomUnitPlusMinius()) * playerCross);
			float distanceToPlayer = 0.0F;
			if (!walk)
			{
				playerTarget.m_y = (1.25F);
			}

			GVector3Float normalToPlayerTarget = Normalise(playerTarget - zombiePosition, distanceToPlayer);
			mLerpAt->SetValue(normalToPlayerTarget, 0.25F, GComponentLerp::TType::TEaseInOut);
			mLerpUp->SetValue(Normalise(CrossProduct(CrossProduct(normalToPlayerTarget, GVector3Float::sUnitY), normalToPlayerTarget)), 0.25F, GComponentLerp::TType::TEaseInOut);

			mStateCountDown = distanceToPlayer / 1.5F;
			mLerpPosition->SetValue(playerTarget, mStateCountDown, GComponentLerp::TType::TEaseInOut);

			PlayAnimation(TZombieBossAnimation::TWalk);
		}
		else //range attack
		{
			//throw a slug?
			if (0.5F < GMath::RandomUnit())
			{
				mStateCountDown = sAttackCountDown;
				PlayAnimation(TZombieBossAnimation::TFlyAttackBite, true);
				

				GVector3Float atPlayer = playerPosition - zombiePosition;
				atPlayer.m_y = (0.0F);
				mLerpAt->SetValue(Normalise(atPlayer), 0.25F, GComponentLerp::TType::TEaseInOut);
				mLerpUp->SetValue(GVector3Float::sUnitY, 0.25F, GComponentLerp::TType::TEaseInOut);

				SpawnSlugJaw();
			}
			else
			{
				//fly to a random location

				PlayAnimation(TZombieBossAnimation::TFly);
		
				const float distanceToPlayer = GMath::SquareRoot(distanceToPlayerSquared) * 0.75F;
				GVector3Float zombieTarget = playerPosition + (playerAt * distanceToPlayer) + ((distanceToPlayer * 0.25F * GMath::RandomUnitPlusMinius()) * playerCross);
				zombieTarget.m_y = (0.5F + (0.5F * GMath::RandomUnit()));
				zombieTarget.m_y = (GMath::Maximum(zombieTarget.m_y, zombiePosition.m_y * 0.8F));

				float distance = 0.0F;
				GVector3Float normalTozombieTarget = NormaliseAprox(zombieTarget - zombiePosition, distance);
				mStateCountDown = distance / 2.5F;
				normalTozombieTarget.m_y = (0.0F);
				normalTozombieTarget = Normalise(normalTozombieTarget);
				const GVector3Float bossUp = GVector3Float::sUnitY;
				//const GVector3Float bossUp = Normalise(CrossProduct(normalTozombieTarget, CrossProduct(GVector3Float::sUnitY, normalTozombieTarget)));

				mLerpPosition->SetValue(zombieTarget, mStateCountDown, GComponentLerp::TType::TEaseInOut);
				mLerpAt->SetValue(normalTozombieTarget, 0.25F, GComponentLerp::TType::TEaseInOut);
				mLerpUp->SetValue(bossUp, 0.25F, GComponentLerp::TType::TEaseInOut);
			}
		}
	}
	else
	{
		float chanceMul = 1.0F;
		switch (difficulty)
		{
		default:
			break;
		case ZombieStateType::TGameDifficulty::TEasy:
			chanceMul = 0.2F;
			break;
		case ZombieStateType::TGameDifficulty::TNormal:
			chanceMul = 0.5F;
			break;
		}

		//chance of attack
		int attackAnimation = TZombieBossAnimation::TCount;
		int damageType = GCOMMON_INVALID_INDEX;
		if (GMath::RandomUnit() < sChanceAttack * in_timeDelta * chanceMul)
		{
			attackAnimation = walk ? TZombieBossAnimation::TWalkAttackBite : TZombieBossAnimation::TFlyAttackBite;
			damageType = ZombieGuiDamage::GetDamageTypeBiteBig();
		}
		else if (GMath::RandomUnit() < sChanceAttack * in_timeDelta * chanceMul)
		{
			if (walk)
			{
				if (0.5F < GMath::RandomUnit())
				{
					attackAnimation = TZombieBossAnimation::TWalkAttackLeft;
					damageType = ZombieGuiDamage::GetDamageTypeSlashLeft();
				}
				else
				{
					attackAnimation = TZombieBossAnimation::TWalkAttackRight;
					damageType = ZombieGuiDamage::GetDamageTypeSlashRight();
				}
			}
			else
			{
				attackAnimation = TZombieBossAnimation::TFlyAttackKick;
				damageType = ZombieGuiDamage::GetDamageTypeSlashLeft();
			}
		}

		if (TZombieBossAnimation::TCount != attackAnimation)
		{
			mParentMission.TakeDamage(2, damageType);
			mStateCountDown = sAttackCountDown;
			PlayAnimation(attackAnimation, true);

			//and face player
			GVector3Float atPlayer = playerPosition - zombiePosition;
			atPlayer.m_y = (0.0F);
			mLerpAt->SetValue(Normalise(atPlayer), 0.25F, GComponentLerp::TType::TEaseInOut);
			mLerpUp->SetValue(GVector3Float::sUnitY, 0.25F, GComponentLerp::TType::TEaseInOut);
		}
		else
		{
			if (walk)
			{
				PlayAnimation(TZombieBossAnimation::TWalkIdle);
			}
			else
			{
				PlayAnimation(TZombieBossAnimation::TFlyIdle);
			}
		}
	}

	return;
}

void SceneNodeComponentClientZombieBoss::PlayAnimation(const int in_animation, const bool in_force)
{
	if ((in_animation == mCurrentAnimation) && (!in_force))
	{
		return;
	}
	mCurrentAnimation = in_animation;

	TPointerSceneNodeComponentAnimation pointerAnimation = mAnimation.lock();
	if (!pointerAnimation)
	{
		return;
	}
	GAnimationManagerInstance& animationManagerInstance = *(pointerAnimation->GetAnimationManagerInstance());
	animationManagerInstance.StopAnimation(sZombieBossAnimationDefaultFadeTime);

	if ((0 <= mCurrentAnimation) &&
		(mCurrentAnimation < TZombieBossAnimation::TCount))
	{
		const GAnimationPlayParam defaultParam(
			sZombieBossAnimationRate[mCurrentAnimation],
			sZombieBossAnimationDefaultFadeTime
			);
		animationManagerInstance.PlayAnimation(
			sZombieBossAnimationName[mCurrentAnimation],
			defaultParam
			);
	}

	return;
}

void SceneNodeComponentClientZombieBoss::SpawnSlugJaw()
{
	TPointerSkeletonInstance pointerSkeleton = mWeakSkeletonInstance.lock();
	TPointerSceneNode pointerParentSceneNode = mParentSceneNode.lock();
	if (!pointerSkeleton || !pointerParentSceneNode)
	{
		return;
	}

	const GMatrix16Float jawTransform = pointerSkeleton->GetBoneObjectMatrix(mBoneJawIndex) * pointerParentSceneNode->GetWorldTransform();

	const GVector3Float at = CrossProduct(jawTransform.GetUp(), jawTransform.GetAt());
	mParentMission.SpawnSlug(
		at,
		jawTransform.GetUp(),
		jawTransform.GetPosition(),
		(at * 5.0F) + GVector3Float(0.0F, 5.0F, 0.0F)
		);

	TPointerZombieSound pZombieSound = mParentMission.GetSound();
	pZombieSound->Play(ZombieSound::GetSoundIDHiss1(), jawTransform.GetPosition());

	return;
}

void SceneNodeComponentClientZombieBoss::UpdateMaggotVisual()
{
	const int offset = GMath::Maximum(0, mMaggotCountHigh - TZombieBossVar::TMaggotCount);
	for (int index = 0; index < TZombieBossVar::TMaggotCount; ++index)
	{
		TPointerSceneNodeComponentVisual pointerVisual = mArrayMaggotVisual[index].lock();
		if (!pointerVisual)
		{
			continue;
		}
		GSceneNodeComponentVisual& visual = *pointerVisual;
		const bool visible = index + offset < mMaggotCount;
		visual.SetVisible(visible);
	}

	return;
}

void SceneNodeComponentClientZombieBoss::DropMaggot(GScene& inout_scene, TPointerSceneNode& inout_sceneNodeRoot)
{
	TPointerSkeletonInstance pointerSkeleton = mWeakSkeletonInstance.lock();
	TPointerSceneNode pointerParentSceneNode = mParentSceneNode.lock();
	if (!pointerSkeleton || !pointerParentSceneNode || !inout_sceneNodeRoot)
	{
		return;
	}
	GSceneNode& sceneNodeRoot = *inout_sceneNodeRoot;

	const GMatrix16Float referenceTransform = pointerSkeleton->GetBoneObjectMatrix(mBoneSpineIndex) * pointerParentSceneNode->GetWorldTransform();

	TPointerSceneNode pointerNewSceneNode;
	pointerNewSceneNode.reset(new GSceneNode("dropMaggot"));
	GSceneNode& newSceneNode = *pointerNewSceneNode;
	newSceneNode.SetParentRelativeTransform(referenceTransform);

	//add visual
	TPointerSceneNodeComponentBase pointerVisual = GSceneNodeComponentVisual::Factory(
		inout_scene, 
		"",
		"maggot.data",
		"material_00.data",
		0.5F,
		GSceneNodeComponentVisual::TVisualFlag::TVisable
		);
	newSceneNode.AddComponent(pointerVisual);

	//add fade commponent
	TPointerSceneNodeComponentBase pointerFade;
	pointerFade.reset(new GSceneNodeComponentFade(inout_scene, 5.0F));
	newSceneNode.AddComponent(pointerFade);

	//add physics component
	TPointerSceneNodeComponentBase pointerPhysics;
	pointerPhysics.reset(new GSceneNodeComponentPhysics(
		1.0F,
		referenceTransform.GetPosition(),
		GVector3Float(GMath::RandomUnitPlusMinius(), GMath::RandomUnitPlusMinius(), GMath::RandomUnitPlusMinius()), 
		GSceneNodeComponentPhysicsLoad::TFlag::TAllowMove
		));
	newSceneNode.AddComponent(pointerPhysics);

	sceneNodeRoot.AddChildNode(pointerNewSceneNode);


	return;
}
