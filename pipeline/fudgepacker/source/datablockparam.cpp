//file: DataBlockParam.cpp

#include "DataBlockParam.h"
#include "DataBlock.h"
#include "packcommon.h"
#include "main.h"

// typedefs
typedef std::vector<U8> TArrayByte;

static void LocalAppendFlatData(
	TArrayByte& inout_arrayFlatData, 
	const TArrayByte& in_arrayData,
	const int in_lowWater,
	const int in_highWater
	)
{
	for (int index = in_lowWater; index < in_highWater; ++index)
	{
		inout_arrayFlatData.push_back(in_arrayData[index]);
	}
	return;
}

// construction
DataBlockParam::DataBlockParam(const DataBlockParam& in_src)
: mArrayFlatData()
, mArrayData()
, mArrayPairIntWeakDataBlock()
, mAlignment(0)
{
	(*this) = in_src;
	return;
}

DataBlockParam::DataBlockParam(const int in_alignment)
: mArrayFlatData()
, mArrayData()
, mArrayPairIntWeakDataBlock()
, mAlignment(in_alignment)
{
	return;
}

DataBlockParam::~DataBlockParam()
{
	return;
}

DataBlockParam& DataBlockParam::operator=(const DataBlockParam& in_rhs)
{
	if (this != &in_rhs)
	{
		mArrayFlatData = in_rhs.mArrayFlatData;
		mArrayData = in_rhs.mArrayData;
		mArrayPairIntWeakDataBlock = in_rhs.mArrayPairIntWeakDataBlock;
		mAlignment = in_rhs.mAlignment;
	}
	return (*this);
}

//write to the data block
void DataBlockParam::AddU8(const U8 in_data)
{
	PackCommon::Append(mArrayFlatData, in_data);
	PackCommon::Append(mArrayData, in_data);
	return;
}

void DataBlockParam::AddU16(const U16 in_data)
{
	PackCommon::Append(mArrayFlatData, in_data);
	PackCommon::Append(mArrayData, in_data);
	return;
}

void DataBlockParam::AddU32(const U32 in_data)
{
	PackCommon::Append(mArrayFlatData, in_data);
	PackCommon::Append(mArrayData, in_data);
	return;
}

void DataBlockParam::AddR32(const R32 in_data)
{
	PackCommon::Append(mArrayFlatData, in_data);
	PackCommon::Append(mArrayData, in_data);
	return;
}

void DataBlockParam::AddArray(const TArrayByte& in_arrayData)
{
	mArrayFlatData.insert(mArrayFlatData.end(), in_arrayData.begin(), in_arrayData.end());
	mArrayData.insert(mArrayData.end(), in_arrayData.begin(), in_arrayData.end());
	return;
}

void DataBlockParam::AddPointer(TPointerDataBlock& in_dataBlock)
{
	if (in_dataBlock)
	{
		mArrayFlatData.insert(mArrayFlatData.end(), in_dataBlock->GetArrayFlatData().begin(), in_dataBlock->GetArrayFlatData().end());
		const int offset = mArrayData.size();
		PackCommon::Append<U32>(mArrayData, 0);	
		mArrayPairIntWeakDataBlock.push_back(TArrayPairIntWeakDataBlock::value_type(offset, in_dataBlock));
	}
	else
	{
		PackCommon::Append<U32>(mArrayFlatData, 0);	
		PackCommon::Append<U32>(mArrayData, 0);	
	}

	return;
}

const bool DataBlockParam::TestDataBlockMatch(const DataBlockParam& in_dataBlockParam)const
{
	//data size match
	if (mArrayFlatData.size() != in_dataBlockParam.mArrayFlatData.size())
	{
		return false;
	}

	//alignment match
	if (0 != (mAlignment % in_dataBlockParam.mAlignment))
	{
		return false;
	}

	for (int index = 0; index < (int)mArrayFlatData.size(); ++index)
	{
		if (mArrayFlatData[index] != in_dataBlockParam.mArrayFlatData[index])
		{
			return false;
		}
	}

	return true;
}

//smaller number gets data block writen towards start of file
const int DataBlockParam::GetWriteSortValue()const
{
	const int size = -(int)((mAlignment << 24) + mArrayData.size());
	return size;
}
