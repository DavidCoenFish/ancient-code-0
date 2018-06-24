//file: ModifierMorphTarget.cpp

#include "ModifierMorphTarget.h"
#include "ModifierMorphTargetStream.h"

#include "DscVertexPool.h"
#include "StreamInfo.h"

#include <TXml.h>
#include <TXmlPack.h>
#include <TPackerMessage.h>
#include <tinyxml.h>
#include <map>
#include <assert.h>

#include <GMeshType.h>

typedef boost::shared_ptr<ModifierMorphTarget> TPointerModifierMorphTarget;
typedef boost::shared_ptr<ModifierMorphTargetStream> TPointerModifierMorphTargetStream;
typedef std::vector<TPointerModifierMorphTargetStream> TArrayPointerModifierMorphTargetStream;

typedef std::vector<int> TArrayInt;
typedef std::vector<unsigned int> TArrayU32;
typedef std::vector<unsigned char> TArrayU8;

typedef boost::shared_ptr<StreamInfo> TPointerStreamInfo;
typedef std::vector<TPointerStreamInfo> TArrayPointerStreamInfo;
	typedef std::vector<bool> TArrayBool;

static const float LocalAbs(const float in_value)
{
	if (in_value < 0.0F)
	{
		return -in_value;
	}
	return in_value;
}

static void LocalAddStream(ModifierMorphTargetStream& inout_stream, const TArrayU8& in_arrayData, const int in_duplicateTableOffset)
{
	TArrayU8& destData = inout_stream.GetStreamData();
	destData.insert(destData.end(), in_arrayData.begin(), in_arrayData.end());
	inout_stream.GetDuplicateOffsetArray().push_back(in_duplicateTableOffset);
	return;
}

static const int LocalAddDuplactes(TArrayU32& inout_duplicateTable, const TArrayU32& in_duplicateArray)
{
	TArrayU32 toAddArray;
	const int duplicateCount = (int)in_duplicateArray.size();
	toAddArray.push_back(duplicateCount);
	toAddArray.insert(toAddArray.end(), in_duplicateArray.begin(), in_duplicateArray.end());
	for (int index = 0; index < (int)inout_duplicateTable.size() - (int)toAddArray.size(); ++index)
	{
		bool match = true;
		for (int subIndex = 0; subIndex < (int)toAddArray.size(); ++subIndex)
		{
			if (inout_duplicateTable[index + subIndex] != toAddArray[subIndex])
			{
				match = false;
				break;
			}
		}

		if (match)
		{
			return index;
		}
	}

	const int offset = (int)inout_duplicateTable.size();
	inout_duplicateTable.insert(inout_duplicateTable.end(), toAddArray.begin(), toAddArray.end());

	return offset;
}

static const bool LocalTestDataMatchFloat(
	const TArrayU8& in_arrayData,
	const DscVertexPool& in_vertexPool,
	const int in_newVertexIndex, 
	const StreamInfo& in_streamInfo
	)
{
	const TArrayU8& vertexData = in_vertexPool.GetVertexStreamData(in_newVertexIndex);
	const int streamOffset = in_streamInfo.GetVertexDataByteOffset();
	const int streamByteCount = in_streamInfo.GetStride();
	if (streamByteCount != (int)in_arrayData.size())
	{
		assert(false);
		return false;
	}
	for (int index = 0; index < streamByteCount; index += (sizeof(float)))
	{
		const float* const pLhs = (float*)&vertexData[streamOffset + index];
		const float* const pRhs = (float*)&in_arrayData[index];

		if (0.0001F < LocalAbs(*pLhs - *pRhs))
		{
			return false;
		}
	}

	return true;
}

static const bool LocalTestDataMatch(
	const TArrayU8& in_arrayData,
	const DscVertexPool& in_vertexPool,
	const int in_newVertexIndex, 
	const StreamInfo& in_streamInfo
	)
{
	const TArrayU8& vertexData = in_vertexPool.GetVertexStreamData(in_newVertexIndex);
	const int streamOffset = in_streamInfo.GetVertexDataByteOffset();
	const int streamByteCount = in_streamInfo.GetStride();
	if (streamByteCount != (int)in_arrayData.size())
	{
		assert(false);
		return false;
	}
	for (int index = 0; index < streamByteCount; ++index)
	{
		if (vertexData[streamOffset + index] != in_arrayData[index])
		{
			return false;
		}
	}

	return true;
}

