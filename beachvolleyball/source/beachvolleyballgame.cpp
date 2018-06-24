//
//  BeachVolleyBallGame.cpp
//  BeachVolleyBall
//
//  Created by David Coen on 2011 04 05
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "BeachVolleyBallGame.h"

#include "BeachVolleyBallState.h"
#include "BeachVolleyBallClientBall.h"
#include "BeachVolleyBallClientOpponent.h"
#include "BeachVolleyBallGameButtonCallBack.h"
#include "BeachVolleyBallGameVar.h"
#include "BeachVolleyBallSound.h"
#include "BeachVolleyBallMenu.h"

#include <GCommon.h>
#include <GInput.h>
#include <GInputTouch.h>
#include <GMath.h>
#include <GScene.h>
#include <GScenePostLoadCallback.h>
#include <GSceneVisitorAll.h>
#include <GGuiNodeComponentButton.h>
#include <GSceneNodeComponentAnimation.h>
#include <GSceneNodeComponentNodeAttach.h>
#include <GSceneNodeComponentVisual.h>
#include <GGuiNodeComponentButtonCallback.h>
#include <GGuiManager.h>
#include <GGuiVisitorUpdate.h>
#include <GGuiVisitorRender.h>
#include <GGuiVisitor.h>
#include <GGuiVisitorInput.h>
#include <GGuiNodeComponentText.h>
#include <GStringUtility.h>
#include <GMatrix16Float.h>
#include <map>

//#define MORPH_TEST

static const float sDefaultCameraPerspectiveBounds = 0.08F;
static const float sDefaultCameraPerspectiveNear = 0.1F;
static const float sDefaultCameraPerspectiveFar = 1000.0F;
static const float sWorldSpaceTouchRadius = 0.05F;
static const float sMaxHitHorizontalServe = 2.5F;
static const float sMaxHitHorizontal = 5.5F;
static const int sPointsToWin = 15; //21;
static const int sPointsDifferenceWin = 2;

typedef boost::shared_ptr<GSceneNodeComponentBase> TPointerSceneNodeComponentBase;
typedef std::vector<GScenePostLoadCallback> TArrayScenePostLoadCallback;
typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;
typedef TPointerSceneNodeComponentBase (*TComponentFactoryClientCallback)(
	GScene& inout_scene, //access to mesh/ material manager
	TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
	void* const in_data
	);
typedef std::pair<TComponentFactoryClientCallback, void*> TPairFactoryClientCallbackData;
typedef std::map<std::string, TPairFactoryClientCallbackData> TMapStringPairFactoryClientCallbackData;

typedef boost::shared_ptr<GGuiNodeComponentText> TPointerGuiNodeComponentText;
typedef boost::weak_ptr<GGuiNodeComponentText> TWeakGuiNodeComponentText;
typedef std::vector<TWeakGuiNodeComponentText> TArrayWeakGuiNodeComponentText;

typedef boost::shared_ptr<GGuiNodeComponentButtonCallback> TPointerGuiNodeComponentButtonCallback;
typedef boost::shared_ptr<GGuiNodeComponentButton> TPointerGuiNodeComponentButton;
typedef boost::weak_ptr<GGuiNodeComponentButton> TWeakGuiNodeComponentButton;
typedef std::vector<TWeakGuiNodeComponentButton> TArrayWeakGuiNodeComponentButton;

struct TState
{
	enum TEnum
	{
		TIntro = 0,
		TPlayerServe,
		TOpponentServe,
		TGameAfterPlayerHit,
		TGameAfterOpponentHit,
		TBallTouchedGround,
		TPlayerFoul,
		TGameOver,
		TGameOverWaitTouch,
		TGameOverFree,

		TCount
	};
};

struct TScoreState
{
	enum TEnum
	{
		TPlayerServe = 0,
		TOpponentServe,

		TCount
	};
};

