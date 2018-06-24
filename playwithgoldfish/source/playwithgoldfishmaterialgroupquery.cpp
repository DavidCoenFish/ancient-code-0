//
//  PlayWithGoldfishMaterialGroupQuery.cpp
//  playwithgoldfish
//
//  Created by David Coen on 2011 01 10
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "PlayWithGoldfishMaterialGroupQuery.h"

#include <GScene.h>
#include <GSceneNode.h>
#include <GSceneNodeComponentVisual.h>
#include <GCommon.h>
#include <GMaterialGroupManager.h>
#include <GRenderMaterial.h>

typedef boost::shared_ptr<GSceneNodeComponentVisual> TPointerSceneNodeComponentVisual;

//public static methods
/*static*/ void PlayWithGoldfishMaterialGroupQuery::Run(
	GScene& inout_scene, 
	GMaterialGroupManager& inout_materialGroupManager,
	TArrayPairStringWeakRenderMaterial& in_arrayMaterial
	)
{
	PlayWithGoldfishMaterialGroupQuery query(inout_materialGroupManager, in_arrayMaterial);
	inout_scene.QueryAll(
		query, 
		GSceneNodeComponentVisual::GetComponentFlag()
		);
	return;
}

//constructor
PlayWithGoldfishMaterialGroupQuery::PlayWithGoldfishMaterialGroupQuery(
	GMaterialGroupManager& inout_materialGroupManager,
	TArrayPairStringWeakRenderMaterial& in_arrayMaterial
	)
: mMaterialGroupManager(inout_materialGroupManager)
, mArrayMaterial(in_arrayMaterial)
{
	return;
}

PlayWithGoldfishMaterialGroupQuery::~PlayWithGoldfishMaterialGroupQuery()
{
	return;
}

//public methods
void PlayWithGoldfishMaterialGroupQuery::QuerySubmitNode(
	GSceneNode& inout_node
	)
{
	//now search the visuals
	const int componentCount = inout_node.GetComponentCount();
	for (int subIndex = 0; subIndex < componentCount; ++subIndex)
	{
		TPointerSceneNodeComponentVisual pointerVisual = inout_node.GetComponentPointer<GSceneNodeComponentVisual>(subIndex);
		if (!pointerVisual)
		{
			continue;
		}

		TPointerRenderMaterial pointerMaterial = pointerVisual->GetMaterial();

		for (TArrayPairStringWeakRenderMaterial::iterator iterator = mArrayMaterial.begin(); iterator != mArrayMaterial.end(); ++iterator)
		{
			if (pointerMaterial != (*iterator).second.lock())
			{
				continue;
			}

			mMaterialGroupManager.RegisterClient(
				pointerVisual,
				(*iterator).first
				);
		}
	}

	return;
}

void PlayWithGoldfishMaterialGroupQuery::QueryFinish()
{
	return;
}
