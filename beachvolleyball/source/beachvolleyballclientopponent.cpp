//
//  BeachVolleyBallClientOpponent.cpp
//  BeachVolleyBall
//
//  Created by David Coen on 2011 04 04
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "BeachVolleyBallClientOpponent.h"
#include "BeachVolleyBallGame.h"
#include "BeachVolleyBallState.h"
#include "BeachVolleyBallClientBall.h"
#include "BeachVolleyBallSound.h"

#include <GComponentLerpFloat.h>
#include <GComponentLerpVector3Float.h>
#include <GSceneNode.h>
#include <GSceneNodeComponentAnimation.h>
#include <GMath.h>
#include <GComponentTilt.h>
#include <GAnimationManagerInstance.h>
#include <GAnimationPlayParam.h>
#include <GScenePostLoadCallback.h>
#include <GCommon.h>
#include <GSkeletonInstance.h>
#include <GScene.h>

typedef boost::shared_ptr<BeachVolleyBallClientOpponent> TPointerBeachVolleyBallClientOpponent;
typedef boost::shared_ptr<BeachVolleyBallClientBall> TPointerBeachVolleyBallClientBall;
typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;
typedef boost::shared_ptr<GAnimationManagerInstance> TPointerAnimationManagerInstance;
typedef boost::shared_ptr<GSkeletonInstance> TPointerSkeletonInstance;

static const float sDefaultAnimationFadeTime = 6.0F / 30.0F;

struct TState
{
	enum TEnum
	{
		TIdle = 0,
		TPlayAnimation,
		TServe,
		TAfterServe,
		TMoveToTakeShot,
		TTakeShot,
		TAfterShot,
		TReturnToIdle,
		TMoveToFall,
		TMoveToFallBack,
		TFall,
		TGameOver,
		TCount
	};
};

struct TGirlData
{
	const float mMaxRunSpeed;
	const float mBallVelocityFactor;
	const float mFallChance;
};
//static const TGirlData sArrayGirlData[BeachVolleyBallType::TGirl::TCount] = 
//{
//	{ 4.25F, 1.0F, 0.33F },
//	{ 4.0F, 0.75F, 0.9F },
//	{ 4.75F, 1.125F, 0.25F },
//	{ 4.5F, 0.75F, 0.5F },
//};
static const TGirlData sArrayGirlData[BeachVolleyBallType::TGirl::TCount] = 
{
	{ 4.0F, 0.8F, 0.33F },
	{ 4.25F, 0.9F, 0.9F },
	{ 4.5F, 0.75F, 0.25F },
	{ 4.75F, 1.0F, 0.5F },
};

struct TAnimation
{
	enum TEnum
	{
		TBack = 0,
		TFall,
		TFallBack,
		THitHigh,
		THitJump,
		THitLow,
		THitMed,
		TIdle,
		TLooseGame,
		TLoosePoint,
		TLooseServe,
		TMenu,
		TRun,
		TServe,
		TWalk,
		TWinGame,
		TWinPoint,
		TWinServe,

		TCount
	};
};

struct TAnimationData
{
	const char* const mName;
	const float mRate;
	const int mFrameCount;
	const int mEventFrame;
};

static const TAnimationData sArrayAnimationData[TAnimation::TCount] = 
{
	{ "back",		1.0f/*1.25f*/, 20, 0 },//tback,
	{ "fall",		0.75f, 70, 0 },//tfall,
	{ "fallback",	0.9f, 75, 0 },//tfall,
	{ "hithigh",	0.75f, 20, 6 },//thithigh, leave hand at frame 6
	{ "hitjump",	0.675f, 25, 10 },//thitjump, leave hand at frame 10
	{ "hitlow",		1.0f, 20, 6 },//thitlow, leave hand at frame 6
	{ "hitmed",		1.0f, 20, 6 },//thitmed, leave hand at frame 6
	{ "idle",		1.0f, 30, 0 },//tidle,
	{ "loosegame",	0.75f, 20, 0 },//tloosegame,
	{ "loosepoint",	0.75f, 20, 0 },//tloosepoint,
	{ "looseserve",	0.75f, 20, 0 },//tlooseserve,
	{ "menu",		0.5f, 30, 0 },//tmenu,
	{ "run",		0.5f/*0.625f*/, 10, 0 },//trun,
	{ "serve",		1.0f, 30, 14 },//tserve, leave hand at frame 14
	{ "walk",		1.0f/*1.25f*/, 20, 0 }, //twalk,
	{ "wingame",	0.75f, 20, 0 },//twingame,
	{ "winpoint",	1.0f, 30, 0 },//twinpoint,
	{ "winserve",	1.0f, 30, 0 },//twinserve,

};

struct TShot
{
	enum TEnum
	{
		TJump = 0,
		THigh,
		TMedium,
		TLow,
		//TFall,

		TCount
	};
};

struct TShotData
{
	const float mHeight[BeachVolleyBallType::TGirl::TCount]; //axisY
	const float mDepth[BeachVolleyBallType::TGirl::TCount];  //axisZ
	const float mVelocity; //release velocity
	const float mBias;
	const int mAnimationId;
};

static const TShotData sArrayShotData[TShot::TCount] = 
{	
	{	{	2.5F,	2.356F,	2.285F,	2.593F	},	{	0.0F,	0.0F,	0.0F,	0.0F	},		1.0F, 1.3F, TAnimation::THitJump }, //TJump
	{	{	2.0F,	1.908F,	1.858F,	2.104F	},	{	0.296F,	0.287F,	0.264F,	0.309F	},		3.0F, 1.1F, TAnimation::THitHigh }, //THigh
	{	{	0.87F,	0.899F,	0.975F,	0.989F	},	{	0.585F,	0.485F,	0.368F,	0.481F	},		5.0F, 1.0F, TAnimation::THitMed }, //TMedium
	{	{	0.576F,	0.542F,	0.659F,	0.652F	},	{	0.585F,	0.571F,	0.512F,	0.596F	},		7.0F, 1.2F, TAnimation::THitLow },  //TLow
};