static void LocalUpdateScore(
	TWeakGuiNodeComponentText& in_scoreText, 
	const int in_score
	)
{
	TPointerGuiNodeComponentText pointerText = in_scoreText.lock();
	if (!pointerText)
	{
		return;
	}

	GGuiNodeComponentText& text = *pointerText;
	const std::string string = GStringUtility::PrintMethod02(in_score);
	text.SetText(string);

	return;
}


//constructor
BeachVolleyBallGame::BeachVolleyBallGame(
	BeachVolleyBallState& inout_parentState, 
	const float in_cameraAspect,
	const BeachVolleyBallType::TGirl::TEnum in_girl,
	const BeachVolleyBallType::TMatch::TEnum in_match,
	const bool in_demoMode
	)
: mGirl(in_girl)
, mMatch(in_match)
, mState(TState::TIntro)
, mScoreState(TScoreState::TPlayerServe)
, mScorePlayer(0)
, mScoreOpponent(0)
, mStateCountdown(1.0F)
, mMessageCountdown(0.0F)
, mParentState(inout_parentState)
, mCamera(
	GCamera::TType::TPerspective,
	sDefaultCameraPerspectiveNear,
	sDefaultCameraPerspectiveFar,
	sDefaultCameraPerspectiveBounds,
	-sDefaultCameraPerspectiveBounds,
	-(sDefaultCameraPerspectiveBounds * in_cameraAspect),
	sDefaultCameraPerspectiveBounds * in_cameraAspect
	)
