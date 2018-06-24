//
//  GMeshInstanceModifierSkin.cpp
//
//  Created by David Coen on 2010 12 27
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GMeshInstanceModifierSkin.h"

#include "GMeshModifierSkin.h"
#include "GMeshModifierSkinBone.h"
#include "GMesh.h"
#include "GMeshModifier.h"
#include "GAssert.h"
#include "GMeshStreamInfo.h"
#include "GMeshVertexData.h"
#include "GMeshStreamInfo.h"
#include "GMeshModifierSkinTargetStream.h"
#include "GMeshModifierSkinBoneVertexData.h"
#include "GScratchPadLock.h"
#include "GSkeletonInstance.h"

typedef boost::shared_ptr<GSkeletonInstance> TPointerSkeletonInstance;
typedef std::vector<int> TArrayInt;

static void LocalPopulateScratchPad(
	float* const inout_scatchpad,
	const bool in_streamWritten,
	const bool in_isPoint,
	const GMesh& in_meshLoad,
	GMeshVertexData& inout_vertexData,
	const GMeshModifierSkin& in_modifierSkinLoad,
	const GMeshStreamInfo& in_streamInfoLoad
	)
{
	unsigned int arrayVertexIndexCount = 0;
	const unsigned int* arrayVertexIndex = 0;
	if (in_isPoint)
	{
		arrayVertexIndexCount = in_modifierSkinLoad.GetPointArrayCount();
		arrayVertexIndex = in_modifierSkinLoad.GetPointArrayPairIndexDuplicateOffset();
	}
	else
	{
		arrayVertexIndexCount = in_modifierSkinLoad.GetVectorArrayCount();
		arrayVertexIndex = in_modifierSkinLoad.GetVectorArrayPairIndexDuplicateOffset();
	}

	const void* vertexDataSource = in_meshLoad.GetVertexData();
	if (in_streamWritten)
	{
		vertexDataSource = inout_vertexData.GetData();
	}

	for (unsigned int index = 0; index < arrayVertexIndexCount; ++index)
	{
		const int subIndexCount = in_streamInfoLoad.GetCount();
		for (int subIndex = 0; subIndex < 3; ++subIndex)
		{
			float value = 0.0F;
			const int magicIndex = arrayVertexIndex[(index * 2)];
			
			if (subIndex < subIndexCount) //for 2d pos case
			{
				value = GMeshVertexData::GetItem<float>(
					vertexDataSource, 
					magicIndex,
					in_meshLoad.GetVertexByteStride(),
					in_streamInfoLoad.GetVertexDataByteOffset(),
					subIndex
					);
			}
			inout_scatchpad[(magicIndex * 3) + subIndex] = value;

			//populate destination with source as default (only needed if stream has not already been written to
			if (!in_streamWritten)
			{
				GMeshVertexData::GetItemRef<float>(
					inout_vertexData.GetData(),
					magicIndex,
					in_meshLoad.GetVertexByteStride(),
					in_streamInfoLoad.GetVertexDataByteOffset(),
					subIndex
					) = value;
			}
		}
	}

	return;
}

