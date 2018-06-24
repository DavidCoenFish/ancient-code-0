//
//  gmesh.cpp
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "gmesh.h"

#include "gassert.h"
#include "gmeshmodifier.h"
#include "gmeshstreaminfo.h"
#include "gmeshtype.h"
#include "GMeshEdge.h"
#include "GMeshModifier.h"

/////////////////////////////////////////////////////////////////////
//constructor
GMesh::GMesh(
	const int in_topologyType,
	const int in_elementCount, //vertex count can be greater than 255 and still use u8 index, as long as there are less than 255 vertexDataCount
	const int in_flag,
	const void* const in_vertexIndexArray,
	const int in_vertexDataCount,
	const int in_vertexDataByteStride,
	const void* const in_vertexData,
	const int in_streamInfoCount,
	const GMeshStreamInfo* const in_arrayStreamInfo,
	const GMeshEdge* const in_edge,
	const int in_modifierCount,
	const GMeshModifier* const in_arrayModifier,
	const char* const in_skeletonName 
	)
	: mTopologyType(in_topologyType)
	, mElementCount(in_elementCount)
	, mFlag(in_flag)
	, mVertexIndexArray(in_vertexIndexArray)
	, mVertexDataCount(in_vertexDataCount)
	, mVertexDataByteStride(in_vertexDataByteStride)
	, mVertexData(in_vertexData)
	, mStreamInfoCount(in_streamInfoCount)
	, mArrayStreamInfo(in_arrayStreamInfo)
	, mEdge(in_edge)
	, mModifierCount(in_modifierCount)
	, mArrayModifier(in_arrayModifier)
	, mSkeletonName(in_skeletonName)
{
	return;
}

GMesh::~GMesh()
{
	return;
}

//public accessors
const GBOOL GMesh::HasIndexU8()const
{
	return (0 != (mFlag & GMeshType::TFlag::TIndexU8));
}

const GBOOL GMesh::HasIndexU16()const
{
	return (0 != (mFlag & GMeshType::TFlag::TIndexU16));
}

const GU8* const GMesh::GetVertexIndexU8()const
{
	return (const unsigned char* const)mVertexIndexArray;
}

const GU16* const GMesh::GetVertexIndexU16()const
{
	return (const unsigned short* const)mVertexIndexArray;
}

const GS32 GMesh::GetVertexIndexType()const
{
	switch (mFlag & GMeshType::TFlag::TIndexMask)
	{
	default:
		break;
	case GMeshType::TFlag::TIndexU8:
		return GMeshType::TStreamType::TUnsignedByte;
	case GMeshType::TFlag::TIndexU16:
		return GMeshType::TStreamType::TUnsignedShort;
	}
	return 0;
}

const GS32 GMesh::GetVertexDataByteSize()const
{
	return (mVertexDataCount * mVertexDataByteStride);
}
