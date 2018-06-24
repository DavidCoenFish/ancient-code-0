//file: ModifierMorphTargetStream.h
#ifndef _ModifierMorphTargetStream_H_
#define _ModifierMorphTargetStream_H_

#include <boost/noncopyable.hpp>
#include <vector>
#include <map>

class TiXmlElement;

/**/
class ModifierMorphTargetStream : public boost::noncopyable 
{
	//typedef
private:
	typedef std::vector<unsigned int> TArrayU32;
	typedef std::vector<unsigned char> TArrayU8;
	typedef std::map<int, int> TMapIntInt;

	//constructor
public:
	ModifierMorphTargetStream();
	~ModifierMorphTargetStream();

	//public methods
public:
	void WriteOutData(
		TiXmlElement& out_parentElement
		)const;

	//public accessors
public:
	TArrayU8& GetStreamData(){ return mStreamData; }
	TArrayU32& GetDuplicateOffsetArray(){ return mDuplicateOffsetArray; }

	const bool TestWritten(const int in_vertexNewIndex);

	//private members
private:
	TArrayU8 mStreamData;
	TArrayU32 mDuplicateOffsetArray;
	TMapIntInt mMapWritenVertexNewIndex;
};

#endif // _ModifierMorphTargetStream_H_