//
//  GMeshInstanceModifierCloth.cpp
//
//  Created by David Coen on 2011 01 03
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GMeshInstanceModifierCloth.h"

#include "GMesh.h"
#include "GMeshModifier.h"
#include "GMeshModifierCloth.h"
#include "GMeshVertexData.h"
#include "GMeshModifierClothVertexData.h"
#include "GMeshStreamInfo.h"
#include "GMatrix16Float.h"
#include "GMeshModifierClothSpringData.h"
#include "GMath.h"
#include "GSceneNode.h"

typedef std::vector<GMeshInstanceModifierClothVertexData> TArrayVertexData;
typedef std::vector<float> TArrayFloat;

static const GVector3Float LocalGetSourcePosition(
	const void* in_vertexDataSource, 
	const int in_vertexByteStride,
	const int in_streamByteOffset,
	const int in_streamChannelCount,
	const int in_vertexIndex
	)
{
	GVector3Float result;
	for (int subIndex = 0; subIndex < 3; ++subIndex)
	{
		float value = 0.0F;

		if (subIndex < in_streamChannelCount) //for 2d pos case
		{
			value = GMeshVertexData::GetItem<float>(
				in_vertexDataSource, 
				in_vertexIndex,
				in_vertexByteStride,
				in_streamByteOffset,
				subIndex
				);
		}
		result.m_data[subIndex] = value;
	}

	return result;
}

static void LocalWriteVertexData(	
	void* const inout_vertexDataDestination,
	const GVector3Float& in_position, 
	const int in_vertexIndex, 
	const int in_vertexByteStride,
	const int in_streamByteOffset,
	const int in_streamChannelCount
	)
{
	for (int subIndex = 0; subIndex < in_streamChannelCount; ++subIndex)
	{
		GMeshVertexData::GetItemRef<float>(
			inout_vertexDataDestination, 
			in_vertexIndex,
			in_vertexByteStride,
			in_streamByteOffset,
			subIndex
			) = in_position.m_data[subIndex];
	}
	return;
}

static void LocalCollectTargetPos(
	TArrayFloat& inout_scratchPad,
	GMeshVertexData& inout_vertexData,
	const GMesh& in_meshLoad,
	const int in_streamWritten,
	const int in_streamTargetIndex,
	const GMeshModifierCloth& in_modifierClothLoad
	)
{
	const void* vertexDataSource = in_meshLoad.GetVertexData();
	if (in_streamWritten)
	{
		vertexDataSource = inout_vertexData.GetData();
	}

	const GMeshStreamInfo& streamInfoLoad = in_meshLoad.GetArrayStreamInfo()[in_streamTargetIndex];
	const int vertexCount = in_modifierClothLoad.GetVertexDataArrayCount();
	float* const pDest = &inout_scratchPad[0];
	for (int index = 0; index < vertexCount; ++index)
	{
		const int vertexIndex = in_modifierClothLoad.GetVertexDataLoadArray()[index].GetVertexIndex(in_modifierClothLoad.GetVectorArrayDuplicate());
		const GVector3Float position = LocalGetSourcePosition(vertexDataSource, in_meshLoad.GetVertexByteStride(), streamInfoLoad.GetVertexDataByteOffset(), streamInfoLoad.GetCount(), vertexIndex);
		pDest[(index * 3) + 0] = position.m_x;
		pDest[(index * 3) + 1] = position.m_y;
		pDest[(index * 3) + 2] = position.m_z;
	}

	return;
}

static void LocalSetClothVertexDataFromScratchPad(
	const TArrayFloat& in_scratchPad,
	TArrayVertexData& inout_arrayClothVertexData,
	GSceneNode& in_sceneNode
	)
{
	const GMatrix16Float& worldTransform = in_sceneNode.GetWorldTransform();
	const int vertexCount = inout_arrayClothVertexData.size();
	const float* const pSource = &in_scratchPad[0];
	for (int index = 0; index < vertexCount; ++index)
	{
		GMeshInstanceModifierClothVertexData& clothVertexData = inout_arrayClothVertexData[index];
		const GVector3Float position = GVector3Float(pSource[(index * 3) + 0], pSource[(index * 3) + 1], pSource[(index * 3) + 2]) * worldTransform;
		clothVertexData.SetPosition(position);
		clothVertexData.SetVelocity(GVector3Float::sZero);
	}
	return;
}

