//
//  BeachVolleyBallClientBall.cpp
//  BeachVolleyBall
//
//  Created by David Coen on 2011 04 04
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "BeachVolleyBallClientBall.h"
#include "BeachVolleyBallGame.h"
#include "BeachVolleyBallSound.h"

#include <GMath.h>
#include <GMatrix16Float.h>
#include <GMatrix9Float.h>
#include <GSceneNode.h>

typedef boost::shared_ptr<BeachVolleyBallClientBall> TPointerBeachVolleyBallClientBall;

static const float sBallRadius = 0.113F;
static const float sGravity = -9.8F;

struct TState
{
	enum TEnum
	{
		TPlayerServe = 0,
		TAttachedToOpponent,
		TInPlay,
		TRolling,
		TCount
	};
};

//static public methods
/*static*/ BeachVolleyBallClientBall::TPointerSceneNodeComponentBase BeachVolleyBallClientBall::Factory(
	GScene& inout_scene, //access to mesh/ material manager
	TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
	void* const in_data
	)
{
	TPointerSceneNodeComponentBase pointer;
	if (!in_data)
	{
		return pointer;
	}

	BeachVolleyBallGame& beachVolleyBallGame = *((BeachVolleyBallGame*)in_data);

	TPointerBeachVolleyBallClientBall pointerBall;
	pointerBall.reset(new BeachVolleyBallClientBall(beachVolleyBallGame));
	beachVolleyBallGame.SetClientBall(pointerBall);

	pointer = pointerBall;

	return pointer;
}

//constructor
BeachVolleyBallClientBall::BeachVolleyBallClientBall(
	BeachVolleyBallGame& inout_parentState	
	)
: mParentState(inout_parentState)
, mState(TState::TCount)
, mPosition()
, mVelocity()
, mAt()
, mUp()
, mAxisRotation()
, mRadianRateRotation(0.0F)
{
	return;
}

BeachVolleyBallClientBall::~BeachVolleyBallClientBall()
{
	return;
}

//implement GSceneNodeComponentClient
/*virtual*/ void BeachVolleyBallClientBall::OnTick(TPointerSceneNode& inout_sceneNode, const float in_timeDelta)
{
	bool updatePosition = false;
	switch (mState)
	{
	default:
		break;
	case TState::TAttachedToOpponent:
		updatePosition = true;
		break;
	case TState::TPlayerServe:
		updatePosition = true;
		{
			const GMatrix9Float rotateMatrix = GMatrix9FloatConstructAxisAngle(mAxisRotation, mRadianRateRotation);
			mAt = mAt * rotateMatrix;
			mUp = mUp * rotateMatrix;
		}
		break;
	case TState::TInPlay:
		SimulateBallPosition(in_timeDelta);
		updatePosition = true;
		break;
	case TState::TRolling:
		updatePosition = true;
		SimulateBallRoll(in_timeDelta);
		break;
	}

	if (updatePosition)
	{
		inout_sceneNode->SetParentRelativeTransform(GMatrix16FloatConstructAtUp(
			mAt,
			mUp,
			GVector3Float::sUnitY,
			GVector3Float::sUnitZ,
			mPosition
			));
	}

	return;
}

//public methods
void BeachVolleyBallClientBall::SetPlayerServe(
	const GVector3Float& in_position, 
	const GVector3Float& in_at,
	const GVector3Float& in_up
	)
{
	mState = TState::TPlayerServe;
	mPosition = in_position;
	mAt = in_at;
	mUp = in_up;
	mAxisRotation = GVector3Float::sUnitY;
	mRadianRateRotation = 0.05F;
	return;
}

