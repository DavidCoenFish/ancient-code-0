#include "Main.h"

#include "TXml.h"
#include "TXmlPack.h"
#include "PackCommon.h"
#include "DataCollector.h"
#include "DataBlockParam.h"

#include <stdio.h>
#include <vector>
#include <map>
#include <string>
#include <tinyxml.h>
#include <TPackerMessage.h>
#include <algorithm>

/*
the output file
//file start, first 4 bytes 
[4] offset to pointer table bytes
.... main data
//at pointer table offset
[4] pointer count
.... offset of pointers in main data

*/

typedef std::vector<U8> TArrayByte;
typedef std::vector<U8> TArrayU8;
typedef std::vector<U16> TArrayU16;
typedef std::vector<U32> TArrayU32;
typedef std::vector<R32> TArrayR32;
typedef boost::shared_ptr<DataBlock> TPointerDataBlock;
typedef std::map<std::string, TiXmlElement*> TMapStringElement;

static const int sWriteSize = 4096;

static void LocalParsePointerLabelsElement(
	TMapStringElement& inout_mapLableElement,
	TiXmlElement* const in_element
	)
{
	TiXmlElement* pTraceElement = TiXmlHandle(in_element).FirstChildElement().Element();
	while (pTraceElement)
	{
		const std::string elementName = pTraceElement->Value();

		if (elementName == "pointer")
		{
			const std::string label = TXml::GetAttributeString(pTraceElement, "label");
			inout_mapLableElement[label] = pTraceElement;
		
			LocalParsePointerLabelsElement(
				inout_mapLableElement,
				pTraceElement
				);
		}

		pTraceElement = pTraceElement->NextSiblingElement();
	}

	return;
}

static void LocalParseElement(
	DataCollector& inout_dataCollector, 
	DataBlockParam&	inout_dataBlockParam,
	TiXmlElement* const in_element,
	TMapStringElement& in_mapLableElement
	)
{
	TiXmlElement* pTraceElement = TiXmlHandle(in_element).FirstChildElement().Element();
	while (pTraceElement)
	{
		const std::string elementName = pTraceElement->Value();

		if (elementName == "u8")
		{
			const U8 value = TXml::GetTextInt(pTraceElement);
			inout_dataBlockParam.AddU8(value);
		}
		else if (elementName == "u16")
		{
			const U16 value = TXml::GetTextInt(pTraceElement);
			inout_dataBlockParam.AddU16(value);
		}
		else if (elementName == "u32")
		{
			const U32 value = TXml::GetTextInt(pTraceElement);
			inout_dataBlockParam.AddU32(value);
		}
		else if (elementName == "r32")
		{
			const R32 value = TXml::GetTextFloat(pTraceElement);
			inout_dataBlockParam.AddR32(value);
		}
		else if (elementName == "string")
		{
			const std::string value = TXml::GetTextString(pTraceElement);
			TPointerDataBlock pointerDataBlock = inout_dataCollector.CreateDataBlockString(value).lock();
			inout_dataBlockParam.AddPointer(pointerDataBlock);
		}
		else if (elementName == "u8array")
		{
			TArrayU8 arrayValue;
			TXmlPack::GetArrayU8(arrayValue, pTraceElement);
			inout_dataBlockParam.AddArray(arrayValue);
		}
		else if (elementName == "u16array")
		{
			TArrayU16 arrayValue;
			TXmlPack::GetArrayU16(arrayValue, pTraceElement);
			for (TArrayU16::iterator iterator = arrayValue.begin(); iterator != arrayValue.end(); ++iterator)
			{
				inout_dataBlockParam.AddU16(*iterator);
			}
		}
		else if (elementName == "u32array")
		{
			TArrayU32 arrayValue;
			TXmlPack::GetArrayU32(arrayValue, pTraceElement);
			for (TArrayU32::iterator iterator = arrayValue.begin(); iterator != arrayValue.end(); ++iterator)
			{
				inout_dataBlockParam.AddU32(*iterator);
			}
		}
		else if (elementName == "r32array")
		{
			TArrayR32 arrayValue;
			TXmlPack::GetArrayR32(arrayValue, pTraceElement);
			for (TArrayR32::iterator iterator = arrayValue.begin(); iterator != arrayValue.end(); ++iterator)
			{
				inout_dataBlockParam.AddR32(*iterator);
			}
		}
		else if (elementName == "pointer")
		{
			const int alignment = TXml::GetAttributeInt(pTraceElement, "alignment", 4);
			const std::string label = TXml::GetAttributeString(pTraceElement, "label");
	
			DataBlockParam childDataBlockParam(alignment);
			LocalParseElement(
				inout_dataCollector, 
				childDataBlockParam,
				pTraceElement,
				in_mapLableElement
				);

			TPointerDataBlock pointerDataBlock = inout_dataCollector.CreateDataBlock(childDataBlockParam, label).lock();
			inout_dataBlockParam.AddPointer(pointerDataBlock);
		}
		else if (elementName == "reference")
		{
			const std::string label = TXml::GetAttributeString(pTraceElement, "label");
			//TPointerDataBlock pointerDataBlock = inout_dataCollector.CreateReferencelabel(label).lock();
			//inout_dataBlockParam.AddPointer(pointerDataBlock);

			TMapStringElement::iterator found = in_mapLableElement.find(label);
			if (found != in_mapLableElement.end())
			{
				TiXmlElement* pElement = (*found).second;
				const int alignment = TXml::GetAttributeInt(pElement, "alignment", 4);
				const std::string label = TXml::GetAttributeString(pElement, "label");
		
				DataBlockParam childDataBlockParam(alignment);
				LocalParseElement(
					inout_dataCollector, 
					childDataBlockParam,
					pElement,
					in_mapLableElement
					);

				TPointerDataBlock pointerDataBlock = inout_dataCollector.CreateDataBlock(childDataBlockParam, label).lock();
				inout_dataBlockParam.AddPointer(pointerDataBlock);
			}
			else
			{
				TPointerDataBlock pointerDataBlock;
				inout_dataBlockParam.AddPointer(pointerDataBlock);
			}
		}

		pTraceElement = pTraceElement->NextSiblingElement();
	}

	return;
}

