//file: packsvg.cpp

#include "packsvg.h"

#include "main.h"
#include "svgnode.h"

#include <TPackerMessage.h>
#include <TXml.h>
#include <TXmlPack.h>
#include <TCommon.h>
#include <tinyxml.h>
#include <GStringUtility.h>
#include <string>
#include <vector>

struct TCommand
{
	enum TEnum
	{
		TMoveAbsolute = 0, //M
		TMoveRelative, //m
		TClosePath, //Z, z
		TLineToAbsolute,
		TLineToRelative,
		THorizontalLineToAbsolute,
		THorizontalLineToRelative,
		TVerticalLineToAbsolute,
		TVerticalLineToRelative,
		TCount
	};
};

typedef std::vector<std::string> TArrayString;
typedef boost::shared_ptr<SvgNode> TPointerSvgNode;

static const TCommand::TEnum LocalGetCurrentCommand(const std::string& in_command)
{
	if (in_command == "M")
	{
		return TCommand::TMoveAbsolute;
	}
	if (in_command == "m")
	{
		return TCommand::TMoveRelative;
	}
	if (in_command == "Z")
	{
		return TCommand::TClosePath;
	}
	if (in_command == "z")
	{
		return TCommand::TClosePath;
	}
	if (in_command == "L")
	{
		return TCommand::TLineToAbsolute;
	}
	if (in_command == "l")
	{
		return TCommand::TLineToRelative;
	}
	if (in_command == "H")
	{
		return TCommand::THorizontalLineToAbsolute;
	}
	if (in_command == "h")
	{
		return TCommand::THorizontalLineToRelative;
	}
	if (in_command == "V")
	{
		return TCommand::TVerticalLineToAbsolute;
	}
	if (in_command == "v")
	{
		return TCommand::TVerticalLineToRelative;
	}
	return TCommand::TCount;
}

static const std::string LocalPrintFloat(const float in_value, const int in_accuracy)
{
	char temp[256];
	switch (in_accuracy)
	{
	default:
		sprintf_s(&temp[0], 256, "%f", in_value);
		break;
	case 0:
		sprintf_s(&temp[0], 256, "%.0f", in_value);
		break;
	case 1:
		sprintf_s(&temp[0], 256, "%.1f", in_value);
		break;
	case 2:
		sprintf_s(&temp[0], 256, "%.2f", in_value);
		break;
	case 3:
		sprintf_s(&temp[0], 256, "%.3f", in_value);
		break;
	case 4:
		sprintf_s(&temp[0], 256, "%.4f", in_value);
		break;
	case 5:
		sprintf_s(&temp[0], 256, "%.5f", in_value);
		break;
	case 6:
		sprintf_s(&temp[0], 256, "%.6f", in_value);
		break;
	case 7:
		sprintf_s(&temp[0], 256, "%.7f", in_value);
		break;
	case 8:
		sprintf_s(&temp[0], 256, "%.8f", in_value);
		break;
	case 9:
		sprintf_s(&temp[0], 256, "%.9f", in_value);
		break;
	}
	return std::string(&temp[0]);
}

static void LocalDealStyle(std::string& out_fill, const std::string& in_styleString)
{
	TArrayString arrayToken;
	//style="fill:#428000;fill-opacity:1;stroke:none"
	GStringUtility::Split(in_styleString, ";", arrayToken);

	for (int index = 0; index < (int)arrayToken.size(); ++index)
	{
		TArrayString arraySubToken;
		GStringUtility::Split(arrayToken[index], ":", arraySubToken);
		if (arraySubToken.size() <= 1)
		{
			continue;
		}
		if (arraySubToken[0] == "fill")
		{
			out_fill = arraySubToken[1];
			return;
		}
	}
}

