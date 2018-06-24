//file: packTexture.cpp

#include "PackTexture.h"

//#include "PackCommon.h"
#include "Main.h"
#include "TexturePaletter.h"

#include <TXml.h>
#include <TXmlPack.h>
#include <TPackerMessage.h>
#include <TCommon.h>
#include <tinyxml.h>
#include <FreeImage.h>
#include <GTextureType.h>
#include <GWin32FileSystem.h>
#include <GBuffer.h>

typedef std::vector<unsigned char> TArrayByte;
typedef std::vector<RGBQUAD> TArrayRGBQUAD;

struct TTypeData
{
	const GTextureType::TType::TEnum mType;
	const std::string mName;
};

typedef unsigned int uint32_t;
struct TPVRTexHeader
{
    uint32_t headerLength;
    uint32_t height;
    uint32_t width;
    uint32_t numMipmaps;
    uint32_t flags;
    uint32_t dataLength;
    uint32_t bpp;
    uint32_t bitmaskRed;
    uint32_t bitmaskGreen;
    uint32_t bitmaskBlue;
    uint32_t bitmaskAlpha;
    uint32_t pvrTag;
    uint32_t numSurfs;
};

static const TTypeData sArrayTypeData[] =
{
	{ GTextureType::TType::TRgb, "TRgb" },
	{ GTextureType::TType::TRgba, "TRgba" }, //iphone working
	{ GTextureType::TType::TGreyscale, "TGreyscale" },
	//{ GTextureType::TType::TGreyscaleAlpha, "greyscaleAlpha" },
	{ GTextureType::TType::TPalette4_rgb8, "TPalette4_rgb8" }, //iphone working
	{ GTextureType::TType::TPalette4_rgba8, "TPalette4_rgba8" }, //iphone working
	{ GTextureType::TType::TPalette4_r5g6b5, "TPalette4_r5g6b5" }, //iphone working
	{ GTextureType::TType::TPalette4_rgba4, "TPalette4_rgba4" }, //iphone working
	{ GTextureType::TType::TPalette4_rgb5a1, "TPalette4_rgb5a1" }, //iphone working
	{ GTextureType::TType::TPalette8_rgb8, "TPalette8_rgb8" }, //iphone working
	{ GTextureType::TType::TPalette8_rgba8, "TPalette8_rgba8" }, //iphone working
	{ GTextureType::TType::TPalette8_r5g6b5, "TPalette8_r5g6b5" }, //iphone working
	{ GTextureType::TType::TPalette8_rgba4, "TPalette8_rgba4" }, //iphone working
	{ GTextureType::TType::TPalette8_rgb5a1, "TPalette8_rgb5a1" }, //iphone working

	{ GTextureType::TType::TPVRTC4_rgb, "TPVRTC4_rgb" },
	{ GTextureType::TType::TPVRTC2_rgb, "TPVRTC2_rgb" },
	{ GTextureType::TType::TPVRTC4_rgba, "TPVRTC4_rgba" },
	{ GTextureType::TType::TPVRTC2_rgba, "TPVRTC2_rgba" }
};

static const GTextureType::TType::TEnum LocalGetType(const std::string& in_name)
{
	for (int index = 0; index < TCOMMON_ARRAY_SIZE(sArrayTypeData); ++index)
	{
		if (in_name == sArrayTypeData[index].mName)
		{
			return sArrayTypeData[index].mType;
		}
	}
	return GTextureType::TType::TCount;
}

static const std::string LocalGetBestMatch(
	TiXmlElement* const in_sourceElement, 
	const std::string& in_elementName,
	const std::string& in_platform
	)
{
	TiXmlElement* pTrace = TiXmlHandle(in_sourceElement).FirstChildElement(in_elementName).Element();
	std::string defaultValue;
	while (pTrace)
	{
		const std::string value = TXml::GetTextString(pTrace);
		const std::string platform = TXml::GetAttributeString(pTrace, "platform");
		if (in_platform == platform)
		{
			return value;
		}

		//the element without platform attribute is the default
		if (platform.empty())
		{
			defaultValue = value;
		}

		pTrace = pTrace->NextSiblingElement(in_elementName);
	}

	return defaultValue;
}


