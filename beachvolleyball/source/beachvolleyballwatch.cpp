//
//  BeachVolleyBallWatch.cpp
//  BeachVolleyBall
//
//  Created by David Coen on 2011 05 02
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "BeachVolleyBallWatch.h"

#include "BeachVolleyBallState.h"
#include "BeachVolleyBallWatchButtonCallBack.h"

#include <GAnimationManagerInstance.h>
#include <GAnimationPlayParam.h>
#include <GCommon.h>
#include <GInput.h>
#include <GInputTouch.h>
#include <GMath.h>
#include <GScene.h>
#include <GScenePostLoadCallback.h>
#include <GSceneVisitorAll.h>
#include <GSceneNodeComponentAnimation.h>
#include <GSceneNodeComponentNodeAttach.h>
#include <GSceneNodeComponentVisual.h>
#include <GGuiManager.h>
#include <GGuiVisitorUpdate.h>
#include <GGuiVisitorRender.h>
#include <GGuiVisitor.h>
#include <GGuiVisitorInput.h>
#include <GGuiNodeComponentButton.h>
#include <GGuiNodeComponentText.h>
#include <GStringUtility.h>
#include <GMatrix9Float.h>

static const float sDefaultCameraPerspectiveBounds = 0.08F;
static const float sDefaultCameraPerspectiveNear = 0.1F;
static const float sDefaultCameraPerspectiveFar = 1000.0F;

static const float sZoomFactor = 200.0F;
static const float sRotateFactor = 100.0F;

typedef boost::shared_ptr<GGuiNodeComponentButtonCallback> TPointerGuiNodeComponentButtonCallback;
typedef std::vector<GScenePostLoadCallback> TArrayScenePostLoadCallback;
typedef boost::shared_ptr<GGuiNodeComponentButton> TPointerGuiNodeComponentButton;
typedef boost::weak_ptr<GGuiNodeComponentButton> TWeakGuiNodeComponentButton;
typedef std::vector<TWeakGuiNodeComponentButton> TArrayWeakGuiNodeComponentButton;

typedef boost::shared_ptr<GGuiNodeComponentText> TPointerGuiNodeComponentText;
typedef boost::weak_ptr<GGuiNodeComponentText> TWeakGuiNodeComponentText;
typedef std::vector<TWeakGuiNodeComponentText> TArrayWeakGuiNodeComponentText;

typedef boost::shared_ptr<GAnimationManagerInstance> TPointerAnimationManagerInstance;

struct TAnimationWatch
{
	enum TEnum
	{
		TMenu = 0,
		TIdle,
		TWin,
		TLoose,
		TCount
	};
};

struct TAnimationWatchData
{
	const char* const mName;
	const char* const mPrettyName;
	const float mRate;
};

static const TAnimationWatchData sArrayAnimationWatchData[TAnimationWatch::TCount] = {
	//TMenu,
	{ "menu", "Pose One", 0.75F },
	//TIdle,
	{ "idle", "Pose Two", 1.0F },
	//TWin,
	{ "wingame", "Victory", 0.75F },
	//TLoose,
	{ "loosegame", "Defeat", 0.75F }
};

static const float sDefaultAnimationWatchFadeTime = 6.0F / 30.0F;

