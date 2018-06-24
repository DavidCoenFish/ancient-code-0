//
//  BeachVolleyBallMenu.h
//  BeachVolleyBall
//
//  Created by David Coen on 2011 04 05
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _BeachVolleyBallMenu_h_
#define _BeachVolleyBallMenu_h_

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <GCamera.h>
#include "BeachVolleyBallType.h"

class BeachVolleyBallClientBall;
class BeachVolleyBallClientOpponent;
class GScene;
class GComponentCameraInput;
class BeachVolleyBallState;
class GInput;
class GSceneNode;
class GGui;
class GRenderComponent;
class GComponentLerpVector3Float;

class BeachVolleyBallMenu : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GComponentCameraInput> TPointerComponentCameraInput;

	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;
	typedef boost::weak_ptr<GSceneNode> TWeakSceneNode;

	typedef boost::shared_ptr<GGui> TPointerGui;
	typedef boost::weak_ptr<GGui> TWeakGui;

	typedef boost::scoped_ptr<GComponentLerpVector3Float> TPointerComponentLerpVector3Float;

	//static public methods
public:
	static void ClickButtonAppStore();

	//constructor
public:
	BeachVolleyBallMenu(
		BeachVolleyBallState& inout_parentState, 
		const float in_cameraAspect,
		const BeachVolleyBallType::TGirl::TEnum in_girl
		);
	~BeachVolleyBallMenu();

	//public methods
public:
	void Input(const GInput& in_input);
	void Tick(const float in_timeDelta);
	void Draw(GRenderComponent& in_renderComponent);
	void Resize(const float in_cameraAspect);

	void ClickButton(const std::string& in_name);

	//public accessors
public:
	GCamera& GetCamera();

	//private methods
private:
	void SetCameraType(const int in_cameraType);
	void SetMenuType(const int in_menuState);
	void ResetResources();
	TPointerGui	GetActiveGui();

	//private members
private:
	BeachVolleyBallState& mParentState;
	//TPointerComponentCameraInput mComponentCameraInput;
	TWeakSceneNode mSceneNodeRoot;
	TWeakSceneNode mSceneNodeBall;
	TWeakSceneNode mSceneNodeActor;

	TWeakGui mGuiLogo;
	TWeakGui mGuiMain;
	TWeakGui mGuiMatch;
	TWeakGui mGuiInfo;
	float mGuiAlpha;
	int mMenuState;
	float mCountdownGuiLogo;
	float mTimeWithoutInput;
	int mTimeWithoutInputCount;

	float mTime;

	GCamera mCamera;
	TPointerComponentLerpVector3Float mLerpCameraPos;
	TPointerComponentLerpVector3Float mLerpCameraAt;
	TPointerComponentLerpVector3Float mLerpCameraUp;

	float mCountdownActivateNextResources;
	bool mFadeMenuDown;
	int mCameraType;


};

#endif //_BeachVolleyBallMenu_h_
