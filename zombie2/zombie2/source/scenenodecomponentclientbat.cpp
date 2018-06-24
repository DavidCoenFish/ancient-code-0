//
//  SceneNodeComponentClientBat.cpp
//  Zombie
//
//  Created by David Coen on 2011 03 20
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "SceneNodeComponentClientBat.h"

#include "ZombieMission.h"
#include "ZombieGameVar.h"
#include "ZombieGuiDamage.h"
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
#include <GCamera.h>

typedef boost::shared_ptr<SceneNodeComponentClientObjectBase> TPointerSceneNodeComponentClientObjectBase;
typedef boost::shared_ptr<GSceneNodeComponentCollisionResult> TPointerSceneNodeComponentCollisionResult;
typedef std::vector<TPointerSceneNodeComponentCollisionResult> TArrayPointerSceneNodeComponentCollisionResult;
typedef boost::shared_ptr<ZombieSound> TPointerZombieSound;

static const float sJumpTime = 1.0F;
static const int sDefaultApproachPlayerCount = 4;
static const float sBatAnimationDefaultFadeTime = 0.08F;
static const float sBatPlayerHeight = 2.0F;

//for scene 07, TODO: get scene id from gameVar
static const float sBatScene06MinX = 18.75F;
static const float sBatScene06MaxX = 25.0F;

struct TState
{
	enum TEnum
	{
		TSpawn = 0,
		TFly,
		TDive,
		TAttached,
		TDead,
		TCount
	};
};

struct TBatAnimation
{
	enum TEnum
	{
		TFly = 0,
		TDive,
		TBite,
		TDeath,
		TCount
	};
};

static const char* const sArrayBatAnimationName[TBatAnimation::TCount] = {
	"fly",
	"dive",
	"bite",
	"death"
};

static const float sArrayBatAnimationRate[TBatAnimation::TCount] = {
	1.0F,
	1.0F,
	1.0F,
	1.0F
};

/**/
//static public methods
/*static*/ SceneNodeComponentClientBat::TPointerSceneNodeComponentBase SceneNodeComponentClientBat::Factory(
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
	ZombieGameVar& zombieGameVar = *(zombieMission.GetGameVar());
	const float healthFactor = zombieGameVar.GetZombieHealthFactor();

	TPointerSceneNodeComponentClientBat pointerZombieOne;
	pointerZombieOne.reset(new SceneNodeComponentClientBat(
		zombieMission,
		inout_owner,
		healthFactor
		));

	pointer = pointerZombieOne;
	inout_arrayPostLoadCallback.push_back(GScenePostLoadCallback(
		inout_owner,
		pointer,
		SceneNodeComponentClientBat::PostLoadCallback
		));

	TPointerSceneNodeComponentClientObjectBase pointerObjectBase = pointerZombieOne;
	zombieMission.AddClientOpponent(*pointerObjectBase);

	return pointer;
}

