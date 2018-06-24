//
//  gmeshvertexdata.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "gmeshvertexdata.h"

//constructor
GMeshVertexData::GMeshVertexData(const GS32 in_vertexCount, const GS32 in_vertexStride)
: mBuffer(in_vertexCount * in_vertexStride)
{
	return;
}

GMeshVertexData::GMeshVertexData(const GS32 in_size, const GVOID* const in_data)
: mBuffer(in_size)
{
	mBuffer.SetData(0, in_size, (const GU8 *const)in_data);
	return;
}

GMeshVertexData::~GMeshVertexData()
{
	return;
}

//public methods
const GVOID* const GMeshVertexData::GetData()const
{
	return mBuffer.GetData();
}

GVOID* const GMeshVertexData::GetData()
{
	return mBuffer.GetData();
}
