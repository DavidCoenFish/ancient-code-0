//
//  SceneNodeComponentClientZombieFly.cpp
//  Zombie
//
//  Created by David Coen on 2011 03 22
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "SceneNodeComponentClientZombieFly.h"

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
#include <GSceneNodeComponentParticle.h>
#include <GComponentLerpVector3Float.h>
#include <GAnimationPlayParam.h>
#include <GSceneNodeComponentFade.h>
#include <GAssert.h>
#include <GSceneVisitorCollision.h>
#include <GSceneVisitorAll.h>

//softbinding names
struct TZombieFlyState
{
	enum TEnum
	{
		TFlyIdle = 0,
		TFly,
		TFlyRecoil,
		TFallToGround,
		TDragIdle,
		TDrag,
		TDragRecoil,
		TDead,

		TCount
	};
};

struct TZombieFlyAnimation
{
	enum TEnum
	{
		TIdle = 0,
		TFly,
		TRecoil,
		TAttack,
		TDeath,
		TDrag_idle,
		TDrag,
		TDrag_recoil,
		TDrag_attack,

		TCount
	};
};
static const float sZombieFlyAnimationRate[TZombieFlyAnimation::TCount] = {
	1.0F, //TIdle = 0,
	1.0F, //TFly,
	1.0F, //TRecoil,
	0.75F, //TAttack,
	1.0F, //TDeath,
	1.0F, //TDrag_idle,
	1.0F, //TDrag,
	1.0F, //TDrag_recoil,
	0.75F //TDrag_attack,
};

