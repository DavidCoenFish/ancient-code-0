//
//  GSceneNodeComponentLightCollector.cpp
//
//  Created by David Coen on 30/08/10.
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneNodeComponentLightCollector.h"

#include "GScene.h"
#include "GSceneType.h"
//#include "GRenderLight.h"

struct TLoadComponentLightCollector
{
	const unsigned int mUpdateFrequency;
	const unsigned int mUpdateOffset;
};

/*static*/ GSceneNodeComponentLightCollector::TPointerSceneNodeComponentBase GSceneNodeComponentLightCollector::Factory(
	GScene& inout_scene, //access to mesh/ material manager
	GSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
	const unsigned char* const in_memory, //the start of the object in memory, eg. somewhere from loadBuffer
	const GBuffer& in_loadBuffer //the rest of the load buffer to extract other poiners
	)
{
	TPointerSceneNodeComponentBase pointer;
	if (!in_memory)
	{
		return pointer;
	}

	const TLoadComponentLightCollector& loadComponentLightCollector = *(const TLoadComponentLightCollector*)in_memory;

	pointer.reset(new GSceneNodeComponentLightCollector(
		loadComponentLightCollector.mUpdateFrequency,
		loadComponentLightCollector.mUpdateOffset
		));

	return pointer;
}

/*static*/ const unsigned int GSceneNodeComponentLightCollector::GetComponentFlag()
{
	return GSceneType::TComponentFlag::TLightCollector;
}

//constructor
GSceneNodeComponentLightCollector::GSceneNodeComponentLightCollector(
	const unsigned int in_updateFrequency,
	const unsigned int in_updateOffset,
	const float in_radius
	)
	: GSceneNodeComponentBase(GetComponentFlag())
	, mUpdateFrequency(in_updateFrequency)
	, mUpdateOffset(in_updateOffset)
	, mRadius(in_radius)
	//, mArrayLight()
{
	//mArrayLight.resize();
	return;
}

/*virtual*/ GSceneNodeComponentLightCollector::~GSceneNodeComponentLightCollector()
{
	return;
}

//public methods
void GSceneNodeComponentLightCollector::Update(
	GScene& inout_scene,
	GSceneNode& inout_parentNode, //may need the scene node world matrix
	const unsigned int in_frameId
	)
{
	if (!NeedsUpdate(in_frameId))
	{
		return;
	}

	//GSceneVisitorCollectLight::Run(
	//	mArrayLight,
	//	inout_scene,
	//	inout_parentNode.GetWorldTransform(),
	//	mRadius
	//	);
	
	return;
}

//private methods
const bool GSceneNodeComponentLightCollector::NeedsUpdate(const int in_frameId)const
{
	if (0 == mUpdateFrequency)
	{
		//static, no updates
		if (0 == in_frameId)
		{
			//only update on first frame
			return true;
		}
		return false;
	}

	const bool update = (0 == ((in_frameId + mUpdateOffset) % mUpdateFrequency));
	return update;
}