static const GVector3Float LocalGetOriginalPosition(TPointerSceneNode& inout_owner)
{
	if (inout_owner)
	{
		return inout_owner->GetWorldTransform().GetPosition();
	}
	return GVector3Float::sZero;
}

static const float LocalGetOriginalRotation(TPointerSceneNode& inout_owner)
{
	if (inout_owner)
	{
		const GVector3Float ditection = inout_owner->GetWorldTransform().GetAt();
		return GMath::ClampRadian(GMath::ATan(ditection.GetX(), ditection.GetZ()));// + GMath::GetPI();
	}
	return 0.0F;
}

//todo: player type/ cost of movement/ height
static const float LocalEvalueShot(
	const GVector3Float& in_testLocation, 
	const float in_testTime, 
	const GVector3Float& in_position,
	const float in_bias,
	const float in_maxRunSpeed
	)
{
	const float approxDistance = LengthAprox(in_testLocation - in_position);
	float speed = 0.0F;
	if (in_testTime)
	{
		speed = approxDistance / in_testTime;
	}

	//can return zero if we cant reach the shot...
	if (in_maxRunSpeed < speed)
	{
		return 0.0F;
	}

	//const float value = approxDistance * in_bias; //(100.0F - speed) + (in_bias * 10.0F);
	const float value = (100000.0F - (in_testTime * in_bias));

	return value;
}


static void LocalHitShotBackToPlayer(
	BeachVolleyBallGame& in_parentState,
	const float in_verticalVelocity,
	const float* const in_ballOriginHeight = NULL
	)
{
	//hit shot back to player
	const GVector3Float playerPosition = in_parentState.GetPlayerPosition();
	TPointerBeachVolleyBallClientBall pointerBall = in_parentState.GetBall();
	if (pointerBall)
	{
		BeachVolleyBallClientBall& ball = *pointerBall;
		//todo: shot origin should be from ball bone of opponent?

		GVector3Float ballOrigin = ball.GetPosition();
		if (in_ballOriginHeight)
		{
			ballOrigin.SetY(*in_ballOriginHeight);
		}
		GVector3Float shotRotation;
		GVector3Float shotVelocity;
		float rotationRate = 0.0F;
		float aproxDistance = LengthAprox(playerPosition - ballOrigin);

		const float firstTryTime = ball.CalculateTimeToPoint(
			shotVelocity,
			shotRotation,
			rotationRate,
			ballOrigin,
			playerPosition + GVector3Float(GMath::RandomUnitPlusMinius() * 0.1F, 0.25F, -0.25F),
			in_verticalVelocity * (GMath::Maximum(1.0F, aproxDistance * 0.2F))
			);
		if (!firstTryTime)
		{
			ball.CalculateTimeToPoint(
								shotVelocity,
								shotRotation,
								rotationRate,
								ballOrigin,
								playerPosition,
								in_verticalVelocity
								);
		}


		ball.SetBallInFlight(
			ballOrigin,
			shotVelocity,
			ball.GetAt(),
			ball.GetUp(),
			shotRotation,
			rotationRate
			);
	}
	in_parentState.NotifyOpponentHitBall();
	return;
}

//static public methods
/*static*/ BeachVolleyBallClientOpponent::TPointerSceneNodeComponentBase BeachVolleyBallClientOpponent::FactoryBikini0(
	GScene& inout_scene, //access to mesh/ material manager
	TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
	void* const in_data
	)
{
	return Factory(inout_scene, inout_owner, inout_arrayPostLoadCallback, in_data, BeachVolleyBallType::TGirl::TBikini, BeachVolleyBallType::TMatch::TOne);
}
/*static*/ BeachVolleyBallClientOpponent::TPointerSceneNodeComponentBase BeachVolleyBallClientOpponent::FactoryBikini1(
	GScene& inout_scene, //access to mesh/ material manager
	TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
	void* const in_data
	)
{
	return Factory(inout_scene, inout_owner, inout_arrayPostLoadCallback, in_data, BeachVolleyBallType::TGirl::TBikini, BeachVolleyBallType::TMatch::TTwo);
}
/*static*/ BeachVolleyBallClientOpponent::TPointerSceneNodeComponentBase BeachVolleyBallClientOpponent::FactoryBikini2(
	GScene& inout_scene, //access to mesh/ material manager
	TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
	void* const in_data
	)
{
	return Factory(inout_scene, inout_owner, inout_arrayPostLoadCallback, in_data, BeachVolleyBallType::TGirl::TBikini, BeachVolleyBallType::TMatch::TThree);
}


/*static*/ BeachVolleyBallClientOpponent::TPointerSceneNodeComponentBase BeachVolleyBallClientOpponent::FactoryReiko0(
	GScene& inout_scene, //access to mesh/ material manager
	TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
	void* const in_data
	)
{
	return Factory(inout_scene, inout_owner, inout_arrayPostLoadCallback, in_data, BeachVolleyBallType::TGirl::TReiko, BeachVolleyBallType::TMatch::TOne);
}
/*static*/ BeachVolleyBallClientOpponent::TPointerSceneNodeComponentBase BeachVolleyBallClientOpponent::FactoryReiko1(
	GScene& inout_scene, //access to mesh/ material manager
	TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
	void* const in_data
	)
{
	return Factory(inout_scene, inout_owner, inout_arrayPostLoadCallback, in_data, BeachVolleyBallType::TGirl::TReiko, BeachVolleyBallType::TMatch::TTwo);
}
/*static*/ BeachVolleyBallClientOpponent::TPointerSceneNodeComponentBase BeachVolleyBallClientOpponent::FactoryReiko2(
	GScene& inout_scene, //access to mesh/ material manager
	TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
	void* const in_data
	)
{
	return Factory(inout_scene, inout_owner, inout_arrayPostLoadCallback, in_data, BeachVolleyBallType::TGirl::TReiko, BeachVolleyBallType::TMatch::TThree);
}

