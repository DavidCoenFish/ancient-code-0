//file: packskeleton.cpp

#include "packskeleton.h"

#include "main.h"
#include "skeletonbone.h"

#include <TPackerMessage.h>
#include <TXml.h>
#include <TXmlPack.h>
#include <TCommon.h>
#include <tinyxml.h>

typedef boost::shared_ptr<SkeletonBone> TPointerSkeletonBone;
typedef std::vector<TPointerSkeletonBone> TArrayPointerSkeletonBone;
typedef std::vector<std::string> TArrayString;

static void LocalGetStreamData(
	TArrayPointerSkeletonBone& out_arrayBone, 
	TiXmlElement* const in_sourceElement
	)
{
	TiXmlElement* pTraceBone = TiXmlHandle(in_sourceElement).FirstChildElement("boneArray").FirstChildElement("bone").Element();
	while (pTraceBone)
	{
		TPointerSkeletonBone pointerBone = SkeletonBone::Factory(*pTraceBone);
		if (pointerBone)
		{
			out_arrayBone.push_back(pointerBone);
		}

		pTraceBone = pTraceBone->NextSiblingElement("bone");
	}
	return;
}

static void LocalGetArrayMorphTarget(
	TArrayString& out_arrayMorphTarget, 
	TiXmlElement* const in_sourceElement
	)
{
	TiXmlElement* pTrace = TiXmlHandle(in_sourceElement).FirstChildElement("morphTargetArray").FirstChildElement("name").Element();
	while (pTrace)
	{
		out_arrayMorphTarget.push_back(TXml::GetTextString(pTrace));

		pTrace = pTrace->NextSiblingElement("name");
	}

	return;
}

// static public methods
/*static*/ const bool PackSkeleton::Pack(
	TiXmlElement* const in_sourceElement,
	TiXmlElement* const in_destinationElement,
	TPackerMessage& in_packMessage
	)
{
	if ((NULL == in_sourceElement) || (NULL == in_destinationElement))
	{
		return false;
	}

	TArrayPointerSkeletonBone arrayBone;
	TArrayString arrayMorphTarget;

	//collect bone data
	LocalGetStreamData(arrayBone, in_sourceElement);

	LocalGetArrayMorphTarget(arrayMorphTarget, in_sourceElement);

	//write out data
	{
		TXmlPack::PutU32(in_destinationElement, arrayBone.size());
		TiXmlElement pointerElement("pointer");
		pointerElement.SetAttribute("alignment", 4);
		for (TArrayPointerSkeletonBone::const_iterator iterator = arrayBone.begin(); iterator != arrayBone.end(); ++iterator)
		{
			(*iterator)->WriteOutData(pointerElement);
		}
		in_destinationElement->InsertEndChild(pointerElement);
	}
	{
		TXmlPack::PutU32(in_destinationElement, arrayMorphTarget.size());
		TiXmlElement pointerElement("pointer");
		pointerElement.SetAttribute("alignment", 4);
		for (TArrayString::const_iterator iterator = arrayMorphTarget.begin(); iterator != arrayMorphTarget.end(); ++iterator)
		{
			TXmlPack::PutString(&pointerElement, *iterator);
		}
		in_destinationElement->InsertEndChild(pointerElement);
	}

	return true;
}