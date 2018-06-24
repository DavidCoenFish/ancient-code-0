//file: FontSpliter.cpp

#include "FontSpliter.h"

#include "Main.h"
#include "FontSpliterData.h"

#include <boost/shared_ptr.hpp>
#include <TXml.h>
#include <TXmlPack.h>
#include <TPackerMessage.h>
#include <TCommon.h>
#include <tinyxml.h>
#include <FreeImage.h>
#include <map>

typedef boost::shared_ptr<FontSpliterData> TPointerFontSpliterData;
typedef std::map<int, TPointerFontSpliterData> TMapIntPointerFontSpliterData;


static const std::string LocalDoctorPath(
	const std::string& in_filePath, 
	const std::string& in_relative
	)
{
	std::string localRelative(in_relative);
	//trim the relative path down to the last dir marker
	while (0 < localRelative.size())
	{
		const int index = localRelative.size() - 1;
		const char value = localRelative[index];
		if ((value == '\\') ||
			(value == '/'))
		{
			break;
		}
		localRelative.resize(index);
	}

	return localRelative + in_filePath;
}

static const bool LocalFirstPixelLastPixelColumn(
	int& out_firstPixelHeight,
	int& out_lastPixelHeight,
	const int in_horizontalPixelTrace,
	FIBITMAP& in_image,
	const int in_imageHeight
	)
{
	out_firstPixelHeight = 0;
	out_lastPixelHeight = 0;
	bool pixelFound = false;
	for (int indexY = 0; indexY < in_imageHeight; ++indexY)
	{
		RGBQUAD value;
		FreeImage_GetPixelColor(&in_image, in_horizontalPixelTrace, indexY, &value);

		const bool hasPixel = (0 != value.rgbReserved);
		if (!pixelFound && hasPixel)
		{
			pixelFound = true;
			out_firstPixelHeight = indexY;
			out_lastPixelHeight = indexY;
		}
		else if(pixelFound && hasPixel)
		{
			out_lastPixelHeight = indexY;
		}
	}
	return pixelFound;
}


static void LocalSliceOffGlyph(
	int& inout_horizontalPixelTrace, 
	std::string& out_glyphFileName,
	int& out_glyphBaseLine,
	FIBITMAP& in_image,
	const int in_glyphValue,
	const int in_baseLine,
	const std::string& in_destinationPath,
	const std::string& in_localDestinationPath,
	TPackerMessage& inout_message,
	const int in_imageWidth,
	const int in_imageHeight
	)
{
	int lowX = 0;
	int highX = 0;
	int lowY = 0;
	int highY = 0;

	bool foundColumn = false;
	for (int indexX = inout_horizontalPixelTrace; indexX < in_imageWidth; ++indexX)
	{
		int testLowY = 0;
		int testHighY = 0;
		const bool testFound = LocalFirstPixelLastPixelColumn(
			testLowY,
			testHighY,
			indexX,
			in_image,
			in_imageHeight
			);

		if (!foundColumn && testFound)
		{
			lowX = indexX;
			highX = indexX;
			lowY = testLowY;
			highY = testHighY;
			foundColumn = true;
		}
		else if (foundColumn && testFound)
		{
			highX = indexX;
			if (testLowY < lowY)
			{
				lowY = testLowY;
			}
			if (highY < testHighY)
			{
				highY = testHighY;
			}
		}
		else if (foundColumn && !testFound)
		{
			inout_horizontalPixelTrace = indexX + 1;
			break;
		}
	}

	if (foundColumn)
	{
		//FIBITMAP* pCopy = FreeImage_Copy(&in_image, lowX, lowY, highX, highY);
		FIBITMAP* pCopy = FreeImage_Copy(&in_image, lowX, (in_imageHeight - highY) - 1, highX + 1, in_imageHeight - lowY);
		char blank[64];
		sprintf_s(blank, 64, "char_%08x.tga", in_glyphValue);

		out_glyphBaseLine = in_baseLine - (in_imageHeight - highY); //free image uses bottomLeft as origin, baseline uses upper left 
		out_glyphFileName = in_destinationPath + &blank[0];
		const std::string filePath = in_localDestinationPath + &blank[0];

		const FREE_IMAGE_FORMAT format = FreeImage_GetFIFFromFilename(filePath.c_str());

		if (!FreeImage_Save(format, pCopy, filePath.c_str()))
		{
			inout_message.AddError(std::string("save failed:") + filePath);
		}

		FreeImage_Unload(pCopy);
	}

	return;
}

