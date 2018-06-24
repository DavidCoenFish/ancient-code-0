//
//  BeachVolleyBallMenu.cpp
//  BeachVolleyBall
//
//  Created by David Coen on 2011 04 05
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "BeachVolleyBallMenu.h"
#include "BeachVolleyBallState.h"
#include "BeachVolleyBallMenuButtonCallBack.h"
#include "BeachVolleyBallGameVar.h"
#include "BeachVolleyBallSound.h"

#include <GCommon.h>
#include <GComponentCameraInput.h>
#include <GScene.h>
#include <GSceneNode.h>
#include <GScenePostLoadCallback.h>
#include <GMath.h>
#include <GSceneVisitorAll.h>
#include <GSceneNodeComponentAnimation.h>
#include <GSceneNodeComponentNodeAttach.h>
#include <GSceneNodeComponentVisual.h>
#include <GAnimationManagerInstance.h>
#include <GAnimationPlayParam.h>
#include <GGui.h>
#include <GGuiLoad.h>
#include <GGuiNodeLoad.h>
#include <GGuiManager.h>
#include <GGuiNodeComponentButton.h>
#include <GColour4Byte.h>
#include <GGuiType.h>
#include <GGuiVisitor.h>
#include <GGuiVisitorUpdate.h>
#include <GGuiVisitorRender.h>
#include <GGuiVisitorInput.h>
#include <GComponentLerpVector3Float.h>
#include <GInput.h>

#ifdef IPHONE
#include <Foundation/Foundation.h>
#include <UIKit/UIKit.h>
#endif

static const float sDefaultCameraPerspectiveBounds = 0.08F;
static const float sDefaultCameraPerspectiveNear = 0.1F;
static const float sDefaultCameraPerspectiveFar = 1000.0F;
static const float sCameraTransitionTime = 1.0F;
static const float sTimeWithoutInputDemoStart = 20.0F;

typedef std::vector<GScenePostLoadCallback> TArrayScenePostLoadCallback;

typedef boost::shared_ptr<GSceneNodeComponentAnimation> TPointerSceneNodeComponentAnimation;
typedef boost::weak_ptr<GSceneNodeComponentAnimation> TWeakSceneNodeComponentAnimation;
typedef std::vector<TWeakSceneNodeComponentAnimation> TArrayWeakSceneNodeComponentAnimation;
typedef boost::shared_ptr<GAnimationManagerInstance> TPointerAnimationManagerInstance;

typedef boost::shared_ptr<GGuiNodeComponentButtonCallback> TPointerGuiNodeComponentButtonCallback;

typedef boost::shared_ptr<GGuiNodeComponentButton> TPointerGuiNodeComponentButton;
typedef boost::weak_ptr<GGuiNodeComponentButton> TWeakGuiNodeComponentButton;
typedef std::vector<TWeakGuiNodeComponentButton> TArrayWeakGuiNodeComponentButton;

typedef boost::shared_ptr<GGui> TPointerGui;
typedef boost::weak_ptr<GGui> TWeakGui;

struct TCameraType
{
	enum TEnum
	{
		TMenuStart = 0,
		TGirlOne,
		TGirlTwo,
		TGirlThree,
		TGirlFour,
		TCount
	};
};

struct TCameraData
{
	const float mPos[3];
	const float mAt[3];
	const float mUp[3];
	const BeachVolleyBallType::TGirl::TEnum mGirl;
	const float mGirlPos[3];
	const float mGirlAt[3];
};

struct TMenuState
{
	enum TEnum
	{
		TNone = 0,
		TLogo,
		TMain,
		TPlay,
		TWatch,
		TInfo,
		TCount
	};
};

static const TCameraData sArrayCameraData[TCameraType::TCount] = {
	//TMenuStart
	{ 
		{ 0.0F, 5.0F, 0.0F },
		{ 0.0F, 1.0F, 0.0F },
		{ 0.0F, 0.0F, 1.0F },
		BeachVolleyBallType::TGirl::TCount,
		{ 0.0F, 0.0F, 0.0F },
		{ 0.0F, 0.0F, 0.0F }
	},
	//TGirlOne
	{ 
		{ 1.481F, 1.0F, -1.121F },
		{ 0.347F, -0.165F, -0.923F },
		{ 0.058F, 0.986F, -0.154F },
		BeachVolleyBallType::TGirl::TBikini,
		{ 2.071F, 0.0F, -2.803F },
		{ -0.523F, 0.0F, 0.852F }
	},
	//TGirlTwo
	{ 
		{ 1.894F, 1.0F, 1.002F },
		{ 0.923F, -0.165F, 0.347F },
		{ 0.155F, 0.986F, 0.058F },
		BeachVolleyBallType::TGirl::TReiko,
		{ 3.035F, 0.0F, 1.515F },
		{ -0.932F, 0.0F, -0.363F }
	},
	//TGirlThree
	{ 
		{ -0.797F, 1.0F, 1.654F },
		{ -0.347F, -0.165F, 0.923F },
		{ -0.058F, 0.986F, 0.155F },
		BeachVolleyBallType::TGirl::TApril,
		{ -1.225F, 0.0F, 2.872F },
		{ 0.423F, 0.0F, -0.906F }
	},
	//TGirlFour
	{ 
		{ -1.384F, 1.25F, -1.279F },
		{ -0.906F, -0.25F, -0.34F },
		{ -0.234F, 0.968F, -0.088F },
		BeachVolleyBallType::TGirl::TCuda,
		{ -2.507F, 0.0F, -1.705F },
		{ 0.906F, 0.0F, 0.423F }
	},
};

