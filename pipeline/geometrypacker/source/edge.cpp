//file: Edge.cpp

#include "Edge.h"

#include "DscVertexPool.h"
#include "Face.h"
#include "StreamInfo.h"

#include <TXml.h>
#include <TXmlPack.h>
#include <TCommon.h>
#include <tinyxml.h>
#include <GMeshType.h>
#include <GCommon.h>
#include <math.h>
#include <assert.h>

struct TNormal
{
	float mData[3];
};
typedef std::vector<TNormal> TArrayNormal;
struct TNormalIndex
{
	int mData[3];
};
typedef std::vector<TNormalIndex> TArrayNormalIndex;

struct TPolyData
{
	int mVertexIndex0;
	int mVertexIndex1;
	int mNormalIndex0;
	int mNormalIndex1;
};

typedef std::vector<Face> TArrayFace;
typedef std::pair<int, int> TPairIntInt;
typedef std::map<TPairIntInt, TPairIntInt> TMapPairIntIntPairIntInt;
typedef std::map<TPairIntInt, TPolyData> TMapPairIntIntPolyData;
typedef std::vector<unsigned char> TArrayByte;
typedef boost::shared_ptr<StreamInfo> TPointerStreamInfo;
typedef std::vector<TPointerStreamInfo> TArrayPointerStreamInfo;

static void LocalCrossProduct(float* const out_result, const float* const in_lhs, const float* const in_rhs)
{
	out_result[0] = (in_lhs[1] * in_rhs[2] ) - ( in_lhs[2] * in_rhs[1] );
	out_result[1] = (in_lhs[2] * in_rhs[0] ) - ( in_lhs[0] * in_rhs[2] );
	out_result[2] = (in_lhs[0] * in_rhs[1] ) - ( in_lhs[1] * in_rhs[0] );
	return;
}

static void LocalNormalise(float* const inout_result)
{
	const float lengthSquared = ((inout_result[0] * inout_result[0]) + (inout_result[1] * inout_result[1]) + (inout_result[2] * inout_result[2]));
	const float length = (float)sqrt(lengthSquared);
	if (0.0000001F < length)
	{
		inout_result[0] /= length;
		inout_result[1] /= length;
		inout_result[2] /= length;
	}
	return;
}

static const int LocalGetCreateNormalIndex(TArrayNormalIndex& inout_arrayNormalVertexNewIndex, const Face& in_face)
{
	TNormalIndex normalIndex;
	normalIndex.mData[0] = in_face.mIndex0;
	normalIndex.mData[1] = in_face.mIndex1;
	normalIndex.mData[2] = in_face.mIndex2;

	for (int index = 0; index < (int)inout_arrayNormalVertexNewIndex.size(); ++index)
	{
		const TNormalIndex& testNormalIndex = inout_arrayNormalVertexNewIndex[index];
		if (((normalIndex.mData[0] == testNormalIndex.mData[0]) &&
			 (normalIndex.mData[1] == testNormalIndex.mData[1]) &&
			 (normalIndex.mData[2] == testNormalIndex.mData[2])) ||
			((normalIndex.mData[0] == testNormalIndex.mData[1]) &&
			 (normalIndex.mData[1] == testNormalIndex.mData[2]) &&
			 (normalIndex.mData[2] == testNormalIndex.mData[0])) ||
			((normalIndex.mData[0] == testNormalIndex.mData[2]) &&
			 (normalIndex.mData[1] == testNormalIndex.mData[0]) &&
			 (normalIndex.mData[2] == testNormalIndex.mData[1])))
		{
			return index;
		}
	}
	const int size = inout_arrayNormalVertexNewIndex.size();
	inout_arrayNormalVertexNewIndex.push_back(normalIndex);
	return size;
}

static const int LocalGetWrittenIndexFromNewVertexIndex(
	const int in_newVertexIndex, 
	const TArrayU32& in_arrayWritenVertexIndex //array of order of writen vertex pool index
	)
{
	for (int index = 0; index < (int)in_arrayWritenVertexIndex.size(); ++index)
	{
		if (in_newVertexIndex == in_arrayWritenVertexIndex[index])
		{
			return index;
		}
	}

	return GCOMMON_INVALID_INDEX;
}

