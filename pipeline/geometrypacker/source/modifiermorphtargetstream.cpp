//file: ModifierMorphTargetStream.cpp

#include "ModifierMorphTargetStream.h"

#include <tinyxml.h>
#include <TXmlPack.h>

//constructor
ModifierMorphTargetStream::ModifierMorphTargetStream()
	: mStreamData()
	, mDuplicateOffsetArray()
{
	return;
}

ModifierMorphTargetStream::~ModifierMorphTargetStream()
{
	return;
}

//public methods
void ModifierMorphTargetStream::WriteOutData(
	TiXmlElement& out_parentElement
	)const
{
	//stream data
	{
		TiXmlElement arrayElement("pointer");
		arrayElement.SetAttribute("alignment", 4);
		TXmlPack::PutArrayU8(&arrayElement, mStreamData);			
		out_parentElement.InsertEndChild(arrayElement);
	}

	//offset data
	TXmlPack::PutU32(&out_parentElement, mDuplicateOffsetArray.size());			
	{
		TiXmlElement arrayElement("pointer");
		arrayElement.SetAttribute("alignment", 4);
		TXmlPack::PutArrayU32(&arrayElement, mDuplicateOffsetArray);			
		out_parentElement.InsertEndChild(arrayElement);
	}

	return;
}

const bool ModifierMorphTargetStream::TestWritten(const int in_vertexNewIndex)
{
	TMapIntInt::iterator found = mMapWritenVertexNewIndex.find(in_vertexNewIndex);
	if (found != mMapWritenVertexNewIndex.end())
	{
		return true;
	}

	mMapWritenVertexNewIndex[in_vertexNewIndex] = 0;

	return false;
}
