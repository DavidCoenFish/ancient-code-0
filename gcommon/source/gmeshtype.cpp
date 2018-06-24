//
//  gmeshtype.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "GMeshType.h"

//static public methods
/*static*/ const GS32 GMeshType::GetSize(const TStreamType::TEnum in_type)
{
	switch (in_type)
	{
	default:
		break;
	case TStreamType::TByte:
	case TStreamType::TUnsignedByte:
		return sizeof(char);
	case TStreamType::TInt:
	case TStreamType::TUnsignedInt:
		return sizeof(int);
	case TStreamType::TFloat:
		return sizeof(float);
	//case TStreamType::TByte2:
	//	return 2 * sizeof(char);
	//case TStreamType::TByte3:
	//	return 3 * sizeof(char);
	//case TStreamType::TByte4:
	//	return 4 * sizeof(char);
	}
	return 0;
}
