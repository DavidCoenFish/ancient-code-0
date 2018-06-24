//file: PackGeometry.cpp

#include "PackGeometry.h"

#include "main.h"
#include "StreamInfo.h"
#include "Modifier.h"
#include "ModifierCloth.h"
#include "ModifierSkin.h"
#include "ModifierMorph.h"
#include "Vertex.h"
#include "Face.h"
#include "Edge.h"
#include "DscVertexPool.h"

#include <TPackerMessage.h>
#include <TXml.h>
#include <TXmlPack.h>
#include <TCommon.h>
#include <tinyxml.h>

#include <GMeshType.h>
#include <map>
#include <NvTriStrip.h>

#define WRITE_ARRAY

static const int sCacheSize = 1024;
/*
GRenderMeshLoad
	const int mTopologyType;
	const int mVertexCount;
	const int mFlag;
	const void* const mVertexIndexArray; //u8 or u16 depending on flag, can be null if not an indexed topology, else has mVertexCount items
	const int mVertexDataCount; //same as vertex count if not indexed geometry
	const int mVertexDataByteStride;
	const void* const mVertexData;
	const int mStreamInfoCount;
	const GRenderMeshStreamInfoLoad* const mArrayStreamInfo;
	const GRenderMeshEdgeLoad* const mRenderMeshEdgeLoad; //for shadow projection
	const int mModifierCount;
	const GMeshModifier* const mArrayModifier;
	const char* const mSkeletonName

GRenderMeshStreamInfoLoad
	int mType;
	int mCount;
	int mUsage;
	int mVertexDataByteOffset;

GMeshModifier
	const char* const mModifierName;
	const void* const mData;

GRenderMeshEdgeLoad
	const int mCountEdgeOnePoly
	const GRenderMeshEdgeOnePolyLoad
	const int mCountEdgeTwoPoly
	const GRenderMeshEdgeTwoPolyLoad
	const int mCountEdgeNormalStatic
	const GRenderMeshEdgeNormalStaticLoad
	const int mCountEdgeNormalDynamic
	const GRenderMeshEdgeNormalDynamicLoad

GRenderMeshEdgeOnePolyLoad
	const int mVertexIndexOne
	const int mVertexIndexTwo
	const int mNormalIndex

GRenderMeshEdgeTwoPolyLoad
	const int mVertexIndexOne
	const int mVertexIndexTwo
	const int mNormalIndexOne
	const int mNormalIndexTwo

GRenderMeshEdgeNormalStaticLoad
	const float mNormal[3]

GRenderMeshEdgeNormalDynamicLoad
	const int mVertexIndex[3]
	
*/

typedef std::vector<U8> TArrayU8;
typedef std::vector<U16> TArrayU16;
typedef std::vector<U32> TArrayU32;
typedef std::vector<Face> TArrayFace;

typedef boost::shared_ptr<StreamInfo> TPointerStreamInfo;
typedef std::vector<TPointerStreamInfo> TArrayPointerStreamInfo;

typedef boost::shared_ptr<Vertex> TPointerVertex;
typedef std::vector<TPointerVertex> TArrayPointerVertex;

typedef boost::shared_ptr<Modifier> TPointerModifier;
typedef std::vector<TPointerModifier> TArrayPointerModifier;

typedef std::map<Vertex, int> TMapVertexInt;

typedef std::pair<int, int> TPairIntInt;
typedef std::map<TPairIntInt, int> TMapPairIntIntInt;
typedef std::vector<int> TArrayInt;

// static local methods
static const GMeshType::TPrimitiveType::TEnum LocalTranslatePrimitiveType(
	const std::string& in_primitiveTypeName,
	TPackerMessage& in_packMessage
	)
{
	if (in_primitiveTypeName == "TPoint")
	{
		return GMeshType::TPrimitiveType::TPoint;
	}
	if (in_primitiveTypeName == "TLine")
	{
		return GMeshType::TPrimitiveType::TLine;
	}
	if (in_primitiveTypeName == "TLineStrip")
	{
		return GMeshType::TPrimitiveType::TLineStrip;
	}
	if (in_primitiveTypeName == "TLineLoop")
	{
		return GMeshType::TPrimitiveType::TLineLoop;
	}
	if (in_primitiveTypeName == "TTriangle")
	{
		return GMeshType::TPrimitiveType::TTriangle;
	}
	if (in_primitiveTypeName == "TTriangleStrip")
	{
		return GMeshType::TPrimitiveType::TTriangleStrip;
	}
	if (in_primitiveTypeName == "TTriangleFan")
	{
		return GMeshType::TPrimitiveType::TTriangleFan;
	}

	in_packMessage.AddError(std::string("could not understand primitiveType:") + in_primitiveTypeName);
	return GMeshType::TPrimitiveType::TTriangle;
}