/*static*/ BeachVolleyBallClientOpponent::TPointerSceneNodeComponentBase BeachVolleyBallClientOpponent::FactoryApril0(
	GScene& inout_scene, //access to mesh/ material manager
	TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
	void* const in_data
	)
{
	return Factory(inout_scene, inout_owner, inout_arrayPostLoadCallback, in_data, BeachVolleyBallType::TGirl::TApril, BeachVolleyBallType::TMatch::TOne);
}
/*static*/ BeachVolleyBallClientOpponent::TPointerSceneNodeComponentBase BeachVolleyBallClientOpponent::FactoryApril1(
	GScene& inout_scene, //access to mesh/ material manager
	TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
	void* const in_data
	)
{
	return Factory(inout_scene, inout_owner, inout_arrayPostLoadCallback, in_data, BeachVolleyBallType::TGirl::TApril, BeachVolleyBallType::TMatch::TTwo);
}
/*static*/ BeachVolleyBallClientOpponent::TPointerSceneNodeComponentBase BeachVolleyBallClientOpponent::FactoryApril2(
	GScene& inout_scene, //access to mesh/ material manager
	TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
	void* const in_data
	)
{
	return Factory(inout_scene, inout_owner, inout_arrayPostLoadCallback, in_data, BeachVolleyBallType::TGirl::TApril, BeachVolleyBallType::TMatch::TThree);
}

/*static*/ BeachVolleyBallClientOpponent::TPointerSceneNodeComponentBase BeachVolleyBallClientOpponent::FactoryCuda0(
	GScene& inout_scene, //access to mesh/ material manager
	TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
	void* const in_data
	)
{
	return Factory(inout_scene, inout_owner, inout_arrayPostLoadCallback, in_data, BeachVolleyBallType::TGirl::TCuda, BeachVolleyBallType::TMatch::TOne);
}
/*static*/ BeachVolleyBallClientOpponent::TPointerSceneNodeComponentBase BeachVolleyBallClientOpponent::FactoryCuda1(
	GScene& inout_scene, //access to mesh/ material manager
	TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
	void* const in_data
	)
{
	return Factory(inout_scene, inout_owner, inout_arrayPostLoadCallback, in_data, BeachVolleyBallType::TGirl::TCuda, BeachVolleyBallType::TMatch::TTwo);
}
/*static*/ BeachVolleyBallClientOpponent::TPointerSceneNodeComponentBase BeachVolleyBallClientOpponent::FactoryCuda2(
	GScene& inout_scene, //access to mesh/ material manager
	TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
	void* const in_data
	)
{
	return Factory(inout_scene, inout_owner, inout_arrayPostLoadCallback, in_data, BeachVolleyBallType::TGirl::TCuda, BeachVolleyBallType::TMatch::TThree);
}

//static private methods
/*static*/ BeachVolleyBallClientOpponent::TPointerSceneNodeComponentBase BeachVolleyBallClientOpponent::Factory(
	GScene& inout_scene, //access to mesh/ material manager
	TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
	void* const in_data,
	const BeachVolleyBallType::TGirl::TEnum in_girl,
	const BeachVolleyBallType::TMatch::TEnum in_match
	)
{
	TPointerSceneNodeComponentBase pointer;
	if (!in_data)
	{
		return pointer;
	}

	BeachVolleyBallGame& beachVolleyBallGame = *((BeachVolleyBallGame*)in_data);

	TPointerBeachVolleyBallClientOpponent pointerOpponent;
	pointerOpponent.reset(new BeachVolleyBallClientOpponent(
		beachVolleyBallGame, 
		inout_owner,
		in_girl,
		in_match
		));
	beachVolleyBallGame.SetClientOpponent(pointerOpponent);

	pointer = pointerOpponent;

	inout_arrayPostLoadCallback.push_back(GScenePostLoadCallback(
		inout_owner,
		pointer,
		BeachVolleyBallClientOpponent::PostLoadCallback
		));

	return pointer;
}

/*static*/ void BeachVolleyBallClientOpponent::PostLoadCallback(
	GScene& inout_scene, //access to mesh/ material manager
	TPointerSceneNode& inout_owner,
	TPointerSceneNodeComponentBase& inout_component,
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback
	)
{
	TPointerBeachVolleyBallClientOpponent pointer = boost::dynamic_pointer_cast<BeachVolleyBallClientOpponent>(inout_component);
	if (!pointer || !inout_owner)
	{
		return;
	}

	GSceneNode& sceneNode = *inout_owner;
	//find the animation
	const int componentCount = sceneNode.GetComponentCount();
	for (int index = 0; index < componentCount; ++index)
	{
		TPointerSceneNodeComponentAnimation pointerAnimation = sceneNode.GetComponentPointer<GSceneNodeComponentAnimation>(index);
		if (pointerAnimation)
		{
			pointer->mComponentAnimation = pointerAnimation;
			continue;
		}
	}

	inout_arrayPostLoadCallback.push_back(GScenePostLoadCallback(
		inout_owner,
		inout_component,
		BeachVolleyBallClientOpponent::PostLoadCallbackB
		));

	return;
}

