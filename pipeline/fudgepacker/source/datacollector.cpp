//file: DataCollector.cpp

#include "DataCollector.h"

#include "DataBlock.h"
#include "PackCommon.h"

typedef std::vector<U8> TArrayByte;
typedef boost::shared_ptr<DataBlock> TPointerDataBlock;
typedef boost::weak_ptr<DataBlock> TWeakDataBlock;
typedef std::map<TWeakDataBlock, int> TMapDataBlockInt;
typedef std::vector<TWeakDataBlock> TArrayWeakDataBlock;
typedef std::map<int, TArrayWeakDataBlock> TMapIntArrayWeakDataBlock;

typedef std::pair<int, TWeakDataBlock> TPairIntWeakDataBlock;
typedef std::vector<TPairIntWeakDataBlock> TArrayPairIntWeakDataBlock;

typedef boost::shared_ptr<DataBlock> TPointerDataBlock;
typedef boost::weak_ptr<DataBlock> TWeakDataBlock;

typedef std::vector<TPointerDataBlock> TArrayPointerDataBlock;

static void LocalWriteDataBlock(
	TArrayByte& out_arrayData, 
	const TPointerDataBlock& in_dataBlock, 
	TMapDataBlockInt& inout_mapDataBlockWrittenOffset, //where each in_dataBlock has been written
	TArrayPairIntWeakDataBlock& inout_arrayWrittenPointerOffset //the offset in out_arrayData that had pointers, and what they pointed to
	)
{
	if (!in_dataBlock)
	{
		return;
	}

	const int alignment = in_dataBlock->GetAlignment();
	PackCommon::Pad(out_arrayData, alignment);

	TWeakDataBlock weakDataBlock = in_dataBlock;
	const int baseOffset = out_arrayData.size();
	inout_mapDataBlockWrittenOffset[weakDataBlock] = baseOffset;
	//inout_arrayWrittenPointerOffset.push_back(TArrayPairIntWeakDataBlock::value_type(baseOffset, weakDataBlock));
	out_arrayData.insert(out_arrayData.end(), in_dataBlock->GetArrayData().begin(), in_dataBlock->GetArrayData().end());

	const TArrayPairIntWeakDataBlock& dataParamPointerArray = in_dataBlock->GetArrayPointerOffset();
	for (TArrayPairIntWeakDataBlock::const_iterator iterator = dataParamPointerArray.begin(); iterator != dataParamPointerArray.end(); ++iterator)
	{
		inout_arrayWrittenPointerOffset.push_back(TArrayPairIntWeakDataBlock::value_type(
			(*iterator).first + baseOffset,
			(*iterator).second
			));
	}

	return;
}

static void LocalFixPointerOffset(
	TArrayByte& out_arrayData,
	const TMapDataBlockInt& in_mapDataBlockWrittenOffset, 
	const TArrayPairIntWeakDataBlock& in_arrayWrittenPointerOffset
	)
{
	for (TArrayPairIntWeakDataBlock::const_iterator iterator = in_arrayWrittenPointerOffset.begin(); iterator != in_arrayWrittenPointerOffset.end(); ++iterator)
	{
		TMapDataBlockInt::const_iterator found = in_mapDataBlockWrittenOffset.find((*iterator).second);
		if (found == in_mapDataBlockWrittenOffset.end())
		{
			continue;
		}

		PackCommon::Replace<U32>(
			out_arrayData,
			(*found).second,
			(*iterator).first
			);
	}

	return;
}

static void LocalWritePointerTable(
	TArrayByte& out_arrayData,
	const TArrayPairIntWeakDataBlock& in_arrayWrittenPointerOffset
	)
{
	PackCommon::Pad(out_arrayData, 4);
	const int baseOffset = out_arrayData.size();
	PackCommon::Replace<U32>(out_arrayData, baseOffset, 0);
	PackCommon::Append<U32>(out_arrayData, in_arrayWrittenPointerOffset.size());

	for (TArrayPairIntWeakDataBlock::const_iterator iterator = in_arrayWrittenPointerOffset.begin(); iterator != in_arrayWrittenPointerOffset.end(); ++iterator)
	{
		PackCommon::Append<U32>(out_arrayData, (*iterator).first);
	}

	return;
}

// construction
DataCollector::DataCollector()
: mRootDataBlock()
, mLabelMap()
, mArrayDataBlock()
{
	return;
}