static const GMeshType::TStreamType::TEnum LocalGetStreamType(const std::string& in_typeName, TPackerMessage& in_packMessage)
{
	if (in_typeName == "TByte")
	{
		return GMeshType::TStreamType::TByte;
	}
	if (in_typeName == "TUnsignedByte")
	{
		return GMeshType::TStreamType::TUnsignedByte;
	}
	if (in_typeName == "TShort")
	{
		return GMeshType::TStreamType::TShort;
	}
	if (in_typeName == "TUnsignedShort")
	{
		return GMeshType::TStreamType::TUnsignedShort;
	}
	if (in_typeName == "TUnsignedInt")
	{
		return GMeshType::TStreamType::TUnsignedInt;
	}
	if (in_typeName == "TFloat")
	{
		return GMeshType::TStreamType::TFloat;
	}
	if (in_typeName == "TByte")
	{
		return GMeshType::TStreamType::TByte;
	}
	
	in_packMessage.AddError(std::string("could not understand stream type:") + in_typeName);
	return GMeshType::TStreamType::TByte;
}

static const GMeshType::TStreamUsage::TEnum LocalGetStreamUsage(const std::string& in_usageName, TPackerMessage& in_packMessage)
{
	if (in_usageName == "TPosition")
	{
		return GMeshType::TStreamUsage::TPosition;
	}
	if (in_usageName == "TNormal")
	{
		return GMeshType::TStreamUsage::TNormal;
	}
	if (in_usageName == "TColour")
	{
		return GMeshType::TStreamUsage::TColour;
	}
	if (in_usageName == "TUv0")
	{
		return GMeshType::TStreamUsage::TUv0;
	}
	if (in_usageName == "TUv1")
	{
		return GMeshType::TStreamUsage::TUv1;
	}
	if (in_usageName == "TTangent0")
	{
		return GMeshType::TStreamUsage::TTangent0;
	}
	if (in_usageName == "TTangent1")
	{
		return GMeshType::TStreamUsage::TTangent1;
	}

	in_packMessage.AddError(std::string("could not understand stream usage:") + in_usageName);
	return GMeshType::TStreamUsage::TNone;
}

static void LocalGetStreamInfoArray(
	TArrayPointerStreamInfo& out_arrayStreamInfo,
	int& out_vertexStride,
	TiXmlElement* const in_sourceElement,
	TPackerMessage& inout_packMessage
	)
{
	out_arrayStreamInfo.clear();
	out_vertexStride = 0;

	TiXmlElement* pTrace = TiXmlHandle(in_sourceElement).FirstChildElement("streamInfoArray").FirstChildElement("streamInfo").Element();
	while (pTrace)
	{
		const std::string usageName = TXml::GetTextString(TiXmlHandle(pTrace).FirstChildElement("usage").Element());
		const std::string typeName = TXml::GetTextString(TiXmlHandle(pTrace).FirstChildElement("type").Element());
		const int count = TXml::GetTextInt(TiXmlHandle(pTrace).FirstChildElement("stride").Element());

		const GMeshType::TStreamType::TEnum type = LocalGetStreamType(typeName, inout_packMessage);
		const GMeshType::TStreamUsage::TEnum usage = LocalGetStreamUsage(usageName, inout_packMessage);

		if (0 != count)
		{
			TPointerStreamInfo pointer;
			pointer.reset(new StreamInfo(type, count, usage, out_vertexStride));
			out_vertexStride += pointer->GetStride();

			out_arrayStreamInfo.push_back(pointer);
		}

		pTrace = pTrace->NextSiblingElement("streamInfo");
	}

	return;
}

static const int LocalAddVertex(
	TArrayPointerVertex& inout_arrayVertex, 
	TMapVertexInt& inout_mapVertexInt, 
	TPointerVertex& in_vertex
	)
{
	TMapVertexInt::const_iterator iterator = inout_mapVertexInt.find(*in_vertex);
	if (iterator != inout_mapVertexInt.end())
	{
		return (*iterator).second;
	}

	const int vertexIndex = (int)inout_arrayVertex.size();
	inout_arrayVertex.push_back(in_vertex);
	inout_mapVertexInt[*in_vertex] = vertexIndex;

	return vertexIndex;
}

static void LocalCollectStreamData(
	TArrayU8& out_streamData, 
	TiXmlElement* const in_sourceElement
	)
{
	TiXmlElement* pTraceElement = TiXmlHandle(in_sourceElement).FirstChildElement().Element();
	while (pTraceElement)
	{
		if (std::string("streamByte") == pTraceElement->Value())
		{
			TiXmlElement* pItem = TiXmlHandle(pTraceElement).FirstChildElement("item").Element();
			while (pItem)
			{
				const unsigned char value = (unsigned char)TXml::GetTextInt(pItem);
				out_streamData.push_back(value);
				pItem = pItem->NextSiblingElement("item");
			}
		}
		else if (std::string("streamFloat") == pTraceElement->Value())
		{
			TiXmlElement* pItem = TiXmlHandle(pTraceElement).FirstChildElement("item").Element();
			while (pItem)
			{
				const float value = TXml::GetTextFloat(pItem);
				out_streamData.push_back(((unsigned char*)&value)[0]);
				out_streamData.push_back(((unsigned char*)&value)[1]);
				out_streamData.push_back(((unsigned char*)&value)[2]);
				out_streamData.push_back(((unsigned char*)&value)[3]);
				pItem = pItem->NextSiblingElement("item");
			}
		}
		else if (std::string("streamInt") == pTraceElement->Value())
		{
			TiXmlElement* pItem = TiXmlHandle(pTraceElement).FirstChildElement("item").Element();
			while (pItem)
			{
				const int value = TXml::GetTextInt(pItem);
				out_streamData.push_back(((unsigned char*)&value)[0]);
				out_streamData.push_back(((unsigned char*)&value)[1]);
				out_streamData.push_back(((unsigned char*)&value)[2]);
				out_streamData.push_back(((unsigned char*)&value)[3]);
				pItem = pItem->NextSiblingElement("item");
			}
		}

		pTraceElement = pTraceElement->NextSiblingElement();
	}

	return;
}