static void LocalCollectDataRgba(
	TArrayByte& out_pixelData,
	FIBITMAP& pImage,
	const U32 in_width,
	const U32 in_height
	)
{
	FIBITMAP* const localImage = FreeImage_ConvertTo32Bits(&pImage);

	TArrayByte pixelData;
	//DLL_API BOOL DLL_CALLCONV FreeImage_GetPixelColor(FIBITMAP *dib, unsigned x, unsigned y, RGBQUAD *value);
	for (int indexY = 0; indexY < (int)in_height; ++indexY)
	{
		for (int indexX = 0; indexX < (int)in_width; ++indexX)
		{
			RGBQUAD rgbquad;
			FreeImage_GetPixelColor(&pImage, indexX, indexY, &rgbquad);
			out_pixelData.push_back(rgbquad.rgbBlue);
			out_pixelData.push_back(rgbquad.rgbGreen);
			out_pixelData.push_back(rgbquad.rgbRed);
			out_pixelData.push_back(rgbquad.rgbReserved);
		}
	}

	FreeImage_Unload(localImage);

	return;
}

static void LocalCollectDataRgb(
	TArrayByte& out_arrayByteData,
	FIBITMAP& pImage,
	const U32 in_width,
	const U32 in_height
	)
{
	FIBITMAP* const localImage = FreeImage_ConvertTo32Bits(&pImage);

	//DLL_API BOOL DLL_CALLCONV FreeImage_GetPixelColor(FIBITMAP *dib, unsigned x, unsigned y, RGBQUAD *value);
	for (int indexY = 0; indexY < (int)in_height; ++indexY)
	{
		for (int indexX = 0; indexX < (int)in_width; ++indexX)
		{
			RGBQUAD rgbquad;
			FreeImage_GetPixelColor(&pImage, indexX, indexY, &rgbquad);
			out_arrayByteData.push_back(rgbquad.rgbBlue);
			out_arrayByteData.push_back(rgbquad.rgbGreen);
			out_arrayByteData.push_back(rgbquad.rgbRed);
		}
	}

	FreeImage_Unload(localImage);

	return;
}

static void LocalCollectDataGreyscale(
	TArrayByte& out_arrayByteData,
	FIBITMAP& pImage,
	const U32 in_width,
	const U32 in_height
	)
{
	FIBITMAP* const localImage = FreeImage_ConvertToGreyscale(&pImage);

	//DLL_API BOOL DLL_CALLCONV FreeImage_GetPixelColor(FIBITMAP *dib, unsigned x, unsigned y, RGBQUAD *value);
	for (int indexY = 0; indexY < (int)in_height; ++indexY)
	{
		for (int indexX = 0; indexX < (int)in_width; ++indexX)
		{
			RGBQUAD rgbquad;
			FreeImage_GetPixelColor(&pImage, indexX, indexY, &rgbquad);
			out_arrayByteData.push_back(((unsigned char*)&rgbquad)[0]);
		}
	}

	FreeImage_Unload(localImage);

	return;
}

static void LocalCollectPixelIndex4(
	TArrayByte& out_arrayDataBlock,
	FIBITMAP& pImage,
	const U32 in_width,
	const U32 in_height
	)
{
	for (int indexY = 0; indexY < (int)in_height; ++indexY)
	{
		for (int indexX = 0; indexX < (int)in_width; indexX += 2)
		{
			unsigned char pixelIndex = 0;
			unsigned char pixelIndexA = 0;
			unsigned char pixelIndexB = 0;
			FreeImage_GetPixelIndex(&pImage, indexX, indexY, &pixelIndexA);
			FreeImage_GetPixelIndex(&pImage, indexX + 1, indexY, &pixelIndexB);
			pixelIndex = (pixelIndexA << 4) | pixelIndexB;
			out_arrayDataBlock.push_back(pixelIndex);
		}
	}

	return;
}

static void LocalCollectPixelIndex8(
	TArrayByte& out_arrayDataBlock,
	FIBITMAP& pImage,
	const U32 in_width,
	const U32 in_height
	)
{
	for (int indexY = 0; indexY < (int)in_height; ++indexY)
	{
		for (int indexX = 0; indexX < (int)in_width; ++indexX)
		{
			unsigned char pixelIndex = 0;
			FreeImage_GetPixelIndex(&pImage, indexX, indexY, &pixelIndex);
			out_arrayDataBlock.push_back(pixelIndex);
		}
	}

	return;
}


