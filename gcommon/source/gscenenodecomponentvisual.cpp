//
//  GSceneNodeComponentVisual.cpp
//
//  Created by David Coen on 30/08/10.
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneNodeComponentVisual.h"

#include "GAssert.h"

#include "GFileManager.h"
#include "GSceneNodeComponentVisualLoad.h"
#include "GScene.h"
#include "GSceneType.h"
#include "GBuffer.h"
#include "GBufferUtility.h"
#include "GResourceManager.h"
#include "GFileSystem.h"
#include "GMaterial.h"
#include "GMaterialManager.h"
#include "GMesh.h"
#include "GMeshInstance.h"
#include "GMaterialInstance.h"
#include "GStringUtility.h"
#include "GCamera.h"
#include "GMath.h"
#include "GMeshManual.h"
#include "GMeshEdgeInstance.h"

////worst case of radius project onto plane, we could miss quiet a portion of the radius
////static const float sRadiusPadFactor = 1.293F; //(1 + (1 - (1 / sqrt(2))));
//hmmn, is this a figment of screen space being from 1 to -1? need to double the radius...
static const float sRadiusPadFactor = 2.6F;

typedef boost::shared_ptr<GRender> TPointerRenderComponent;

/*static*/ GSceneNodeComponentVisual::TPointerSceneNodeComponentBase GSceneNodeComponentVisual::Factory(
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
	const GSceneNodeComponentVisualLoad& componentLoad = *((GSceneNodeComponentVisualLoad*)in_componentData);

	pointer = Factory(
		inout_scene, //access to mesh/ material manager
		GStringUtility::SafeString(componentLoad.GetData()),
		GStringUtility::SafeString(componentLoad.GetMeshName()),
		GStringUtility::SafeString(componentLoad.GetMaterialName()),
		componentLoad.GetRadius(),
		componentLoad.GetFlag()
		);

	return pointer;
}

/*static*/ GSceneNodeComponentVisual::TPointerSceneNodeComponentBase GSceneNodeComponentVisual::Factory(
	GScene& inout_scene, //access to mesh/ material manager
	const std::string& in_data,
	const std::string& in_meshName,
	const std::string& in_materialName,
	const float in_radius,
	const int in_flag
	)
{
	TWeakBuffer weakMesh = inout_scene.GetResourceManager()->GetFileManager().LoadManagedFile(in_meshName, true, true);
	TPointerBuffer mesh = weakMesh.lock();
	TWeakMaterialInstance weakMaterial = inout_scene.GetResourceManager()->GetMaterialInstance(in_materialName, true, false);
	TPointerMaterialInstance material = weakMaterial.lock();

	TPointerSceneNodeComponentBase pointer;
	pointer.reset(new GSceneNodeComponentVisual(
		in_data,
		mesh,
		material,
		in_radius,
		in_flag
		));

	return pointer;

}

/*static*/ const unsigned int GSceneNodeComponentVisual::GetComponentFlag()
{
	return GSceneType::TComponentFlag::TVisual;
}

//constructor
GSceneNodeComponentVisual::GSceneNodeComponentVisual(
	const std::string& in_data,
	TPointerBuffer& in_mesh,
	TPointerMaterialInstance& in_materialInstance,
	const float in_radius,
	const int in_flag
	)
	: GSceneNodeComponentBase(GetComponentFlag())
	, mData(in_data)
	, mMesh(in_mesh)
	, mMaterialInstance(in_materialInstance)
	, mRadius(in_radius)
	, mFlag(in_flag)
{
	TPointerBuffer asset = mMesh.lock();
	if (asset)
	{
		mFlag |= TVisualFlag::TRenderMeshAsset;
	}

	return;
}

