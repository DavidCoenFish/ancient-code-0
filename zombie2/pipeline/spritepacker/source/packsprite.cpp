//file: PackSprite.cpp

#include "PackSprite.h"

#include "main.h"
#include "Image.h"
#include "SpriteData.h"
#include "ImageWrittenUV.h"

#include <TXml.h>
#include <TXmlPack.h>
#include <TCommon.h>
#include <tinyxml.h>
#include <map>
#include <TPackerMessage.h>
#include <FreeImage.h>

typedef boost::shared_ptr<Image> TPointerImage;
typedef boost::shared_ptr<SpriteData> TPointerSpriteData;

typedef std::vector<TPointerImage> TArrayPointerImage;
typedef std::vector<TPointerSpriteData> TArrayPointerSpriteData;
typedef boost::shared_ptr<ImageWrittenUV> TPointerImageWrittenUV;
typedef std::map<Image*, TPointerImageWrittenUV> TMapImageImageWrittenUV;

static void LocalGathSpriteSource(
	TArrayPointerImage& inout_arrayImage,
	TArrayPointerSpriteData& inout_arraySpriteData,
	TiXmlElement& in_spriteSourceElement
	)
{
	const std::string name = TXml::GetTextString(TiXmlHandle(&in_spriteSourceElement).FirstChildElement("name").Element());
	const std::string filePath = TXml::GetTextString(TiXmlHandle(&in_spriteSourceElement).FirstChildElement("file").Element());
	const std::string filePathAlpha = TXml::GetTextString(TiXmlHandle(&in_spriteSourceElement).FirstChildElement("fileAlpha").Element());

	printf("%s\n", name.c_str());
	return;
}

static const bool LocalPackSpriteImage(
	TPointerImage& inout_targetImage,
	TMapImageImageWrittenUV& inout_mapWrittenImage,
	const TArrayPointerImage& in_arrayImage
	)
{
	return true;
}

static void LocalWriteElement(
	TiXmlElement* const in_destinationElement,
	const TMapImageImageWrittenUV& in_mapWrittenImage,
	const TArrayPointerSpriteData& in_arraySpriteData
	)
{
	int count = 0;
	TiXmlElement pointerElement("pointer");
	for (TArrayPointerSpriteData::const_iterator iterator = in_arraySpriteData.begin(); iterator != in_arraySpriteData.end(); ++iterator)
	{
		const SpriteData& spriteData = *(*iterator);
		TMapImageImageWrittenUV::const_iterator found = in_mapWrittenImage.find(spriteData.GetImage().lock().get());
		if (found == in_mapWrittenImage.end())
		{
			continue;
		}

		count += 1;

		spriteData.Serialise(pointerElement, *found->second);
	}

	TXmlPack::PutU32(in_destinationElement, count); 
	in_destinationElement->InsertEndChild(pointerElement);
}


static void LocalSaveImage(
	TPointerImage& inout_targetImage,
	const std::string& in_filePath,
	TPackerMessage& inout_message
	)
{
	if (!inout_targetImage)
	{
		return;
	}
	Image& srcImage = *inout_targetImage;

	const FREE_IMAGE_FORMAT format = FreeImage_GetFIFFromFilename(in_filePath.c_str());
	FIBITMAP* pImage = FreeImage_AllocateT(FIT_BITMAP, srcImage.GetWidth(), srcImage.GetHeight(), 32);

	//blit image
	Image::BlitFreeImage(
		*pImage,
		srcImage,
		0,
		0,
		srcImage.GetWidth(),
		srcImage.GetHeight()
		);

	if (!FreeImage_Save(format, pImage, in_filePath.c_str()))
	{
		inout_message.AddError(std::string("Failed to save image: ") + in_filePath);
	}

	FreeImage_Unload(pImage);

	return;

}

/*static*/ const bool PackSprite::Pack(
	TiXmlElement* const in_sourceElement,
	TiXmlElement* const in_destinationElement,
	TPackerMessage& inout_message
	)
{
	if ((!in_sourceElement) || 
		(!in_destinationElement))
	{
		return false;
	}

	//gather destination data
	const std::string destinationImageFilePath = TXml::GetTextString(TiXmlHandle(in_sourceElement).FirstChildElement("destination").FirstChildElement("imageFile").Element());
	const int destinationImageWidth = TXml::GetTextInt(TiXmlHandle(in_sourceElement).FirstChildElement("destination").FirstChildElement("size").FirstChildElement("x").Element());
	const int destinationImageHeight = TXml::GetTextInt(TiXmlHandle(in_sourceElement).FirstChildElement("destination").FirstChildElement("size").FirstChildElement("y").Element());
	const float writeBoundsTop = TXml::GetTextFloat(TiXmlHandle(in_sourceElement).FirstChildElement("destination").FirstChildElement("writeBoundsTopLeft").FirstChildElement("x").Element());
	const float writeBoundsLeft = TXml::GetTextFloat(TiXmlHandle(in_sourceElement).FirstChildElement("destination").FirstChildElement("writeBoundsTopLeft").FirstChildElement("y").Element());
	const float writeBoundsBottom = TXml::GetTextFloat(TiXmlHandle(in_sourceElement).FirstChildElement("destination").FirstChildElement("writeBoundsBottomRight").FirstChildElement("x").Element());
	const float writeBoundsRight = TXml::GetTextFloat(TiXmlHandle(in_sourceElement).FirstChildElement("destination").FirstChildElement("writeBoundsBottomRight").FirstChildElement("y").Element());

	//gather sprite source data
	TArrayPointerImage arrayImage;
	TArrayPointerSpriteData arraySpriteData;
	TiXmlElement* trace = TiXmlHandle(in_sourceElement).FirstChildElement("arraySpriteSource").FirstChildElement("spriteSource").Element();
	while (trace)
	{
		LocalGathSpriteSource(
			arrayImage,
			arraySpriteData,			
			*trace);
		trace = trace->NextSiblingElement("spriteSource");
	}

	TPointerImage targetImage;
	targetImage.reset(new Image(destinationImageWidth, destinationImageHeight));

	//pack sprites
	TMapImageImageWrittenUV mapWrittenImage;
	LocalPackSpriteImage(
		targetImage,
		mapWrittenImage,
		arrayImage
		);

	//write destination element
	LocalWriteElement(
		in_destinationElement,
		mapWrittenImage,
		arraySpriteData
		);

	//save destination image
	LocalSaveImage(targetImage, destinationImageFilePath, inout_message);

	return true;
}