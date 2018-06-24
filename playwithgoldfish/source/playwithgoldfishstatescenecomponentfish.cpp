//
//  PlayWithGoldfishStateSceneComponentFish.cpp
//  playwithgoldfish
//
//  Created by David Coen on 2011 01 13
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "PlayWithGoldfishStateSceneComponentFish.h"

#include "PlayWithGoldfishState.h"
#include "PlayWithGoldfishStateComponentAi.h"
#include "PlayWithGoldfishStateComponentAnimation.h"

#include <GCamera.h>
#include <GInput.h>
#include <GInputTouch.h>
#include <GSceneNode.h>
#include <GSceneType.h>
#include <GSceneNodeComponentVisual.h>
#include <GSceneNodeComponentAnimation.h>
#include <GSceneNodeComponentNodeAttach.h>
#include <GScenePostLoadCallback.h>
#include <GRenderMeshInstance.h>
#include <GMath.h>
#include <GScene.h>
#include <GDictionary.h>

#include <boost/weak_ptr.hpp>

static const char* const sPosition = "position";
static const char* const sFacing = "facing";
static const char* const sTouchActive = "touchActive";
static const char* const sTouchBegin = "touchBegin";
static const char* const sTouchWorldPoint = "touchWorldPoint";
static const char* const sCameraTarget = "cameraTarget";

//static local methods
static const GMatrix16Float LocalMakeTransform(const GVector3Float& in_position, const float in_facing)
{
	GMatrix16Float transform = GMatrix16FloatConstructAxisAngle(GVector3Float::sUnitY, in_facing);
	transform.SetPosition(in_position);
	return transform;
}


//static public methods
/*static*/ PlayWithGoldfishStateSceneComponentFish::TPointerSceneNodeComponentBase PlayWithGoldfishStateSceneComponentFish::Factory(
	GScene& inout_scene,
	GSceneNode& inout_owner,
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
	void* const in_data
	)
{
	TPointerPlayWithGoldfishStateSceneComponentFish pointer;

	if (in_data)
	{
		PlayWithGoldfishState& playWithGoldfishState = *((PlayWithGoldfishState*)in_data);
		pointer.reset(new PlayWithGoldfishStateSceneComponentFish(playWithGoldfishState));
		playWithGoldfishState.AddComponentFish(pointer);
	}

	TPointerSceneNodeComponentBase pointerBase(pointer);

	inout_arrayPostLoadCallback.push_back(GScenePostLoadCallback(
		&inout_owner,
		pointerBase,
		PlayWithGoldfishStateSceneComponentFish::PostLoadCallback
		));

	return pointerBase;
}


/*static*/ const GVector3Float& PlayWithGoldfishStateSceneComponentFish::GetPosition(GDictionary& inout_dictionary)
{
	return inout_dictionary.GetValueVector3Float(sPosition);
}

/*static*/ void PlayWithGoldfishStateSceneComponentFish::SetPosition(GDictionary& inout_dictionary, const GVector3Float& in_position)
{
	inout_dictionary.SetValueVector3Float(sPosition, in_position);
	return;
}

/*static*/ const float PlayWithGoldfishStateSceneComponentFish::GetFacing(GDictionary& inout_dictionary)
{
	return inout_dictionary.GetValueFloat(sFacing);
}

/*static*/ void PlayWithGoldfishStateSceneComponentFish::SetFacing(GDictionary& inout_dictionary, const float in_facing)
{
	inout_dictionary.SetValueFloat(sFacing, in_facing);
	return;
}

/*static*/ const bool PlayWithGoldfishStateSceneComponentFish::GetTouchBegin(GDictionary& inout_dictionary)
{
	return inout_dictionary.GetValueBool(sTouchBegin);
}
/*static*/ void PlayWithGoldfishStateSceneComponentFish::SetTouchBegin(GDictionary& inout_dictionary, const bool in_value)
{
	inout_dictionary.SetValueBool(sTouchBegin, in_value);
	return;
}

/*static*/ const bool PlayWithGoldfishStateSceneComponentFish::GetTouchActive(GDictionary& inout_dictionary)
{
	return inout_dictionary.GetValueBool(sTouchActive);
}
/*static*/ void PlayWithGoldfishStateSceneComponentFish::SetTouchActive(GDictionary& inout_dictionary, const bool in_value)
{
	inout_dictionary.SetValueBool(sTouchActive, in_value);
	return;
}
/*static*/ const GVector3Float& PlayWithGoldfishStateSceneComponentFish::GetTouchWorldPoint(GDictionary& inout_dictionary)
{
	return inout_dictionary.GetValueVector3Float(sTouchWorldPoint);
}
/*static*/ void PlayWithGoldfishStateSceneComponentFish::SetTouchWorldPoint(GDictionary& inout_dictionary, const GVector3Float& in_value)
{
	inout_dictionary.SetValueVector3Float(sTouchWorldPoint, in_value);
	return;
}
/*static*/ const GVector3Float& PlayWithGoldfishStateSceneComponentFish::GetCameraTarget(GDictionary& inout_dictionary)
{
	return inout_dictionary.GetValueVector3Float(sCameraTarget);
}
/*static*/ void PlayWithGoldfishStateSceneComponentFish::SetCameraTarget(GDictionary& inout_dictionary, const GVector3Float& in_value)
{
	inout_dictionary.SetValueVector3Float(sCameraTarget, in_value);
	return;
}

