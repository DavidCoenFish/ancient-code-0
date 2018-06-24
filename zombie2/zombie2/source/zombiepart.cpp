//
//  ZombiePart.cpp
//  Zombie
//
//  Created by David Coen on 2011 02 28
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "ZombiePart.h"

#include "ZombiePartLoad.h"
#include "ZombiePartDataLoad.h"
#include <GSceneNode.h>
#include <GMath.h>
#include <vector>
#include <string>
#include <GSceneNodeComponentVisual.h>

typedef std::pair<float, std::string> TPairFloatString;
typedef std::vector<TPairFloatString> TArrayPairFloatString;

static const int LocalGetVisualGroup(
	const ZombiePartLoad& in_zombiePartLoad, 
	const float in_health
	)
{
	const int count = in_zombiePartLoad.GetArrayDataCount();
	if (0 == count)
	{
		return 0;
	}
	for (int index = 0; index < count; ++index)
	{
		if (in_health <= in_zombiePartLoad.GetArrayData()[index].mValue )
		{
			return index;
		}
	}

	return count - 1;
}

//constructor
ZombiePart::ZombiePart(
	const ZombiePartLoad& in_zombiePartLoad,
	const float in_health
	)
	: mZombiePartLoad(in_zombiePartLoad)
	, mHealth(in_health)
	, mVisualGroup(LocalGetVisualGroup(in_zombiePartLoad, in_health))
{
	mArrayArrayVisual.resize(in_zombiePartLoad.GetArrayDataCount());
	return;
}

ZombiePart::~ZombiePart()
{
	return;
}

//public methods
void ZombiePart::ModifiyHealth(float& inout_damage)
{
	const float damage = GMath::Minimum(GMath::Maximum(0.0F, mHealth), -inout_damage);
	inout_damage += damage;
	mHealth -= damage;

	SetHealth(mHealth);
	return;
}

void ZombiePart::SetHealth(const float in_health)
{
	mHealth = in_health;
	const int visualGroup = LocalGetVisualGroup(mZombiePartLoad, mHealth);
	if (visualGroup != mVisualGroup)
	{
		//hide the old group
		for (TArrayPairWeakSceneNodeComponentVisualWeakSceneNode::iterator iterator = mArrayArrayVisual[mVisualGroup].begin(); iterator != mArrayArrayVisual[mVisualGroup].end(); ++iterator)
		{
			TPointerSceneNodeComponentVisual pointerVisual = (*iterator).first.lock();
			if (pointerVisual)
			{
				pointerVisual->SetVisible(false);
			}
		}

		mVisualGroup = visualGroup;

		//show the new group
		for (TArrayPairWeakSceneNodeComponentVisualWeakSceneNode::iterator iterator = mArrayArrayVisual[mVisualGroup].begin(); iterator != mArrayArrayVisual[mVisualGroup].end(); ++iterator)
		{
			TPointerSceneNodeComponentVisual pointerVisual = (*iterator).first.lock();
			if (pointerVisual)
			{
				pointerVisual->SetVisible(true);
			}
		}
	}
	return;
}

void ZombiePart::QuerySubmitNode(TPointerSceneNode& inout_sceneNode)
{
	GSceneNode& sceneNode = *inout_sceneNode;
	const int componentCount = sceneNode.GetComponentCount();
	for (int index = 0; index < componentCount; ++index)
	{
		TPointerSceneNodeComponentVisual pVisual = sceneNode.GetComponentPointer<GSceneNodeComponentVisual>(index);
		if (NULL == pVisual)
		{
			continue;
		}

		const std::string& data = pVisual->GetData();

		for (int subIndex = 0; subIndex < mZombiePartLoad.GetArrayDataCount(); ++subIndex)
		{
			if (data != mZombiePartLoad.GetArrayData()[subIndex].GetVisualData())
			{
				continue;
			}

			mArrayArrayVisual[subIndex].push_back(TPairWeakSceneNodeComponentVisualWeakSceneNode(
				pVisual,
				inout_sceneNode
				));

			if (subIndex == mVisualGroup)
			{
				pVisual->SetVisible(true);
			}
			else
			{
				pVisual->SetVisible(false);
			}

			break;
		}
	}


	return;
}


//to detach limb, we are going to take over the current visual nodes
void ZombiePart::StealCurrentVisual(TArrayPairWeakSceneNodeComponentVisualWeakSceneNode& out_arrayVisual)
{
	out_arrayVisual = mArrayArrayVisual[mVisualGroup];
	mArrayArrayVisual[mVisualGroup].clear();
	return;
}