/*static*/ void BeachVolleyBallClientOpponent::PostLoadCallbackB(
	GScene& inout_scene, //access to mesh/ material manager
	TPointerSceneNode& inout_owner,
	TPointerSceneNodeComponentBase& inout_component,
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback
	)
{
	TPointerBeachVolleyBallClientOpponent pointer = boost::dynamic_pointer_cast<BeachVolleyBallClientOpponent>(inout_component);
	if (!pointer || !inout_owner)
	{
		return;
	}

	BeachVolleyBallClientOpponent& clientOpponent = *pointer;
	TPointerSceneNodeComponentAnimation pointerComponentAnimation = clientOpponent.mComponentAnimation.lock();
	if (!pointerComponentAnimation)
	{
		return;
	}

	GSceneNodeComponentAnimation& componentAnimation = *pointerComponentAnimation;

	for (int index = 0; index < componentAnimation.GetSkeletonCount(); ++index)
	{
		TPointerSkeletonInstance& pointerSkeleton = componentAnimation.GetSkeleton(index);
		if (!pointerSkeleton)
		{
			continue;
		}
		GSkeletonInstance& skeleton = *pointerSkeleton;

		const int morphIndex = skeleton.GetMorphTargetIndex("weird");
		//const int morphIndex = skeleton.GetMorphTargetIndex("leftfist");
		if (GCOMMON_INVALID_INDEX != morphIndex)
		{
			clientOpponent.mMorphTargetId = morphIndex;
		}

		const int boneIndex = skeleton.GetBoneIndex("BoneBall");
		if (GCOMMON_INVALID_INDEX != boneIndex)
		{
			clientOpponent.mBallBoneId = boneIndex;
			clientOpponent.mSkeleton = pointerSkeleton;
		}

		const int boneHeadIndex = skeleton.GetBoneIndex("Bip01 Head");
		if (GCOMMON_INVALID_INDEX != boneHeadIndex)
		{
			componentAnimation.AddBoneCallback(
				boneHeadIndex,
				pointerSkeleton,
				BoneUpdateCallback,
				pointer.get()
				);
		}

		const int boneNeckIndex = skeleton.GetBoneIndex("Bip01 Neck");
		if (GCOMMON_INVALID_INDEX != boneNeckIndex)
		{
			clientOpponent.mNeckBoneId = boneNeckIndex;
		}
	}

	return;
}

/*static*/ void BeachVolleyBallClientOpponent::BoneUpdateCallback(
	GVector3Float& inout_at,
	GVector3Float& inout_up,
	GVector3Float& inout_pos,
	float& inout_scale,
	GSceneNode& in_parentSceneNode,
	const float in_timeDelta,
	void* in_callbackData,
	GSkeletonInstance& inout_skeletonInstance
	)
{
	if (!in_callbackData)
	{
		return;
	}
	BeachVolleyBallClientOpponent& self = *(BeachVolleyBallClientOpponent*)in_callbackData;

	if ((0.0F == self.mLookAtBallWeight) &&
		(0.0F == self.mLookAtPlayerWeight))
	{
		return;
	}

	TPointerBeachVolleyBallClientBall pointerBall = self.mParentState.GetBall();
	if (!pointerBall)
	{
		return;
	}

	const GMatrix16Float parentWorldTransform = inout_skeletonInstance.GetBoneObjectMatrix(self.mNeckBoneId) * in_parentSceneNode.GetWorldTransform();
	const GMatrix16Float invertedParentWorldTransform = parentWorldTransform.ReturnInverse();

	//put ball and player into parent relative space, negate as inout_at is towards the back, not the front of the head
	const GVector3Float& ballPosition = -(pointerBall->GetPosition() * invertedParentWorldTransform);
	const GVector3Float playerPosition = -((self.mParentState.GetPlayerPosition() + GVector3Float(0.0F, 1.75F, 0.0F)) * invertedParentWorldTransform);
	GVector3Float atBall = NormaliseAprox(ballPosition);
	GVector3Float atPlayer = NormaliseAprox(playerPosition);
//#ifdef DSC_DEBUG
//	{
//		GScene& scene = DEBUG_GET_SCENE()
//		const GVector3Float headPos = inout_pos * parentWorldTransform;
//		GMatrix16Float transform = GMatrix16FloatConstructAtUp(
//			inout_at,
//			inout_up,
//			GVector3Float::sUnitY,
//			GVector3Float::sUnitZ,
//			headPos
//			);
//		scene.GetArrayDebugMatrix().push_back(transform);
//
//		transform.SetPosition(headPos + (GMatrix16FloatMultiplyNoTranslate(inout_at, parentWorldTransform)));
//		scene.GetArrayDebugMatrix().push_back(transform);
//
//		transform.SetPosition(headPos + (GMatrix16FloatMultiplyNoTranslate(atBall, parentWorldTransform)));
//		scene.GetArrayDebugMatrix().push_back(transform);
//	}
//#endif

	//limit the at vector
	{
		const float dotAt = DotProduct(inout_at, atBall);
		if (dotAt < 0.4F)
		{
			atBall += (inout_at * (0.4F - dotAt));
		}
	}
	{
		const float dotAt = DotProduct(inout_at, atPlayer);
		if (dotAt < 0.4F)
		{
			atPlayer += (inout_at * (0.4F - dotAt));
		}
	}

	//apply look at
	inout_at = Lerp(GMath::Clamp(self.mLookAtPlayerWeight, 0.0F, 1.0F), inout_at, atPlayer);
	inout_at = Lerp(GMath::Clamp(self.mLookAtBallWeight, 0.0F, 1.0F), inout_at, atBall);
	//inout_at = NormaliseAprox(


	return;
}

//constructor
BeachVolleyBallClientOpponent::BeachVolleyBallClientOpponent(
	BeachVolleyBallGame& inout_parentGame,
	TPointerSceneNode& inout_owner,
	const BeachVolleyBallType::TGirl::TEnum in_girl,
	const BeachVolleyBallType::TMatch::TEnum in_match
	)
	: mParentState(inout_parentGame)
	, mOriginalPosition(LocalGetOriginalPosition(inout_owner))
	, mOriginalRotation(LocalGetOriginalRotation(inout_owner))
	, mGirl(in_girl)
	, mMatch(in_match)
	, mState(0)
	, mLerpPosition()
	, mLerpRotation()
	, mStateCountdown(0.0F)
	, mShotType(0)
	, mSetTeleport(true)
	, mSetTeleportTilt(false)
	//, mPlayFallAnimation(false)
	, mComponentAnimation()
	, mAnimation(TAnimation::TIdle)
	, mBallBoneId(GCOMMON_INVALID_INDEX)
	, mMorphTargetId(GCOMMON_INVALID_INDEX)
	, mLookAtPlayer(false)
	, mLookAtPlayerWeight(0.0F)
	, mLookAtBall(false)
	, mLookAtBallWeight(0.0F)
	, mNeckBoneId(GCOMMON_INVALID_INDEX)
	, mMoveRotate(false)
	, mMoveRotateTime(0.0F)
	, mMoveRotateCountdown(0.0F)
{
	mLerpPosition.reset(new GComponentLerpVector3Float(mOriginalPosition));
	mLerpRotation.reset(new GComponentLerpFloat(mOriginalRotation));
	mComponentTilt.reset(new GComponentTilt(
		10.0F,
		7.5F,
		0.25F,
		-GVector3Float::sUnitY,
		mOriginalPosition
		));

	return;
}

