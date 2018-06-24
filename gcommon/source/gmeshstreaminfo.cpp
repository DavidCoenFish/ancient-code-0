//
//  gmeshstreaminfo.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "gmeshstreaminfo.h"

#include "gmeshtype.h"

//static public methods
/*static*/ GVOID GMeshStreamInfo::AppendStreamInfo(
	TArrayStreamInfo& inout_arrayStreamInfo,
	const GMeshType::TStreamType::TEnum in_type,
	const GS32 in_count,
	const GMeshType::TStreamUsage::TEnum in_usage
	)
{
	const int vertexDataByteOffset = CalculateVertexDataByteSize(inout_arrayStreamInfo);

	inout_arrayStreamInfo.push_back(GMeshStreamInfo(
		in_type,
		in_count,
		in_usage,
		vertexDataByteOffset
		));

	return;
}

/*static*/ const GS32 GMeshStreamInfo::CalculateVertexDataByteSize(
	const TArrayStreamInfo& in_arrayStreamInfo
	)
{
	GS32 returnSize = 0;
	for (TArrayStreamInfo::const_iterator iterator = in_arrayStreamInfo.begin(); iterator != in_arrayStreamInfo.end(); ++iterator)
	{
		returnSize += GMeshType::GetSize((GMeshType::TStreamType::TEnum)(*iterator).GetType()) * (*iterator).GetCount();
	}	

	return returnSize;
}

//constructor
GMeshStreamInfo::GMeshStreamInfo(
	const GMeshType::TStreamType::TEnum in_type,
	const GS32 in_count,
	const GMeshType::TStreamUsage::TEnum in_usage,
	const GS32 in_vertexDataByteOffset
	)
	: mType(in_type)
	, mCount(in_count)
	, mUsage(in_usage)
	, mVertexDataByteOffset(in_vertexDataByteOffset)
{
	return;
}

GMeshStreamInfo::GMeshStreamInfo(const GMeshStreamInfo& in_src)
	: mType(GMeshType::TStreamType::TFloat)
	, mCount(0)
	, mUsage(GMeshType::TStreamUsage::TCount)
	, mVertexDataByteOffset(0)
{
	(*this) = in_src;
	return;
}

GMeshStreamInfo::~GMeshStreamInfo()
{
	return;
}

//operator
const GMeshStreamInfo& GMeshStreamInfo::operator=(const GMeshStreamInfo& in_rhs)
{
	if (this != &in_rhs)
	{
		mType = in_rhs.mType;
		mCount = in_rhs.mCount;
		mUsage = in_rhs.mUsage;
		mVertexDataByteOffset = in_rhs.mVertexDataByteOffset;
	}

	return (*this);
}