static void LocalProcessImageData(
	TMapIntPointerFontSpliterData& inout_mapValueData,
	FIBITMAP& in_image,
	const std::string& in_charSequence,
	const int in_baseLine,
	const std::string& in_destinationPath,
	const std::string& in_localDestinationPath,
	TPackerMessage& inout_message
	)
{
	int horizontalPixelTrace = 0;
	int charSequenceTrace = 0;
	const int width = FreeImage_GetWidth(&in_image);
	const int height = FreeImage_GetHeight(&in_image);

	while ((charSequenceTrace < (int)in_charSequence.length()) && (horizontalPixelTrace < width))
	{
		const int value = (unsigned char)(in_charSequence[charSequenceTrace]);
		charSequenceTrace += 1;

		std::string glyphFileName;
		int glyphBaseLine = 0;

		LocalSliceOffGlyph(
			horizontalPixelTrace, 
			glyphFileName,
			glyphBaseLine,
			in_image,
			value,
			in_baseLine,
			in_destinationPath,
			in_localDestinationPath,
			inout_message,
			width,
			height
			);

		if (!glyphFileName.empty())
		{
			TPointerFontSpliterData fontSpliterData;
			fontSpliterData.reset(new FontSpliterData(glyphFileName, glyphBaseLine));
			inout_mapValueData[value] = fontSpliterData;
		}
	}

	return;
}

static void LocalProcessImage(
	TMapIntPointerFontSpliterData& inout_mapValueData,
	const std::string& in_filePath,
	const std::string& in_charSequence,
	const int in_baseLine,
	const std::string& in_destinationPath,
	const std::string& in_localDestinationPath,
	TPackerMessage& inout_message
	)
{
	//try to load image
	const FREE_IMAGE_FORMAT format = FreeImage_GetFIFFromFilename(in_filePath.c_str());
	FIBITMAP* pImage = FreeImage_Load(format, in_filePath.c_str());
	if (!pImage)
	{
		inout_message.AddError(std::string("load failed:") + in_filePath);
	}

	LocalProcessImageData(
		inout_mapValueData,
		*pImage,
		in_charSequence,
		in_baseLine,
		in_destinationPath,
		in_localDestinationPath,
		inout_message
		);

	FreeImage_Unload(pImage);

	return;
}

