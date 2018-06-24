//
//  gmaterialtype.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#ifndef _gmaterialtype_h_
#define _gmaterialtype_h_
#include "gcommon.h"
/**/
class GMaterialType
{
	//////////////////////////////////////////////
	//typedef
public:
    struct TBlendMode
	{
		enum TEnum
		{
			//WARN: magic numbers from opengl
			TZero				= 0x0000,
			TOne                = 0x0001,
			TSrcColor			= 0x0300,
			TOneMinusSrcColor	= 0x0301,
			TSrcAlpha			= 0x0302,
			TOneMinusSrcAlpha	= 0x0303,
			TDstAlpha			= 0x0304,
			TOneMinusDstAlpha	= 0x0305,
			TDstColor			= 0x0306,
			TOneMinusDstColor	= 0x0307,
			TSrcAlphaSaturate	= 0x0308
		};
	};

	//reflected in tools (materialPack.exe)
	struct TState
	{
		enum TFlag
		{
			TNone				= 0x0000,

			TFaceCW				= 0x0001, //(lightwave is clockwise) meshload pipeline is CW
			TFaceCull			= 0x0002,

			TAlphaBlend			= 0x0004,

			TColourWrite		= 0x0008,

			TDepthRead			= 0x0010,
			//TDepthReadAlway
			TDepthWrite			= 0x0040,

			TStencilShadowFront	= 0x0100,
			TStencilShadowBack	= 0x0200,
			TStencilShadow		= 0x0400,
			////TStencilReadAlway
			//TStencilWriteIncDepth	= 0x0400,
			//TStencilWriteZero	= 0x0800,

			TUseLight			= 0x1000,
			TUseTexture			= 0x2000,
			TDoBasePass		    = 0x4000, //hack: help out opengles1 to let it know what shader ops to mimic
			TDoEnvironment      = 0x8000, //hack: help out opengles1 to let it know what shader ops to mimic

			TPadFlagSize		= 0xFFFFFFFF

		};
	};

	/////////////////////////////////////////////////////////////////
	//disabled
private:
	~GMaterialType();

};

#endif //_gmaterialtype_h_