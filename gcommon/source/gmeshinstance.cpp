//
//  GMeshInstance.cpp
//
//  Created by David Coen on 2010 12 27
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GMeshInstance.h"

#include "GBuffer.h"
#include "GBufferUtility.h"
#include "GMesh.h"
#include "GMeshVertexData.h"
#include "GMeshModifier.h"
#include "GMeshInstanceModifierCloth.h"
#include "GMeshInstanceModifierSkin.h"
#include "GMeshInstanceModifierMorph.h"
#include "GMeshModifierSkin.h"
#include "GMatrix16Float.h"
#include "GMeshModifierSkinBone.h"
#include "GMeshManual.h"

//constructor
GMeshInstance::GMeshInstance(TPointerBuffer& inout_buffer)
: mBuffer(inout_buffer)
, mMeshManual()
, mVertexData()
, mArrayModifier()
, mUseAssetMesh(true)
, mUseRenderMeshManual(false)
{
	Init();	
	return;
}

GMeshInstance::GMeshInstance(TPointerMeshManual& inout_meshManual)
: mBuffer()
, mMeshManual(inout_meshManual)
, mVertexData()
, mArrayModifier()
, mUseAssetMesh(false)
, mUseRenderMeshManual(true)
{
	Init();	
	return;
}

GMeshInstance::~GMeshInstance()
{
	return;
}

//public methods
const int GMeshInstance::GetMorphTargetCount()const
{
	//todo
	return 0;
}

const char* const GMeshInstance::GetMorphTargetName(const int in_morphTargetIndex)const
{
	//todo
	return 0;
}

void GMeshInstance::SetMorphTargetWeight(const int in_morphTargetIndex, const float in_weight)
{
	//todo
	return;
}

//skin
const char* const GMeshInstance::GetSkeletonName()const
{
	const GMesh* const pMesh = GetMesh();
	if (!pMesh)
	{
		return 0;
	}

	return pMesh->GetSkeletonName();
}

const bool GMeshInstance::GetHasBone(const std::string& in_boneName)
{
	const GMesh* const pMesh = GetMesh();
	if (!pMesh)
	{
		return false;
	}

	for (int modifierIndex = 0; modifierIndex < pMesh->GetModifierCount(); ++modifierIndex)
	{
		if (pMesh->GetArrayModifier()[modifierIndex].GetIsSkin())
		{
			const GMeshModifierSkin& skin = pMesh->GetArrayModifier()[modifierIndex].GetSkin();
			for (int boneIndex = 0; boneIndex < (int)skin.GetArrayBoneCount(); ++boneIndex)
			{
				if (in_boneName == skin.GetBone(boneIndex).GetBoneName())
				{
					return true;
				}
			}
		}
	}

	return false;
}

void GMeshInstance::SetSkeletonInstance(TPointerSkeletonInstance& inout_skeletonInstance)
{
	const GMesh* const pMesh = GetMesh();
	if (!pMesh)
	{
		return;
	}

	for (TArrayPointerRenderMeshInstanceModifier::iterator iterator = mArrayModifier.begin(); iterator != mArrayModifier.end(); ++iterator)
	{
		TPointerRenderMeshInstanceModifierSkin pointerSkin = boost::dynamic_pointer_cast<GMeshInstanceModifierSkin>(*iterator);
		if (pointerSkin)
		{
			pointerSkin->SetSkeletonInstance(
				inout_skeletonInstance,
				*pMesh
				);
		}

		TPointerRenderMeshInstanceModifierMorph pointerMorph = boost::dynamic_pointer_cast<GMeshInstanceModifierMorph>(*iterator);
		if (pointerMorph)
		{
			pointerMorph->SetSkeletonInstance(
				inout_skeletonInstance,
				*pMesh
				);

		}

	}

	return;
}

