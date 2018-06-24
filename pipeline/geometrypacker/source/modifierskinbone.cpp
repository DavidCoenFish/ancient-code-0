//file: ModifierSkinBone.cpp

#include "ModifierSkinBone.h"

#include "DscVertexPool.h"
#include "VertexData.h"
#include "StreamInfo.h"

#include <TXml.h>
#include <TXmlPack.h>
#include <TPackerMessage.h>
#include <tinyxml.h>
#include <map>

typedef std::pair<int, float> TPairIntFloat;
typedef std::vector<TPairIntFloat> TArrayPairIntFloat;
typedef boost::shared_ptr<ModifierSkinBone> TPointerModifierSkinBone;
typedef std::vector<ModifierSkinBoneVertexData> TArrayModifierSkinBoneVertexData;

typedef std::vector<int> TArrayInt;
typedef std::vector<unsigned char> TArrayU8;

typedef boost::shared_ptr<StreamInfo> TPointerStreamInfo;
typedef std::vector<TPointerStreamInfo> TArrayPointerStreamInfo;

typedef std::map<VertexData, TArrayInt> TMapVertexDataArrayInt;

typedef std::vector<unsigned int> TArrayU32;
typedef std::map<int, TArrayInt> TMapIntArrayInt;
//typedef std::map<Vertex, TArrayInt> TMapVertexArrayInt;
typedef std::map<int, int> TMapIntInt;

/*
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
*/

static void LocalAddSortedUnique(TArrayInt& inout_arrayInt, const int in_value)
{	
	TMapIntInt mapIntInt;
	for (TArrayInt::const_iterator iterator = inout_arrayInt.begin(); iterator != inout_arrayInt.end(); ++iterator)
	{
		mapIntInt.insert(TMapIntInt::value_type(*iterator, 0));
	}

	mapIntInt.insert(TMapIntInt::value_type(in_value, 0));

	inout_arrayInt.clear();
	for (TMapIntInt::const_iterator iterator = mapIntInt.begin(); iterator != mapIntInt.end(); ++iterator)
	{
		inout_arrayInt.push_back((*iterator).first);
	}
	return;
}

static void LocalGetMatrixXml(float* const inout_matrix, TiXmlElement* const in_parent)
{
	TiXmlElement* pMatrixTrace = TiXmlHandle(in_parent).FirstChildElement("float").Element();
	float value = 0.0F;
	for (int index = 0; index < 16; ++index)
	{
		if (pMatrixTrace)
		{
			value = TXml::GetTextFloat(pMatrixTrace);
		}
		else
		{
			value = 0.0F;
			if ((0 == index) ||
				(5 == index) ||
				(10 == index) ||
				(15 == index))
			{
				value = 1.0F;
			}
		}
		inout_matrix[index] = value;
		if (pMatrixTrace)
		{
			pMatrixTrace = pMatrixTrace->NextSiblingElement("float");
		}
	}

	return;
}

static void LocalGetKeyIndexWeight(TArrayPairIntFloat& out_arrayKeyIndexWeight, TiXmlElement* const in_parent)
{
	TiXmlElement* pIndexTrace = TiXmlHandle(in_parent).FirstChildElement("vertexKeyIndex").FirstChildElement("index").Element();
	TiXmlElement* pWeightTrace = TiXmlHandle(in_parent).FirstChildElement("vertexKeyWeight").FirstChildElement("weight").Element();

	while (pIndexTrace)
	{
		const int index = TXml::GetTextInt(pIndexTrace);
		const float weight = TXml::GetTextFloat(pWeightTrace);

		if (0.0001F < weight)
		{
			out_arrayKeyIndexWeight.push_back(TPairIntFloat(index, weight));
		}

		pIndexTrace = pIndexTrace->NextSiblingElement("index");
		if (pWeightTrace)
		{
			pWeightTrace = pWeightTrace->NextSiblingElement("weight");
		}
	}

	return;
}