void BeachVolleyBallClientBall::SetBallInFlight(
	const GVector3Float& in_position, 
	const GVector3Float& in_velocity, 
	const GVector3Float& in_at,
	const GVector3Float& in_up,
	const GVector3Float& in_axisRotation,
	const float in_radianRateRotation
	)
{
	mState = TState::TInPlay;
	mBounceCount = 0;
	mPosition = in_position;
	mVelocity = in_velocity;
	mAt = in_at;
	mUp = in_up;
	mAxisRotation = in_axisRotation;
	mRadianRateRotation = in_radianRateRotation;

	//sound
	int soundId = 0;
	if (4.0F < LengthAprox(in_velocity))
	{
		soundId = BeachVolleyBallSound::GetSoundIDHitHard();
	}
	else
	{
		soundId = BeachVolleyBallSound::GetSoundIDHit();
	}

	mParentState.GetSoundManager()->PlayPositionSound(
		soundId,
		mPosition
		);

	return;
}

void BeachVolleyBallClientBall::SetBallAttachedToPlayer(
	const GVector3Float& in_position, 
	const GVector3Float& in_at,
	const GVector3Float& in_up
	)
{
	mState = TState::TAttachedToOpponent;
	mPosition = in_position;
	mAt = in_at;
	mUp = in_up;
	return;
}

const bool BeachVolleyBallClientBall::GetPredictedPosition(
	GVector3Float& out_locationA,
	float& out_timeA,
	GVector3Float& out_locationB,
	float& out_timeB,
	const float in_height
	)
{
	out_timeA = 0.0F;
	out_timeB = 0.0F;

	//const float height = mPosition.GetY();
	//const float velocity = mVelocity.GetY();
	float rootA = 0.0F;
	float rootB = 0.0F;
	bool valid = false;
	if (GMath::QuadraticRoots(
		rootA,
		rootB,
		0.5F * sGravity,
		mVelocity.GetY(),
		mPosition.GetY() - in_height
		))
	{
		//const float maxRoot = GMath::Maximum(rootA, rootB);
		if (0.0F < rootA)
		{
			valid = true;
			out_timeA = rootA;
			out_locationA = GVector3Float(
				mPosition.GetX() + (mVelocity.GetX() * rootA),
				0.0F,
				mPosition.GetZ() + (mVelocity.GetZ() * rootA)
				);
		}

		if (0.0F < rootB)
		{
			valid = true;
			out_timeB = rootB;
			out_locationB = GVector3Float(
				mPosition.GetX() + (mVelocity.GetX() * rootB),
				0.0F,
				mPosition.GetZ() + (mVelocity.GetZ() * rootB)
				);
		}
	}

	return valid;
}

const float BeachVolleyBallClientBall::CalculateTimeToPoint(
	GVector3Float& out_shotVelocity,
	GVector3Float& out_shotRotation,
	float& out_rotationRate,
	const GVector3Float& in_shotOrigin,
	const GVector3Float& in_shotTarget,
	const float in_verticalVelocity
	)
{
	float distance = 0.0F;
	GVector3Float offset = (in_shotTarget - in_shotOrigin);
	offset.SetY(0.0F);
	const GVector3Float normalToPlayer = Normalise(offset, distance);

	out_shotRotation = CrossProduct(GVector3Float::sUnitY, normalToPlayer);
	out_rotationRate = 0.5F;
	float time = 0.0F;

	float rootA = 0.0F;
	float rootB = 0.0F;
	if (GMath::QuadraticRoots(rootA, rootB, (0.5F * sGravity), in_verticalVelocity, in_shotOrigin.GetY() - in_shotTarget.GetY()))
	{
		time = GMath::Maximum(rootA, rootB);
	}

	if (time)
	{
		const float speed = distance / time;
		out_shotVelocity = GVector3Float(normalToPlayer.GetX() * speed, in_verticalVelocity, normalToPlayer.GetZ() * speed);
		out_rotationRate = speed / 20.0F;
	}

	return time;
}
//public accessors
const float BeachVolleyBallClientBall::GetRadius()const
{
	return sBallRadius;
}

