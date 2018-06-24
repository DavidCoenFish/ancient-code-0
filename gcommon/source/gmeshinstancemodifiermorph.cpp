//
//  GMeshInstanceModifierMorph.cpp
//
//  Created by David Coen on 2011 03 08
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GMeshInstanceModifierMorph.h"

#include "GMesh.h"
#include "GMeshModifier.h"
#include "GMeshModifierMorph.h"
#include "GMeshModifierMorphTarget.h"
#include "GMeshModifierMorphTargetStream.h"
#include "GMeshVertexData.h"
#include "GMath.h"
#include "GMeshStreamInfo.h"
#include "GCommon.h"
#include "GSkeletonInstance.h"
#include "GScratchPadLock.h"

typedef std::vector<int> TArrayInt;

static const bool LocalTestWriteToThisStream(
	const int in_streamIndex, 
	const TArrayInt& in_arraySkeletonMorphTargetIndex,
	const GSkeletonInstance& in_skeletonInstance, 
	const GMeshModifierMorph& in_morphLoad
	)
{
	const int count = in_arraySkeletonMorphTargetIndex.size();
	for (int index = 0; index < count; ++index)
	{
		const float weight = in_skeletonInstance.GetMorphTargetWeight(in_arraySkeletonMorphTargetIndex[index]);
		if (!weight)
		{
			continue;
		}

		if (in_morphLoad.GetArrayTarget()[index].GetArrayStream()[in_streamIndex].GetArrayDuplicateTableOffsetCount())
		{
			return true;
		}
	}

	return false;
}

static const int LocalGetStreamsToWrite(
	const int in_streamCount, 
	const TArrayInt& in_arraySkeletonMorphTargetIndex,
	const GSkeletonInstance& in_skeletonInstance, 
	const GMeshModifierMorph& in_morphLoad
	)
{
	int flag = 0;
	for (int index = 0; index < in_streamCount; ++index)
	{
		if (LocalTestWriteToThisStream(
			index,
			in_arraySkeletonMorphTargetIndex,
			in_skeletonInstance,
			in_morphLoad
			))
		{
			flag |= (1 << index);
		}
	}

	return flag;
}

static void LocalMorphValue(
	unsigned char* const in_pDestination,
	const unsigned char* const in_pSource,
	const int in_vertexDataOffset,
	const int in_vertexDataSize,
	const float in_weight,
	const unsigned char* const in_pMorphSource,
	const GMeshStreamInfo& in_streamInfoLoad
	)
{
	const int count = in_streamInfoLoad.GetCount();
	switch (in_streamInfoLoad.GetType())
	{
	default:
		break;
	case GMeshType::TStreamType::TByte:
	case GMeshType::TStreamType::TUnsignedByte:
		{
			for (int index = 0; index < count; ++index)
			{
				unsigned char& destinationValue = *(unsigned char*)(in_pDestination + in_vertexDataOffset + (sizeof(unsigned char) * index));
				const unsigned char originalValue = *(unsigned char*)(in_pSource + in_vertexDataOffset + (sizeof(unsigned char) * index));
				const unsigned char morphValue = *(unsigned char*)(in_pMorphSource + (sizeof(unsigned char) * index));
				destinationValue = (unsigned char)GMath::Clamp(GMath::Lerp(in_weight, destinationValue, morphValue - originalValue), 0, 255);
			}
		}
		break;
	case GMeshType::TStreamType::TFloat:
		{
			for (int index = 0; index < count; ++index)
			{
				float& destinationValue = *(float*)(in_pDestination + in_vertexDataOffset + (sizeof(float) * index));
				const float originalValue = *(float*)(in_pSource + in_vertexDataOffset + (sizeof(float) * index));
				const float morphValue = *(float*)(in_pMorphSource + (sizeof(float) * index));
				destinationValue += GMath::Lerp(in_weight, 0.0F, morphValue - originalValue);
			}
		}
		break;
	case GMeshType::TStreamType::TInt:
	case GMeshType::TStreamType::TUnsignedInt:
		{
			for (int index = 0; index < count; ++index)
			{
				int& destinationValue = *(int*)(in_pDestination + in_vertexDataOffset + (sizeof(int) * index));
				const int originalValue = *(int*)(in_pSource + in_vertexDataOffset + (sizeof(int) * index));
				const int morphValue = *(int*)(in_pMorphSource + (sizeof(int) * index));
				destinationValue += GMath::Lerp(in_weight, 0, morphValue - originalValue);
			}
		}
		break;
	}

	return;
}

