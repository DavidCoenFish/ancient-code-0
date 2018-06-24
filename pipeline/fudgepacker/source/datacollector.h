//file: DataCollector.h
#ifndef _DataCollector_H_
#define _DataCollector_H_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <TCommon.h>
#include <string>
#include <vector>
#include <map>

class DataBlock;
class DataBlockParam;

/*
	data block labels are required that first useage of a label defines the data... makes things quite a bit easier
*/
class DataCollector : public boost::noncopyable
{
	// typedefs
private:
	typedef std::vector<U8> TArrayByte;

	typedef boost::shared_ptr<DataBlock> TPointerDataBlock;
	typedef boost::weak_ptr<DataBlock> TWeakDataBlock;

	typedef std::vector<TPointerDataBlock> TArrayPointerDataBlock;

	typedef std::map<std::string, TWeakDataBlock> TMapStringDataBlock;

	// construction
public:
	DataCollector();
	~DataCollector();

	// public methods
public:
	void CreateRootDataBlock(const DataBlockParam& in_dataBlockParam);

	TWeakDataBlock CreateDataBlock(
		const DataBlockParam& in_dataBlockParam, 
		const std::string& in_label = ""
		);

	TWeakDataBlock CreateReferencelabel(
		const std::string& in_label = ""
		);

	TWeakDataBlock FindDataBlock(
		const std::string& in_label
		);

	TWeakDataBlock CreateDataBlockString(
		const std::string& in_text,
		const std::string& in_label = ""
		);

	void FinishLoad();

	void WriteOutDataBlock(TArrayByte& out_arrayData)const;

	//private members
private:
	TPointerDataBlock mRootDataBlock;

	TMapStringDataBlock mLabelMap;

	TArrayPointerDataBlock mArrayDataBlock;

};

#endif // _packCommon_H_