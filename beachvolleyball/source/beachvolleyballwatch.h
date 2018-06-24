//
//  BeachVolleyBallWatch.h
//  BeachVolleyBall
//
//  Created by David Coen on 2011 05 02
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _BeachVolleyBallWatch_h_
#define _BeachVolleyBallWatch_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <GCamera.h>
#include <GVector2Float.h>
#include <GVector3Float.h>
#include "BeachVolleyBallType.h"

class BeachVolleyBallState;
class BeachVolleyBallClientBall;
class BeachVolleyBallClientOpponent;
class GScene;
class GInput;
class GSceneNode;
class GGui;
class GRenderComponent;
class GGuiNodeComponentText;
class GSceneNodeComponentAnimation;

class BeachVolleyBallWatch : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<BeachVolleyBallClientOpponent> TPointerBeachVolleyBallClientOpponent;
	typedef boost::weak_ptr<BeachVolleyBallClientOpponent> TWeakBeachVolleyBallClientOpponent;

	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;
	typedef boost::weak_ptr<GSceneNode> TWeakSceneNode;

	typedef boost::shared_ptr<GSceneNodeComponentAnimation> TPointerSceneNodeComponentAnimation;
	typedef boost::weak_ptr<GSceneNodeComponentAnimation> TWeakSceneNodeComponentAnimation;

	typedef boost::shared_ptr<GGui> TPointerGui;
	typedef boost::weak_ptr<GGui> TWeakGui;

	typedef boost::weak_ptr<GGuiNodeComponentText> TWeakGuiNodeComponentText;

	//constructor
public:
	BeachVolleyBallWatch(
		BeachVolleyBallState& inout_parentState, 
		const float in_cameraAspect,
		const BeachVolleyBallType::TGirl::TEnum in_girl,
		const BeachVolleyBallType::TMatch::TEnum in_match
		);
	~BeachVolleyBallWatch();

	//public methods
public:
	void Input(const GInput& in_input);
	void Tick(const float in_timeDelta);
	void Draw(GRenderComponent& in_renderComponent);
	void Resize(const float in_cameraAspect);

	void ClickButton(const std::string& in_name);

	//public accessors
public:
	GCamera& GetCamera(){ return mCamera; }

	//private methods
private:
	void UpdateCamera();
	void SetAnimation(const int in_animationId, const float in_fadeTime);
	void FlushInput(const GInput& in_input);

	//private members
private:
	BeachVolleyBallState& mParentState;
	GCamera mCamera;
	TWeakSceneNode mSceneNodeRoot;

	TWeakSceneNodeComponentAnimation mActorAnimation;

	int mAnimationId;
	float mGuiFadeIn;
	float mFadeDownText;
	TWeakGui mGui;
	TWeakGuiNodeComponentText mMessageText;
	TWeakGuiNodeComponentText mAnimationText;

	//gather input, but apply on tick, so input is frame rate dependant
	GVector2Float mTouchDelta;
	float mZoomDelta;

	GVector3Float mCameraOrigin;
	float mZoom;
	float mLatitude;
	float mLongitude;

};

#endif //_BeachVolleyBallWatch_h_
