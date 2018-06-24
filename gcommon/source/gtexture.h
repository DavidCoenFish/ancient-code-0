//  gtexture.h

#ifndef _gtexture_h_
#define _gtexture_h_

#include <boost/noncopyable.hpp>
#include "gcommon.h"
#include "gcolour4float.h"
#include "gtexturetype.h"

/*
the 'image' of data to cast memory as after loading from tools output file

todo: TMagnifyFilter is redundant to TMipmapMagnifyFilter and TMipmapMinifyFilter
*/
class GTexture : public boost::noncopyable
{
	////////////////////////////////////////////////////
	//constructor
public:
	GTexture(
		const GU32 in_width,
		const GU32 in_height,
		const GTextureType::TType::TEnum in_type,
		const GVOID* const in_data,
		const GTextureType::TMagnifyFilter::TEnum in_magnifyFilter = GTextureType::TMagnifyFilter::TLinear,
		const GTextureType::TWrapMode::TEnum in_wrapMode = GTextureType::TWrapMode::TRepeat,
		const GTextureType::TMipmapMagnifyFilter::TEnum in_mipmapMagnifyFilter = GTextureType::TMipmapMagnifyFilter::TLinear,
		const GTextureType::TMipmapMinifyFilter::TEnum in_mipmapMinifyFilter = GTextureType::TMipmapMinifyFilter::TNearestMipmapLinear
		);
	~GTexture();

	////////////////////////////////////////////////////
	//public accessors
public:
	const GU32 GetWidth()const{ return m_width; }
	const GU32 GetHeight()const{ return m_height; }
	const GTextureType::TType::TEnum GetType()const;
	const GVOID* const GetData()const{ return m_data; }
	const GTextureType::TMagnifyFilter::TEnum GetMagnifyFilter()const;
	const GTextureType::TWrapMode::TEnum GetWrapMode()const;
	const GTextureType::TMipmapMagnifyFilter::TEnum GetMipmapMagnifyFilter()const;
	const GTextureType::TMipmapMinifyFilter::TEnum GetMipmapMinifyFilter()const;

	////////////////////////////////////////////////////
	//private members;
private:
	const GU32 m_width;
	const GU32 m_height;
	const GU32 m_flag;
	const GVOID* const m_data;

};

#endif //_gtexture_h_