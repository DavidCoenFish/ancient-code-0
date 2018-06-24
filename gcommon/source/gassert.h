//
//  gassert.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#ifndef _gassert_H_
#define _gassert_H_

#include "gcommon.h"

class GAssert
{
private:
	~GAssert();
public:
	static void Run(const GBOOL in_condition, const GCHAR* const in_message);
};

#ifdef DSC_DEBUG
	//#define GASSERT(CONDITION, HMESSAGE) BOOST_ASSERT(CONDITION);
	#define GASSERT(CONDITION, HMESSAGE) GAssert::Run(CONDITION, HMESSAGE);
#else
	#define GASSERT(CONDITION, HMESSAGE)
#endif

#define GASSERT_STATIC(expr, msg) \
{ GCompileTimeError< ( ( expr ) != 0 ) >STATIC_ASSERT_##msg; ( void )STATIC_ASSERT_##msg; } 

template< GBOOL >struct GCompileTimeError;
template<>struct GCompileTimeError< GTRUE > {};

/**/

#endif// _gassert_H_
