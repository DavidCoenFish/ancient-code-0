//
//  GSceneVisitorFustrum.cpp
//
//  Created by David Coen on 2011 02 15
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneVisitorFustrum.h"

#include "GScene.h"
#include "GSceneNode.h"
#include "GCamera.h"
#include "GSceneNodeComponentVisual.h"

//public static methods
/*static*/ void GSceneVisitorFustrum::Run(
	GScene& inout_scene,
	GCamera& in_camera		
	)
{
	GSceneVisitorFustrum visitor(in_camera);
	inout_scene.QueryAll(
		visitor, 
		GSceneNodeComponentVisual::GetComponentFlag()
		);

	return;
}

//constructor
GSceneVisitorFustrum::GSceneVisitorFustrum(
	GCamera& in_camera
	)
	: mCamera(in_camera)
{
	return;
}

GSceneVisitorFustrum::~GSceneVisitorFustrum()
{
	return;
}

//public methods
void GSceneVisitorFustrum::QuerySubmitNode(
	TPointerSceneNode& inout_node
	)
{
	GSceneNode& sceneNode = *inout_node;
	const int componentCount = sceneNode.GetComponentCount();
	for (int index = 0; index < componentCount; ++index)
	{
		GSceneNodeComponentVisual* const pComponentVisual = sceneNode.GetComponent<GSceneNodeComponentVisual>(index);
		if ((pComponentVisual) &&
			(pComponentVisual->GetVisible()))
		{
			pComponentVisual->UpdateOffscreen(
				sceneNode,
				mCamera
				);
		}
	}

	return;
}

void GSceneVisitorFustrum::QueryFinish()
{
	return;
}