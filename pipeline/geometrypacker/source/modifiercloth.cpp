//file: ModifierCloth.cpp

#include "ModifierCloth.h"

#include "ModifierClothSpringData.h"
#include "ModifierClothVertexData.h"
#include "DscVertexPool.h"
#include "StreamInfo.h"

#include <map>
#include <TCommon.h>
#include <GMeshModifier.h>
#include <TXml.h>
#include <TXmlPack.h>
#include <tinyxml.h>

typedef std::vector<unsigned int> TArrayU32;
typedef std::vector<int> TArrayInt;
typedef boost::shared_ptr<StreamInfo> TPointerStreamInfo;
typedef std::vector<TPointerStreamInfo> TArrayPointerStreamInfo;

typedef std::vector<ModifierClothSpringData> TArrayModifierClothSpringData;
typedef std::vector<ModifierClothVertexData> TArrayModifierClothVertexData;
typedef std::map<int, ModifierClothVertexData> TMapIntModifierClothVertexData;

/*
class GRenderMeshModifierClothLoad
	const float mGravity;
	const float mMass;
	const float mSpringConstant;
	const float mSpringDampen;
	const unsigned int mSpringDataArrayCount;
	const GRenderMeshModifierClothSpringDataLoad* const mSpringDataLoadArray;
	
	const unsigned int mVertexDataArrayCount;
	const GRenderMeshModifierClothVertexDataLoad* const mVertexDataLoadArray;

	const unsigned int* const mVectorArrayDuplicate; //length whatever to hold data for mVertexDataLoadArray [count, [index]]

	const unsigned int mStreamTargetArrayCount;
	const unsigned int* const mStreamTargetArray;

class GRenderMeshModifierClothSpringDataLoad
	const int mVertexIndexOne;
	const int mVertexIndexTwo;
	const float mLength;

class GRenderMeshModifierClothVertexDataLoad
	//const int mVertexIndex;
	const float mWeight;
	const int mDuplicateTableOffset;
	*/

//static const int LocalGetUsage(
//	TArrayInt& out_arrayDuplicate,
//	const TArrayU32& in_arrayWritenVertexIndex,
//	const DscVertexPool& in_vertexPool,
//	const int in_keyIndexTwo
//	)
//{
//	int result = -1;
//	for (int index = 0; index < (int)in_arrayWritenVertexIndex.size(); ++index)
//	{
//		if (in_keyIndexTwo != in_vertexPool.GetKeyIndex(in_arrayWritenVertexIndex[index]))
//		{
//			continue;
//		}
//
//		if (-1 == result)
//		{
//			result = index;
//		}
//		else
//		{
//			out_arrayDuplicate.push_back(index);
//		}
//	}
//	return result;
//}
//
//static const int LocalGetFirstUsage(
//	const TArrayU32& in_arrayWritenVertexIndex,
//	const DscVertexPool& in_vertexPool,
//	const int in_keyIndexTwo
//	)
//{
//	for (int index = 0; index < (int)in_arrayWritenVertexIndex.size(); ++index)
//	{
//		if (in_keyIndexTwo != in_vertexPool.GetKeyIndex(in_arrayWritenVertexIndex[index]))
//		{
//			continue;
//		}
//
//		return index;
//	}
//	return -1;
//}

static void LocalReadSpringData(TArrayModifierClothSpringData& out_arraySpringData, TiXmlElement& in_sourceElement, const TArrayU32& in_arrayWritenVertexIndex, const DscVertexPool& in_vertexPool)
{
	TiXmlElement* pTrace = TiXmlHandle(&in_sourceElement).FirstChildElement("springDataArray").FirstChildElement("springData").Element();
	while (pTrace)
	{
		const int keyIndexOne = TXml::GetTextInt(TiXmlHandle(pTrace).FirstChildElement("indexOne").Element());
		const int keyIndexTwo = TXml::GetTextInt(TiXmlHandle(pTrace).FirstChildElement("indexTwo").Element());
		const float length = TXml::GetTextFloat(TiXmlHandle(pTrace).FirstChildElement("length").Element());

		//const int vertexIndexOne = LocalGetFirstUsage(in_arrayWritenVertexIndex, in_arrayVertex, keyIndexOne);
		//const int vertexIndexTwo = LocalGetFirstUsage(in_arrayWritenVertexIndex, in_arrayVertex, keyIndexTwo);

		out_arraySpringData.push_back(ModifierClothSpringData(
			keyIndexOne,
			keyIndexTwo,
			length
			));
	
		pTrace = pTrace->NextSiblingElement("springData");
	}

	return;
}