, mClientOpponent()
, mClientBall()
, mSceneNodeRoot()
, mHasScreenTouch(false)
, mMultiTouchFoul(false)
, mFlushInput(true)
, mDemoMode(in_demoMode)
, mScreenTouch()
, mScreenTouchDelta()
, mRallyCount(0)
{
	inout_parentState.SetWorldFacing(BeachVolleyBallState::GetWorldFacingTypeGame(in_girl));
	inout_parentState.SetDrawShadows(true);

	inout_parentState.GetSoundManager()->SetContext(BeachVolleyBallSound::GetContextMatch(in_girl, in_match));

	if (inout_parentState.GetScene())
	{
		GScene& scene = *inout_parentState.GetScene();
		TPointerSceneNode sceneNode = scene.CreateSceneNode("gameRoot");
		scene.AddSceneRoot(sceneNode);
		mSceneNodeRoot = sceneNode;

		TMapStringPairFactoryClientCallbackData& factoryClientCallbackData = scene.GetClientComponentFactory();
		factoryClientCallbackData.clear();

		factoryClientCallbackData["clientball"] = TPairFactoryClientCallbackData(BeachVolleyBallClientBall::Factory, this);
		factoryClientCallbackData["clientbikini0"] = TPairFactoryClientCallbackData(BeachVolleyBallClientOpponent::FactoryBikini0, this);
		factoryClientCallbackData["clientbikini1"] = TPairFactoryClientCallbackData(BeachVolleyBallClientOpponent::FactoryBikini1, this);
		factoryClientCallbackData["clientbikini2"] = TPairFactoryClientCallbackData(BeachVolleyBallClientOpponent::FactoryBikini2, this);
		factoryClientCallbackData["clientreiko0"] = TPairFactoryClientCallbackData(BeachVolleyBallClientOpponent::FactoryReiko0, this);
		factoryClientCallbackData["clientreiko1"] = TPairFactoryClientCallbackData(BeachVolleyBallClientOpponent::FactoryReiko1, this);
		factoryClientCallbackData["clientreiko2"] = TPairFactoryClientCallbackData(BeachVolleyBallClientOpponent::FactoryReiko2, this);
		factoryClientCallbackData["clientapril0"] = TPairFactoryClientCallbackData(BeachVolleyBallClientOpponent::FactoryApril0, this);
		factoryClientCallbackData["clientapril1"] = TPairFactoryClientCallbackData(BeachVolleyBallClientOpponent::FactoryApril1, this);
		factoryClientCallbackData["clientapril2"] = TPairFactoryClientCallbackData(BeachVolleyBallClientOpponent::FactoryApril2, this);
		factoryClientCallbackData["clientcuda0"] = TPairFactoryClientCallbackData(BeachVolleyBallClientOpponent::FactoryCuda0, this);
		factoryClientCallbackData["clientcuda1"] = TPairFactoryClientCallbackData(BeachVolleyBallClientOpponent::FactoryCuda1, this);
		factoryClientCallbackData["clientcuda2"] = TPairFactoryClientCallbackData(BeachVolleyBallClientOpponent::FactoryCuda2, this);

		TArrayScenePostLoadCallback arrayPostLoadCallback;
		{
			TPointerSceneNode sceneNodeBall = scene.CreateSceneNode("menuBall");
			sceneNode->AddChildNode(sceneNodeBall);

			GMatrix16Float transform(GMatrix16Float::sIdentity);
			transform.SetPosition(GVector3Float(0.0F, 0.0F, -1.0F));
			sceneNodeBall->SetParentRelativeTransform(transform);

			scene.SceneNodeFactory("factoryball", "", sceneNodeBall, arrayPostLoadCallback);
		}
		{
			TPointerSceneNode sceneNodeActor = scene.CreateSceneNode("menuActor");
			sceneNode->AddChildNode(sceneNodeActor);

			GMatrix16Float transform(GMatrix16Float::sIdentity);
			transform.SetPosition(GVector3Float(0.0F, 0.0F, 6.5F));
			sceneNodeActor->SetParentRelativeTransform(transform);

			scene.SceneNodeFactory(
				BeachVolleyBallType::GetFactoryName(in_girl, in_match),
				"", 
				sceneNodeActor, 
				arrayPostLoadCallback
				);
		}

		GScene::FactoryPostLoad(arrayPostLoadCallback, scene);
	}

	mCamera.SetTransform(
		GVector3Float(0.0F, -0.107F, -0.994F),
		GVector3Float(0.0F, 0.994F, -0.107F),
		GVector3Float(0.0F, 1.602F, 8.867F)
		);
	mParentState.GetSoundManager()->SetContextPosition(
		mCamera.GetTransformPos(),
		mCamera.GetTransformAt(),
		mCamera.GetTransformUp(),
		GVector3Float::sZero
		);

	mParentState.GetGuiManager()->SetGuiColour(GColour4Float(1.0F, 1.0F, 1.0F, 1.0F));
	mGuiScore = mParentState.GetGuiManager()->LoadGui("guiscore.data");

	TPointerGuiNodeComponentButtonCallback pointerButtonCallback;
	pointerButtonCallback.reset(new BeachVolleyBallGameButtonCallBack(*this));

	TPointerGui pointerGui = mGuiScore.lock();
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
			if (text.GetName() == "TextPlayerScore")
			{
				mPlayerScoreText = (*iterator);
			}
			else if (text.GetName() == "TextOpponentScore")
			{
				mOpponentScoreText = (*iterator);
			}
			else if (text.GetName() == "TextMessage")
			{
				mMessageText = (*iterator);
			}
			else if (text.GetName() == "TextWin")
			{
				mMessageTextWin = (*iterator);
			}
		}

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
			button.SetCallback(pointerButtonCallback);
		}
	}

#ifdef FREE
	mGuiThanks = mParentState.GetGuiManager()->LoadGui("guiinfothanks.data");
	TPointerGui pointerGuiThanks = mGuiThanks.lock();
	if (pointerGuiThanks)
	{
		GGui& gui = *pointerGuiThanks;
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
			button.SetCallback(pointerButtonCallback);
		}
	}
#endif

	if (mDemoMode)
	{
		TPointerGuiNodeComponentText pointerMessageText = mMessageText.lock();
		if (pointerMessageText)
		{
			GGuiNodeComponentText& messageText = *pointerMessageText;

			messageText.SetText("  Touch to exit Demo");
			messageText.SetVisible(true);
			mMessageCountdown = 0.0F;
		}
	}

	return;
}

