//
//  gmeshmanual.cpp
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "gmeshmanual.h"

#include "gassert.h"
#include "gmeshvertexdata.h"
#include "gmesh.h"
#include "gmeshstreaminfo.h"

//constructor
GMeshManual::GMeshManual(
	const GMeshType::TPrimitiveType::TEnum in_topologyType,
	const GS32 in_vertexCount,
	const TArrayStreamInfo& in_arrayStreamInfo
	)
	: mMesh(0)
	, mVertexData()
	, mArrayIndexByte()
	, mArrayIndexShort()
	, mArrayStreamInfo(in_arrayStreamInfo)
{
	const GS32 vertexDataStride = GMeshStreamInfo::CalculateVertexDataByteSize(mArrayStreamInfo);

	mVertexData.reset(new GMeshVertexData(in_vertexCount, vertexDataStride));

	mMesh.reset(new GMesh(
		in_topologyType,
		in_vertexCount, 
		0,
		0,
		in_vertexCount,
		vertexDataStride,
		mVertexData->GetData(),
		mArrayStreamInfo.size(),
		&mArrayStreamInfo[0]
		));

	return;
}

GMeshManual::GMeshManual(
	const GMeshType::TPrimitiveType::TEnum in_topologyType,
	const TArrayByte& in_arrayVertexIndex, //vertex count = in_arrayVertexIndex.size
	const GS32 in_vertexDataCount, //assert in_vertexDataCount < 256
	const TArrayStreamInfo& in_arrayStreamInfo
	)
	: mMesh(0)
	, mVertexData()
	, mArrayIndexByte(in_arrayVertexIndex)
	, mArrayIndexShort()
	, mArrayStreamInfo(in_arrayStreamInfo)
{
	const GS32 vertexDataStride = GMeshStreamInfo::CalculateVertexDataByteSize(mArrayStreamInfo);

	mVertexData.reset(new GMeshVertexData(in_vertexDataCount, vertexDataStride));

	mMesh.reset(new GMesh(
		in_topologyType,
		mArrayIndexByte.size(), 
		GMeshType::TFlag::TIndexU8,
		&mArrayIndexByte[0],
		in_vertexDataCount,
		vertexDataStride,
		mVertexData->GetData(),
		mArrayStreamInfo.size(),
		&mArrayStreamInfo[0]
		));

	return;
}

GMeshManual::GMeshManual(
	const GMeshType::TPrimitiveType::TEnum in_topologyType,
	const TArrayShort& in_arrayVertexIndex,
	const GS32 in_vertexDataCount, //assert in_vertexDataCount < 65536
	const TArrayStreamInfo& in_arrayStreamInfo
	)
	: mMesh(0)
	, mVertexData()
	, mArrayIndexByte()
	, mArrayIndexShort(in_arrayVertexIndex)
	, mArrayStreamInfo(in_arrayStreamInfo)
{
	const GS32 vertexDataStride = GMeshStreamInfo::CalculateVertexDataByteSize(mArrayStreamInfo);

	mVertexData.reset(new GMeshVertexData(in_vertexDataCount, vertexDataStride));

	mMesh.reset(new GMesh(
		in_topologyType,
		mArrayIndexShort.size(), 
		GMeshType::TFlag::TIndexU16,
		&mArrayIndexShort[0],
		in_vertexDataCount,
		vertexDataStride,
		mVertexData->GetData(),
		mArrayStreamInfo.size(),
		&mArrayStreamInfo[0]
		));

	return;
}

GMeshManual::~GMeshManual()
{
	return;
}

//public accessors
const GMesh& GMeshManual::GetMesh()const
{
	GASSERT(0 != mMesh, "invalid state");
	return *mMesh;
}

const GR32 GMeshManual::GetFloat(const GS32 in_vertexDataIndex, const GS32 in_streamIndex, const GS32 in_subIndex)const
{
	return (mVertexData->GetItem<GR32>(
		mVertexData->GetData(),
		in_vertexDataIndex,
		mMesh->GetVertexByteStride(),
		mMesh->GetArrayStreamInfo()[in_streamIndex].GetVertexDataByteOffset(),
		in_subIndex
		));
}

GR32& GMeshManual::GetFloatRef(const GS32 in_vertexDataIndex, const GS32 in_streamIndex, const GS32 in_subIndex)
{
	GASSERT(in_vertexDataIndex < mMesh->GetVertexDataCount(), "sanity check");
	return (mVertexData->GetItemRef<GR32>(
		mVertexData->GetData(),
		in_vertexDataIndex,
		mMesh->GetVertexByteStride(),
		mMesh->GetArrayStreamInfo()[in_streamIndex].GetVertexDataByteOffset(),
		in_subIndex
		));
}

const GU8 GMeshManual::GetByte(const GS32 in_vertexDataIndex, const GS32 in_streamIndex, const GS32 in_subIndex)const
{
	return (mVertexData->GetItem<GU8>(
		mVertexData->GetData(),
		in_vertexDataIndex,
		mMesh->GetVertexByteStride(),
		mMesh->GetArrayStreamInfo()[in_streamIndex].GetVertexDataByteOffset(),
		in_subIndex
		));
}

GU8& GMeshManual::GetByteRef(const GS32 in_vertexDataIndex, const GS32 in_streamIndex, const GS32 in_subIndex)
{
	return (mVertexData->GetItemRef<GU8>(
		mVertexData->GetData(),
		in_vertexDataIndex,
		mMesh->GetVertexByteStride(),
		mMesh->GetArrayStreamInfo()[in_streamIndex].GetVertexDataByteOffset(),
		in_subIndex
		));
}

//wanted to allow change of topology data (index) but don't want byte or short exposed
void GMeshManual::SetArrayIndex(const GS32 in_index, const GS32 in_value)
{
	if (mArrayIndexByte.size())
	{
		mArrayIndexByte[in_index] = (GU8)in_value;
	}
	else if (mArrayIndexShort.size())
	{
		mArrayIndexShort[in_index] = (GU16)in_value;
	}

	return;
}

const GS32 GMeshManual::GetElementCount()const
{
	if (mMesh)
	{
		mMesh->GetElementCount();
	}

	return 0;
}


void GMeshManual::SetElementCount(const GS32 in_elementCount)
{
	if (mMesh)
	{
		mMesh->SetElementCount(in_elementCount);
	}
}

const GS32 GMeshManual::GetMaxElementCount()const
{
	if (mArrayIndexByte.size())
	{
		return mArrayIndexByte.size();
	}
	else if (mArrayIndexShort.size())
	{
		return mArrayIndexShort.size();
	}

	return GetMaxVertexCount();
}

const GS32 GMeshManual::GetMaxVertexCount()const
{
	if (mMesh)
	{
		return mMesh->GetVertexDataCount();
	}
	return 0;
}