static void LocalSetButtonCallback(
	TPointerGuiNodeComponentButtonCallback& inout_pointerButtonCallback, 
	TWeakGui& in_gui
	)
{
	TPointerGui pointerGui = in_gui.lock();
	if (pointerGui)
	{
		GGui& gui = *pointerGui;
		TArrayWeakGuiNodeComponentButton arrayButton;
		GGuiVisitor<GGuiNodeComponentButton>::Run(gui, arrayButton);
		for (TArrayWeakGuiNodeComponentButton::iterator iterator = arrayButton.begin(); iterator != arrayButton.end(); ++iterator)
		{
			TPointerGuiNodeComponentButton pointerButton = (*iterator).lock();
			if (!pointerButton)
			{
				continue;
			}
			GGuiNodeComponentButton& button = *pointerButton;
			button.SetCallback(inout_pointerButtonCallback);
		}
	}

	return;
}

const void LocalSetMatch(
	const bool in_hasMatch1,
	const bool in_hasMatch2,
	const bool in_hasMatch3,
	GGui& in_gui
	)
{
	GGuiNode* const nodeMatch1 = GGuiVisitorNodeName::FindNode(in_gui, "NodeMatch1");
	GGuiNode* const nodeMatch2 = GGuiVisitorNodeName::FindNode(in_gui, "NodeMatch2");
	GGuiNode* const nodeMatch3 = GGuiVisitorNodeName::FindNode(in_gui, "NodeMatch3");

	if (in_hasMatch1 &&
		in_hasMatch2 &&
		in_hasMatch3)
	{
		if (nodeMatch1)
		{
			nodeMatch1->SetTopLeft(GGuiCoordinate(
				GVector2Float(0.5F, 1.0F), 
				GVector2Float(-127.0F, -186.0F)
				));
		}
		if (nodeMatch2)
		{
			nodeMatch2->SetTopLeft(GGuiCoordinate(
				GVector2Float(0.5F, 1.0F), 
				GVector2Float(-127.0F, -124.0F)
				));
		}
		if (nodeMatch3)
		{
			nodeMatch3->SetTopLeft(GGuiCoordinate(
				GVector2Float(0.5F, 1.0F), 
				GVector2Float(-127.0F, -62.0F)
				));
		}

	}
	else if (in_hasMatch1 &&
		in_hasMatch2)
	{
		if (nodeMatch1)
		{
			nodeMatch1->SetTopLeft(GGuiCoordinate(
				GVector2Float(0.5F, 1.0F), 
				GVector2Float(-127.0F, -124.0F)
				));
		}
		if (nodeMatch2)
		{
			nodeMatch2->SetTopLeft(GGuiCoordinate(
				GVector2Float(0.5F, 1.0F), 
				GVector2Float(-127.0F, -62.0F)
				));
		}
		if (nodeMatch3)
		{
			nodeMatch3->SetTopLeft(GGuiCoordinate(
				GVector2Float(0.5F, 1.0F), 
				GVector2Float(-127.0F, 0.0F)
				));
		}
	}
	else if (in_hasMatch1)
	{
		if (nodeMatch1)
		{
			nodeMatch1->SetTopLeft(GGuiCoordinate(
				GVector2Float(0.5F, 1.0F), 
				GVector2Float(-127.0F, -62.0F)
				));
		}
		if (nodeMatch2)
		{
			nodeMatch2->SetTopLeft(GGuiCoordinate(
				GVector2Float(0.5F, 1.0F), 
				GVector2Float(-127.0F, 0.0F)
				));
		}
		if (nodeMatch3)
		{
			nodeMatch3->SetTopLeft(GGuiCoordinate(
				GVector2Float(0.5F, 1.0F), 
				GVector2Float(-127.0F, 0.0F)
				));
		}
	}
	else
	{
		if (nodeMatch1)
		{
			nodeMatch1->SetTopLeft(GGuiCoordinate(
				GVector2Float(0.5F, 1.0F), 
				GVector2Float(-127.0F, 0.0F)
				));
		}
		if (nodeMatch2)
		{
			nodeMatch2->SetTopLeft(GGuiCoordinate(
				GVector2Float(0.5F, 1.0F), 
				GVector2Float(-127.0F, 0.0F)
				));
		}
		if (nodeMatch3)
		{
			nodeMatch3->SetTopLeft(GGuiCoordinate(
				GVector2Float(0.5F, 1.0F), 
				GVector2Float(-127.0F, 0.0F)
				));
		}
	}
		

	return;
}

