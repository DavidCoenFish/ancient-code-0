//file: ModifierSkin.cpp

#include "ModifierSkin.h"

#include "ModifierSkinBone.h"
#include "DscVertexPool.h"
#include "StreamInfo.h"

#include <map>
#include <TCommon.h>
#include <GMeshModifier.h>
#include <TXml.h>
#include <TXmlPack.h>
#include <tinyxml.h>

/*
class GRenderMeshModifierSkinLoad
	const unsigned int mArrayBoneCount;
	const GRenderMeshModifierSkinBoneLoad* const mArrayBone;

	const unsigned int mArrayTargetStreamCount;
	const GRenderMeshModifierSkinTargetStreamLoad* const mArrayTargetStreamLoad;

	const unsigned int mPointArrayCount;
	const unsigned int* const mPointArrayVertexIndex; //length ArrayCount
	const unsigned int* const mPointArrayDuplicateOffset;//length ArrayCount
	const unsigned int* const mPointArrayDuplicate; //length whatever to hold data for mArrayDuplicateOffset [count, [index]]

	const unsigned int mVectorArrayCount;
	const unsigned int* const mVectorArrayVertexIndex; //length ArrayCount
	const unsigned int* const mVectorArrayDuplicateOffset;//length ArrayCount
	const unsigned int* const mVectorArrayDuplicate; //length whatever to hold data for mArrayDuplicateOffset [count, [index]]

class GRenderMeshModifierSkinBoneLoad
	const char* const mBoneName;
	const unsigned int mParentIndex;
	const GMatrix16Float mParentRelativeMatrix;
	const GMatrix16Float mInvertBindMatrix;

	const unsigned int mPointArrayVertexDataLoadCount;
	const GRenderMeshModifierSkinBoneVertexDataLoad* const mPointArrayVertexDataLoad;

	const unsigned int mVectorArrayVertexDataLoadCount;
	const GRenderMeshModifierSkinBoneVertexDataLoad* const mVectorArrayVertexDataLoad;

class GRenderMeshModifierSkinBoneVertexDataLoad
	const unsigned int mVertexTargetIndex;
	const float mWeight;

class GRenderMeshModifierSkinTargetStreamLoad
	const unsigned int mStreamIndex;
	const unsigned int mFlag;

*/

typedef std::vector<int> TArrayInt;
typedef boost::shared_ptr<ModifierSkinBone> TPointerModifierSkinBone;
typedef std::vector<TPointerModifierSkinBone> TArrayPointerModifierSkinBone;
typedef std::pair<int, int> TPairIntInt;
typedef std::vector<TPairIntInt> TArrayPairIntInt;

typedef std::map<int, TArrayInt> TMapIntArrayInt;
//typedef std::map<Vertex, TArrayInt> TMapVertexArrayInt;

//change usage maps into arrays
static void LocalExtractPointUsage(TArrayPairIntInt& out_pointUsageArrayIndexOffset, TArrayInt& out_pointArrayDuplicate, const TMapIntArrayInt& in_mapPointUsage)
{
	for (TMapIntArrayInt::const_iterator iterator = in_mapPointUsage.begin(); iterator != in_mapPointUsage.end(); ++iterator)
	{
		int duplicateOffset = -1;
		const TArrayInt& arrayIndex = (*iterator).second;
		const int vertexIndex = (*iterator).first;
		const int duplicateCount = (int)arrayIndex.size();
		if (0 < duplicateCount)
		{
			duplicateOffset = (int)out_pointArrayDuplicate.size();
			out_pointArrayDuplicate.push_back(duplicateCount);
			for (int index = 0; index < (int)arrayIndex.size(); ++index)
			{
				out_pointArrayDuplicate.push_back(arrayIndex[index]);
			}
		}

		out_pointUsageArrayIndexOffset.push_back(TPairIntInt(vertexIndex, duplicateOffset));
	}
	return;
}

static void LocalWriteArrayBone(TiXmlElement* const in_parentElement, const TArrayPointerModifierSkinBone& in_arrayBone)
{

	TiXmlElement arrayElement("pointer");
	arrayElement.SetAttribute("alignment", 4);
	for (TArrayPointerModifierSkinBone::const_iterator iterator = in_arrayBone.begin(); iterator != in_arrayBone.end(); ++iterator)
	{
		(*iterator)->WriteOutData(arrayElement);
	}

	//const unsigned int mArrayBoneCount;
	TXmlPack::PutU32(in_parentElement, in_arrayBone.size());
	//const GRenderMeshModifierSkinBoneLoad* const mArrayBone;
	in_parentElement->InsertEndChild(arrayElement);

	return;
}

