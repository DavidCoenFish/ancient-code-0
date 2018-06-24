//file: Edge.h
#ifndef _Edge_H_
#define _Edge_H_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

class DscVertexPool;
struct Face;
class TiXmlElement;
class StreamInfo;

class Edge : public boost::noncopyable 
{
	//typedef
private:
	struct TEdgeOnePoly
	{
		int mVertexIndex[2];
		int mNormalIndex;
	};
	struct TEdgeTwoPoly
	{
		int mVertexIndex[2];
		int mNormalIndex[2];
	};
	struct TNormalStatic
	{
		float mData[3];
	};
	struct TNormalDynamic
	{
		int mVertexIndex[3];
	};

	typedef std::vector<TEdgeOnePoly> TArrayEdgeOnePoly;
	typedef std::vector<TEdgeTwoPoly> TArrayEdgeTwoPoly;
	typedef std::vector<TNormalStatic> TArrayNormalStatic;
	typedef std::vector<TNormalDynamic> TArrayNormalDynamic;

	typedef std::vector<unsigned int> TArrayU32;
	typedef std::vector<Face> TArrayFace;
	typedef boost::shared_ptr<StreamInfo> TPointerStreamInfo;
	typedef std::vector<TPointerStreamInfo> TArrayPointerStreamInfo;

	//static public methods
public:
	static void CollectData(
		Edge& out_edge,
		const TArrayFace in_arrayFace, 
		const DscVertexPool& in_vertexPool, 
		const TArrayU32& in_arrayWritenVertexIndex,
		const TArrayPointerStreamInfo& in_arrayStreamInfo,
		const bool in_staticEdge,
		const bool in_dynamicEdge
		);
	static void WriteOutEdge(
		TiXmlElement* const in_destinationElement, 
		const Edge& in_edge
		);

	//construction
public:
	Edge();
	~Edge();

	//public accessors
public:
	const TArrayEdgeOnePoly& GetArrayEdgeOnePoly()const { return mArrayEdgeOnePoly; }
	const TArrayEdgeTwoPoly& GetArrayEdgeTwoPoly()const { return mArrayEdgeTwoPoly; }
	const TArrayNormalStatic& GetArrayNormalStatic()const { return mArrayNormalStatic; }
	const TArrayNormalDynamic& GetArrayNormalDynamic()const { return mArrayNormalDynamic; }

	//private members
private:
	TArrayEdgeOnePoly mArrayEdgeOnePoly; 
	TArrayEdgeTwoPoly mArrayEdgeTwoPoly;
	TArrayNormalStatic mArrayNormalStatic;
	TArrayNormalDynamic mArrayNormalDynamic;

};

#endif // _Edge_H_