static void LocalPullOutVertexPosInfo(
	TArrayModifierSkinBoneVertexData& out_arrayModifierSkinBoneVertexDataPos, 
	TArrayPairIntFloat& in_arrayKeyIndexWeight, 
	const DscVertexPool& in_vertexPool,
	const TArrayU32& in_arrayWritenVertexIndex,
	TMapIntArrayInt& inout_mapPointUsage,
	TPackerMessage& in_packMessage,
	const std::string& in_boneName
	)
{
	for (TArrayPairIntFloat::const_iterator iterator = in_arrayKeyIndexWeight.begin(); iterator != in_arrayKeyIndexWeight.end(); ++iterator)
	{
		const int keyIndex = (*iterator).first;
		//get the array of vertex with this key
		TArrayInt arrayVertexIndex;
		for (int index = 0; index < in_vertexPool.GetNewCount(); ++index)
		{
			if (keyIndex != in_vertexPool.GetKeyIndex(index))
			{
				continue;
			}
			arrayVertexIndex.push_back(index);
		}

		if (0 == arrayVertexIndex.size())
		{
			//assert(false); //no vertex exisits for key used in skinning?
			std::string message = "isolated vertex found on bone:";
			in_packMessage.AddError(message + in_boneName);
			continue;
		}

		TArrayInt arrayWritenVertexIndex;
		for (int index = 0; index < (int)in_arrayWritenVertexIndex.size(); ++index)
		{
			const int vertexIndex = in_arrayWritenVertexIndex[index];
			for (TArrayInt::const_iterator subIterator = arrayVertexIndex.begin(); subIterator != arrayVertexIndex.end(); ++subIterator)
			{
				if (vertexIndex == (*subIterator))
				{
					arrayWritenVertexIndex.push_back(index);
				}
			}
		}

		if (0 == arrayWritenVertexIndex.size())
		{
			//assert(false); //no writen vertex exisits for vertex?
			continue;
		}

		//is it in the usage map
		const int firstWrittenIndex = arrayWritenVertexIndex[0];
		TMapIntArrayInt::const_iterator found = inout_mapPointUsage.find(firstWrittenIndex);
		if (found == inout_mapPointUsage.end())
		{
			inout_mapPointUsage[firstWrittenIndex] = TArrayInt(arrayWritenVertexIndex.begin() + 1, arrayWritenVertexIndex.end());
		}

		out_arrayModifierSkinBoneVertexDataPos.push_back(TArrayModifierSkinBoneVertexData::value_type(
			firstWrittenIndex,
			(*iterator).second
			));
	}

	return;
}

static void LocalCollectVertexData(
	VertexData& inout_vertexData, 
	const TArrayPointerStreamInfo& in_arrayStreamInfo, 
	const TArrayU8& in_vertex,
	const TArrayInt& in_arrayStreamIndexNormal
	)
{
	for (TArrayInt::const_iterator iterator = in_arrayStreamIndexNormal.begin(); iterator != in_arrayStreamIndexNormal.end(); ++iterator)
	{
		const StreamInfo& streamInfo = *in_arrayStreamInfo[*iterator];
		const int byteOffset = streamInfo.GetVertexDataByteOffset();
		const int stride = streamInfo.GetStride();
		//const TArrayU8& vertexBytes = in_vertex->GetArrayByte();
	
		for (int index = byteOffset; index < (byteOffset + stride ); ++index)
		{
			inout_vertexData.GetArrayByte().push_back(in_vertex[index]);
		}
	}

	return;
}

