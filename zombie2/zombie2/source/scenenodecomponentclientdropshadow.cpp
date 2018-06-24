//
//  SceneNodeComponentClientDropShadow.cpp
//  Zombie
//
//  Created by David Coen on 2011 03 14
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "SceneNodeComponentClientDropShadow.h"

#include "ZombieMission.h"

#include <GAssert.h>
#include <GCamera.h>
#include <GScene.h>
#include <GSceneNode.h>
#include <GSceneNodeComponentBase.h>
#include <GSceneNodeComponentVisual.h>
#include <GScenePostLoadCallback.h>
#include <GVector2Float.h>
#include <GMatrix16Float.h>
#include <GMath.h>

typedef boost::shared_ptr<GRender> TPointerRenderComponent;

//static public methods
/*static*/ SceneNodeComponentClientDropShadow::TPointerSceneNodeComponentBase SceneNodeComponentClientDropShadow::Factory(
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

	//make a scene node
	TPointerSceneNode pointerChildSceneNode;
	pointerChildSceneNode.reset(new GSceneNode("dropShadowSceneNode"));
	if (pointerChildSceneNode)
	{
		GSceneNode& childSceneNode = *pointerChildSceneNode;
		childSceneNode.SetDontInheritParentTransform(true);

		boost::shared_ptr<GSceneNodeComponentBase> pointerComponent = GSceneNodeComponentVisual::Factory(
			inout_scene,
			"dropshadow",
			"shadowmesh.data",
			"material_shadow00.data",
			1.0,
			0);

		boost::shared_ptr<GSceneNodeComponentVisual> pVisual = boost::dynamic_pointer_cast< GSceneNodeComponentVisual >(pointerComponent);
		if (pVisual)
			pVisual->SetVisible(true);

		childSceneNode.AddComponent(pointerComponent);
		
		inout_owner->AddChildNode(pointerChildSceneNode);
	}

	pointer.reset(new SceneNodeComponentClientDropShadow(pointerChildSceneNode));

	return pointer;
}

//constructor
SceneNodeComponentClientDropShadow::SceneNodeComponentClientDropShadow(TPointerSceneNode& inout_childSceneNode)
: mChildSceneNode(inout_childSceneNode)
{
	return;
}

/*virtual*/ SceneNodeComponentClientDropShadow::~SceneNodeComponentClientDropShadow()
{
	return;
}

//implement GSceneNodeComponentClient
/*virtual*/ void SceneNodeComponentClientDropShadow::OnTick(TPointerSceneNode& inout_sceneNode, const float in_timeDelta)
{
	TPointerSceneNode childNode = mChildSceneNode.lock();
	if (childNode && inout_sceneNode)
	{
		GVector3Float position = inout_sceneNode->GetWorldTransform().GetPosition();
		const float height = position.m_y;
		position.m_y = (0.01F);

		const float scale = GMath::Clamp((2.0F - height) * 6.0F, 0.01F, 3.0F);
		GMatrix16Float transform(
			scale, 0.0F, 0.0F, 0.0F,
			0.0F, scale, 0.0F, 0.0F,
			0.0F, 0.0F, scale, 0.0F,
			0.0F, 0.0F, 0.0F, 1.0F
			);
		transform.SetPosition(position);
		childNode->SetParentRelativeTransform(transform);
	}
	return;
}