static void LocalPullOutVertexDataStreamByte(
	ModifierMorphTargetStream& inout_stream,
	TArrayU32& inout_duplicateTable,
	TiXmlElement& in_sourceElement,
	const StreamInfo& in_streamInfo,
	const DscVertexPool& in_vertexPool,
	const TArrayU32& in_arrayWritenVertexIndex,
	const int in_vertexIndex,
	const std::string& in_targetName,
	TPackerMessage& in_packMessage
	)
{
	//pull out the stream data
	TArrayU8 arrayData;
	int itemCount = 0;
	TiXmlElement* pTrace = TiXmlHandle(&in_sourceElement).FirstChildElement("item").Element();
	while (pTrace)
	{
		const unsigned char value = (unsigned char)TXml::GetTextInt(pTrace);
		arrayData.push_back(value);
		itemCount += 1;
		pTrace = pTrace->NextSiblingElement("item");
	}

	if (itemCount != in_streamInfo.GetCount())
	{
		in_packMessage.AddError(std::string("bad item count byte stream in morph target:") + in_targetName);
		return;
	}

	//cmp with vertex data
	const int newVertexIndex = in_vertexPool.GetNewIndex(in_vertexIndex);
	if (inout_stream.TestWritten(newVertexIndex))
	{
		//already wrote this vertex for this stream
		return;
	}

	if (LocalTestDataMatch(arrayData, in_vertexPool, newVertexIndex, in_streamInfo))
	{
		//this morph value doesn't change the originial
		return;
	}

	//work out duplicates
	TArrayU32 duplicateArray;
	DscVertexPoolFindDuplicate(duplicateArray, newVertexIndex, in_arrayWritenVertexIndex, in_streamInfo, in_vertexPool);
	if (duplicateArray.size() <= 0)
	{
		return;
	}

	//add duplicates
	const int duplicateOffset = LocalAddDuplactes(inout_duplicateTable, duplicateArray);

	//add to stream
	LocalAddStream(inout_stream, arrayData, duplicateOffset);

	return;
}

static void LocalPullOutVertexDataStreamFloat(
	ModifierMorphTargetStream& inout_stream,
	TArrayU32& inout_duplicateTable,
	TiXmlElement& in_sourceElement,
	const StreamInfo& in_streamInfo,
	const DscVertexPool& in_vertexPool,
	const TArrayU32& in_arrayWritenVertexIndex,
	const int in_vertexIndex,
	const std::string& in_targetName,
	TPackerMessage& in_packMessage
	)
{
	//put out the stream data
	TArrayU8 arrayData;
	int itemCount = 0;
	TiXmlElement* pTrace = TiXmlHandle(&in_sourceElement).FirstChildElement("item").Element();
	while (pTrace)
	{
		const float value = TXml::GetTextFloat(pTrace);
		const unsigned char* const pData = (unsigned char*)&value;
		arrayData.push_back(pData[0]);
		arrayData.push_back(pData[1]);
		arrayData.push_back(pData[2]);
		arrayData.push_back(pData[3]);
		itemCount += 1;
		pTrace = pTrace->NextSiblingElement("item");
	}

	if (itemCount != in_streamInfo.GetCount())
	{
		in_packMessage.AddError(std::string("bad item count byte stream in morph target:") + in_targetName);
		return;
	}

	//cmp with vertex data
	const int newVertexIndex = in_vertexPool.GetNewIndex(in_vertexIndex);
	if (inout_stream.TestWritten(newVertexIndex))
	{
		//already wrote this vertex for this stream
		return;
	}

	if (LocalTestDataMatchFloat(arrayData, in_vertexPool, newVertexIndex, in_streamInfo))
	{
		//this morph value doesn't change the originial
		return;
	}

	//work out duplicates
	TArrayU32 duplicateArray;
	DscVertexPoolFindDuplicate(duplicateArray, newVertexIndex, in_arrayWritenVertexIndex, in_streamInfo, in_vertexPool);
	if (duplicateArray.size() <= 0)
	{
		return;
	}

	//mark all duplcate vertex as written
	for (TArrayU32::const_iterator iterator = duplicateArray.begin(); iterator != duplicateArray.end(); ++iterator)
	{
		const int localNewVertexIndex = in_arrayWritenVertexIndex[*iterator];
		inout_stream.TestWritten(localNewVertexIndex);
	}

	//add duplicates
	const int duplicateOffset = LocalAddDuplactes(inout_duplicateTable, duplicateArray);

	//add to stream
	LocalAddStream(inout_stream, arrayData, duplicateOffset);

	return;
}