/*static*/ const bool FontSpliter::Pack(
	TiXmlElement* const in_sourceElement,
	TiXmlElement* const in_destinationElement,
	const std::string& in_sourceFilePath,
	TPackerMessage& inout_message
	)
{
	if ((!in_sourceElement) || (!in_destinationElement))
	{
		return false;
	}

    const int spaceWidth = TXml::GetTextInt(TiXmlHandle(in_sourceElement).FirstChildElement("source").FirstChildElement("spaceWidth").Element());
    const int newLine = TXml::GetTextInt(TiXmlHandle(in_sourceElement).FirstChildElement("source").FirstChildElement("newLine").Element());
	const std::string destinationPath = TXml::GetTextString(TiXmlHandle(in_sourceElement).FirstChildElement("destination").FirstChildElement("destinationPath").Element());
	const std::string localDestinationPath = LocalDoctorPath(destinationPath, in_sourceFilePath);

	TMapIntPointerFontSpliterData mapValueData;
	TiXmlElement* pTraceElement = TiXmlHandle(in_sourceElement).FirstChildElement("source").FirstChildElement("image").Element();
	while (pTraceElement)
	{
		const std::string filePath = TXml::GetTextString(TiXmlHandle(pTraceElement).FirstChildElement("filePath").Element());
		const std::string charSequence = TXml::GetTextString(TiXmlHandle(pTraceElement).FirstChildElement("charSequence").Element());
		const int baseLine = TXml::GetTextInt(TiXmlHandle(pTraceElement).FirstChildElement("baseLine").Element());

		const std::string localFilePath = LocalDoctorPath(filePath, in_sourceFilePath);

		LocalProcessImage(
			mapValueData,
			localFilePath,
			charSequence,
			baseLine,
			destinationPath,
			localDestinationPath,
			inout_message
			);

		pTraceElement = pTraceElement->NextSiblingElement("image");
	}

	const std::string tagetTexture = TXml::GetTextString(TiXmlHandle(in_sourceElement).FirstChildElement("destination").FirstChildElement("tagetTexture").Element());
    const int targetWidth = TXml::GetTextInt(TiXmlHandle(in_sourceElement).FirstChildElement("destination").FirstChildElement("targetWidth").Element());
    const int targetHeight = TXml::GetTextInt(TiXmlHandle(in_sourceElement).FirstChildElement("destination").FirstChildElement("targetHeight").Element());
    const int writeOriginX = TXml::GetTextInt(TiXmlHandle(in_sourceElement).FirstChildElement("destination").FirstChildElement("writeOriginX").Element());
    const int writeOriginY = TXml::GetTextInt(TiXmlHandle(in_sourceElement).FirstChildElement("destination").FirstChildElement("writeOriginY").Element());
    const int writeWidth = TXml::GetTextInt(TiXmlHandle(in_sourceElement).FirstChildElement("destination").FirstChildElement("writeWidth").Element());
    const int writeHeight = TXml::GetTextInt(TiXmlHandle(in_sourceElement).FirstChildElement("destination").FirstChildElement("writeHeight").Element());

	//write destination xml
	{
		TiXmlElement valueElement("spaceWidth");
		TXml::SetTextInt(&valueElement, spaceWidth);
		in_destinationElement->InsertEndChild(valueElement);
	}
	{
		TiXmlElement valueElement("newLine");
		TXml::SetTextInt(&valueElement, newLine);
		in_destinationElement->InsertEndChild(valueElement);
	}
	//write fontDataArray
	TiXmlElement fontDataArrayElement("fontDataArray");
	for (TMapIntPointerFontSpliterData::iterator iterator = mapValueData.begin(); iterator != mapValueData.end(); ++iterator)
	{
		TiXmlElement fontDataElement("fontData");
		{
			TiXmlElement valueElement("filePath");
			TXml::SetTextString(&valueElement, (*iterator).second->GetFileName());
			fontDataElement.InsertEndChild(valueElement);
		}
		{
			TiXmlElement valueElement("value");
			TXml::SetTextInt(&valueElement, (*iterator).first);
			fontDataElement.InsertEndChild(valueElement);
		}
		{
			TiXmlElement valueElement("baseLine");
			TXml::SetTextInt(&valueElement, (*iterator).second->GetBaseLine());
			fontDataElement.InsertEndChild(valueElement);
		}

		fontDataArrayElement.InsertEndChild(fontDataElement);
	}
	in_destinationElement->InsertEndChild(fontDataArrayElement);

	//write destination element
	TiXmlElement destinationElement("destination");
	{
		TiXmlElement valueElement("tagetTexture");
		TXml::SetTextString(&valueElement, tagetTexture);
		destinationElement.InsertEndChild(valueElement);
	}
	{
		TiXmlElement valueElement("targetWidth");
		TXml::SetTextInt(&valueElement, targetWidth);
		destinationElement.InsertEndChild(valueElement);
	}
	{
		TiXmlElement valueElement("targetHeight");
		TXml::SetTextInt(&valueElement, targetHeight);
		destinationElement.InsertEndChild(valueElement);
	}
	{
		TiXmlElement valueElement("writeOriginX");
		TXml::SetTextInt(&valueElement, writeOriginX);
		destinationElement.InsertEndChild(valueElement);
	}
	{
		TiXmlElement valueElement("writeOriginY");
		TXml::SetTextInt(&valueElement, writeOriginY);
		destinationElement.InsertEndChild(valueElement);
	}
	{
		TiXmlElement valueElement("writeWidth");
		TXml::SetTextInt(&valueElement, writeWidth);
		destinationElement.InsertEndChild(valueElement);
	}
	{
		TiXmlElement valueElement("writeHeight");
		TXml::SetTextInt(&valueElement, writeHeight);
		destinationElement.InsertEndChild(valueElement);
	}
	in_destinationElement->InsertEndChild(destinationElement);

	return true;
}