//tick cloth, jiggle... applies skinning
void GMeshInstance::Update(const float in_timeDelta, GSceneNode& in_sceneNode)
{
	if (!mVertexData)
	{
		return;
	}
	const GMesh* const pMesh = GetMesh();
	if (!pMesh)
	{
		return;
	}

	int flagWrittenStreams = 0;
	GMeshVertexData& renderMeshVertexData = *mVertexData;

	//start out with a clean vertex data
	memcpy(renderMeshVertexData.GetData(), pMesh->GetVertexData(), pMesh->GetVertexDataByteSize());

	for (TArrayPointerRenderMeshInstanceModifier::iterator iterator = mArrayModifier.begin(); iterator != mArrayModifier.end(); ++iterator)
	{
		TPointerRenderMeshInstanceModifier& modifier = (*iterator);
		if (!modifier)
		{
			continue;
		}
		modifier->TickApplyModifier(
			*mVertexData,
			flagWrittenStreams,
			//WARNING //HACK //force uniform frame rate, cloth going explodo on iPhone
			//0.0166667F, //in_timeDelta,
			std::min(0.01666667F, in_timeDelta),
			*pMesh,
			in_sceneNode
			);
	}

	return;
}

//public accessors
const void* const GMeshInstance::GetVertexData()const
{
	return mVertexData->GetData();
}

//private methods
void GMeshInstance::Init()
{
	const GMesh* const pMesh = GetMesh();
	if (!pMesh)
	{
		return;
	}
	const GMesh& mesh = *pMesh;

	mVertexData.reset(new GMeshVertexData(
		mesh.GetVertexDataByteSize(),
		mesh.GetVertexData()
		));

	//make modifiers to match
	const int count = mesh.GetModifierCount();
	for (int index = 0; index < count; ++index)
	{
		const GMeshModifier& modifier = mesh.GetArrayModifier()[index];
		if (modifier.GetIsCloth())
		{
			TPointerRenderMeshInstanceModifier pointer = GMeshInstanceModifierCloth::Factory(mesh, index);
			mArrayModifier.push_back(pointer);
		}
		else if (modifier.GetIsSkin())
		{
			TPointerRenderMeshInstanceModifier pointer = GMeshInstanceModifierSkin::Factory(mesh, index);
			mArrayModifier.push_back(pointer);
		}
		else if (modifier.GetIsMorph())
		{
			TPointerRenderMeshInstanceModifier pointer = GMeshInstanceModifierMorph::Factory(mesh, index);
			mArrayModifier.push_back(pointer);
		}
		else
		{
			//damn, to preserve index with mesh, but now we have NULL modifiers
			TPointerRenderMeshInstanceModifier pointer;
			mArrayModifier.push_back(pointer);
		}
	}

	return;
}

const GMesh* const GMeshInstance::GetMesh()const
{
	if (mUseAssetMesh)
	{
		TPointerBuffer buffer = mBuffer.lock();
		if (!buffer)
		{
			return NULL;
		}
		const GMesh& mesh = GBufferUtility::GetLoadBufferRoot<GMesh>(*buffer);
		return &mesh;
	}
	else if (mUseRenderMeshManual)
	{
		TPointerMeshManual pointerMeshManual = mMeshManual.lock();
		if (!pointerMeshManual)
		{
			return NULL;
		}
		const GMesh& mesh = pointerMeshManual->GetMesh();
		return &mesh;
	}
	return NULL;
}

const GMeshModifierSkinBone* const GMeshInstance::GetSkinBone(
	const int in_boneIndex, 
	int* const out_modifierIndex,
	int* const out_modifierBoneIndex	
	)const
{
	const GMesh* const pMeshLoad = GetMesh();
	if (!pMeshLoad)
	{
		return NULL;
	}

	const int modiferCount = pMeshLoad->GetModifierCount();
	int traceBoneIndex = in_boneIndex;
	for (int index = 0; index < modiferCount; ++index)
	{
		const GMeshModifier& modifierLoad = pMeshLoad->GetArrayModifier()[index];
		if (!modifierLoad.GetIsSkin())
		{
			continue;
		}
		const GMeshModifierSkin& skinLoad = modifierLoad.GetSkin();
		const int boneCount = skinLoad.GetArrayBoneCount();
		if (traceBoneIndex < boneCount)
		{
			if (out_modifierIndex)
			{
				*out_modifierIndex = index;
			}
			if (out_modifierBoneIndex)
			{
				*out_modifierBoneIndex = traceBoneIndex;
			}
			return &skinLoad.GetBone(traceBoneIndex);
		}
		traceBoneIndex -= boneCount;
	}

	return NULL;
}