static const bool LocalDealPath(
	TiXmlElement& in_sourceElement,
	const int in_accuracy,
	const GMatrix9Float& absoluteTransform, 
	std::string& inout_output, 
	const bool in_written,
	std::string& out_fill
	)
{
	const std::string styleString = TXml::GetAttributeString(&in_sourceElement, "style");
	LocalDealStyle(out_fill, styleString);
	const std::string dataString = TXml::GetAttributeString(&in_sourceElement, "d");
	TArrayString arrayToken;
	GStringUtility::Split(dataString, " ,", arrayToken);
	if (arrayToken.size() < 3)
	{
		return false;
	}

	//this is the comma bettween path blocks, not lables
	if (in_written)
	{
		inout_output += ",";
	}

	inout_output += "\n[";
	//TiXmlElement element("path");
	std::string text;


	int trace = 0;
	GVector3Float currentPoint;
	GVector3Float currentSubpathInitialPoint;
	TCommand::TEnum command = LocalGetCurrentCommand(arrayToken[trace]);
	trace += 1;

	bool writingData = false;

	while (trace < (int)arrayToken.size())
	{
		const TCommand::TEnum testCommand = LocalGetCurrentCommand(arrayToken[trace]);
		if (TCommand::TCount != testCommand)
		{
			if (1 != trace)
			{			
				if ((TCommand::TMoveAbsolute == command) ||
					(TCommand::TMoveRelative == command))
				{
					//new path
					inout_output += "],[";
					writingData = false;
				}
			}

			command = testCommand;
			trace += 1;
		}

		if (TCommand::TClosePath == command)
		{
			currentPoint = currentSubpathInitialPoint;
		}
		else if (TCommand::TMoveAbsolute == command)
		{
			currentPoint = GVector3Float(TXml::GetTextFloat(arrayToken[trace]), TXml::GetTextFloat(arrayToken[trace + 1]), 1.0F);
			trace += 2;
			currentSubpathInitialPoint = currentPoint;
			command = TCommand::TLineToAbsolute;
		}
		else if (TCommand::TMoveRelative == command)
		{
			currentPoint = GVector3Float(TXml::GetTextFloat(arrayToken[trace]), TXml::GetTextFloat(arrayToken[trace + 1]), 1.0F);
			trace += 2;
			currentSubpathInitialPoint = currentPoint;
			command = TCommand::TLineToRelative;
		}
		else if (TCommand::TLineToAbsolute == command)
		{
			currentPoint = GVector3Float(TXml::GetTextFloat(arrayToken[trace]), TXml::GetTextFloat(arrayToken[trace + 1]), 1.0F);
			trace += 2;
		}
		else if (TCommand::TLineToRelative == command)
		{
			currentPoint += (GVector3Float(TXml::GetTextFloat(arrayToken[trace]), TXml::GetTextFloat(arrayToken[trace + 1]), 1.0F));
			trace += 2;
		}
		else if (TCommand::THorizontalLineToAbsolute == command)
		{
			currentPoint.m_x = TXml::GetTextFloat(arrayToken[trace]);
			trace += 1;
		}
		else if (TCommand::THorizontalLineToRelative == command)
		{
			currentPoint.m_x += TXml::GetTextFloat(arrayToken[trace]);
			trace += 1;
		}
		else if (TCommand::TVerticalLineToAbsolute == command)
		{
			currentPoint.m_y = TXml::GetTextFloat(arrayToken[trace]);
			trace += 1;
		}
		else if (TCommand::TVerticalLineToRelative == command)
		{
			currentPoint.m_y += TXml::GetTextFloat(arrayToken[trace]);
			trace += 1;
		}
		else
		{
			break;
		}

		if (writingData)
		{
			text += ",";
		}

		const GVector3Float writePoint = (currentPoint * absoluteTransform);
		text += LocalPrintFloat(writePoint.m_x, in_accuracy);
		text += ",";
		text += LocalPrintFloat(writePoint.m_y - 1.0F, in_accuracy);
		writingData = true;

		if (TCommand::TClosePath == command)
		{
			break;
		}
	}

	inout_output += text;
	inout_output += "]";

	//TXmlPack::PutString(&in_destinationElement, text);

	return true;
}