static void LocalPullOutVertexDataStreamInt(
	ModifierMorphTargetStream& inout_stream,
	TArrayU32& inout_duplicateTable,
	TiXmlElement& in_sourceElement,
	const StreamInfo& in_streamInfo,
	const DscVertexPool& in_vertexPool,
	const TArrayU32& in_arrayWritenVertexIndex,
	const int in_vertexIndex,
	const std::string& in_targetName,
	TPackerMessage& in_packMessage
	)
{
	//put out the stream data
	TArrayU8 arrayData;
	int itemCount = 0;
	TiXmlElement* pTrace = TiXmlHandle(&in_sourceElement).FirstChildElement("item").Element();
	while (pTrace)
	{
		const int value = TXml::GetTextInt(pTrace);
		const unsigned char* const pData = (unsigned char*)&value;
		arrayData.push_back(pData[0]);
		arrayData.push_back(pData[1]);
		arrayData.push_back(pData[2]);
		arrayData.push_back(pData[3]);
		itemCount += 1;
		pTrace = pTrace->NextSiblingElement("item");
	}

	if (itemCount != in_streamInfo.GetCount())
	{
		in_packMessage.AddError(std::string("bad item count byte stream in morph target:") + in_targetName);
		return;
	}

	//cmp with vertex data
	const int newVertexIndex = in_vertexPool.GetNewIndex(in_vertexIndex);
	if (inout_stream.TestWritten(newVertexIndex))
	{
		//already wrote this vertex for this stream
		return;
	}

	if (LocalTestDataMatch(arrayData, in_vertexPool, newVertexIndex, in_streamInfo))
	{
		//this morph value doesn't change the originial
		return;
	}

	//work out duplicates
	TArrayU32 duplicateArray;
	DscVertexPoolFindDuplicate(duplicateArray, newVertexIndex, in_arrayWritenVertexIndex, in_streamInfo, in_vertexPool);
	if (duplicateArray.size() <= 0)
	{
		return;
	}

	//add duplicates
	const int duplicateOffset = LocalAddDuplactes(inout_duplicateTable, duplicateArray);

	//add to stream
	LocalAddStream(inout_stream, arrayData, duplicateOffset);

	return;
}

static void LocalPullOutVertexData(
	TArrayPointerModifierMorphTargetStream& inout_arrayStream,
	TArrayU32& inout_duplicateTable,
	TiXmlElement& in_sourceElement,
	const TArrayPointerStreamInfo& in_arrayStreamInfo, 
	const DscVertexPool& in_vertexPool,
	const TArrayU32& in_arrayWritenVertexIndex,
	const TArrayBool& in_arrayStreamSupported,
	const int in_vertexIndex,
	const std::string& in_targetName,
	TPackerMessage& in_packMessage
	)
{
	int streamTrace = 0;
	TiXmlElement* pTrace = TiXmlHandle(&in_sourceElement).FirstChildElement("streamArray").FirstChildElement().Element();
	while (pTrace)
	{
		bool streamSupported = true;
		if (streamTrace < (int)in_arrayStreamSupported.size())
		{
			streamSupported = in_arrayStreamSupported[streamTrace];
		}

		if (!streamSupported)
		{
			streamTrace += 1;
		}
		else
		{
			if (std::string("streamByte") == pTrace->Value())
			{
				if (((int)in_arrayStreamInfo.size() <= streamTrace) || 
					((GMeshType::TStreamType::TByte != in_arrayStreamInfo[streamTrace]->GetType()) &&
					((GMeshType::TStreamType::TUnsignedByte != in_arrayStreamInfo[streamTrace]->GetType()))))
				{
					in_packMessage.AddError(std::string("unexpected streamByte in morph target:") + in_targetName);
					return;
				}

				LocalPullOutVertexDataStreamByte(
					*inout_arrayStream[streamTrace],
					inout_duplicateTable,
					*pTrace,
					*in_arrayStreamInfo[streamTrace],
					in_vertexPool,
					in_arrayWritenVertexIndex,
					in_vertexIndex,
					in_targetName,
					in_packMessage
					);

				streamTrace += 1;
			}
			else if (std::string("streamFloat") == pTrace->Value())
			{
				if (((int)in_arrayStreamInfo.size() <= streamTrace) || 
					(GMeshType::TStreamType::TFloat != in_arrayStreamInfo[streamTrace]->GetType()))
				{
					in_packMessage.AddError(std::string("unexpected streamFloat in morph target:") + in_targetName);
					return;
				}

				LocalPullOutVertexDataStreamFloat(
					*inout_arrayStream[streamTrace],
					inout_duplicateTable,
					*pTrace,
					*in_arrayStreamInfo[streamTrace],
					in_vertexPool,
					in_arrayWritenVertexIndex,
					in_vertexIndex,
					in_targetName,
					in_packMessage
					);

				streamTrace += 1;
			}
			else if (std::string("streamInt") == pTrace->Value())
			{
				if (((int)in_arrayStreamInfo.size() <= streamTrace) || 
					((GMeshType::TStreamType::TInt != in_arrayStreamInfo[streamTrace]->GetType()) &&
					((GMeshType::TStreamType::TUnsignedInt != in_arrayStreamInfo[streamTrace]->GetType()))))
				{
					in_packMessage.AddError(std::string("unexpected streamInt in morph target:") + in_targetName);
					return;
				}

				LocalPullOutVertexDataStreamInt(
					*inout_arrayStream[streamTrace],
					inout_duplicateTable,
					*pTrace,
					*in_arrayStreamInfo[streamTrace],
					in_vertexPool,
					in_arrayWritenVertexIndex,
					in_vertexIndex,
					in_targetName,
					in_packMessage
					);

				streamTrace += 1;
			}
		}

		pTrace = pTrace->NextSiblingElement();
	}

	return;
}


