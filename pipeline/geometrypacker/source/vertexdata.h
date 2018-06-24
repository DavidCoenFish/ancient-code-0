//file: VertexData.h
#ifndef _VertexData_H_
#define _VertexData_H_

#include <boost/noncopyable.hpp>
#include <vector>

class TiXmlElement;

/**/
class VertexData : public boost::noncopyable 
{
	//typedef
private:
	typedef std::vector<unsigned char> TArrayByte;

	//construction
public:
	VertexData();
	VertexData(const VertexData& in_src);
	~VertexData();

	//operators
public:
	const VertexData& operator=(const VertexData& in_rhs);
	const bool operator==(const VertexData& in_rhs)const;
	const bool operator<(const VertexData& in_rhs)const;

	//public accessors
public:
	const TArrayByte& GetArrayByte()const{ return mArrayByte; }
	TArrayByte& GetArrayByte(){ return mArrayByte; }

	//private members
private:
	TArrayByte mArrayByte;
};

#endif // _VertexData_H_