static void LocalCollectVertexData(
	DscVertexPool& out_vertexPool,
	TiXmlElement* const in_sourceElement
	)
{
	TMapVertexInt mapVertexInt;
	int trace = 0;

	{
		TiXmlElement* pTraceVertex = TiXmlHandle(in_sourceElement).FirstChildElement("vertexArray").FirstChildElement("vertex").Element();
		while (pTraceVertex)
		{
			const int keyIndex = TXml::GetTextInt(TiXmlHandle(pTraceVertex).FirstChildElement("keyIndex").Element());
			TArrayU8 streamData;
			LocalCollectStreamData(streamData, TiXmlHandle(pTraceVertex).FirstChildElement("streamArray").Element());

			out_vertexPool.AddVertex(
				trace,
				keyIndex,
				streamData
				);
			trace += 1;

			pTraceVertex = pTraceVertex->NextSiblingElement("vertex");
		}
	}

	//and for a laugh, go thought the modifiers incase there is morph target
	TiXmlElement* pTraceModifier = TiXmlHandle(in_sourceElement).FirstChildElement("modifierArray").FirstChildElement().Element();
	while (pTraceModifier)
	{
		if (std::string("modifierMorph") == pTraceModifier->Value())
		{
			TiXmlElement* pMorphTarget = TiXmlHandle(pTraceModifier).FirstChildElement("target").Element();
			while (pMorphTarget)
			{
				trace = 0;
				TiXmlElement* pTraceVertex = TiXmlHandle(pMorphTarget).FirstChildElement("vertexArray").FirstChildElement("vertex").Element();
				while (pTraceVertex)
				{
					TArrayU8 streamData;
					LocalCollectStreamData(streamData, TiXmlHandle(pTraceVertex).FirstChildElement("streamArray").Element());

					out_vertexPool.AppendVertexCmpData(trace, streamData);
					trace += 1;

					pTraceVertex = pTraceVertex->NextSiblingElement("vertex");
				}
				pMorphTarget = pMorphTarget->NextSiblingElement("target");
			}
		}

		pTraceModifier = pTraceModifier->NextSiblingElement();
	}

	out_vertexPool.RemoveDuplicateVertex();

	return;
}

static void LocalCollectFaceData(
	TArrayFace& out_arrayFace,
	DscVertexPool& in_vertexPool,
	TiXmlElement* const in_sourceElement
	)
{

	TiXmlElement* pTraceFace = TiXmlHandle(in_sourceElement).FirstChildElement("faceArray").FirstChildElement("face").Element();
	while (pTraceFace)
	{
		const int index0 = TXml::GetTextInt(TiXmlHandle(pTraceFace).FirstChildElement("index0").Element());
		const int index1 = TXml::GetTextInt(TiXmlHandle(pTraceFace).FirstChildElement("index1").Element());
		const int index2 = TXml::GetTextInt(TiXmlHandle(pTraceFace).FirstChildElement("index2").Element());

		out_arrayFace.push_back(Face(
			in_vertexPool.GetNewIndex(index0),
			in_vertexPool.GetNewIndex(index1),
			in_vertexPool.GetNewIndex(index2)
			));

		pTraceFace = pTraceFace->NextSiblingElement("face");
	}
	return;
}

static void LocalAppendShort(
	TArrayU8& inout_array, 
	const unsigned short in_value
	)
{
	const unsigned char* const pTrace = (unsigned char*)&in_value;
	for (int index = 0; index < sizeof(unsigned short); ++index)
	{
		inout_array.push_back(pTrace[index]);
	}
	return;
}

static void LocalVertexArrayToVertexData(
	TArrayU8& out_vertexData, 
	int& out_vertexDataCount, 
	TArrayU32& out_arrayWritenVertexIndex,
	const DscVertexPool& in_vertexPool
	)
{
	out_vertexDataCount = in_vertexPool.GetNewCount();
	for (int index = 0; index < out_vertexDataCount; ++index)
	{
		const TArrayU8& arrayData = in_vertexPool.GetVertexStreamData(index);
		out_vertexData.insert(
			out_vertexData.end(), 
			arrayData.begin(),
			arrayData.end()
			);
		out_arrayWritenVertexIndex.push_back(index); //(*iterator)->GetKeyIndex());
	}
	return;
}

