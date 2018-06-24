//file: SpriteData.cpp

#include "SpriteData.h"
#include "ImageWrittenUV.h"

#include <TXmlPack.h>
#include "ImageWrittenUV.h"

// construction
SpriteData::SpriteData(
	const std::string in_name,
	const TOrientation::TEnum in_orientation,
	TPointerImage& inout_image
	)
	: m_name(in_name)
	, m_orientation(in_orientation)
	, m_image(inout_image)
{
	return;
}

SpriteData::~SpriteData()
{
	return;
}

//public methods
void SpriteData::Serialise(TiXmlElement& inout_data, const ImageWrittenUV& in_writtenUV)const
{
	TXmlPack::PutString(&inout_data, m_name); 
	switch (m_orientation)
	{
	default:
	case TOrientation::T12:
	case TOrientation::T12Flip:
	case TOrientation::T03:
	case TOrientation::T03Flip:
	case TOrientation::T06:
	case TOrientation::T06Flip:
	case TOrientation::T09:
	case TOrientation::T09Flip:
		TXmlPack::PutR32(&inout_data, in_writtenUV.GetTopLeftU()); 
		TXmlPack::PutR32(&inout_data, in_writtenUV.GetTopLeftV()); 
		TXmlPack::PutR32(&inout_data, in_writtenUV.GetBottomRightU()); 
		TXmlPack::PutR32(&inout_data, in_writtenUV.GetTopLeftV()); 
		TXmlPack::PutR32(&inout_data, in_writtenUV.GetTopLeftU()); 
		TXmlPack::PutR32(&inout_data, in_writtenUV.GetBottomRightV()); 
		TXmlPack::PutR32(&inout_data, in_writtenUV.GetBottomRightU()); 
		TXmlPack::PutR32(&inout_data, in_writtenUV.GetBottomRightV()); 
		break;
	}

	return;
}
