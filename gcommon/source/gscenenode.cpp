//
//  GSceneNode.cpp
//
//  Created by David Coen on 10/09/30.
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneNode.h"

#include "GScene.h"
#include "GSceneNodeLoad.h"
#include "GSceneNodeComponentBase.h"
#include "GScenePostLoadCallback.h"

static void LocalWriteFlag(unsigned int& inout_flag, const unsigned int in_bit, const bool in_active)
{
	if (in_active == (in_bit == (inout_flag & in_bit)))
	{
		return;
	}
	if (in_active)
	{
		inout_flag |= in_bit;
	}
	else
	{
		inout_flag &= ~in_bit;
	}
	return;
}

//static public methods
/*static*/ GSceneNode::TPointerSceneNode GSceneNode::Factory(
	GScene& inout_scene, //access to mesh/ material manager
	TArrayScenePostLoadCallback& inout_arrayPostLoad,
	const GSceneNodeLoad& in_sceneNodeLoad
	)
{
	TPointerSceneNode pointer;

	pointer.reset(new GSceneNode(in_sceneNodeLoad.GetName()));
	pointer->SetParentRelativeTransform(in_sceneNodeLoad.GetTransformParentRelative());

	FactoryLoadChildrenAndComponents(
		pointer,
		inout_scene,
		inout_arrayPostLoad,
		in_sceneNodeLoad
		);

	return pointer;
}

//exposed for sceneNodeFactory
/*static*/ void GSceneNode::FactoryLoadChildrenAndComponents(
	TPointerSceneNode& inout_owner,
	GScene& inout_scene, //access to mesh/ material manager
	TArrayScenePostLoadCallback& inout_arrayPostLoad,
	const GSceneNodeLoad& in_sceneNodeLoad
	)
{
	GSceneNode& sceneNode = *inout_owner;
	//create components
	for (int index = 0; index < in_sceneNodeLoad.GetComponentCount(); ++index)
	{
		TPointerSceneNodeComponentBase pointerComponent = inout_scene.LoadComponent(
			inout_owner,
			inout_arrayPostLoad,
			in_sceneNodeLoad.GetComponentLoad(index)
			);
		if (!pointerComponent)
		{
			continue;
		}
		sceneNode.mArrayComponent.push_back(pointerComponent);
		sceneNode.mFlag |= TFlag::TComponentFlagDirty;
	}

	//create children
	for (int index = 0; index < in_sceneNodeLoad.GetChildCount(); ++index)
	{
		TPointerSceneNode pointerChild = Factory(
			inout_scene,
			inout_arrayPostLoad,
			in_sceneNodeLoad.GetChild(index)
			);

		if (!pointerChild)
		{
			continue;
		}
		//sceneNode.mArrayChildren.push_back(pointerChild);
		//sceneNode.mFlag |= TFlag::TComponentFlagHierarchyDirty;
		sceneNode.AddChildNode(pointerChild);
	}

	return;
}

//constructor
GSceneNode::GSceneNode(
	const std::string& in_name
	)
	: mParent(NULL)
	, mName(in_name)
	, mParentRelativeTransform(GMatrix16Float::sIdentity)
	, mWorldTransform()
	, mFlag(TFlag::TWorldTransformDirty | TFlag::TWorldInvertDirty | TFlag::TComponentFlagDirty | TFlag::TComponentFlagHierarchyDirty)
	, mArrayComponent()
	, mArrayChildren()
	, mComponentComposititeFlag(0)
	, mChildComponentComposititeFlag(0)
{
	return;
}

GSceneNode::~GSceneNode()
{
	return;
}

//public methods
void GSceneNode::WriteFlagDownTree(const unsigned int in_flag, const bool in_active)
{
	LocalWriteFlag(mFlag, in_flag, in_active);

	//visit all children
	for (TArrayPointerSceneNode::iterator iterator = mArrayChildren.begin(); iterator != mArrayChildren.end(); ++iterator)
	{
		(*iterator)->WriteFlagDownTree(in_flag, in_active);
	}

	return;
}

void GSceneNode::WriteFlagUpTree(const unsigned int in_flag, const bool in_active)
{
	LocalWriteFlag(mFlag, in_flag, in_active);

	if (mParent)
	{
		mParent->WriteFlagUpTree(in_flag, in_active);
	}

	return;
}

//public accessors
const int GSceneNode::GetComponentCount()const
{
	return mArrayComponent.size();
}

const GMatrix16Float& GSceneNode::GetParentRelativeTransform()const
{
	return mParentRelativeTransform;
}

void GSceneNode::SetParentRelativeTransform(const GMatrix16Float& in_parentRelativeTransform)
{
	mParentRelativeTransform = in_parentRelativeTransform;
	WriteFlagDownTree(TFlag::TWorldInvertDirty | TFlag::TWorldTransformDirty, true);
	//WriteFlagDownTree(TFlag::TWorldTransformDirty, true);
	return;
}

const GMatrix16Float& GSceneNode::GetWorldTransform()
{
	if (0 != (mFlag & TFlag::TWorldTransformDirty))
	{
		if (mParent && (0 == (mFlag & TFlag::TDontInheritParentTransform)))
		{
			mWorldTransform = mParentRelativeTransform * mParent->GetWorldTransform();
		}
		else
		{
			mWorldTransform = mParentRelativeTransform;
		}

		LocalWriteFlag(mFlag, TFlag::TWorldTransformDirty, false);
	}

	return mWorldTransform;
}

