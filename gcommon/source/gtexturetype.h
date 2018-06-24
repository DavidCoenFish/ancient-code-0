//  gtexturetype.h

#ifndef _gtexturetype_h_
#define _gtexturetype_h_

#include "gcommon.h"

/*
don't use openGL constants (ie, 0x2601 for GL_LINEAR as sooner or latter, some implementation could be a different value)
*/
class GTextureType
{
	//////////////////////////////////////////////
	//typedef
public:
	struct TType
	{
		enum TEnum
		{
			TRgba = 0, //default
			TRgb, //no iphone support
			TGreyscale, //no iphone support
			TGreyscaleAlpha, //no iphone support
			TPalette4_rgb8,
			TPalette4_rgba8,
			TPalette4_r5g6b5,
			TPalette4_rgba4,
			TPalette4_rgb5a1,
			TPalette8_rgb8,
			TPalette8_rgba8,
			TPalette8_r5g6b5,
			TPalette8_rgba4,
			TPalette8_rgb5a1,
			
			TPVRTC4_rgb,
			TPVRTC2_rgb,
			TPVRTC4_rgba,
			TPVRTC2_rgba,
            
            TRenderTarget,

			TCount,
			TPad = 0xFFFFFFFF
		};
	};

	struct TTypeFlag
	{
		enum TEnum
		{
			TNone					= 0x0,

			TTypeMask				= 0x000000FF,

			TMagnifyFilterNearest	= 0x00000100,
			TWrapModeClamp			= 0x00000200,
			TMipmapMagnifyFilterNearest	= 0x00000400,
			TMipmapMinifyFilterNearest	= 0x00000800,
			TMipmapMinifyFilterNearestMipmapNearest	= 0x00001000,
			TMipmapMinifyFilterNearestMipmapLinear	= 0x00002000,
			TMipmapMinifyFilterLinearMipmapLinear	= 0x00004000,
			TMipmapMinifyFilterLinearMipmapNearest	= 0x00008000,
		};
	};

	struct TMagnifyFilter
	{
		enum TEnum
		{
			//NB, default as zero
			TLinear = 0,
			TNearest,
			TCount
		};
	};

	struct TWrapMode
	{
		enum TEnum
		{
			//NB, default as zero
			TRepeat = 0,
			TClamp, //CLAMP_TO_EDGE
			//TMirroredRepeat,  MIRRORED_REPEAT  opengl es only?

			TCount
		};
	};

	struct TMipmapMagnifyFilter
	{
		enum TEnum
		{
			//NB, default as zero			
			TLinear = 0,
			TNearest,
			TCount
		};
	};

	struct TMipmapMinifyFilter
	{
		enum TEnum
		{
			//NB, default as zero
			TLinear = 0,
			TNearest,
			TNearestMipmapNearest,
			TNearestMipmapLinear,
			TLinearMipmapNearest,
			TLinearMipmapLinear,

			TCount
		};
	};

	/////////////////////////////////////////////////////////////////
	//static public
public:
	static const GU32 MakeFlag(
		const TType::TEnum in_type,
		const TMagnifyFilter::TEnum in_magnifyFilter = GTextureType::TMagnifyFilter::TLinear,
		const TWrapMode::TEnum in_wrapMode = GTextureType::TWrapMode::TRepeat,
		const TMipmapMagnifyFilter::TEnum in_mipmapMagnifyFilter = GTextureType::TMipmapMagnifyFilter::TLinear,
		const TMipmapMinifyFilter::TEnum in_mipmapMinifyFilter = GTextureType::TMipmapMinifyFilter::TNearestMipmapLinear		
		);
	static GVOID UnMakeFlag(
		TType::TEnum& out_type,
		TMagnifyFilter::TEnum& out_magnifyFilter,
		TWrapMode::TEnum& out_wrapMode,
		TMipmapMagnifyFilter::TEnum& out_mipmapMagnifyFilter,
		TMipmapMinifyFilter::TEnum& out_mipmapMinifyFilter,
		const GU32 in_flag
		);
	static const TType::TEnum GetTypeFromFlag(const GU32 in_flag);
	static const TMagnifyFilter::TEnum GetMagnifyFilterFromFlag(const GU32 in_flag);
	static const TWrapMode::TEnum GetWrapModeFromFlag(const GU32 in_flag);
	static const TMipmapMagnifyFilter::TEnum GetMipmapMagnifyFilterFromFlag(const GU32 in_flag);
	static const TMipmapMinifyFilter::TEnum GetMipmapMinifyFilterFromFlag(const GU32 in_flag);

	/////////////////////////////////////////////////////////////////
	//disabled
private:
	~GTextureType();

};

#endif //_gtexturetype_h_