//file: DscVertex.h
#ifndef _DscVertex_H_
#define _DscVertex_H_

#include <boost/noncopyable.hpp>
#include <vector>

class TiXmlElement;

/**/
class DscVertex : public boost::noncopyable 
{
	//typedef
private:
	typedef std::vector<unsigned char> TArrayByte;

	//construction
public:
	DscVertex(const int in_keyIndex, const TArrayByte& in_arrayByte);
	DscVertex(const DscVertex& in_src);
	~DscVertex();

	//operators
public:
	const DscVertex& operator=(const DscVertex& in_rhs);
	const bool operator==(const DscVertex& in_rhs)const;
	const bool operator<(const DscVertex& in_rhs)const;

	//public methods
public:
	void AppendCmpData(const TArrayByte& in_arrayByte);

	//public accessors
public:
	const int GetKeyIndex()const{ return mKeyIndex; }
	const TArrayByte& GetArrayByteData()const{ return mArrayByteData; }

	//private members
private:
	int mKeyIndex;
	TArrayByte mArrayByteData;
	TArrayByte mArrayByteCmp;

};

#endif // _DscVertex_H_