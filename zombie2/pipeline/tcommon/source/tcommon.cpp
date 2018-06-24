//file: TCommon.cpp

#include "TCommon.h"
#include <stdio.h>

// static public methods
/*static*/ const int TCommon::StringToFlag(const TArrayPairStringInt& in_arrayStringFlag, const std::string& in_flag)
{
	for (TArrayPairStringInt::const_iterator iterator = in_arrayStringFlag.begin(); iterator != in_arrayStringFlag.end(); ++iterator)
	{
		if ((*iterator).first == in_flag)
		{
			return (*iterator).second;
		}
	}
	return 0;
}

/*static*/ const std::string TCommon::IntToString(const int in_value)
{
	char text[256];
	text[0] = 0;
	sprintf_s(&text[0], 256, "%d", in_value);
	return &text[0];
}