static void LocalParseGroup(
	TiXmlElement* const in_element,
	TPointerSvgNode& in_parentSvgNode, 
	const int in_accuracy,
	//TiXmlElement* const in_destinationElement,
	std::string& inout_output, 
	TPackerMessage& in_packMessage,
	bool& inout_firstGroup
	)
{
	if (!in_element)
	{
		return;
	}
	TPointerSvgNode svgNode = SvgNode::Factory(in_parentSvgNode, *in_element);

	const std::string name = TXml::GetAttributeString(in_element, "inkscape:label");
	if (name.empty())
	{
		return;
	}

	TiXmlElement* pTracePath = TiXmlHandle(in_element).FirstChildElement("path").Element();
	if (!pTracePath)
	{
		return;
	}


	if (!inout_firstGroup)
	{
		inout_output += ",\n";
	}
	inout_output += "{\"data\":[";

	std::string localFill;

	//look for children path
	const GMatrix9Float absoluteTransform = svgNode->GetAbsoluteTransform();
	bool writenData = false;
	while (pTracePath)
	{
		writenData |= LocalDealPath(*pTracePath, in_accuracy, absoluteTransform, inout_output, writenData, localFill);
		pTracePath = pTracePath->NextSiblingElement("path");
	}

	//recursion
	TiXmlElement* pTrace = TiXmlHandle(in_element).FirstChildElement("g").Element();
	bool firstGroup = true;
	while (pTrace)
	{
		//LocalParseSvg(pTrace, svgNode, &destElement, in_packMessage);
		LocalParseGroup(pTrace, svgNode, in_accuracy, inout_output, in_packMessage, firstGroup);
		pTrace = pTrace->NextSiblingElement();
	}

	//in_destinationElement->InsertEndChild(destElement);

	inout_output += "],\n\"colour\":\"";
	inout_output += localFill;
	inout_output += "\"}";
	inout_firstGroup = false;
}

static void LocalPack(
	TiXmlElement* const in_sourceElement,
	std::string& inout_string,
	TPackerMessage& in_packMessage,
	const int in_accuracy
	)
{
	TPointerSvgNode rootSvgNode;
	TiXmlElement* pTrace = TiXmlHandle(in_sourceElement).FirstChildElement("g").Element();
	bool firstGroup = true;
	while (pTrace)
	{
		LocalParseGroup(pTrace, rootSvgNode, in_accuracy, inout_string, in_packMessage, firstGroup);
		pTrace = pTrace->NextSiblingElement("g");
	}

	return;
}

// static public methods
/*static*/ const bool PackSvg::Pack(
	TiXmlElement* const in_sourceElement,
	//TiXmlElement* const in_destinationElement,
	std::string& inout_string,
	TPackerMessage& in_packMessage,
	const int in_accuracy
	)
{
	if (NULL == in_sourceElement)
	{
		return false;
	}

	inout_string = "var s_characterDrawData = {\n";
	bool written = false;

	TiXmlElement* pTraceData = TiXmlHandle(in_sourceElement).FirstChildElement("data").Element();
	while (pTraceData)
	{
		const std::string dataKey = TXml::GetTextString(TiXmlHandle(pTraceData).FirstChildElement("key").Element());
		const std::string fileKey = TXml::GetTextString(TiXmlHandle(pTraceData).FirstChildElement("file").Element());

		TiXmlDocument sourceDocument;
		if (sourceDocument.LoadFile(fileKey))
		{
			in_packMessage.AddMessage(std::string("load data xml:") + fileKey);
		}
		else
		{
			in_packMessage.AddError(std::string("load data xml:") + fileKey, true);
		}

		if (written)
		{
			inout_string += ",\n";
		}
		inout_string += "\"";
		inout_string += dataKey;
		inout_string += "\":[";

		//printf("%s %s\n", dataKey.c_str(), fileKey.c_str());
		LocalPack(
			sourceDocument.RootElement(),
			inout_string,
			in_packMessage,
			in_accuracy
			);
		inout_string += "]";
		written = true;

		pTraceData = pTraceData->NextSiblingElement("data");
	}

	inout_string += "};";

	return true;
}