static void LocalAppendPixelIndex(
	TArrayByte& out_dataArray,
	const TArrayByte& in_arrayPixelIndex,
	const bool in_4bit
	)
{
	if (in_4bit)
	{
		for (unsigned int index = 0; index < in_arrayPixelIndex.size(); index += 2)
		{
			const unsigned char pixelIndexA = in_arrayPixelIndex[index];
			const unsigned char pixelIndexB = in_arrayPixelIndex[index + 1];
			const unsigned char pixelIndex = ((pixelIndexA << 4) & 0xF0) | (pixelIndexB & 0x0F);
			out_dataArray.push_back(pixelIndex);
		}
	}
	else
	{
		out_dataArray.insert(out_dataArray.end(), in_arrayPixelIndex.begin(), in_arrayPixelIndex.end());
	}
	return;
}

static void LocalCollectPaletteRgb(
	TArrayByte& out_arrayDataBlock,
	FIBITMAP& pImage,
	const U32 in_paletteColourCount
	)
{
	RGBQUAD* const palette = FreeImage_GetPalette(&pImage);
	for (unsigned int index = 0; index < in_paletteColourCount; ++index)
	{
		RGBQUAD& rgbquad = palette[index];
		out_arrayDataBlock.push_back(rgbquad.rgbRed);
		out_arrayDataBlock.push_back(rgbquad.rgbGreen);
		out_arrayDataBlock.push_back(rgbquad.rgbBlue);
	}

	return;
}
static void LocalCollectPaletteRgba(
	TArrayByte& out_arrayDataBlock,
	FIBITMAP& pImage,
	const U32 in_paletteColourCount
	)
{
	RGBQUAD* const palette = FreeImage_GetPalette(&pImage);
	for (unsigned int index = 0; index < in_paletteColourCount; ++index)
	{
		RGBQUAD& rgbquad = palette[index];
		out_arrayDataBlock.push_back(rgbquad.rgbRed);
		out_arrayDataBlock.push_back(rgbquad.rgbGreen);
		out_arrayDataBlock.push_back(rgbquad.rgbBlue);
		out_arrayDataBlock.push_back(rgbquad.rgbReserved);
	}

	return;
}
static void LocalCollectPaletteR5g6b5(
	TArrayByte& out_arrayDataBlock,
	FIBITMAP& pImage,
	const U32 in_paletteColourCount
	)
{
	RGBQUAD* const palette = FreeImage_GetPalette(&pImage);
	for (unsigned int index = 0; index < in_paletteColourCount; ++index)
	{
		RGBQUAD& rgbquad = palette[index];
		const unsigned short value = (((rgbquad.rgbRed>>3)&0x1F) << 11) | (((rgbquad.rgbGreen>>2)& 0x3F) << 5) | ((rgbquad.rgbBlue>>3) & 0x1F);
		out_arrayDataBlock.push_back(((unsigned char*)&value)[0]);
		out_arrayDataBlock.push_back(((unsigned char*)&value)[1]);
	}

	return;
}

static void LocalCollectDataPaletteRgb8(
	TArrayByte& out_arrayByteData,
	FIBITMAP& pImage,
	const U32 in_width,
	const U32 in_height,
	const int in_paletteColourCount,
	TPackerMessage& inout_message
	)
{
	FIBITMAP* const localImage24 = FreeImage_ConvertTo24Bits(&pImage);
	FIBITMAP* const localImagePal = FreeImage_ColorQuantizeEx(localImage24, FIQ_WUQUANT, in_paletteColourCount);

	if (!localImagePal)
	{
		inout_message.AddError("palette failed");
		return;
	}

	LocalCollectPaletteRgb(
		out_arrayByteData,
		*localImagePal,
		in_paletteColourCount
		);

	if (16 == in_paletteColourCount)
	{
		LocalCollectPixelIndex4(
			out_arrayByteData,
			*localImagePal,
			in_width,
			in_height
			);
	}
	else
	{
		LocalCollectPixelIndex8(
			out_arrayByteData,
			*localImagePal,
			in_width,
			in_height
			);
	}

	FreeImage_Unload(localImagePal);
	FreeImage_Unload(localImage24);
	return;
}
static void LocalAppendPaletteRgba8(
	TArrayByte& out_dataArray,
	const TArrayRGBQUAD& in_arrayPalette
	)
{
	for (unsigned int index = 0; index < in_arrayPalette.size(); ++index)
	{
		const RGBQUAD& rgbquad = in_arrayPalette[index];
		out_dataArray.push_back(rgbquad.rgbRed);
		out_dataArray.push_back(rgbquad.rgbGreen);
		out_dataArray.push_back(rgbquad.rgbBlue);
		out_dataArray.push_back(rgbquad.rgbReserved);
	}
	return;
}