static void LocalApplyMorphVertexStream(
	GMeshVertexData& inout_vertexData,
	const unsigned char* const in_pSource,
	const float in_weight,
	const int in_elementCount,
	const int* const in_arrayElement,
	const GMeshModifierMorphTargetStream& in_streamLoad,
	const int in_streamLoadIndex,
	const GMesh& in_meshLoad,
	const int in_streamIndex
	)
{
	const GMeshStreamInfo& streamInfoLoad = in_meshLoad.GetArrayStreamInfo()[in_streamIndex];
	const int vertexDataOffset = (in_meshLoad.GetVertexByteStride() * in_arrayElement[0]) + streamInfoLoad.GetVertexDataByteOffset();
	const int vertexDataSize = GMeshType::GetSize(streamInfoLoad.GetType()) * streamInfoLoad.GetCount();

	unsigned char* const pDestination = (unsigned char* const)inout_vertexData.GetData();
	unsigned char* const pMorphSource = ((unsigned char*)in_streamLoad.GetStreamData()) + (vertexDataSize * in_streamLoadIndex);

	LocalMorphValue(
		pDestination,
		in_pSource,
		vertexDataOffset,
		vertexDataSize,
		in_weight,
		pMorphSource,
		streamInfoLoad
		);
		
	//apply duplicates
	for (int index = 1; index < in_elementCount; ++index)
	{
		const int duplicateDataOffset = (in_meshLoad.GetVertexByteStride() * in_arrayElement[index]) + streamInfoLoad.GetVertexDataByteOffset();
		memcpy(pDestination + duplicateDataOffset, pDestination + vertexDataOffset, vertexDataSize);
	}

	return;
}

static void LocalApplyStream(
	GMeshVertexData& inout_vertexData,
	const int in_flagWrittenStreams,
	const int in_streamIndex,
	const GMesh& in_meshLoad,
	const GSkeletonInstance& in_skeletonInstance, 
	const TArrayInt& in_arraySkeletonMorphTargetIndex,
	const GMeshModifierMorph& in_morphLoad,
	unsigned char* const in_scratchPad = 0
	)
{
	const unsigned char* pSource = 0;
	const bool streamWritten = (0 != (in_flagWrittenStreams & (1 << in_streamIndex)));
	if (streamWritten)
	{
		pSource = in_scratchPad;
	}
	else
	{
		pSource = (unsigned char*)in_meshLoad.GetVertexData();
	}

	const int targetCount = (int)in_arraySkeletonMorphTargetIndex.size();
	for (int targetIndex = 0; targetIndex < targetCount; ++targetIndex)
	{
		const float weight = in_skeletonInstance.GetMorphTargetWeight(in_arraySkeletonMorphTargetIndex[targetIndex]);
		if (!weight)
		{
			continue;
		}

		const GMeshModifierMorphTarget& targetLoad = in_morphLoad.GetArrayTarget()[targetIndex];
		const GMeshModifierMorphTargetStream& streamLoad = targetLoad.GetArrayStream()[in_streamIndex];

		const int duplicateTableOffsetCount = streamLoad.GetArrayDuplicateTableOffsetCount();
		if (!duplicateTableOffsetCount)
		{
			continue;
		}

		const int* const arrayDuplicateTableOffset = streamLoad.GetArrayDuplicateTableOffset();
		const int* const duplicateTable = targetLoad.GetDuplicateTable();
		for (int duplicateTableOffsetIndex = 0; duplicateTableOffsetIndex < duplicateTableOffsetCount; ++duplicateTableOffsetIndex)
		{
			const int offset = arrayDuplicateTableOffset[duplicateTableOffsetIndex];
			const int elementCount = duplicateTable[offset];
			LocalApplyMorphVertexStream(
				inout_vertexData,
				pSource,
				weight,
				elementCount,
				&duplicateTable[offset + 1],
				streamLoad,
				duplicateTableOffsetIndex,
				in_meshLoad,
				in_streamIndex
				);
		}
	}

	return;
}

//static public methods
/*static*/ GMeshInstanceModifierMorph::TPointerRenderMeshInstanceModifierMorph GMeshInstanceModifierMorph::Factory(
	const GMesh& in_meshLoad,
	const int in_modifierIndex
	)
{
	TPointerRenderMeshInstanceModifierMorph pointer;
	const GMeshModifierMorph& modifierMorphLoad = in_meshLoad.GetArrayModifier()[in_modifierIndex].GetMorph();

	pointer.reset(new GMeshInstanceModifierMorph(
		modifierMorphLoad.GetArrayTargetCount(),
		in_modifierIndex
		));

	return pointer;
}

