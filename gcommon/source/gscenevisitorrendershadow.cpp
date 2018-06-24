//
//  GSceneVisitorRenderShadow.cpp
//
//  Created by David Coen on 2011 04 02
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneVisitorRenderShadow.h"

#include "GScene.h"
#include "GSceneNode.h"
#include "GCamera.h"
#include "GRender.h"
#include "GSceneNodeComponentVisual.h"
#include "GMeshEdgeInstance.h"
#include "GMesh.h"
#include "GMeshManual.h"
#include "GMath.h"

//#define USE_SHADOW_LENGTH_HACK

//public static methods
/*static*/ void GSceneVisitorRenderShadow::Run(
	GScene& inout_scene,
	GRender& in_renderer,
	const GVector3Float& in_shadowDirection,
	const float in_shadowDistance,
	const GMaterialInstance& in_materialShadowFront,
	const GMaterialInstance& in_materialShadowBack,
	const GMaterialInstance& in_materialShadow,
	const GMeshManual& in_shadowMesh,
	GCamera& inout_worldCamera,
	GCamera& inout_shadowCamera	
	)
{
	GSceneVisitorRenderShadow visitor(in_renderer, in_shadowDirection, in_shadowDistance, in_materialShadowFront, in_materialShadowBack, in_materialShadow, in_shadowMesh, inout_worldCamera, inout_shadowCamera);
	inout_scene.QueryAll(
		visitor, 
		GSceneNodeComponentVisual::GetComponentFlag()
		);

	return;
}

/*static*/ void GSceneVisitorRenderShadow::Run(
	TPointerSceneNode& inout_sceneNode,
	GRender& in_renderer,
	const GVector3Float& in_shadowDirection,
	const float in_shadowDistance,
	const GMaterialInstance& in_materialShadowFront,
	const GMaterialInstance& in_materialShadowBack,
	const GMaterialInstance& in_materialShadow,
	const GMeshManual& in_shadowMesh,
	GCamera& inout_worldCamera,
	GCamera& inout_shadowCamera
	)
{
	GSceneVisitorRenderShadow visitor(in_renderer, in_shadowDirection, in_shadowDistance, in_materialShadowFront, in_materialShadowBack, in_materialShadow, in_shadowMesh, inout_worldCamera, inout_shadowCamera);
	GSceneNode::QueryAll<GSceneVisitorRenderShadow>(
		inout_sceneNode, 
		visitor, 
		GSceneNodeComponentVisual::GetComponentFlag()
		);

	return;
}

//constructor
GSceneVisitorRenderShadow::GSceneVisitorRenderShadow(
	GRender& in_renderer,
	const GVector3Float& in_shadowDirection,
	const float in_shadowDistance,
	const GMaterialInstance& in_materialShadowFront,
	const GMaterialInstance& in_materialShadowBack,
	const GMaterialInstance& in_materialShadow,
	const GMeshManual& in_shadowMesh,
	GCamera& inout_worldCamera,
	GCamera& inout_shadowCamera
	)
	: mRenderer(in_renderer)
	, mShadowDirection(in_shadowDirection)
	, mShadowDistance(in_shadowDistance)
	, mMaterialShadowFront(in_materialShadowFront)
	, mMaterialShadowBack(in_materialShadowBack)
	, mMaterialShadow(in_materialShadow)
	, mShadowMesh(in_shadowMesh)
	, mShadowCamera(inout_shadowCamera)
{
	mRenderer.SetCamera(inout_worldCamera);
	return;
}

GSceneVisitorRenderShadow::~GSceneVisitorRenderShadow()
{
	return;
}

//public methods
void GSceneVisitorRenderShadow::QuerySubmitNode(
	TPointerSceneNode& inout_node
	)
{
	GSceneNode& sceneNode = *inout_node;
	bool transformValid = false;
	GVector3Float objectSpaceDirection;


	const int componentCount = sceneNode.GetComponentCount();
	for (int index = 0; index < componentCount; ++index)
	{
		GSceneNodeComponentVisual* const pComponentVisual = sceneNode.GetComponent<GSceneNodeComponentVisual>(index);
		if ((pComponentVisual) &&
			(pComponentVisual->GetVisible()) &&
			(!pComponentVisual->GetOffscreen()) &&
			(pComponentVisual->GetMeshHasShadow())
			)
		{
			if (!transformValid)
			{
				mRenderer.SetObjectTransform(sceneNode.GetWorldTransform());
				objectSpaceDirection = GMatrix16FloatMultiplyNoTranslate(mShadowDirection, sceneNode.GetWorldTransformInvert());
				transformValid = true;
			}

			const GMesh* const pMesh = pComponentVisual->GetMesh();
			const void* const pVertexData = pComponentVisual->GetMeshVertexData();

			GMeshEdgeInstance& edgeInstance = *pComponentVisual->GetMeshEdgeInstance();
#ifdef USE_SHADOW_LENGTH_HACK
			const float shadowDistance = GMath::Maximum(0.0F, sceneNode.GetWorldTransform().GetPosition().m_y) + (pComponentVisual->GetRadius() * 1.4F) + 0.5F;
#else
			const float shadowDistance = mShadowDistance;
#endif
			edgeInstance.Update(
				*pMesh,
				pVertexData,
				objectSpaceDirection,
				shadowDistance
				);

			const GMesh& shadowMesh = edgeInstance.GetShadowMesh();

			mRenderer.SetMaterial(mMaterialShadowBack);
			mRenderer.DrawMesh(
				shadowMesh,
				shadowMesh.GetVertexData()
				);
			mRenderer.SetMaterial(mMaterialShadowFront);
			mRenderer.DrawMesh(
				shadowMesh,
				shadowMesh.GetVertexData()
				);
		}
	}

	return;
}

void GSceneVisitorRenderShadow::QueryFinish()
{
	mRenderer.SetCamera(mShadowCamera);
	mRenderer.SetObjectTransform(GMatrix16Float::sIdentity);
	mRenderer.SetMaterial(mMaterialShadow);
	mRenderer.DrawMesh(
		mShadowMesh.GetMesh(),
		mShadowMesh.GetMesh().GetVertexData()
		);

	return;
}