static void LocalReadVertexData(TArrayModifierClothVertexData& out_arrayVertexData, TArrayInt& out_arrayDuplicate, TiXmlElement& in_sourceElement, const TArrayU32& in_arrayWritenVertexIndex, const DscVertexPool& in_vertexPool)
{
	TiXmlElement* pTrace = TiXmlHandle(&in_sourceElement).FirstChildElement("vertexWeightArray").FirstChildElement("weight").Element();
	//TMapIntModifierClothVertexData mapVertexData;
	int keyIndex = 0;
	while (pTrace)
	{
		const float weight = TXml::GetTextFloat(pTrace);
		//if (0.0F != weight) we have zero weight verts to anchor cloth to other geometry?
		{
			TArrayU32 arrayDuplicates;
			DscVertexPoolFindDuplicateKey(
				arrayDuplicates, 
				keyIndex, 
				in_arrayWritenVertexIndex, 
				in_vertexPool
				);

			int duplicateOffset = -1;
			if (0 != arrayDuplicates.size())
			{
				const int duplicateOffset = out_arrayDuplicate.size();
				out_arrayDuplicate.push_back(arrayDuplicates.size());
				out_arrayDuplicate.insert(out_arrayDuplicate.end(), arrayDuplicates.begin(), arrayDuplicates.end());

				out_arrayVertexData.push_back(ModifierClothVertexData(
					weight,
					duplicateOffset
					));
			}
		}
	
		keyIndex += 1;
		pTrace = pTrace->NextSiblingElement("weight");
	}

	return;
}

static void LocalGatherTargetStreams(TArrayInt& out_arrayStreamTarget, const TArrayPointerStreamInfo& in_arrayStreamInfo)
{
	for (int index = 0; index < (int)in_arrayStreamInfo.size(); ++index)
	{
		if (in_arrayStreamInfo[index]->GetIsPosition())
		{
			out_arrayStreamTarget.push_back(index);
		}
	}

	return;
}

//static methods
/*static*/ ModifierCloth::TPointerModifierCloth ModifierCloth::Factory(
	TiXmlElement& in_sourceElement,
	const TArrayPointerStreamInfo& in_arrayStreamInfo, 
	const DscVertexPool& in_vertexPool,
	const TArrayU32& in_arrayWritenVertexIndex,
	TPackerMessage& in_packMessage
	)
{
	TPointerModifierCloth pointer;

	const float gravity = TXml::GetTextFloat(TiXmlHandle(&in_sourceElement).FirstChildElement("gravity").Element());
	const float mass = TXml::GetTextFloat(TiXmlHandle(&in_sourceElement).FirstChildElement("mass").Element());
	const float springConstant = TXml::GetTextFloat(TiXmlHandle(&in_sourceElement).FirstChildElement("springConstant").Element());
	const float springDampen = TXml::GetTextFloat(TiXmlHandle(&in_sourceElement).FirstChildElement("springDampen").Element());
	TArrayModifierClothSpringData arraySpringData;
	TArrayModifierClothVertexData arrayVertexData;
	TArrayInt arrayDuplicate;
	TArrayInt arrayStreamTarget;

	LocalReadSpringData(arraySpringData, in_sourceElement, in_arrayWritenVertexIndex, in_vertexPool);
	LocalReadVertexData(arrayVertexData, arrayDuplicate, in_sourceElement, in_arrayWritenVertexIndex, in_vertexPool);
	LocalGatherTargetStreams(arrayStreamTarget, in_arrayStreamInfo);

	pointer.reset(new ModifierCloth(
		gravity,
		mass,
		springConstant,
		springDampen,
		arraySpringData,
		arrayVertexData,
		arrayDuplicate,
		arrayStreamTarget
		));

	return pointer;
}

