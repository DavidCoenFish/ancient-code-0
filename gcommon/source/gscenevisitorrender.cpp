//
//  GSceneVisitorRender.cpp
//
//  Created by David Coen on 10/09/30.
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneVisitorRender.h"

#include "GScene.h"
#include "GSceneNode.h"
#include "GSceneVisitorRenderQueueItem.h"
#include "GCamera.h"
#include "GRender.h"
#include "GSceneNodeComponentVisual.h"
#include "GSceneNodeComponentLightCollector.h"

//public static methods
/*static*/ void GSceneVisitorRender::Run(
	GScene& inout_scene,
	GRender& in_renderer,
	GCamera& in_camera		
	)
{
	GSceneVisitorRender visitor(in_renderer, in_camera);
	inout_scene.QueryAll(
		visitor, 
		GSceneNodeComponentVisual::GetComponentFlag()
		);

	return;
}

/*static*/ void GSceneVisitorRender::Run(
	TPointerSceneNode& inout_sceneNode,
	GRender& in_renderer,
	GCamera& in_camera		
	)
{
	GSceneVisitorRender visitor(in_renderer, in_camera);
	GSceneNode::QueryAll<GSceneVisitorRender>(
		inout_sceneNode, 
		visitor, 
		GSceneNodeComponentVisual::GetComponentFlag()
		);

	return;
}

//constructor
GSceneVisitorRender::GSceneVisitorRender(
	GRender& in_renderer,
	GCamera& in_camera
	)
	: mRender(in_renderer)
	, mCamera(in_camera)
	, mCameraPosition(in_camera.GetTransformPos())
	, mCameraDirection(in_camera.GetTransformAt())
	, mRenderQueueItem(NULL)
{
	mRender.SetCamera(in_camera);
	return;
}

GSceneVisitorRender::~GSceneVisitorRender()
{
	return;
}

//public methods
void GSceneVisitorRender::QuerySubmitNode(
	TPointerSceneNode& inout_node
	)
{
	GSceneNode& sceneNode = *inout_node;
	const int componentCount = sceneNode.GetComponentCount();
	for (int index = 0; index < componentCount; ++index)
	{
		GSceneNodeComponentVisual* const pComponentVisual = sceneNode.GetComponent<GSceneNodeComponentVisual>(index);
		if ((pComponentVisual) &&
			(pComponentVisual->GetVisible()) &&
			(!pComponentVisual->GetOffscreen())
			)
		{
			RenderComponentVisual(
				sceneNode,
				*pComponentVisual
				);
			continue;
		}
	}

	return;
}

void GSceneVisitorRender::RenderComponentVisual(
	GSceneNode& inout_node,
	GSceneNodeComponentVisual& in_componentVisual
	)
{
	GMatrix16Float worldMatrix; //argh
	
	if (in_componentVisual.GetUseCameraTransform())
	{
		//TODO: we don't have the camera transform at the moment
		//if (in_componentVisual.GetJustUsePosition())
		//{
			worldMatrix = inout_node.GetWorldTransform();
			worldMatrix.SetPosition(GVector3Float(mCameraPosition.m_x, 0.0F, mCameraPosition.m_z));
		//}
	}
	else if (in_componentVisual.GetJustUsePosition())
	{
		worldMatrix = GMatrix16Float::sIdentity;
		worldMatrix.SetPosition(inout_node.GetWorldTransform().GetPosition());
	}
	else
	{
		worldMatrix = inout_node.GetWorldTransform();
	}

	//const float componentRadius = in_componentVisual.GetRadius();
	const GVector3Float position = worldMatrix.GetPosition();
	//const float alphaSortValue = DotProduct(mCameraDirection, position - mCameraPosition) + componentRadius;
	//const float alphaSortValue = DotProduct(mCameraDirection, position - mCameraPosition);
	const float alphaSortValue = DotProduct(mCameraDirection, mCameraPosition - position);
	//if (alphaSortValue < (componentRadius * (-2.0F)))
	//{
	//	return;
	//}

	if (in_componentVisual.GetHasAlpha())
	{
		TPointerMaterialInstance pointerMaterial = in_componentVisual.GetMaterial();
		RenderObjectAlpha(
			worldMatrix,
			in_componentVisual.GetMesh(),
			in_componentVisual.GetMeshVertexData(),
			pointerMaterial.get(),
			alphaSortValue,
			//in_componentVisual.GetHasLight(),
			inout_node
			);
	}
	else
	{
		TPointerMaterialInstance pointerMaterial = in_componentVisual.GetMaterial();
		RenderObject(
			worldMatrix,
			in_componentVisual.GetMesh(),
			in_componentVisual.GetMeshVertexData(),
			pointerMaterial.get(),
			//in_componentVisual.GetHasLight(),
			inout_node
			);
	}

	return;
}