//constructor
GMeshInstanceModifierMorph::GMeshInstanceModifierMorph(
	const int in_arrayMorphTargetCount,
	const int in_modifierIndex
	)
	: GMeshInstanceModifier()
	, mModifierIndex(in_modifierIndex)
	, mSkeletonInstance()
	, mArraySkeletonMorphTargetIndex()
{
	mArraySkeletonMorphTargetIndex.resize(in_arrayMorphTargetCount);
	for (int index = 0; index < in_arrayMorphTargetCount; ++index)
	{
		mArraySkeletonMorphTargetIndex[index] = GCOMMON_INVALID_INDEX;
	}
	return;
}

GMeshInstanceModifierMorph::~GMeshInstanceModifierMorph()
{
	return;
}

//implement GMeshInstanceModifier
/*virtual*/ void GMeshInstanceModifierMorph::OnTickApplyModifier(
	GMeshVertexData& inout_vertexData,
	int& inout_flagWrittenStreams,
	const float in_timeDelta,
	const GMesh& in_meshLoad,
	GSceneNode& in_sceneNode
	)
{
	const GMeshModifierMorph& modifierMorphLoad = in_meshLoad.GetArrayModifier()[mModifierIndex].GetMorph();
	TPointerSkeletonInstance pointerSkeletonInstance = mSkeletonInstance.lock();
	if (!pointerSkeletonInstance)
	{
		return;
	}
	const GSkeletonInstance& skeletonInstance = *pointerSkeletonInstance;

	const int streamToWriteFlag = LocalGetStreamsToWrite(
		in_meshLoad.GetStreamInfoCount(), 	
		mArraySkeletonMorphTargetIndex,
		skeletonInstance, 
		modifierMorphLoad
		);
	if (!streamToWriteFlag)
	{
		//nothing to do here, move along
		return;
	}

	const GMeshModifierMorph& morphLoad = in_meshLoad.GetArrayModifier()[mModifierIndex].GetMorph();
	if (0 != (streamToWriteFlag & inout_flagWrittenStreams))
	{
		//we need to make a copy of inout_vertexData else source will be overwritten
		// rather than muck about with element_count * stream_count copies, just copy it all
		const int size = in_meshLoad.GetVertexDataByteSize();
		GScratchPadLock<unsigned char> scatchpad(size);
		unsigned char* const pScatchpad = scatchpad.GetMemory();
		memcpy(pScatchpad, inout_vertexData.GetData(), size);

		for (int index = 0; index < in_meshLoad.GetStreamInfoCount(); ++index)
		{
			if (0 == (streamToWriteFlag & (1 << index)))
			{
				continue;
			}
			LocalApplyStream(
				inout_vertexData,
				inout_flagWrittenStreams,
				index,
				in_meshLoad,
				skeletonInstance, 
				mArraySkeletonMorphTargetIndex,
				morphLoad,
				pScatchpad
				);
		}
	}
	else
	{
		for (int index = 0; index < in_meshLoad.GetStreamInfoCount(); ++index)
		{
			if (0 == (streamToWriteFlag & (1 << index)))
			{
				continue;
			}
			LocalApplyStream(
				inout_vertexData,
				inout_flagWrittenStreams,
				index,
				in_meshLoad,
				skeletonInstance, 
				mArraySkeletonMorphTargetIndex,
				morphLoad,
				0
				);
		}
	}
	inout_flagWrittenStreams |= streamToWriteFlag;

	return;
}

//public accessors
void GMeshInstanceModifierMorph::SetSkeletonInstance(
	TPointerSkeletonInstance& inout_skeletonInstance,
	const GMesh& in_meshLoad
	)
{
	mSkeletonInstance = inout_skeletonInstance;

	if (inout_skeletonInstance)
	{
		const GMeshModifierMorph& morphLoad = in_meshLoad.GetArrayModifier()[mModifierIndex].GetMorph();
		for (int index = 0; index < (int)morphLoad.GetArrayTargetCount(); ++index)
		{
			mArraySkeletonMorphTargetIndex[index] = inout_skeletonInstance->GetMorphTargetIndex(morphLoad.GetArrayTarget()[index].GetTargetName());
		}
	}

	return;
}