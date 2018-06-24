//
//  BeachVolleyBallMovie.cpp
//  BeachVolleyBall
//
//  Created by David Coen on 2011 05 08
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "BeachVolleyBallMovie.h"

#include "BeachVolleyBallState.h"

#include <GCommon.h>
#include <GMath.h>
#include <GScene.h>
#include <GScenePostLoadCallback.h>
#include <GSceneVisitorAll.h>
#include <GGuiNodeComponentButton.h>
#include <GSceneNodeComponentAnimation.h>
#include <GSceneNodeComponentNodeAttach.h>
#include <GSceneNodeComponentVisual.h>
#include <GStringUtility.h>
#include <GAnimationPlayParam.h>
#include <GAnimationManagerInstance.h>
#include <map>

static const float sDefaultCameraPerspectiveBounds = 0.08F;
static const float sDefaultCameraPerspectiveNear = 0.1F;
static const float sDefaultCameraPerspectiveFar = 1000.0F;
static const float sDefaultGirlCountdown = 1.0F;

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
typedef boost::shared_ptr<GSceneNodeComponentAnimation> TPointerSceneNodeComponentAnimation;
typedef boost::weak_ptr<GSceneNodeComponentAnimation> TWeakSceneNodeComponentAnimation;
typedef std::vector<TWeakSceneNodeComponentAnimation> TArrayWeakSceneNodeComponentAnimation;
typedef boost::shared_ptr<GAnimationManagerInstance> TPointerAnimationManagerInstance;

struct TMovieGirl
{
	enum TEnum
	{
		TBikini0,
		TBikini1,
		TBikini2,
		TReiko0,
		TReiko1,
		TReiko2,
		TApril0,
		TApril1,
		TApril2,
		TCuda0,
		TCuda1,
		TCuda2,

		TCount
	};
};

static void LocalGetGirl(
	BeachVolleyBallType::TGirl::TEnum& out_girl,
	BeachVolleyBallType::TMatch::TEnum& out_match,
	const TMovieGirl::TEnum in_girl
	)
{
	out_girl = BeachVolleyBallType::TGirl::TCount;
	out_match = BeachVolleyBallType::TMatch::TCount;
	switch (in_girl)
	{
	default:
		break;
	case TMovieGirl::TApril0:
		out_girl = BeachVolleyBallType::TGirl::TApril;
		out_match = BeachVolleyBallType::TMatch::TOne;
		break;
	case TMovieGirl::TApril1:
		out_girl = BeachVolleyBallType::TGirl::TApril;
		out_match = BeachVolleyBallType::TMatch::TTwo;
		break;
	case TMovieGirl::TApril2:
		out_girl = BeachVolleyBallType::TGirl::TApril;
		out_match = BeachVolleyBallType::TMatch::TThree;
		break;

	case TMovieGirl::TBikini0:
		out_girl = BeachVolleyBallType::TGirl::TBikini;
		out_match = BeachVolleyBallType::TMatch::TOne;
		break;
	case TMovieGirl::TBikini1:
		out_girl = BeachVolleyBallType::TGirl::TBikini;
		out_match = BeachVolleyBallType::TMatch::TTwo;
		break;
	case TMovieGirl::TBikini2:
		out_girl = BeachVolleyBallType::TGirl::TBikini;
		out_match = BeachVolleyBallType::TMatch::TThree;
		break;

	case TMovieGirl::TCuda0:
		out_girl = BeachVolleyBallType::TGirl::TCuda;
		out_match = BeachVolleyBallType::TMatch::TOne;
		break;
	case TMovieGirl::TCuda1:
		out_girl = BeachVolleyBallType::TGirl::TCuda;
		out_match = BeachVolleyBallType::TMatch::TTwo;
		break;
	case TMovieGirl::TCuda2:
		out_girl = BeachVolleyBallType::TGirl::TCuda;
		out_match = BeachVolleyBallType::TMatch::TThree;
		break;

	case TMovieGirl::TReiko0:
		out_girl = BeachVolleyBallType::TGirl::TReiko;
		out_match = BeachVolleyBallType::TMatch::TOne;
		break;
	case TMovieGirl::TReiko1:
		out_girl = BeachVolleyBallType::TGirl::TReiko;
		out_match = BeachVolleyBallType::TMatch::TTwo;
		break;
	case TMovieGirl::TReiko2:
		out_girl = BeachVolleyBallType::TGirl::TReiko;
		out_match = BeachVolleyBallType::TMatch::TThree;
		break;
	}
	return;
}