static void LocalCollectDataPaletteRgba8(
	TArrayByte& out_arrayByteData,
	FIBITMAP& pImage,
	const U32 in_width,
	const U32 in_height,
	const int in_paletteColourCount
	)
{
	FIBITMAP* const localImage32 = FreeImage_ConvertTo32Bits(&pImage);
	TArrayRGBQUAD arrayPalette;
	TArrayByte arrayPixelIndex;
	texturePaletter::Run(
		arrayPalette,
		arrayPixelIndex,
		pImage,
		in_paletteColourCount
		);

	LocalAppendPaletteRgba8(
		out_arrayByteData,
		arrayPalette
		);

	LocalAppendPixelIndex(
		out_arrayByteData,
		arrayPixelIndex,
		(16 == in_paletteColourCount)
		);


	FreeImage_Unload(localImage32);
	return;
}

static void LocalCollectDataPaletteR5g6b5(
	TArrayByte& out_arrayByteData,
	FIBITMAP& pImage,
	const U32 in_width,
	const U32 in_height,
	const int in_paletteColourCount,
	TPackerMessage& inout_message
	)
{
	FIBITMAP* const localImage24 = FreeImage_ConvertTo24Bits(&pImage);
	FIBITMAP* const localImagePal = FreeImage_ColorQuantizeEx(localImage24, FIQ_WUQUANT, in_paletteColourCount);

	if (!localImagePal)
	{
		inout_message.AddError("palette failed\n");
		return;
	}

	TArrayByte dataArray;

	LocalCollectPaletteR5g6b5(
		out_arrayByteData,
		*localImagePal,
		in_paletteColourCount
		);

	if (16 == in_paletteColourCount)
	{
		LocalCollectPixelIndex4(
			out_arrayByteData,
			*localImagePal,
			in_width,
			in_height
			);
	}
	else
	{
		LocalCollectPixelIndex8(
			out_arrayByteData,
			*localImagePal,
			in_width,
			in_height
			);
	}

	FreeImage_Unload(localImagePal);
	FreeImage_Unload(localImage24);
	return;
}

static void LocalAppendPaletteRgba4(
	TArrayByte& out_dataArray,
	const TArrayRGBQUAD& in_arrayPalette
	)
{
	for (unsigned int index = 0; index < in_arrayPalette.size(); ++index)
	{
		const RGBQUAD& rgbquad = in_arrayPalette[index];
		out_dataArray.push_back((rgbquad.rgbBlue & 0xF0) | ((rgbquad.rgbReserved >> 4) & 0x0F));
		out_dataArray.push_back((rgbquad.rgbRed & 0xF0) | ((rgbquad.rgbGreen>>4) & 0x0F));
	}
	return;
}

static void LocalCollectDataPaletteRgba4(
	TArrayByte& out_arrayByteData,
	FIBITMAP& pImage,
	const U32 in_width,
	const U32 in_height,
	const int in_paletteColourCount
	)
{
	FIBITMAP* const localImage32 = FreeImage_ConvertTo32Bits(&pImage);
	TArrayRGBQUAD arrayPalette;
	TArrayByte arrayPixelIndex;
	texturePaletter::Run(
		arrayPalette,
		arrayPixelIndex,
		pImage,
		in_paletteColourCount
		);


	LocalAppendPaletteRgba4(
		out_arrayByteData,
		arrayPalette
		);

	LocalAppendPixelIndex(
		out_arrayByteData,
		arrayPixelIndex,
		(16 == in_paletteColourCount)
		);

	FreeImage_Unload(localImage32);
	return;
}

