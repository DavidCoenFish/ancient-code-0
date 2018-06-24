//file: skeletonbone.cpp

#include "skeletonbone.h"

#include <TXml.h>
#include <TXmlPack.h>
#include <tinyxml.h>
#include <map>

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

//static methods
/*static*/ SkeletonBone::TPointerSkeletonBone SkeletonBone::Factory(
	TiXmlElement& in_sourceElement
	)
{
	TPointerSkeletonBone pointer;

	const std::string name = TXml::GetTextString(TiXmlHandle(&in_sourceElement).FirstChildElement("name").Element());
	const int parentIndex = TXml::GetTextInt(TiXmlHandle(&in_sourceElement).FirstChildElement("parentIndex").Element());
	float parentRelativeMatrix[16];
	LocalGetMatrixXml(parentRelativeMatrix, TiXmlHandle(&in_sourceElement).FirstChildElement("parentRelativeMatrix").Element());

	pointer.reset(new SkeletonBone(
		name,
		parentIndex,
		parentRelativeMatrix
		));

	return pointer;
}

//constructor
SkeletonBone::SkeletonBone(
	const std::string& in_name,
	const int in_parentIndex,
	const float* const in_parentRelativeMatrix
	)
	: mName(in_name)
	, mParentIndex(in_parentIndex)
	//, mParentRelativeMatrix()
{
	for (int index = 0; index < 16; ++index)
	{
		mParentRelativeMatrix[index] = in_parentRelativeMatrix[index];
	}
	return;
}

SkeletonBone::~SkeletonBone()
{
	return;
}

//public methods
void SkeletonBone::WriteOutData(
	TiXmlElement& inout_parentElement
	)const
{
	//const char* const mBoneName;
	TXmlPack::PutString(&inout_parentElement, mName);
	
	//const int mParentIndex;
	TXmlPack::PutU32(&inout_parentElement, mParentIndex);

	//const GMatrix16Float mParentRelativeMatrix;
	for (int index = 0; index < 16; ++index)
	{
		TXmlPack::PutR32(&inout_parentElement, mParentRelativeMatrix[index]);
	}

	return;
}
