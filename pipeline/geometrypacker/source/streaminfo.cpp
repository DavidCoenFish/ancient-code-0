//file: StreamInfo.cpp

#include "StreamInfo.h"
#include "GMeshType.h"

static const int LocalGetStride(const GMeshType::TStreamType::TEnum in_type, const int in_count)
{
	switch (in_type)
	{
	default:
		break;
	case GMeshType::TStreamType::TByte:
	case GMeshType::TStreamType::TUnsignedByte:
		return (sizeof(char) * in_count);
	case GMeshType::TStreamType::TShort:
	case GMeshType::TStreamType::TUnsignedShort:
		return (sizeof(short) * in_count);
	case GMeshType::TStreamType::TInt:
	case GMeshType::TStreamType::TUnsignedInt:
		return (sizeof(int) * in_count);
	case GMeshType::TStreamType::TFloat:
		return (sizeof(float) * in_count);
	}

	return 1;
}

//construction
StreamInfo::StreamInfo(
	const int in_type,
	const int in_count,
	const int in_usage,
	const int in_vertexDataByteOffset
	)
	: mType(in_type)
	, mCount(in_count)
	, mUsage(in_usage)
	, mStride(LocalGetStride((GMeshType::TStreamType::TEnum)in_type, in_count))
	, mVertexDataByteOffset(in_vertexDataByteOffset)
{
	return;
}

StreamInfo::~StreamInfo()
{
	return;
}

//public members
void StreamInfo::AddByte(const unsigned char in_value)
{
	mArrayByte.push_back(in_value);
	return;
}

void StreamInfo::AddFloat(const float in_value)
{
	unsigned char* pTrace = (unsigned char*)&in_value;
	for (int index = 0; index < sizeof(float); ++index)
	{
		mArrayByte.push_back(pTrace[index]);
	}
	return;
}

void StreamInfo::AddInt(const int in_value)
{
	unsigned char* pTrace = (unsigned char*)&in_value;
	for (int index = 0; index < sizeof(int); ++index)
	{
		mArrayByte.push_back(pTrace[index]);
	}
	return;
}

void StreamInfo::AppendDataToArrayByte(
	TArrayByte& inout_arrayByte,
	const int in_index
	)
{
	inout_arrayByte.insert(
		inout_arrayByte.end(),
		mArrayByte.begin() + (mStride * in_index),
		mArrayByte.begin() + (mStride * (in_index + 1))
		);
	return;
}

const bool StreamInfo::GetIsPosition()const
{
	switch (mUsage)
	{
	default:
		break;
	case GMeshType::TStreamUsage::TPosition:
		return true;
	}

	return false;
}

const bool StreamInfo::GetIsNormal()const
{
	switch (mUsage)
	{
	default:
		break;
	case GMeshType::TStreamUsage::TNormal:
	case GMeshType::TStreamUsage::TTangent0:
	case GMeshType::TStreamUsage::TTangent1:
		return true;
	}

	return false;
}