static void LocalLoadXml(
	DataCollector& inout_dataCollector, 
	const std::string& in_sourceXml,
	TPackerMessage& inout_message
	)
{
	//load the xml
	TiXmlDocument sourceDocument;
	if (sourceDocument.LoadFile(in_sourceXml.c_str()))
	{
		inout_message.AddMessage(std::string("LoadXml:") + in_sourceXml);
	}
	else
	{
		inout_message.AddError(std::string("Failed LoadXml:") + in_sourceXml);
		return;
	}

	DataBlockParam dataBlockParam(4);

	//preload the pointers with labels
	TMapStringElement mapLableElement;
	LocalParsePointerLabelsElement(
		mapLableElement,
		sourceDocument.RootElement()
		);

	//parse the xml
	LocalParseElement(
		inout_dataCollector,
		dataBlockParam,
		sourceDocument.RootElement(),
		mapLableElement
		);

	inout_dataCollector.CreateRootDataBlock(dataBlockParam);

	return;
}

static const bool LocalFileFromBuffer( 
	FILE& inout_file,
	const TArrayByte& in_arrayByte
	)
{
	int writeRemaining = (int)in_arrayByte.size();
	int trace = 0;
	bool ok = true;
	while (sWriteSize < writeRemaining)
	{
		if (1 != (int)fwrite((void*)(&in_arrayByte[0] + trace), sWriteSize, 1, &inout_file))
		{
			ok = false;
			break;
		}
		writeRemaining -= sWriteSize;
		trace += sWriteSize;
	}

	if (ok && (0 != writeRemaining))
	{
		if (1 != (int)fwrite((void*)(&in_arrayByte[0] + trace), writeRemaining, 1, &inout_file))
		{
			ok = false;
		}
	}

	return ok;
}

static void LocalSaveFile(
	const TArrayByte& in_arrayByte, 
	const std::string& in_destinationFileName, 
	TPackerMessage& inout_message
	)
{
	FILE* outFile = 0;
	fopen_s(&outFile, in_destinationFileName.c_str(), "wb");
	if (outFile)
	{
		if (LocalFileFromBuffer(*outFile, in_arrayByte))
		{
			inout_message.AddMessage(std::string("write to file:" + in_destinationFileName));
		}
		else
		{
			inout_message.AddError(std::string("FAILED: write to file:" + in_destinationFileName), true);
			return;
		}
	}
	fclose(outFile);
	return;
}

static const int LocalRun(const std::string& in_sourceXml, const std::string& in_destinationFileName)
{
	TPackerMessage message;
	message.AddMessage(std::string("FudgePacker::Run(") + in_sourceXml + ", " + in_destinationFileName + ")");
	DataCollector dataCollector;

	LocalLoadXml(dataCollector, in_sourceXml, message);

	{
		TArrayByte arrayByte;
		dataCollector.WriteOutDataBlock(arrayByte);
		LocalSaveFile(arrayByte, in_destinationFileName, message);
	}

	const int result = (int)message.GetCritical();

	message.Print();

	return result;
}

//application inpoint
int main(int in_argc, char ** in_argv)
{
	int result = 0;
	if (3 != in_argc)
	{
		printf( "\nusage:\\FudgePacker.exe src.xml destFileName.data\n" ); 
	}
	else
	{
		result = LocalRun(in_argv[1], in_argv[2]);
	}

#if DSC_DEBUG
	printf( "\n<press enter key to exit>" ); 
	char c; 
	scanf_s("%c", &c); 
#endif

	return result;
}
