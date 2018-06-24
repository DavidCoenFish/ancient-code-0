//
//  gassert.cpp
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "gassert.h"
#include <boost/assert.hpp>

/*static*/ void GAssert::Run(const bool in_condition, const GCHAR* const in_message)
{
	if (!in_condition)
	{
		GU32 foo = 0; 
		foo += 1;//some dummy expression for adding breakpoint

		BOOST_ASSERT(in_condition);
	}
	return;
}