//private methods
void BeachVolleyBallClientBall::SimulateBallPosition(const float in_timeDelta)
{
	const GVector3Float oldVelocity = mVelocity;

	//gravity
	mVelocity += (GVector3Float(0.0F, sGravity, 0.0F) * in_timeDelta);

	//wind
	//const float windHeight = mPosition.GetY() - 0.5F;
	//if (0.0F < windHeight)
	//{
	//	mVelocity.SetX(GMath::Lerp(in_timeDelta * 0.1F, mVelocity.GetX(), GMath::Maximum(1.0F, windHeight * 0.33F)));
	//	mVelocity.SetZ(GMath::Lerp(in_timeDelta * 0.1F, mVelocity.GetZ(), GMath::Maximum(1.0F, windHeight * 0.33F)));
	//}

	//intergrate position
	mPosition += (mVelocity + oldVelocity) * (0.5F * in_timeDelta);

	if (mPosition.GetY() < sBallRadius)
	{
		if (0 == mBounceCount)
		{
			mParentState.NotifyBallTouchedGround();
		}

		mBounceCount += 1;
		if (5 < mBounceCount)
		{
			mState = TState::TRolling;
			mPosition.SetY(sBallRadius);
			mVelocity.SetY(0.0F);
			return;
		}

		const float height = mPosition.GetY();
		mPosition.SetY(sBallRadius + ((sBallRadius - height) * 0.75F));
		mVelocity.SetY(mVelocity.GetY() * (-0.6F));

		mVelocity.SetX(mVelocity.GetX() + (GMath::RandomUnitPlusMinius() * 0.25F));
		mVelocity.SetZ(mVelocity.GetZ() + (GMath::RandomUnitPlusMinius() * 0.25F));


		float speed = 0.0F;
		mAxisRotation = Normalise(
			(mAxisRotation) + 
			Normalise(CrossProduct(GVector3Float::sUnitY, GVector3Float(mVelocity.GetX(), 0.0F, mVelocity.GetZ())), speed)
			);
		//mAxisRotation = Normalise(CrossProduct(GVector3Float::sUnitY, GVector3Float(mVelocity.GetX(), 0.0F, mVelocity.GetZ())), speed);
		mRadianRateRotation = (mRadianRateRotation + (speed * sBallRadius)) * 0.5F;
	}

	if ((0.00001F < GMath::Abs(mRadianRateRotation)) &&
		(0.1F < LengthSquared(mAxisRotation)))
	{
		const GMatrix9Float rotateMatrix = GMatrix9FloatConstructAxisAngle(mAxisRotation, mRadianRateRotation);
		mAt = mAt * rotateMatrix;
		mUp = mUp * rotateMatrix;
	}

	return;
}

void BeachVolleyBallClientBall::SimulateBallRoll(const float in_timeDelta)
{
	float scalarVelocity = 0.0F;
	mVelocity = Normalise(mVelocity, scalarVelocity);
	mVelocity.SetX(mVelocity.GetX() + (GMath::RandomUnitPlusMinius() * 2.5F * in_timeDelta));
	mVelocity.SetZ(mVelocity.GetZ() + (GMath::RandomUnitPlusMinius() * 2.5F * in_timeDelta));

	//reduce velocity
	scalarVelocity *= (1.0F - GMath::Minimum(1.0F, in_timeDelta));
	mVelocity *= scalarVelocity;

	mPosition += (mVelocity * in_timeDelta);

	mAxisRotation = Normalise(CrossProduct(GVector3Float::sUnitY, GVector3Float(mVelocity.GetX(), 0.0F, mVelocity.GetZ())));
	mRadianRateRotation = (scalarVelocity * sBallRadius);

	if ((0.00001F < GMath::Abs(mRadianRateRotation)) &&
		(0.1F < LengthSquared(mAxisRotation)))
	{
		const GMatrix9Float rotateMatrix = GMatrix9FloatConstructAxisAngle(mAxisRotation, mRadianRateRotation);
		mAt = mAt * rotateMatrix;
		mUp = mUp * rotateMatrix;
	}

	return;
}
