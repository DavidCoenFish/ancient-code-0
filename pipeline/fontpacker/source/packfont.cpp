//file: PackFont.cpp

#include "PackFont.h"
#include "PackFontData.h"
#include "Image.h"

#include <TXml.h>
#include <TXmlPack.h>
#include <TPackerMessage.h>
#include <TCommon.h>
#include <tinyxml.h>
#include <FreeImage.h>
#include <boost/shared_ptr.hpp>

typedef boost::shared_ptr<PackFontData> TPointerPackFontData;
typedef std::vector<TPointerPackFontData> TArrayPointerPackFontData;
typedef boost::shared_ptr<Image> TPointerImage;

const bool SortPackFontData(const TPointerPackFontData& in_lhs, const TPointerPackFontData& in_rhs)
{
	if (in_lhs->GetHeight() == in_rhs->GetHeight())
	{
		return (in_lhs->GetWidth() < in_rhs->GetWidth());
	}
	return (in_lhs->GetHeight() < in_rhs->GetHeight());
}

//local static methods
static const bool LocalWriteOverlap(
	Image& in_writeImage,
	Image& in_testImage,
	const int in_originX,
	const int in_originY,
	const int in_writeWidth,
	const int in_writeHeight
	)
{
	for (int indexY = 0; indexY < in_writeHeight; ++indexY)
	{
		for (int indexX = 0; indexX < in_writeWidth; ++indexX)
		{
			if (in_writeImage.GetWritten(indexX + in_originX, indexY + in_originY) &&
				in_testImage.GetWritten(indexX, indexY))
			{
				return true;
			}
		}
	}

	return false;
}

static void LocalWritePadGlyph(
	TPointerImage& inout_writeImage,
	PackFontData& inout_packFontData,
	const int in_x,
	const int in_y
	)
{
	Image::Blit(
		*inout_writeImage,
		*inout_packFontData.GetImage(),
		in_x + 1,
		in_y + 1,
		inout_packFontData.GetWidth(),
		inout_packFontData.GetHeight()
		);

	for (int indexY = 0; indexY <= inout_packFontData.GetHeight(); ++indexY)
	{
		for (int indexX = 0; indexX <= inout_packFontData.GetWidth(); ++indexX)
		{
			if ((0 == indexX) ||
				(0 == indexY))
			{
				inout_writeImage->SetWritten(in_x + indexX, in_y + indexY, true);
				//inout_writeImage->SetPixel(in_x + indexX, in_y + indexY, 255, 0, 0, 255);
			}
			else
			{
				break;
			}
		}
	}

	inout_packFontData.GetUVTop() = in_y + 1;
	inout_packFontData.GetUVBottom() = in_y + inout_packFontData.GetHeight() + 1;
	inout_packFontData.GetUVLeft() = in_x + 1;
	inout_packFontData.GetUVRight() = in_x + inout_packFontData.GetWidth() + 1;

	return;
}


static const bool LocalWriteFromTopLeft(
	int& inout_traceHeight,
	TPointerImage& inout_writeImage,
	TArrayPointerPackFontData& inout_arrayFontData
	)
{
	const int originalVerticalTrace = inout_traceHeight;
	int horizontalTrace = 0;
	bool written = true;
	bool anyWritten = false;
	while (written)
	{
		written = false;
		const int remaingSpace = inout_writeImage->GetWidth() - horizontalTrace;
		for (TArrayPointerPackFontData::reverse_iterator iterator = inout_arrayFontData.rbegin(); iterator != inout_arrayFontData.rend(); ++iterator)
		{
			PackFontData& packFontData = *(*iterator);
			if ((packFontData.GetWidth() + 1 < remaingSpace) &&
				(originalVerticalTrace + 1 + packFontData.GetHeight() <= inout_writeImage->GetHeight()))
			{
				LocalWritePadGlyph(
					inout_writeImage,
					packFontData,
					horizontalTrace,
					originalVerticalTrace
					);

				int offset = (inout_arrayFontData.rend() - iterator) - 1;

				inout_arrayFontData.erase(inout_arrayFontData.begin() + offset);
				written = true;
				anyWritten = true;

				horizontalTrace += (1 + packFontData.GetWidth());
				const int testTrace = originalVerticalTrace + 1 + packFontData.GetHeight();
				if (inout_traceHeight < testTrace)
				{
					inout_traceHeight = testTrace;
				}

				break;
			}
		}
	}

	return anyWritten;
}