static void LocalWriteArrayTargetStream(TiXmlElement* const in_parentElement, const TArrayInt& in_arrayStreamIndexPos, const TArrayInt& in_arrayStreamIndexNormal)
{
	TiXmlElement arrayElement("pointer");
	arrayElement.SetAttribute("alignment", 4);

	for (TArrayInt::const_iterator iterator = in_arrayStreamIndexPos.begin(); iterator != in_arrayStreamIndexPos.end(); ++iterator)
	{
		TXmlPack::PutU32(&arrayElement, (*iterator));
		TXmlPack::PutU32(&arrayElement, 0);
	}

	for (TArrayInt::const_iterator iterator = in_arrayStreamIndexNormal.begin(); iterator != in_arrayStreamIndexNormal.end(); ++iterator)
	{
		TXmlPack::PutU32(&arrayElement, (*iterator));
		TXmlPack::PutU32(&arrayElement, 1);
	}

	//const unsigned int mArrayTargetStreamCount;
	TXmlPack::PutU32(in_parentElement, in_arrayStreamIndexPos.size() + in_arrayStreamIndexNormal.size());
	//const GRenderMeshModifierSkinTargetStreamLoad* const mArrayTargetStreamLoad;
	in_parentElement->InsertEndChild(arrayElement);

	return;
}

static void LocalWriteUsage(TiXmlElement* const in_parentElement, const TArrayPairIntInt& in_pointUsageArrayIndexOffset, const TArrayInt& in_pointArrayDuplicate)
{
	//const unsigned int mPointArrayCount;
	TXmlPack::PutU32(in_parentElement, in_pointUsageArrayIndexOffset.size());

	//const unsigned int* const mPointArrayVertexIndex; //length ArrayCount
	//const unsigned int* const mPointArrayDuplicateOffset;//length ArrayCount
	{
		TiXmlElement arrayElement("pointer");
		arrayElement.SetAttribute("alignment", 4);

		for (TArrayPairIntInt::const_iterator iterator = in_pointUsageArrayIndexOffset.begin(); iterator != in_pointUsageArrayIndexOffset.end(); ++iterator)
		{
			TXmlPack::PutU32(&arrayElement, (*iterator).first);
			TXmlPack::PutU32(&arrayElement, (*iterator).second);
		}

		in_parentElement->InsertEndChild(arrayElement);
	}

	//const unsigned int* const mPointArrayDuplicate; //length whatever to hold data for mArrayDuplicateOffset [count, [index]]
	{
		TiXmlElement arrayElement("pointer");
		arrayElement.SetAttribute("alignment", 4);

		for (TArrayInt::const_iterator iterator = in_pointArrayDuplicate.begin(); iterator != in_pointArrayDuplicate.end(); ++iterator)
		{
			TXmlPack::PutU32(&arrayElement, (*iterator));
		}

		in_parentElement->InsertEndChild(arrayElement);		
	}

	return;
}


