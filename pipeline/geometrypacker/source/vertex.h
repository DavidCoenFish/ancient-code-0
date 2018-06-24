//file: Vertex.h
#ifndef _Vertex_H_
#define _Vertex_H_

#include <boost/noncopyable.hpp>
#include <vector>

class TiXmlElement;

/**/
class Vertex : public boost::noncopyable 
{
	//typedef
private:
	typedef std::vector<unsigned char> TArrayByte;

	//construction
public:
	Vertex(const int in_keyIndex);
	Vertex(const Vertex& in_src);
	~Vertex();

	//operators
public:
	const Vertex& operator=(const Vertex& in_rhs);
	const bool operator==(const Vertex& in_rhs)const;
	const bool operator<(const Vertex& in_rhs)const;

	//public accessors
public:
	const TArrayByte& GetArrayByte()const{ return mArrayByte; }
	TArrayByte& GetArrayByte(){ return mArrayByte; }
	const int GetKeyIndex()const{ return mKeyIndex; }

	//private members
private:
	int mKeyIndex;
	TArrayByte mArrayByte;
};

#endif // _Vertex_H_