static const BeachVolleyBallType::TGirl::TEnum LocalGetGirlCamera(const int in_cameraType)
{
	switch (in_cameraType)
	{
	default:
		break;
	case TCameraType::TGirlOne:
		return BeachVolleyBallType::TGirl::TBikini;
	case TCameraType::TGirlTwo:
		return BeachVolleyBallType::TGirl::TReiko;
	case TCameraType::TGirlThree:
		return BeachVolleyBallType::TGirl::TApril;
	case TCameraType::TGirlFour:
		return BeachVolleyBallType::TGirl::TCuda;
	}

	return BeachVolleyBallType::TGirl::TCount;
}

/*static*/ void BeachVolleyBallMenu::ClickButtonAppStore()
{
#ifdef IPHONE
	NSURL *appStoreUrl = [NSURL URLWithString:@"http://itunes.apple.com/au/app/bikini-beach-volleyball/id436729444"];

	[[UIApplication sharedApplication] openURL:appStoreUrl];
#endif
	return;
}

//constructor
BeachVolleyBallMenu::BeachVolleyBallMenu(
	BeachVolleyBallState& inout_parentState, 
	const float in_cameraAspect,
	const BeachVolleyBallType::TGirl::TEnum in_girl
	)
	: mParentState(inout_parentState)
	, mSceneNodeRoot()
	, mSceneNodeBall()
	, mSceneNodeActor()
	, mGuiMain()
	, mGuiMatch()
	, mGuiInfo()
	, mGuiAlpha(0.0F)
	, mMenuState(TMenuState::TNone)
	, mTime(0.0F)
	, mTimeWithoutInput(0.0F)
	, mTimeWithoutInputCount(0)
	, mCamera(
		GCamera::TType::TPerspective,
		sDefaultCameraPerspectiveNear,
		sDefaultCameraPerspectiveFar,
		sDefaultCameraPerspectiveBounds,
		-sDefaultCameraPerspectiveBounds,
		-(sDefaultCameraPerspectiveBounds * in_cameraAspect),
		sDefaultCameraPerspectiveBounds * in_cameraAspect
		)
	, mLerpCameraPos()
	, mLerpCameraAt()
	, mLerpCameraUp()
	, mCountdownActivateNextResources(0.0F)
	, mFadeMenuDown(false)
	, mCameraType(TCameraType::TMenuStart)
	, mCountdownGuiLogo(0.0F)
{
	switch (in_girl)
	{
	default:
		break;
	case BeachVolleyBallType::TGirl::TBikini:
		mCameraType = TCameraType::TGirlOne;
		break;
	case BeachVolleyBallType::TGirl::TReiko:
		mCameraType = TCameraType::TGirlTwo;
		break;
	case BeachVolleyBallType::TGirl::TApril:
		mCameraType = TCameraType::TGirlThree;
		break;
	case BeachVolleyBallType::TGirl::TCuda:
		mCameraType = TCameraType::TGirlFour;
		break;
	}

	inout_parentState.SetWorldFacing(BeachVolleyBallState::GetWorldFacingTypeIntro());
	inout_parentState.SetDrawShadows(false);
	inout_parentState.GetSoundManager()->SetContext(BeachVolleyBallSound::GetContextMenu());

	mLerpCameraPos.reset(new GComponentLerpVector3Float(GVector3Float(
		sArrayCameraData[mCameraType].mPos[0],
		sArrayCameraData[mCameraType].mPos[1],
		sArrayCameraData[mCameraType].mPos[2]
		)));
	mLerpCameraAt.reset(new GComponentLerpVector3Float(GVector3Float(
		sArrayCameraData[mCameraType].mAt[0],
		sArrayCameraData[mCameraType].mAt[1],
		sArrayCameraData[mCameraType].mAt[2]
		)));
	mLerpCameraUp.reset(new GComponentLerpVector3Float(GVector3Float(
		sArrayCameraData[mCameraType].mUp[0],
		sArrayCameraData[mCameraType].mUp[1],
		sArrayCameraData[mCameraType].mUp[2]
		)));

	mCamera.SetTransform(
		mLerpCameraAt->GetValue(),
		mLerpCameraUp->GetValue(),
		mLerpCameraPos->GetValue()
		);

	if (inout_parentState.GetScene())
	{
		GScene& scene = *inout_parentState.GetScene();
		TPointerSceneNode sceneNode = scene.CreateSceneNode("menuRoot");
		scene.AddSceneRoot(sceneNode);
		mSceneNodeRoot = sceneNode;

		TArrayScenePostLoadCallback arrayPostLoadCallback;
		{
			TPointerSceneNode sceneNodeBall = scene.CreateSceneNode("menuBall");
			mSceneNodeBall = sceneNodeBall;
			sceneNode->AddChildNode(sceneNodeBall);
			scene.SceneNodeFactory("factoryball", "", sceneNodeBall, arrayPostLoadCallback);
			GMatrix16Float transform = GMatrix16FloatConstructAxisAngle(GVector3Float::sUnitX, GMath::GetHalfPI());
			transform.SetPosition(GVector3Float(0.0F, 5.3F, 0.0F));
			sceneNodeBall->SetParentRelativeTransform(transform);
		}

		GScene::FactoryPostLoad(arrayPostLoadCallback, scene);
	}

	mParentState.GetGuiManager()->SetGuiColour(GColour4Float(1.0F, 1.0F, 1.0F, 1.0F));
	mGuiMain = mParentState.GetGuiManager()->LoadGui("guimain.data");
	mGuiMatch = mParentState.GetGuiManager()->LoadGui("guimain2.data");
#ifdef FREE
	mGuiInfo = mParentState.GetGuiManager()->LoadGui("guiinfofree.data");
#else
	mGuiInfo = mParentState.GetGuiManager()->LoadGui("guiinfo.data");
#endif

	TPointerGuiNodeComponentButtonCallback pointerButtonCallback;
	pointerButtonCallback.reset(new BeachVolleyBallMenuButtonCallBack(*this));

	LocalSetButtonCallback(pointerButtonCallback, mGuiMain);
	LocalSetButtonCallback(pointerButtonCallback, mGuiMatch);
	LocalSetButtonCallback(pointerButtonCallback, mGuiInfo);

	if (BeachVolleyBallType::TGirl::TCount == in_girl)
	{
		SetMenuType(TMenuState::TLogo);
		mGuiLogo = mParentState.GetGuiManager()->LoadGui("guilogo.data");
		mCountdownGuiLogo = 0.5F;
		mFadeMenuDown = true;
		mGuiAlpha = 1.0F;
	}
	else
	{
		ResetResources();
	}

	return;
}

