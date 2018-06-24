//
//  gapplicationwindowtype.h
//  GCommon
//
//  Created by David Coen on 2011 05 30
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//
#ifndef _gapplicationwindowtype_H_
#define _gapplicationwindowtype_H_

#include "gcommon.h"

//interface class for application tasks
class GApplicationWindowType
{
	///////////////////////////////////////////////////////
	//typedef
public:
	struct TOrientation
	{
		enum TEnum
		{
			TTwelveOclock = 0,
			TThreeOclock,
			TSixOclock,
			TNineOclock,

			TFaceDown,
			TFaceUp,

			TCount
		};
	};

	///////////////////////////////////////////////////////
	//disabled
private:
	~GApplicationWindowType();

};

#endif //#ifndef _gapplicationwindowtype_H_