static void LocalSetVertexDataFromScratchPad(
	const TArrayFloat& in_scratchPad,
	GMeshVertexData& inout_vertexData,
	const GMesh& in_meshLoad,
	const int in_streamTargetIndex,
	const GMeshModifierCloth& in_modifierClothLoad
	)
{
	const GMeshStreamInfo& streamInfoLoad = in_meshLoad.GetArrayStreamInfo()[in_streamTargetIndex];
	const int vertexCount = in_modifierClothLoad.GetVertexDataArrayCount();
	const float* const pSource = &in_scratchPad[0];
	for (int index = 0; index < vertexCount; ++index)
	{
		const GMeshModifierClothVertexData& vertexDataLoad = in_modifierClothLoad.GetVertexDataLoadArray()[index];
		const int vertexIndex = vertexDataLoad.GetVertexIndex(in_modifierClothLoad.GetVectorArrayDuplicate());
		const GVector3Float position(pSource[(index * 3) + 0], pSource[(index * 3) + 1], pSource[(index * 3) + 2]);

		LocalWriteVertexData(
			inout_vertexData.GetData(),
			position,
			vertexIndex,
			in_meshLoad.GetVertexByteStride(),
			streamInfoLoad.GetVertexDataByteOffset(),
			streamInfoLoad.GetCount()
			);

		const int duplicateOffset = vertexDataLoad.GetDuplicateTableOffset();
		if (-1 == duplicateOffset)
		{
			continue;
		}

		const unsigned int* const arrayDuplicate = in_modifierClothLoad.GetVectorArrayDuplicate();
		const int duplicateCount = arrayDuplicate[duplicateOffset];

		//apply duplicates
		for (int subIndex = 0; subIndex < duplicateCount; ++subIndex)
		{
			const unsigned int destinationIndex = arrayDuplicate[duplicateOffset + subIndex + 1];
			LocalWriteVertexData(
				inout_vertexData.GetData(), 
				position, 
				destinationIndex, 
				in_meshLoad.GetVertexByteStride(), 
				streamInfoLoad.GetVertexDataByteOffset(), 
				streamInfoLoad.GetCount()
				);
		}

	}

	return;
}

static void LocalApplyForcesToClothVertexData(
	TArrayVertexData& inout_arrayClothVertexData,
	const TArrayFloat& in_scratchPad, //object space target
	const GMeshModifierCloth& in_modifierClothLoad,
	const float in_timeDelta,
	GSceneNode& in_sceneNode
	)
{
	//apply spring force (cloth springs)
	const float springConstant = in_modifierClothLoad.GetSpringConstant();
	const float springDampen = in_modifierClothLoad.GetSpringDampen();
	const float massInvert = 1.0F / in_modifierClothLoad.GetMass();
	const int springCount = in_modifierClothLoad.GetSpringDataArrayCount();
	for (int index = 0; index < springCount; ++index)
	{
		const GMeshModifierClothSpringData& springDataLoad = in_modifierClothLoad.GetSpringDataLoadArray()[index];
		GMeshInstanceModifierClothVertexData& springVertexOne = inout_arrayClothVertexData[springDataLoad.GetSpringVertexIndexOne()];
		GMeshInstanceModifierClothVertexData& springVertexTwo = inout_arrayClothVertexData[springDataLoad.GetSpringVertexIndexTwo()];

		const float targetLength = springDataLoad.GetLength();

		//scale space by length (so target length is 1.0) to help SqrtAprox accuracy
		const GVector3Float offset = (springVertexTwo.GetPosition() - springVertexOne.GetPosition()) * (1.0F / targetLength);
		const float lengthRatio = GMath::SquareRootAprox(LengthSquared(offset));
		if (!GMath::AlmostZero(lengthRatio))
		{
			const GVector3Float offsetNormal = offset * (1.0F / lengthRatio);
			const float force = -(((lengthRatio * targetLength) - targetLength) * springConstant * 10.0F);
			const GVector3Float springAcceleration = offsetNormal * (force * massInvert);

			springVertexOne.MinusAcceleration(springAcceleration);
			springVertexTwo.AddAcceleration(springAcceleration);
		}

		//inner friction
		const GVector3Float frictionAcceleration = (springVertexTwo.GetVelocity() - springVertexOne.GetVelocity()) * (springDampen * massInvert * 2.0F);
		springVertexOne.AddAcceleration(frictionAcceleration);
		springVertexTwo.MinusAcceleration(frictionAcceleration);
	}

	//apply vertex force
	const GMatrix16Float& worldTransform = in_sceneNode.GetWorldTransform();
	const int vertexCount = inout_arrayClothVertexData.size();
	const float gravity = in_modifierClothLoad.GetGravity() * 5.0F;
	const float* const pSource = &in_scratchPad[0];
	for (int index = 0; index < vertexCount; ++index)
	{
		GMeshInstanceModifierClothVertexData& clothVertexData = inout_arrayClothVertexData[index];

		//spring to target
		const GVector3Float targetPosition = GVector3Float(pSource[(index * 3) + 0], pSource[(index * 3) + 1], pSource[(index * 3) + 2]) * worldTransform;
		const GVector3Float offset = (targetPosition - clothVertexData.GetPosition());
		const float length = GMath::SquareRootAprox(LengthSquared(offset));
		if (!GMath::AlmostZero(length))
		{
			const GVector3Float offsetNormal = offset * (1.0F / length);
			const float force = (length * springConstant * 0.01F);
			const GVector3Float springAcceleration = offsetNormal * (force * massInvert);

			clothVertexData.AddAcceleration(springAcceleration);
		}
		//inner friction (spring to target)
		const GVector3Float frictionAcceleration = (clothVertexData.GetVelocity()) * (springDampen * massInvert);
		clothVertexData.MinusAcceleration(frictionAcceleration);

		//gravity
		if (gravity)
		{
			clothVertexData.AddAcceleration(GVector3Float(0.0F, gravity, 0.0F));
		}

		clothVertexData.IntergrateAndClearAcceleration(in_timeDelta);
	}

	return;
}