BeachVolleyBallClientOpponent::~BeachVolleyBallClientOpponent()
{
	return;
}

//implement GSceneNodeComponentClient
/*virtual*/ void BeachVolleyBallClientOpponent::OnTick(TPointerSceneNode& inout_sceneNode, const float in_timeDelta)
{
	//update opponent transform
	mLerpPosition->Tick(in_timeDelta);
	mLerpRotation->Tick(in_timeDelta);
	GMatrix16Float transform = GMatrix16FloatConstructAxisAngle(GVector3Float::sUnitY, mLerpRotation->GetValue());

	if (mComponentTilt)
	{
		mComponentTilt->Update(
			in_timeDelta,
			mLerpPosition->GetValue(),
			mSetTeleport | mSetTeleportTilt
			);
		GMatrix16Float tiltMatrix;
		mComponentTilt->GetTiltMatrix(
			tiltMatrix
			);
		transform = tiltMatrix * transform;
	}
	transform.SetPosition(mLerpPosition->GetValue());
	mSetTeleportTilt = false;

	//update rotate
	if (mMoveRotate)
	{
		mMoveRotateCountdown -= in_timeDelta;
		if (mMoveRotateCountdown <= 0.0F)
		{
			mMoveRotate = false;
			mLerpRotation->SetValue(mOriginalRotation, mMoveRotateTime);
		}
	}

	//update lookat weight
	if (mLookAtBall)
	{
		mLookAtBallWeight += 2.0F * in_timeDelta;
		if (1.5F < mLookAtBallWeight)
		{
			mLookAtBall = false; 
		}
	}
	else if (0.0F != mLookAtBallWeight)
	{
		mLookAtBallWeight -= 1.5F * in_timeDelta;
		if (mLookAtBallWeight < 0.0F)
		{
			mLookAtBallWeight = 0.0F;
		}
	}

	if (mLookAtPlayer)
	{
		mLookAtPlayerWeight += 1.0F * in_timeDelta;
		if (1.0F < mLookAtPlayerWeight)
		{
			mLookAtPlayer = false; 
		}
	}
	else if (0.0F != mLookAtPlayerWeight)
	{
		mLookAtPlayerWeight -= 0.75F * in_timeDelta;
		if (mLookAtPlayerWeight < 0.0F)
		{
			mLookAtPlayerWeight = 0.0F;
		}
	}

	bool returnToOrigin = false;

	//update state
	switch (mState)
	{
	default:
		break;
	case TState::TPlayAnimation:
		mStateCountdown -= in_timeDelta;
		if (mStateCountdown < 0.0F)
		{
			returnToOrigin = true;
		}
		break;
	case TState::TServe:
		mStateCountdown -= in_timeDelta;

		//update ball position
		{
			TPointerBeachVolleyBallClientBall pointerBall = mParentState.GetBall();
			TPointerSkeletonInstance pointerSkeleton = mSkeleton.lock();
			if (pointerBall &&
				pointerSkeleton &&
				(GCOMMON_INVALID_INDEX != mBallBoneId))
			{
				GMatrix16Float ballTransform = pointerSkeleton->GetParentRelativeBoneMatrix(mBallBoneId) * transform;
				BeachVolleyBallClientBall& ball = *pointerBall;
				ball.SetBallAttachedToPlayer(
					ballTransform.GetPosition(),
					ballTransform.GetAt(),
					ballTransform.GetUp()
					);
			}
		}

		if (mStateCountdown <= 0.0F)
		{
			mStateCountdown = (((sArrayAnimationData[TAnimation::TServe].mFrameCount - sArrayAnimationData[TAnimation::TServe].mEventFrame) / sArrayAnimationData[TAnimation::TServe].mRate) / 30.0F) - 0.2F;
			mState = TState::TAfterServe;

			LocalHitShotBackToPlayer(
				mParentState,
				5.0F * sArrayGirlData[mGirl].mBallVelocityFactor
				);

			mLookAtPlayer = true;
		}
		break;
	case TState::TAfterServe:
		mStateCountdown -= in_timeDelta;
		if (mStateCountdown <= 0.0F)
		{
			mState = TState::TIdle;
			PlayAnimation(TAnimation::TIdle, sDefaultAnimationFadeTime);
		}
		break;
	case TState::TMoveToTakeShot:
		mStateCountdown -= in_timeDelta;
		if (mStateCountdown <= 0.0F)
		{
			mState = TState::TTakeShot;
			const int animationId = sArrayShotData[mShotType].mAnimationId;
			mStateCountdown = (sArrayAnimationData[animationId].mEventFrame / sArrayAnimationData[animationId].mRate) / 30.0F;
			PlayAnimation(animationId, sDefaultAnimationFadeTime);
		}
		break;
	case TState::TMoveToFall:
	case TState::TMoveToFallBack:
		mStateCountdown -= in_timeDelta;
		if (mStateCountdown <= 0.0F)
		{
			if (mState == TState::TMoveToFall)
			{
				PlayAnimation(TAnimation::TFall, sDefaultAnimationFadeTime);
				mStateCountdown = ((sArrayAnimationData[TAnimation::TFall].mFrameCount / sArrayAnimationData[TAnimation::TFall].mRate) - 5.0F) / 30.0F;
			}
			else
			{
				PlayAnimation(TAnimation::TFallBack, sDefaultAnimationFadeTime);
				mStateCountdown = ((sArrayAnimationData[TAnimation::TFallBack].mFrameCount / sArrayAnimationData[TAnimation::TFallBack].mRate) - 5.0F) / 30.0F;
			}

			mState = TState::TFall;
		}
		break;
	case TState::TTakeShot:
		mStateCountdown -= in_timeDelta;
		if (mStateCountdown <= 0.0F)
		{
			mState = TState::TAfterShot;

			const int animationId = sArrayShotData[mShotType].mAnimationId;
			mStateCountdown = (((sArrayAnimationData[animationId].mFrameCount - sArrayAnimationData[animationId].mEventFrame) / sArrayAnimationData[animationId].mRate) / 30.0F) - 0.2F;

			LocalHitShotBackToPlayer(
				mParentState,
				sArrayShotData[mShotType].mVelocity * sArrayGirlData[mGirl].mBallVelocityFactor,
				&sArrayShotData[mShotType].mHeight[mGirl]
				);

			mLookAtPlayer = true;
			mLookAtBall = false;
		}
		break;
	case TState::TAfterShot:
		mStateCountdown -= in_timeDelta;
		if (mStateCountdown <= 0.0F)
		{
			returnToOrigin = true;
		}
		break;
	case TState::TReturnToIdle:
		mStateCountdown -= in_timeDelta;
		if (mStateCountdown <= 0.0F)
		{
			mState = TState::TIdle;
			PlayAnimation(TAnimation::TIdle, sDefaultAnimationFadeTime);
		}
		break;
	case TState::TFall:
		mStateCountdown -= in_timeDelta;
		if (mStateCountdown <= 0.0F)
		{
			returnToOrigin = true;
		}
		break;
	}

	if (returnToOrigin)
	{
		mState = TState::TReturnToIdle;
		const GVector3Float currentPosition = mLerpPosition->GetValue();
		const float distanceToTravel = LengthAprox(currentPosition - mOriginalPosition);

		if (0.1F < distanceToTravel)
		{
			float time = distanceToTravel / sArrayGirlData[mGirl].mMaxRunSpeed;
			if (time < 0.5F)
			{
				time *= 2.0F;
			}
			mLerpPosition->SetValue(mOriginalPosition, time, GComponentLerp::TType::TEaseInOut);

			SelectMoveAnimation(
				mOriginalPosition,
				currentPosition,
				GVector3Float(0.0F, 0.0F, 1.0F),
				time
				);

			mStateCountdown = time;
		}
		else
		{
			mStateCountdown = 0.0F;
		}
	}

	inout_sceneNode->SetParentRelativeTransform(transform);
	inout_sceneNode->SetTeleportFlag(mSetTeleport);
	mSetTeleport = false;

	return;
}

