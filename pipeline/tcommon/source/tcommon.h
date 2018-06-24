//file: TCommon.h
#ifndef _TCommon_H_
#define _TCommon_H_

#include <vector>
#include <string>

typedef float R32;
typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned int U32;

#define TCOMMON_ARRAY_SIZE( ARRAY ) ( sizeof( ARRAY ) / sizeof( ARRAY[ 0 ] ) )

/**/
struct TCommon
{
	//typedef
private:
	typedef std::pair<std::string, int> TPairStringInt;
	typedef std::vector<TPairStringInt> TArrayPairStringInt;

	// disabled
private:
	~TCommon();

	// static public methods
public:
	static const int StringToFlag(const TArrayPairStringInt& in_arrayStringFlag, const std::string& in_flag);

	static const std::string IntToString(const int in_value);


};

#endif // _TCommon_H_