ModifierCloth::ModifierCloth(
	const float in_gravity,
	const float in_mass,
	const float in_springConstant,
	const float in_springDampen,
	const TArrayModifierClothSpringData& in_arraySpringData,
	const TArrayModifierClothVertexData& in_arrayVertexData,
	const TArrayInt& in_arrayDuplicate,
	const TArrayInt& in_arrayStreamTarget
	)
	: mGravity(in_gravity)
	, mMass(in_mass)
	, mSpringConstant(in_springConstant)
	, mSpringDampen(in_springDampen)
	, mArraySpringData(in_arraySpringData)
	, mArrayVertexData(in_arrayVertexData)
	, mArrayDuplicate(in_arrayDuplicate)
	, mArrayStreamTarget(in_arrayStreamTarget)
{
	return;
}

/*virtual*/ ModifierCloth::~ModifierCloth()
{
	return;
}

/*virtual*/ void ModifierCloth::OnWriteOutData(
	TiXmlElement& inout_parentElement
	)const
{
	TiXmlElement pointerElement("pointer");
	pointerElement.SetAttribute("alignment", 4);
	
	//const float mGravity;
	TXmlPack::PutR32(&pointerElement, mGravity);
	//const float mMass;
	TXmlPack::PutR32(&pointerElement, mMass);
	//const float mSpringConstant;
	TXmlPack::PutR32(&pointerElement, mSpringConstant);
	//const float mSpringDampen;
	TXmlPack::PutR32(&pointerElement, mSpringDampen);

	//const unsigned int mSpringDataArrayCount;
	TXmlPack::PutU32(&pointerElement, mArraySpringData.size());
	//const GRenderMeshModifierClothSpringDataLoad* const mSpringDataLoadArray;
	{
		TiXmlElement pointerElementArraySpring("pointer");
		pointerElementArraySpring.SetAttribute("alignment", 4);

		for (TArrayModifierClothSpringData::const_iterator iterator = mArraySpringData.begin(); iterator != mArraySpringData.end(); ++iterator)
		{
			TXmlPack::PutU32(&pointerElementArraySpring, (*iterator).GetVertexIndexOne() );
			TXmlPack::PutU32(&pointerElementArraySpring, (*iterator).GetVertexIndexTwo() );
			TXmlPack::PutR32(&pointerElementArraySpring, (*iterator).GetLength() );
		}

		pointerElement.InsertEndChild(pointerElementArraySpring);
	}

	//const unsigned int mVertexDataArrayCount;
	TXmlPack::PutU32(&pointerElement, mArrayVertexData.size());
	//const GRenderMeshModifierClothVertexDataLoad* const mVertexDataLoadArray;
	{
		TiXmlElement pointerElementArrayVertex("pointer");
		pointerElementArrayVertex.SetAttribute("alignment", 4);

		for (TArrayModifierClothVertexData::const_iterator iterator = mArrayVertexData.begin(); iterator != mArrayVertexData.end(); ++iterator)
		{
			//TXmlPack::PutU32(&pointerElementArrayVertex, (*iterator).GetVertexIndex() );
			TXmlPack::PutR32(&pointerElementArrayVertex, (*iterator).GetVertexWeight() );
			TXmlPack::PutU32(&pointerElementArrayVertex, (*iterator).GetDuplicateTableOffset() );
		}

		pointerElement.InsertEndChild(pointerElementArrayVertex);
	}

	//const unsigned int* const mVectorArrayDuplicate; //length whatever to hold data for mVertexDataLoadArray [count, [index]]
	{
		TiXmlElement arrayElement("pointer");
		arrayElement.SetAttribute("alignment", 4);

		for (TArrayInt::const_iterator iterator = mArrayDuplicate.begin(); iterator != mArrayDuplicate.end(); ++iterator)
		{
			TXmlPack::PutU32(&arrayElement, (*iterator));
		}

		pointerElement.InsertEndChild(arrayElement);		
	}

	//const unsigned int mStreamTargetArrayCount;
	TXmlPack::PutU32(&pointerElement, mArrayStreamTarget.size());
	//const unsigned int* const mStreamTargetArray;
	{
		TiXmlElement arrayElement("pointer");
		arrayElement.SetAttribute("alignment", 4);

		for (TArrayInt::const_iterator iterator = mArrayStreamTarget.begin(); iterator != mArrayStreamTarget.end(); ++iterator)
		{
			TXmlPack::PutU32(&arrayElement, (*iterator));
		}

		pointerElement.InsertEndChild(arrayElement);		
	}

	TXmlPack::PutU32(&inout_parentElement, (U32)GMeshModifier::TType::TCloth);
	inout_parentElement.InsertEndChild(pointerElement);

	return;
}

