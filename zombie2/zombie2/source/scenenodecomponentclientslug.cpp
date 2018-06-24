//
//  SceneNodeComponentClientSlug.cpp
//  Zombie
//
//  Created by David Coen on 2011 02 22
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "SceneNodeComponentClientSlug.h"

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
static const int sDefaultApproachPlayerCount = 5;
static const float sSlugAnimationDefaultFadeTime = 0.08F;
static const float sSlugHealthFactor = 0.75F;

struct TState
{
	enum TEnum
	{
		TSpawn = 0,
		TIdle,
		TMove,
		TJump,
		TFlight,
		TAttached,
		TDead,
		TCount
	};
};

struct TSlugAnimation
{
	enum TEnum
	{
		TAttach = 0,
		TDeath,
		TIdle,
		TJump,
		TMove,
		TRecoil,
		TCount
	};
};

static const char* const sArraySlugAnimationName[TSlugAnimation::TCount] = {
	"attach",
	"death",
	"idle",
	"jump",
	"move",
	"recoil"
};

static const float sArraySlugAnimationRate[TSlugAnimation::TCount] = {
	1.5F,
	1.0F,
	1.0F,
	1.0F,
	2.0F,
	1.0F
};

static const float LocalCalculateJumpHeight(const float in_jumpHeight, const float in_stateTimer)
{
	//use part of a quadradic curve for the jump
	const float ratio = (((1.0F - (in_stateTimer / sJumpTime)) * 1.5F) -  1.0F);
	const float height = in_jumpHeight * (1.0F - (ratio * ratio));
	return height;
}

/**/
//static public methods
/*static*/ SceneNodeComponentClientSlug::TPointerSceneNodeComponentBase SceneNodeComponentClientSlug::Factory(
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

	TPointerSceneNodeComponentClientSlug pointerZombieOne;
	pointerZombieOne.reset(new SceneNodeComponentClientSlug(
		zombieMission,
		inout_owner,
		healthFactor * sSlugHealthFactor
		));

	pointer = pointerZombieOne;
	inout_arrayPostLoadCallback.push_back(GScenePostLoadCallback(
		inout_owner,
		pointer,
		SceneNodeComponentClientSlug::PostLoadCallback
		));

	TPointerSceneNodeComponentClientObjectBase pointerObjectBase = pointerZombieOne;
	zombieMission.AddClientOpponent(*pointerObjectBase);

	return pointer;
}

