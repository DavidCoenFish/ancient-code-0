//
//  gbufferutility.cpp
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "gbufferutility.h"
#include "gbuffer.h"
#include "gassert.h"

/*static*/ void GBufferUtility::AppendNull(GBuffer& inout_buffer)
{
	unsigned char value = 0;
	inout_buffer.AppendData(1, &value);
	return;
}

/*static*/ const std::string GBufferUtility::GetStringSafe(GS32& inout_cursorByte, const GBuffer& in_buffer)
{
	std::string result;

	if ((inout_cursorByte < 0) || (in_buffer.GetCount() <= inout_cursorByte))
	{
		return result;
	}

	const GCHAR* const pTrace = (const GCHAR* const)in_buffer.GetData();;
	GCHAR temp[2];
	temp[1] = 0;
	do
	{
		const GCHAR value = pTrace[inout_cursorByte];
		temp[0] = value;
		inout_cursorByte += 1;
		result.append(&temp[0]);
		if (0 == value)
		{
			break;
		}
	}
	while (inout_cursorByte < in_buffer.GetCount());

	
	return result;
}

/*static*/ void GBufferUtility::SetString(GBuffer& inout_buffer, const std::string& in_string)
{
	inout_buffer.AppendData(in_string.size(), (const unsigned char *const)in_string.c_str());
	AppendNull(inout_buffer);
	return;
}

/*static*/ void GBufferUtility::LoadBufferApplyPointerTable(GBuffer& inout_loadBuffer)
{
	if (inout_loadBuffer.GetCount() < 4)
	{
		return;
	}
	const GS32 pointerTableOffset = GBufferUtility::GetItemOffset<GS32>(inout_loadBuffer, 0);
	if (0 == pointerTableOffset)
	{
		return;
	}

	//add the base address to all the pointers inside the buffer
	const GU32 bassAddress = (GU32)inout_loadBuffer.GetData();
	const GS32 pointerCount = GBufferUtility::GetItemOffset<GS32>(inout_loadBuffer, pointerTableOffset);
	for (GS32 index = 0; index < pointerCount; ++index)
	{
		//pointerOffset's + 1 is because the first item in the array is the size
		const GS32 pointerOffset = GBufferUtility::GetItemOffset<GS32>(inout_loadBuffer, pointerTableOffset + (sizeof(GS32) * (index + 1)));
		const GS32 pointerValue = GBufferUtility::GetItemOffset<GS32>(inout_loadBuffer, pointerOffset);
		if (0 == pointerValue)
		{
			//the null pointer case, we have 4 bytes at the start, so even reference to the root would be 4
			continue;
		}
		GBufferUtility::SetItemOffset<GS32>(inout_loadBuffer, pointerOffset, bassAddress + pointerValue);
	}

	//shrink the buffer, removes the pointer table
	inout_loadBuffer.SetCount(pointerTableOffset);

	return;
}