GSceneNodeComponentVisual::GSceneNodeComponentVisual(
	const std::string& in_data,
	TPointerMeshManual& in_meshManual,
	TPointerMaterialInstance& in_materialInstance,
	const float in_radius,
	const int in_flag
	)
	: GSceneNodeComponentBase(GetComponentFlag())
	, mData(in_data)
	, mMeshManual(in_meshManual)
	, mMaterialInstance(in_materialInstance)
	, mRadius(in_radius)
	, mFlag(in_flag)
{
	TPointerMeshManual pointerMesh = mMeshManual.lock();
	if (pointerMesh)
	{
		mFlag |= TVisualFlag::TRenderMeshManual;
	}

	return;
}

/*virtual*/ GSceneNodeComponentVisual::~GSceneNodeComponentVisual()
{
	return;
}

//public methods
void GSceneNodeComponentVisual::Tick(TPointerSceneNode& in_parent, const float in_timeDelta)
{
	if (!GetMeshHasModifier() || (!GetVisible()) || GetFreeze() || GetOffscreen())
	{
		return;
	}

	TPointerMeshInstance pointerMesh = GetMeshInstance();
	if (pointerMesh)
	{
		pointerMesh->Update(
			in_timeDelta, 
			*in_parent
			);
	}
}

void GSceneNodeComponentVisual::UpdateOffscreen(GSceneNode& in_parent, GCamera& in_camera)
{
	const GVector3Float result = in_parent.GetWorldTransform().GetPosition() * in_camera.GetProjectionMatrix();
	const float nearPlane = in_camera.GetNear();

	const float resultZ = result.m_z;
	if (resultZ + mRadius < nearPlane) //behind camera
	{
		SetOffscreen(true);
		return;
	}

	float screenX = GMath::Abs(result.m_x);
	float screenY = GMath::Abs(result.m_y);
	float screenRadius = mRadius * sRadiusPadFactor;

	if (resultZ)
	{
		const float mul = GMath::Abs(1.0F / resultZ);
		screenX *= mul;
		screenY *= mul;
		screenRadius *= mul;
	}

	if ((1.0F < (screenX - screenRadius)) ||
		(1.0F < (screenY - screenRadius)))
	{
		SetOffscreen(true);
		return;
	}

	SetOffscreen(false);
	return;
}

//public accessors
const bool GSceneNodeComponentVisual::GetVisible()const
{
	return (0 != (mFlag & TVisualFlag::TVisable));
}

void GSceneNodeComponentVisual::SetVisible(const bool in_visible)
{
	if (in_visible)
	{
		mFlag |= TVisualFlag::TVisable;
	}
	else
	{
		mFlag &= ~TVisualFlag::TVisable;
	}
	return;
}

const bool GSceneNodeComponentVisual::GetFreeze()const
{
	return (0 != (mFlag & TVisualFlag::TFreeze));
}

void GSceneNodeComponentVisual::SetFreeze(const bool in_freeze)
{
	if (in_freeze)
	{
		mFlag |= TVisualFlag::TFreeze;
	}
	else
	{
		mFlag &= ~TVisualFlag::TFreeze;
	}
	return;
}

const bool GSceneNodeComponentVisual::GetOffscreen()const
{
	return (0 != (mFlag & TVisualFlag::TOffscreen));
}

void GSceneNodeComponentVisual::SetOffscreen(const bool in_offscreen)
{
	if (in_offscreen)
	{
		mFlag |= TVisualFlag::TOffscreen;
	}
	else
	{
		mFlag &= ~TVisualFlag::TOffscreen;
	}
	return;
}

const bool GSceneNodeComponentVisual::GetHasAlpha()const
{
	TPointerMaterialInstance material = mMaterialInstance.lock();
	if (material)
	{
		//TODO: or object alpha != 1.0F
		return material->GetFlagAlphaBlend();
	}

	return false;
}

const bool GSceneNodeComponentVisual::GetHasLight()const
{
	TPointerMaterialInstance material = mMaterialInstance.lock();
	if (material)
	{
		return material->GetFlagUseLight();
	}

	return false;
}

const bool GSceneNodeComponentVisual::GetUseCameraTransform()const
{
	return (0 != (mFlag & TVisualFlag::TUseCameraTransform));
}

