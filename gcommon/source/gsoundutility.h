//
//  GSoundUtility.h
//

#ifndef _GSoundUtility_h_
#define _GSoundUtility_h_

#include <boost/noncopyable.hpp>
#include "GCommon.h"

#ifdef DSC_DEBUG
	#define AL_ERROR_CHECK() GSoundUtility::ErrorCheck()
#else
	#define AL_ERROR_CHECK()
#endif


/**/
class GSoundUtility : public boost::noncopyable 
{
#ifdef DSC_DEBUG
public:
	static GVOID ErrorCheck();
#endif
};

#endif 