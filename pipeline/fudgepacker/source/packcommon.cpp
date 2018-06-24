//file: PackCommon.cpp

#include "PackCommon.h"

/*static*/ void PackCommon::Pad(
	TArrayByte& inout_arrayByte,
	const int in_alignment
	)
{
	if ((0 == in_alignment) || (1 == in_alignment))
	{
		return;
	}

	const int remainder = ((int)inout_arrayByte.size() % in_alignment);
	if (0 == remainder)
	{
		return;
	}

	for (int index = 0; index < (in_alignment - remainder); ++index)
	{
		inout_arrayByte.push_back(0);
	}

	return;
}

/*static*/ void PackCommon::StringToArray(
	TArrayByte& out_arrayByte,
	const std::string& in_string
	)
{
	for (int index = 0; index < (int)in_string.length(); ++index)
	{
		out_arrayByte.push_back(in_string[index]);
	}
	out_arrayByte.push_back(0);

	return;
}