BeachVolleyBallMenu::~BeachVolleyBallMenu()
{
	if (mParentState.GetScene())
	{
		GScene& scene = *mParentState.GetScene();
		TPointerSceneNode sceneNode = mSceneNodeRoot.lock();
		scene.SceneNodeRequestDelete(sceneNode);
		scene.SceneNodeProcessDeleteQueue();
	}
	if (mParentState.GetGuiManager())
	{
		mParentState.GetGuiManager()->DestroyGui(mGuiMain);
		mParentState.GetGuiManager()->DestroyGui(mGuiInfo);
		mParentState.GetGuiManager()->DestroyGui(mGuiMatch);
		mParentState.GetGuiManager()->DestroyGui(mGuiLogo);
	}

	return;
}

//public methods
void BeachVolleyBallMenu::Input(const GInput& in_input)
{
	TPointerGui pointerGui = GetActiveGui();
	if (pointerGui)
	{
		GGui& gui = *pointerGui;
		GGuiVisitorInput::Run(gui, in_input);
	}
	if (in_input.GetTouchCount())
	{
		mTimeWithoutInput = 0.0F;
		mTimeWithoutInputCount = 0;
	}
	return;
}

void BeachVolleyBallMenu::Tick(const float in_timeDelta)
{
	mTimeWithoutInput += in_timeDelta;
	if (0.0F < mCountdownGuiLogo)
	{
		mCountdownGuiLogo -= in_timeDelta;
		if (mCountdownGuiLogo <= 0.0F)
		{
			mCountdownGuiLogo = 0.0F;
			SetCameraType(TCameraType::TGirlOne);
		}
	}
	switch (mMenuState)
	{
	default:
		break;
	case TMenuState::TLogo:
	case TMenuState::TInfo:
	case TMenuState::TMain:
	case TMenuState::TPlay:
	case TMenuState::TWatch:
		if (mFadeMenuDown)
		{
			mGuiAlpha -= (in_timeDelta * 2.0F);
		}
		else
		{
			mGuiAlpha += (in_timeDelta * 2.0F);
		}
		break;
	}
	mGuiAlpha = GMath::Clamp(mGuiAlpha, 0.0F, 1.0F);
	mParentState.GetGuiManager()->SetGuiColour(GColour4Float(1.0F, 1.0F, 1.0F, mGuiAlpha));

	if (mLerpCameraPos->GetActive() || 
		mLerpCameraAt->GetActive() ||
		mLerpCameraUp->GetActive())
	{
		mLerpCameraPos->Tick(in_timeDelta);
		mLerpCameraAt->Tick(in_timeDelta);
		mLerpCameraUp->Tick(in_timeDelta);

		GVector3Float at = mLerpCameraAt->GetValue();
		GVector3Float up = mLerpCameraUp->GetValue();
		Orthogonalise(at, up);

		const GVector3Float oldPos = mCamera.GetTransformPos();
		mCamera.SetTransform(
			at,
			up,
			mLerpCameraPos->GetValue()
			);

		mParentState.GetSoundManager()->SetContextPosition(
			mCamera.GetTransformPos(),
			at,
			up,
			(mCamera.GetTransformPos() - oldPos) / in_timeDelta
			);
	}

	mTime += in_timeDelta;
	TPointerSceneNode sceneNodeBall = mSceneNodeBall.lock();
	if (sceneNodeBall)
	{
		GMatrix16Float transform = GMatrix16FloatConstructAxisAngle(GVector3Float::sUnitX, GMath::GetHalfPI());
		transform = GMatrix16FloatConstructAxisAngle(GVector3Float::sUnitZ, mTime * 0.2F) * transform;
		transform.SetPosition(GVector3Float(0.0F, 5.3F, 0.0F));
		sceneNodeBall->SetParentRelativeTransform(transform);
	}

	if (mParentState.GetScene())
	{
		GScene& scene = *mParentState.GetScene();
		GSceneVisitorAllTick<GSceneNodeComponentAnimation>::Run(scene, in_timeDelta);
		GSceneVisitorAllUpdate<GSceneNodeComponentNodeAttach>::Run(scene);
		GSceneVisitorAllTick<GSceneNodeComponentVisual>::Run(scene, in_timeDelta);
	}

	//turn off teleport flag
	{
		TPointerSceneNode pointerSceneNode = mSceneNodeActor.lock();
		if (pointerSceneNode)
		{
			pointerSceneNode->SetTeleportFlag(false);
		}
	}

	if (0.0F != mCountdownActivateNextResources)
	{
		mCountdownActivateNextResources -= in_timeDelta;
		if (mCountdownActivateNextResources <= 0.0F)
		{
			mCountdownActivateNextResources = 0.0F;
			ResetResources();
		}
	}

	if ((sTimeWithoutInputDemoStart + (2.0F * mTimeWithoutInputCount)) < mTimeWithoutInput)
	{
		mTimeWithoutInputCount += 1;
		if ((4 < mTimeWithoutInputCount) &&
			(GMath::RandomUnit() < 0.25F))
		{
			const BeachVolleyBallType::TGirl::TEnum girl = LocalGetGirlCamera(mCameraType);
			int temp = 0;
			if (GMath::RandomUnit() < 0.33F)
			{
				temp += 1;
			}
			if (GMath::RandomUnit() < 0.33F)
			{
				temp += 1;
			}
			const BeachVolleyBallType::TMatch::TEnum match = (BeachVolleyBallType::TMatch::TEnum)temp;

			mParentState.SetStateRequestData(girl, match);
			mParentState.SetStateRequest(BeachVolleyBallState::GetStateGameDemo());
		}
		else
		{
			ClickButton("ButtonRight");
		}
	}

	return;
}

