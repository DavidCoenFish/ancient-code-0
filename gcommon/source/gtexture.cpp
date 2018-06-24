//  gtexture.h

#include "gtexture.h"

////////////////////////////////////////////////////
//constructor
GTexture::GTexture(
	const GU32 in_width,
	const GU32 in_height,
	const GTextureType::TType::TEnum in_type,
	const GVOID* const in_data,
	const GTextureType::TMagnifyFilter::TEnum in_magnifyFilter,
	const GTextureType::TWrapMode::TEnum in_wrapMode,
	const GTextureType::TMipmapMagnifyFilter::TEnum in_mipmapMagnifyFilter,
	const GTextureType::TMipmapMinifyFilter::TEnum in_mipmapMinifyFilter
	)
	: m_width(in_width)
	, m_height(in_height)
	, m_flag(GTextureType::MakeFlag(in_type, in_magnifyFilter, in_wrapMode, in_mipmapMagnifyFilter, in_mipmapMinifyFilter))
	, m_data(in_data)
{
	return;
}

GTexture::~GTexture()
{
	return;
}

////////////////////////////////////////////////////
//public accessors
const GTextureType::TType::TEnum GTexture::GetType()const
{
	return GTextureType::GetTypeFromFlag(m_flag);
}

const GTextureType::TMagnifyFilter::TEnum GTexture::GetMagnifyFilter()const
{
	return GTextureType::GetMagnifyFilterFromFlag(m_flag);
}

const GTextureType::TWrapMode::TEnum GTexture::GetWrapMode()const
{
	return GTextureType::GetWrapModeFromFlag(m_flag);
}

const GTextureType::TMipmapMagnifyFilter::TEnum GTexture::GetMipmapMagnifyFilter()const
{
	return GTextureType::GetMipmapMagnifyFilterFromFlag(m_flag);
}

const GTextureType::TMipmapMinifyFilter::TEnum GTexture::GetMipmapMinifyFilter()const
{
	return GTextureType::GetMipmapMinifyFilterFromFlag(m_flag);
}