//apply skinning
static void LocalApplySkinning(
	GMeshVertexData& inout_vertexData,
	const float* const in_scatchpad,
	const GMesh& in_meshLoad,
	const bool in_isPoint,
	const GMeshModifierSkin& in_modifierSkinLoad,
	TPointerSkeletonInstance& in_skeletonInstance,
	const GMeshStreamInfo& in_streamInfoLoad,
	const TArrayInt& in_mapBoneIndex
	)
{
	for (int boneIndex = 0; boneIndex < (int)in_modifierSkinLoad.GetArrayBoneCount(); ++boneIndex)
	{
		const GMeshModifierSkinBone& boneLoad = in_modifierSkinLoad.GetBone(boneIndex);
		int boneVertexCount = 0;
		const GMeshModifierSkinBoneVertexData* boneVertexArray = 0;
		if (in_isPoint)
		{
			boneVertexCount = boneLoad.GetPointArrayVertexDataLoadCount();
			boneVertexArray = boneLoad.GetPointArrayVertexDataLoad();
		}
		else
		{
			boneVertexCount = boneLoad.GetVectorArrayVertexDataLoadCount();
			boneVertexArray = boneLoad.GetVectorArrayVertexDataLoad();
		}

		//bale now if bone influences no vertes
		if (boneVertexCount <= 0)
		{
			continue;
		}

		const int skeletonBoneIndex = in_mapBoneIndex[boneIndex];
		if (GCOMMON_INVALID_INDEX == skeletonBoneIndex)
		{
			continue;
		}
		
		const GMatrix16Float boneTransform = boneLoad.GetInvertBindMatrix() * in_skeletonInstance->GetBoneObjectMatrix(skeletonBoneIndex);
		GVector3Float result;
		for (int boneVertexIndex = 0; boneVertexIndex < boneVertexCount; ++boneVertexIndex)
		{
			const int vertexIndex = boneVertexArray[boneVertexIndex].GetVertexTargetIndex();
			const float weight = boneVertexArray[boneVertexIndex].GetWeight();
			const GVector3Float source(&(in_scatchpad[0]) + (vertexIndex * 3));
			if (in_isPoint)
			{
				result = source * boneTransform;
			}
			else
			{
				result = GMatrix16FloatMultiplyNoTranslate(source, boneTransform);
			}

			GVector3Float& resultFinial = *(GVector3Float*)(&GMeshVertexData::GetItemRef<float>(
				inout_vertexData.GetData(),
				vertexIndex,
				in_meshLoad.GetVertexByteStride(),
				in_streamInfoLoad.GetVertexDataByteOffset(),
				0
				));

			const int subIndexCount = in_streamInfoLoad.GetCount();
			//for (int subIndex = 0; subIndex < subIndexCount; ++subIndex)
			//{
			//	resultFinial[subIndex] += ((result[subIndex] - source[subIndex]) * weight);
			//}
			//GASSERT((2 == subIndexCount) || (3 == subIndexCount), "currently making an assumption that subIndexCount will support 2 or 3");
			if (1.0F == weight)
			{
				resultFinial.m_x = result.m_x;
				resultFinial.m_y = result.m_y;
				if (3 == subIndexCount)
				{
					resultFinial.m_z = result.m_z;
				}
			}
			else
			{
				resultFinial.m_x += ((result.m_x - source.m_x) * weight);
				resultFinial.m_y += ((result.m_y - source.m_y) * weight);
				if (3 == subIndexCount)
				{
					resultFinial.m_z += ((result.m_z - source.m_z) * weight);
				}
			}
		}
	}

	return;
}

static void LocalApplyDuplicates(
	GMeshVertexData& inout_vertexData,
	const float* const in_scatchpad,
	const GMesh& in_meshLoad,
	const bool in_isPoint,
	const GMeshModifierSkin& in_modifierSkinLoad,
	const GMeshStreamInfo& in_streamInfoLoad
	)
{
	unsigned int arrayCount = 0;
	const unsigned int* arrayPairVertexIndexDuplicateOffset = 0;
	const unsigned int* arrayDuplicate = 0;

	if (in_isPoint)
	{
		arrayCount = in_modifierSkinLoad.GetPointArrayCount();
		arrayPairVertexIndexDuplicateOffset = in_modifierSkinLoad.GetPointArrayPairIndexDuplicateOffset();
		arrayDuplicate = in_modifierSkinLoad.GetPointArrayDuplicate();
	}
	else
	{
		arrayCount = in_modifierSkinLoad.GetVectorArrayCount();
		arrayPairVertexIndexDuplicateOffset = in_modifierSkinLoad.GetVectorArrayPairIndexDuplicateOffset();
		arrayDuplicate = in_modifierSkinLoad.GetVectorArrayDuplicate();
	}

	if (!arrayDuplicate)
	{
		return;
	}

	const int streamDataCount = in_streamInfoLoad.GetCount();

	for (unsigned int index = 0; index < arrayCount; ++index)
	{
		const int duplicateOffset = (int)arrayPairVertexIndexDuplicateOffset[(index * 2) + 1];
		if (-1 == duplicateOffset)
		{
			continue;
		}

		const unsigned int sourceIndex = arrayPairVertexIndexDuplicateOffset[(index * 2)];
		const unsigned int duplicateCount = arrayDuplicate[duplicateOffset];
		for (unsigned int subIndex = 0; subIndex < duplicateCount; ++subIndex)
		{
			const unsigned int destinationIndex = arrayDuplicate[duplicateOffset + subIndex + 1];

			float* const pDestination= (float*)(&GMeshVertexData::GetItemRef<float>(
				inout_vertexData.GetData(),
				destinationIndex,
				in_meshLoad.GetVertexByteStride(),
				in_streamInfoLoad.GetVertexDataByteOffset(),
				0
				));
			//const float* const pSource = &in_scatchpad[sourceIndex * 3];
			const float* const pSource = (float*)(&GMeshVertexData::GetItemRef<float>(
				inout_vertexData.GetData(),
				sourceIndex,
				in_meshLoad.GetVertexByteStride(),
				in_streamInfoLoad.GetVertexDataByteOffset(),
				0
				));
			for (int subSubIndex = 0; subSubIndex < streamDataCount; ++subSubIndex)
			{
				pDestination[subSubIndex] = pSource[subSubIndex];
			}
		}
	}

	return;
}