DataCollector::~DataCollector()
{
	return;
}

// public methods
void DataCollector::CreateRootDataBlock(const DataBlockParam& in_dataBlockParam)
{
	mRootDataBlock.reset(new DataBlock(in_dataBlockParam));
	return;
}

DataCollector::TWeakDataBlock DataCollector::CreateDataBlock(
	const DataBlockParam& in_dataBlockParam, 
	const std::string& in_label
	)
{
	TWeakDataBlock weakPointer;
	if (0 == in_dataBlockParam.GetArrayData().size())
	{
		return weakPointer;
	}
	
	//find exsisting by label
	if (!in_label.empty())
	{
		TMapStringDataBlock::iterator found = mLabelMap.find(in_label);
		if (found != mLabelMap.end())
		{
			return (*found).second;
		}
	}

	//find exsisting by data match
	TPointerDataBlock pointer;
	for (TArrayPointerDataBlock::iterator iterator = mArrayDataBlock.begin(); iterator != mArrayDataBlock.end(); ++iterator)
	{
		if ((*iterator)->TestDataBlockMatch(in_dataBlockParam))
		{
			pointer = (*iterator);
			break;
		}
	}

	if (!pointer)
	{
		pointer.reset(new DataBlock(in_dataBlockParam));
		mArrayDataBlock.push_back(pointer);
	}

	if (!in_label.empty())
	{
		mLabelMap[in_label] = pointer;
	}

	weakPointer = pointer;
	return weakPointer;
}

DataCollector::TWeakDataBlock DataCollector::FindDataBlock(
	const std::string& in_label
	)
{
	TMapStringDataBlock::iterator found = mLabelMap.find(in_label);
	if (found != mLabelMap.end())
	{
		return (*found).second;
	}
	TWeakDataBlock weakPointer;
	return weakPointer;
}

DataCollector::TWeakDataBlock DataCollector::CreateDataBlockString(
	const std::string& in_text,
	const std::string& in_label
	)
{
	DataBlockParam dataBlockParam(1);
	TArrayByte arrayByte;
	PackCommon::StringToArray(arrayByte, in_text);
	dataBlockParam.AddArray(arrayByte);
	TWeakDataBlock weakPointer = CreateDataBlock(
		dataBlockParam,
		in_label
		);
	return weakPointer;
}

void DataCollector::WriteOutDataBlock(TArrayByte& out_arrayData)const
{
	//space for the pointer table offset
	PackCommon::Append<U32>(out_arrayData, 0);

	TMapDataBlockInt mapDataBlockWrittenOffset;
	TArrayPairIntWeakDataBlock arrayWrittenPointerOffset;
	TMapIntArrayWeakDataBlock mapSortedDataBlockToWrite;

	//write the data root
	LocalWriteDataBlock(out_arrayData, mRootDataBlock, mapDataBlockWrittenOffset, arrayWrittenPointerOffset);

	//sort the data blocks
	for (TArrayPointerDataBlock::const_iterator iterator = mArrayDataBlock.begin(); iterator != mArrayDataBlock.end(); ++iterator)
	{
		int sortValue = (*iterator)->GetWriteSortValue();
		TMapIntArrayWeakDataBlock::iterator found = mapSortedDataBlockToWrite.find(sortValue);
		if (found != mapSortedDataBlockToWrite.end())
		{
			(*found).second.push_back(*iterator);
		}
		else
		{
			TArrayWeakDataBlock arrayData;
			arrayData.push_back(*iterator);
			mapSortedDataBlockToWrite[sortValue] = arrayData;
		}
	}

	//write the data blocks
	for (TMapIntArrayWeakDataBlock::iterator iterator = mapSortedDataBlockToWrite.begin(); iterator != mapSortedDataBlockToWrite.end(); ++iterator)
	{
		for (TArrayWeakDataBlock::iterator subIterator = (*iterator).second.begin(); subIterator != (*iterator).second.end(); ++subIterator)
		{
			LocalWriteDataBlock(out_arrayData, (*subIterator).lock(), mapDataBlockWrittenOffset, arrayWrittenPointerOffset);
		}
	}

	//fix the pointers
	LocalFixPointerOffset(
		out_arrayData,
		mapDataBlockWrittenOffset, 
		arrayWrittenPointerOffset
		);

	//write the pointer table
	LocalWritePointerTable(
		out_arrayData,
		arrayWrittenPointerOffset
		);

	return;
}

