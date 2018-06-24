//
//  BeachVolleyBallGame.h
//  BeachVolleyBall
//
//  Created by David Coen on 2011 04 05
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _BeachVolleyBallGame_h_
#define _BeachVolleyBallGame_h_

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <GCamera.h>
#include <GVector2Float.h>
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
class BeachVolleyBallSound;

class BeachVolleyBallGame : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<BeachVolleyBallClientBall> TPointerBeachVolleyBallClientBall;
	typedef boost::weak_ptr<BeachVolleyBallClientBall> TWeakBeachVolleyBallClientBall;

	typedef boost::shared_ptr<BeachVolleyBallClientOpponent> TPointerBeachVolleyBallClientOpponent;
	typedef boost::weak_ptr<BeachVolleyBallClientOpponent> TWeakBeachVolleyBallClientOpponent;

	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;
	typedef boost::weak_ptr<GSceneNode> TWeakSceneNode;

	typedef boost::shared_ptr<GGui> TPointerGui;
	typedef boost::weak_ptr<GGui> TWeakGui;

	typedef boost::weak_ptr<GGuiNodeComponentText> TWeakGuiNodeComponentText;
	typedef boost::scoped_ptr<BeachVolleyBallSound> TPointerBeachVolleyBallSound;

	//constructor
public:
	BeachVolleyBallGame(
		BeachVolleyBallState& inout_parentState, 
		const float in_cameraAspect,
		const BeachVolleyBallType::TGirl::TEnum in_girl,
		const BeachVolleyBallType::TMatch::TEnum in_match,
		const bool in_demoMode = false
		);
	~BeachVolleyBallGame();

	//public methods
public:
	void Input(const GInput& in_input);
	void Tick(const float in_timeDelta);
	void Draw(GRenderComponent& in_renderComponent);
	void Resize(const float in_cameraAspect);

	void NotifyBallTouchedGround();
	void NotifyOpponentHitBall();

	void ClickButton(const std::string& in_buttonName);
	void Shake();

	//public accessors
public:
	GCamera& GetCamera(){ return mCamera; }
	void SetClientBall(TPointerBeachVolleyBallClientBall& in_clientBall);
	void SetClientOpponent(TPointerBeachVolleyBallClientOpponent& in_clientOpponent);
	const GVector3Float GetPlayerPosition()const;
	TPointerBeachVolleyBallClientBall GetBall(); //because opponent want to know ball pos, tell ball about hit
	TPointerBeachVolleyBallSound& GetSoundManager();

	//private methods
private:
	void SetStatePlayerServe();
	void SetStateOpponentServe();
	void SetStateGameOver();
	void HitBall(const float in_timeDelta);
	void MultiTouchFoul();
	void ScorePoint(const bool in_playerPoint);
	void FlushInput(const GInput& in_input);
	
	//private members
private:
	const BeachVolleyBallType::TGirl::TEnum mGirl;
	const BeachVolleyBallType::TMatch::TEnum mMatch;
	int mState;
	int mScoreState;
	int mScorePlayer;
	int mScoreOpponent;
	float mStateCountdown;
	float mMessageCountdown;
	BeachVolleyBallState& mParentState;
	GCamera mCamera;
	TWeakBeachVolleyBallClientOpponent mClientOpponent;
	TWeakBeachVolleyBallClientBall mClientBall;
	TWeakSceneNode mSceneNodeRoot;

	bool mHasScreenTouch;
	bool mMultiTouchFoul;
	bool mFlushInput;
	const bool mDemoMode;
	GVector2Float mScreenTouch;
	GVector2Float mScreenTouchDelta;
	int mRallyCount;

	TWeakGui mGuiScore;
	TWeakGuiNodeComponentText mPlayerScoreText;
	TWeakGuiNodeComponentText mOpponentScoreText;
	TWeakGuiNodeComponentText mMessageText;
	TWeakGuiNodeComponentText mMessageTextWin;
#ifdef FREE
	TWeakGui mGuiThanks;
#endif
};

#endif //_BeachVolleyBallGame_h_