//static methods
/*static*/ ModifierSkin::TPointerModifierSkin ModifierSkin::Factory(
	TiXmlElement& in_sourceElement,
	const TArrayPointerStreamInfo& in_arrayStreamInfo, 
	const DscVertexPool& in_vertexPool,
	const TArrayU32& in_arrayWritenVertexIndex,
	TPackerMessage& in_packMessage
	)
{
	TPointerModifierSkin pointer;

	TArrayInt arrayStreamIndexPos;
	TArrayInt arrayStreamIndexNormal;
	TArrayPointerModifierSkinBone arrayBone;
	TArrayPairIntInt pointUsageArrayIndexOffset;
	TArrayInt pointArrayDuplicate;
	TArrayPairIntInt vectorUsageArrayIndexOffset;
	TArrayInt vectorArrayDuplicate;

	TMapIntArrayInt mapPointUsage;
	TMapIntArrayInt mapVectorUsage;

	//stream info
	for (int index = 0; index < (int)in_arrayStreamInfo.size(); ++index)
	{
		if (in_arrayStreamInfo[index]->GetIsPosition())
		{
			arrayStreamIndexPos.push_back(index);
		}
		else if (in_arrayStreamInfo[index]->GetIsNormal())
		{
			arrayStreamIndexNormal.push_back(index);
		}
	}

	//bones
	TiXmlElement* pTrace = TiXmlHandle(&in_sourceElement).FirstChildElement("bone").Element();
	while (pTrace)
	{
		TPointerModifierSkinBone pointerBone = ModifierSkinBone::Factory(
			*pTrace, 
			in_arrayStreamInfo, 
			in_vertexPool, 
			in_arrayWritenVertexIndex,
			arrayStreamIndexPos, 
			arrayStreamIndexNormal,
			mapPointUsage,
			mapVectorUsage,
			in_packMessage
			);
		arrayBone.push_back(pointerBone);
		pTrace = pTrace->NextSiblingElement("bone");
	}

	//change usage maps into arrays
	LocalExtractPointUsage(pointUsageArrayIndexOffset, pointArrayDuplicate, mapPointUsage);
	LocalExtractPointUsage(vectorUsageArrayIndexOffset, vectorArrayDuplicate, mapVectorUsage);

	pointer.reset(new ModifierSkin(
		arrayStreamIndexPos,
		arrayStreamIndexNormal,
		arrayBone,
		pointUsageArrayIndexOffset,
		pointArrayDuplicate,
		vectorUsageArrayIndexOffset,
		vectorArrayDuplicate
		));

	return pointer;
}

ModifierSkin::ModifierSkin(
	const TArrayInt& in_arrayStreamIndexPos,
	const TArrayInt& in_arrayStreamIndexNormal,
	const TArrayPointerModifierSkinBone& in_arrayBone,
	const TArrayPairIntInt& in_pointUsageArrayIndexOffset,
	const TArrayInt& in_pointArrayDuplicate,
	const TArrayPairIntInt& in_vectorUsageArrayIndexOffset,
	const TArrayInt& in_vectorArrayDuplicate				   
	)
	: mArrayStreamIndexPos(in_arrayStreamIndexPos)
	, mArrayStreamIndexNormal(in_arrayStreamIndexNormal)
	, mArrayBone(in_arrayBone)
	, mPointUsageArrayIndexOffset(in_pointUsageArrayIndexOffset)
	, mPointArrayDuplicate(in_pointArrayDuplicate)
	, mVectorUsageArrayIndexOffset(in_vectorUsageArrayIndexOffset)
	, mVectorArrayDuplicate(in_vectorArrayDuplicate)
{
	return;
}

/*virtual*/ ModifierSkin::~ModifierSkin()
{
	return;
}

/*virtual*/ void ModifierSkin::OnWriteOutData(
	TiXmlElement& inout_parentElement
	)const
{
	TiXmlElement pointerElement("pointer");
	pointerElement.SetAttribute("alignment", 4);
	
	//const unsigned int mArrayBoneCount;
	//const GRenderMeshModifierSkinBoneLoad* const mArrayBone;
	LocalWriteArrayBone(&pointerElement, mArrayBone);

	//const unsigned int mArrayTargetStreamCount;
	//const GRenderMeshModifierSkinTargetStreamLoad* const mArrayTargetStreamLoad;
	LocalWriteArrayTargetStream(&pointerElement, mArrayStreamIndexPos, mArrayStreamIndexNormal);

	//const unsigned int mPointArrayCount;
	//const unsigned int* const mPointArrayVertexIndex; //length ArrayCount
	//const unsigned int* const mPointArrayDuplicateOffset;//length ArrayCount
	//const unsigned int* const mPointArrayDuplicate; //length whatever to hold data for mArrayDuplicateOffset [count, [index]]
	LocalWriteUsage(&pointerElement, mPointUsageArrayIndexOffset, mPointArrayDuplicate);

	//const unsigned int mVectorArrayCount;
	//const unsigned int* const mVectorArrayVertexIndex; //length ArrayCount
	//const unsigned int* const mVectorArrayDuplicateOffset;//length ArrayCount
	//const unsigned int* const mVectorArrayDuplicate; //length whatever to hold data for mArrayDuplicateOffset [count, [index]]
	LocalWriteUsage(&pointerElement, mVectorUsageArrayIndexOffset, mVectorArrayDuplicate);
;
	TXmlPack::PutU32(&inout_parentElement, (U32)GMeshModifier::TType::TSkin);
	inout_parentElement.InsertEndChild(pointerElement);

	return;
}