static const bool LocalWriteFromBottomRightPushUp(
	int& inout_traceHeight,
	TPointerImage& inout_writeImage,
	TArrayPointerPackFontData& inout_arrayFontData
	)
{
	bool anyWritten = false;
	bool written = true;

	while (written)
	{
		written = false;
		for (TArrayPointerPackFontData::iterator iterator = inout_arrayFontData.begin(); iterator != inout_arrayFontData.end(); ++iterator)
		{
			PackFontData& packFontData = *(*iterator);
			for (int indexX = 0; indexX < inout_writeImage->GetWidth() - (packFontData.GetWidth() + 1); ++indexX)
			{
				if (!Image::TestWritten(*inout_writeImage, indexX, inout_traceHeight - (packFontData.GetHeight() + 1), packFontData.GetWidth() + 1, packFontData.GetHeight() + 1))
				{
					written = true;
					anyWritten = true;
					LocalWritePadGlyph(
						inout_writeImage,
						packFontData,
						indexX,
						inout_traceHeight - (packFontData.GetHeight() + 1)
						);
					break;
				}
			}
			if (written)
			{
				inout_arrayFontData.erase(iterator);
				break;
			}
		}
	}

	return anyWritten;
}

static void LocalFillUpPixelData(
	TPointerImage& inout_writeImage,
	TArrayPointerPackFontData& inout_arrayFontData
	)
{
	int traceHeight = 0;
	while (inout_arrayFontData.size())
	{
		bool written = false;
		written |= LocalWriteFromTopLeft(
			traceHeight,
			inout_writeImage,
			inout_arrayFontData
			);
		written |= LocalWriteFromBottomRightPushUp(
			traceHeight,
			inout_writeImage,
			inout_arrayFontData
			);
		if (!written)
		{
			break;
		}
	}

	return;
}

static void LocalPackFontData(
	TArrayPointerPackFontData& in_arrayData,
	const std::string in_tagetTexture,
	const int in_targetWidth,
	const int in_targetHeight,
	const int in_writeOriginX,
	const int in_writeOriginY,
	const int in_writeWidth,
	const int in_writeHeight,
	TPackerMessage& inout_message
	)
{
	TPointerImage writeImage = Image::FactoryBlank(in_writeWidth, in_writeHeight);

	TArrayPointerPackFontData copyFontData;
	//copy the visible glyphs
	for (TArrayPointerPackFontData::iterator iterator = in_arrayData.begin(); iterator != in_arrayData.end(); ++iterator)
	{
		if((*iterator)->GetIsVisible())
		{
			copyFontData.push_back(*iterator);
		}
	}

	//sort font data
	std::sort(copyFontData.begin(), copyFontData.end(), SortPackFontData);

	//write into pixel data
	LocalFillUpPixelData(
		writeImage,
		copyFontData
		);

	if (copyFontData.size())
	{
		char blank[64];
		sprintf_s(blank, 64, "%d", copyFontData.size());
		inout_message.AddError(std::string("Failed to fit glyph count: ") + &blank[0]);
	}

	//write pixel data to result image
	const FREE_IMAGE_FORMAT format = FreeImage_GetFIFFromFilename(in_tagetTexture.c_str());
	FIBITMAP* pImage = FreeImage_Load(format, in_tagetTexture.c_str());
	if (!pImage)
	{
		pImage = FreeImage_AllocateT(FIT_BITMAP, in_targetWidth, in_targetHeight, 32);
	}

	//blit image
	Image::BlitFreeImage(
		*pImage,
		*writeImage,
		in_writeOriginX,
		in_writeOriginY,
		in_writeWidth,
		in_writeHeight
		);

	if (!FreeImage_Save(format, pImage, in_tagetTexture.c_str()))
	{
		inout_message.AddError(std::string("Failed to save image: ") + in_tagetTexture);
	}

	FreeImage_Unload(pImage);

	return;
}

