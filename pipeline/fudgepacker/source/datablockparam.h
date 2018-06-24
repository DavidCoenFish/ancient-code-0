//file: DataBlockParam.h
#ifndef _DataBlockParam_H_
#define _DataBlockParam_H_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <TCommon.h>
#include <string>
#include <vector>

class DataBlock;

/**/
class DataBlockParam //: public boost::noncopyable
{
	// typedefs
private:
	typedef std::vector<U8> TArrayByte;

	typedef boost::shared_ptr<DataBlock> TPointerDataBlock;
	typedef boost::weak_ptr<DataBlock> TWeakDataBlock;

	typedef std::pair<int, TWeakDataBlock> TPairIntWeakDataBlock;
	typedef std::vector<TPairIntWeakDataBlock> TArrayPairIntWeakDataBlock;

	// construction
public:
	DataBlockParam(const DataBlockParam& in_src);
	DataBlockParam(const int in_alignment = 4);
	~DataBlockParam();

	DataBlockParam& operator=(const DataBlockParam& in_rhs);

	//write to the data block
	void AddU8(const U8 in_data);
	void AddU16(const U16 in_data);
	void AddU32(const U32 in_data);
	void AddR32(const R32 in_data);
	void AddArray(const TArrayByte& in_arrayData);
	void AddPointer(TPointerDataBlock& in_DataBlock);

	const TArrayByte& GetArrayFlatData()const{ return mArrayFlatData; }
	const TArrayByte& GetArrayData()const{ return mArrayData; }
	const TArrayPairIntWeakDataBlock& GetArrayPointerOffset()const{ return mArrayPairIntWeakDataBlock; }
	const int GetAlignment()const{ return mAlignment; }

	const bool TestDataBlockMatch(const DataBlockParam& in_dataBlockParam)const;

	//bigger number gets data block writen towards start of file
	const int GetWriteSortValue()const;

	//private members
private:
	TArrayByte mArrayFlatData;
	TArrayByte mArrayData;
	TArrayPairIntWeakDataBlock mArrayPairIntWeakDataBlock;
	int mAlignment;

};

#endif // _packCommon_H_