//constructor
BeachVolleyBallWatch::BeachVolleyBallWatch(
	BeachVolleyBallState& inout_parentState, 
	const float in_cameraAspect,
	const BeachVolleyBallType::TGirl::TEnum in_girl,
	const BeachVolleyBallType::TMatch::TEnum in_match
	)
	: mParentState(inout_parentState)
	, mCamera(
		GCamera::TType::TPerspective,
		sDefaultCameraPerspectiveNear,
		sDefaultCameraPerspectiveFar,
		sDefaultCameraPerspectiveBounds,
		-sDefaultCameraPerspectiveBounds,
		-(sDefaultCameraPerspectiveBounds * in_cameraAspect),
		sDefaultCameraPerspectiveBounds * in_cameraAspect	
		)
	, mSceneNodeRoot()
	, mActorAnimation()
	, mAnimationId(TAnimationWatch::TCount)
	, mGuiFadeIn(0.0F)
	, mFadeDownText(5.0F)
	, mGui()
	, mTouchDelta()
	, mZoomDelta(0.0F)
	, mCameraOrigin(0.0F, 1.0F, 0.0F)
	, mZoom(1.5F)
	, mLatitude(0.0F)
	, mLongitude(0.0F)
{
	inout_parentState.SetWorldFacing(BeachVolleyBallState::GetWorldFacingTypeGame(in_girl));
	inout_parentState.SetDrawShadows(true);

	if (inout_parentState.GetScene())
	{
		GScene& scene = *inout_parentState.GetScene();
		TPointerSceneNode sceneNode = scene.CreateSceneNode("watchRoot");
		sceneNode->SetTeleportFlag(true);
		scene.AddSceneRoot(sceneNode);
		mSceneNodeRoot = sceneNode;

		TArrayScenePostLoadCallback arrayPostLoadCallback;
		scene.SceneNodeFactory(
			BeachVolleyBallType::GetFactoryName(in_girl, in_match),
			"", 
			sceneNode, 
			arrayPostLoadCallback
			);

		GScene::FactoryPostLoad(arrayPostLoadCallback, scene);

		const int componentCount = sceneNode->GetComponentCount();
		for (int index = 0; index < componentCount; ++index)
		{
			TPointerSceneNodeComponentAnimation pointerAnimation = sceneNode->GetComponentPointer<GSceneNodeComponentAnimation>(index);
			if (pointerAnimation)
			{
				mActorAnimation = pointerAnimation;
			}
		}
	}

	mGui = mParentState.GetGuiManager()->LoadGui("guiwatch.data");

	TPointerGui pointerGui = mGui.lock();
	if (pointerGui)
	{
		GGui& gui = *pointerGui;

		TArrayWeakGuiNodeComponentText arrayText;
		GGuiVisitor<GGuiNodeComponentText>::Run(gui, arrayText);

		for (TArrayWeakGuiNodeComponentText::iterator iterator = arrayText.begin(); iterator != arrayText.end(); ++iterator)
		{
			TPointerGuiNodeComponentText pointerTest = (*iterator).lock();
			if (!pointerTest)
			{
				continue;
			}
			GGuiNodeComponentText& text = *pointerTest;
			if (text.GetName() == "TextMessage01")
			{
				mMessageText = (*iterator);
			}
			else if (text.GetName() == "Text09")
			{
				mAnimationText = (*iterator);
			}
		}


		TPointerGuiNodeComponentButtonCallback buttonCallback;
		buttonCallback.reset(new BeachVolleyBallWatchButtonCallBack(*this));

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
			button.SetCallback(buttonCallback);
		}
	}

	SetAnimation(TAnimationWatch::TMenu, 0.0F);
	UpdateCamera();

	return;
}

BeachVolleyBallWatch::~BeachVolleyBallWatch()
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
		mParentState.GetGuiManager()->DestroyGui(mGui);
	}

	return;
}

//public methods
void BeachVolleyBallWatch::Input(const GInput& in_input)
{
	TPointerGui pointerGui = mGui.lock();
	if (pointerGui)
	{
		GGui& gui = *pointerGui;
		GGuiVisitorInput::Run(gui, in_input);
	}

	const int touchCount = in_input.GetTouchCount();
	for (int index = 0; index < touchCount; ++index)
	{
		const GInputTouch& touch = in_input.GetTouch(index);
		mTouchDelta -= touch.GetTouchDelta();

		for (int subIndex = index + 1; subIndex < touchCount; ++subIndex)
		{
			const GInputTouch& subTouch = in_input.GetTouch(subIndex);
			const float prevLength = LengthAprox(
				(subTouch.GetTouchLocation() - subTouch.GetTouchDelta()) - 
				(touch.GetTouchLocation() - touch.GetTouchDelta())
				);
			const float length = LengthAprox(subTouch.GetTouchLocation() - touch.GetTouchLocation());

			mZoom += (prevLength - length);
		}
	}

	FlushInput(in_input);
	
	return;
}

void BeachVolleyBallWatch::Tick(const float in_timeDelta)
{
	if (mGuiFadeIn < 1.0f)
	{
		mGuiFadeIn = GMath::Minimum(1.0F, mGuiFadeIn + in_timeDelta);

		mParentState.GetGuiManager()->SetGuiColour(GColour4Float(1.0F, 1.0F, 1.0F, mGuiFadeIn));
	}
	if (0.0F < mFadeDownText)
	{
		mFadeDownText -= in_timeDelta;
		TPointerGuiNodeComponentText pointerText = mMessageText.lock();
		if (pointerText)
		{
			GGuiNodeComponentText& text = *pointerText;
			if (mFadeDownText < 0.0F)
			{
				text.SetVisible(false);
			}
			else if (mFadeDownText < 1.0F)
			{
				text.SetAlpha(mFadeDownText);
			}
		}
	}

	//tick the scene
	if (mParentState.GetScene())
	{
		GScene& scene = *mParentState.GetScene();
		GSceneVisitorAllTick<GSceneNodeComponentAnimation>::Run(scene, in_timeDelta);
		GSceneVisitorAllUpdate<GSceneNodeComponentNodeAttach>::Run(scene);

		GSceneVisitorAllTick<GSceneNodeComponentVisual>::Run(scene, in_timeDelta);
	}

	//tick camera
	{
		mZoom = GMath::Clamp(mZoom + (mZoomDelta * in_timeDelta * sZoomFactor), 1.0F, 4.0F);
		mZoomDelta = 0.0F;
		mLatitude = GMath::ClampRadian(mLatitude + (mTouchDelta.GetX() * in_timeDelta * sRotateFactor));
		mLongitude = GMath::Clamp(mLongitude + (mTouchDelta.GetY() * in_timeDelta * sRotateFactor), -1.4F, 0.2F);
		mTouchDelta = GVector2Float::sZero;

		UpdateCamera();
	}

	//scene node
	TPointerSceneNode sceneRoot = mSceneNodeRoot.lock();
	if (sceneRoot)
	{
		sceneRoot->SetTeleportFlag(false);
	}

	return;
}


