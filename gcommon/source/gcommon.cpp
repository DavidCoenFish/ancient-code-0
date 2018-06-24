//
//  gcommon.cpp
//  GCommon
//
//  Created by David Coen on 2011 05 29
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "gcommon.h"

#ifdef WIN32
	#include <Windows.h>
#endif

GVOID GCommonLog(const GCHAR* const in_message)
{
#ifdef WIN32
	OutputDebugStringA(in_message);
#endif
}
