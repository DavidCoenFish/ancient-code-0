//file: DataBlock.h
#ifndef _DataBlock_H_
#define _DataBlock_H_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <TCommon.h>
#include <string>
#include <vector>
#include "DataBlockParam.h"

class DataCollector;

/**/
class DataBlock : public boost::noncopyable
{
	friend DataCollector; //access constructor
	// typedefs
private:
	typedef std::vector<U8> TArrayByte;

	typedef boost::shared_ptr<DataBlock> TPointerDataBlock;
	typedef boost::weak_ptr<DataBlock> TWeakDataBlock;

	typedef std::pair<int, TWeakDataBlock> TPairIntWeakDataBlock;
	typedef std::vector<TPairIntWeakDataBlock> TArrayPairIntWeakDataBlock;

	// construction
private:
	DataBlock(const DataBlockParam& in_dataBlockParam);
public:
	~DataBlock();

	//public methods
public:
	//void SetDataBlock(const DataBlockParam& in_dataBlockParam);

	//void ReplacePointer(TPointerDataBlock& in_oldPointer, TPointerDataBlock& in_newPointer);
	//void GenerateFlatData();
	const bool TestDataBlockMatch(const DataBlockParam& in_dataBlock)const;

	const TArrayByte& GetArrayFlatData()const;
	const TArrayByte& GetArrayData()const;
	const TArrayPairIntWeakDataBlock& GetArrayPointerOffset()const;
	const int GetAlignment()const;

	const int GetWriteSortValue()const;

	//private members
private:
	//resolve labels with a preload pass
	////making it non const allows labels...
	////////would have been nice to const the param, but is convient to have late assignment so we can have labeld dataBlocks create before the data is loaded
	//////DataBlockParam mDataBlockParam;
	////const DataBlockParam mDataBlockParam;
	//DataBlockParam mDataBlockParam;
	const DataBlockParam mDataBlockParam;

};

#endif // _packCommon_H_