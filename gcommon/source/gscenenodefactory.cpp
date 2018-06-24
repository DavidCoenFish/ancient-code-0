//
//  GSceneNodeFactory.cpp
//
//  Created by David Coen on 2010 12 29
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneNodeFactory.h"

#include "GFileManager.h"
#include "GBuffer.h"
#include "GBufferUtility.h"
#include "GSceneLoad.h"
#include "GSceneNodeLoad.h"

typedef std::map<std::string, const GSceneNodeLoad*> TMapStringSceneNodeLoad;

static void LocalMapSceneNodeLoad(
	TMapStringSceneNodeLoad& inout_mapNameSceneNodeLoad,
	const GSceneNodeLoad& in_sceneNodeLoad
	)
{
	//recurse
	for (int index = 0; index < in_sceneNodeLoad.GetChildCount(); ++index)
	{
		const GSceneNodeLoad& child = in_sceneNodeLoad.GetChild(index);
		LocalMapSceneNodeLoad(
			inout_mapNameSceneNodeLoad,
			child
			);
	}

	inout_mapNameSceneNodeLoad[in_sceneNodeLoad.GetName()] = &in_sceneNodeLoad;

	return;
}

//static public methods
/*static*/ GSceneNodeFactory::TPointerSceneNodeFactory GSceneNodeFactory::Factory(
	GFileManager& inout_fileManager,
	const std::string& in_factorySceneFileName
	)
{
	TPointerSceneNodeFactory pointer;
	{
		TWeakBuffer factoryAsset = inout_fileManager.LoadManagedFile(in_factorySceneFileName, true, true);
		TPointerBuffer pointerFactoryAsset = factoryAsset.lock();
		if (pointerFactoryAsset)
		{
			pointer.reset(new GSceneNodeFactory(pointerFactoryAsset));
		}
	}

	return pointer;
}

//constructor
GSceneNodeFactory::GSceneNodeFactory(TPointerBuffer& inout_sceneLoadAsset)
: mSceneLoadAsset(inout_sceneLoadAsset)
, mMapNameSceneNodeLoad()
{
	if (!inout_sceneLoadAsset)
	{
		return;
	}

	const GSceneLoad& sceneLoad = GBufferUtility::GetLoadBufferRoot<GSceneLoad>(*inout_sceneLoadAsset);
	for (int index = 0; index < sceneLoad.GetSceneNodeLoadCount(); ++index)
	{
		LocalMapSceneNodeLoad(
			mMapNameSceneNodeLoad,
			sceneLoad.GetSceneNodeLoad(index)
			);
	}

	return;
}

GSceneNodeFactory::~GSceneNodeFactory()
{
	return;
}

//public methods
const GSceneNodeLoad* const GSceneNodeFactory::GetSceneNodeLoad(const std::string& in_name)const
{
	TPointerBuffer pointerAsset = mSceneLoadAsset.lock();
	if (!pointerAsset)
	{
		return NULL;
	}

	TMapStringSceneNodeLoad::const_iterator iterator = mMapNameSceneNodeLoad.find(in_name);
	if (iterator != mMapNameSceneNodeLoad.end())
	{
		return (*iterator).second;
	}

	return NULL;
}
