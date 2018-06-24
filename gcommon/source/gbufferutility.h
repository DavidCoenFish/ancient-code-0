//
//  gbufferutility.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#ifndef _GBufferUtility_h_
#define _GBufferUtility_h_

#include <string>
#include "gcommon.h"
#include "gassert.h"
#include "gbuffer.h"

//class GBuffer;

class GBufferUtility
{
	//////////////////////////////////////////////////
	//disabled
private:
	~GBufferUtility();

	//////////////////////////////////////////////////
	//static public methods
public:
	//to be able to use file contents as string
	static void AppendNull(GBuffer& inout_buffer);

	static void LoadBufferApplyPointerTable(GBuffer& inout_loadBuffer);

	template<typename IN_TYPELOAD>
	static const IN_TYPELOAD& GetLoadBufferRoot(GBuffer& inout_loadBuffer)
	{
		GASSERT(0 != inout_loadBuffer.GetCount(), "buffer empty");
		return *((IN_TYPELOAD*)(inout_loadBuffer.GetData() + 4));
	}

	template <typename IN_TYPE>
	static const IN_TYPE& GetItem(const GBuffer& in_buffer, const GS32 in_index)
	{
		GASSERT((0 <= in_index) && (in_index < (GS32)(in_buffer.GetCount() / sizeof(IN_TYPE))), "invalid param");
		return *((const IN_TYPE*)&(in_buffer.GetData()[in_index * sizeof(IN_TYPE)]));
	}

	template <typename IN_TYPE>
	static IN_TYPE& GetItem(GBuffer& in_buffer, const GS32 in_index)
	{
		GASSERT((0 <= in_index) && (in_index < (GS32)(in_buffer.GetCount() / sizeof(IN_TYPE))), "invalid param");
		return *((IN_TYPE*)&(in_buffer.GetData()[in_index * sizeof(IN_TYPE)]));
	}

	template <typename IN_TYPE>
	static const IN_TYPE GetItemSafe(GS32& inout_cursorByte, const GBuffer& in_buffer)
	{
		if ((inout_cursorByte < 0) || ((GS32)in_buffer.GetCount() < (inout_cursorByte + (GS32)sizeof(IN_TYPE))))
		{
			return 0;
		}
		const IN_TYPE result = GetItemOffset<IN_TYPE>(in_buffer, inout_cursorByte);
		inout_cursorByte += sizeof(IN_TYPE);
		return result;
	}

	static const std::string GetStringSafe(GS32& inout_cursorByte, const GBuffer& in_buffer);
	static void SetString(GBuffer& inout_buffer, const std::string& in_string);

	template <typename IN_TYPE>
	static const IN_TYPE GetItemOffset(const GBuffer& in_buffer, const GS32 in_byteOffset)
	{
		GASSERT((0 <= in_byteOffset) && (in_byteOffset + (GS32)sizeof(IN_TYPE) <= in_buffer.GetCount()), "invalid param");
		IN_TYPE result;
		GU8* const pTrace = (unsigned char* const)&result;
		const GU8* const pSource = in_buffer.GetData();
		for (GS32 index = 0; index < sizeof(IN_TYPE); ++index)
		{
			pTrace[index] = pSource[in_byteOffset + index];
		}
		return result;
	}

	template <typename IN_TYPE>
	static void SetItemOffset(GBuffer& in_buffer, const GS32 in_byteOffset, const IN_TYPE in_value)
	{
		GASSERT((0 <= in_byteOffset) && (in_byteOffset + (int)sizeof(IN_TYPE) <= in_buffer.GetCount()), "invalid param");
		unsigned char* const pTrace = in_buffer.GetData();
		const unsigned char* const pSource = (unsigned char* const)&in_value;
		for (int index = 0; index < sizeof(IN_TYPE); ++index)
		{
			 pTrace[in_byteOffset + index] = pSource[index];
		}
		return;
	}

};

#endif 