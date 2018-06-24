//
//  SceneNodeComponentClientTarget.cpp
//  Zombie
//
//  Created by David Coen on 2011 02 09
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "SceneNodeComponentClientTarget.h"

#include "ZombieMission.h"

#include <GCamera.h>
#include <GSceneNode.h>
#include <GSceneNodeComponentBase.h>
#include <GScenePostLoadCallback.h>
#include <GVector2Float.h>
#include <GMatrix16Float.h>
#include <GAssert.h>
#include <GMath.h>

//static public methods
/*static*/ SceneNodeComponentClientTarget::TPointerSceneNodeComponentBase SceneNodeComponentClientTarget::Factory(
	GScene& inout_scene,
	TPointerSceneNode& inout_owner,
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
	void* const in_data //address of ApplicationTaskGame
	)
{
	TPointerSceneNodeComponentBase pointer;
	if (!in_data)
	{
		return pointer;
	}

	ZombieMission& zombieMission = *((ZombieMission*)in_data);

	pointer.reset(new SceneNodeComponentClientTarget(zombieMission));

	return pointer;
}

//constructor
SceneNodeComponentClientTarget::SceneNodeComponentClientTarget(ZombieMission& inout_zombieMission)
: GSceneNodeComponentClient()
, mWorldPos()
{
	inout_zombieMission.AddClientTarget(*this);
	return;
}

/*virtual*/ SceneNodeComponentClientTarget::~SceneNodeComponentClientTarget()
{
	return;
}

//public methods
const GVector2Float SceneNodeComponentClientTarget::GetScreenPos(GCamera& in_camera)
{
	const GVector3Float result = mWorldPos * in_camera.GetProjectionMatrix();
	if (0.0F < result.m_z)
	{
		return GVector2Float(result.m_x / result.m_z, result.m_y / result.m_z);
	}

	GVector2Float behindPlayerResult(result.m_x, result.m_y);
	const float magX = GMath::Abs(behindPlayerResult.m_x);
	const float magY = GMath::Abs(behindPlayerResult.m_y);
	if (magX < magY)
	{
		behindPlayerResult /= magY;
	}
	else if (0.0F < magX)
	{
		behindPlayerResult /= magX;
	}

	return behindPlayerResult;
}

//implement GSceneNodeComponentClient
/*virtual*/ void SceneNodeComponentClientTarget::OnTick(TPointerSceneNode& inout_sceneNode, const float in_timeDelta)
{
	mWorldPos = inout_sceneNode->GetWorldTransform().GetPosition();
	return;
}