BeachVolleyBallGame::~BeachVolleyBallGame()
{
	if (mParentState.GetScene())
	{
		GScene& scene = *mParentState.GetScene();
		TPointerSceneNode sceneNode = mSceneNodeRoot.lock();
		scene.SceneNodeRequestDelete(sceneNode);
		scene.SceneNodeProcessDeleteQueue();

		TMapStringPairFactoryClientCallbackData& factoryClientCallbackData = scene.GetClientComponentFactory();
		factoryClientCallbackData.clear();
	}
	if (mParentState.GetGuiManager())
	{
		mParentState.GetGuiManager()->DestroyGui(mGuiScore);

#ifdef FREE
		mParentState.GetGuiManager()->DestroyGui(mGuiThanks);
#endif
	}

	return;
}

//public methods
void BeachVolleyBallGame::Input(const GInput& in_input)
{
	mHasScreenTouch = false;
	mMultiTouchFoul = false;
#ifdef FREE
	TPointerGui pointerGui;
	if (mState == TState::TGameOverFree)
	{
		pointerGui = mGuiThanks.lock();
	}
	else
	{
		pointerGui = mGuiScore.lock();
	}
#else
	TPointerGui pointerGui = mGuiScore.lock();
#endif
	if (pointerGui)
	{
		GGui& gui = *pointerGui;
		GGuiVisitorInput::Run(gui, in_input);
	}

	if ((mState == TState::TGameOverWaitTouch) ||
		(mDemoMode))
	{
		if (in_input.GetTouchCount())
		{
			mParentState.SetStateRequest(BeachVolleyBallState::GetStateMenu());
		}	
		return;
	}
	else if ((mState == TState::TGameOver) || 
		(mState == TState::TGameOverFree))
	{
		return;
	}

	//try to hit ball
	if (1 == in_input.GetTouchCount())
	{
		const GInputTouch& touch = in_input.GetTouch(0);
		mScreenTouch = touch.GetTouchLocation();
		mScreenTouchDelta = touch.GetTouchDelta();
		mHasScreenTouch = true;
	}
	else if (1 < in_input.GetTouchCount())
	{
		mMultiTouchFoul = true;
		mFlushInput = true;
	}
	
	if (mFlushInput)
	{
		mFlushInput = false;
		FlushInput(in_input);
	}
	
	return;
}

void BeachVolleyBallGame::Tick(const float in_timeDelta)
{
	if (0.0F < mMessageCountdown)
	{
		mMessageCountdown -= in_timeDelta;
		if (mMessageCountdown <= 0.0F)
		{
			mMessageCountdown = 0.0F;
			TPointerGuiNodeComponentText messageText = mMessageText.lock();
			if (messageText)
			{
				messageText->SetVisible(false);
			}
		}
	}

	switch (mState)
	{
	default:
		break;
	case TState::TGameOver:
		mStateCountdown -= in_timeDelta;
		if (mStateCountdown < 0.0F)
		{
			if (mDemoMode)
			{
				mParentState.SetStateRequest(BeachVolleyBallState::GetStateMenu());
				return;
			}
			mState = TState::TGameOverWaitTouch;
			TPointerGuiNodeComponentText pointerMessageText = mMessageText.lock();
			if (pointerMessageText)
			{
				GGuiNodeComponentText& messageText = *pointerMessageText;

				messageText.SetText("  Touch to continue");
				messageText.SetVisible(true);
			}

		}
		break;
	case TState::TPlayerFoul:
		mStateCountdown -= in_timeDelta;
		if (mStateCountdown < 0.0F)
		{
			mScoreState = TScoreState::TOpponentServe;
			SetStateOpponentServe();
		}
		break;

	case TState::TIntro:
	case TState::TBallTouchedGround:
		mStateCountdown -= in_timeDelta;
		if (mStateCountdown < 0.0F)
		{
			if (TScoreState::TPlayerServe == mScoreState)
			{
				SetStatePlayerServe();
			}
			else
			{
				SetStateOpponentServe();
			}
		}
		break;
	}

	bool detectHitBall = false;
	switch (mState)
	{
	default:
		break;
	case TState::TPlayerServe:
	case TState::TGameAfterOpponentHit:
		detectHitBall = true;
		break;
	}

	if (mDemoMode && 
		detectHitBall &&
		(GMath::RandomUnit() < (in_timeDelta * 2.0F)))
	{
		mHasScreenTouch = true;

	}

	if (mMultiTouchFoul && detectHitBall)
	{
		mHasScreenTouch = false;
		MultiTouchFoul();
	} 
	else if (mHasScreenTouch && detectHitBall)
	{
		mHasScreenTouch = false;
		HitBall(in_timeDelta);
	}
	mHasScreenTouch = false;
	mHasScreenTouch = false;

	//tick the scene
	if (mParentState.GetScene())
	{
		GScene& scene = *mParentState.GetScene();
		GSceneVisitorAllTick<GSceneNodeComponentClient>::Run(scene,in_timeDelta);
		GSceneVisitorAllTick<GSceneNodeComponentAnimation>::Run(scene, in_timeDelta);
		GSceneVisitorAllUpdate<GSceneNodeComponentNodeAttach>::Run(scene);

#ifdef MORPH_TEST
		//if you want to manually set morph weight, need to do it after the animation update, 
		// else change the default value in the animation instance?
		TPointerBeachVolleyBallClientOpponent pointerClientOpponent = mClientOpponent.lock();
		if (pointerClientOpponent)
		{
			pointerClientOpponent->DoMorphTest(((float)(mRallyCount) * 0.05F));
			//pointerClientOpponent->DoMorphTest(1.0F);
		}
#endif

		GSceneVisitorAllTick<GSceneNodeComponentVisual>::Run(scene, in_timeDelta);
	}

	return;
}


