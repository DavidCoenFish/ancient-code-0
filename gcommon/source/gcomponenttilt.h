//file: GComponentTilt.h
#ifndef _GComponentTilt_H_
#define _GComponentTilt_H_

#include <boost/noncopyable.hpp>

#include "GVector3Float.h"
#include "GPhysicsComponentSpring.h"
#include "GPhysicsComponentLimit.h"

class GMatrix16Float;

/**/
class GComponentTilt : public boost::noncopyable
{
	//constructor
public:
	GComponentTilt(
		const float in_springForce,
		const float in_springInnerFriction,
		const float in_maxDistance,
		const GVector3Float& in_centerGravity,
		const GVector3Float& in_position
		);
	~GComponentTilt();

	// public methods
public:
	void Update(
		const float in_timeDelta,
		const GVector3Float& in_newPosition,
		const bool in_teleport
		);

	void ZeroTilt();
	void GetTiltMatrix(
		GMatrix16Float& out_transform
		);

	// private members
private:
	GPhysicsComponentSpring mSpring;
	GPhysicsComponentLimit mLimit;
	GVector3Float mCenterGravity;
	GVector3Float mTracePoint;
	GVector3Float mPosition;
	GVector3Float mVelocity;

};

#endif // _GComponentTilt_H_