/*static*/ const bool PackFont::Pack(
	TiXmlElement* const in_sourceElement,
	TiXmlElement* const in_destinationElement,
	TPackerMessage& inout_message,
	const std::string& in_sourcePath
	)
{
	if ((!in_sourceElement) || (!in_destinationElement))
	{
		return false;
	}

    const int spaceWidth = TXml::GetTextInt(TiXmlHandle(in_sourceElement).FirstChildElement("spaceWidth").Element());
    const int newLine = TXml::GetTextInt(TiXmlHandle(in_sourceElement).FirstChildElement("newLine").Element());

	const std::string tagetTexture = TXml::GetTextString(TiXmlHandle(in_sourceElement).FirstChildElement("destination").FirstChildElement("tagetTexture").Element());
    const int targetWidth = TXml::GetTextInt(TiXmlHandle(in_sourceElement).FirstChildElement("destination").FirstChildElement("targetWidth").Element());
    const int targetHeight = TXml::GetTextInt(TiXmlHandle(in_sourceElement).FirstChildElement("destination").FirstChildElement("targetHeight").Element());
    const int writeOriginX = TXml::GetTextInt(TiXmlHandle(in_sourceElement).FirstChildElement("destination").FirstChildElement("writeOriginX").Element());
    const int writeOriginY = TXml::GetTextInt(TiXmlHandle(in_sourceElement).FirstChildElement("destination").FirstChildElement("writeOriginY").Element());
    const int writeWidth = TXml::GetTextInt(TiXmlHandle(in_sourceElement).FirstChildElement("destination").FirstChildElement("writeWidth").Element());
    const int writeHeight = TXml::GetTextInt(TiXmlHandle(in_sourceElement).FirstChildElement("destination").FirstChildElement("writeHeight").Element());

	TArrayPointerPackFontData arrayData;
	{
		TPointerPackFontData pointerData = PackFontData::FactorySpace(spaceWidth);
		if (pointerData)
		{
			arrayData.push_back(pointerData);
		}
	}

	TiXmlElement* pTraceElement = TiXmlHandle(in_sourceElement).FirstChildElement("fontDataArray").FirstChildElement("fontData").Element();
	while (pTraceElement)
	{
		TPointerPackFontData pointerData = PackFontData::Factory(
			*pTraceElement,
			in_sourcePath,
			inout_message
			);

		if (pointerData)
		{
			arrayData.push_back(pointerData);
		}

		pTraceElement = pTraceElement->NextSiblingElement("fontData");
	}

	LocalPackFontData(
		arrayData,
		tagetTexture,
		targetWidth,
		targetHeight,
		writeOriginX,
		writeOriginY,
		writeWidth,
		writeHeight,
		inout_message
		);

	//const float mLineVerticalPad;
	TXmlPack::PutR32(in_destinationElement, (float)newLine); 
	//const int mDataCount;
	TXmlPack::PutU32(in_destinationElement, arrayData.size()); 
	//const GGuiFontDataLoad* const mArrayData;
	{
		TiXmlElement pointerElement("pointer");
		for (TArrayPointerPackFontData::iterator iterator = arrayData.begin(); iterator != arrayData.end(); ++iterator)
		{
			const PackFontData& data = *(*iterator);
			//const unsigned int mID;
			TXmlPack::PutU32(&pointerElement, data.GetID()); 
			//const GVector2Float mUVTopLeft;
			TXmlPack::PutR32(&pointerElement, ((float)(data.GetUVLeft() + writeOriginX)) / ((float)targetWidth)); 
			TXmlPack::PutR32(&pointerElement, 1.0F - ((float)(data.GetUVTop() + writeOriginY)) / ((float)targetHeight)); 
			//const GVector2Float mUVBottomRight;
			TXmlPack::PutR32(&pointerElement, ((float)(data.GetUVRight() + writeOriginX)) / ((float)targetWidth)); 
			TXmlPack::PutR32(&pointerElement, 1.0F - ((float)(data.GetUVBottom() + writeOriginY)) / ((float)targetHeight)); 

			//const GVector2Float mSize;
			TXmlPack::PutR32(&pointerElement, (float)data.GetWidth()); 
			TXmlPack::PutR32(&pointerElement, (float)data.GetHeight()); 

			//const float mWaterLine; //distance from top to common line (waterline) of font
			TXmlPack::PutR32(&pointerElement, data.GetBaseLine()); 
			//const int mFlag;
			TXmlPack::PutU32(&pointerElement, data.GetFlag()); 

		}
		in_destinationElement->InsertEndChild(pointerElement);
	}

	return true;
}