static void LocalConvertIndex(
	int& out_elementCount, 
	int& out_flag, 
	TArrayU8& out_arrayIndex, 
	const TArrayInt& in_arrayIndex
	)
{
	out_elementCount = (int)in_arrayIndex.size();
	if (out_elementCount < 255)
	{
		out_flag |= GMeshType::TFlag::TIndexU8;
		for (TArrayInt::const_iterator iterator = in_arrayIndex.begin(); iterator != in_arrayIndex.end(); ++iterator)
		{
			out_arrayIndex.push_back((unsigned char)(*iterator));
		}
	}
	else
	{
		out_flag |= GMeshType::TFlag::TIndexU16;
		for (TArrayInt::const_iterator iterator = in_arrayIndex.begin(); iterator != in_arrayIndex.end(); ++iterator)
		{
			LocalAppendShort(
				out_arrayIndex, 
				(unsigned short)(*iterator)
				);
		}
	}

	return;
}

static void LocalConvertVertexDataPoint(
	int& out_elementCount, 
	TArrayU8& out_vertexData, 
	int& out_vertexDataCount, 
	int& out_flag, 
	TArrayU8& out_arrayIndex,
	TArrayU32& out_arrayWritenVertexIndex,
	const DscVertexPool& in_vertexPool, 
	const bool in_useIndex
	)
{
	LocalVertexArrayToVertexData(out_vertexData, out_vertexDataCount, out_arrayWritenVertexIndex, in_vertexPool);

	if (in_useIndex)
	{
		TArrayInt arrayIndex;
		for (int index = 0; index < out_vertexDataCount; ++index)
		{
			arrayIndex.push_back(index);
		}
		LocalConvertIndex(out_elementCount, out_flag, out_arrayIndex, arrayIndex);
	}
	else
	{
		out_elementCount = out_vertexDataCount;
	}

	return;
}

static void LocalConvertVertexDataLineIndex(
	int& out_elementCount, 
	TArrayU8& out_vertexData, 
	int& out_vertexDataCount, 
	int& out_flag, 
	TArrayU8& out_arrayIndex, 
	TArrayU32& out_arrayWritenVertexIndex,
	const DscVertexPool& in_vertexPool, 
	const TArrayFace& in_arrayFace
	)
{
	LocalVertexArrayToVertexData(out_vertexData, out_vertexDataCount, out_arrayWritenVertexIndex, in_vertexPool);
	TArrayInt arrayIndex;
	TMapPairIntIntInt mapEdge;
	for (TArrayFace::const_iterator iterator = in_arrayFace.begin(); iterator != in_arrayFace.end(); ++iterator)
	{
		TPairIntInt edge0((*iterator).mIndex0, (*iterator).mIndex1);
		TPairIntInt edge0a((*iterator).mIndex1, (*iterator).mIndex0);
		TPairIntInt edge1((*iterator).mIndex1, (*iterator).mIndex2);
		TPairIntInt edge1a((*iterator).mIndex2, (*iterator).mIndex1);
		TPairIntInt edge2((*iterator).mIndex2, (*iterator).mIndex0);
		TPairIntInt edge2a((*iterator).mIndex0, (*iterator).mIndex2);

		if (mapEdge.find(edge0) == mapEdge.end())
		{
			mapEdge[edge0] = 0;
			mapEdge[edge0a] = 0;
			arrayIndex.push_back(edge0.first);
			arrayIndex.push_back(edge0.second);
		}
		if (mapEdge.find(edge1) == mapEdge.end())
		{
			mapEdge[edge1] = 0;
			mapEdge[edge1a] = 0;
			arrayIndex.push_back(edge1.first);
			arrayIndex.push_back(edge1.second);
		}
		if (mapEdge.find(edge2) == mapEdge.end())
		{
			mapEdge[edge2] = 0;
			mapEdge[edge2a] = 0;
			arrayIndex.push_back(edge2.first);
			arrayIndex.push_back(edge2.second);
		}
	}

	LocalConvertIndex(out_elementCount, out_flag, out_arrayIndex, arrayIndex);
	return;
}

