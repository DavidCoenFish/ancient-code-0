//
//  GSceneNodeUtility.cpp
//
//  Created by David Coen on 2011 01 29
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneNodeUtility.h"

#include "GScene.h"
#include "GSceneNode.h"
#include "GSceneNodeComponentVisual.h"

//public static methods
/*static*/ void GSceneNodeUtility::RecusiveSetVisualComponentVisible(TPointerSceneNode& inout_sceneNode, const bool in_visible)
{
	GSceneNodeUtilityVisitorVisiblity::Run(inout_sceneNode, in_visible);
	return;
}

/*static*/ GSceneNodeUtility::TPointerSceneNode GSceneNodeUtility::FindSceneNodeName(GScene& inout_scene, const std::string& in_name)
{
	return GSceneNodeUtilityVisitorName::Run(inout_scene, in_name);
}

/*static*/ GSceneNodeUtility::TPointerSceneNode GSceneNodeUtility::FindSceneNodeName(TPointerSceneNode& inout_sceneNode, const std::string& in_name)
{
	return GSceneNodeUtilityVisitorName::Run(inout_sceneNode, in_name);
}

/*static*/ void GSceneNodeUtilityVisitorVisiblity::Run(TPointerSceneNode& inout_sceneNode, const bool in_visible)
{
	GSceneNodeUtilityVisitorVisiblity visitor(in_visible);
	GSceneNode::QueryAll(inout_sceneNode, visitor, GSceneNodeComponentVisual::GetComponentFlag());
	return;
}

GSceneNodeUtilityVisitorVisiblity::GSceneNodeUtilityVisitorVisiblity(const bool in_visiblity)
: mVisible(in_visiblity)
{
	return;
}

GSceneNodeUtilityVisitorVisiblity::~GSceneNodeUtilityVisitorVisiblity()
{
	return;
}

//contract with with GSceneNode::QueryAll
void GSceneNodeUtilityVisitorVisiblity::QuerySubmitNode(TPointerSceneNode& inout_sceneNode)
{
	GSceneNode& sceneNode = *inout_sceneNode;
	const int componentCount = sceneNode.GetComponentCount();
	for (int index = 0; index < componentCount; ++index)
	{
		GSceneNodeComponentVisual* const pVisual = sceneNode.GetComponent<GSceneNodeComponentVisual>(index);
		if (NULL == pVisual)
		{
			continue;
		}

		pVisual->SetVisible(mVisible);
	}

	return;
}

/*static*/ GSceneNodeUtilityVisitorName::TPointerSceneNode GSceneNodeUtilityVisitorName::Run(GScene& inout_scene, const std::string& in_name)
{
	GSceneNodeUtilityVisitorName visitor(in_name);
	inout_scene.QueryAll(visitor, 0xFFFFFFFF);
	return visitor.mFoundSceneNode;
}

/*static*/ GSceneNodeUtilityVisitorName::TPointerSceneNode GSceneNodeUtilityVisitorName::Run(TPointerSceneNode& inout_sceneNode, const std::string& in_name)
{
	GSceneNodeUtilityVisitorName visitor(in_name);
	GSceneNode::QueryAll(inout_sceneNode, visitor, 0xFFFFFFFF);
	return visitor.mFoundSceneNode;
}

GSceneNodeUtilityVisitorName::GSceneNodeUtilityVisitorName(const std::string& in_name)
: mName(in_name)
, mFoundSceneNode()
, mFound(false)
{
	return;
}

GSceneNodeUtilityVisitorName::~GSceneNodeUtilityVisitorName()
{
	return;
}

//contract with with GSceneNode::QueryAll
void GSceneNodeUtilityVisitorName::QuerySubmitNode(TPointerSceneNode& inout_sceneNode)
{
	if (mFound)
	{
		return;
	}

	if (inout_sceneNode->GetName() == mName)
	{
		mFound = true;
		mFoundSceneNode = inout_sceneNode;
	}

	return;
}

void GSceneNodeUtilityVisitorName::QueryFinish()
{
	return;
}
