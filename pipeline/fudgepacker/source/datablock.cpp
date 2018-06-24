//file: DataBlock.cpp

#include "DataBlock.h"

#include "DataBlockParam.h"

// construction
DataBlock::DataBlock(const DataBlockParam& in_dataBlockParam)
: mDataBlockParam(in_dataBlockParam)
{
	return;
}

DataBlock::~DataBlock()
{
	return;
}

//public methods
//void DataBlock::SetDataBlock(const DataBlockParam& in_dataBlockParam)
//{
//	mDataBlockParam = in_dataBlockParam;
//	return;
//}
//
//void DataBlock::ReplacePointer(TPointerDataBlock& in_oldPointer, TPointerDataBlock& in_newPointer)
//{
//	mDataBlockParam.ReplacePointer(in_oldPointer, in_newPointer);
//	return;
//}
//
//void DataBlock::GenerateFlatData()
//{
//	mDataBlockParam.GenerateFlatData();
//	return;
//}

const bool DataBlock::TestDataBlockMatch(const DataBlockParam& in_dataBlock)const
{
	return mDataBlockParam.TestDataBlockMatch(in_dataBlock);
}

const DataBlock::TArrayByte& DataBlock::GetArrayFlatData()const
{
	return mDataBlockParam.GetArrayFlatData();
}

const DataBlock::TArrayByte& DataBlock::GetArrayData()const
{
	return mDataBlockParam.GetArrayData();
}

const DataBlock::TArrayPairIntWeakDataBlock& DataBlock::GetArrayPointerOffset()const
{
	return mDataBlockParam.GetArrayPointerOffset();
}

const int DataBlock::GetAlignment()const
{
	return mDataBlockParam.GetAlignment();
}

const int DataBlock::GetWriteSortValue()const
{
	return mDataBlockParam.GetWriteSortValue();
}