//static private methods
/*static*/ void SceneNodeComponentClientSlug::PostLoadCallback(
	GScene& inout_scene, //access to mesh/ material manager
	TPointerSceneNode& inout_owner,
	TPointerSceneNodeComponentBase& inout_component,
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback
	)
{
	TPointerSceneNodeComponentClientSlug pointer = boost::dynamic_pointer_cast<SceneNodeComponentClientSlug>(inout_component);
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
SceneNodeComponentClientSlug::SceneNodeComponentClientSlug(
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
	, mCurrentAnimation(TSlugAnimation::TCount)
	, mAnimationCountdown(0.0F)
{
	const GMatrix16Float& worldTransform = in_parentSceneNode->GetWorldTransform();
	mLerpPosition.reset(new GComponentLerpVector3Float(worldTransform.GetPosition()));
	mLerpAt.reset(new GComponentLerpVector3Float(worldTransform.GetAt()));
	mLerpUp.reset(new GComponentLerpVector3Float(worldTransform.GetUp()));

	return;
}

/*virtual*/ SceneNodeComponentClientSlug::~SceneNodeComponentClientSlug()
{
	return;
}

//implement GSceneNodeComponentClient
/*virtual*/ void SceneNodeComponentClientSlug::OnTick(TPointerSceneNode& inout_sceneNode, const float in_timeDelta)
{
	if (0.0F < mAnimationCountdown)
	{
		mAnimationCountdown -= in_timeDelta;
		if (mAnimationCountdown <= 0.0F)
		{
			switch(mState)
			{
			default:
				PlayAnimation(TSlugAnimation::TIdle);
				break;
			case TState::TFlight:
			case TState::TJump:
				PlayAnimation(TSlugAnimation::TJump);
				break;
			case TState::TMove:
				PlayAnimation(TSlugAnimation::TMove);
				break;
			case TState::TDead:
				PlayAnimation(TSlugAnimation::TDeath);
				break;
			}
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
			//return to idle after spawn, move, flight
			mState = TState::TIdle;
			PlayAnimation(TSlugAnimation::TIdle);
			mStateTimer = GMath::RandomUnit();
			break;
		case TState::TJump:
			{
				//finished jump (attach/ attack player)
				mState = TState::TAttached;
				PlayAnimation(TSlugAnimation::TAttach);
				mStateTimer = 2.0F;
				mParentMission.TakeDamage(1, ZombieGuiDamage::GetDamageTypeBiteSmall());

				GVector3Float position = mLerpPosition->GetValue();
				position.m_y = (mJumpHeight);// * (0.75F));

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
					pZombieSound->Play(ZombieSound::GetSoundIDSlugDeath1(), position);
				}
			}
			break;
		case TState::TDead:
			mParentMission.RequestNodeDestroy(inout_sceneNode);
			return;	
		case TState::TIdle:
			mApproachPlayerCount -= 1;
			if (0 < mApproachPlayerCount)
			{
				doMove = true;
				PlayAnimation(TSlugAnimation::TMove);
			}
			else
			{
				doAttack = true;
				PlayAnimation(TSlugAnimation::TJump);
			}
			break;
		case TState::TAttached:
			//todo: wriggle anim?
			mParentMission.TakeDamage(1, ZombieGuiDamage::GetDamageTypeBiteSmall());
			mStateTimer = 2.0F;

			TPointerZombieSound pZombieSound = mParentMission.GetSound();
			if (pZombieSound)
			{
				pZombieSound->Play(ZombieSound::GetSoundIDSlugDeath1(), mLerpPosition->GetValue());
			}

			break;
		}
	}

	//if slug is in flight, check not past player
	if (TState::TFlight == mState)
	{
		ZombieGameVar& gameVar = *mParentMission.GetGameVar();
		const GVector3Float playerPosition = gameVar.GetPlayerPosition();
		const GVector3Float playerAt = gameVar.GetPlayerDirection();
		const GVector3Float slugPosition = mLerpPosition->GetValue();
		const float distanceToPlayer = DotProduct(playerAt, slugPosition - playerPosition);
		if (distanceToPlayer < 0.5F)
		{
			mState = TState::TAttached;
			PlayAnimation(TSlugAnimation::TAttach);
			mStateTimer = 2.0F;
			//mParentMission.TakeDamage(1, ZombieGuiDamage::GetDamageTypeBiteSmall());

			GVector3Float position = mLerpPosition->GetValue();
			position.m_y = (mJumpHeight);

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
				pZombieSound->Play(ZombieSound::GetSoundIDSlugDeath1(), mLerpPosition->GetValue());
			}
		}
	}

	if (doMove)
	{
		//move closer to player
		ZombieGameVar& gameVar = *mParentMission.GetGameVar();
		const GVector3Float playerPosition = gameVar.GetPlayerPosition();
		const GVector3Float playerAt = gameVar.GetPlayerDirection();
		const GVector3Float playerCross = CrossProduct(playerAt, GVector3Float::sUnitY);
		const GVector3Float slugPosition = mLerpPosition->GetValue();

		const GVector3Float playerTarget = playerPosition + playerAt + ((0.7F * GMath::RandomUnitPlusMinius()) * playerCross);
		const GVector3Float slugTheoreticalTarget = slugPosition + ((2.0F * GMath::RandomUnitPlusMinius()) * playerCross);

		const float ratio = 1.0F / (float(mApproachPlayerCount));
		const GVector3Float slugTarget = Lerp(ratio, slugTheoreticalTarget, playerTarget);

		float distance = 0.0F;
		const GVector3Float normalToSlugTarget = Normalise(slugTarget - slugPosition, distance);

		float speed = 0.5F;
		const ZombieStateType::TGameDifficulty::TEnum difficulty = gameVar.GetGameDifficulty();
		switch (difficulty)
		{
		default:
			break;
		case ZombieStateType::TGameDifficulty::TNormal:
			speed = 0.55F;
			break;
		case ZombieStateType::TGameDifficulty::THard:
			speed = 0.6F;
			break;
		}

		float timeToPlayer = distance / speed;
		mLerpPosition->SetValue(slugTarget, timeToPlayer, GComponentLerp::TType::TEaseInOut);
		mStateTimer = timeToPlayer;

		mLerpAt->SetValue(normalToSlugTarget, 0.25F, GComponentLerp::TType::TEaseInOut);
		mLerpUp->SetValue(GVector3Float::sUnitY, 0.25F, GComponentLerp::TType::TEaseInOut);
	}
	if (doAttack)
	{
		mJumpHeight = 1.0F + GMath::RandomUnit();
		mState = TState::TJump;
		mStateTimer = sJumpTime;
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
	else if (mTransformDirty || (TState::TJump == mState) || (TState::TFlight == mState))
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
		if (TState::TJump == mState)
		{
			const float height = LocalCalculateJumpHeight(mJumpHeight, mStateTimer);
			position.m_y = (height);
		}
		else if (TState::TFlight == mState)
		{
			const float oldVel = mVelocity.m_y;
			mVelocity.m_y = (oldVel - (9.8F * in_timeDelta));
			mJumpHeight += ((oldVel + mVelocity.m_y) * 0.5F * in_timeDelta);
			position.m_y = (mJumpHeight);
		}

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
/*virtual*/ void SceneNodeComponentClientSlug::OnCollectDamageReceivers(
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

/*virtual*/ void SceneNodeComponentClientSlug::OnApplyDamage(
	float& inout_damage, 
	const std::string& in_data
	)
{
	const float damage = GMath::Minimum(GMath::Maximum(0.0F, mHealth), -inout_damage);
	inout_damage += damage;
	mHealth -= damage;

	return;
}

/*virtual*/ void SceneNodeComponentClientSlug::OnAfterDamagePass(GScene& inout_scene, TPointerSceneNode& inout_sceenRoot)
{
	if ((TState::TDead != mState) && (mHealth <= 0.0F))
	{
		GVector3Float position = mLerpPosition->GetValue();
		float oldHeight = position.m_y;
		if (TState::TJump == mState)
		{
			oldHeight = LocalCalculateJumpHeight(mJumpHeight, mStateTimer);
		}
		else if (TState::TFlight == mState)
		{
			oldHeight = mJumpHeight;
		}

		mState = TState::TDead;
		mStateTimer = 2.5F;
		PlayAnimation(TSlugAnimation::TDeath);

		position.m_y = (oldHeight);
		mLerpPosition->SetValue(position);
		position.m_y = (0.0F);
		mLerpPosition->SetValue(position, oldHeight * 0.25F, GComponentLerp::TType::TLinear);

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
			pZombieSound->Play(ZombieSound::GetSoundIDSlugDeath0(), mLerpPosition->GetValue());
		}

		//recode death
		ZombieGameVar& gameVar = *(mParentMission.GetGameVar());
		gameVar.SetPersistanceKillCount(gameVar.GetPersistanceKillCount() + 1);
	}
	else
	{
		mAnimationCountdown = 0.3F;
		PlayAnimation(TSlugAnimation::TRecoil);
	}

	return;
}

/*virtual*/ const bool SceneNodeComponentClientSlug::OnGetAiActive()const
{
	return true;
}

/*virtual*/ void SceneNodeComponentClientSlug::OnDestroy()
{
	TPointerSceneNode pointerSceneNode = mSceneNode.lock();
	mParentMission.RequestNodeDestroy(pointerSceneNode);
}

//public methods
void SceneNodeComponentClientSlug::SetFlightMode(const GVector3Float& in_velocity)
{
	mState = TState::TFlight;
	GVector3Float position = mLerpPosition->GetValue();
	mJumpHeight = position.m_y;
	position.m_y = (0.0F);
	mLerpPosition->SetValue(position);
	mVelocity = in_velocity;

	float rootA = 0.0F;
	float rootB = 0.0F;
	if (GMath::QuadraticRoots(rootA, rootB, -(0.5F * 9.8F), in_velocity.m_y, mJumpHeight))
	{
		mStateTimer = GMath::Maximum(0.0F, GMath::Maximum(rootA, rootB));
	}
	else
	{
		mStateTimer = 0.0F;
	}

	if (0.0F < mStateTimer)
	{
		position.m_x = (position.m_x + (in_velocity.m_x * mStateTimer));
		position.m_z = (position.m_z + (in_velocity.m_z * mStateTimer));

		mLerpPosition->SetValue(position, mStateTimer, GComponentLerp::TType::TLinear);
	}

	GVector3Float at = mLerpAt->GetValue();
	at.m_y = (0.0F);
	at = Normalise(at);
	mLerpAt->SetValue(at);
	mLerpUp->SetValue(GVector3Float::sUnitY);

	PlayAnimation(TSlugAnimation::TJump);

	return;
}

//private methods
/**/
void SceneNodeComponentClientSlug::PlayAnimation(const int in_animation, const bool in_force)
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
	animationManagerInstance.StopAnimation(sSlugAnimationDefaultFadeTime);

	if ((0 <= mCurrentAnimation) &&
		(mCurrentAnimation < TSlugAnimation::TCount))
	{
		const GAnimationPlayParam defaultParam(
			sArraySlugAnimationRate[mCurrentAnimation],
			sSlugAnimationDefaultFadeTime
			);
		animationManagerInstance.PlayAnimation(
			sArraySlugAnimationName[mCurrentAnimation],
			defaultParam
			);
	}

	return;
}
/**/