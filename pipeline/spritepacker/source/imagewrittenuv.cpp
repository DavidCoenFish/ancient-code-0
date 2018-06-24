//file: ImageWrittenUV.cpp

#include "ImageWrittenUV.h"

// construction
ImageWrittenUV::ImageWrittenUV(
	const float in_topLeftU,
	const float in_topLeftV,
	const float in_bottomRightU,
	const float in_bottomRightV
	)
	: m_topLeftU(in_topLeftU)
	, m_topLeftV(in_topLeftV)
	, m_bottomRightU(in_bottomRightU)
	, m_bottomRightV(in_bottomRightV)
{
	return;
}

ImageWrittenUV::~ImageWrittenUV()
{
	return;
}