void BeachVolleyBallWatch::Draw(GRenderComponent& in_renderComponent)
{
	TPointerGui pointerGui = mGui.lock();
	if (pointerGui)
	{
		GGui& gui = *pointerGui;

		GGuiVisitorUpdate::Run(gui);
		GGuiVisitorRender::Run(gui, in_renderComponent);
	}

	return;
}

void BeachVolleyBallWatch::Resize(const float in_cameraAspect)
{
	mCamera.SetTop(sDefaultCameraPerspectiveBounds);
	mCamera.SetBottom(-sDefaultCameraPerspectiveBounds);
	mCamera.SetLeft(-sDefaultCameraPerspectiveBounds * in_cameraAspect);
	mCamera.SetRight(sDefaultCameraPerspectiveBounds * in_cameraAspect);

	return;
}

void BeachVolleyBallWatch::ClickButton(const std::string& in_name)
{
	mParentState.PlaySoundMenuClick();

	if (in_name == "ButtonBack")
	{
		mParentState.SetStateRequest(BeachVolleyBallState::GetStateMenu());
		return;
	}
	else if (in_name == "ButtonLeft")
	{
		mAnimationId -= 1;
		if (mAnimationId < 0)
		{
			mAnimationId += TAnimationWatch::TCount;
		}
		SetAnimation(mAnimationId, sDefaultAnimationWatchFadeTime);
	}
	else if (in_name == "ButtonRight")
	{
		SetAnimation((mAnimationId + 1) % TAnimationWatch::TCount, sDefaultAnimationWatchFadeTime);
	}

	return;
}

//private methods
void BeachVolleyBallWatch::UpdateCamera()
{
	GVector3Float at(-GVector3Float::sUnitZ);
	GVector3Float up(GVector3Float::sUnitY);

	{
		const GMatrix9Float rotate = 
			GMatrix9FloatConstructAxisAngle(GVector3Float::sUnitX, mLongitude) *
			GMatrix9FloatConstructAxisAngle(GVector3Float::sUnitY, mLatitude);
		at = at * rotate;
		up = up * rotate;
	}

	const GVector3Float position = mCameraOrigin - (at * mZoom);

	mCamera.SetTransform(
		at,
		up,
		position
		);

	return;
}

void BeachVolleyBallWatch::SetAnimation(const int in_animationId, const float in_fadeTime)
{
	mAnimationId = in_animationId;

	TPointerSceneNodeComponentAnimation pointerAnimation = mActorAnimation.lock();
	if (!pointerAnimation)
	{
		return;
	}
	TPointerAnimationManagerInstance& pointerAnimationManagerInstance = pointerAnimation->GetAnimationManagerInstance();
	if (!pointerAnimationManagerInstance)
	{
		return;
	}

	GAnimationManagerInstance& animationManagerInstance = *pointerAnimationManagerInstance;
	GAnimationPlayParam playParam(
		sArrayAnimationWatchData[mAnimationId].mRate,
		in_fadeTime
		);

	animationManagerInstance.PlayAnimation(
		sArrayAnimationWatchData[mAnimationId].mName,
		playParam
		);

	TPointerGuiNodeComponentText pointerText = mAnimationText.lock();
	if (pointerText)
	{
		GGuiNodeComponentText& text = *pointerText;
		text.SetVisible(true);
		text.SetText(sArrayAnimationWatchData[mAnimationId].mPrettyName);
	}

	return;
}


//hack. there is a situation where iOS seems to loose touch events, so clear the input
void BeachVolleyBallWatch::FlushInput(const GInput& in_input)
{
	GInput& input = (GInput&)in_input;
	input.SetTouchCount(0);
	return;
}