static void LocalAppendPaletteRgb5a1(
	TArrayByte& out_dataArray,
	const TArrayRGBQUAD& in_arrayPalette
	)
{
	for (unsigned int index = 0; index < in_arrayPalette.size(); ++index)
	{
		const RGBQUAD& rgbquad = in_arrayPalette[index];
		const unsigned short value = (((rgbquad.rgbRed>>3)&0x1F) << 11) | (((rgbquad.rgbGreen>>3)& 0x1F) << 6) | (((rgbquad.rgbBlue>>3) & 0x1F) << 1) | ((127 < rgbquad.rgbReserved)? 1 : 0);
		out_dataArray.push_back(((unsigned char*)&value)[0]);
		out_dataArray.push_back(((unsigned char*)&value)[1]);
	}
}

static void LocalCollectDataPaletteRgb5a1(
	TArrayByte& out_arrayByteData,
	FIBITMAP& pImage,
	const U32 in_width,
	const U32 in_height,
	const int in_paletteColourCount
	)
{
	FIBITMAP* const localImage32 = FreeImage_ConvertTo32Bits(&pImage);
	TArrayRGBQUAD arrayPalette;
	TArrayByte arrayPixelIndex;
	texturePaletter::Run(
		arrayPalette,
		arrayPixelIndex,
		pImage,
		in_paletteColourCount
		);

	LocalAppendPaletteRgb5a1(
		out_arrayByteData,
		arrayPalette
		);

	LocalAppendPixelIndex(
		out_arrayByteData,
		arrayPixelIndex,
		(16 == in_paletteColourCount)
		);

	FreeImage_Unload(localImage32);
	return;
}

static void LocalCollectData(
	TArrayByte& out_arrayByteData,
	FIBITMAP& pImage,
	const U32 in_width,
	const U32 in_height,
	const GTextureType::TType::TEnum in_type,
	TPackerMessage& inout_message
	)
{
	switch (in_type)
	{
	default:
		inout_message.AddError("unsuported image destination type");
		break;
	case GTextureType::TType::TRgba:
		LocalCollectDataRgba(
			out_arrayByteData,
			pImage,
			in_width,
			in_height
			);
		break;
	case GTextureType::TType::TRgb:
		LocalCollectDataRgb(
			out_arrayByteData,
			pImage,
			in_width,
			in_height
			);
		break;
	case GTextureType::TType::TGreyscale:
		LocalCollectDataGreyscale(
			out_arrayByteData,
			pImage,
			in_width,
			in_height
			);
		break;
	//case GTextureType::TType::TGreyscaleAlpha:
	//	LocalCollectDataGreyscaleAlpha(
	//		out_pointerDataBlock,
	//		out_dataByteSize,
	//		out_dataOffset,
	//		pImage,
	//		in_width,
	//		in_height
	//		);
	//	break;
	case GTextureType::TType::TPalette4_rgb8:
		LocalCollectDataPaletteRgb8(
			out_arrayByteData,
			pImage,
			in_width,
			in_height,
			16,
			inout_message
			);
		break;
	case GTextureType::TType::TPalette4_rgba8:
		LocalCollectDataPaletteRgba8(
			out_arrayByteData,
			pImage,
			in_width,
			in_height,
			16
			);
		break;
	case GTextureType::TType::TPalette4_r5g6b5:
		LocalCollectDataPaletteR5g6b5(
			out_arrayByteData,
			pImage,
			in_width,
			in_height,
			16,
			inout_message
			);
		break;
	case GTextureType::TType::TPalette4_rgba4:
		LocalCollectDataPaletteRgba4(
			out_arrayByteData,
			pImage,
			in_width,
			in_height,
			16
			);
		break;
	case GTextureType::TType::TPalette4_rgb5a1:
		LocalCollectDataPaletteRgb5a1(
			out_arrayByteData,
			pImage,
			in_width,
			in_height,
			16
			);
		break;
	case GTextureType::TType::TPalette8_rgb8:
		LocalCollectDataPaletteRgb8(
			out_arrayByteData,
			pImage,
			in_width,
			in_height,
			256,
			inout_message
			);
		break;
	case GTextureType::TType::TPalette8_rgba8:
		LocalCollectDataPaletteRgba8(
			out_arrayByteData,
			pImage,
			in_width,
			in_height,
			256
			);
		break;
	case GTextureType::TType::TPalette8_r5g6b5:
		LocalCollectDataPaletteR5g6b5(
			out_arrayByteData,
			pImage,
			in_width,
			in_height,
			256,
			inout_message
			);
		break;
	case GTextureType::TType::TPalette8_rgba4:
		LocalCollectDataPaletteRgba4(
			out_arrayByteData,
			pImage,
			in_width,
			in_height,
			256
			);
		break;
	case GTextureType::TType::TPalette8_rgb5a1:
		LocalCollectDataPaletteRgb5a1(
			out_arrayByteData,
			pImage,
			in_width,
			in_height,
			256
			);
		break;
	}

	return;
}

