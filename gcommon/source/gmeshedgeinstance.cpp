//
//  GMeshEdgeInstance.cpp
//
//  Created by David Coen on 2011 04 02
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GMeshEdgeInstance.h"

#include "GMesh.h"
#include "GMeshManual.h"
#include "GMeshStreamInfo.h"
#include "GMeshEdge.h"
#include "GMeshEdgeNormalStatic.h"
#include "GMeshEdgeNormalDynamic.h"
#include "GMeshEdgeOnePoly.h"
#include "GMeshEdgeTwoPoly.h"
#include "GMeshVertexData.h"
#include "GVector3Float.h"
#include "GAssert.h"
#include "GCommon.h"

typedef std::vector<GMeshStreamInfo> TArrayStreamInfoLoad;
typedef std::vector<bool> TArrayBool;

static const int LocalGetStreamPosByteOffset(const GMesh& in_meshLoad)
{
	const int streamCount = in_meshLoad.GetStreamInfoCount();
	for (int index = 0; index < streamCount; ++index)
	{
		const GMeshStreamInfo& streamInfo = in_meshLoad.GetArrayStreamInfo()[index];
		if (GMeshType::TStreamUsage::TPosition == streamInfo.GetUsage())
		{
			return streamInfo.GetVertexDataByteOffset();
		}
	}
	return GCOMMON_INVALID_INDEX;
}

static const int LocalGetStreamPosCount(const GMesh& in_meshLoad)
{
	const int streamCount = in_meshLoad.GetStreamInfoCount();
	for (int index = 0; index < streamCount; ++index)
	{
		const GMeshStreamInfo& streamInfo = in_meshLoad.GetArrayStreamInfo()[index];
		if (GMeshType::TStreamUsage::TPosition == streamInfo.GetUsage())
		{
			return streamInfo.GetCount();
		}
	}
	return 0;
}

static void LocalUpdateStaticNormalFacing(
	TArrayBool& out_arrayNormalFacing,
	const GVector3Float& in_objectSpaceDirection,
	const int in_arrayNormalStaticLoadCount,
	const GMeshEdgeNormalStatic* const in_arrayNormalStaticLoad
	)
{
	const int baseIndex = out_arrayNormalFacing.size();
	out_arrayNormalFacing.resize(baseIndex + in_arrayNormalStaticLoadCount);
	for (int index = 0; index < in_arrayNormalStaticLoadCount; ++index)
	{
		const float result = DotProduct(
			in_objectSpaceDirection,
			in_arrayNormalStaticLoad[index].GetNormal()
			);
		out_arrayNormalFacing[baseIndex + index] = (0.0F <= result);
	}

	return;
}

static const GVector3Float LocalGetVertexPos(
	const GMesh& in_meshLoad,
	const void* const in_vertexData,
	const int in_streamPosByteOffset,
	const int in_streamPosCount,
	const int in_vertexIndex
	)
{
	const float* const pData = &GMeshVertexData::GetItemRef<float>(
		(void* const)in_vertexData,
		in_vertexIndex,
		in_meshLoad.GetVertexByteStride(),
		in_streamPosByteOffset,
		0
		);
	GVector3Float result;
	for (int index = 0; index < in_streamPosCount; ++index)
	{
		result.m_data[index] = pData[index]; 
	}
	return result;
}

