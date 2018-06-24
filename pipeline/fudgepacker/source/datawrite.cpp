//file: pointerdata.cpp

#include "pointerdata.h"
#include "packcommon.h"
#include "main.h"
#include "TXml.h"
#include <tinyxml.h>

// construction
PointerData::PointerData(
	const std::string& in_string
	)
	: mAllignment(1)
	, mLable()
	, mArrayData()
	, mArrayPointerOffset()
{
	if (in_string.empty())
	{
		return;
	}

	for (int index = 0; index < (int)in_string.size(); ++index)
	{
		mArrayData.push_back(in_string[index]);
	}
	mArrayData.push_back(0);

	return;
}
PointerData::PointerData(
	const int in_allignment,
	const std::string& in_lable
	)
	: mAllignment(in_allignment)
	, mLable(in_lable)
	, mArrayData()
	, mArrayPointerOffset()
{
	return;
}

PointerData::~PointerData()
{
	return;
}

void PointerData::AddU8(const U8 in_data)
{
	PackCommon::Append(mArrayData, in_data);
	return;
}

void PointerData::AddU16(const U16 in_data)
{
	PackCommon::Append(mArrayData, in_data);
	return;
}

void PointerData::AddU32(const U32 in_data)
{
	PackCommon::Append(mArrayData, in_data);
	return;
}

void PointerData::AddR32(const R32 in_data)
{
	PackCommon::Append(mArrayData, in_data);
	return;
}

void PointerData::AddPointer(TPointerPointerData& in_pointerData)
{
	const int offset = (int)mArrayData.size();
	PackCommon::Append(mArrayData, U32(0));
	mArrayPointerOffset.push_back(TPairIntPointerPointerData(offset, in_pointerData));
	return;
}

const int PointerData::GetSortValue()const
{
	int result = (mAllignment << 24) | (mArrayData.size());
	return -result;
}
