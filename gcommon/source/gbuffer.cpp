//
//  gbuffer.cpp
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "gbuffer.h"
#include "gassert.h"

//constructor
GBuffer::GBuffer(const GS32 in_byteCount)
: mArrayData()
{
	SetCount(in_byteCount);
	return;
}

GBuffer::GBuffer(const GBuffer& in_src)
: mArrayData()
{
	(*this) = in_src;
	return;
}

GBuffer::GBuffer()
: mArrayData()
{
	return;
}

GBuffer::~GBuffer()
{
	return;
}

//operators
const GBuffer& GBuffer::operator=(const GBuffer& in_rhs)
{
	if (this != &in_rhs)
	{
		mArrayData = in_rhs.mArrayData;
	}
	return (*this);
}

//public methods
//assert if not enough space
void GBuffer::SetData(
	const GS32 in_byteOffset, 
	const GS32 in_byteLength,
	const GU8* const in_data
	)
{
	if (0 == in_byteLength)
	{
		return;
	}

	GASSERT(NULL != in_data, "invalid param");
	GASSERT(in_byteOffset + in_byteLength <= GetCount(), "invalid param");

	GASSERT(0 != mArrayData.size(), "invalid state");
	GU8* const pData = &mArrayData[0];
	for (GS32 index = 0; index < in_byteLength; ++index)
	{
		pData[in_byteOffset + index] = in_data[index];
	}

	return;
}

void GBuffer::AppendData(
	const GS32 in_byteLength,
	const GU8* const in_data
	)
{
	if (0 == in_byteLength)
	{
		return;
	}
	const GS32 oldSize = GetCount();
	SetCount(oldSize + in_byteLength);

	SetData(oldSize, in_byteLength, in_data);

	return;
}

void GBuffer::Clear()
{
	mArrayData.clear();
	return;
}

//public accessors
void GBuffer::SetCount(const int in_byteCount)
{
	mArrayData.resize(in_byteCount);
	return;
}

const int GBuffer::GetCount()const
{
	return (int)mArrayData.size();
}