static void LocalLoadAlphaIntoImage(
	FIBITMAP*& inout_image,
	const std::string& in_textureAlphaNamePath,
	TPackerMessage& inout_message
	)
{
	const FREE_IMAGE_FORMAT format = FreeImage_GetFIFFromFilename(in_textureAlphaNamePath.c_str());
	FIBITMAP* const pImageAlpha = FreeImage_Load(format, in_textureAlphaNamePath.c_str());
	FIBITMAP* const pAlphaGreyScale = FreeImage_ConvertToGreyscale(pImageAlpha);
	FIBITMAP* const pAlphaGreyScale24 = FreeImage_ConvertTo24Bits(pAlphaGreyScale);

	if (!pImageAlpha)
	{
		inout_message.AddError(std::string("load alpha texture failed") + in_textureAlphaNamePath);
		return;
	}

	const int width = FreeImage_GetWidth(inout_image);
	const int height = FreeImage_GetHeight(inout_image);
	const int widthAlpha = FreeImage_GetWidth(pImageAlpha);
	const int heightAlpha = FreeImage_GetHeight(pImageAlpha);

	if ((width != widthAlpha) || (height != heightAlpha))
	{
		inout_message.AddError(std::string("alpha texture failed size match") + in_textureAlphaNamePath);
		return;
	}

	FIBITMAP* const localImage32 = FreeImage_ConvertTo32Bits(inout_image);
	FreeImage_Unload(inout_image);
	inout_image = localImage32;

	for (int indexY = 0; indexY < height; ++indexY)
	{
		for (int indexX = 0; indexX < width; ++indexX)
		{
			RGBQUAD alphaPixel;
			RGBQUAD texturePixel;
			
			FreeImage_GetPixelColor(pAlphaGreyScale24, indexX, indexY, &alphaPixel);
			FreeImage_GetPixelColor(inout_image, indexX, indexY, &texturePixel);

			texturePixel.rgbReserved = alphaPixel.rgbRed;

			FreeImage_SetPixelColor(inout_image, indexX, indexY, &texturePixel);
		}
	}
	FreeImage_Unload(pAlphaGreyScale24);
	FreeImage_Unload(pAlphaGreyScale);
	FreeImage_Unload(pImageAlpha);

	//FreeImage_Save(FIF_PNG, inout_image, "debug_alpha.png");

	return;
}

static void LocalDealPVRTC(
	TArrayByte& out_arrayByteData,
	U32& out_width,
	U32& out_height,
	const std::string& in_textureNamePath,
	const std::string& in_textureAlphaNamePath,
	const GTextureType::TType::TEnum in_type,
	TPackerMessage& inout_message
	)
{
	GWin32FileSystem fileSystem;
	GBuffer buffer;
	if (!fileSystem.LoadFile(
		buffer,
		in_textureNamePath
		))
	{
		inout_message.AddError(std::string("Load source failed:") + in_textureNamePath, true);
		return;
	}

	const TPVRTexHeader& pvrtcHeader = *(TPVRTexHeader*)buffer.GetData();
	pvrtcHeader;

	out_width = pvrtcHeader.width;
	out_height = pvrtcHeader.height;
	const unsigned char* const pTrace = buffer.GetData();
	const int count = buffer.GetCount();
	out_arrayByteData.resize(count - pvrtcHeader.headerLength);
	for (int index = 0; index < (int)(count - pvrtcHeader.headerLength); ++index)
	{
		out_arrayByteData[index] = pTrace[index + pvrtcHeader.headerLength];
	}

	return;
}