void BeachVolleyBallMenu::Draw(GRenderComponent& in_renderComponent)
{
	TPointerGui pointerGui = GetActiveGui();
	if (pointerGui)
	{
		GGui& gui = *pointerGui;

		GGuiVisitorUpdate::Run(gui);
		GGuiVisitorRender::Run(gui, in_renderComponent);
	}

	return;
}


void BeachVolleyBallMenu::Resize(const float in_cameraAspect)
{
	mCamera.SetTop(sDefaultCameraPerspectiveBounds);
	mCamera.SetBottom(-sDefaultCameraPerspectiveBounds);
	mCamera.SetLeft(-sDefaultCameraPerspectiveBounds * in_cameraAspect);
	mCamera.SetRight(sDefaultCameraPerspectiveBounds * in_cameraAspect);

	return;
}

void BeachVolleyBallMenu::ClickButton(const std::string& in_name)
{
	bool triggerPlay = false;
	bool triggerWatch = false;
	BeachVolleyBallType::TMatch::TEnum match = BeachVolleyBallType::TMatch::TCount;

	mParentState.PlaySoundMenuClick();

	if (in_name == "ButtonLeft")
	{
		switch (mCameraType)
		{
		default:
		case TCameraType::TGirlOne:
			SetCameraType(TCameraType::TGirlFour);
			break;
		case TCameraType::TGirlTwo:
			SetCameraType(TCameraType::TGirlOne);
			break;
		case TCameraType::TGirlThree:
			SetCameraType(TCameraType::TGirlTwo);
			break;
		case TCameraType::TGirlFour:
			SetCameraType(TCameraType::TGirlThree);
			break;
		}
	}
	else if (in_name == "ButtonRight")
	{
		switch (mCameraType)
		{
		default:
		case TCameraType::TGirlOne:
			SetCameraType(TCameraType::TGirlTwo);
			break;
		case TCameraType::TGirlTwo:
			SetCameraType(TCameraType::TGirlThree);
			break;
		case TCameraType::TGirlThree:
			SetCameraType(TCameraType::TGirlFour);
			break;
		case TCameraType::TGirlFour:
			SetCameraType(TCameraType::TGirlOne);
			break;
		}
	}
	else if (in_name == "ButtonPlay")
	{
		SetMenuType(TMenuState::TPlay);
		mGuiAlpha = 0.0F;
	}
	else if (in_name == "ButtonMatch1")
	{
		if (mMenuState == TMenuState::TPlay)
		{
			triggerPlay = true;
		}
		else if (mMenuState == TMenuState::TWatch)
		{
			triggerWatch = true;
		}
		match = BeachVolleyBallType::TMatch::TOne;
	}
	else if (in_name == "ButtonMatch2")
	{
		if (mMenuState == TMenuState::TPlay)
		{
			triggerPlay = true;
		}
		else if (mMenuState == TMenuState::TWatch)
		{
			triggerWatch = true;
		}
		match = BeachVolleyBallType::TMatch::TTwo;
	}
	else if (in_name == "ButtonMatch3")
	{
		if (mMenuState == TMenuState::TPlay)
		{
			triggerPlay = true;
		}
		else if (mMenuState == TMenuState::TWatch)
		{
			triggerWatch = true;
		}
		match = BeachVolleyBallType::TMatch::TThree;
	}
	else if (in_name == "ButtonWatch")
	{
		SetMenuType(TMenuState::TWatch);
		mGuiAlpha = 0.0F;
		return;
	}
	else if (in_name == "ButtonBack")
	{
		SetMenuType(TMenuState::TMain);
		mGuiAlpha = 0.0F;
		return;
	}
	else if (in_name == "ButtonInfo")
	{
		SetMenuType(TMenuState::TInfo);
		mGuiAlpha = 0.0F;
		return;
	}
	else if (in_name == "ButtonAppStore")
	{
		ClickButtonAppStore();
	}

	if (!triggerPlay &&
		!triggerWatch)
	{
		return;
	}
	const BeachVolleyBallType::TGirl::TEnum girl = LocalGetGirlCamera(mCameraType);

	if (girl == BeachVolleyBallType::TGirl::TCount)
	{
		return;
	}

	if (triggerPlay && mParentState.GetGameVar()->MenuHasPlayMatch(girl, match))
	{
		mParentState.SetStateRequestData(girl, match);
		mParentState.SetStateRequest(BeachVolleyBallState::GetStateGame());
	}
	
	if (triggerWatch && mParentState.GetGameVar()->MenuHasPlayMatch(girl, match))
	{
		mParentState.SetStateRequestData(girl, match);
		mParentState.SetStateRequest(BeachVolleyBallState::GetStateWatch());
	}

	return;
}

