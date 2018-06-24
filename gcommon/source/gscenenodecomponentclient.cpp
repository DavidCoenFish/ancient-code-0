//
//  GSceneNodeComponentClient.cpp
//
//  Created by David Coen on 10/10/05.
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneNodeComponentClient.h"

#include "GScene.h"
#include "GSceneType.h"
#include "GSceneNodeComponentClientLoad.h"

//static public methods
/*static*/ GSceneNodeComponentClient::TPointerSceneNodeComponentBase GSceneNodeComponentClient::Factory(
	GScene& inout_scene, //access to mesh/ material manager
	TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
	const void* const in_componentData
	)
{
	TPointerSceneNodeComponentBase pointer;
	if (!in_componentData)
	{
		return pointer;
	}

	const GSceneNodeComponentClientLoad& clientLoad = *((GSceneNodeComponentClientLoad*)in_componentData);

	const std::string clientName = clientLoad.GetClientName();

	pointer = inout_scene.ClientComponentFactory(
		clientName,
		inout_owner,
		inout_arrayPostLoadCallback
		);

	return pointer;
}

/*static*/ const unsigned int GSceneNodeComponentClient::GetComponentFlag()
{
	return GSceneType::TComponentFlag::TClient;
}

//constructor
GSceneNodeComponentClient::GSceneNodeComponentClient()
: GSceneNodeComponentBase(GetComponentFlag())
{
	return;
}

/*virtual*/ GSceneNodeComponentClient::~GSceneNodeComponentClient()
{
	return;
}

//public methods
void GSceneNodeComponentClient::Tick(TPointerSceneNode& inout_sceneNode, const float in_timeDelta)
{
	OnTick(inout_sceneNode, in_timeDelta);
	return;
}

//private methods
/*virtual*/ void GSceneNodeComponentClient::OnTick(TPointerSceneNode& inout_sceneNode, const float in_timeDelta)
{
	//NOP
	return;
}
