//file: GComponentTilt.cpp

#include "GComponentTilt.h"

#include "GMatrix16Float.h"
#include "GMath.h"

//constructor
GComponentTilt::GComponentTilt(
	const float in_springForce,
	const float in_springInnerFriction,
	const float in_maxDistance,
	const GVector3Float& in_centerGravity,
	const GVector3Float& in_position
	)
: mSpring(in_springForce, in_springInnerFriction)
, mLimit(in_maxDistance)
, mCenterGravity(in_centerGravity)
, mTracePoint(in_position + in_centerGravity)
, mPosition(in_position)
, mVelocity()
{
	return;
}

GComponentTilt::~GComponentTilt()
{
	return;
}

// public methods
void GComponentTilt::Update(
	const float in_timeDelta,
	const GVector3Float& in_newPosition,
	const bool in_teleport
	)
{
	if (in_teleport)
	{
		mTracePoint = in_newPosition + mCenterGravity;
	}

	GVector3Float targetVelocity;
	if (in_timeDelta)
	{
		targetVelocity = (in_newPosition - mPosition) / in_timeDelta;
	}
	mPosition = in_newPosition;

	//spring force
	const GVector3Float force = mSpring.CalculateSpringForce(
		mPosition + mCenterGravity,
		0.0F,
		mTracePoint
		);

	//spring friction
	const GVector3Float friction = mSpring.CalculateSpringFriction(
		mVelocity,
		targetVelocity
		);

	//intergrate
	mVelocity += ((force + friction) * in_timeDelta);
	const GVector3Float oldTracePoint = mTracePoint;
	mTracePoint += (mVelocity * in_timeDelta);

	//do limit
	if (mLimit.CalculateLimit(mTracePoint, mPosition + mCenterGravity))
	{
		mVelocity = (mTracePoint - oldTracePoint) / in_timeDelta;
	}

	return;
}


void GComponentTilt::ZeroTilt()
{
	mTracePoint = mPosition + mCenterGravity;
	mVelocity = GVector3Float::sZero;
	return;
}

void GComponentTilt::GetTiltMatrix(
	GMatrix16Float& out_transform
	)
{
	const GVector3Float upBase = Normalise(mCenterGravity);
	const GVector3Float atBase = Normalise(CrossProduct(GVector3Float::sUnitX, upBase));

	const GVector3Float up = Normalise(mTracePoint - mPosition);
	const GVector3Float at = Normalise(CrossProduct(GVector3Float::sUnitX, up));

	if (GMath::Abs(LengthSquared(at - atBase)) < 0.001F)
	{
		out_transform = GMatrix16Float::sIdentity;
	}

	out_transform = GMatrix16FloatConstructAtUp(
		at, 
		up,
		atBase, 
		upBase
		);

	return;
}
