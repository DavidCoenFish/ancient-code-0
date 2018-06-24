//
//  GSceneNodeComponentFactoryStub.cpp
//
//  Created by David Coen on 10/10/05.
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneNodeComponentFactoryStub.h"

#include "GScene.h"
#include "GSceneType.h"
#include "GSceneNodeComponentFactoryStubLoad.h"

//static public methods
/*static*/ GSceneNodeComponentFactoryStub::TPointerSceneNodeComponentBase GSceneNodeComponentFactoryStub::Factory(
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

	const GSceneNodeComponentFactoryStubLoad& factoryStubLoad = *((GSceneNodeComponentFactoryStubLoad *)in_componentData);
	std::string name;
	if (factoryStubLoad.GetName())
	{
		name = factoryStubLoad.GetName();
	}
	std::string data;
	if (factoryStubLoad.GetData())
	{
		data = factoryStubLoad.GetData();
	}

	inout_scene.SceneNodeFactory(
		name,
		data,
		inout_owner,
		inout_arrayPostLoadCallback
		);

	return pointer;
}

/*static*/ const unsigned int GSceneNodeComponentFactoryStub::GetComponentFlag()
{
	return GSceneType::TComponentFlag::TFactoryStub;
}