static void LocalUpdateDynamicNormalFacing(
	TArrayBool& out_arrayNormalFacing,
	const GVector3Float& in_objectSpaceDirection,
	const int in_arrayNormalDynamicLoadCount,
	const GMeshEdgeNormalDynamic* const in_arrayNormalDynamicLoad,
	const GMesh& in_meshLoad,
	const void* const in_vertexData,
	const int in_streamPosByteOffset,
	const int in_streamPosCount
	)
{
	const int baseIndex = out_arrayNormalFacing.size();
	out_arrayNormalFacing.resize(baseIndex + in_arrayNormalDynamicLoadCount);
	for (int index = 0; index < in_arrayNormalDynamicLoadCount; ++index)
	{
		const GMeshEdgeNormalDynamic& normalDynamicLoad = in_arrayNormalDynamicLoad[index];
		const GVector3Float vertexOne = LocalGetVertexPos(in_meshLoad, in_vertexData, in_streamPosByteOffset, in_streamPosCount, normalDynamicLoad.GetVertexIndex0());
		const GVector3Float vertexTwo = LocalGetVertexPos(in_meshLoad, in_vertexData, in_streamPosByteOffset, in_streamPosCount, normalDynamicLoad.GetVertexIndex1());
		const GVector3Float vertexThree = LocalGetVertexPos(in_meshLoad, in_vertexData, in_streamPosByteOffset, in_streamPosCount, normalDynamicLoad.GetVertexIndex2());
		const GVector3Float faceDirection = CrossProduct(vertexTwo - vertexOne, vertexThree - vertexOne);
		const float result = DotProduct(
			in_objectSpaceDirection,
			faceDirection
			);
		out_arrayNormalFacing[baseIndex + index] = (0.0F <= result);
	}

	return;
}

static void LocalAddShadowQuad(
	int& inout_traceVertex,
	GMeshManual& inout_shadowMesh,
	const GVector3Float& in_vertexOne,
	const GVector3Float& in_vertexTwo,
	const GVector3Float& in_vertexOneOffset,
	const GVector3Float& in_vertexTwoOffset
	)
{
	float* const pData = &inout_shadowMesh.GetFloatRef(inout_traceVertex, 0, 0);
	inout_traceVertex += 6;
	*(GVector3Float*)(pData) = in_vertexOne;
	*(GVector3Float*)(pData + 3) = in_vertexTwo;
	*(GVector3Float*)(pData + 6) = in_vertexOneOffset;
	*(GVector3Float*)(pData + 9) = in_vertexTwo;
	*(GVector3Float*)(pData + 12) = in_vertexTwoOffset;
	*(GVector3Float*)(pData + 15) = in_vertexOneOffset;
	return;
}

//constructor
GMeshEdgeInstance::GMeshEdgeInstance(const GMesh& in_meshLoad)
: mStreamPosByteOffset(LocalGetStreamPosByteOffset(in_meshLoad))
, mStreamPosCount(LocalGetStreamPosCount(in_meshLoad))
, mShadowMesh()
, mDirection()
, mDistance(0.0F)
{
	GASSERT(NULL != in_meshLoad.GetEdge(), "invalid code path");
	const GMeshEdge& meshEdgeLoad = *in_meshLoad.GetEdge();

	TArrayStreamInfoLoad arrayStreamLoad;
	GMeshStreamInfo::AppendStreamInfo(
		arrayStreamLoad,
		GMeshType::TStreamType::TFloat,
		3, 
		GMeshType::TStreamUsage::TPosition
		);
	const int maxVertexCount = (meshEdgeLoad.GetArrayOnePolyLoadCount() + meshEdgeLoad.GetArrayTwoPolyLoadCount()) * 6;
	mShadowMesh.reset(new GMeshManual(
		GMeshType::TPrimitiveType::TTriangle,
		maxVertexCount,
		arrayStreamLoad
		));
	return;
}

GMeshEdgeInstance::~GMeshEdgeInstance()
{
	return;
}

