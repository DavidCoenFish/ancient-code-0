//
//  GAnimationType.h
//
//  Created by David Coen on 2011 01 05
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GAnimationType_h_
#define _GAnimationType_h_

#include <string>

class GAnimationType
{
	//typedef
public:
	struct TStreamType
	{
		enum TEnum
		{
			TPosition = 0, //vec3
			TRotation, //vec3[2]
			TScale, //float
			TNoteTrack, //string
			TMorphWeight, //float

			TCount
		};
	};

	struct TAnimationFlag
	{
		enum TEnum
		{
			TNone		= 0x00,
			TLoop		= 0x01
		};
	};

	//static public methods
public:
	static const int GetStreamTypeStride(const TStreamType::TEnum in_streamType);

	//disabled
private:
	~GAnimationType();

};

#endif 