void BeachVolleyBallGame::Draw(GRenderComponent& in_renderComponent)
{
#ifdef FREE
	TPointerGui pointerGui;
	if (mState == TState::TGameOverFree)
	{
		pointerGui = mGuiThanks.lock();
	}
	else
	{
		pointerGui = mGuiScore.lock();
	}
#else
	TPointerGui pointerGui = mGuiScore.lock();
#endif

	if (pointerGui)
	{
		GGui& gui = *pointerGui;

		GGuiVisitorUpdate::Run(gui);
		GGuiVisitorRender::Run(gui, in_renderComponent);
	}

	return;
}

void BeachVolleyBallGame::Resize(const float in_cameraAspect)
{
	mCamera.SetTop(sDefaultCameraPerspectiveBounds);
	mCamera.SetBottom(-sDefaultCameraPerspectiveBounds);
	mCamera.SetLeft(-sDefaultCameraPerspectiveBounds * in_cameraAspect);
	mCamera.SetRight(sDefaultCameraPerspectiveBounds * in_cameraAspect);

	return;
}

void BeachVolleyBallGame::NotifyBallTouchedGround()
{
	if ((TState::TGameAfterPlayerHit == mState) ||
		(TState::TGameAfterOpponentHit == mState))
	{
		const bool playerLastTouch = (TState::TGameAfterPlayerHit == mState);

		ScorePoint(playerLastTouch);
	}

	return;
}

void BeachVolleyBallGame::NotifyOpponentHitBall()
{
	mRallyCount += 1;
	mState = TState::TGameAfterOpponentHit;
	return;
}

void BeachVolleyBallGame::ClickButton(const std::string& in_buttonName)
{
	mParentState.PlaySoundMenuClick();

	if (in_buttonName == "ButtonBack")
	{
		mParentState.SetStateRequest(BeachVolleyBallState::GetStateMenu());
	}
	else if (in_buttonName == "ButtonAppStore")
	{
		BeachVolleyBallMenu::ClickButtonAppStore();
	}
	return;
}

void BeachVolleyBallGame::Shake()
{
	SetStatePlayerServe();
	ScorePoint(true);
	return;
}


//public accessors
void BeachVolleyBallGame::SetClientBall(TPointerBeachVolleyBallClientBall& in_clientBall)
{
	mClientBall = in_clientBall;
	return;
}

