//file: DscVertexPool.h
#ifndef _DscVertexPool_H_
#define _DscVertexPool_H_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <map>

class TiXmlElement;
class DscVertex;
class DscVertexPool;
class StreamInfo;

typedef std::vector<unsigned int> TArrayU32;

void DscVertexPoolFindDuplicate(
	TArrayU32& out_duplicateArray, 
	const int in_newVertexIndex, 
	const TArrayU32& in_arrayWritenVertexIndex, 
	const StreamInfo& in_streamInfo,
	const DscVertexPool& in_vertexPool
	);

void DscVertexPoolFindDuplicateKey(
	TArrayU32& out_duplicateArray, 
	const int in_keyIndex, 
	const TArrayU32& in_arrayWritenVertexIndex, 
	const DscVertexPool& in_vertexPool
	);

/**/
class DscVertexPool : public boost::noncopyable 
{
	//typedef
private:
	typedef std::vector<unsigned char> TArrayByte;

	typedef boost::shared_ptr<DscVertex> TPointerDscVertex;
	typedef std::vector<TPointerDscVertex> TArrayPointerDscVertex;
	typedef std::vector<int> TArrayInt;
	typedef std::map<int, TPointerDscVertex> TMapIntVertex;

	//construction
public:
	DscVertexPool();
	~DscVertexPool();

	//public methods
public:
	void AddVertex(
		const int in_vertexIndex,
		const int in_keyIndex,
		const TArrayByte& in_streamData
		);

	//allow morph data to split vertex in main body of vertex, don by adding additional cmp data for the vertex
	void AppendVertexCmpData(
		const int in_vertexIndex,
		const TArrayByte& in_additionalCmpData
		);

	void RemoveDuplicateVertex();

	const int GetNewIndex(const int in_vertexIndex)const;
	const TArrayByte& GetVertexStreamData(const int in_newIndex)const;
	const int GetKeyIndex(const int in_newIndex)const;

	const int GetNewCount()const{ return mNewCount; }

	//private members
private:
	TArrayPointerDscVertex mArrayPointerDscVertex;
	TArrayInt mArrayNewIndex;
	int mNewCount; //count of the new index, ie, max + 1

	TMapIntVertex mMapNewIndexVertex; //bad, multiple ownership of vertex

};

#endif // _DscVertexPool_H_