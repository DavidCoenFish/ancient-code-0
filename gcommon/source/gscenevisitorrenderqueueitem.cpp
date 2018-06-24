//
//  GSceneVisitorRenderQueueItem.cpp
//
//  Created by David Coen on 10/09/30.
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneVisitorRenderQueueItem.h"

//constructor
GSceneVisitorRenderQueueItem::GSceneVisitorRenderQueueItem(
	const float in_sortValue,
	const GMatrix16Float& inout_worldTransform,
	const GMesh* const in_meshLoad,
	const void* const in_meshVertexData,
	const GMaterialInstance* const inout_material//,
	//const bool in_GetLight,
	//const TArrayPointerRenderLight* const in_arrayLight
	)
	: mQueueItemNext(NULL)
	, mSortValue(in_sortValue)
	, mWorldTransform(inout_worldTransform)
	, mMesh(in_meshLoad)
	, mMeshVertexData(in_meshVertexData)
	, mMaterial(inout_material)
	//, mGetLight(in_GetLight)
	//, mArrayLight(in_arrayLight)
{
	return;
}

GSceneVisitorRenderQueueItem::~GSceneVisitorRenderQueueItem()
{
	return;
}
