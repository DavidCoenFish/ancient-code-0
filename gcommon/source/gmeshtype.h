//
//  gmeshtype.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#ifndef _GMeshType_h_
#define _GMeshType_h_

#include "gcommon.h"

struct GMeshType
{
	//typedef
public:
	//WARNING: enums included by pipeline, changes will require rebuild of pipeline and assets
	struct TPrimitiveType
	{
		enum TEnum
		{
			//WARN: magic numbers from opengl
			TPoint = 0,
			TLine = 1,
			TLineStrip = 3,
			TLineLoop = 2,
			TTriangle = 4,
			TTriangleStrip = 5,
			TTriangleFan = 6
		};
	};

	//WARNING: enums included by pipeline, changes will require rebuild of pipeline and assets
	struct TStreamType
	{
		enum TEnum
		{
			//WARN: magic numbers from opengl
			TByte				= 0x1400,
			TUnsignedByte		= 0x1401,
			TShort				= 0x1402,
			TUnsignedShort		= 0x1403,
			TInt				= 0x1404,
			TUnsignedInt		= 0x1405,
			TFloat				= 0x1406,
			//TByte2 = 0x1407,
			//TByte3 = 0x1408,
			//TByte4 = 0x1409
		};
	};

	//WARNING: enums included by pipeline, changes will require rebuild of pipeline and assets
	//for Opengles 1, which client state to enable for the data. better if called stream usage?
	struct TStreamUsage //ClientState
	{
		enum TEnum
		{
			TNone = 0,
			TPosition, //opengles1 client state Vertex
			TNormal, //opengles1 client state Normal
			TColour, //opengles1 client state Color
			TUv0, //opengles1 client state TextureCoord
			TUv1,
			TTangent0,
			TTangent1,
			TData,

			TCount
		};
	};

	//WARNING: enums included by pipeline, changes will require rebuild of pipeline and assets
	struct TFlag
	{
		enum TEnum
		{
			TNone		= 0x00,
			TIndexU8	= 0x01,
			TIndexU16	= 0x02,

			TGenerateEdge = 0x04,

			TIndexMask	= TIndexU8 | TIndexU16

		};
	};

	//static public methods
public:
	static const GS32 GetSize(const TStreamType::TEnum in_type);

	//disabled
private:
	~GMeshType();

};


#endif 