static void LocalConvertVertexDataLine(
	int& out_elementCount, 
	TArrayU8& out_vertexData, 
	int& out_vertexDataCount, 
	TArrayU32& out_arrayWritenVertexIndex,
	const DscVertexPool& in_vertexPool, 
	const TArrayFace& in_arrayFace
	)
{
	TMapPairIntIntInt mapEdge;
	for (TArrayFace::const_iterator iterator = in_arrayFace.begin(); iterator != in_arrayFace.end(); ++iterator)
	{
		TPairIntInt edge0((*iterator).mIndex0, (*iterator).mIndex1);
		TPairIntInt edge0a((*iterator).mIndex1, (*iterator).mIndex0);
		TPairIntInt edge1((*iterator).mIndex1, (*iterator).mIndex2);
		TPairIntInt edge1a((*iterator).mIndex2, (*iterator).mIndex1);
		TPairIntInt edge2((*iterator).mIndex2, (*iterator).mIndex0);
		TPairIntInt edge2a((*iterator).mIndex0, (*iterator).mIndex2);

		if (mapEdge.find(edge0) == mapEdge.end())
		{
			mapEdge[edge0] = 0;
			mapEdge[edge0a] = 0;

			const TArrayU8& vertex0 = in_vertexPool.GetVertexStreamData(edge0.first);
			const TArrayU8& vertex1 = in_vertexPool.GetVertexStreamData(edge0.second);
			out_vertexData.insert(out_vertexData.end(), vertex0.begin(), vertex0.end());
			out_arrayWritenVertexIndex.push_back(edge0.first);
			out_vertexData.insert(out_vertexData.end(), vertex1.begin(), vertex1.end());
			out_arrayWritenVertexIndex.push_back(edge0.second);

			out_vertexDataCount += 2;
			out_elementCount += 2;
		}
		if (mapEdge.find(edge1) == mapEdge.end())
		{
			mapEdge[edge1] = 0;
			mapEdge[edge1a] = 0;

			const TArrayU8& vertex0 = in_vertexPool.GetVertexStreamData(edge1.first);
			const TArrayU8& vertex1 = in_vertexPool.GetVertexStreamData(edge1.second);
			out_vertexData.insert(out_vertexData.end(), vertex0.begin(), vertex0.end());
			out_arrayWritenVertexIndex.push_back(edge1.first);
			out_vertexData.insert(out_vertexData.end(), vertex1.begin(), vertex1.end());
			out_arrayWritenVertexIndex.push_back(edge1.second);

			out_vertexDataCount += 2;
			out_elementCount += 2;
		}
		if (mapEdge.find(edge2) == mapEdge.end())
		{
			mapEdge[edge2] = 0;
			mapEdge[edge2a] = 0;

			const TArrayU8& vertex0 = in_vertexPool.GetVertexStreamData(edge2.first);
			const TArrayU8& vertex1 = in_vertexPool.GetVertexStreamData(edge2.second);
			out_vertexData.insert(out_vertexData.end(), vertex0.begin(), vertex0.end());
			out_arrayWritenVertexIndex.push_back(edge2.first);
			out_vertexData.insert(out_vertexData.end(), vertex1.begin(), vertex1.end());
			out_arrayWritenVertexIndex.push_back(edge2.second);

			out_vertexDataCount += 2;
			out_elementCount += 2;
		}
	}
	return;
}

static void LocalConvertVertexDataTriangleIndex(
	int& out_elementCount, 
	TArrayU8& out_vertexData, 
	int& out_vertexDataCount, 
	int& out_flag, 
	TArrayU8& out_arrayIndex, 
	TArrayU32& out_arrayWritenVertexIndex,
	const DscVertexPool& in_vertexPool, 
	const TArrayFace& in_arrayFace
	)
{
	LocalVertexArrayToVertexData(out_vertexData, out_vertexDataCount, out_arrayWritenVertexIndex, in_vertexPool);
	TArrayInt arrayIndex;
	for (TArrayFace::const_iterator iterator = in_arrayFace.begin(); iterator != in_arrayFace.end(); ++iterator)
	{
		arrayIndex.push_back((*iterator).mIndex0);
		arrayIndex.push_back((*iterator).mIndex1);
		arrayIndex.push_back((*iterator).mIndex2);
	}

	LocalConvertIndex(out_elementCount, out_flag, out_arrayIndex, arrayIndex);
	return;
}

static void LocalConvertVertexDataTriangle(
	int& out_elementCount, 
	TArrayU8& out_vertexData, 
	int& out_vertexDataCount, 
	TArrayU32& out_arrayWritenVertexIndex,
	const DscVertexPool& in_vertexPool, 
	const TArrayFace& in_arrayFace
	)
{
	for (TArrayFace::const_iterator iterator = in_arrayFace.begin(); iterator != in_arrayFace.end(); ++iterator)
	{
		const TArrayU8& vertex0 = in_vertexPool.GetVertexStreamData((*iterator).mIndex0);
		const TArrayU8& vertex1 = in_vertexPool.GetVertexStreamData((*iterator).mIndex1);
		const TArrayU8& vertex2 = in_vertexPool.GetVertexStreamData((*iterator).mIndex2);

		out_vertexData.insert(out_vertexData.end(), vertex0.begin(), vertex0.end());
		out_arrayWritenVertexIndex.push_back((*iterator).mIndex0);
		out_vertexData.insert(out_vertexData.end(), vertex1.begin(), vertex1.end());
		out_arrayWritenVertexIndex.push_back((*iterator).mIndex1);
		out_vertexData.insert(out_vertexData.end(), vertex2.begin(), vertex2.end());
		out_arrayWritenVertexIndex.push_back((*iterator).mIndex2);

		out_elementCount += 3;
		out_vertexDataCount += 3;
	}

	return;
}