static void LocalGetNormal(
	float* const out_normal, 
	const Face& in_faceVertexIndex, 
	const DscVertexPool& in_vertexPool,
	const int in_positionOffset,
	const int in_positionCount
	)
{
	int vertexPoolIndex[3];
	vertexPoolIndex[0] = in_faceVertexIndex.mIndex0;
	vertexPoolIndex[1] = in_faceVertexIndex.mIndex1;
	vertexPoolIndex[2] = in_faceVertexIndex.mIndex2;

	float data[9];
	for (int index = 0; index < 9; ++index)
	{
		data[index] = 0.0F;
	}

	for (int index = 0; index < 3; ++index)
	{
		 const TArrayByte& arrayStreamData = in_vertexPool.GetVertexStreamData(vertexPoolIndex[index]);
		 float* const pData = (float*)(&arrayStreamData[in_positionOffset]); 
		 for (int subIndex = 0; subIndex < in_positionCount; ++subIndex)
		 {
			 data[(index * 3) + subIndex] = pData[subIndex];
		 }
	}

	float edge0[3];
	edge0[0] = data[3] - data[0];
	edge0[1] = data[4] - data[1];
	edge0[2] = data[5] - data[2];
	float edge1[3];
	edge1[0] = data[6] - data[0];
	edge1[1] = data[7] - data[1];
	edge1[2] = data[8] - data[2];

	LocalCrossProduct(out_normal, edge1, edge0);
	LocalNormalise(out_normal);

	return;
}

static const int LocalGetCreateNormal(
	TArrayNormal& inout_arrayNormal, 
	const Face& in_faceVertexIndex, 
	const DscVertexPool& in_vertexPool,
	const int in_positionOffset,
	const int in_positionCount
	)
{
	TNormal normal;
	LocalGetNormal(
		normal.mData, 
		in_faceVertexIndex, 
		in_vertexPool,
		in_positionOffset,
		in_positionCount
		);

	//find match for this normal
	int bestMatch = GCOMMON_INVALID_INDEX;
	float distance = 0.001F;
	for (int index = 0; index < (int)inout_arrayNormal.size(); ++index)
	{
		float offset[3];
		offset[0] = inout_arrayNormal[index].mData[0] - normal.mData[0];
		offset[1] = inout_arrayNormal[index].mData[1] - normal.mData[1];
		offset[2] = inout_arrayNormal[index].mData[2] - normal.mData[2];
		const float testDistance = ((offset[0] * offset[0]) + (offset[1] * offset[1]) + (offset[2] * offset[2]));
		if (testDistance < distance)
		{
			distance = testDistance;
			bestMatch = index;
		}
	}

	if (GCOMMON_INVALID_INDEX != bestMatch)
	{
		return bestMatch;
	}

	const int result = inout_arrayNormal.size();
	inout_arrayNormal.push_back(normal);

	return result;
}