//public methods
void BeachVolleyBallClientOpponent::NotifyBallTouchGround()
{
	mLookAtBall = true;
	return;
}

void BeachVolleyBallClientOpponent::NotifyPlayerServe()
{
	if (mLerpPosition->GetValue() != mOriginalPosition)
	{
		mSetTeleportTilt = true;
	}

	//reset the player
	mLerpPosition->SetValue(mOriginalPosition);
	mLerpRotation->SetValue(mOriginalRotation, sDefaultAnimationFadeTime);
	mState = TState::TIdle;
	PlayAnimation(TAnimation::TIdle, sDefaultAnimationFadeTime);

	mLookAtPlayer = true;
	mMoveRotate = false;

	return;
}

void BeachVolleyBallClientOpponent::NotifyOpponentServe()
{
	if (mLerpPosition->GetValue() != mOriginalPosition)
	{
		mSetTeleportTilt = true;
	}

	mLerpPosition->SetValue(mOriginalPosition);
	mLerpRotation->SetValue(mOriginalRotation, sDefaultAnimationFadeTime);
	mState = TState::TServe;
	PlayAnimation(TAnimation::TServe, sDefaultAnimationFadeTime);
	mStateCountdown = (sArrayAnimationData[TAnimation::TServe].mEventFrame / sArrayAnimationData[TAnimation::TServe].mRate) / 30.0F;

	mLookAtPlayer = true;
	mMoveRotate = false;

	return;
}