//public methods
void GMeshEdgeInstance::Update(
	const GMesh& in_meshLoad,
	const void* const in_vertexData,
	const GVector3Float& in_objectSpaceDirection,
	const float in_distance
	)
{
	const GMeshEdge& edgeLoad = *in_meshLoad.GetEdge();

	//bail if only static and param are the same (shadow mesh already generated)
	if (edgeLoad.GetArrayNormalStaticLoadCount() && 
		(0 == edgeLoad.GetArrayNormalDynamicLoadCount()) &&
		(in_objectSpaceDirection == mDirection) &&
		(in_distance == mDistance))
	{
		return;
	}

	mDirection = in_objectSpaceDirection;
	mDistance = in_distance;

	//update normals
	TArrayBool arrayNormalFacing;
	if (edgeLoad.GetArrayNormalStaticLoadCount())
	{
		LocalUpdateStaticNormalFacing(
			arrayNormalFacing,
			in_objectSpaceDirection,
			edgeLoad.GetArrayNormalStaticLoadCount(),
			edgeLoad.GetArrayNormalStaticLoad()
			);
	}
	
	if (edgeLoad.GetArrayNormalDynamicLoadCount())
	{
		LocalUpdateDynamicNormalFacing(
			arrayNormalFacing,
			in_objectSpaceDirection,
			edgeLoad.GetArrayNormalDynamicLoadCount(),
			edgeLoad.GetArrayNormalDynamicLoad(),
			in_meshLoad,
			in_vertexData,
			mStreamPosByteOffset,
			mStreamPosCount
			);
	}

	//generate the mesh
	const GVector3Float offset = in_objectSpaceDirection * in_distance;
	GMeshManual& shadowMesh = *mShadowMesh;
	int traceVertex = 0;
	for (int index = 0; index < edgeLoad.GetArrayOnePolyLoadCount(); ++index)
	{
		const GMeshEdgeOnePoly& edgeOnePoly = edgeLoad.GetArrayOnePolyLoad()[index];
		const int normalIndex = edgeOnePoly.GetNormalIndex();

		GASSERT((0 <= normalIndex) && (normalIndex < (int)arrayNormalFacing.size()), "sanity check");
		if (!arrayNormalFacing[normalIndex])
		{
			continue;
		}
		const GVector3Float vertexOne = LocalGetVertexPos(
			in_meshLoad,
			in_vertexData,
			mStreamPosByteOffset,
			mStreamPosCount,
			edgeOnePoly.GetVertexIndexOne()
			);
		const GVector3Float vertexTwo = LocalGetVertexPos(
			in_meshLoad,
			in_vertexData,
			mStreamPosByteOffset,
			mStreamPosCount,
			edgeOnePoly.GetVertexIndexTwo()
			);

		LocalAddShadowQuad(
			traceVertex,
			shadowMesh,
			vertexOne,
			vertexTwo,
			vertexOne + offset,
			vertexTwo + offset
			);
	}
	for (int index = 0; index < edgeLoad.GetArrayTwoPolyLoadCount(); ++index)
	{
		const GMeshEdgeTwoPoly& edgeTwoPoly = edgeLoad.GetArrayTwoPolyLoad()[index];

		const int normalIndexOne = edgeTwoPoly.GetNormalIndexOne();
		const int normalIndexTwo = edgeTwoPoly.GetNormalIndexTwo();

		GASSERT((0 <= normalIndexOne) && (normalIndexOne < (int)arrayNormalFacing.size()), "sanity check");
		GASSERT((0 <= normalIndexTwo) && (normalIndexTwo < (int)arrayNormalFacing.size()), "sanity check");

		const bool normalOneFacing = arrayNormalFacing[normalIndexOne];
		const bool normalTwoFacing = arrayNormalFacing[normalIndexTwo];

		if (normalOneFacing == normalTwoFacing)
		{
			continue;
		}
		const GVector3Float vertexOne = LocalGetVertexPos(
			in_meshLoad,
			in_vertexData,
			mStreamPosByteOffset,
			mStreamPosCount,
			edgeTwoPoly.GetVertexIndexOne()
			);
		const GVector3Float vertexTwo = LocalGetVertexPos(
			in_meshLoad,
			in_vertexData,
			mStreamPosByteOffset,
			mStreamPosCount,
			edgeTwoPoly.GetVertexIndexTwo()
			);

		if (normalOneFacing)
		{
			LocalAddShadowQuad(
				traceVertex,
				shadowMesh,
				vertexOne,
				vertexTwo,
				vertexOne + offset,
				vertexTwo + offset
				);
		}
		else
		{
			LocalAddShadowQuad(
				traceVertex,
				shadowMesh,
				vertexTwo,
				vertexOne,
				vertexTwo + offset,
				vertexOne + offset
				);
		}
	}

	shadowMesh.SetElementCount(traceVertex);

	return;
}

//public accessors
const GMesh& GMeshEdgeInstance::GetShadowMesh()const
{
	return mShadowMesh->GetMesh();
}

const void* const GMeshEdgeInstance::GetShadowVertexData()const
{
	return mShadowMesh->GetMesh().GetVertexData();
}

