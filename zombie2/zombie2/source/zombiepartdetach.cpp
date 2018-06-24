//
//  ZombiePartDetach.cpp
//  Zombie
//
//  Created by David Coen on 2011 02 08
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "ZombiePartDetach.h"

#include <GSceneNode.h>
#include <GSceneNodeComponentBase.h>
#include <GSceneNodeComponentVisual.h>
#include <GSceneNodeComponentFade.h>
#include <GSceneNodeComponentPhysics.h>
#include <GSceneNodeComponentPhysicsLoad.h>
#include <GMath.h>

//constructor
ZombiePartDetach::ZombiePartDetach(
	GScene& inout_scene,
	TPointerSceneNode& inout_sceenRoot, //child from this
	TPointerSceneNode& in_zombieParentSceneNode //use this transform
	)
	: mWeakSceneNode()
	, mWeakSceneNodeZombieParent(in_zombieParentSceneNode)
	, mArrayPartIndex()
{
	if (!inout_sceenRoot)
	{
		return;
	}
	GSceneNode& sceneNodeRoot = *inout_sceenRoot;
	GSceneNode& zombieParentSceneNode = *in_zombieParentSceneNode;

	TPointerSceneNode pointerNewSceneNode;
	pointerNewSceneNode.reset(new GSceneNode("detachedLimb"));
	inout_sceenRoot->AddChildNode(pointerNewSceneNode);
	GSceneNode& newSceneNode = *pointerNewSceneNode;
	newSceneNode.SetParentRelativeTransform(zombieParentSceneNode.GetWorldTransform());
	mInvertWorldTransform = zombieParentSceneNode.GetWorldTransformInvert();

	//add fade commponent
	TPointerSceneNodeComponentBase pointerFade;
	pointerFade.reset(new GSceneNodeComponentFade(inout_scene, 5.0F));
	newSceneNode.AddComponent(pointerFade);

	//add physics component
	TPointerSceneNodeComponentBase pointerPhysics;
	pointerPhysics.reset(new GSceneNodeComponentPhysics(
		1.0F,
		zombieParentSceneNode.GetWorldTransform().GetPosition(),
		GVector3Float(GMath::RandomUnitPlusMinius(), GMath::RandomUnit() * 2.0F, GMath::RandomUnitPlusMinius()),
		GSceneNodeComponentPhysicsLoad::TFlag::TAllowMove | GSceneNodeComponentPhysicsLoad::TFlag::TAllowRotate
		));
	newSceneNode.AddComponent(pointerPhysics);

	sceneNodeRoot.AddChildNode(pointerNewSceneNode);

	mWeakSceneNode = pointerNewSceneNode;

	return;
}

ZombiePartDetach::~ZombiePartDetach()
{
	return;
}

//public methods
const bool ZombiePartDetach::SearchPartIndex(const int in_partIndex)const
{
	for (TArrayInt::const_iterator iterator = mArrayPartIndex.begin(); iterator != mArrayPartIndex.end(); ++iterator)
	{
		if (in_partIndex == (*iterator))
		{
			return true;
		}
	}

	return false;
}

void ZombiePartDetach::AddParts(
	const int in_partIndex,
	TArrayPairWeakSceneNodeComponentVisualWeakSceneNode& inout_arrayVisualComponent
	)
{
	TPointerSceneNode pointerZombieParentSceneNode = mWeakSceneNodeZombieParent.lock();
	if (!pointerZombieParentSceneNode)
	{
		return;
	}
	GSceneNode& zombieParentSceneNode = *pointerZombieParentSceneNode;

	mArrayPartIndex.push_back(in_partIndex);
	//mArrayVisualNode.insert(mArrayVisualNode.end(), inout_arrayVisualComponent.begin(), inout_arrayVisualComponent.end());
	for (TArrayPairWeakSceneNodeComponentVisualWeakSceneNode::iterator iterator = inout_arrayVisualComponent.begin(); iterator != inout_arrayVisualComponent.end(); ++iterator)
	{
		TPointerSceneNodeComponentVisual pointerVisual = (*iterator).first.lock();
		TPointerSceneNode pointerNode = (*iterator).second.lock();

		if (!pointerVisual || !pointerNode)
		{
			continue;
		}

		TPointerSceneNodeComponentBase pointerComponentBase(pointerVisual);

		GSceneNode& oldParent = *pointerNode;
		oldParent.RemoveComponent(pointerComponentBase);
		pointerVisual->SetFreeze(true);

		//rather harsh way of getting parts detached, making dummy ZombiePartDetach
		TPointerSceneNode pointerDetachSceneNode = mWeakSceneNode.lock();
		if (!pointerDetachSceneNode)
		{
			continue;
		}
		GSceneNode& detachSceneNode = *pointerDetachSceneNode;


		if (&oldParent == &zombieParentSceneNode)
		{
			detachSceneNode.AddComponent(pointerComponentBase);
		}
		else
		{
			TPointerSceneNode pointerNewSceneNode;
			pointerNewSceneNode.reset(new GSceneNode("detachLimbChild"));
			detachSceneNode.AddChildNode(pointerNewSceneNode);

			if (!pointerNewSceneNode)
			{
				continue;
			}
			GSceneNode& newSceneNode = *pointerNewSceneNode;
			newSceneNode.SetParentRelativeTransform(oldParent.GetWorldTransform() * mInvertWorldTransform);
			newSceneNode.AddComponent(pointerComponentBase);
			detachSceneNode.AddChildNode(pointerNewSceneNode);
		}
	}
	return;
}

/*
void ZombiePartDetach::TransferIntoSceneNode(
	GSceneNode& inout_node
	)
{
	for (TArrayPairWeakSceneNodeComponentVisualWeakSceneNode::iterator iterator = mArrayVisualNode.begin(); iterator != mArrayVisualNode.end(); ++iterator)
	{
		TPointerSceneNodeComponentVisual pointerVisual = (*iterator).first.lock();
		TPointerSceneNode pointerNode = (*iterator).second.lock();

		if (!pointerVisual || !pointerNode)
		{
			continue;
		}

		TPointerSceneNodeComponentBase pointerComponentBase(pointerVisual);

		GSceneNode& oldParent = *pointerNode;
		oldParent.RemoveComponent(pointerComponentBase);

		inout_node.AddComponent(pointerComponentBase);
		pointerVisual->SetFreeze(true);
	}

	return;
}
*/