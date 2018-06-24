//
//  GAnimationType.cpp
//
//  Created by David Coen on 2011 01 05
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GAnimationType.h"

#include "GVector3Float.h"

//static public methods
/*static*/ const int GAnimationType::GetStreamTypeStride(const TStreamType::TEnum in_streamType)
{
	switch (in_streamType)
	{
	default:
		break;
	case TStreamType::TPosition:
		return sizeof(GVector3Float);
	case TStreamType::TRotation:
		return (sizeof(GVector3Float) * 2);
	case TStreamType::TScale:
		return sizeof(float);
	case TStreamType::TMorphWeight:
		return sizeof(float);
	}
	return 0;
}