static void LocalLoadImage(
	TArrayByte& out_arrayByteData,
	U32& out_width,
	U32& out_height,
	const std::string& in_textureNamePath,
	const std::string& in_textureAlphaNamePath,
	const GTextureType::TType::TEnum in_type,
	TPackerMessage& inout_message
	)
{
	if ((GTextureType::TType::TPVRTC2_rgb == in_type) ||
		(GTextureType::TType::TPVRTC4_rgb == in_type) ||
		(GTextureType::TType::TPVRTC2_rgba == in_type) ||
		(GTextureType::TType::TPVRTC4_rgba == in_type))
	{
		LocalDealPVRTC(
			out_arrayByteData,
			out_width,
			out_height,
			in_textureNamePath,
			in_textureAlphaNamePath,
			in_type,
			inout_message
			);
		return;
	}

	const FREE_IMAGE_FORMAT format = FreeImage_GetFIFFromFilename(in_textureNamePath.c_str());
	FIBITMAP* pImage = FreeImage_Load(format, in_textureNamePath.c_str());
	if (!pImage)
	{
		inout_message.AddError(std::string("texture failed:") + in_textureNamePath);
	}

	if (pImage && !in_textureAlphaNamePath.empty())
	{
		LocalLoadAlphaIntoImage(
			pImage,
			in_textureAlphaNamePath,
			inout_message
			);
	}

	out_width = FreeImage_GetWidth(pImage);
	out_height = FreeImage_GetHeight(pImage);

	LocalCollectData(
		out_arrayByteData,
		*pImage,
		out_width,
		out_height,
		in_type,
		inout_message
		);

	FreeImage_Unload(pImage);

	return;
};

static const bool LocalGetParam(
	std::string& out_textureNamePath, 
	std::string& out_textureAlphaNamePath, 
	GTextureType::TType::TEnum& out_type,
	std::string& out_typeString,
	TiXmlElement* const in_sourceElement,
	const std::string& in_dataName,
	const std::string& in_platform,
	TPackerMessage& inout_message		
	)
{
	if (in_dataName == TXml::GetTextString(TiXmlHandle(in_sourceElement).FirstChildElement("textureDataName").Element()))
	{
		out_textureNamePath = LocalGetBestMatch(in_sourceElement, "textureFile", in_platform);
		out_textureAlphaNamePath = LocalGetBestMatch(in_sourceElement, "textureAlphaFile", in_platform);
		out_typeString = LocalGetBestMatch(in_sourceElement, "textureDestinationType", in_platform);
		if (out_typeString.empty())
		{
			out_typeString = "TRgba";
		}

		out_type = LocalGetType(out_typeString);

		return true;
	}

	TiXmlElement* pTrace = TiXmlHandle(in_sourceElement).FirstChildElement().Element();
	while (pTrace)
	{
		if (LocalGetParam(
				out_textureNamePath, 
				out_textureAlphaNamePath, 
				out_type,
				out_typeString,
				pTrace,
				in_dataName,
				in_platform,
				inout_message		
				))
		{
			return true;
		}

		pTrace = pTrace->NextSiblingElement();
	}

	return false;
}

/*
struct TLoadTextureData
{
	unsigned int mWidth;
	unsigned int mHeight;
	unsigned int mTypeStringOffset;
	unsigned int mDataOffset;
};
*/
//expect to find from source element, the [source texture file/ source texture and alpha file] and the desired type
//we write to the set the width, height, typeString and data block of the texture
/*static*/ const bool PackTexture::Pack(
	TiXmlElement* const in_sourceElement,
	TiXmlElement* const in_destinationElement,
	const std::string& in_dataName,
	const std::string& in_platform,
	TPackerMessage& inout_message
	)
{
	if ((!in_sourceElement) || (!in_destinationElement))
	{
		return false;
	}

	unsigned int width = 0;
	unsigned int height = 0;
	TArrayByte arrayByteData;

	std::string textureNamePath;
	std::string textureAlphaNamePath;
	GTextureType::TType::TEnum type;
	std::string typeString;

	if (LocalGetParam(textureNamePath, textureAlphaNamePath, type, typeString, in_sourceElement, in_dataName, in_platform, inout_message))
	{
		LocalLoadImage(
			arrayByteData,
			width,
			height,
			textureNamePath,
			textureAlphaNamePath,
			type,
			inout_message
			);
	}
	else
	{
		inout_message.AddError(std::string("Did not find texture source:") + in_dataName);
	}

	//write destination xml
	TXmlPack::PutU32(in_destinationElement, width);
	TXmlPack::PutU32(in_destinationElement, height);
	TXmlPack::PutU32(in_destinationElement, type);
	TiXmlElement pointerElement("pointer");
	TXmlPack::PutArrayU8(&pointerElement, arrayByteData);
	in_destinationElement->InsertEndChild(pointerElement);

	return true;
}