void BeachVolleyBallGame::SetClientOpponent(TPointerBeachVolleyBallClientOpponent& in_clientOpponent)
{
	mClientOpponent = in_clientOpponent;
	return;
}

const GVector3Float BeachVolleyBallGame::GetPlayerPosition()const
{
	const GVector3Float& cameraPosition = mCamera.GetTransformPos();
	return GVector3Float(
		cameraPosition.GetX(),
		0.0F,
		cameraPosition.GetZ()
		);
}

BeachVolleyBallGame::TPointerBeachVolleyBallClientBall BeachVolleyBallGame::GetBall() //because opponent want to know ball pos, tell ball about hit
{
	return mClientBall.lock();
}

BeachVolleyBallGame::TPointerBeachVolleyBallSound& BeachVolleyBallGame::GetSoundManager()
{
	return mParentState.GetSoundManager();
}

//private methods
void BeachVolleyBallGame::SetStatePlayerServe()
{
	mRallyCount = 0;
	mScoreState = TScoreState::TPlayerServe;
	mState = TState::TPlayerServe;
	TPointerBeachVolleyBallClientBall pointerBall = mClientBall.lock();
	if (pointerBall)
	{
		pointerBall->SetPlayerServe(
			GVector3Float(0.227F, 1.592F, 8.0F), 
			//GVector3Float(0.227F, 1.25F, 8.0F), 
			GVector3Float::sUnitY ,
			GVector3Float::sUnitZ
			);
	}

	TPointerBeachVolleyBallClientOpponent pointerOpponent = mClientOpponent.lock();
	if (pointerOpponent)
	{
		pointerOpponent->NotifyPlayerServe();
	}

	TPointerGuiNodeComponentText pointerMessageText = mMessageText.lock();
	if (pointerMessageText && !mDemoMode)
	{
		GGuiNodeComponentText& messageText = *pointerMessageText;

		messageText.SetText(" Touch ball to serve");
		messageText.SetVisible(true);
		mMessageCountdown = 0.0F;
	}

	return;
}

void BeachVolleyBallGame::SetStateOpponentServe()
{
	mRallyCount = 0;
	mScoreState = TScoreState::TOpponentServe;
	mState = TState::TOpponentServe;

	TPointerBeachVolleyBallClientOpponent pointerOpponent = mClientOpponent.lock();
	if (pointerOpponent)
	{
		pointerOpponent->NotifyOpponentServe();
	}

	TPointerGuiNodeComponentText pointerMessageText = mMessageText.lock();
	if (pointerMessageText && !mDemoMode)
	{
		GGuiNodeComponentText& messageText = *pointerMessageText;

		messageText.SetText("       Get Ready");
		messageText.SetVisible(true);
		mMessageCountdown = 1.0F;
	}

	return;
}

void BeachVolleyBallGame::SetStateGameOver()
{
	mState = TState::TGameOver;
	mStateCountdown = 3.0F;

	mCamera.SetTransform(
		GVector3Float(0.0F, 0.0F, -1.0F),
		GVector3Float(0.0F, 1.0F, 0.0F),
		GVector3Float(0.0F, 1.067F, 7.577F)
		);

	if (mScorePlayer < mScoreOpponent)
	{
		//girl wins
		TPointerGuiNodeComponentText pointerMessageTextWin = mMessageTextWin.lock();
		if (pointerMessageTextWin)
		{
			GGuiNodeComponentText& messageTextWin = *pointerMessageTextWin;
			messageTextWin.SetText("You Loose");
			messageTextWin.SetVisible(true);
		}
	}
	else
	{
		//player wins
#ifdef FREE
		if (mMatch == BeachVolleyBallType::TMatch::TThree)
		{
			mState = TState::TGameOverFree;
			return;
		}
#endif
		TPointerGuiNodeComponentText pointerMessageTextWin = mMessageTextWin.lock();
		if (pointerMessageTextWin)
		{
			GGuiNodeComponentText& messageTextWin = *pointerMessageTextWin;
			messageTextWin.SetText(" You Win");
			messageTextWin.SetVisible(true);
		}
	}

	return;
}