//static public methods
/*static*/ void Edge::CollectData(
	Edge& out_edge,
	const TArrayFace in_arrayFace, //face uses vertex pool new index
	const DscVertexPool& in_vertexPool,
	const TArrayU32& in_arrayWritenVertexIndex, //array of order of writen vertex pool index
	const TArrayPointerStreamInfo& in_arrayStreamInfo,
	const bool in_staticEdge,
	const bool in_dynamicEdge
	)
{
	TArrayNormalIndex arrayNormalVertexNewIndex;
	TArrayNormal arrayNormal;
	TMapPairIntIntPolyData mapVertexKeyIndexPolyData;

	//find the position info
	int positionOffset = 0; 
	int positionCount = 0;
	for (int index = 0; index < (int)in_arrayStreamInfo.size(); ++index)
	{
		const StreamInfo& streamInfo = *(in_arrayStreamInfo[index]);
		if (GMeshType::TStreamUsage::TPosition == streamInfo.GetUsage())
		{
			positionOffset = streamInfo.GetVertexDataByteOffset();
			positionCount = streamInfo.GetCount();
			break;
		}
	}

	//collect the edges and normals
	for (TArrayFace::const_iterator iterator = in_arrayFace.begin(); iterator != in_arrayFace.end(); ++iterator)
	{
		const Face& face = (*iterator);

		//bail if this face never got written
		if ((GCOMMON_INVALID_INDEX == LocalGetWrittenIndexFromNewVertexIndex(face.mIndex0, in_arrayWritenVertexIndex)) ||
			(GCOMMON_INVALID_INDEX == LocalGetWrittenIndexFromNewVertexIndex(face.mIndex1, in_arrayWritenVertexIndex)) ||
			(GCOMMON_INVALID_INDEX == LocalGetWrittenIndexFromNewVertexIndex(face.mIndex2, in_arrayWritenVertexIndex)))
		{
			continue;
		}

		const int vertexKey0 = in_vertexPool.GetKeyIndex(face.mIndex0);
		const int vertexKey1 = in_vertexPool.GetKeyIndex(face.mIndex1);
		const int vertexKey2 = in_vertexPool.GetKeyIndex(face.mIndex2);

		int normalIndex = GCOMMON_INVALID_INDEX;
		if (in_staticEdge)
		{
			normalIndex = LocalGetCreateNormal(arrayNormal, face, in_vertexPool, positionOffset, positionCount);
		}
		else if (in_dynamicEdge)
		{
			normalIndex = LocalGetCreateNormalIndex(arrayNormalVertexNewIndex, face);
		}

		TPairIntInt edge[3];
		TPairIntInt edgeBackwards[3];
		TPolyData polyData[3];
		
		edge[0].first = vertexKey0;
		edge[0].second = vertexKey1;
		edgeBackwards[0].first = edge[0].second;
		edgeBackwards[0].second = edge[0].first;
		polyData[0].mVertexIndex0 = face.mIndex0;
		polyData[0].mVertexIndex1 = face.mIndex1;
		polyData[0].mNormalIndex0 = normalIndex;
		polyData[0].mNormalIndex1 = GCOMMON_INVALID_INDEX;
		
		edge[1].first = vertexKey1;
		edge[1].second = vertexKey2;
		edgeBackwards[1].first = edge[1].second;
		edgeBackwards[1].second = edge[1].first;
		polyData[1].mVertexIndex0 = face.mIndex1;
		polyData[1].mVertexIndex1 = face.mIndex2;
		polyData[1].mNormalIndex0 = normalIndex;
		polyData[1].mNormalIndex1 = GCOMMON_INVALID_INDEX;
		
		edge[2].first = vertexKey2;
		edge[2].second = vertexKey0;
		edgeBackwards[2].first = edge[2].second;
		edgeBackwards[2].second = edge[2].first;
		polyData[2].mVertexIndex0 = face.mIndex2;
		polyData[2].mVertexIndex1 = face.mIndex0;
		polyData[2].mNormalIndex0 = normalIndex;
		polyData[2].mNormalIndex1 = GCOMMON_INVALID_INDEX;

		assert(-1 != normalIndex);

		for (int index = 0; index < 3; ++index)
		{
			bool edgeFound = false;
			//can we attach to exsisting edge
			TMapPairIntIntPolyData::iterator found = mapVertexKeyIndexPolyData.find(edgeBackwards[index]);
			if ((found != mapVertexKeyIndexPolyData.end()) &&
				(GCOMMON_INVALID_INDEX == (*found).second.mNormalIndex1))
			{
				edgeFound = true;
				(*found).second.mNormalIndex1 = normalIndex;
			}

			//make a new edge
			if (!edgeFound)
			{
				mapVertexKeyIndexPolyData[edge[index]] = polyData[index];
			}
		}
	}

	//put the edge data into the struct
	for (TMapPairIntIntPolyData::iterator iterator = mapVertexKeyIndexPolyData.begin(); iterator != mapVertexKeyIndexPolyData.end(); ++iterator)
	{
		const TPolyData polyData = (*iterator).second;
		if ((GCOMMON_INVALID_INDEX != polyData.mNormalIndex0) &&
			(GCOMMON_INVALID_INDEX != polyData.mNormalIndex1))
		{
			TEdgeTwoPoly edgeTwoPoly;
			edgeTwoPoly.mVertexIndex[0] = LocalGetWrittenIndexFromNewVertexIndex(polyData.mVertexIndex0, in_arrayWritenVertexIndex);
			edgeTwoPoly.mVertexIndex[1] = LocalGetWrittenIndexFromNewVertexIndex(polyData.mVertexIndex1, in_arrayWritenVertexIndex);
			edgeTwoPoly.mNormalIndex[0] = polyData.mNormalIndex0;
			edgeTwoPoly.mNormalIndex[1] = polyData.mNormalIndex1;

			assert(edgeTwoPoly.mVertexIndex[0] != edgeTwoPoly.mVertexIndex[1]);
	
			//you know what, if the edge has the same normal on earch side, then don't add it, wont cast a shadow...
			if (edgeTwoPoly.mNormalIndex[0] != edgeTwoPoly.mNormalIndex[1])
			{
				out_edge.mArrayEdgeTwoPoly.push_back(edgeTwoPoly);
			}
		}
		else if (GCOMMON_INVALID_INDEX != polyData.mNormalIndex0)
		{
			TEdgeOnePoly edgeOnePoly;
			edgeOnePoly.mVertexIndex[0] = LocalGetWrittenIndexFromNewVertexIndex(polyData.mVertexIndex0, in_arrayWritenVertexIndex);
			edgeOnePoly.mVertexIndex[1] = LocalGetWrittenIndexFromNewVertexIndex(polyData.mVertexIndex1, in_arrayWritenVertexIndex);
			edgeOnePoly.mNormalIndex = polyData.mNormalIndex0;

			out_edge.mArrayEdgeOnePoly.push_back(edgeOnePoly);
		}
	}

	//put the normal data into the struct
	if (in_staticEdge)
	{
		for (TArrayNormal::iterator iterator = arrayNormal.begin(); iterator != arrayNormal.end(); ++iterator)
		{
			const TNormal& normal = (*iterator);
			TNormalStatic normalStatic;
			normalStatic.mData[0] = normal.mData[0];
			normalStatic.mData[1] = normal.mData[1];
			normalStatic.mData[2] = normal.mData[2];
			out_edge.mArrayNormalStatic.push_back(normalStatic);
		}
	}
	else if (in_dynamicEdge)
	{
		for (TArrayNormalIndex::iterator iterator = arrayNormalVertexNewIndex.begin(); iterator != arrayNormalVertexNewIndex.end(); ++iterator)
		{
			const TNormalIndex& normalIndex = (*iterator);
			TNormalDynamic normalDynamic;
			normalDynamic.mVertexIndex[0] = LocalGetWrittenIndexFromNewVertexIndex(normalIndex.mData[0], in_arrayWritenVertexIndex); 
			normalDynamic.mVertexIndex[1] = LocalGetWrittenIndexFromNewVertexIndex(normalIndex.mData[1], in_arrayWritenVertexIndex); 
			normalDynamic.mVertexIndex[2] = LocalGetWrittenIndexFromNewVertexIndex(normalIndex.mData[2], in_arrayWritenVertexIndex); 
			out_edge.mArrayNormalDynamic.push_back(normalDynamic);
		}
	}

	return;
}