static void LocalConvertVertexDataTriangleStripIndex(
	int& out_elementCount, 
	TArrayU8& out_vertexData, 
	int& out_vertexDataCount, 
	int& out_flag, 
	TArrayU8& out_arrayIndex, 
	TArrayU32& out_arrayWritenVertexIndex,
	const DscVertexPool& in_vertexPool, 
	const TArrayFace& in_arrayFace
	)
{
	SetCacheSize(sCacheSize);
	const int numIndices = in_arrayFace.size() * 3;
	unsigned short* indices = new unsigned short[numIndices];

	int trace = 0;
	for (TArrayFace::const_iterator iterator = in_arrayFace.begin(); iterator != in_arrayFace.end(); ++iterator)
	{
		indices[trace] = (*iterator).mIndex0;
		trace += 1;
		indices[trace] = (*iterator).mIndex1;
		trace += 1;
		indices[trace] = (*iterator).mIndex2;
		trace += 1;
	}
	PrimitiveGroup* primGroups = 0;
	unsigned short numGroups = 0;
	GenerateStrips(
		indices, 
		numIndices,
		&primGroups,
		&numGroups
		);

	LocalVertexArrayToVertexData(out_vertexData, out_vertexDataCount, out_arrayWritenVertexIndex, in_vertexPool);

	TArrayInt arrayIndex;
	for (int index = 0; index < (int)numGroups; ++index)
	{
		if (PT_STRIP != primGroups[index].type)
		{
			continue;
		}
		for (int subIndex = 0; subIndex < (int)primGroups[index].numIndices; ++subIndex)
		{
			arrayIndex.push_back(primGroups[index].indices[subIndex]);
		}
	}
	LocalConvertIndex(out_elementCount, out_flag, out_arrayIndex, arrayIndex);

	delete [] indices;
	delete [] primGroups;

	return;
}

static void LocalConvertVertexDataTriangleStrip(
	int& out_elementCount, 
	TArrayU8& out_vertexData, 
	int& out_vertexDataCount, 
	TArrayU32& out_arrayWritenVertexIndex,
	const DscVertexPool& in_vertexPool, 
	const TArrayFace& in_arrayFace
	)
{
	SetCacheSize(sCacheSize);
	const int numIndices = in_arrayFace.size() * 3;
	unsigned short* indices = new unsigned short[numIndices];

	int trace = 0;
	for (TArrayFace::const_iterator iterator = in_arrayFace.begin(); iterator != in_arrayFace.end(); ++iterator)
	{
		indices[trace] = (*iterator).mIndex0;
		trace += 1;
		indices[trace] = (*iterator).mIndex1;
		trace += 1;
		indices[trace] = (*iterator).mIndex2;
		trace += 1;
	}
	PrimitiveGroup* primGroups = 0;
	unsigned short numGroups = 0;
	GenerateStrips(
		indices, 
		numIndices,
		&primGroups,
		&numGroups
		);

	for (int index = 0; index < (int)numGroups; ++index)
	{
		if (PT_STRIP != primGroups[index].type)
		{
			continue;
		}
		for (int subIndex = 0; subIndex < (int)primGroups[index].numIndices; ++subIndex)
		{
			const TArrayU8& vertex = in_vertexPool.GetVertexStreamData(primGroups[index].indices[subIndex]);
			out_vertexData.insert(out_vertexData.end(), vertex.begin(), vertex.end());
			out_arrayWritenVertexIndex.push_back(primGroups[index].indices[subIndex]);
			out_elementCount += 1;
			out_vertexDataCount += 1;
		}
	}

	delete [] indices;
	delete [] primGroups;

	return;
}

static const bool LocalConvertVertexData(
	int& out_elementCount, 
	TArrayU8& out_vertexData, 
	int& out_vertexDataCount, 
	int& out_flag, 
	TArrayU8& out_arrayIndex, 
	TArrayU32& out_arrayWritenVertexIndex,
	const DscVertexPool& in_vertexPool, 
	const TArrayFace& in_arrayFace, 
	const GMeshType::TPrimitiveType::TEnum in_primitiveType, 
	const bool in_useIndex
	)
{
	switch (in_primitiveType)
	{
	default:
		break;
	case GMeshType::TPrimitiveType::TPoint:
		LocalConvertVertexDataPoint(
			out_elementCount,
			out_vertexData,
			out_vertexDataCount,
			out_flag,
			out_arrayIndex,  
			out_arrayWritenVertexIndex,
			in_vertexPool, 
			in_useIndex
			);
		return true;
	case GMeshType::TPrimitiveType::TLine:
		if (in_useIndex)
		{
			LocalConvertVertexDataLineIndex(
				out_elementCount,
				out_vertexData,
				out_vertexDataCount,
				out_flag,
				out_arrayIndex,  
				out_arrayWritenVertexIndex,
				in_vertexPool,
				in_arrayFace
				);
		}
		else
		{
			LocalConvertVertexDataLine(
				out_elementCount,
				out_vertexData,
				out_vertexDataCount,
				out_arrayWritenVertexIndex,
				in_vertexPool,
				in_arrayFace
				);
		}
		return true;

	case GMeshType::TPrimitiveType::TLineStrip:
		break;
	case GMeshType::TPrimitiveType::TLineLoop:
		break;
	case GMeshType::TPrimitiveType::TTriangle:
		if (in_useIndex)
		{
			LocalConvertVertexDataTriangleIndex(
				out_elementCount,
				out_vertexData,
				out_vertexDataCount,
				out_flag,
				out_arrayIndex,  
				out_arrayWritenVertexIndex,
				in_vertexPool,
				in_arrayFace
				);
		}
		else
		{
			LocalConvertVertexDataTriangle(
				out_elementCount,
				out_vertexData,
				out_vertexDataCount,
				out_arrayWritenVertexIndex,
				in_vertexPool,
				in_arrayFace
				);
		}
		return true;
	case GMeshType::TPrimitiveType::TTriangleStrip:
		if (in_useIndex)
		{
			LocalConvertVertexDataTriangleStripIndex(
				out_elementCount,
				out_vertexData,
				out_vertexDataCount,
				out_flag,
				out_arrayIndex,  
				out_arrayWritenVertexIndex,
				in_vertexPool,
				in_arrayFace
				);
		}
		else
		{
			LocalConvertVertexDataTriangleStrip(
				out_elementCount,
				out_vertexData,
				out_vertexDataCount,
				out_arrayWritenVertexIndex,
				in_vertexPool,
				in_arrayFace
				);
		}
		return true;
	case GMeshType::TPrimitiveType::TTriangleFan:
		break;
	}
	return true;
}