void BeachVolleyBallGame::HitBall(const float in_timeDelta)
{
	if (0.0F == in_timeDelta)
	{
		return;
	}

	TPointerBeachVolleyBallClientBall pointerBall = mClientBall.lock();
	if (!pointerBall)
	{
		return;
	}

	BeachVolleyBallClientBall& ball = *pointerBall; 
	const GVector3Float& ballPosition = ball.GetPosition();
	const GVector3Float& ballVelocity = ball.GetVelocity();

	if (0.0F < ballVelocity.GetY())
	{
		//allowed to touch ball if it is still or traveling down
		return;
	}

	bool hit = false;
	GVector3Float hitDirection;

	if (mDemoMode)
	{
		hit = true;
		hitDirection = Normalise(GVector3Float(GMath::RandomUnitPlusMinius(), GMath::RandomUnitPlusMinius(), GMath::RandomUnitPlusMinius()));
		mScreenTouchDelta.SetX(GMath::RandomUnitPlusMinius() * in_timeDelta * 10.0F);
		mScreenTouchDelta.SetY(GMath::RandomUnitPlusMinius() * in_timeDelta * 10.0F);
	}
	else
	{
		GVector3Float touchClosestToBall;
		GVector3Float ballClosestToTouch;
		SegmentSegmentClosestPointOfApproach(
			touchClosestToBall,
			ballClosestToTouch,
			mCamera.ReverseProjectPoint(mScreenTouch, ballPosition),
			mCamera.ReverseProjectPoint(mScreenTouch - mScreenTouchDelta, ballPosition),
			ballPosition,
			ballPosition - (ballVelocity * in_timeDelta)
			);

		const float distanceSquared = LengthSquared(touchClosestToBall - ballClosestToTouch);
		const float maxDistanceSquared = GMath::Square(ball.GetRadius() + sWorldSpaceTouchRadius);

		if (distanceSquared < maxDistanceSquared)
		{
			hit = true;
			hitDirection = Normalise(touchClosestToBall - ballClosestToTouch);
		}
	}

//#ifdef DSC_DEBUG
//	{
//		GScene& scene = DEBUG_GET_SCENE()
//		GMatrix16Float transform(GMatrix16Float::sIdentity);
//
//		transform.SetPosition(touchClosestToBall);
//		scene.GetArrayDebugMatrix().push_back(transform);
//
//		transform.SetPosition(ballClosestToTouch);
//		scene.GetArrayDebugMatrix().push_back(transform);
//	}
//#endif

	if (hit)
	{
		float verticalAmount = 1.0F + GMath::Clamp((-mScreenTouchDelta.GetY()) / (in_timeDelta * 2.0F), 0.0F, 5.0F);
		float depthAmount = 1.0F + (verticalAmount * 0.1F) + GMath::Minimum(GMath::Abs(mScreenTouchDelta.GetX() / (in_timeDelta * 2.0F)) * 0.5F, 5.0F);
		float horizontalAmount = -(hitDirection.GetX() * 1.5F);
		float rotationSpeed = GMath::Minimum(0.18F, (LengthAprox(mScreenTouchDelta) / in_timeDelta) * 0.05F);

		if (mState == TState::TPlayerServe)
		{
			//don't spike off serve
			depthAmount = GMath::Minimum(sMaxHitHorizontalServe, depthAmount);
		}
		else
		{
			depthAmount = GMath::Minimum(sMaxHitHorizontal, depthAmount);
		}

		mState = TState::TGameAfterPlayerHit;

		GVector3Float hitBallVelocity(
			horizontalAmount,
			verticalAmount,
			-depthAmount
			);

		const GVector3Float rotationAxis = Normalise(MakeCrossVector(-hitDirection));

		ball.SetBallInFlight(
			ballPosition, 
			hitBallVelocity, // * (1.0F + ((rayLength / in_timeDelta) * 4.0F)),  //argh, should be frame rate dependant
			ball.GetAt(),
			ball.GetUp(),
			rotationAxis, //-GVector3Float::sUnitX,
			rotationSpeed
			);		

		TPointerBeachVolleyBallClientOpponent pointerOpponent = mClientOpponent.lock();
		if (pointerOpponent)
		{
			pointerOpponent->NotifyPlayerHitBall(ball);
		}

		mRallyCount += 1;

		TPointerGuiNodeComponentText pointerMessageText = mMessageText.lock();
		if (pointerMessageText && !mDemoMode)
		{
			GGuiNodeComponentText& messageText = *pointerMessageText;
			messageText.SetVisible(false);
		}
	}

	return;
}