/*static*/ void Edge::WriteOutEdge(
	TiXmlElement* const in_destinationElement, 
	const Edge& in_edge
	)
{
	TiXmlElement pointerElement("pointer");
	pointerElement.SetAttribute("alignment", 4);

	{
		TXmlPack::PutU32(&pointerElement, in_edge.mArrayEdgeOnePoly.size());
		TiXmlElement pointerData("pointer");
		pointerData.SetAttribute("alignment", 4);
		for (int index = 0; index < (int)in_edge.mArrayEdgeOnePoly.size(); ++index)
		{
			TXmlPack::PutU32(&pointerData, in_edge.mArrayEdgeOnePoly[index].mVertexIndex[0]);
			TXmlPack::PutU32(&pointerData, in_edge.mArrayEdgeOnePoly[index].mVertexIndex[1]);
			TXmlPack::PutU32(&pointerData, in_edge.mArrayEdgeOnePoly[index].mNormalIndex);
		}
		pointerElement.InsertEndChild(pointerData);
	}
	
	{
		TXmlPack::PutU32(&pointerElement, in_edge.mArrayEdgeTwoPoly.size());
		TiXmlElement pointerData("pointer");
		pointerData.SetAttribute("alignment", 4);
		for (int index = 0; index < (int)in_edge.mArrayEdgeTwoPoly.size(); ++index)
		{
			TXmlPack::PutU32(&pointerData, in_edge.mArrayEdgeTwoPoly[index].mVertexIndex[0]);
			TXmlPack::PutU32(&pointerData, in_edge.mArrayEdgeTwoPoly[index].mVertexIndex[1]);
			TXmlPack::PutU32(&pointerData, in_edge.mArrayEdgeTwoPoly[index].mNormalIndex[0]);
			TXmlPack::PutU32(&pointerData, in_edge.mArrayEdgeTwoPoly[index].mNormalIndex[1]);
		}
		pointerElement.InsertEndChild(pointerData);
	}
	
	{
		TXmlPack::PutU32(&pointerElement, in_edge.mArrayNormalStatic.size());
		TiXmlElement pointerData("pointer");
		pointerData.SetAttribute("alignment", 4);
		for (int index = 0; index < (int)in_edge.mArrayNormalStatic.size(); ++index)
		{
			TXmlPack::PutR32(&pointerData, in_edge.mArrayNormalStatic[index].mData[0]);
			TXmlPack::PutR32(&pointerData, in_edge.mArrayNormalStatic[index].mData[1]);
			TXmlPack::PutR32(&pointerData, in_edge.mArrayNormalStatic[index].mData[2]);
		}
		pointerElement.InsertEndChild(pointerData);
	}
	
	{
		TXmlPack::PutU32(&pointerElement, in_edge.mArrayNormalDynamic.size());
		TiXmlElement pointerData("pointer");
		pointerData.SetAttribute("alignment", 4);
		for (int index = 0; index < (int)in_edge.mArrayNormalDynamic.size(); ++index)
		{
			TXmlPack::PutU32(&pointerData, in_edge.mArrayNormalDynamic[index].mVertexIndex[0]);
			TXmlPack::PutU32(&pointerData, in_edge.mArrayNormalDynamic[index].mVertexIndex[1]);
			TXmlPack::PutU32(&pointerData, in_edge.mArrayNormalDynamic[index].mVertexIndex[2]);
		}
		pointerElement.InsertEndChild(pointerData);
	}

	in_destinationElement->InsertEndChild(pointerElement);

	return;
}

//construction
Edge::Edge()
{
	return;
}

Edge::~Edge()
{
	return;
}