//collect modifier
static void LocalCollectModifier(
	TArrayPointerModifier& out_arrayModifier, 
	const TArrayPointerStreamInfo& in_arrayStreamInfo,
	const DscVertexPool& in_vertexPool, //to get the key index
	const TArrayU32& in_arrayWritenVertexIndex, //location that vertex written at
	TiXmlElement* const in_sourceElement,
	TPackerMessage& in_packMessage
	)
{
	TiXmlElement* pTrace = TiXmlHandle(in_sourceElement).FirstChildElement("modifierArray").FirstChildElement().Element();

	while (pTrace)
	{
		const std::string elementName = pTrace->Value();

		if (elementName == "modifierSkin")
		{
			TPointerModifier pointer = ModifierSkin::Factory(*pTrace, in_arrayStreamInfo, in_vertexPool, in_arrayWritenVertexIndex, in_packMessage);
			if (pointer)
			{
				out_arrayModifier.push_back(pointer);
			}
		}
		else if (elementName == "modifierCloth")
		{
			TPointerModifier pointer = ModifierCloth::Factory(*pTrace, in_arrayStreamInfo, in_vertexPool, in_arrayWritenVertexIndex, in_packMessage);
			if (pointer)
			{
				out_arrayModifier.push_back(pointer);
			}
		}
		else if (elementName == "modifierMorph")
		{
			TPointerModifier pointer = ModifierMorph::Factory(*pTrace, in_arrayStreamInfo, in_vertexPool, in_arrayWritenVertexIndex, in_packMessage);
			if (pointer)
			{
				out_arrayModifier.push_back(pointer);
			}
		}

		pTrace = pTrace->NextSiblingElement();
	}

	return;
}

static void LocalWriteOutIndexArray(
	TiXmlElement* const in_destinationElement, 
	const TArrayU8& in_arrayIndex
	)
{
	TiXmlElement pointerElement("pointer");
	pointerElement.SetAttribute("alignment", 4);
#ifdef WRITE_ARRAY
	TXmlPack::PutArrayU8(&pointerElement, in_arrayIndex);
#else
	for (TArrayU8::const_iterator iterator = in_arrayIndex.begin(); iterator != in_arrayIndex.end(); ++iterator)
	{
		TXmlPack::PutU8(&pointerElement, (*iterator));
	}
#endif

	in_destinationElement->InsertEndChild(pointerElement);

	return;
}

static void LocalWriteOutVertexData(
	TiXmlElement* const in_destinationElement, 
	const TArrayU8& in_vertexData
	)
{
	TiXmlElement pointerElement("pointer");
	pointerElement.SetAttribute("alignment", 4);

#ifdef WRITE_ARRAY
	TXmlPack::PutArrayU8(&pointerElement, in_vertexData);
#else
	for (TArrayU8::const_iterator iterator = in_vertexData.begin(); iterator != in_vertexData.end(); iterator += 4)
	{
		float value = 0.0F;
		unsigned char* const pTrace = (unsigned char* const)&value;
		pTrace[0] = *(iterator);
		pTrace[1] = *(iterator + 1);
		pTrace[2] = *(iterator + 2);
		pTrace[3] = *(iterator + 3);

		TXmlPack::PutR32(&pointerElement, value);
	}
#endif

	in_destinationElement->InsertEndChild(pointerElement);

	return;
}

