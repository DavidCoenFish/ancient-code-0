//file: pointerdata.h
#ifndef _pointerdata_H_
#define _pointerdata_H_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <TCommon.h>
#include <string>
#include <vector>

/*

*/
class PointerData : public boost::noncopyable
{
	// typedefs
private:
	typedef std::vector<U8> TArrayByte;
	typedef std::vector<int> TArrayInt;
	typedef boost::shared_ptr<PointerData> TPointerPointerData;
	typedef std::pair<int, TPointerPointerData> TPairIntPointerPointerData;
	typedef std::vector<TPairIntPointerPointerData> TArrayPairIntPointerPointerData;

	// construction
public:
	//pointer to string constructor
	PointerData(
		const std::string& in_string
		);
	//pointer to data
	PointerData(
		const int in_allignment,
		const std::string& in_lable
		);
	~PointerData();

	void AddU8(const U8 in_data);
	void AddU16(const U16 in_data);
	void AddU32(const U32 in_data);
	void AddR32(const R32 in_data);
	void AddPointer(TPointerPointerData& in_pointerData);

	const int GetSortValue()const;

	const int GetAllignment()const{ return mAllignment; }
	const TArrayByte& GetArrayData()const{ return mArrayData; }
	const TArrayPairIntPointerPointerData& GetArrayPointerOffset()const{ return mArrayPointerOffset; }

	//private members
private:
	const int mAllignment;
	const std::string mLable;
	TArrayByte mArrayData;
	TArrayPairIntPointerPointerData mArrayPointerOffset;

};

#endif // _packCommon_H_