//public accessors
GCamera& BeachVolleyBallMenu::GetCamera()
{
	return mCamera;
}

//private methods
void BeachVolleyBallMenu::SetCameraType(const int in_cameraType)
{
	if (mCameraType == in_cameraType)
	{
		return;
	}

	mCameraType = in_cameraType;
	mFadeMenuDown = true;
	mCountdownActivateNextResources = sCameraTransitionTime / 2.0F;
	mLerpCameraPos->SetValue(GVector3Float(
			sArrayCameraData[mCameraType].mPos[0],
			sArrayCameraData[mCameraType].mPos[1],
			sArrayCameraData[mCameraType].mPos[2]
			),
		sCameraTransitionTime,
		GComponentLerp::TType::TEaseInOut
		);
	mLerpCameraAt->SetValue(GVector3Float(
			sArrayCameraData[mCameraType].mAt[0],
			sArrayCameraData[mCameraType].mAt[1],
			sArrayCameraData[mCameraType].mAt[2]
			),
		sCameraTransitionTime,
		GComponentLerp::TType::TEaseInOut
		);
	mLerpCameraUp->SetValue(GVector3Float(
			sArrayCameraData[mCameraType].mUp[0],
			sArrayCameraData[mCameraType].mUp[1],
			sArrayCameraData[mCameraType].mUp[2]
			),
		sCameraTransitionTime,
		GComponentLerp::TType::TEaseInOut
		);

	return;
}