static void LocalWriteOutStreamInfo(
	TiXmlElement* const in_destinationElement, 
	const TArrayPointerStreamInfo& in_arrayStreamInfo
	)
{
	TXmlPack::PutU32(in_destinationElement, in_arrayStreamInfo.size());
	TiXmlElement pointerElement("pointer");
	pointerElement.SetAttribute("alignment", 4);
	for (TArrayPointerStreamInfo::const_iterator iterator = in_arrayStreamInfo.begin(); iterator != in_arrayStreamInfo.end(); ++iterator)
	{
		TXmlPack::PutU32(&pointerElement, (*iterator)->GetType());
		TXmlPack::PutU32(&pointerElement, (*iterator)->GetCount());
		TXmlPack::PutU32(&pointerElement, (*iterator)->GetUsage());
		TXmlPack::PutU32(&pointerElement, (*iterator)->GetVertexDataByteOffset());
	}
	in_destinationElement->InsertEndChild(pointerElement);

	return;
}

static void LocalWriteOutModifier(
	TiXmlElement* const in_destinationElement, 
	const TArrayPointerModifier& in_arrayModifier
	)
{
	TXmlPack::PutU32(in_destinationElement, in_arrayModifier.size());
	TiXmlElement pointerElement("pointer");
	pointerElement.SetAttribute("alignment", 4);
	for (TArrayPointerModifier::const_iterator iterator = in_arrayModifier.begin(); iterator != in_arrayModifier.end(); ++iterator)
	{
		(*iterator)->WriteOutData(pointerElement);
	}
	in_destinationElement->InsertEndChild(pointerElement);

	return;
}


// static public methods
/*static*/ const bool PackGeometry::Pack(
	TiXmlElement* const in_sourceElement,
	TiXmlElement* const in_destinationElement,
	TPackerMessage& in_packMessage
	)
{
	if ((NULL == in_sourceElement) || (NULL == in_destinationElement))
	{
		return false;
	}

	//gather data
	const GMeshType::TPrimitiveType::TEnum primitiveType = LocalTranslatePrimitiveType(TXml::GetTextString(TiXmlHandle(in_sourceElement).FirstChildElement("primitiveType").Element()), in_packMessage);
	const bool useIndex = TXml::GetTextBool(TiXmlHandle(in_sourceElement).FirstChildElement("useIndex").Element());
	const bool staticEdgeList = TXml::GetTextBool(TiXmlHandle(in_sourceElement).FirstChildElement("staticEdgeList").Element());
	const bool dynamicEdgeList = TXml::GetTextBool(TiXmlHandle(in_sourceElement).FirstChildElement("dynamicEdgeList").Element());
	int elementCount = 0;
	int flag = 0;
	TArrayU8 arrayIndex;
	TArrayU8 vertexData;
	TArrayU32 arrayWritenVertexIndex;
	int vertexDataCount = 0;
	int vertexDataStride = 0;
	TArrayPointerStreamInfo arrayStreamInfo;
	TArrayPointerModifier arrayModifier;
	TArrayFace arrayFace;
	DscVertexPool vertexPool;
	const std::string skeletonName = TXml::GetTextString(TiXmlHandle(in_sourceElement).FirstChildElement("skeleton").Element());

	//collect stream data
	LocalGetStreamInfoArray(arrayStreamInfo, vertexDataStride, in_sourceElement, in_packMessage);

	//collect vertex data
	LocalCollectVertexData(vertexPool, in_sourceElement);

	//collect face data
	LocalCollectFaceData(arrayFace, vertexPool, in_sourceElement); //not, the face index is NOT the key index, it is the index of the raw data in the vertex pool

	//transform into topology
	if (!LocalConvertVertexData(elementCount, vertexData, vertexDataCount, flag, arrayIndex, arrayWritenVertexIndex, vertexPool, arrayFace, primitiveType, useIndex))
	{
		in_packMessage.AddError("primitive type not supported");
	}
	else
	{
		in_packMessage.AddMessage(std::string("packed geometry elementCount:") + TCommon::IntToString(elementCount) + std::string(" vertexCount:") + TCommon::IntToString(vertexDataCount));
	}

	Edge edge;
	if (staticEdgeList || dynamicEdgeList)
	{
		Edge::CollectData(edge, arrayFace, vertexPool, arrayWritenVertexIndex, arrayStreamInfo, staticEdgeList, dynamicEdgeList);
	}

	//collect modifier
	LocalCollectModifier(arrayModifier, arrayStreamInfo, vertexPool, arrayWritenVertexIndex, in_sourceElement, in_packMessage);

	//write out data
	TXmlPack::PutU32(in_destinationElement, primitiveType);
	TXmlPack::PutU32(in_destinationElement, elementCount);
	TXmlPack::PutU32(in_destinationElement, flag);
	LocalWriteOutIndexArray(in_destinationElement, arrayIndex);
	TXmlPack::PutU32(in_destinationElement, vertexDataCount);
	TXmlPack::PutU32(in_destinationElement, vertexDataStride);
	LocalWriteOutVertexData(in_destinationElement, vertexData);
	LocalWriteOutStreamInfo(in_destinationElement, arrayStreamInfo);
	if (staticEdgeList || dynamicEdgeList)
	{
		Edge::WriteOutEdge(in_destinationElement, edge);
	}
	else
	{
		TXmlPack::PutU32(in_destinationElement, 0);
	}

	LocalWriteOutModifier(in_destinationElement, arrayModifier);
	TXmlPack::PutString(in_destinationElement, skeletonName);

	return true;
}