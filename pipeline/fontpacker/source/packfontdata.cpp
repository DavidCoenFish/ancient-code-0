//file: PackFontData.cpp

#include "PackFontData.h"

#include <TPackerMessage.h>
#include <TXml.h>
#include <tinyxml.h>
#include <FreeImage.h>
#include "Image.h"

struct TGuiFontDataLoadFlag
{
	enum TEnum
	{
		TNone			= 0x00,
		TSupportAnsii	= 0x01, //first byte of the id can be used for ansi characters
		TVisible		= 0x02, //make mesh for this gliph
		TWordWrapBreak	= 0x04  //for wordwrap
	};
};

typedef std::vector<unsigned char> TArrayByte;

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

static void LocalPullDataOutOfImage(
	TArrayByte& out_arrayImageData,
	int& out_width,
	int& out_height,
	FIBITMAP& in_image
	)
{
	out_width = FreeImage_GetWidth(&in_image);
	out_height = FreeImage_GetHeight(&in_image);
	out_arrayImageData.resize(out_width * out_height * 4);
	for (int indexY = 0; indexY < out_height; ++indexY)
	{
		for (int indexX = 0; indexX < out_width; ++indexX)
		{
			RGBQUAD value;
			FreeImage_GetPixelColor(&in_image, indexX, indexY, &value);
			const int baseIndex = ((indexY * out_width) + indexX) * 4;
			out_arrayImageData[baseIndex + 0] = value.rgbRed;
			out_arrayImageData[baseIndex + 1] = value.rgbGreen;
			out_arrayImageData[baseIndex + 2] = value.rgbBlue;
			out_arrayImageData[baseIndex + 3] = value.rgbReserved;
		}
	}

	return;
}

// static public methods
/*static*/ PackFontData::TPointerPackFontData PackFontData::FactorySpace(
	const int in_spaceWidth
	)
{
	TPointerImage pointerImage;
	TPointerPackFontData pointerPackFontData;
	pointerPackFontData.reset(new PackFontData(
		(int)' ',
		0.0F,
		TGuiFontDataLoadFlag::TSupportAnsii | TGuiFontDataLoadFlag::TWordWrapBreak,
		pointerImage,
		in_spaceWidth,
		0
		));

	return pointerPackFontData;
}

/*static*/ PackFontData::TPointerPackFontData PackFontData::Factory(
	TiXmlElement& in_element,
	const std::string in_sourceFilePath,
	TPackerMessage& inout_message
	)
{
	const std::string filePath = TXml::GetTextString(TiXmlHandle(&in_element).FirstChildElement("filePath").Element());
    const int value = TXml::GetTextInt(TiXmlHandle(&in_element).FirstChildElement("value").Element());
    const int baseLine = TXml::GetTextInt(TiXmlHandle(&in_element).FirstChildElement("baseLine").Element());

	int flag = TGuiFontDataLoadFlag::TVisible;

	if (value < 256)
	{
		flag |= TGuiFontDataLoadFlag::TSupportAnsii;
	}

	const std::string localImagePath = LocalDoctorPath(filePath, in_sourceFilePath);
	const FREE_IMAGE_FORMAT format = FreeImage_GetFIFFromFilename(localImagePath.c_str());
	FIBITMAP* pImage = FreeImage_Load(format, localImagePath.c_str());
	if (!pImage)
	{
		inout_message.AddError(std::string("load failed:") + localImagePath);
		return TPointerPackFontData();
	}

	TPointerImage pointerImage = Image::FactoryFreeImage(*pImage);

	FreeImage_Unload(pImage);

	TPointerPackFontData pointerPackFontData;
	pointerPackFontData.reset(new PackFontData(
		value,
		(float)baseLine,
		flag,
		pointerImage,
		pointerImage->GetWidth(),
		pointerImage->GetHeight()
		));

	return pointerPackFontData;
}

PackFontData::PackFontData(
	const unsigned int in_ID,
	const float in_baseLine,
	const int in_flag,
	const TPointerImage& in_image,
	const int in_width,
	const int in_height
	)
	: mID(in_ID)
	, mBaseLine(in_baseLine)
	, mFlag(in_flag)
	, mImage(in_image)
	, mWidth(in_width)
	, mHeight(in_height)
	, mUVTop(0)
	, mUVLeft(0)
	, mUVBottom(0)
	, mUVRight(0)
{
	return;
}

PackFontData::~PackFontData()
{
	return;
}

// accessors
const bool PackFontData::GetIsVisible()const
{
	return (0 != (mFlag & TGuiFontDataLoadFlag::TVisible));
}