void BeachVolleyBallMenu::SetMenuType(const int in_menuState)
{
	mMenuState = in_menuState;

	switch (mMenuState)
	{
	default:
		break;
	case TMenuState::TMain:
		{
			TPointerGui pointerGui = mGuiMain.lock();
			if (pointerGui)
			{
				const bool hasPlay = mParentState.GetGameVar()->MenuHasPlay(sArrayCameraData[mCameraType].mGirl);
				const bool hasWatch = mParentState.GetGameVar()->MenuHasWatch(sArrayCameraData[mCameraType].mGirl);

				GGuiNode* const nodePlay = GGuiVisitorNodeName::FindNode(*pointerGui, "NodePlay");
				GGuiNode* const nodeWatch = GGuiVisitorNodeName::FindNode(*pointerGui, "NodeWatch");

				if (hasPlay && hasWatch)
				{
					if (nodePlay)
					{
						nodePlay->SetTopLeft(GGuiCoordinate(
							GVector2Float(0.5F, 1.0F), 
							GVector2Float(-127.0F, -124.0F)
							));
					}
					if (nodeWatch)
					{
						nodeWatch->SetTopLeft(GGuiCoordinate(
							GVector2Float(0.5F, 1.0F), 
							GVector2Float(-127.0F, -62.0F)
							));
					}
				}
				else if (hasPlay)
				{
					if (nodePlay)
					{
						nodePlay->SetTopLeft(GGuiCoordinate(
							GVector2Float(0.5F, 1.0F), 
							GVector2Float(-127.0F, -100.0F)
							));
					}
					if (nodeWatch)
					{
						nodeWatch->SetTopLeft(GGuiCoordinate(
							GVector2Float(0.5F, 1.0F), 
							GVector2Float(-127.0F, 0.0F)
							));
					}
				}
				else
				{
					if (nodePlay)
					{
						nodePlay->SetTopLeft(GGuiCoordinate(
							GVector2Float(0.5F, 1.0F), 
							GVector2Float(-127.0F, 0.0F)
							));
					}
					if (nodeWatch)
					{
						nodeWatch->SetTopLeft(GGuiCoordinate(
							GVector2Float(0.5F, 1.0F), 
							GVector2Float(-127.0F, 0.0F)
							));
					}
				}
			}
		}
		break;
	case TMenuState::TPlay:
		{
			TPointerGui pointerGui = mGuiMatch.lock();
			if (pointerGui)
			{
				const bool hasMatch1 = mParentState.GetGameVar()->MenuHasPlayMatch(sArrayCameraData[mCameraType].mGirl, BeachVolleyBallType::TMatch::TOne);
				const bool hasMatch2 = mParentState.GetGameVar()->MenuHasPlayMatch(sArrayCameraData[mCameraType].mGirl, BeachVolleyBallType::TMatch::TTwo);
				const bool hasMatch3 = mParentState.GetGameVar()->MenuHasPlayMatch(sArrayCameraData[mCameraType].mGirl, BeachVolleyBallType::TMatch::TThree);

				LocalSetMatch(
					hasMatch1,
					hasMatch2,
					hasMatch3,
					*pointerGui
					);
			}
		}
		break;
	case TMenuState::TWatch:
		{
			TPointerGui pointerGui = mGuiMatch.lock();
			if (pointerGui)
			{
				const bool hasMatch1 = mParentState.GetGameVar()->MenuHasWatchMatch(sArrayCameraData[mCameraType].mGirl, BeachVolleyBallType::TMatch::TOne);
				const bool hasMatch2 = mParentState.GetGameVar()->MenuHasWatchMatch(sArrayCameraData[mCameraType].mGirl, BeachVolleyBallType::TMatch::TTwo);
				const bool hasMatch3 = mParentState.GetGameVar()->MenuHasWatchMatch(sArrayCameraData[mCameraType].mGirl, BeachVolleyBallType::TMatch::TThree);

				LocalSetMatch(
					hasMatch1,
					hasMatch2,
					hasMatch3,
					*pointerGui
					);
			}
		}
		break;
	}

	return;
}

