//
//  GSceneNodeComponentFade.cpp
//
//  Created by David Coen on 2011 02 08
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneNodeComponentFade.h"

#include "GScene.h"
#include "GSceneType.h"
#include "GComponentLerpFloat.h"
#include "GSceneNodeComponentFadeLoad.h"
#include "GSceneNode.h"
#include "GAssert.h"
#include "GMath.h"

//static public methods
/*static*/ GSceneNodeComponentFade::TPointerSceneNodeComponentBase GSceneNodeComponentFade::Factory(
	GScene& inout_scene, //access to mesh/ material manager
	TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
	const void* const in_componentData
	)
{
	TPointerSceneNodeComponentFade pointer;
	if (!in_componentData)
	{
		return pointer;
	}

	const GSceneNodeComponentFadeLoad& componentLoad = *((GSceneNodeComponentFadeLoad*)in_componentData);

	pointer.reset(new GSceneNodeComponentFade(
		inout_scene,
		componentLoad.GetDuration()
		));

	return pointer;
}


/*static*/ const unsigned int GSceneNodeComponentFade::GetComponentFlag()
{
	return GSceneType::TComponentFlag::TFade;
}

//constructor
GSceneNodeComponentFade::GSceneNodeComponentFade(
	GScene& inout_scene,
	const float in_duration
	)
: GSceneNodeComponentBase(GetComponentFlag())
, mScene(inout_scene)
, mLerpFade()
{
	mLerpFade.reset(new GComponentLerpFloat(1.0F, in_duration, 0.0F));
	return;
}

/*virtual*/ GSceneNodeComponentFade::~GSceneNodeComponentFade()
{
	return;
}

//public methods
void GSceneNodeComponentFade::Tick(TPointerSceneNode& inout_node, const float in_timeDelta)
{
	mLerpFade->Tick(in_timeDelta);
	if (!mLerpFade->GetActive())
	{
		mScene.SceneNodeRequestDelete(inout_node);
	}

	////debug: just mucking around
	//GMatrix16Float tempMatrix = inout_node->GetParentRelativeTransform();
	//GVector3Float pos = tempMatrix.GetPosition();
	//pos[1] -= in_timeDelta * 0.5F;
	//tempMatrix.SetPosition(pos);
	//inout_node->SetParentRelativeTransform(tempMatrix);

	return;
}