void BeachVolleyBallClientOpponent::NotifyPlayerHitBall(BeachVolleyBallClientBall& in_ball)
{
	const GVector3Float position = mLerpPosition->GetValue();
	const GVector3Float gameDirection(0.0F, 0.0F, 1.0F);

	mLookAtBall = true;

	// i want to know where the ball will be at comfortable hit height, at spike height or at low shot height
	float bestValue = 0.0F;
	int shotType = TShot::TCount;
	float shotTime = 0.0F;
	GVector3Float shotLocation;

	for (int index = 0; index < TShot::TCount; ++index)
	{
		GVector3Float testLocationA;
		float testTimeA;
		GVector3Float testLocationB;
		float testTimeB;

		if (!in_ball.GetPredictedPosition(testLocationA, testTimeA, testLocationB, testTimeB, sArrayShotData[index].mHeight[mGirl]))
		{
			continue;
		}

		const int animationId = sArrayShotData[index].mAnimationId;
		const float shotSetupTime = ((sArrayAnimationData[animationId].mEventFrame / sArrayAnimationData[animationId].mRate) / 30.0F);

		if (shotSetupTime <= testTimeA)
		{
			const float testValue = LocalEvalueShot(
				testLocationA, 
				testTimeA, 
				position,
				sArrayShotData[index].mBias,
				sArrayGirlData[mGirl].mMaxRunSpeed
				);
			if (bestValue < testValue)
			{
				bestValue = testValue;
				shotType = (TShot::TEnum)index;
				shotTime = testTimeA;
				shotLocation = testLocationA;
			}
		}

		if (shotSetupTime <= testTimeB)
		{
			const float testValue = LocalEvalueShot(
				testLocationB, 
				testTimeB, 
				position,
				sArrayShotData[index].mBias, 
				sArrayGirlData[mGirl].mMaxRunSpeed
				);
			if (bestValue < testValue)
			{
				bestValue = testValue;
				shotType = (TShot::TEnum)index;
				shotTime = testTimeB;
				shotLocation = testLocationB;
			}
		}
	}

	bool girlFall = false;
	if (TShot::TCount != shotType)
	{
		shotLocation -= gameDirection * sArrayShotData[shotType].mDepth[mGirl];
		mLerpPosition->SetValue(shotLocation, shotTime, GComponentLerp::TType::TEaseInOut);
		const int animationId = sArrayShotData[shotType].mAnimationId;
		mStateCountdown = shotTime - ((sArrayAnimationData[animationId].mEventFrame / sArrayAnimationData[animationId].mRate) / 30.0F);

		if (mStateCountdown < 0.0F)
		{
			girlFall = true;
		}
		else
		{
			mShotType = shotType;
			mState = TState::TMoveToTakeShot;

			SelectMoveAnimation(
				shotLocation,
				position,
				gameDirection,
				shotTime
				);
		}
	}
	else
	{
		girlFall = true;
	}

	if (girlFall && (GMath::RandomUnit() < sArrayGirlData[mGirl].mFallChance))
	{
		float timeA = 0.0F;
		float timeB = 0.0F;
		float timeBest = 0.0F;
		GVector3Float locationA;
		GVector3Float locationB;
		GVector3Float locationBest;
		//stand there and watch if illegal shot, or run and try to make shot but miss
		if (in_ball.GetPredictedPosition(locationA, timeA, locationB, timeB, 0.0F))
		{
			if ((((0.0F < timeA) &&
				(0.0F < timeB)) &&
				(timeA < timeB)) ||
				(0.0F < timeB))
			{
				timeBest = timeB;
				locationBest = locationB;
			}
			else
			{
				timeBest = timeA;
				locationBest = locationA;
			}
		}

		if (0.0F < timeBest)
		{
			float distance = 0.0F;
			const GVector3Float towardsBall = (NormaliseAprox(locationBest - position, distance));
			const bool forward = (0.0F < DotProduct(towardsBall, gameDirection));

			mStateCountdown = GMath::Clamp(timeBest - sDefaultAnimationFadeTime, 0.25F, 1.0F);

			const GVector3Float targetLocation = position + (towardsBall * (sArrayGirlData[mGirl].mMaxRunSpeed * mStateCountdown));
			mLerpPosition->SetValue(
				targetLocation,
				mStateCountdown + sDefaultAnimationFadeTime
				);

			if (forward)
			{
				mState = TState::TMoveToFall;
			}
			else
			{
				mState = TState::TMoveToFallBack;
			}

			SelectMoveAnimation(
				targetLocation,
				position,
				gameDirection,
				mStateCountdown
				);
		}
	}

	return;
}

void BeachVolleyBallClientOpponent::NotifyOpponentWinPoint()
{
	mState = TState::TPlayAnimation;
	PlayAnimation(TAnimation::TWinPoint, sDefaultAnimationFadeTime);
	mStateCountdown = ((sArrayAnimationData[TAnimation::TWinPoint].mFrameCount / sArrayAnimationData[TAnimation::TWinPoint].mRate) / 30.0F) - 0.2F;
	mLerpPosition->SetValue(mLerpPosition->GetValue());
	return;
}

void BeachVolleyBallClientOpponent::NotifyOpponentWinServe()
{
	mState = TState::TPlayAnimation;
	PlayAnimation(TAnimation::TWinServe, sDefaultAnimationFadeTime);
	mStateCountdown = ((sArrayAnimationData[TAnimation::TWinServe].mFrameCount / sArrayAnimationData[TAnimation::TWinServe].mRate) / 30.0F) - 0.2F;
	mLerpPosition->SetValue(mLerpPosition->GetValue());
	return;
}

void BeachVolleyBallClientOpponent::NotifyOpponentWinGame()
{
	mState = TState::TGameOver;
	PlayAnimation(TAnimation::TWinGame, sDefaultAnimationFadeTime);
	mLerpPosition->SetValue(mOriginalPosition);
	mLerpRotation->SetValue(mOriginalRotation);
	mSetTeleport = true;
	return;
}

void BeachVolleyBallClientOpponent::NotifyOpponentLostPoint()
{
	if (mState == TState::TFall)
	{
		return;
	}
	mState = TState::TPlayAnimation;
	PlayAnimation(TAnimation::TLoosePoint, sDefaultAnimationFadeTime);
	mStateCountdown = ((sArrayAnimationData[TAnimation::TLoosePoint].mFrameCount / sArrayAnimationData[TAnimation::TLoosePoint].mRate) / 30.0F) - 0.2F;
	mLerpPosition->SetValue(mLerpPosition->GetValue());
	return;
}

void BeachVolleyBallClientOpponent::NotifyOpponentLostServe()
{
	if (mState == TState::TFall)
	{
		return;
	}

	mState = TState::TPlayAnimation;
	PlayAnimation(TAnimation::TLooseServe, sDefaultAnimationFadeTime);
	mStateCountdown = ((sArrayAnimationData[TAnimation::TLooseServe].mFrameCount / sArrayAnimationData[TAnimation::TLooseServe].mRate) / 30.0F) - 0.2F;
	mLerpPosition->SetValue(mLerpPosition->GetValue());
	return;
}

void BeachVolleyBallClientOpponent::NotifyOpponentLostGame()
{
	mState = TState::TGameOver;
	PlayAnimation(TAnimation::TLooseGame, sDefaultAnimationFadeTime);
	mLerpPosition->SetValue(mOriginalPosition);
	mLerpRotation->SetValue(mOriginalRotation);
	mSetTeleport = true;
	return;
}