void GSceneNodeComponentVisual::SetUseCameraTransform(const bool in_useCameraTransform)
{
	if (in_useCameraTransform)
	{
		mFlag |= TVisualFlag::TUseCameraTransform;
	}
	else
	{
		mFlag &= ~TVisualFlag::TUseCameraTransform;
	}
	return;
}

const bool GSceneNodeComponentVisual::GetJustUsePosition()const
{
	return (0 != (mFlag & TVisualFlag::TJustUsePosition));
}

void GSceneNodeComponentVisual::SetJustUsePosition(const bool in_justUsePosition)
{
	if (in_justUsePosition)
	{
		mFlag |= TVisualFlag::TJustUsePosition;
	}
	else
	{
		mFlag &= ~TVisualFlag::TJustUsePosition;
	}
	return;
}

const bool GSceneNodeComponentVisual::GetMeshHasModifier()const
{
	const GMesh* const pMesh = GetMesh();
	if (pMesh)
	{
		return (0 != pMesh->GetModifierCount());
	}

	return false;
}

const bool GSceneNodeComponentVisual::GetMeshHasShadow()const
{
	const GMesh* const pMesh = GetMesh();
	if (pMesh)
	{
		return (NULL != pMesh->GetEdge());
	}

	return false;
}


// else provide public render method? render
const GMesh* const GSceneNodeComponentVisual::GetMesh()const
{
	if (0 != (TVisualFlag::TRenderMeshAsset & mFlag))
	{
		TPointerBuffer asset = mMesh.lock();
		if (asset)
		{
			const GMesh& mesh = GBufferUtility::GetLoadBufferRoot<GMesh>(*asset);
			return &mesh;
		}
	}
	else if (0 != (TVisualFlag::TRenderMeshManual & mFlag))
	{
		TPointerMeshManual meshManual = mMeshManual.lock();
		if (meshManual)
		{
			const GMesh& meshLoad = meshManual->GetMesh();
			return &meshLoad;
		}
	}

	return NULL;
}


const void* const GSceneNodeComponentVisual::GetMeshVertexData()const
{
	//if we have a GMeshInstance, return it's vertex data
	if (mMeshInstance)
	{
		return mMeshInstance->GetVertexData();
	}

	const GMesh* const pMesh = GetMesh();
	if (pMesh)
	{
		return pMesh->GetVertexData();
	}

	return NULL;
}

GSceneNodeComponentVisual::TPointerMaterialInstance GSceneNodeComponentVisual::GetMaterial()
{
	return mMaterialInstance.lock();
}
	
void GSceneNodeComponentVisual::SetMaterial(TPointerMaterialInstance& inout_material)
{
	mMaterialInstance = inout_material;
	return;
}

GSceneNodeComponentVisual::TPointerMeshInstance& GSceneNodeComponentVisual::GetMeshInstance()
{
	if (!mMeshInstance)
	{
		if (0 != (TVisualFlag::TRenderMeshAsset & mFlag))
		{
			TPointerBuffer asset = mMesh.lock();
			if (asset)
			{
				mMeshInstance.reset(new GMeshInstance(asset));
			}
		}
		else if (0 != (TVisualFlag::TRenderMeshManual & mFlag))
		{
			TPointerMeshManual meshManual = mMeshManual.lock();
			if (meshManual)
			{
				mMeshInstance.reset(new GMeshInstance(meshManual));
			}
		}
	}

	return mMeshInstance;
}

GSceneNodeComponentVisual::TPointerMeshEdgeInstance& GSceneNodeComponentVisual::GetMeshEdgeInstance()
{
	if (!mMeshEdgeInstance)
	{
		const GMesh* const pMesh = GetMesh();
		if (pMesh)
		{
			mMeshEdgeInstance.reset(new GMeshEdgeInstance(*pMesh));
		}
	}

	return mMeshEdgeInstance;
}