static void LocalPullOutVertexNormalInfo(
	TArrayModifierSkinBoneVertexData& out_arrayModifierSkinBoneVertexDataNormal, 
	TArrayPairIntFloat& in_arrayKeyIndexWeight, 
	const TArrayPointerStreamInfo& in_arrayStreamInfo, 
	const DscVertexPool& in_vertexPool,
	const TArrayU32& in_arrayWritenVertexIndex,
	const TArrayInt& in_arrayStreamIndexNormal,
	TMapIntArrayInt& inout_mapVectorUsage
	)
{
	for (TArrayPairIntFloat::const_iterator iterator = in_arrayKeyIndexWeight.begin(); iterator != in_arrayKeyIndexWeight.end(); ++iterator)
	{
		const int keyIndex = (*iterator).first;
		//get the array of vertex with this key
		TArrayInt arrayVertexIndex;
		for (int index = 0; index < in_vertexPool.GetNewCount(); ++index)
		{
			if (keyIndex != in_vertexPool.GetKeyIndex(index))
			{
				continue;
			}
			arrayVertexIndex.push_back(index);
		}

		if (0 == arrayVertexIndex.size())
		{
			assert(false); //no vertex exisits for key used in skinning?
			continue;
		}

		//make a map of the vertex data
		TMapVertexDataArrayInt mapVertexData;
		for (TArrayInt::const_iterator subIterator = arrayVertexIndex.begin(); subIterator != arrayVertexIndex.end(); ++subIterator)
		{
			const int vertexIndex = (*subIterator);
			VertexData vertexData;
			LocalCollectVertexData(vertexData, in_arrayStreamInfo, in_vertexPool.GetVertexStreamData(vertexIndex), in_arrayStreamIndexNormal);

			TMapVertexDataArrayInt::iterator found = mapVertexData.find(vertexData);
			if (found != mapVertexData.end())
			{
				(*found).second.push_back(vertexIndex);
			}
			else
			{
				TArrayInt arrayData;
				arrayData.push_back(vertexIndex);
				mapVertexData[vertexData] = arrayData;
			}
		}

		//for each vertex data in map, add a skin vertex target, and usage info
		for (TMapVertexDataArrayInt::const_iterator subIterator = mapVertexData.begin(); subIterator != mapVertexData.end(); ++subIterator)
		{
			TArrayInt arrayWritenVertexIndex;
			const TArrayInt& arrayVertexIndex = (*subIterator).second;

			for (int index = 0; index < (int)in_arrayWritenVertexIndex.size(); ++index)
			{
				const int vertexIndex = in_arrayWritenVertexIndex[index];
				for (TArrayInt::const_iterator subIterator = arrayVertexIndex.begin(); subIterator != arrayVertexIndex.end(); ++subIterator)
				{
					if (vertexIndex == (*subIterator))
					{
						arrayWritenVertexIndex.push_back(index);
					}
				}
			}

			if (0 == arrayWritenVertexIndex.size())
			{
				assert(false); //no writen vertex exisits for vertex?
				continue;
			}

			//is it in the usage map
			const int firstWrittenIndex = arrayWritenVertexIndex[0];
			TMapIntArrayInt::const_iterator found = inout_mapVectorUsage.find(firstWrittenIndex);
			if (found == inout_mapVectorUsage.end())
			{
				inout_mapVectorUsage[firstWrittenIndex] = TArrayInt(arrayWritenVertexIndex.begin() + 1, arrayWritenVertexIndex.end());
			}

			out_arrayModifierSkinBoneVertexDataNormal.push_back(TArrayModifierSkinBoneVertexData::value_type(
				firstWrittenIndex,
				(*iterator).second
				));
		}
	}

	return;
}

//static methods
/*static*/ ModifierSkinBone::TPointerModifierSkinBone ModifierSkinBone::Factory(
	TiXmlElement& in_sourceElement,
	const TArrayPointerStreamInfo& in_arrayStreamInfo, 
	const DscVertexPool& in_vertexPool,
	const TArrayU32& in_arrayWritenVertexIndex,
	const TArrayInt& in_arrayStreamIndexPosition,
	const TArrayInt& in_arrayStreamIndexNormal,
	TMapIntArrayInt& inout_mapPointUsage,
	TMapIntArrayInt& inout_mapVectorUsage,
	TPackerMessage& in_packMessage
	)
{
	TPointerModifierSkinBone pointer;

	const std::string name = TXml::GetTextString(TiXmlHandle(&in_sourceElement).FirstChildElement("name").Element());
	//const int parentIndex = TXml::GetTextInt(TiXmlHandle(&in_sourceElement).FirstChildElement("parentIndex").Element());
	//float parentRelativeMatrix[16];
	//LocalGetMatrixXml(parentRelativeMatrix, TiXmlHandle(&in_sourceElement).FirstChildElement("parentRelativeMatrix").Element());
	float invertBindMatrix[16];
	LocalGetMatrixXml(invertBindMatrix, TiXmlHandle(&in_sourceElement).FirstChildElement("invertBindMatrix").Element());

	TArrayPairIntFloat arrayKeyIndexWeight;
	LocalGetKeyIndexWeight(arrayKeyIndexWeight, &in_sourceElement);

	TArrayModifierSkinBoneVertexData arrayModifierSkinBoneVertexDataPos;
	if (0 != in_arrayStreamIndexPosition.size())
	{
		LocalPullOutVertexPosInfo(
			arrayModifierSkinBoneVertexDataPos, 
			arrayKeyIndexWeight, 
			in_vertexPool, 
			in_arrayWritenVertexIndex, 
			inout_mapPointUsage,
			in_packMessage, 
			name
			);
	}

	TArrayModifierSkinBoneVertexData arrayModifierSkinBoneVertexDataNormal;
	if (0 != in_arrayStreamIndexNormal.size())
	{
		LocalPullOutVertexNormalInfo(
			arrayModifierSkinBoneVertexDataNormal,
			arrayKeyIndexWeight, 
			in_arrayStreamInfo,
			in_vertexPool,
			in_arrayWritenVertexIndex,
			in_arrayStreamIndexNormal,
			inout_mapVectorUsage
			);
	}

	pointer.reset(new ModifierSkinBone(
		name,
		//parentIndex,
		//parentRelativeMatrix,
		invertBindMatrix,
		arrayModifierSkinBoneVertexDataPos,
		arrayModifierSkinBoneVertexDataNormal
		));

	return pointer;
}