//static methods
/*static*/ ModifierMorphTarget::TPointerModifierMorphTarget ModifierMorphTarget::Factory(
	TiXmlElement& in_sourceElement,
	const TArrayPointerStreamInfo& in_arrayStreamInfo, 
	const DscVertexPool& in_vertexPool,
	const TArrayU32& in_arrayWritenVertexIndex,
	const TArrayBool& in_arrayStreamSupported,
	TPackerMessage& in_packMessage
	)
{
	TPointerModifierMorphTarget pointer;

	const std::string name = TXml::GetTextString(TiXmlHandle(&in_sourceElement).FirstChildElement("name").Element());
	TArrayPointerModifierMorphTargetStream arrayStream;
	TArrayU32 duplicateTable;

	for (int index = 0; index < (int)in_arrayStreamInfo.size(); ++index)
	{
		TPointerModifierMorphTargetStream pointerStream;
		pointerStream.reset(new ModifierMorphTargetStream);
		arrayStream.push_back(pointerStream);
	}

	int vertexTrace = 0;
	TiXmlElement* pTrace = TiXmlHandle(&in_sourceElement).FirstChildElement("vertexArray").FirstChildElement("vertex").Element();
	while (pTrace)
	{
		LocalPullOutVertexData(
			arrayStream,
			duplicateTable,
			*pTrace,
			in_arrayStreamInfo,
			in_vertexPool,
			in_arrayWritenVertexIndex,
			in_arrayStreamSupported,
			vertexTrace,
			name,
			in_packMessage
			);

		vertexTrace += 1;
		pTrace = pTrace->NextSiblingElement("vertex");
	}

	pointer.reset(new ModifierMorphTarget(
		name,
		arrayStream,
		duplicateTable
		));

	return pointer;
}

//constructor
ModifierMorphTarget::ModifierMorphTarget(
	const std::string& in_name,
	const TArrayPointerModifierMorphTargetStream& in_arrayStream, //WARN: transfer ownership
	const TArrayU32& in_duplicateTable
	)
	: mName(in_name)
	, mArrayStream(in_arrayStream)
	, mDuplicateTable(in_duplicateTable)
{
	return;
}

ModifierMorphTarget::~ModifierMorphTarget()
{
	return;
}

//public methods
void ModifierMorphTarget::WriteOutData(
	TiXmlElement& out_parentElement
	)const
{
	TXmlPack::PutString(&out_parentElement, mName);
	TXmlPack::PutU32(&out_parentElement, mArrayStream.size());
	{
		TiXmlElement arrayElement("pointer");
		arrayElement.SetAttribute("alignment", 4);

		for (TArrayPointerModifierMorphTargetStream::const_iterator iterator = mArrayStream.begin(); iterator != mArrayStream.end(); ++iterator)
		{
			(*iterator)->WriteOutData(arrayElement);
		}

		out_parentElement.InsertEndChild(arrayElement);
	}
	{
		TiXmlElement arrayElement("pointer");
		arrayElement.SetAttribute("alignment", 4);
		TXmlPack::PutArrayU32(&arrayElement, mDuplicateTable);
		out_parentElement.InsertEndChild(arrayElement);
	}

	return;
}
