//
//  BeachVolleyBallClientOpponent.h
//  BeachVolleyBall
//
//  Created by David Coen on 2011 04 04
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _BeachVolleyBallClientOpponent_h_
#define _BeachVolleyBallClientOpponent_h_

#include <GSceneNodeComponentClient.h>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <GVector3Float.h>
#include "BeachVolleyBallType.h"

class BeachVolleyBallGame;
class BeachVolleyBallClientBall;
class GComponentLerpFloat;
class GComponentLerpVector3Float;
class GComponentTilt;
class GSceneNodeComponentAnimation;
class GSkeletonInstance;

class BeachVolleyBallClientOpponent : public GSceneNodeComponentClient
{
	//typedef
private:
	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;
	typedef boost::shared_ptr<GSceneNodeComponentBase> TPointerSceneNodeComponentBase;

	typedef std::vector<GScenePostLoadCallback> TArrayScenePostLoadCallback;

	typedef boost::shared_ptr<GComponentLerpVector3Float> TPointerComponentLerpVector3Float;
	typedef boost::shared_ptr<GComponentLerpFloat> TPointerComponentLerpFloat;
	typedef boost::shared_ptr<GComponentTilt> TPointerComponentTilt;

	typedef boost::shared_ptr<GSceneNodeComponentAnimation> TPointerSceneNodeComponentAnimation;
	typedef boost::weak_ptr<GSceneNodeComponentAnimation> TWeakSceneNodeComponentAnimation;

	typedef boost::shared_ptr<GSkeletonInstance> TPointerSkeletonInstance;
	typedef boost::weak_ptr<GSkeletonInstance> TWeakSkeletonInstance;

	//static public methods
public:
	static TPointerSceneNodeComponentBase FactoryBikini0(
		GScene& inout_scene, //access to mesh/ material manager
		TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
		void* const in_data
		);
	static TPointerSceneNodeComponentBase FactoryBikini1(
		GScene& inout_scene, //access to mesh/ material manager
		TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
		void* const in_data
		);
	static TPointerSceneNodeComponentBase FactoryBikini2(
		GScene& inout_scene, //access to mesh/ material manager
		TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
		void* const in_data
		);

	static TPointerSceneNodeComponentBase FactoryReiko0(
		GScene& inout_scene, //access to mesh/ material manager
		TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
		void* const in_data
		);
	static TPointerSceneNodeComponentBase FactoryReiko1(
		GScene& inout_scene, //access to mesh/ material manager
		TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
		void* const in_data
		);
	static TPointerSceneNodeComponentBase FactoryReiko2(
		GScene& inout_scene, //access to mesh/ material manager
		TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
		void* const in_data
		);

	static TPointerSceneNodeComponentBase FactoryApril0(
		GScene& inout_scene, //access to mesh/ material manager
		TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
		void* const in_data
		);
	static TPointerSceneNodeComponentBase FactoryApril1(
		GScene& inout_scene, //access to mesh/ material manager
		TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
		void* const in_data
		);
	static TPointerSceneNodeComponentBase FactoryApril2(
		GScene& inout_scene, //access to mesh/ material manager
		TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
		void* const in_data
		);

	static TPointerSceneNodeComponentBase FactoryCuda0(
		GScene& inout_scene, //access to mesh/ material manager
		TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
		void* const in_data
		);
	static TPointerSceneNodeComponentBase FactoryCuda2(
		GScene& inout_scene, //access to mesh/ material manager
		TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
		void* const in_data
		);
	static TPointerSceneNodeComponentBase FactoryCuda1(
		GScene& inout_scene, //access to mesh/ material manager
		TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
		void* const in_data
		);

	//static private methods
private:
	static TPointerSceneNodeComponentBase  Factory(
		GScene& inout_scene, //access to mesh/ material manager
		TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
		void* const in_data,
		const BeachVolleyBallType::TGirl::TEnum in_girl,
		const BeachVolleyBallType::TMatch::TEnum in_match
		);

	static void PostLoadCallback(
		GScene& inout_scene, //access to mesh/ material manager
		TPointerSceneNode& inout_owner,
		TPointerSceneNodeComponentBase& inout_component,
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback
		);
	static void PostLoadCallbackB(
		GScene& inout_scene, //access to mesh/ material manager
		TPointerSceneNode& inout_owner,
		TPointerSceneNodeComponentBase& inout_component,
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback
		);
	static void BoneUpdateCallback(
		GVector3Float& inout_at,
		GVector3Float& inout_up,
		GVector3Float& inout_pos,
		float& inout_scale,
		GSceneNode& in_parentSceneNode,
		const float in_timeDelta,
		void* in_callbackData,
		GSkeletonInstance& inout_skeletonInstance
		);

	//constructor
public:
	BeachVolleyBallClientOpponent(
		BeachVolleyBallGame& inout_parentGame,
		TPointerSceneNode& inout_owner,
		const BeachVolleyBallType::TGirl::TEnum in_girl,
		const BeachVolleyBallType::TMatch::TEnum in_match
		);
	~BeachVolleyBallClientOpponent();

	//implement GSceneNodeComponentClient
private:
	virtual void OnTick(TPointerSceneNode& inout_sceneNode, const float in_timeDelta);

	//public methods
public:
	void NotifyBallTouchGround();
	void NotifyPlayerServe();
	void NotifyOpponentServe();
	void NotifyPlayerHitBall(BeachVolleyBallClientBall& in_ball);

	void NotifyOpponentWinPoint();
	void NotifyOpponentWinServe();
	void NotifyOpponentWinGame();

	void NotifyOpponentLostPoint();
	void NotifyOpponentLostServe();
	void NotifyOpponentLostGame();

	void DoMorphTest(const float in_amount);

	//public accessors
public:

	//private methods
private:
	void PlayAnimation(
		const int in_animationId,
		const float in_fadeInTime,
		const float in_rateMul = 1.0F,
		const float in_force = false
		);

	void SelectMoveAnimation(
		const GVector3Float& in_targetPosition,
		const GVector3Float& in_position,
		const GVector3Float& in_gameDirection,
		const float in_time
		);

	//private members
private:
	BeachVolleyBallGame& mParentState;
	const GVector3Float mOriginalPosition;
	const float mOriginalRotation;
	const BeachVolleyBallType::TGirl::TEnum mGirl;
	const BeachVolleyBallType::TMatch::TEnum mMatch;
	int mState;
	TPointerComponentLerpVector3Float mLerpPosition;
	TPointerComponentLerpFloat mLerpRotation;
	TPointerComponentTilt mComponentTilt;

	float mStateCountdown;
	int mShotType;
	bool mSetTeleport;
	bool mSetTeleportTilt;
	//bool mPlayFallAnimation; //going through the states to play a fall animation, ignore win/loss point/serve notification

	TWeakSceneNodeComponentAnimation mComponentAnimation;
	int mAnimation;

	TWeakSkeletonInstance mSkeleton;
	int mBallBoneId;
	int mNeckBoneId;
	int mMorphTargetId;

	bool mLookAtPlayer;
	float mLookAtPlayerWeight;
	bool mLookAtBall;
	float mLookAtBallWeight;

	bool mMoveRotate;
	float mMoveRotateTime;
	float mMoveRotateCountdown;

};

#endif //_BeachVolleyBallClientOpponent_h_