void BeachVolleyBallClientOpponent::DoMorphTest(const float in_amount)
{
	TPointerSkeletonInstance pointerSkeleton = mSkeleton.lock();
	if (pointerSkeleton &&
		(GCOMMON_INVALID_INDEX != mMorphTargetId))
	{
		pointerSkeleton->SetMorphTargetWeight(mMorphTargetId, in_amount);
	}
	return;
}

//private methods
void BeachVolleyBallClientOpponent::PlayAnimation(
	const int in_animationId,
	const float in_fadeInTime,
	const float in_rateMul,
	const float in_force
	)
{
	if ((mAnimation == in_animationId) &&
		(!in_force))
	{
		//bail, already playing
		return;
	}

	mAnimation = in_animationId;

	TPointerSceneNodeComponentAnimation componentAnimation = mComponentAnimation.lock();
	if (!componentAnimation)
	{
		return;
	}

	TPointerAnimationManagerInstance& pointerAnimationManagerInstance = componentAnimation->GetAnimationManagerInstance();
	if (!pointerAnimationManagerInstance)
	{
		return;
	}

	GAnimationManagerInstance& animationManagerInstance = *pointerAnimationManagerInstance;
	GAnimationPlayParam playParam(
		sArrayAnimationData[mAnimation].mRate * in_rateMul,
		in_fadeInTime
		);
	animationManagerInstance.StopAnimation(in_fadeInTime);
	animationManagerInstance.PlayAnimation(sArrayAnimationData[mAnimation].mName, playParam);


	//play sound
	int soundId = GCOMMON_INVALID_INDEX;
	bool soundForce = false;
	switch (mAnimation)
	{
	default:
		break;
	case TAnimation::TFall:
		soundId = BeachVolleyBallSound::GetSoundIDFall();
		soundForce = true;
		break;
	case TAnimation::TFallBack:
		soundId = BeachVolleyBallSound::GetSoundIDFallBack();
		soundForce = true;
		break;
	case TAnimation::THitHigh:
		soundId = BeachVolleyBallSound::GetSoundIDHitHigh();
		break;
	case TAnimation::THitJump:
		soundId = BeachVolleyBallSound::GetSoundIDHitJump();
		break;
	case TAnimation::THitLow:
		soundId = BeachVolleyBallSound::GetSoundIDHitLow();
		break;
	case TAnimation::THitMed:
		soundId = BeachVolleyBallSound::GetSoundIDHitMed();
		break;
	case TAnimation::TLooseGame:
		soundId = BeachVolleyBallSound::GetSoundIDLooseGame();
		soundForce = true;
		break;
	case TAnimation::TLoosePoint:
		soundId = BeachVolleyBallSound::GetSoundIDLoosePoint();
		break;
	case TAnimation::TLooseServe:
		soundId = BeachVolleyBallSound::GetSoundIDLooseServe();
		break;
	case TAnimation::TServe:
		soundId = BeachVolleyBallSound::GetSoundIDServe();
		break;
	case TAnimation::TWinGame:
		soundId = BeachVolleyBallSound::GetSoundIDWinGame();
		soundForce = true;
		break;
	case TAnimation::TWinPoint:
		soundId = BeachVolleyBallSound::GetSoundIDWinPoint();
		break;
	case TAnimation::TWinServe:
		soundId = BeachVolleyBallSound::GetSoundIDWinServe();
		break;
	}

	if ((GCOMMON_INVALID_INDEX != soundId) &&
		(mParentState.GetSoundManager()))
	{
		BeachVolleyBallSound& soundManager = *mParentState.GetSoundManager();
		soundManager.PlayGirlSound(
			soundId,
			mLerpPosition->GetValue(),
			soundForce
			);
	}

	return;
}

void BeachVolleyBallClientOpponent::SelectMoveAnimation(
	const GVector3Float& in_targetPosition,
	const GVector3Float& in_position,
	const GVector3Float& in_gameDirection,
	const float in_time
	)
{
	//either walk, back or run
	const bool forwards = (0.0F < DotProduct(in_gameDirection, in_targetPosition - in_position));
	float distApprox = 0.0F;
	const GVector3Float directionApprox = Normalise(in_targetPosition - in_position, distApprox);
	bool move = false;

	if (forwards)
	{
		if (0.0F != in_time)
		{
			const float speed = distApprox / in_time;
			if (speed < 2.0F)
			{
				const float rate = GMath::Clamp(speed, 0.75F, 1.5F);
				PlayAnimation(TAnimation::TWalk, sDefaultAnimationFadeTime, rate);
				move = true;
			}
			else
			{
				const float rate = GMath::Clamp(speed / 3.0F, 0.75F, 1.5F);
				PlayAnimation(TAnimation::TRun, sDefaultAnimationFadeTime, rate);
				move = true;
			}
		}
		else
		{
			PlayAnimation(TAnimation::TIdle, sDefaultAnimationFadeTime);
		}
	}
	else if (in_time)
	{
		const float rate = GMath::Clamp(distApprox / in_time, 0.75F, 1.5F);
		PlayAnimation(TAnimation::TBack, sDefaultAnimationFadeTime, rate);
		move = true;
	}
	
	if (move)
	{
		const float currentRotation = mLerpRotation->GetValue();
		float facingDelta = 0.0F;
		
		if (forwards)
		{
			facingDelta = GMath::ClampRadian(GMath::ATan(directionApprox.GetX(), directionApprox.GetZ()) - currentRotation);
		}
		else
		{
			facingDelta = GMath::ClampRadian(GMath::ATan(-directionApprox.GetX(), -directionApprox.GetZ()) - currentRotation);
		}

		mMoveRotate = true;
		mMoveRotateTime = in_time * 0.5F;
		mMoveRotateCountdown = mMoveRotateTime;
		mLerpRotation->SetValue(facingDelta + currentRotation, mMoveRotateTime);
	}

	return;
}