//static private methods
/*static*/ void SceneNodeComponentClientBat::PostLoadCallback(
	GScene& inout_scene, //access to mesh/ material manager
	TPointerSceneNode& inout_owner,
	TPointerSceneNodeComponentBase& inout_component,
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback
	)
{
	TPointerSceneNodeComponentClientBat pointer = boost::dynamic_pointer_cast<SceneNodeComponentClientBat>(inout_component);
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

		TPointerSceneNodeComponentAnimation pointerAnimation = sceneNode.GetComponentPointer<GSceneNodeComponentAnimation>(index);
		if (pointerAnimation)
		{
			pointer->mAnimation = pointerAnimation;
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
SceneNodeComponentClientBat::SceneNodeComponentClientBat(
	ZombieMission& in_parent,
	TPointerSceneNode& in_parentSceneNode,
	const float in_healthFactor
	)
	: SceneNodeComponentClientObjectBase(in_parentSceneNode->GetName())
	, mSceneNode(in_parentSceneNode)
	, mParentMission(in_parent)
	, mHealth(in_healthFactor)
	, mStateTimer(0.0F)
	, mState(TState::TSpawn)
	, mApproachPlayerCount(sDefaultApproachPlayerCount)
	, mCurrentAnimation(TBatAnimation::TCount)
	, mAnimationCountdown(0.0F)
{
	const GMatrix16Float& worldTransform = in_parentSceneNode->GetWorldTransform();
	mLerpPosition.reset(new GComponentLerpVector3Float(worldTransform.GetPosition()));
	mLerpAt.reset(new GComponentLerpVector3Float(worldTransform.GetAt()));
	mLerpUp.reset(new GComponentLerpVector3Float(worldTransform.GetUp()));

	return;
}

/*virtual*/ SceneNodeComponentClientBat::~SceneNodeComponentClientBat()
{
	return;
}

//implement GSceneNodeComponentClient
/*virtual*/ void SceneNodeComponentClientBat::OnTick(TPointerSceneNode& inout_sceneNode, const float in_timeDelta)
{
	if (0.0F < mAnimationCountdown)
	{
		mAnimationCountdown -= in_timeDelta;
		if (mAnimationCountdown <= 0.0F)
		{
			switch(mState)
			{
			default:
				PlayAnimation(TBatAnimation::TFly);
				break;
			case TState::TDive:
				PlayAnimation(TBatAnimation::TDive);
				break;
				break;
			case TState::TDead:
				PlayAnimation(TBatAnimation::TDeath);
				break;
			case TState::TAttached:
				PlayAnimation(TBatAnimation::TBite);
				break;
			}
		}
	}

	//hack, if in dive state, and up to player, finish dive state
	if (TState::TDive == mState)
	{
		ZombieGameVar& gameVar = *mParentMission.GetGameVar();
		const GVector3Float playerPosition = gameVar.GetPlayerPosition();
		const GVector3Float playerAt = gameVar.GetPlayerDirection();
		const GVector3Float batPosition = mLerpPosition->GetValue();

		const float offset = DotProduct(playerAt, batPosition - playerPosition);
		if (offset < 0.25F)
		{
			mStateTimer = -1.0F;
		}
	}

	mStateTimer -= in_timeDelta;
	bool doAttack = false;
	bool doMove = false;
	if (mStateTimer < 0.0F)
	{
		switch (mState)
		{
		default:
			break;
		case TState::TDive:
			{
				//finished jump (attach/ attack player)
				mState = TState::TAttached;
				PlayAnimation(TBatAnimation::TBite);
				mStateTimer = 1.0F;
				mParentMission.TakeDamage(1, ZombieGuiDamage::GetDamageTypeBiteSmall());

				GVector3Float position = mLerpPosition->GetValue();

				GCamera& camera = mParentMission.GetCamera();
				GVector3Float screenSpace = position * camera.GetProjectionMatrix();
				if (screenSpace.m_z)
				{
					screenSpace.m_x = (screenSpace.m_x / screenSpace.m_z);
					screenSpace.m_y = (screenSpace.m_y / screenSpace.m_z);
				}

				mScreenPosAttach.SetData(
					GMath::Clamp(screenSpace.m_x, -0.9F, 0.9F) * 0.5F * camera.GetAspect(), 
					GMath::Clamp(screenSpace.m_y, -0.9F, 0.9F) * 0.5F
					);

				TPointerZombieSound pZombieSound = mParentMission.GetSound();
				if (pZombieSound)
				{
					pZombieSound->Play(ZombieSound::GetSoundIDHiss3(), mLerpPosition->GetValue());
				}
			}
			break;
		case TState::TDead:
			mParentMission.RequestNodeDestroy(inout_sceneNode);
			return;	
		case TState::TSpawn:
		case TState::TFly:
			mApproachPlayerCount -= 1;
			if (0 < mApproachPlayerCount)
			{
				doMove = true;
				PlayAnimation(TBatAnimation::TFly);
			}
			else
			{
				doAttack = true;
				PlayAnimation(TBatAnimation::TDive);
			}
			break;
		case TState::TAttached:
			//todo: wriggle anim?
			mParentMission.TakeDamage(1, ZombieGuiDamage::GetDamageTypeBiteSmall());
			mStateTimer = 1.0F;

			TPointerZombieSound pZombieSound = mParentMission.GetSound();
			if (pZombieSound)
			{
				pZombieSound->Play(ZombieSound::GetSoundIDHiss3(), mLerpPosition->GetValue());
			}

			break;
		}
	}

	if (doMove)
	{
		//move closer to player
		ZombieGameVar& gameVar = *mParentMission.GetGameVar();
		const GVector3Float playerPosition = gameVar.GetPlayerPosition();
		const GVector3Float playerAt = gameVar.GetPlayerDirection();
		const GVector3Float playerCross = CrossProduct(playerAt, GVector3Float::sUnitY);
		const GVector3Float batPosition = mLerpPosition->GetValue();

		const GVector3Float playerTarget = playerPosition + playerAt + ((0.7F * GMath::RandomUnitPlusMinius()) * playerCross) + GVector3Float(0.0F, sBatPlayerHeight, 0.0F);
		GVector3Float batTheoreticalTarget = 
			batPosition + 
			(playerAt * GMath::RandomUnit()) + 
			((3.0F * GMath::RandomUnitPlusMinius()) * playerCross) +
			GVector3Float(0.0F, GMath::RandomUnitPlusMinius(), 0.0F)
			;

		if (ZombieMission::GetMissionIdSceneBats() == gameVar.GetMissionId())
		{
			batTheoreticalTarget.m_x = (GMath::Clamp(batTheoreticalTarget.m_x, sBatScene06MinX, sBatScene06MaxX));
		}

		const float ratio = 0.5F / (float(mApproachPlayerCount));
		const GVector3Float batTarget = Lerp(ratio, batTheoreticalTarget, playerTarget);

		float distance = 0.0F;
		const GVector3Float normalToBatTarget = NormaliseAprox(batTarget - batPosition, distance);

		float speed = 3.0F;
		const ZombieStateType::TGameDifficulty::TEnum difficulty = gameVar.GetGameDifficulty();
		switch (difficulty)
		{
		default:
			break;
		case ZombieStateType::TGameDifficulty::TNormal:
			speed = 3.5F;
			break;
		case ZombieStateType::TGameDifficulty::THard:
			speed = 4.0F;
			break;
		}

		float timeToPlayer = distance / speed;
		mLerpPosition->SetValue(batTarget, timeToPlayer, GComponentLerp::TType::TEaseInOut);
		mStateTimer = timeToPlayer;

		mLerpAt->SetValue(normalToBatTarget, 0.25F, GComponentLerp::TType::TEaseInOut);
		const GVector3Float batUp = Normalise(CrossProduct(normalToBatTarget, playerCross));
		mLerpUp->SetValue(batUp, 0.25F, GComponentLerp::TType::TEaseInOut);
	}
	if (doAttack)
	{
		mState = TState::TDive;
		mStateTimer = sJumpTime;

		ZombieGameVar& gameVar = *mParentMission.GetGameVar();
		const GVector3Float playerPosition = gameVar.GetPlayerPosition();
		const GVector3Float playerAt = gameVar.GetPlayerDirection();
		const GVector3Float playerCross = CrossProduct(playerAt, GVector3Float::sUnitY);
		const GVector3Float batPosition = mLerpPosition->GetValue();

		const GVector3Float playerTarget = 
			playerPosition + 
			(playerAt * 2.0F) + 
			((0.7F * GMath::RandomUnitPlusMinius()) * playerCross) + 
			GVector3Float(0.0F, 1.25F + GMath::RandomUnit(), 0.0F);

		mLerpPosition->SetValue(playerTarget, mStateTimer, GComponentLerp::TType::TLinear);

		const GVector3Float normalBatToPlayer = Normalise(playerTarget - batPosition);
		mLerpAt->SetValue(normalBatToPlayer, 0.25F, GComponentLerp::TType::TEaseInOut);
		const GVector3Float batUp = Normalise(CrossProduct(normalBatToPlayer, playerCross));
		mLerpUp->SetValue(batUp, 0.25F, GComponentLerp::TType::TEaseInOut);
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

	if (TState::TAttached == mState)
	{ 
		GCamera& camera = mParentMission.GetCamera();
		GVector3Float position = camera.GetTransformPos() +
			camera.GetTransformAt() + 
			(camera.GetTransformUp() * mScreenPosAttach.m_y) +
			(CrossProduct(camera.GetTransformAt(), camera.GetTransformUp()) * mScreenPosAttach.m_x);

		mLerpPosition->SetValue(position);

		const GMatrix16Float transform = GMatrix16FloatConstructAtUp(
			-camera.GetTransformAt(), 
			camera.GetTransformUp(),
			GVector3Float::sUnitZ, 
			GVector3Float::sUnitY, 
			position
			);
		inout_sceneNode->SetParentRelativeTransform(transform);
	}
	else if (mTransformDirty)
	{
		mTransformDirty = false;
		GVector3Float at = mLerpAt->GetValue();
		GVector3Float up = mLerpUp->GetValue();
		if ((mLerpAt->GetActive()) ||
			(mLerpUp->GetActive()))
		{
			Orthogonalise(at, up);
		}

		GVector3Float position = mLerpPosition->GetValue();

		const GMatrix16Float transform = GMatrix16FloatConstructAtUp(
			at, 
			up,
			GVector3Float::sUnitZ, 
			GVector3Float::sUnitY, 
			position
			);
		inout_sceneNode->SetParentRelativeTransform(transform);
	}

	return;
}
//implement SceneNodeComponentClientObjectBase
/*virtual*/ void SceneNodeComponentClientBat::OnCollectDamageReceivers(
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

/*virtual*/ void SceneNodeComponentClientBat::OnApplyDamage(
	float& inout_damage, 
	const std::string& in_data
	)
{
	const float damage = GMath::Minimum(GMath::Maximum(0.0F, mHealth), -inout_damage);
	inout_damage += damage;
	mHealth -= damage;
	return;
}

/*virtual*/ void SceneNodeComponentClientBat::OnAfterDamagePass(GScene& inout_scene, TPointerSceneNode& inout_sceenRoot)
{
	if ((TState::TDead != mState) && (mHealth <= 0.0F))
	{
		GVector3Float position = mLerpPosition->GetValue();

		mState = TState::TDead;
		mStateTimer = 2.5F;
		PlayAnimation(TBatAnimation::TDeath);

		const float oldHeight = position.m_y;
		mLerpPosition->SetValue(position);
		position.m_y = (-1.0F);
		position += GVector3Float(GMath::RandomUnitPlusMinius() * 2.0F, 0.0F, GMath::RandomUnitPlusMinius() * 2.0F);
		mLerpPosition->SetValue(position, oldHeight * 0.15F, GComponentLerp::TType::TLinear);

		TPointerSceneNodeComponentCollision pointerCollision = mCollision.lock();
		if (pointerCollision)
		{
			pointerCollision->SetActive(false);
		}

		TPointerSceneNodeComponentParticle pointerParticle = mParticle.lock();
		if (pointerParticle)
		{
			pointerParticle->SetVisible(true);
			pointerParticle->RequestEmittor(
				2, 
				0.5F
				);
		}

		TPointerZombieSound pZombieSound = mParentMission.GetSound();
		if (pZombieSound)
		{
			pZombieSound->Play(ZombieSound::GetSoundIDSlugDeath1(), mLerpPosition->GetValue());
		}

		//recode death
		ZombieGameVar& gameVar = *(mParentMission.GetGameVar());
		gameVar.SetPersistanceKillCount(gameVar.GetPersistanceKillCount() + 1);
	}
	else
	{
		mAnimationCountdown = 0.3F;
		PlayAnimation(TBatAnimation::TDeath);
	}

	return;
}

/*virtual*/ const bool SceneNodeComponentClientBat::OnGetAiActive()const
{
	return true;
}

/*virtual*/ void SceneNodeComponentClientBat::OnDestroy()
{
	TPointerSceneNode pointerSceneNode = mSceneNode.lock();
	mParentMission.RequestNodeDestroy(pointerSceneNode);
}

//private methods
/**/
void SceneNodeComponentClientBat::PlayAnimation(const int in_animation, const bool in_force)
{
	if (!in_force)
	{
		if ((in_animation == mCurrentAnimation) ||
			(0.0F < mAnimationCountdown))
		{
			return;
		}

	}
	mCurrentAnimation = in_animation;

	TPointerSceneNodeComponentAnimation pointerAnimation = mAnimation.lock();
	if (!pointerAnimation)
	{
		return;
	}
	GAnimationManagerInstance& animationManagerInstance = *(pointerAnimation->GetAnimationManagerInstance());
	animationManagerInstance.StopAnimation(sBatAnimationDefaultFadeTime);

	if ((0 <= mCurrentAnimation) &&
		(mCurrentAnimation < TBatAnimation::TCount))
	{
		const GAnimationPlayParam defaultParam(
			sArrayBatAnimationRate[mCurrentAnimation],
			sBatAnimationDefaultFadeTime
			);
		animationManagerInstance.PlayAnimation(
			sArrayBatAnimationName[mCurrentAnimation],
			defaultParam
			);
	}

	return;
}
/**/