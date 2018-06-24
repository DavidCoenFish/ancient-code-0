//file: StreamInfo.h
#ifndef _StreamInfo_H_
#define _StreamInfo_H_

#include <boost/noncopyable.hpp>
#include <vector>

class TiXmlElement;

/**/
class StreamInfo : public boost::noncopyable 
{
	//typedef
private:
	typedef std::vector<unsigned char> TArrayByte;

	//construction
public:
	StreamInfo(
		const int in_type,
		const int in_count,
		const int in_usage,
		const int in_vertexDataByteOffset
		);
	~StreamInfo();

	//public members
public:
	void AddByte(const unsigned char in_value);
	void AddFloat(const float in_value);
	void AddInt(const int in_value);

	void AppendDataToArrayByte(
		TArrayByte& inout_arrayByte,
		const int in_index
		);

	//public accessors
public:
	const int GetType()const{ return mType; }
	const int GetCount()const{ return mCount; }
	const int GetUsage()const{ return mUsage; }
	const int GetStride()const{ return mStride; }
	const int GetVertexDataByteOffset()const{ return mVertexDataByteOffset; }
	//const TArrayByte& GetArrayByte()const{ return mArrayByte; }

	const bool GetIsPosition()const;
	const bool GetIsNormal()const;

	//private members
private:
	const int mType;
	const int mCount;
	const int mUsage;
	const int mVertexDataByteOffset;
	const int mStride;

	TArrayByte mArrayByte;

};

#endif // _StreamInfo_H_