void BeachVolleyBallGame::MultiTouchFoul()
{
	TPointerGuiNodeComponentText pointerMessageText = mMessageText.lock();
	if (pointerMessageText && !mDemoMode)
	{
		GGuiNodeComponentText& messageText = *pointerMessageText;

		messageText.SetText("   Multi touch foul");
		messageText.SetVisible(true);
		mMessageCountdown = 3.0F;
	}

	mStateCountdown = 3.0F;
	mState = TState::TPlayerFoul;

	//call ScorePoint AFTER setting state to playerFoul, score point could set GameOver state
	ScorePoint(false);

	return;
}

void BeachVolleyBallGame::ScorePoint(const bool in_playerPoint)
{
	bool opponentWinGame = false;
	bool opponentLostGame = false;
	bool opponentWinServe = false;
	bool opponentLostServe = false;
	bool opponentWinPoint = false;
	bool opponentLostPoint = false;

	if (in_playerPoint)
	{
		opponentLostServe = (mScoreState != TScoreState::TPlayerServe);
		opponentLostPoint = true;
		mScoreState = TScoreState::TPlayerServe;

		mScorePlayer += 1;
		LocalUpdateScore(mPlayerScoreText, mScorePlayer);
	}
	else
	{
		opponentWinServe = (mScoreState != TScoreState::TOpponentServe);
		opponentWinPoint = true;

		mScoreState = TScoreState::TOpponentServe;

		mScoreOpponent += 1;
		LocalUpdateScore(mOpponentScoreText, mScoreOpponent);
	}

	//const int difference = GMath::Abs(mScorePlayer - mScoreOpponent);
	if ((sPointsToWin <= mScorePlayer) &&
		(mScoreOpponent + sPointsDifferenceWin <= mScorePlayer))
	{
		opponentLostGame = true;
		SetStateGameOver();
		if (!mDemoMode)
		{
			mParentState.GetGameVar()->NotifyPlayerWin(mGirl, mMatch);
		}
	}
	else if ((sPointsToWin <= mScoreOpponent) &&
		(mScorePlayer + sPointsDifferenceWin <= mScoreOpponent))
	{
		opponentWinGame = true;
		SetStateGameOver();
		if (!mDemoMode)
		{
			mParentState.GetGameVar()->NotifyOpponentWin(mGirl, mMatch);
		}
	}
	else
	{
		mState = TState::TBallTouchedGround;
		mStateCountdown = 3.0F;
	}

	TPointerBeachVolleyBallClientOpponent clientOpponent = mClientOpponent.lock();
	if (clientOpponent)
	{
		if (opponentWinGame)
		{
			clientOpponent->NotifyOpponentWinGame();
		}
		else if (opponentLostGame)
		{
			clientOpponent->NotifyOpponentLostGame();
		}
		else if (opponentWinServe)
		{
			clientOpponent->NotifyOpponentWinServe();
		}
		else if (opponentLostServe)
		{
			clientOpponent->NotifyOpponentLostServe();
		}
		else if (opponentWinPoint)
		{
			clientOpponent->NotifyOpponentWinPoint();
		}
		else if (opponentLostPoint)
		{
			clientOpponent->NotifyOpponentLostPoint();
		}
	}

	return;
}

//hack. there is a situation where iOS seems to loose touch events, so clear the input
void BeachVolleyBallGame::FlushInput(const GInput& in_input)
{
	GInput& input = (GInput&)in_input;
	input.SetTouchCount(0);
	return;
}