static void LocalSetVertexDataFromClothVertexData(
	TArrayVertexData& inout_arrayClothVertexData,
	const TArrayFloat& in_scratchPad, //object space target
	GSceneNode& in_sceneNode,
	const float in_timeDelta, //limit the weight lerp by tick?
	GMeshVertexData& inout_vertexData,
	const GMesh& in_meshLoad,
	const int in_streamTargetIndex,
	const GMeshModifierCloth& in_modifierClothLoad	
	)
{
	const GMeshStreamInfo& streamInfoLoad = in_meshLoad.GetArrayStreamInfo()[in_streamTargetIndex];
	const int vertexCount = in_modifierClothLoad.GetVertexDataArrayCount();
	const float* const pSource = &in_scratchPad[0];
	const GMatrix16Float& worldTransformInvert = in_sceneNode.GetWorldTransformInvert();
	GVector3Float position;
	for (int index = 0; index < vertexCount; ++index)
	{
		GMeshInstanceModifierClothVertexData& clothVertexData = inout_arrayClothVertexData[index];

		const GMeshModifierClothVertexData& vertexDataLoad = in_modifierClothLoad.GetVertexDataLoadArray()[index];
		const int vertexIndex = vertexDataLoad.GetVertexIndex(in_modifierClothLoad.GetVectorArrayDuplicate());

		const float weight = vertexDataLoad.GetWeight();
		if (0.0F == weight)
		{
			position = GVector3Float(pSource[(index * 3) + 0], pSource[(index * 3) + 1], pSource[(index * 3) + 2]);
			clothVertexData.SetPosition(position * in_sceneNode.GetWorldTransform());
			clothVertexData.SetVelocity(GVector3Float::sZero);
		}
		else if (1.0F == weight)
		{
			position = inout_arrayClothVertexData[index].GetPosition() * worldTransformInvert;
		}
		else
		{
			//lerp position to target, limit by time
			//const float ratio = GMath::Clamp(1.0F - ((1.0F - (weight * weight)) * in_timeDelta), 0.0F, 1.0F);
			//const float ratio = GMath::Clamp(1.0F - ((1.0F - weight) * in_timeDelta), 0.0F, 1.0F);
			const float ratio = GMath::Clamp(weight, 0.0F, 1.0F);

			position = Lerp(
				ratio, 
				GVector3Float(pSource[(index * 3) + 0], pSource[(index * 3) + 1], pSource[(index * 3) + 2]),
				inout_arrayClothVertexData[index].GetPosition() * worldTransformInvert
				);

			clothVertexData.SetPosition(position * in_sceneNode.GetWorldTransform());
			clothVertexData.SetVelocity(clothVertexData.GetVelocity() * ratio);
		}

		LocalWriteVertexData(
			inout_vertexData.GetData(),
			position,
			vertexIndex,
			in_meshLoad.GetVertexByteStride(),
			streamInfoLoad.GetVertexDataByteOffset(),
			streamInfoLoad.GetCount()
			);

		const int duplicateOffset = vertexDataLoad.GetDuplicateTableOffset();
		if (-1 == duplicateOffset)
		{
			continue;
		}

		const unsigned int* const arrayDuplicate = in_modifierClothLoad.GetVectorArrayDuplicate();
		const int duplicateCount = arrayDuplicate[duplicateOffset];

		//apply duplicates (the first item on the duplicate table is the source vertex
		for (int subIndex = 1; subIndex < duplicateCount; ++subIndex)
		{
			const unsigned int destinationIndex = arrayDuplicate[duplicateOffset + subIndex + 1];
			LocalWriteVertexData(
				inout_vertexData.GetData(), 
				position, 
				destinationIndex, 
				in_meshLoad.GetVertexByteStride(), 
				streamInfoLoad.GetVertexDataByteOffset(), 
				streamInfoLoad.GetCount()
				);
		}

	}

	return;
}