//static public methods
/*static*/ GMeshInstanceModifierSkin::TPointerRenderMeshInstanceModifierSkin GMeshInstanceModifierSkin::Factory(
	const GMesh& in_meshLoad,
	const int in_modifierIndex
	)
{
	TPointerRenderMeshInstanceModifierSkin pointer;
	pointer.reset(new GMeshInstanceModifierSkin(
		in_meshLoad.GetArrayModifier()[in_modifierIndex].GetSkin().GetArrayBoneCount(),
		in_modifierIndex
		));

	return pointer;
}

//constructor
GMeshInstanceModifierSkin::GMeshInstanceModifierSkin(
	const int in_arrayBoneCount,
	const int in_modifierIndex
	)
: mModifierIndex(in_modifierIndex)
, mArraySkeletonBoneIndex()
{
	mArraySkeletonBoneIndex.resize(in_arrayBoneCount);
	for (TArrayInt::iterator iterator = mArraySkeletonBoneIndex.begin(); iterator != mArraySkeletonBoneIndex.end(); ++iterator)
	{
		(*iterator) = GCOMMON_INVALID_INDEX;
	}

	return;
}

GMeshInstanceModifierSkin::~GMeshInstanceModifierSkin()
{
	return;
}

//implement GMeshInstanceModifier
/*virtual*/ void GMeshInstanceModifierSkin::OnTickApplyModifier(
	GMeshVertexData& inout_vertexData,
	int& inout_flagWrittenStreams,
	const float in_timeDelta,
	const GMesh& in_meshLoad,
	GSceneNode& in_sceneNode
	)
{
	GScratchPadLock<float> scatchpad(in_meshLoad.GetElementCount() * 3);
	float* const pScatchpad = scatchpad.GetMemory();

	TPointerSkeletonInstance skeletonInstance = mSkeletonInstance.lock();
	if (!skeletonInstance || !pScatchpad)
	{
		return;
	}

	const GMeshModifierSkin& modifierSkinLoad = in_meshLoad.GetArrayModifier()[mModifierIndex].GetSkin();
	int countTargetStream = modifierSkinLoad.GetArrayTargetStreamCount();
	for (int indexTargetStream = 0; indexTargetStream < countTargetStream; ++indexTargetStream)
	{
		const GMeshModifierSkinTargetStream& targetStreamLoad = modifierSkinLoad.GetArrayTargetStreamLoad(indexTargetStream);
		const GMeshStreamInfo& streamInfoLoad = in_meshLoad.GetArrayStreamInfo()[targetStreamLoad.GetStreamIndex()];
		const int flag = 1 << targetStreamLoad.GetStreamIndex();

		LocalPopulateScratchPad(
			pScatchpad,
			(0 != (inout_flagWrittenStreams & flag)), 
			targetStreamLoad.GetIsPoint(),
			in_meshLoad, 
			inout_vertexData, 
			modifierSkinLoad,
			streamInfoLoad
			);

		LocalApplySkinning(
			inout_vertexData,
			pScatchpad,
			in_meshLoad,
			targetStreamLoad.GetIsPoint(),
			modifierSkinLoad,
			skeletonInstance,
			streamInfoLoad,
			mArraySkeletonBoneIndex
			);

		LocalApplyDuplicates(
			inout_vertexData, 
			pScatchpad,
			in_meshLoad, 
			targetStreamLoad.GetIsPoint(),
			modifierSkinLoad,
			streamInfoLoad
			);

		inout_flagWrittenStreams |= flag;
	}

	return;
}

//public accessors
void GMeshInstanceModifierSkin::SetSkeletonInstance(
	TPointerSkeletonInstance& inout_skeletonInstance,
	const GMesh& in_meshLoad
	)
{
	mSkeletonInstance = inout_skeletonInstance;

	for (TArrayInt::iterator iterator = mArraySkeletonBoneIndex.begin(); iterator != mArraySkeletonBoneIndex.end(); ++iterator)
	{
		(*iterator) = GCOMMON_INVALID_INDEX;
	}

	if (inout_skeletonInstance)
	{
		const GMeshModifierSkin& skinLoad = in_meshLoad.GetArrayModifier()[mModifierIndex].GetSkin();
		for (int index = 0; index < (int)skinLoad.GetArrayBoneCount(); ++index)
		{
			mArraySkeletonBoneIndex[index] = inout_skeletonInstance->GetBoneIndex(skinLoad.GetBone(index).GetBoneName());
		}
	}

	return;
}