static const char* const sZombieFlyAnimationName[TZombieFlyAnimation::TCount] = {
	"idle",
	"fly",
	"recoil",
	"attack",
	"death",
	"drag_idle",
	"drag",
	"drag_recoil",
	"drag_attack"
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
static const float sZombieFlyHealthFactor = 4.0F;
static const float sZombieFlyDeathDuration = 1.5F;
static const float sZombieFlyAnimationDefaultFadeTime = 5.0F / 30.0F;
static const float sRecoilCountDown = 5.0F / 30.0F;
static const float sAttackCountDown = 15.0F / 30.0F; //(10 / .75) / 30
static const float sChanceAttack = 1.0F / 2.0F;

//////////////////////////////////////////////////////////////////
// Local static methods
//////////////////////////////////////////////////////////////////

//static public methods
/*static*/ SceneNodeComponentClientZombieFly::TPointerSceneNodeComponentBase SceneNodeComponentClientZombieFly::Factory(
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
	const float healthFactor = zombieGameVar.GetZombieHealthFactor() * sZombieFlyHealthFactor;
	const std::string& factoryData = inout_scene.GetFactoryData();

	TPointerSceneNodeComponentClientZombieFly pointerZombieFly;
	pointerZombieFly.reset(new SceneNodeComponentClientZombieFly(
		zombieMission,
		inout_owner,
		factoryData,
		healthFactor,
		healthFactor
		));

	pointer = pointerZombieFly;

	inout_arrayPostLoadCallback.push_back(GScenePostLoadCallback(
		inout_owner,
		pointer,
		SceneNodeComponentClientZombieFly::PostLoadCallback
		));

	TPointerSceneNodeComponentClientObjectBase pointerObjectBase = pointerZombieFly;
	zombieMission.AddClientOpponent(*pointerObjectBase);

	return pointer;
}

//static private methods
/*static*/ void SceneNodeComponentClientZombieFly::PostLoadCallback(
	GScene& inout_scene, //access to mesh/ material manager
	TPointerSceneNode& inout_owner,
	TPointerSceneNodeComponentBase& inout_component,
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback
	)
{
	TPointerSceneNodeComponentClientZombieFly pointer = boost::dynamic_pointer_cast<SceneNodeComponentClientZombieFly>(inout_component);
	if (!pointer || !inout_owner)
	{
		return;
	}
	SceneNodeComponentClientZombieFly& zombieFly = *pointer;

	GSceneNode& sceneNodeParent = *inout_owner;

	//find the collision
	GSceneVisitorAllCollect<GSceneNodeComponentCollision>::Run(inout_owner, zombieFly.mArrayCollision);

	//find the animation
	const int componentCount = sceneNodeParent.GetComponentCount();
	for (int index = 0; index < componentCount; ++index)
	{
		TPointerSceneNodeComponentAnimation pointerAnimation = sceneNodeParent.GetComponentPointer<GSceneNodeComponentAnimation>(index);
		if (pointerAnimation)
		{
			zombieFly.mAnimation = pointerAnimation;
			break;
		}
	}

	return;
}

//constructor
SceneNodeComponentClientZombieFly::SceneNodeComponentClientZombieFly(
	ZombieMission& in_parent,
	TPointerSceneNode& in_parentSceneNode,
	const std::string& in_factoryData,
	const float in_healthFly,
	const float in_healthDrag
	)
	: SceneNodeComponentClientObjectBase(in_parentSceneNode->GetName())
	, mParentMission(in_parent)
	, mParentSceneNode(in_parentSceneNode)
	, mFactoryData(in_factoryData)
	, mHealthFly(in_healthFly)
	, mHealthDrag(in_healthDrag)
	, mTransformDirty(true)
	, mAiControl(false)
	, mStateCountDown(0.0F)
	, mState(TZombieFlyState::TFlyIdle)
	, mCurrentAnimation(TZombieFlyAnimation::TIdle)
	, mLerpPosition()
	, mLerpAt()
	, mLerpUp()
{
	const GMatrix16Float& worldTransform = in_parentSceneNode->GetWorldTransform();
	mLerpPosition.reset(new GComponentLerpVector3Float(worldTransform.GetPosition()));
	mLerpAt.reset(new GComponentLerpVector3Float(worldTransform.GetAt()));
	mLerpUp.reset(new GComponentLerpVector3Float(worldTransform.GetUp()));

	return;
}

/*virtual*/ SceneNodeComponentClientZombieFly::~SceneNodeComponentClientZombieFly()
{
	return;
}

//implement GSceneNodeComponentClient
/*virtual*/ void SceneNodeComponentClientZombieFly::OnTick(TPointerSceneNode& inout_sceneNode, const float in_timeDelta)
{
	if (mAiControl)
	{
		mStateCountDown -= in_timeDelta;
	}

	if (mStateCountDown < 0.0F)
	{
		switch (mState)
		{
		default:
			break;
		case TZombieFlyState::TDead:
			mParentMission.RequestNodeDestroy(inout_sceneNode);
			return;
			break;
		case TZombieFlyState::TFlyIdle:
		case TZombieFlyState::TFly:
		case TZombieFlyState::TFlyRecoil:
			mState = TZombieFlyState::TFlyIdle;
			AiSelectAction(in_timeDelta);
			break;
		case TZombieFlyState::TFallToGround:
		case TZombieFlyState::TDragIdle:
		case TZombieFlyState::TDrag:
		case TZombieFlyState::TDragRecoil:
			mState = TZombieFlyState::TDragIdle;
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
/*virtual*/ void SceneNodeComponentClientZombieFly::OnCollectDamageReceivers(
	TArrayPairClientObjectCollisionResult& out_arrayClientObjectCollisionResult,
	const GVector3Float& in_worldSpaceConeOrigin,
	const GVector3Float& in_worldSpaceConeDirection,
	const float in_coneLength,
	const float in_coneRadiusPoint,
	const float in_coneRadiusBase
	)
{
	if (TZombieFlyState::TDead == mState)
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

/*virtual*/ void SceneNodeComponentClientZombieFly::OnApplyDamage(
	float& inout_damage, 
	const std::string& in_data
	)
{
	if (TZombieFlyState::TDead == mState)
	{
		return;
	}

	//if you shoot at cinematic zombie, ai is activated and it startes chasing the player
	mAiControl = true;

	if (0.0F < mHealthFly)
	{
		const float damage = GMath::Minimum(GMath::Maximum(0.0F, mHealthFly), -inout_damage);
		inout_damage += damage;
		mHealthFly -= damage;

		if (mHealthFly <= 0.0F)
		{
			mState = TZombieFlyState::TFallToGround;
			GVector3Float position = mLerpPosition->GetValue();
			mStateCountDown = position.m_y / 10.0F;
			position.m_y = (0.0F);
			mLerpPosition->SetValue(position, mStateCountDown);
			PlayAnimation(TZombieFlyAnimation::TRecoil, true);

			GVector3Float at = mLerpAt->GetValue();
			at.m_y = (0.0F);
			at = Normalise(at);
			mLerpAt->SetValue(at, mStateCountDown);
			mLerpUp->SetValue(GVector3Float::sUnitY, mStateCountDown);
		}
		else 
		{
			mState = TZombieFlyState::TFlyRecoil;
			mStateCountDown = sRecoilCountDown;
			PlayAnimation(TZombieFlyAnimation::TRecoil, true);
		}
	}

	if (inout_damage < 0.0F)
	{
		const float damage = GMath::Minimum(GMath::Maximum(0.0F, mHealthDrag), -inout_damage);
		inout_damage += damage;
		mHealthDrag -= damage;

		if (mHealthDrag <= 0.0F)
		{
			mState = TZombieFlyState::TDead;
			mStateCountDown = sZombieFlyDeathDuration;
			PlayAnimation(TZombieFlyAnimation::TDeath, true);
			GVector3Float position = mLerpPosition->GetValue();
			const float height = position.m_y;
			position.m_y = (0.0F);
			mLerpPosition->SetValue(position, height / 10.0F);

			//recorde death
			ZombieGameVar& gameVar = *(mParentMission.GetGameVar());
			gameVar.SetPersistanceKillCount(gameVar.GetPersistanceKillCount() + 1);
		}
		else 
		{
			mState = TZombieFlyState::TDragRecoil;
			mStateCountDown = sRecoilCountDown;
			PlayAnimation(TZombieFlyAnimation::TRecoil, true);
		}
	}

	return;
}

/*virtual*/ void SceneNodeComponentClientZombieFly::OnAfterDamagePass(GScene& inout_scene, TPointerSceneNode& inout_sceenRoot)
{
	return;
}

/*virtual*/ void SceneNodeComponentClientZombieFly::OnAnimate(
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

	animationManagerInstance.StopAnimation(sZombieFlyAnimationDefaultFadeTime);
	animationManagerInstance.PlayAnimation(
		in_animationName,
		in_playParam
		);

	return;
}

/*virtual*/ void SceneNodeComponentClientZombieFly::OnMove(
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

/*virtual*/ void SceneNodeComponentClientZombieFly::OnRotate(
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

/*virtual*/ void SceneNodeComponentClientZombieFly::OnSetAiActive(const bool in_active)
{
	mAiControl = in_active;
	return;
}

/*virtual*/ const bool SceneNodeComponentClientZombieFly::OnGetAiActive()const
{
	return mAiControl;
}

/*virtual*/ void SceneNodeComponentClientZombieFly::OnDestroy()
{
	TPointerSceneNode pointerSceneNode = mParentSceneNode.lock();
	mParentMission.RequestNodeDestroy(pointerSceneNode);
}

//private methods
void SceneNodeComponentClientZombieFly::AiSelectAction(const float in_timeDelta)
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

	const bool moveTowardsPlayer = (4.0F < distanceToPlayerSquared);

	bool drag = false;
	switch (mState)
	{
	default:
		break;
	case TZombieFlyState::TDrag:
	case TZombieFlyState::TDragIdle:
	case TZombieFlyState::TDragRecoil:
		drag = true;
		break;
	}

	if (drag && (0.0F != zombiePosition.m_y))
	{
		zombiePosition.m_y = (0.0F);
		mLerpPosition->SetValue(zombiePosition, 0.0F);
	}

	if (moveTowardsPlayer)
	{
		TPointerZombieSound pZombieSound = mParentMission.GetSound();
		if (pZombieSound)
		{
			if (GMath::RandomUnit() < 0.5F)
			{
				pZombieSound->Play(ZombieSound::GetSoundIDHiss4(), mLerpPosition->GetValue());
			}
		}

		GVector3Float playerTarget = playerPosition + (playerAt * 1.5F) + ((0.33F * GMath::RandomUnitPlusMinius()) * playerCross);
		float distanceToPlayer = 0.0F;
		if (!drag)
		{
			playerTarget.m_y = (1.25F);
		}

		GVector3Float normalToPlayerTarget = Normalise(playerTarget - zombiePosition, distanceToPlayer);
		//GVector3Float normalToPlayer = NormaliseAprox(playerPosition - playerTarget);
		//mLerpAt->SetValue(normalToPlayer, 0.25F, GComponentLerp::TType::TEaseInOut);
		mLerpAt->SetValue(normalToPlayerTarget, 0.25F, GComponentLerp::TType::TEaseInOut);
		mLerpUp->SetValue(Normalise(CrossProduct(normalToPlayerTarget, playerCross)), 0.25F, GComponentLerp::TType::TEaseInOut);

		float speed = 2.5F;
		if (drag)
		{
			speed = 0.5F;
		}
		else
		{
			switch (difficulty)	
			{
			default:
				break;
			case ZombieStateType::TGameDifficulty::TNormal:
				speed = 3.5F;
				break;
			case ZombieStateType::TGameDifficulty::THard:
				speed = 4.5F;
				break;
			}
		}

		if (speed)
		{
			float timeToPlayer = distanceToPlayer / speed;
			mLerpPosition->SetValue(playerTarget, timeToPlayer, GComponentLerp::TType::TEaseInOut);
			mStateCountDown = timeToPlayer;

			if (drag)
			{
				PlayAnimation(TZombieFlyAnimation::TDrag);
			}
			else
			{
				PlayAnimation(TZombieFlyAnimation::TFly);
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
		int attackAnimation = TZombieFlyAnimation::TCount;
		int damageType = GCOMMON_INVALID_INDEX;
		if (GMath::RandomUnit() < sChanceAttack * in_timeDelta * chanceMul)
		{
			attackAnimation = drag ? TZombieFlyAnimation::TDrag_attack : TZombieFlyAnimation::TAttack;
			damageType = ZombieGuiDamage::GetDamageTypeBiteBig();
		}

		if (TZombieFlyAnimation::TCount != attackAnimation)
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
			if (drag)
			{
				PlayAnimation(TZombieFlyAnimation::TDrag_idle);
			}
			else
			{
				PlayAnimation(TZombieFlyAnimation::TIdle);
			}
		}
	}

	return;
}

void SceneNodeComponentClientZombieFly::PlayAnimation(const int in_animation, const bool in_force)
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
	animationManagerInstance.StopAnimation(sZombieFlyAnimationDefaultFadeTime);

	if ((0 <= mCurrentAnimation) &&
		(mCurrentAnimation < TZombieFlyAnimation::TCount))
	{
		const GAnimationPlayParam defaultParam(
			sZombieFlyAnimationRate[mCurrentAnimation],
			sZombieFlyAnimationDefaultFadeTime
			);
		animationManagerInstance.PlayAnimation(
			sZombieFlyAnimationName[mCurrentAnimation],
			defaultParam
			);
	}

	return;
}