static void LocalDealStream(
	TArrayVertexData& inout_arrayClothVertexData,
	GMeshVertexData& inout_vertexData,
	const GMesh& in_meshLoad,
	const int in_streamWritten,
	const int in_streamTargetIndex,
	const GMeshModifierCloth& in_modifierClothLoad,
	GSceneNode& in_sceneNode,
	const float in_timeDelta
	)
{
	//TODO: make singelton scratchpad class, needed for skin as well
	TArrayFloat scratchPad; //object space target position
	scratchPad.resize(inout_arrayClothVertexData.size() * 3);

	LocalCollectTargetPos(
		scratchPad,
		inout_vertexData,
		in_meshLoad,
		in_streamWritten,
		in_streamTargetIndex,
		in_modifierClothLoad
		);

	if (in_sceneNode.GetTeleportFlag())
	{
		LocalSetClothVertexDataFromScratchPad(
			scratchPad,
			inout_arrayClothVertexData,
			in_sceneNode
			);

		//we only need to set the target vertex data if it was not already been written to
		if (!in_streamWritten)
		{
			LocalSetVertexDataFromScratchPad(
				scratchPad,
				inout_vertexData,
				in_meshLoad,
				in_streamTargetIndex,
				in_modifierClothLoad
				);
		}
	}
	else
	{
		LocalApplyForcesToClothVertexData(
			inout_arrayClothVertexData,
			scratchPad, //object space target
			in_modifierClothLoad,
			in_timeDelta,
			in_sceneNode
			);
		LocalSetVertexDataFromClothVertexData(
			inout_arrayClothVertexData,
			scratchPad, //object space target
			in_sceneNode,
			in_timeDelta, //limit the weight lerp by tick?
			inout_vertexData,
			in_meshLoad,
			in_streamTargetIndex,
			in_modifierClothLoad
			);
	}

	return;
}

//static public methods
/*static*/ GMeshInstanceModifierCloth::TPointerRenderMeshInstanceModifierCloth GMeshInstanceModifierCloth::Factory(
	const GMesh& in_meshLoad,
	const int in_modifierIndex
	)
{
	TPointerRenderMeshInstanceModifierCloth pointer;
	const GMeshModifierCloth& modifierClothLoad = in_meshLoad.GetArrayModifier()[in_modifierIndex].GetCloth();

	pointer.reset(new GMeshInstanceModifierCloth(in_modifierIndex));

	//now set up the vertex data 
	if (1 <= modifierClothLoad.GetStreamTargetArrayCount())
	{
		const int vertexDataCount = modifierClothLoad.GetVertexDataArrayCount();
		pointer->mArrayVertexData.resize(vertexDataCount);
	}

	return pointer;
}

//constructor
GMeshInstanceModifierCloth::GMeshInstanceModifierCloth(
	const int in_modifierIndex
	)
	: GMeshInstanceModifier()
	, mModifierIndex(in_modifierIndex)
	, mArrayVertexData()
{
	return;
}

GMeshInstanceModifierCloth::~GMeshInstanceModifierCloth()
{
	return;
}

//implement GMeshInstanceModifier
/*virtual*/ void GMeshInstanceModifierCloth::OnTickApplyModifier(
	GMeshVertexData& inout_vertexData,
	int& inout_flagWrittenStreams,
	const float in_timeDelta,
	const GMesh& in_meshLoad,
	GSceneNode& in_sceneNode
	)
{
	const GMeshModifierCloth& modifierClothLoad = in_meshLoad.GetArrayModifier()[mModifierIndex].GetCloth();
	int countTargetStream = modifierClothLoad.GetStreamTargetArrayCount();
	for (int index = 0; index < countTargetStream; ++index)
	{
		const int streamTargetIndex = modifierClothLoad.GetStreamTargetArray()[index];
		const bool streamWritten = (0 != (inout_flagWrittenStreams & (1 << streamTargetIndex)));

		//TODO: deal with more than one steam, without assuming it is position
		if (0 == index)
		{
			LocalDealStream(
				mArrayVertexData,
				inout_vertexData,
				in_meshLoad,
				streamWritten,
				streamTargetIndex,
				modifierClothLoad,
				in_sceneNode,
				in_timeDelta
				);
		}

		inout_flagWrittenStreams |= (1 << streamTargetIndex);
	}

	return;
}