void GSceneVisitorRender::RenderObject(
	const GMatrix16Float& in_worldTransform,
	const GMesh* const in_meshLoad,
	const void* const in_meshVertexData,
	const GMaterialInstance* const in_material,
	//const bool in_GetLight,
	GSceneNode& inout_node
	)
{
	//const TArrayPointerRenderLight* pArrayLight = NULL;
	//if (in_GetLight)
	//{
	//	GSceneNodeComponentLightCollector* const pLightCollector = inout_node.GetComponent<GSceneNodeComponentLightCollector>();
	//	if (pLightCollector)
	//	{
	//		pArrayLight = &pLightCollector->GetLightArray();
	//	}
	//}

	Render(
		in_worldTransform,
		//in_GetLight,
		//pArrayLight,
		in_meshLoad,
		in_meshVertexData,
		in_material
		);

	return;
}

void GSceneVisitorRender::RenderObjectAlpha(
	const GMatrix16Float& in_worldTransform,
	const GMesh* const in_meshLoad,
	const void* const in_meshVertexData,
	const GMaterialInstance* const in_material,
	const float in_alphaSortValue,
	//const bool in_GetLight,
	GSceneNode& inout_node
	)
{
	if ((!in_material) ||
		(!in_meshLoad) ||
		(!in_meshVertexData))
	{
		return;
	}

	//const TArrayPointerRenderLight* arrayLight = NULL;
	//if (in_GetLight)
	//{
		//GSceneNodeComponentLightCollector* const pLightCollector = inout_node.GetComponent<GSceneNodeComponentLightCollector>();
		//if (pLightCollector)
		//{
		//	arrayLight = &pLightCollector->GetLightArray();
		//}
	//}

	GSceneVisitorRenderQueueItem* pItem = new GSceneVisitorRenderQueueItem(
		in_alphaSortValue, 
		in_worldTransform,
		in_meshLoad,
		in_meshVertexData,
		in_material
		//in_GetLight,
		//arrayLight
		);

	if (!mRenderQueueItem)
	{
		//nothing on list
		mRenderQueueItem = pItem;
	}
	else
	{
		GSceneVisitorRenderQueueItem* pTraceItem = mRenderQueueItem;
		//render furtest to nearest, so big value at front of que
		if (pTraceItem->GetSortValue() < in_alphaSortValue)
		{
			pItem->SetQueueItemNext(mRenderQueueItem);
			mRenderQueueItem = pItem;
		}
		else
		{
			GSceneVisitorRenderQueueItem* pTraceNext = pTraceItem->GetQueueItemNext();
			while (pTraceNext)
			{
				if (pTraceNext->GetSortValue() <= in_alphaSortValue)
				{
					break;
				}
				pTraceItem = pTraceNext;
				pTraceNext = pTraceItem->GetQueueItemNext();
			}

			pItem->SetQueueItemNext(pTraceNext);
			pTraceItem->SetQueueItemNext(pItem);
		}
	}

	return;
}

void GSceneVisitorRender::QueryFinish()
{
	//draw all the render queue items
	GSceneVisitorRenderQueueItem* pTraceItem = mRenderQueueItem;
	while (pTraceItem)
	{
		GSceneVisitorRenderQueueItem* const pTraceNext = pTraceItem->GetQueueItemNext();

		Render(
			pTraceItem->GetWorldTransform(),
			//pTraceItem->GetLight(),
			//pTraceItem->GetArrayLight(),
			pTraceItem->GetMesh(),
			pTraceItem->GetMeshVertexData(),
			pTraceItem->GetMaterial()
			);

		delete pTraceItem;
		pTraceItem = pTraceNext;
	}

	mRenderQueueItem = NULL;

	return;
}

void GSceneVisitorRender::Render(
	const GMatrix16Float& in_worldTransform,
	//const bool in_GetLight,
	//const TArrayPointerRenderLight* const in_arrayLight,
	const GMesh* const in_meshLoad,
	const void* const in_meshVertexData,
	const GMaterialInstance* const in_material
	)
{
	if ((!in_meshLoad) || (!in_meshVertexData) || (!in_material))
	{
		return;
	}

	mRender.SetMaterial(*in_material);
	mRender.SetObjectTransform(in_worldTransform);

	//if (in_GetLight && in_arrayLight)
	//{
	//	mRender.SetLight(
	//		*in_arrayLight,
	//		GColour4Float::sBlack
	//		);
	//}
	//else
	//{
	//	mRender.SetLightDisabled();
	//}


	mRender.DrawMesh(
		*in_meshLoad,
		in_meshVertexData
		);

	return;
}
