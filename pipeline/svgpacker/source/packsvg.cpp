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


static const bool LocalDealPath(
	TiXmlElement& in_sourceElement,
	const int in_accuracy,
	const GMatrix9Float& absoluteTransform, 
	std::string& inout_output, 
	const std::string& in_lable, 
	bool& inout_currentLableWritten,
	const bool in_written
	//TiXmlElement& in_destinationElement
	)
{
	const std::string dataString = TXml::GetAttributeString(&in_sourceElement, "d");
	TArrayString arrayToken;
	GStringUtility::Split(dataString, " ,", arrayToken);
	if (arrayToken.size() < 3)
	{
		return false;
	}

	if (!inout_currentLableWritten)
	{
		inout_output += "\nvar s_";
		inout_output += in_lable;
		inout_output += "=[";
		inout_currentLableWritten = true;
	}

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

static void LocalParseSvg(
	TiXmlElement* const in_element,
	TPointerSvgNode& in_parentSvgNode, 
	const int in_accuracy,
	//TiXmlElement* const in_destinationElement,
	std::string& inout_output, 
	const std::string& in_lable, 
	bool& inout_currentLableWritten,
	TPackerMessage& in_packMessage
	)
{
	if (!in_element)
	{
		return;
	}
	TPointerSvgNode svgNode = SvgNode::Factory(in_parentSvgNode, *in_element);
	std::string localLable(in_lable);
	bool localCurrentLableWritten = false;
	bool* currentLableWritten = &inout_currentLableWritten;

	//TiXmlElement destElement("node");

	//look for children lable
	//TiXmlElement* pLableTrace = TiXmlHandle(in_element).FirstChildElement("title").Element();
	//if (pLableTrace)
	const std::string name = TXml::GetAttributeString(in_element, "inkscape:label");
	if (!name.empty())
	{
		//const std::string name = TXml::GetTextString(pLableTrace);
		localLable += name;
		currentLableWritten = &localCurrentLableWritten;
		//TiXmlElement titleElement("title");
		//TXmlPack::PutString(&titleElement, name);
		//in_destinationElement->InsertEndChild(titleElement);
	}

	//look for children path
	const GMatrix9Float absoluteTransform = svgNode->GetAbsoluteTransform();
	TiXmlElement* pTracePath = TiXmlHandle(in_element).FirstChildElement("path").Element();
	bool writenData = false;
	while (pTracePath)
	{
		writenData |= LocalDealPath(*pTracePath, in_accuracy, absoluteTransform, inout_output, localLable, *currentLableWritten, writenData);
		pTracePath = pTracePath->NextSiblingElement("path");
	}

	//recursion
	TiXmlElement* pTrace = TiXmlHandle(in_element).FirstChildElement().Element();
	while (pTrace)
	{
		//LocalParseSvg(pTrace, svgNode, &destElement, in_packMessage);
		LocalParseSvg(pTrace, svgNode, in_accuracy, inout_output, localLable, *currentLableWritten, in_packMessage);
		pTrace = pTrace->NextSiblingElement();
	}

	//in_destinationElement->InsertEndChild(destElement);

	if (localCurrentLableWritten)
	{
		inout_output += "\n];";
	}
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
	//if ((NULL == in_sourceElement) || (NULL == in_destinationElement))
	if (NULL == in_sourceElement)
	{
		return false;
	}

	TPointerSvgNode rootSvgNode;
	TiXmlElement* pTrace = TiXmlHandle(in_sourceElement).FirstChildElement().Element();
	std::string lable;
	bool currentLableWritten = false;
	while (pTrace)
	{
		LocalParseSvg(pTrace, rootSvgNode, in_accuracy, inout_string, lable, currentLableWritten, in_packMessage);
		pTrace = pTrace->NextSiblingElement();
	}

	//TXml::SetTextString(in_destinationElement, output);

	return true;
}