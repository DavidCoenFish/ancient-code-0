//
//  BeachVolleyBallClientBall.h
//  BeachVolleyBall
//
//  Created by David Coen on 2011 04 04
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _BeachVolleyBallClientBall_h_
#define _BeachVolleyBallClientBall_h_

#include <GSceneNodeComponentClient.h>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <GVector3Float.h>

class BeachVolleyBallGame;
class GSceneNode;
class GSceneNodeComponentBase;
class GScenePostLoadCallback;

class BeachVolleyBallClientBall : public GSceneNodeComponentClient
{
	//typedef
private:
	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;
	typedef boost::shared_ptr<GSceneNodeComponentBase> TPointerSceneNodeComponentBase;
	typedef std::vector<GScenePostLoadCallback> TArrayScenePostLoadCallback;

	//static public methods
public:
	static TPointerSceneNodeComponentBase Factory(
		GScene& inout_scene, //access to mesh/ material manager
		TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
		void* const in_data
		);

	//constructor
public:
	BeachVolleyBallClientBall(
		BeachVolleyBallGame& inout_parentState
		);
	~BeachVolleyBallClientBall();

	//implement GSceneNodeComponentClient
private:
	virtual void OnTick(TPointerSceneNode& inout_sceneNode, const float in_timeDelta);

	//public methods
public:
	void SetPlayerServe(
		const GVector3Float& in_position, 
		const GVector3Float& in_at,
		const GVector3Float& in_up
		);
	void SetBallInFlight(
		const GVector3Float& in_position, 
		const GVector3Float& in_velocity, 
		const GVector3Float& in_at,
		const GVector3Float& in_up,
		const GVector3Float& in_axisRotation,
		const float in_radianRateRotation
		);
	void SetBallAttachedToPlayer(
		const GVector3Float& in_position, 
		const GVector3Float& in_at,
		const GVector3Float& in_up
		);

	const bool GetPredictedPosition(
		GVector3Float& out_locationA,
		float& out_timeA,
		GVector3Float& out_locationB,
		float& out_timeB,
		const float in_height
		);

	const float CalculateTimeToPoint(
		GVector3Float& out_shotVelocity,
		GVector3Float& out_shotRotation,
		float& out_rotationRate,
		const GVector3Float& in_shotOrigin,
		const GVector3Float& in_shotTarget,
		const float in_verticalVelocity
		);

	//public accessors
public:
	const float GetRadius()const;
	const GVector3Float& GetPosition()const{ return mPosition; }
	const GVector3Float& GetVelocity()const{ return mVelocity; }
	const GVector3Float& GetAt()const{ return mAt; }
	const GVector3Float& GetUp()const{ return mUp; }

	//private methods
private:
	void SimulateBallPosition(const float in_timeDelta);
	void SimulateBallRoll(const float in_timeDelta);

	//private members
private:
	BeachVolleyBallGame& mParentState;
	int mState;
	int mBounceCount;
	GVector3Float mPosition;
	GVector3Float mVelocity; 
	GVector3Float mAt;
	GVector3Float mUp;
	GVector3Float mAxisRotation;
	float mRadianRateRotation;

};

#endif //_BeachVolleyBallClientBall_h_
