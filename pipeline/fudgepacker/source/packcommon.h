//file: packCommon.h
#ifndef _packCommon_H_
#define _packCommon_H_

#include <string>
#include <vector>
#include "main.h"

class TiXmlElement;
/**/
struct PackCommon
{
	// typedefs
private:
	typedef std::vector<unsigned char> TArrayByte;

	// disabled
private:
	~PackCommon();

	// static public methods
public:
	template <typename IN_TYPE>
	static void Append(TArrayByte& out_arrayByte, const IN_TYPE in_value)
	{
		const unsigned char* const pData = (const unsigned char* const)&in_value;
		for (int index = 0; index < sizeof(IN_TYPE); ++index)
		{
			out_arrayByte.push_back(pData[index]);
		}
		return;
	}

	template <typename IN_TYPE>
	static void Replace(TArrayByte& out_arrayByte, const IN_TYPE in_value, const int in_offset)
	{
		const unsigned char* const pData = (const unsigned char* const)&in_value;
		for (int index = 0; index < sizeof(IN_TYPE); ++index)
		{
			out_arrayByte[in_offset + index] = (pData[index]);
		}
		return;
	}

	static void Pad(
		TArrayByte& inout_arrayByte,
		const int in_alignment
		);

	static void StringToArray(
		TArrayByte& out_arrayByte,
		const std::string& in_string
		);

};

#endif // _packCommon_H_