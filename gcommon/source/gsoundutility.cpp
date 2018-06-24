//
//  GSoundManager.cpp
//

#include "GSoundUtility.h"

#include "gassert.h"

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#ifdef DSC_DEBUG
/*static*/ GVOID GSoundUtility::ErrorCheck()
{
//#define AL_INVALID_NAME                           0xA001  40961

	ALenum error = AL_NO_ERROR;
	do
	{
		error = alGetError();
		if (error != AL_NO_ERROR)
		{
			GASSERT(0, "open al error");
			//assert(0); //"open al error");
			break;
		}
	}
	while (error != AL_NO_ERROR);

	return;
}
#endif