void BeachVolleyBallMenu::ResetResources()
{
	mParentState.SetDrawShadows(true);

	GScene& scene = *mParentState.GetScene();
	{
		TPointerSceneNode sceneNodeBall = mSceneNodeBall.lock(); 
		scene.SceneNodeRequestDelete(sceneNodeBall);
	}
	{
		TPointerSceneNode sceneNodeActor = mSceneNodeActor.lock(); 
		scene.SceneNodeRequestDelete(sceneNodeActor);
	}
	scene.SceneNodeProcessDeleteQueue();

	if (BeachVolleyBallType::TGirl::TCount != sArrayCameraData[mCameraType].mGirl)
	{
		TArrayScenePostLoadCallback arrayPostLoadCallback;

		TPointerSceneNode sceneNode = mSceneNodeRoot.lock();
		TPointerSceneNode sceneNodeActor = scene.CreateSceneNode("menuActor");
		mSceneNodeActor = sceneNodeActor;
		sceneNode->AddChildNode(sceneNodeActor);
		
		BeachVolleyBallType::TGirl::TEnum girl = sArrayCameraData[mCameraType].mGirl;
		BeachVolleyBallType::TMatch::TEnum match = BeachVolleyBallType::TMatch::TOne;
		BeachVolleyBallGameVar& gameVar = *mParentState.GetGameVar();
		if (gameVar.MenuHasPlayMatch(girl, BeachVolleyBallType::TMatch::TThree))
		{
			match = BeachVolleyBallType::TMatch::TThree;
		}
		else if (gameVar.MenuHasPlayMatch(girl, BeachVolleyBallType::TMatch::TTwo))
		{
			match = BeachVolleyBallType::TMatch::TTwo;
		}

		scene.SceneNodeFactory(
			BeachVolleyBallType::GetFactoryName(girl, match),
			"", 
			sceneNodeActor, 
			arrayPostLoadCallback
			);
		GMatrix16Float transform = GMatrix16FloatConstructAtUp(
			GVector3Float(
				sArrayCameraData[mCameraType].mGirlAt[0],
				sArrayCameraData[mCameraType].mGirlAt[1],
				sArrayCameraData[mCameraType].mGirlAt[2]
				),
			GVector3Float::sUnitY, 
			GVector3Float::sUnitZ, 
			GVector3Float::sUnitY, 
			GVector3Float(
				sArrayCameraData[mCameraType].mGirlPos[0],
				sArrayCameraData[mCameraType].mGirlPos[1],
				sArrayCameraData[mCameraType].mGirlPos[2]
				)
			);
		sceneNodeActor->SetParentRelativeTransform(transform);
		sceneNodeActor->SetTeleportFlag(true);

		GScene::FactoryPostLoad(arrayPostLoadCallback, scene);

		//find all animation components in scene and set there animation to menu, no fade in
		TArrayWeakSceneNodeComponentAnimation arrayComponentAnimation;
		GSceneVisitorAllCollect<GSceneNodeComponentAnimation>::Run(scene, arrayComponentAnimation);

		GAnimationPlayParam animationParam(
			0.75F,//const float in_tickRate,
			0.0F //const float in_fadeInDuration,
			);
		for (TArrayWeakSceneNodeComponentAnimation::iterator iterator = arrayComponentAnimation.begin(); iterator != arrayComponentAnimation.end(); ++iterator)
		{
			TPointerSceneNodeComponentAnimation pointerAnimation = (*iterator).lock();
			if (!pointerAnimation)
			{
				continue;
			}
			GSceneNodeComponentAnimation& componentAnimation = *pointerAnimation;

			TPointerAnimationManagerInstance& animationManager = componentAnimation.GetAnimationManagerInstance();
			if (!animationManager)
			{
				continue;
			}
			animationManager->PlayAnimation("menu", animationParam);
		}
	}

	//menu
	if (BeachVolleyBallType::TGirl::TCount != sArrayCameraData[mCameraType].mGirl)
	{
		mFadeMenuDown = false;
		SetMenuType(TMenuState::TMain);
	}

	return;
}

BeachVolleyBallMenu::TPointerGui BeachVolleyBallMenu::GetActiveGui()
{
	switch (mMenuState)
	{
	default:
		break;
	case TMenuState::TLogo:
		return mGuiLogo.lock();
	case TMenuState::TMain:
		return mGuiMain.lock();
	case TMenuState::TInfo:
		return mGuiInfo.lock();
	case TMenuState::TPlay:
	case TMenuState::TWatch:
		return mGuiMatch.lock();
	}

	return TPointerGui();
}