//constructor
ModifierSkinBone::ModifierSkinBone(
	const std::string& in_name,
	const float* const in_invertBindMatrix,
	const TArrayModifierSkinBoneVertexData& in_arrayModifierSkinBoneVertexDataPos,
	const TArrayModifierSkinBoneVertexData& in_arrayModifierSkinBoneVertexDataNormal
	)
	: mName(in_name)
	//, mInvertBindMatrix()
	, mArrayModifierSkinBoneVertexDataPos(in_arrayModifierSkinBoneVertexDataPos)
	, mArrayModifierSkinBoneVertexDataNormal(in_arrayModifierSkinBoneVertexDataNormal)
{
	for (int index = 0; index < 16; ++index)
	{
		mInvertBindMatrix[index] = in_invertBindMatrix[index];
	}
	return;
}

ModifierSkinBone::~ModifierSkinBone()
{
	return;
}

/*
class GRenderMeshModifierSkinLoad
	unsigned int 
*/
//public methods
void ModifierSkinBone::WriteOutData(
	TiXmlElement& inout_parentElement
	)const
{
	//const char* const mBoneName;
	TXmlPack::PutString(&inout_parentElement, mName);

	//const GMatrix16Float mInvertBindMatrix;
	for (int index = 0; index < 16; ++index)
	{
		TXmlPack::PutR32(&inout_parentElement, mInvertBindMatrix[index]);
	}

	//const unsigned int mPointArrayVertexDataLoadCount;
	TXmlPack::PutU32(&inout_parentElement, mArrayModifierSkinBoneVertexDataPos.size());
	//const GRenderMeshModifierSkinBoneVertexDataLoad* const mPointArrayVertexDataLoad;
	{
		TiXmlElement arrayElement("pointer");
		arrayElement.SetAttribute("alignment", 4);
		for (TArrayModifierSkinBoneVertexData::const_iterator iterator = mArrayModifierSkinBoneVertexDataPos.begin(); iterator != mArrayModifierSkinBoneVertexDataPos.end(); ++iterator)
		{
			//const unsigned int mVertexTargetIndex;
			TXmlPack::PutU32(&arrayElement, (*iterator).VertexIndex());			
			//const float mWeight;
			TXmlPack::PutR32(&arrayElement, (*iterator).VertexWeight());			
		}
		inout_parentElement.InsertEndChild(arrayElement);
	}

	//const unsigned int mVectorArrayVertexDataLoadCount;
	TXmlPack::PutU32(&inout_parentElement, mArrayModifierSkinBoneVertexDataNormal.size());
	//const GRenderMeshModifierSkinBoneVertexDataLoad* const mVectorArrayVertexDataLoad;
	{
		TiXmlElement arrayElement("pointer");
		arrayElement.SetAttribute("alignment", 4);
		for (TArrayModifierSkinBoneVertexData::const_iterator iterator = mArrayModifierSkinBoneVertexDataNormal.begin(); iterator != mArrayModifierSkinBoneVertexDataNormal.end(); ++iterator)
		{
			//const unsigned int mVertexTargetIndex;
			TXmlPack::PutU32(&arrayElement, (*iterator).VertexIndex());			
			//const float mWeight;
			TXmlPack::PutR32(&arrayElement, (*iterator).VertexWeight());			
		}
		inout_parentElement.InsertEndChild(arrayElement);
	}

	return;
}