/*static*/ void PlayWithGoldfishStateSceneComponentFish::PostLoadCallback(
	GScene& inout_scene, //access to mesh/ material manager
	GSceneNode& inout_owner,
	TPointerSceneNodeComponentBase& inout_component,
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback
	)
{
	TPointerPlayWithGoldfishStateSceneComponentFish pointer = boost::dynamic_pointer_cast<PlayWithGoldfishStateSceneComponentFish>(inout_component);

	//locate the animation component, get the animationManagerInstance
	const int componentCount = inout_owner.GetComponentCount();
	for (int index = 0; index < componentCount; ++index)
	{
		GSceneNodeComponentAnimation* const pAnimationComponent = inout_owner.GetComponent<GSceneNodeComponentAnimation>(index);
		if (!pAnimationComponent)
		{
			continue;
		}

		pointer->mComponentAnimation.reset(new PlayWithGoldfishStateComponentAnimation(pAnimationComponent->GetAnimationManagerInstance()));

		break;
	}

	return;
}

//constructor
PlayWithGoldfishStateSceneComponentFish::PlayWithGoldfishStateSceneComponentFish(
	PlayWithGoldfishState& inout_parentState
	)
	: GSceneNodeComponentClient()
	, mParentState(inout_parentState)
	, mTeleport(true)
	, mDictionary()
	, mMovementType(PlayWithGoldfishStateType::TMovementType::TCount)
	, mAnimationType(PlayWithGoldfishStateType::TAnimationType::TCount)
{
	mComponentAi.reset(new PlayWithGoldfishStateComponentAi);
	mDictionary.reset(new GDictionary);
	return;
}

/*virtual*/ PlayWithGoldfishStateSceneComponentFish::~PlayWithGoldfishStateSceneComponentFish()
{
	return;
}

//implement GSceneNodeComponentClient
/*virtual*/ void PlayWithGoldfishStateSceneComponentFish::OnTick(GSceneNode& inout_sceneNode, const float in_timeDelta)
{
	//set teleport flag
	inout_sceneNode.SetTeleportFlag(mTeleport);
	mTeleport = false;

	if (!mDictionary)
	{
		return;
	}	

	//tick ai
	if (mComponentAi && (PlayWithGoldfishStateType::TMovementType::TAI == mMovementType))
	{
		mComponentAi->Tick(in_timeDelta, *mDictionary);
	}

	//update scene node position
	switch (mMovementType)
	{
	default:
		break;
	case PlayWithGoldfishStateType::TMovementType::TAI:
		{
			const GMatrix16Float transform = LocalMakeTransform(GetPosition(*mDictionary), GetFacing(*mDictionary));
			inout_sceneNode.SetParentRelativeTransform(transform);
		}
		break;
	case PlayWithGoldfishStateType::TMovementType::TInput:
		{
			const GMatrix16Float transform = LocalMakeTransform(GetTouchWorldPoint(*mDictionary), 0.0F);
			inout_sceneNode.SetParentRelativeTransform(transform);
		}
		break;
	case PlayWithGoldfishStateType::TMovementType::TNone:
		{
			const GMatrix16Float transform = LocalMakeTransform(GVector3Float::sZero, 0.0F);
			inout_sceneNode.SetParentRelativeTransform(transform);
		}
		break;
	}

	//update animation
	if (mComponentAnimation)
	{
		mComponentAnimation->Tick(
			in_timeDelta,
			mAnimationType,
			*mDictionary
			);
	}


	//clear input var
	SetTouchBegin(*mDictionary, false);
	SetTouchActive(*mDictionary, false);

	return;
}

//public methods
void PlayWithGoldfishStateSceneComponentFish::ShakeWindow()
{
	SetTouch(GVector3Float::sZero, false);
	mTeleport = true;
	
	if (!mDictionary)
	{
		return;
	}
	SetPosition(*mDictionary, GVector3Float::sZero);
	SetFacing(*mDictionary, 0.0F);
	
	if (mComponentAi)
	{
		mComponentAi->ShakeWindow(*mDictionary);
	}

	return;
}

void PlayWithGoldfishStateSceneComponentFish::SetTouch(const GVector3Float& in_touchWorldPoint, const bool in_touchBegin)
{
	if (mDictionary)
	{
		SetTouchWorldPoint(*mDictionary, in_touchWorldPoint);
		SetTouchBegin(*mDictionary, in_touchBegin);
		SetTouchActive(*mDictionary, true);
	}
	return;
}

void PlayWithGoldfishStateSceneComponentFish::SetMovementType(const PlayWithGoldfishStateType::TMovementType::TEnum in_movementType)
{
	if (mMovementType == in_movementType)
	{
		return;
	}
	mMovementType = in_movementType;
	mTeleport = true;

	return;
}

void PlayWithGoldfishStateSceneComponentFish::SetAnimationType(const PlayWithGoldfishStateType::TAnimationType::TEnum in_animationType)
{
	if (mAnimationType == in_animationType)
	{
		return;
	}
	mAnimationType = in_animationType;

	return;
}

void PlayWithGoldfishStateSceneComponentFish::SetCameraTarget(const GVector3Float& in_cameraTarget)
{
	if (mDictionary)
	{
		SetCameraTarget(*mDictionary, in_cameraTarget);
	}

	return;
}

/**/