//file: ModifierMorph.cpp

#include "ModifierMorph.h"
#include "ModifierMorphTarget.h"

#include "DscVertexPool.h"
#include "StreamInfo.h"

#include <map>
#include <TCommon.h>
#include <GMeshModifier.h>
#include <TXml.h>
#include <TXmlPack.h>
#include <tinyxml.h>

/*
class GRenderMeshModifierMorphLoad
	const unsigned int mArrayTargetCount;
	const GRenderMeshModifierMorphTargetLoad* const mArrayTarget;

class GRenderMeshModifierMorphTargetLoad
	const char* const mTargetName;
	int streamCount
	streamArray
	int* duplicateTable //[ duplicate count, [duplicate0, duplicate1, ....]],

class GRenderMeshModifierMorphTargetStreamLoad
	void* streamData
	int duplicateTableIndexCount
	int* duplicateTableIndexArray (the 'i'th item uses the 'i'th stream data, with 'i'th item being the index of the duplicate table

*/

//static methods
/*static*/ ModifierMorph::TPointerModifierMorph ModifierMorph::Factory(
	TiXmlElement& in_sourceElement,
	const TArrayPointerStreamInfo& in_arrayStreamInfo, 
	const DscVertexPool& in_vertexPool,
	const TArrayU32& in_arrayWritenVertexIndex,
	TPackerMessage& in_packMessage
	)
{
	TPointerModifierMorph pointer;
	TArrayPointerModifierMorphTarget arrayTarget;

	TArrayBool arrayStreamSupported;
	//streams supported
	{
		TiXmlElement* pTrace = TiXmlHandle(&in_sourceElement).FirstChildElement("streamSupported").FirstChildElement("item").Element();
		while (pTrace)
		{
			const bool value = TXml::GetTextBool(pTrace);
			arrayStreamSupported.push_back(value);
			pTrace = pTrace->NextSiblingElement("item");
		}
	}
	//targets
	{
		TiXmlElement* pTrace = TiXmlHandle(&in_sourceElement).FirstChildElement("target").Element();
		while (pTrace)
		{
			TPointerModifierMorphTarget pointerTarget = ModifierMorphTarget::Factory(
				*pTrace, 
				in_arrayStreamInfo, 
				in_vertexPool, 
				in_arrayWritenVertexIndex,
				arrayStreamSupported,
				in_packMessage
				);
			arrayTarget.push_back(pointerTarget);

			pTrace = pTrace->NextSiblingElement("target");
		}
	}

	pointer.reset(new ModifierMorph(arrayTarget, arrayStreamSupported));

	return pointer;
}

//constructor
ModifierMorph::ModifierMorph(
	TArrayPointerModifierMorphTarget& in_arrayTarget, //WARN: transfer ownership
	const TArrayBool& in_arrayStreamSupported
	)
	: Modifier()
	, mArrayTarget(in_arrayTarget)
	, mArrayStreamSupported(in_arrayStreamSupported)
{
	return;
}

/*virtual*/ ModifierMorph::~ModifierMorph()
{
	return;
}


//implement Modifier
/*virtual*/ void ModifierMorph::OnWriteOutData(
	TiXmlElement& inout_parentElement
	)const
{
	TiXmlElement pointerElement("pointer");
	pointerElement.SetAttribute("alignment", 4);	
	TXmlPack::PutU32(&pointerElement, mArrayTarget.size());
	{
		TiXmlElement arrayElement("pointer");
		arrayElement.SetAttribute("alignment", 4);	
		for (TArrayPointerModifierMorphTarget::const_iterator iterator = mArrayTarget.begin(); iterator != mArrayTarget.end(); ++iterator)
		{
			(*iterator)->WriteOutData(arrayElement);
		}

		pointerElement.InsertEndChild(arrayElement);
	}

	TXmlPack::PutU32(&inout_parentElement, (U32)GMeshModifier::TType::TMorph);
	inout_parentElement.InsertEndChild(pointerElement);

	return;
}