//constructor
BeachVolleyBallMovie::BeachVolleyBallMovie(
	BeachVolleyBallState& inout_parentState, 
	const float in_cameraAspect
	)
	: mParentState(inout_parentState)
	, mSceneNodeRoot()
	, mCamera(
	GCamera::TType::TPerspective,
		sDefaultCameraPerspectiveNear,
		sDefaultCameraPerspectiveFar,
		sDefaultCameraPerspectiveBounds,
		-sDefaultCameraPerspectiveBounds,
		-(sDefaultCameraPerspectiveBounds * in_cameraAspect),
		sDefaultCameraPerspectiveBounds * in_cameraAspect
		)
	, mTime(0.0F)
	, mGirlCountdown(0.0F)
	, mGirl(TMovieGirl::TBikini0)
	, mStartCountDown(2.0F)
{
	inout_parentState.SetDrawShadows(true);

	if (inout_parentState.GetScene())
	{
		GScene& scene = *inout_parentState.GetScene();
		TPointerSceneNode sceneNode = scene.CreateSceneNode("gameRoot");
		scene.AddSceneRoot(sceneNode);
		mSceneNodeRoot = sceneNode;

		TArrayScenePostLoadCallback arrayPostLoadCallback;
		for (int index = 0; index < TMovieGirl::TCount; ++index)
		{
			BeachVolleyBallType::TGirl::TEnum girl;
			BeachVolleyBallType::TMatch::TEnum match;
			LocalGetGirl(girl, match, (TMovieGirl::TEnum)index);

			TPointerSceneNode sceneNodeActor = scene.CreateSceneNode("movieActor");
			//sceneNode->AddChildNode(sceneNodeActor);

			scene.SceneNodeFactory(
				BeachVolleyBallType::GetFactoryName(girl, match),
				"", 
				sceneNodeActor, 
				arrayPostLoadCallback
				);

			mArraySceneNode.push_back(sceneNodeActor);

			//find all animation components in scene and set there animation to menu, no fade in
			TArrayWeakSceneNodeComponentAnimation arrayComponentAnimation;
			GSceneVisitorAllCollect<GSceneNodeComponentAnimation>::Run(sceneNodeActor, arrayComponentAnimation);

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

		GScene::FactoryPostLoad(arrayPostLoadCallback, scene);

		sceneNode->AddChildNode(mArraySceneNode[mGirl]);
		mSceneNodeGirl = mArraySceneNode[mGirl];
		mGirl += 1;
		mGirlCountdown = sDefaultGirlCountdown;
	}

	UpdateCamera();

	return;
}

BeachVolleyBallMovie::~BeachVolleyBallMovie()
{
	mArraySceneNode.clear();
	if (mParentState.GetScene())
	{
		GScene& scene = *mParentState.GetScene();
		TPointerSceneNode sceneNode = mSceneNodeRoot.lock();
		scene.SceneNodeRequestDelete(sceneNode);
		scene.SceneNodeProcessDeleteQueue();
	}

	return;
}

//public methods
void BeachVolleyBallMovie::Tick(const float in_timeDelta)
{
	if (0.0F < mStartCountDown)
	{
		mStartCountDown -= in_timeDelta;
	}
	else
	{
		mGirlCountdown -= in_timeDelta;
		if ((mGirlCountdown <= 0.0F) &&
			(mGirl < TMovieGirl::TCount))
		{
			mGirlCountdown = sDefaultGirlCountdown;
			TPointerSceneNode pointerRoot = mSceneNodeRoot.lock();
			pointerRoot->RemoveChild(mSceneNodeGirl.lock());
			//mGirl = (mGirl + 1) % mArraySceneNode.size();

			TPointerSceneNode pointerGirl = mArraySceneNode[mGirl];
			mGirl += 1;

			pointerRoot->AddChildNode(pointerGirl);
			mSceneNodeGirl = pointerGirl;
			pointerGirl->SetTeleportFlag(true);
		}
		else
		{
			TPointerSceneNode pointerGirl = mSceneNodeGirl.lock();
			if (pointerGirl)
			{
				pointerGirl->SetTeleportFlag(false);
			}
		}

		mTime += in_timeDelta;
		UpdateCamera();
	}

	//tick the scene
	if (mParentState.GetScene())
	{
		GScene& scene = *mParentState.GetScene();
		GSceneVisitorAllTick<GSceneNodeComponentAnimation>::Run(scene, in_timeDelta);
		GSceneVisitorAllTick<GSceneNodeComponentVisual>::Run(scene, in_timeDelta);
	}

	return;
}

void BeachVolleyBallMovie::Resize(const float in_cameraAspect)
{
	mCamera.SetTop(sDefaultCameraPerspectiveBounds);
	mCamera.SetBottom(-sDefaultCameraPerspectiveBounds);
	mCamera.SetLeft(-sDefaultCameraPerspectiveBounds * in_cameraAspect);
	mCamera.SetRight(sDefaultCameraPerspectiveBounds * in_cameraAspect);

	return;
}

//private methods
void BeachVolleyBallMovie::UpdateCamera()
{
	const GVector3Float at(GMath::Sin(mTime * 0.125F), 0.0F, -GMath::Cos(mTime * 0.125F));
	const GVector3Float pos(at.GetX() * (-1.5F), 1.0F, at.GetZ() * (-1.5F));

	mCamera.SetTransform(
		at,
		GVector3Float::sUnitY,
		pos
		);

	return;
}
