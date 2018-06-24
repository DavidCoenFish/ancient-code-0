//
//  SceneNodeComponentClientObjectBase.cpp
//  Zombie
//
//  Created by David Coen on 2011 01 29
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "SceneNodeComponentClientObjectBase.h"
#include "ZombieMission.h"

//constructor
SceneNodeComponentClientObjectBase::SceneNodeComponentClientObjectBase(const std::string& in_name)
: mListHook()
, mName(in_name)
{
	return;
}

/*virtual*/ SceneNodeComponentClientObjectBase::~SceneNodeComponentClientObjectBase()
{
	return;
}

//public methods
void SceneNodeComponentClientObjectBase::CollectDamageReceivers(
	TArrayPairClientObjectCollisionResult& out_arrayClientObjectCollisionResult,
	const GVector3Float& in_worldSpaceConeOrigin,
	const GVector3Float& in_worldSpaceConeDirection,
	const float in_coneLength,
	const float in_coneRadiusPoint,
	const float in_coneRadiusBase
	)
{
	OnCollectDamageReceivers(
		out_arrayClientObjectCollisionResult,
		in_worldSpaceConeOrigin,
		in_worldSpaceConeDirection,
		in_coneLength,
		in_coneRadiusPoint,
		in_coneRadiusBase
		);
	return;
}

void SceneNodeComponentClientObjectBase::ApplyDamage(
	float& inout_damage, 
	const std::string& in_data
	)
{
	OnApplyDamage(inout_damage, in_data);
	return;
}

void SceneNodeComponentClientObjectBase::AfterDamagePass(GScene& inout_scene, TPointerSceneNode& inout_sceenRoot)
{
	OnAfterDamagePass(inout_scene, inout_sceenRoot);
	return;
}

void SceneNodeComponentClientObjectBase::Animate(
	const std::string& in_animationName, 
	const GAnimationPlayParam& in_playParam
	)
{
	OnAnimate(
		in_animationName, 
		in_playParam
		);
	return;
}

void SceneNodeComponentClientObjectBase::Move(
	const GVector3Float& in_position,
	const float in_transitionTime,
	const int in_transitionType
	)
{
	OnMove(
		in_position,
		in_transitionTime,
		in_transitionType
		);
	return;
}

void SceneNodeComponentClientObjectBase::Rotate(
	const GVector3Float& in_at,
	const GVector3Float& in_up,
	const float in_transitionTime,
	const int in_transitionType
	)
{
	OnRotate(
		in_at,
		in_up,
		in_transitionTime,
		in_transitionType
		);
	return;
}

void SceneNodeComponentClientObjectBase::SetAiActive(const bool in_active)
{
	OnSetAiActive(in_active);
	return;
}

const bool SceneNodeComponentClientObjectBase::GetAiActive()const
{
	return OnGetAiActive();
}

void SceneNodeComponentClientObjectBase::Destroy()
{
	return OnDestroy();
}

//interface
/*virtual*/ void SceneNodeComponentClientObjectBase::OnCollectDamageReceivers(
	TArrayPairClientObjectCollisionResult& out_arrayClientObjectCollisionResult,
	const GVector3Float& in_worldSpaceConeOrigin,
	const GVector3Float& in_worldSpaceConeDirection,
	const float in_coneLength,
	const float in_coneRadiusPoint,
	const float in_coneRadiusBase
	)
{
	return;
}

/*virtual*/ void SceneNodeComponentClientObjectBase::OnApplyDamage(
	float& inout_damage, 
	const std::string& in_data
	)
{
	return;
}

/*virtual*/ void SceneNodeComponentClientObjectBase::OnAfterDamagePass(GScene&, TPointerSceneNode&)
{
	return;
}

/*virtual*/ void SceneNodeComponentClientObjectBase::OnAnimate(
	const std::string&, 
	const GAnimationPlayParam&
	)
{
	return;
}
/*virtual*/ void SceneNodeComponentClientObjectBase::OnMove(
	const GVector3Float& in_position,
	const float in_transitionTime,
	const int in_transitionType
	)
{
	return;
}

/*virtual*/ void SceneNodeComponentClientObjectBase::OnRotate(
	const GVector3Float& in_at,
	const GVector3Float& in_up,
	const float in_transitionTime,
	const int in_transitionType
	)
{
	return;
}

/*virtual*/ void SceneNodeComponentClientObjectBase::OnSetAiActive(const bool)
{
	return;
}

/*virtual*/ const bool SceneNodeComponentClientObjectBase::OnGetAiActive()const
{
	return false;
}

/*virtual*/ void SceneNodeComponentClientObjectBase::OnDestroy()
{
	return;
}
