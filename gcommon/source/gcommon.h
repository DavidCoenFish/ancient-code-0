//
//  gcommon.h
//  GCommon
//
//  Created by David Coen on 2011 05 29
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//
#ifndef _gcommon_H_
#define _gcommon_H_

///////////////////////////////////////////////////////
//typedef
typedef void			GVOID;	//excessive, not really a type?
typedef bool			GBOOL;
typedef char			GCHAR;
typedef unsigned char	GU8;
typedef signed char		GS8;
typedef unsigned short	GU16;
typedef signed short	GS16;
typedef unsigned int	GU32;
typedef signed int		GS32;
typedef int				GINT;	//threre are time you just want the native int
typedef float			GR32;
typedef double			GR64;
typedef float			GFLOAT; //threre are time you just want the native floating point and don't care about matching memory byte count with file contents

///////////////////////////////////////////////////////
//defines
#ifndef GTRUE
	#define GTRUE true
#endif
#ifndef GFALSE
	#define GFALSE false
#endif
#ifndef GNULL
	#define GNULL 0
#endif

#define GCOMMON_INVALID_INDEX -1
//translate input token into a string
#define GCOMMON_TOKEN(INPUT) #INPUT
#define GCOMMON_TOKEN_TOKEN(INPUT_ONE, INPUT_TWO) #INPUT_ONE#INPUT_TWO
#define GCOMMON_TOKEN_TOKEN_TOKEN(INPUT_ONE, INPUT_TWO, INPUT_THREE) (#INPUT_ONE#INPUT_TWO#INPUT_THREE)

#define GCOMMON_UNUSED_VAR(VAR)

//common way of changing index to flag, no negative index
#define GCOMMON_INDEX_TO_FLAG(INDEX)							(1 << (INDEX))

#define GCOMMON_ARRAY_SIZE(ARRAY) ( sizeof(ARRAY) / sizeof(ARRAY[0]))

GVOID GCommonLog(const GCHAR* const in_message);
#define GCOMMON_LOG(IN_MESSAGE) GCommonLog(IN_MESSAGE);


#endif //#ifndef _gcommon_H_