void GSceneNode::SetWorldTransform(const GMatrix16Float& in_worldTransform)
{
	if (!mParent || (0 != (mFlag & TFlag::TDontInheritParentTransform)))
	{
		SetParentRelativeTransform(in_worldTransform);
		return;
	}
#if 0
	GMatrix16Float parentWorldTransform = mParent->GetWorldTransform();
	//do a cheap invert
	const GVector3Float parentTransformPosition = parentWorldTransform.GetPosition();
	parentWorldTransform.SetPosition(GVector3Float::sZero);
	parentWorldTransform.TransposeSelf();

	GMatrix16Float parentRelative = in_worldTransform * parentWorldTransform;
	parentRelative.SetPosition(parentRelative.GetPosition() - parentTransformPosition);

	SetParentRelativeTransform(parentRelative);
#else
	const GMatrix16Float& parentInvert = mParent->GetWorldTransformInvert();
	const GMatrix16Float parentRelative = in_worldTransform * parentInvert;
	SetParentRelativeTransform(parentRelative);
#endif

	return;
}

/**/
const GMatrix16Float& GSceneNode::GetWorldTransformInvert()
{
	if (0 != (mFlag & TFlag::TWorldInvertDirty))
	{
		mWorldTransformInvert = GetWorldTransform().ReturnInverse();

		LocalWriteFlag(mFlag, TFlag::TWorldInvertDirty, false);
	}

	return mWorldTransformInvert;
}
/**/

const unsigned int GSceneNode::GetComponentComposititeFlag()
{
	if (mFlag & TFlag::TComponentFlagDirty)
	{
		mFlag &= ~TFlag::TComponentFlagDirty;
		//iterate through the components
		mComponentComposititeFlag = 0;
		for (TArrayPointerSceneNodeComponentBase::const_iterator iterator = mArrayComponent.begin(); iterator != mArrayComponent.end(); ++iterator)
		{
			mComponentComposititeFlag |= (*iterator)->GetComponentFlag();
		}
	}
	return mComponentComposititeFlag;
}

const unsigned int GSceneNode::GetChildComponentComposititeFlag()
{
	if (mFlag & TFlag::TComponentFlagHierarchyDirty)
	{
		mFlag &= ~TFlag::TComponentFlagHierarchyDirty;
		//iterate through the children
		mChildComponentComposititeFlag = 0;
		for (TArrayPointerSceneNode::iterator iterator = mArrayChildren.begin(); iterator != mArrayChildren.end(); ++iterator)
		{
			mChildComponentComposititeFlag |= (*iterator)->GetComponentComposititeFlag();
			mChildComponentComposititeFlag |= (*iterator)->GetChildComponentComposititeFlag();
		}
	}
	return mChildComponentComposititeFlag;
}

void GSceneNode::AddChildNode(TPointerSceneNode& inout_childSceneNode)
{
	if (!inout_childSceneNode)
	{
		return;
	}

	//mFlag |= TFlag::TComponentFlagHierarchyDirty;
	WriteFlagUpTree(TFlag::TComponentFlagHierarchyDirty, true);
	mArrayChildren.push_back(inout_childSceneNode);
	inout_childSceneNode->mParent = this;

	return;
}

void GSceneNode::RemoveChild(const TPointerSceneNode& in_pointerSceneNode)
{
	TArrayPointerSceneNode::iterator iterator = mArrayChildren.begin(); 
	while (iterator != mArrayChildren.end())
	{
		if (in_pointerSceneNode == (*iterator))
		{
			iterator = mArrayChildren.erase(iterator);
		}
		else
		{
			++iterator;
		}
	}
	WriteFlagUpTree(TFlag::TComponentFlagHierarchyDirty, true);
	in_pointerSceneNode->mParent = 0;
	return;
}

void GSceneNode::AddComponent(TPointerSceneNodeComponentBase& inout_component)
{
	mFlag |= TFlag::TComponentFlagDirty;
	WriteFlagUpTree(TFlag::TComponentFlagHierarchyDirty, true);
	mArrayComponent.push_back(inout_component);
	return;
}

void GSceneNode::RemoveComponent(const TPointerSceneNodeComponentBase& in_component)
{
	mFlag |= TFlag::TComponentFlagDirty;
	TArrayPointerSceneNodeComponentBase::iterator iterator = mArrayComponent.begin(); 
	while (iterator != mArrayComponent.end())
	{
		if (in_component == (*iterator))
		{
			iterator = mArrayComponent.erase(iterator);
		}
		else
		{
			++iterator;
		}
	}
	WriteFlagUpTree(TFlag::TComponentFlagHierarchyDirty, true);
	return;
}

//teleport flag writes down tree
void GSceneNode::SetTeleportFlag(const bool in_state)
{
	WriteFlagDownTree(TFlag::TTeleport, in_state);
	return;
}

const bool GSceneNode::GetTeleportFlag()const
{
	return (0 != (mFlag & TFlag::TTeleport));
}

void GSceneNode::SetDontInheritParentTransform(const bool in_state)
{
	LocalWriteFlag(mFlag, TFlag::TDontInheritParentTransform, in_state);
}

const bool GSceneNode::GetDontInheritParentTransform()const
{
	return (0 != (mFlag & TFlag::TDontInheritParentTransform));
}
